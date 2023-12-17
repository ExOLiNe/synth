//
// Created by Aleksandr on 26-Feb-23.
//

#include "SynthVoice.h"
#include "SynthSound.h"
#include <juce_gui_extra.h>
#include <cmath>

#ifdef PROFILING_ENABLED
#include <Tracy.hpp>
#endif

#define ASSERT_NAN(value) jassert(!::isnan(value))

namespace audio {
    double fineFactor = std::pow(4.0, 1.0 / params::osc::fine.maxValue);

    SynthVoice::SynthVoice(const juce::AudioProcessorValueTreeState& apvts, const juce::String id, const size_t voiceId):
            id(id), voiceId(voiceId),
            oscParams(apvts, id),
            volumeAdsrParams(apvts, params::volumeADSRName),
            adsr1Params(apvts, adsr1Id),
            adsr2Params(apvts, adsr2Id),
            adsr1Amps(apvts, id, adsr1Id),
            adsr2Amps(apvts, id, adsr2Id),
            lfo1Amps(apvts, id, lfo1Id),
            lfo2Amps(apvts, id, lfo2Id),
            lfo1Atomic(apvts.getRawParameterValue(lfo1Id)),
            lfo2Atomic(apvts.getRawParameterValue(lfo2Id)),
            currentWaveTableIndex((int)oscParams.waveTableIndex->load()),
            waveTables(WaveTables::getInstance()->copyWaveTables()),
            filterParams(apvts, id)
                       {
                           filter.setMode(juce::dsp::LadderFilterMode::LPF24);
    }

    bool SynthVoice::canPlaySound(juce::SynthesiserSound* sound) {
        return dynamic_cast<SynthSound*>(sound) != nullptr;
    }

    void SynthVoice::startNote(int midiNoteNumber, float, juce::SynthesiserSound *,
                               int) {
        midiNote = midiNoteNumber;
        frequency = juce::MidiMessage::getMidiNoteInHertz(midiNote + (int)oscParams.semitone->load());
        for (auto & table : waveTables) {
            table.resetPhaseOffset();
        }
        volumeADSR.noteOn();
        ADSR1.noteOn();
        ADSR2.noteOn();
    }

    void SynthVoice::stopNote(float, bool) {
        volumeADSR.noteOff();
        ADSR1.noteOff();
        ADSR2.noteOff();
    }

    void SynthVoice::pitchWheelMoved(int) {

    }

    void SynthVoice::controllerMoved(int, int) {

    }

    void SynthVoice::prepareToPlay(double sampleRate, int, int) {
        juce::ADSR::Parameters volumeADSRParams { 0.f, 0.f, 0.f, 0.f};
        volumeADSR.setSampleRate(sampleRate);
        volumeADSR.setParameters(volumeADSRParams);
        volumeADSR.reset();

        juce::ADSR::Parameters ADSR1Params { 0.f, 0.f, 0.f, 0.f};
        ADSR1.setSampleRate(sampleRate);
        ADSR1.setParameters(ADSR1Params);
        ADSR1.reset();

        juce::ADSR::Parameters ADSR2Params { 0.f, 0.f, 0.f, 0.f};
        ADSR2.setSampleRate(sampleRate);
        ADSR2.setParameters(ADSR2Params);
        ADSR2.reset();
    }

    void SynthVoice::updateFilter() {
        /*auto adsr1 = ADSR1Buffer.getReadPointer(0);
        auto adsr2 = ADSR2Buffer.getReadPointer(0);*/
        auto filterFreq = filterParams.filterFreq->load();
        auto finalFreq = filterFreq
                + filterFreq * lfo1Amps.filterFreqAmp->load() * lfo1Values.current
                + filterFreq * lfo2Amps.filterFreqAmp->load() * lfo2Values.current;
                /*+ filterFreq * adsr1Amps.filterFreqAmp->load() * adsr1[0]
                + filterFreq * adsr2Amps.filterFreqAmp->load() * adsr2[0];*/
        filter.setCutoffFrequencyHz(finalFreq);
        filter.setResonance(filterParams.filterReso->load() / 100.f);
        filter.setDrive(1.f);
    }

