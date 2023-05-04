//
// Created by Aleksandr on 26-Feb-23.
//

#include "SynthVoice.h"
#include "SynthSound.h"
#include <juce_gui_extra.h>
#include <cmath>
#include <Tracy.hpp>

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
            lfo1(apvts.getRawParameterValue(lfo1Id)),
            lfo2(apvts.getRawParameterValue(lfo2Id)),
            currentWaveTableIndex((int)oscParams.waveTableIndex->load()),
            waveTables(WaveTables::getInstance()->copyWaveTables())
                       {

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

    void SynthVoice::renderNextBlock(juce::AudioBuffer<float> &outputBuffer, int startSample, int numSamples) {
        ZoneScoped;
        if (oscParams.gain->load() == 0.0f) {
            return;
        }

        currentVoiceBuffer.clear();
        if (currentVoiceBuffer.getNumSamples() < numSamples) {
            currentVoiceBuffer.setSize(outputBuffer.getNumChannels(), numSamples);
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

        updateADSR();
        updateSemitone();
        updateWaveTableIndex();

        auto voicePtrL = currentVoiceBuffer.getWritePointer(Channel::LEFT);
        auto voicePtrR = currentVoiceBuffer.getWritePointer(Channel::RIGHT);

        WaveTable& currentWaveTable = waveTables[currentWaveTableIndex];

        const float floatWaveTablePos = getFloatWaveTablePos(currentWaveTable);
        const int upperWaveIndex = (int)std::ceil(floatWaveTablePos);
        const int lowerWaveIndex = (int)std::floor(floatWaveTablePos);

        // Calculate gain factors of the nearest waves
        const float upperWaveGainFactor = floatWaveTablePos - (float)lowerWaveIndex;
        const float lowerWaveGainFactor = 1.0f - upperWaveGainFactor;

        fineValues.current = getFine();
        phaseValues.current = oscParams.phase->load();
        detuneValues.current = oscParams.detune->load();
        gainValues.current = oscParams.gain->load();
        panValues.current = oscParams.pan->load();
        const int voices = (int)oscParams.voices->load();

        lfo1Values.current = lfo1->load();
        lfo2Values.current = lfo2->load();

        auto sampleRate = getSampleRate();

        auto lfo1FineAmpValue = lfo1Amps.fineAmp->load();
        auto lfo2FineAmpValue = lfo2Amps.fineAmp->load();

        for (int i = 0; i < numSamples; ++i) {
            ZoneNamed(sample_handle, true);
            auto freqLFO1Offset = frequency * lfo1FineAmpValue * getSmoothValue(lfo1Values, numSamples, i);
            auto freqLFO2Offset = frequency * lfo2FineAmpValue * getSmoothValue(lfo2Values, numSamples, i);
            auto freqADSR1Offset = frequency * ADSR1Ptr[i] * adsr1Amps.fineAmp->load();
            auto freqADSR2Offset = frequency * ADSR2Ptr[i] * adsr2Amps.fineAmp->load();
            const double finalFrequency = frequency * getSmoothValue(fineValues, numSamples, i)
                                          + freqLFO1Offset + freqLFO2Offset + freqADSR1Offset + freqADSR2Offset;
            currentWaveTable.shiftPhase();
            const float phaseOffset = getSmoothValue(phaseValues, numSamples, i)
                    + sampleRate / finalFrequency * lfo1Amps.phaseAmp->load();
            const float detune = getSmoothValue(detuneValues, numSamples, i);
            float output = 0.f;
            for (int voiceIndex = 0; voiceIndex < voices; ++voiceIndex) {
                const double detuneFactor = std::pow(std::exp(std::log(2) / 1200), detune * ((float)voices / 2 - (float)voiceIndex));

                // Morphing between the nearest waves
                const float upperOutput = currentWaveTable.generateSample(
                        finalFrequency * detuneFactor, getSampleRate(), upperWaveIndex, phaseOffset
                );
                const float lowerOutput = currentWaveTable.generateSample(
                        finalFrequency * detuneFactor, getSampleRate(), lowerWaveIndex, phaseOffset
                );
                output += upperOutput * upperWaveGainFactor + lowerOutput * lowerWaveGainFactor;
            }
            output /= (float)voices;

            // gain & pan
            output *= 0.4f * (getSmoothValue(gainValues, numSamples, i) / 100.0f);
            if (lfo1Amps.gainAmp->load() > 0.0001f) {
                output *= (1.f + lfo1Amps.gainAmp->load()) * getSmoothValue(lfo1Values, numSamples, i);
            }
            if (lfo2Amps.gainAmp->load() > 0.0001f) {
                output *= (1.f + lfo2Amps.gainAmp->load()) * getSmoothValue(lfo2Values, numSamples, i);
            }
            /*if (id == "osc1" && voiceId == 0) {
                logger.log(lfo1Values.current);
            }*/
            const float pan = getSmoothValue(panValues, numSamples, i) +
                    lfo1Amps.panAmp->load() * getSmoothValue(lfo1Values, numSamples, i);
            voicePtrL[i] += output * getPanGain(Channel::LEFT, pan);
            voicePtrR[i] += output * getPanGain(Channel::RIGHT, pan);
        }

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

    float SynthVoice::getFloatWaveTablePos(const WaveTable& waveTable) const {
        ZoneScoped;
        return oscParams.waveTablePos->load() / 100 * (waveTable.waveTable.size() - 1);
    }

    float SynthVoice::getPanGain(const Channel& channel, float pan) const {
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

    float SynthVoice::getFine() const {
        return (float)std::pow(fineFactor, oscParams.fine->load());
    }

    //float SynthVoice::getSmoothValue(const EffectValues& values, int bufSize, int step)
}