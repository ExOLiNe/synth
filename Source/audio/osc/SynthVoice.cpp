//
// Created by Aleksandr on 26-Feb-23.
//

#include "SynthVoice.h"
#include "SynthSound.h"
#include <juce_gui_extra.h>
#include <cmath>

namespace audio {
    double fineFactor = std::pow(4.0, 1.0 / params::osc::fine.maxValue);

    SynthVoice::SynthVoice(const juce::AudioProcessorValueTreeState& apvts, const juce::String id, const size_t voiceId):
            id(id), voiceId(voiceId),
            waveTableIndex(apvts.getRawParameterValue(id + params::osc::waveTableTypeName)),
            waveTablePos(apvts.getRawParameterValue(id + params::osc::wtPos.name)),
            gainAtomic(apvts.getRawParameterValue(id + params::osc::level.name)),
            panAtomic(apvts.getRawParameterValue(id + params::osc::pan.name)),
            voicesAtomic(apvts.getRawParameterValue(id + params::osc::voices.name)),
            detuneAtomic(apvts.getRawParameterValue(id + params::osc::detune.name)),
            phaseAtomic(apvts.getRawParameterValue(id + params::osc::phase.name)),
            semitoneAtomic(apvts.getRawParameterValue(id + params::osc::semitone.name)),
            fineAtomic(apvts.getRawParameterValue(id + params::osc::fine.name)),
            volumeAttack(apvts.getRawParameterValue(params::volumeADSRName + params::adsr::attack.name)),
            volumeDecay(apvts.getRawParameterValue(params::volumeADSRName + params::adsr::decay.name)),
            volumeSustain(apvts.getRawParameterValue(params::volumeADSRName + params::adsr::sustain.name)),
            volumeRelease(apvts.getRawParameterValue(params::volumeADSRName + params::adsr::release.name)),
            ADSR1Attack(apvts.getRawParameterValue(adsr1Id + params::adsr::attack.name)),
            ADSR1Decay(apvts.getRawParameterValue(adsr1Id + params::adsr::decay.name)),
            ADSR1Sustain(apvts.getRawParameterValue(adsr1Id + params::adsr::sustain.name)),
            ADSR1Release(apvts.getRawParameterValue(adsr1Id + params::adsr::release.name)),
            ADSR2Attack(apvts.getRawParameterValue(adsr2Id + params::adsr::attack.name)),
            ADSR2Decay(apvts.getRawParameterValue(adsr2Id + params::adsr::decay.name)),
            ADSR2Sustain(apvts.getRawParameterValue(adsr2Id + params::adsr::sustain.name)),
            ADSR2Release(apvts.getRawParameterValue(adsr2Id + params::adsr::release.name)),
            ADSR1GainAmp(apvts.getRawParameterValue(id + params::osc::level.name + adsr1Id)),
            ADSR1PanAmp(apvts.getRawParameterValue(id + params::osc::pan.name + adsr1Id)),
            ADSR1PhaseAmp(apvts.getRawParameterValue(id + params::osc::phase.name + adsr1Id)),
            ADSR1FineAmp(apvts.getRawParameterValue(id + params::osc::fine.name + adsr1Id)),
            ADSR2GainAmp(apvts.getRawParameterValue(id + params::osc::level.name + adsr2Id)),
            ADSR2PanAmp(apvts.getRawParameterValue(id + params::osc::pan.name + adsr2Id)),
            ADSR2PhaseAmp(apvts.getRawParameterValue(id + params::osc::phase.name + adsr2Id)),
            ADSR2FineAmp(apvts.getRawParameterValue(id + params::osc::fine.name + adsr2Id)),
            lfo1(apvts.getRawParameterValue(lfo1Id)),
            lfo2(apvts.getRawParameterValue(lfo2Id)),
            lfo1GainAmp(apvts.getRawParameterValue(id + params::osc::level.name + lfo1Id)),
            lfo1PanAmp(apvts.getRawParameterValue(id + params::osc::pan.name + lfo1Id)),
            lfo1PhaseAmp(apvts.getRawParameterValue(id + params::osc::phase.name + lfo1Id)),
            lfo1FineAmp(apvts.getRawParameterValue(id + params::osc::fine.name + lfo1Id)),
            lfo2GainAmp(apvts.getRawParameterValue(id + params::osc::level.name + lfo2Id)),
            lfo2PanAmp(apvts.getRawParameterValue(id + params::osc::pan.name + lfo2Id)),
            lfo2PhaseAmp(apvts.getRawParameterValue(id + params::osc::phase.name + lfo2Id)),
            lfo2FineAmp(apvts.getRawParameterValue(id + params::osc::fine.name + lfo2Id)),
            currentWaveTableIndex((int)waveTableIndex->load()),
            waveTables(WaveTables::getInstance()->copyWaveTables())
                       {

    }

    bool SynthVoice::canPlaySound(juce::SynthesiserSound* sound) {
        return dynamic_cast<SynthSound*>(sound) != nullptr;
    }

    void SynthVoice::startNote(int midiNoteNumber, float, juce::SynthesiserSound *,
                               int) {
        midiNote = midiNoteNumber;
        frequency = juce::MidiMessage::getMidiNoteInHertz(midiNote + (int)semitoneAtomic->load());
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
        volumeADSR.setSampleRate(sampleRate);
        volumeADSR.setParameters(volumeADSRParams);
        volumeADSR.reset();

        ADSR1.setSampleRate(sampleRate);
        ADSR1.setParameters(ADSR1Params);
        ADSR1.reset();

        ADSR2.setSampleRate(sampleRate);
        ADSR2.setParameters(ADSR2Params);
        ADSR2.reset();
    }