    void SynthVoice::renderNextBlock(juce::AudioBuffer<float> &outputBuffer, int startSample, int numSamples) {
#ifdef PROFILING_ENABLED
        ZoneScoped;
#endif
        if (oscParams.gain->load() == 0.0f || frequency == 0.0) {
            return;
        }

        currentVoiceBuffer.clear();
        if (currentVoiceBuffer.getNumSamples() < numSamples) {
            currentVoiceBuffer.setSize(outputBuffer.getNumChannels(), numSamples); // TODO prevent realloc
            ADSR1Buffer.setSize(outputBuffer.getNumChannels(), numSamples);
            ADSR2Buffer.setSize(outputBuffer.getNumChannels(), numSamples);
        }

        auto ADSR1Ptr = ADSR1Buffer.getWritePointer(0);
        auto ADSR2Ptr = ADSR2Buffer.getWritePointer(0);
        for (int i = 0; i < numSamples; ++i) {
            ADSR1Ptr[i] = 1.f;
            ADSR2Ptr[i] = 1.f;
        }

        ADSR1.applyEnvelopeToBuffer(ADSR1Buffer, 0, numSamples);
        ADSR2.applyEnvelopeToBuffer(ADSR2Buffer, 0, numSamples);

        updatePreviousValues(
            fineValues, phaseValues, detuneValues, gainValues, panValues,
            lfo1Values, lfo2Values
        );

        lfo1Values.current = lfo1Atomic->load();
        lfo2Values.current = lfo2Atomic->load();

        updateADSR();
        updateSemitone();
        updateWaveTableIndex();
        updateFilter();

        auto voicePtrL = currentVoiceBuffer.getWritePointer(Channel::LEFT);
        auto voicePtrR = currentVoiceBuffer.getWritePointer(Channel::RIGHT);

        WaveTable& currentWaveTable = waveTables[currentWaveTableIndex];

        fineValues.current = getFine();
        phaseValues.current = oscParams.phase->load();
        detuneValues.current = oscParams.detune->load();
        gainValues.current = oscParams.gain->load();
        panValues.current = oscParams.pan->load();
        const int voices = (int)oscParams.voices->load();


        auto sampleRate = getSampleRate();

        for (int i = 0; i < numSamples; ++i) {
#ifdef PROFILING_ENABLED
            ZoneNamed(sample_handle, true);
#endif

            auto modulators = ModulatorCalculatedValues {
                .lfo1 = getSmoothValue(lfo1Values, numSamples, i),
                .lfo2 = getSmoothValue(lfo2Values, numSamples, i),
                .adsr1 = ADSR1Ptr[i],
                .adsr2 = ADSR2Ptr[i]
            };

            auto morphingWave = getMorphingWave(getFloatWaveTablePos(currentWaveTable, modulators));

            const double finalFrequency = getFrequency(i, numSamples, modulators);

            jassert(finalFrequency != 0);

            ASSERT_NAN(finalFrequency);

            currentWaveTable.shiftPhase();

            const float phaseOffset = getSmoothValue(phaseValues, numSamples, i)
                    + static_cast<float>(sampleRate / finalFrequency * lfo1Amps.phaseAmp->load());

            ASSERT_NAN(getSmoothValue(phaseValues, numSamples, i));

            ASSERT_NAN(phaseOffset);

            const float detune = getSmoothValue(detuneValues, numSamples, i);

            float output = 0.f;
            for (int voiceIndex = 0; voiceIndex < voices; ++voiceIndex) {
                const double detuneFactor = std::pow(std::exp(std::log(2) / 1200), detune * ((float)voices / 2 - (float)voiceIndex));

                // Morphing between the nearest waves
                const float upperOutput = currentWaveTable.generateSample(
                        finalFrequency * detuneFactor, getSampleRate(), morphingWave.top.index, phaseOffset
                );
                ASSERT_NAN(upperOutput);
                const float lowerOutput = currentWaveTable.generateSample(
                        finalFrequency * detuneFactor, getSampleRate(), morphingWave.bottom.index, phaseOffset
                );
                ASSERT_NAN(lowerOutput);
                output += upperOutput * morphingWave.top.volume + lowerOutput * morphingWave.bottom.volume;
                ASSERT_NAN(output);
            }
            output /= (float)voices;

            ASSERT_NAN(output);

            // gain & pan
            output *= 0.4f * (getSmoothValue(gainValues, numSamples, i) / 100.0f);
            if (lfo1Amps.gainAmp->load() > 0.0001f) {
                output *= (1.f + lfo1Amps.gainAmp->load()) * modulators.lfo1;
            }
            if (lfo2Amps.gainAmp->load() > 0.0001f) {
                output *= (1.f + lfo2Amps.gainAmp->load()) * modulators.lfo2;
            }
            const float pan = getSmoothValue(panValues, numSamples, i) +
                    lfo1Amps.panAmp->load() * modulators.lfo1;
            voicePtrL[i] += output * getPanGain(Channel::LEFT, pan);
            voicePtrR[i] += output * getPanGain(Channel::RIGHT, pan);
        }

        juce::dsp::AudioBlock<float> currentVoiceBlock(currentVoiceBuffer);

        auto contextReplacing = juce::dsp::ProcessContextReplacing<float> { currentVoiceBlock };

        filter.process(contextReplacing);

        volumeADSR.applyEnvelopeToBuffer(currentVoiceBuffer, 0, numSamples);

        auto outputPtrL = outputBuffer.getWritePointer(Channel::LEFT);
        auto outputPtrR = outputBuffer.getWritePointer(Channel::RIGHT);

        for (int i = 0; i < numSamples; ++i) {
            outputPtrL[i + startSample] += voicePtrL[i];
            outputPtrR[i + startSample] += voicePtrR[i];
        }

#ifdef DEBUG_BUFFERS
        outputPtrL[numSamples - 1] = -2.f * gainValues.current;
        outputPtrR[numSamples - 1] = -2.f * gainValues.current;
#endif
    }

    float SynthVoice::getPanGain(const Channel& channel, float pan) {
        float direction = channel == Channel::LEFT ? -1.0f : 1.0f;
        return (direction * pan + 1) / 2.0f;
    }

    void SynthVoice::updateADSR() {
        juce::ADSR::Parameters ADSR1Params;
        ADSR1Params.attack = adsr1Params.attack->load() * params::adsr::attackFactor;
        ADSR1Params.decay = adsr1Params.decay->load() * params::adsr::decayFactor;
        ADSR1Params.sustain = adsr1Params.sustain->load() * params::adsr::sustainFactor;
        ADSR1Params.release = adsr1Params.release->load() * params::adsr::releaseFactor;
        ADSR1.setParameters(ADSR1Params);

        juce::ADSR::Parameters ADSR2Params;
        ADSR2Params.attack = adsr2Params.attack->load() * params::adsr::attackFactor;
        ADSR2Params.decay = adsr2Params.decay->load() * params::adsr::decayFactor;
        ADSR2Params.sustain = adsr2Params.sustain->load() * params::adsr::sustainFactor;
        ADSR2Params.release = adsr2Params.release->load() * params::adsr::releaseFactor;
        ADSR2.setParameters(ADSR2Params);

        juce::ADSR::Parameters volumeADSRParams;
        volumeADSRParams.attack = volumeAdsrParams.attack->load() * params::adsr::attackFactor;
        volumeADSRParams.decay = volumeAdsrParams.decay->load() * params::adsr::decayFactor;
        volumeADSRParams.sustain = volumeAdsrParams.sustain->load() * params::adsr::sustainFactor;
        volumeADSRParams.release = volumeAdsrParams.release->load() * params::adsr::releaseFactor;
        volumeADSR.setParameters(volumeADSRParams);
    }