    void SynthVoice::renderNextBlock(juce::AudioBuffer<float> &outputBuffer, int startSample, int numSamples) {
        if (gainAtomic->load() == 0.0f) {
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

            /*lfo1GainAmpValues,  lfo2GainAmpValues,
            lfo1PanAmpValues,   lfo2PanAmpValues,
            lfo1PhaseAmpValues, lfo2PhaseAmpValues,
            lfo1FineAmpValues,  lfo2FineAmpValues,*/
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
        phaseValues.current = phaseAtomic->load();
        detuneValues.current = detuneAtomic->load();
        gainValues.current = gainAtomic->load();
        panValues.current = panAtomic->load();
        const int voices = (int)voicesAtomic->load();

        lfo1Values.current = lfo1->load();
        lfo2Values.current = lfo2->load();

        /*LOAD_CURRENT_LFO_VALUE(Gain);
        LOAD_CURRENT_LFO_VALUE(Pan);
        LOAD_CURRENT_LFO_VALUE(Phase);
        LOAD_CURRENT_LFO_VALUE(Fine);*/

        auto sampleRate = getSampleRate();

        auto lfo1FineAmpValue = lfo1FineAmp->load();
        auto lfo2FineAmpValue = lfo2FineAmp->load();

        for (int i = 0; i < numSamples; ++i) {
            auto freqLFO1Offset = frequency * lfo1FineAmpValue * getSmoothValue(lfo1Values, numSamples, i);
            auto freqLFO2Offset = frequency * lfo2FineAmpValue * getSmoothValue(lfo2Values, numSamples, i);
            auto freqADSR1Offset = frequency * ADSR1Ptr[i] * ADSR1FineAmp->load();
            auto freqADSR2Offset = frequency * ADSR2Ptr[i] * ADSR2FineAmp->load();
            const double finalFrequency = frequency * getSmoothValue(fineValues, numSamples, i)
                                          + freqLFO1Offset + freqLFO2Offset + freqADSR1Offset + freqADSR2Offset;
            currentWaveTable.shiftPhase();
            const float phaseOffset = getSmoothValue(phaseValues, numSamples, i)
                    + sampleRate / finalFrequency * lfo1PhaseAmp->load();
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
            if (lfo1GainAmp->load() > 0.0001f) {
                output *= (1.f + lfo1GainAmp->load()) * getSmoothValue(lfo1Values, numSamples, i);
            }
            if (lfo2GainAmp->load() > 0.0001f) {
                output *= (1.f + lfo2GainAmp->load()) * getSmoothValue(lfo2Values, numSamples, i);
            }
            /*if (id == "osc1" && voiceId == 0) {
                logger.log(lfo1Values.current);
            }*/
            const float pan = getSmoothValue(panValues, numSamples, i) +
                    lfo1PanAmp->load() * getSmoothValue(lfo1Values, numSamples, i);
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
        return waveTablePos->load() / 100 * (waveTable.waveTable.size() - 1);
    }

    float SynthVoice::getPanGain(const Channel& channel, float pan) const {
        float direction = channel == Channel::LEFT ? -1.0f : 1.0f;
        return (direction * pan + 1) / 2.0f;
    }

    void SynthVoice::updateADSR() {
        ADSR1Params.attack = ADSR1Attack->load() * params::adsr::attackFactor;
        ADSR1Params.decay = ADSR1Decay->load() * params::adsr::decayFactor;
        ADSR1Params.sustain = ADSR1Sustain->load() * params::adsr::sustainFactor;
        ADSR1Params.release = ADSR1Release->load() * params::adsr::releaseFactor;
        ADSR1.setParameters(ADSR1Params);

        ADSR2Params.attack = ADSR2Attack->load() * params::adsr::attackFactor;
        ADSR2Params.decay = ADSR2Decay->load() * params::adsr::decayFactor;
        ADSR2Params.sustain = ADSR2Sustain->load() * params::adsr::sustainFactor;
        ADSR2Params.release = ADSR2Release->load() * params::adsr::releaseFactor;
        ADSR2.setParameters(ADSR2Params);

        volumeADSRParams.attack = volumeAttack->load() * params::adsr::attackFactor;
        volumeADSRParams.decay = volumeDecay->load() * params::adsr::decayFactor;
        volumeADSRParams.sustain = volumeSustain->load() * params::adsr::sustainFactor;
        volumeADSRParams.release = volumeRelease->load() * params::adsr::releaseFactor;
        volumeADSR.setParameters(volumeADSRParams);
    }

    void SynthVoice::updateSemitone() {
        if (previousSemitoneOffset != (int)semitoneAtomic->load()) {
            frequency = juce::MidiMessage::getMidiNoteInHertz(midiNote + (int)semitoneAtomic->load());
            previousSemitoneOffset = (int)semitoneAtomic->load();
        }
    }

    void SynthVoice::updateWaveTableIndex() {
        currentWaveTableIndex = (int)waveTableIndex->load() - 1;
        if (currentWaveTableIndex == -1) currentWaveTableIndex = 0;
    }

    float SynthVoice::getFine() const {
        return (float)std::pow(fineFactor, fineAtomic->load());
    }

    //float SynthVoice::getSmoothValue(const EffectValues& values, int bufSize, int step)
}