    void SynthVoice::updateSemitone() {
        if (previousSemitoneOffset != (int)oscParams.semitone->load()) {
            frequency = juce::MidiMessage::getMidiNoteInHertz(midiNote + (int)oscParams.semitone->load());
            previousSemitoneOffset = (int)oscParams.semitone->load();
        }
    }

    void SynthVoice::updateWaveTableIndex() {
        currentWaveTableIndex = (int)oscParams.waveTableIndex->load() - 1;
        if (currentWaveTableIndex == -1) currentWaveTableIndex = 0;
    }

    float SynthVoice::getFloatWaveTablePos(const WaveTable& waveTable, const ModulatorCalculatedValues& modulators) const {
#ifdef PROFILING_ENABLED
        ZoneScoped;
#endif
        auto maxPossibleIndex = static_cast<float>(waveTable.waveTable.size() - 1);
        auto lfo1 = modulators.lfo1 * lfo1Amps.wtPos->load() * maxPossibleIndex;
        auto lfo2 = modulators.lfo2 * lfo2Amps.wtPos->load() * maxPossibleIndex;
        auto adsr1 = modulators.adsr1 * adsr1Amps.wtPos->load() * maxPossibleIndex;
        auto adsr2 = modulators.adsr2 * adsr2Amps.wtPos->load() * maxPossibleIndex;
        auto waveTablePos =  oscParams.waveTablePos->load() / 100 * maxPossibleIndex +
            lfo1 + lfo2 + adsr1 + adsr2;
        if (waveTablePos > maxPossibleIndex) {
            waveTablePos = maxPossibleIndex;
        } else if (waveTablePos < 0) {
            waveTablePos = 0.f;
        }
        return waveTablePos;
    }

    MorphingWave SynthVoice::getMorphingWave(float waveTablePos) {
        auto upperWaveIndex = static_cast<size_t>(std::ceil(waveTablePos));
        auto lowerWaveIndex = static_cast<size_t>(std::floor(waveTablePos));

        // Calculate gain factors of the nearest waves
        const float upperWaveGainFactor = waveTablePos - static_cast<float>(lowerWaveIndex);
        const float lowerWaveGainFactor = 1.0f - upperWaveGainFactor;
        return MorphingWave {
            .bottom { .index = lowerWaveIndex, .volume = lowerWaveGainFactor },
            .top { .index = upperWaveIndex, .volume = upperWaveGainFactor }
        };
    }

    float SynthVoice::getFine() const {
        return (float)std::pow(fineFactor, oscParams.fine->load());
    }

    double SynthVoice::getFrequency(int i, int numSamples, const ModulatorCalculatedValues& modulators) {
        auto freqLFO1Offset = frequency * lfo1Amps.fineAmp->load() * modulators.lfo1;
        auto freqLFO2Offset = frequency * lfo2Amps.fineAmp->load() * modulators.lfo2;
        auto freqADSR1Offset = frequency * modulators.adsr1 * adsr1Amps.fineAmp->load();
        auto freqADSR2Offset = frequency * modulators.adsr2 * adsr2Amps.fineAmp->load();
        ASSERT_NAN(getSmoothValue(fineValues, numSamples, i));
        ASSERT_NAN(freqLFO1Offset);
        ASSERT_NAN(freqLFO2Offset);
        ASSERT_NAN(freqADSR1Offset);
        ASSERT_NAN(freqADSR2Offset);
        return frequency * getSmoothValue(fineValues, numSamples, i)
                                      + freqLFO1Offset + freqLFO2Offset + freqADSR1Offset + freqADSR2Offset;
    }
}