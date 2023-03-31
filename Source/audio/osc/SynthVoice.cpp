//
// Created by Aleksandr on 26-Feb-23.
//

#include "SynthVoice.h"
#include "SynthSound.h"
#include <juce_gui_extra/juce_gui_extra.h>
#include <cmath>

namespace audio {
    double fineFactor = std::pow(4.0, 1.0 / params::osc::fine.maxValue);

    SynthVoice::SynthVoice(const juce::AudioProcessorValueTreeState& apvts, const juce::String id, const size_t voiceId):
            id(id), voiceId(voiceId), waveTables(WaveTables::getInstance()->copyWaveTables()),
            waveTablePos(apvts.getRawParameterValue(id + params::osc::wtPos.name)),
            waveTableIndex(apvts.getRawParameterValue(id + params::osc::waveTableTypeName)),
            gainAtomic(apvts.getRawParameterValue(id + params::osc::level.name)),
            panAtomic(apvts.getRawParameterValue(id + params::osc::pan.name)),
            voicesAtomic(apvts.getRawParameterValue(id + params::osc::voices.name)),
            detuneAtomic(apvts.getRawParameterValue(id + params::osc::detune.name)),
            phaseAtomic(apvts.getRawParameterValue(id + params::osc::phase.name)),
            semitoneAtomic(apvts.getRawParameterValue(id + params::osc::semitone.name)),
            fineAtomic(apvts.getRawParameterValue(id + params::osc::fine.name)),
            currentWaveTableIndex((int)waveTableIndex->load()),
            volumeAttack(apvts.getRawParameterValue(params::volumeADSRName + params::adsr::attack.name)),
            volumeDecay(apvts.getRawParameterValue(params::volumeADSRName + params::adsr::decay.name)),
            volumeSustain(apvts.getRawParameterValue(params::volumeADSRName + params::adsr::sustain.name)),
            volumeRelease(apvts.getRawParameterValue(params::volumeADSRName + params::adsr::release.name)),
            lfo1(apvts.getRawParameterValue(lfo1Id)),
            lfo2(apvts.getRawParameterValue(lfo2Id)),
            lfo1GainAmp(apvts.getRawParameterValue(id + lfo1Id + params::osc::level.name)),
            lfo2GainAmp(apvts.getRawParameterValue(id + lfo2Id + params::osc::level.name)),
            lfo1PanAmp(apvts.getRawParameterValue(id + lfo1Id + params::osc::pan.name)),
            lfo2PanAmp(apvts.getRawParameterValue(id + lfo2Id + params::osc::pan.name)),
            lfo1PhaseAmp(apvts.getRawParameterValue(id + lfo1Id + params::osc::phase.name)),
            lfo2PhaseAmp(apvts.getRawParameterValue(id + lfo2Id + params::osc::phase.name)),
            lfo1FineAmp(apvts.getRawParameterValue(id + lfo1Id + params::osc::fine.name)),
            lfo2FineAmp(apvts.getRawParameterValue(id + lfo2Id + params::osc::fine.name))
                       {

    }

    bool SynthVoice::canPlaySound(juce::SynthesiserSound* sound) {
        return dynamic_cast<SynthSound*>(sound) != nullptr;
    }

    void SynthVoice::startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound *sound,
                               int currentPitchWheelPosition) {
        midiNote = midiNoteNumber;
        frequency = juce::MidiMessage::getMidiNoteInHertz(midiNote + (int)semitoneAtomic->load());
        for (auto & table : waveTables) {
            table.resetPhaseOffset();
        }
        volumeADSR.noteOn();
    }

    void SynthVoice::stopNote(float velocity, bool allowTailOff) {
        volumeADSR.noteOff();
    }

    void SynthVoice::pitchWheelMoved(int newPitchWheelValue) {

    }

    void SynthVoice::controllerMoved(int controllerNumber, int newControllerValue) {

    }

    void SynthVoice::prepareToPlay(double sampleRate, int samplesPerBlock, int outputChannels) {
        volumeADSR.setSampleRate(sampleRate);
        volumeADSR.setParameters(volumeADSRParams);
        volumeADSR.reset();
    }

    void SynthVoice::renderNextBlock(juce::AudioBuffer<float> &outputBuffer, int startSample, int numSamples) {
        if (gainAtomic->load() == 0.0f) {
            return;
        }

        currentVoiceBuffer.clear();
        if (currentVoiceBuffer.getNumSamples() < numSamples) {
            currentVoiceBuffer.setSize(outputBuffer.getNumChannels(), numSamples);
        }

        updatePreviousValues(
            fineValues, phaseValues, detuneValues, gainValues, panValues,

            lfo1GainAmpValues,  lfo2GainAmpValues,
            lfo1PanAmpValues,   lfo2PanAmpValues,
            lfo1PhaseAmpValues, lfo2PhaseAmpValues,
            lfo1FineAmpValues,  lfo2FineAmpValues,
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
        //DBG(juce::String(lfo1Values.previous) + " " + std::to_string(lfo1Values.current));
        lfo2Values.current = lfo2->load();

        LOAD_CURRENT_LFO_VALUE(Gain);
        LOAD_CURRENT_LFO_VALUE(Pan);
        LOAD_CURRENT_LFO_VALUE(Phase);
        LOAD_CURRENT_LFO_VALUE(Fine);

        float maxFreq = 0.f;

        for (int i = 0; i < numSamples; ++i) {
            //auto freqLfoOffset = frequency * /*lfo1FineAmpValues.current */ 0.1f * getSmoothValue(lfo1Values, numSamples, i);
            //freqLfoOffset = 0.f;
            const double finalFrequency = frequency; /** getSmoothValue(fineValues, numSamples, i) + freqLfoOffset*/;

            if (finalFrequency > maxFreq) {
                maxFreq = finalFrequency;
            }
            currentWaveTable.shiftPhase();
            const float phaseOffset = getSmoothValue(phaseValues, numSamples, i);
            const float detune = getSmoothValue(detuneValues, numSamples, i);
            float output = 0.0;
            for (int voiceIndex = 0; voiceIndex < voices; ++voiceIndex) {
                const double detuneFactor = std::pow(std::exp(std::log(2) / 1200), detune * ((float)voices / 2 - voiceIndex));

                // Morphing between the nearest waves
                const float upperOutput = currentWaveTable.generateSample(
                        finalFrequency * detuneFactor, getSampleRate(), upperWaveIndex, phaseOffset
                );
                const float lowerOutput = currentWaveTable.generateSample(
                        finalFrequency * detuneFactor, getSampleRate(), lowerWaveIndex, phaseOffset
                );
                output += upperOutput * upperWaveGainFactor + lowerOutput * lowerWaveGainFactor;
            }
            output /= voices;

            // gain & pan
            output *= 0.4f * (getSmoothValue(gainValues, numSamples, i) / 100.0f);
            /*output *= getSmoothValue(lfo1Values, numSamples, i);*/
            if (id == "osc1" && voiceId == 0) {
                logger.log(lfo1Values.current);
            }
            const float pan = getSmoothValue(panValues, numSamples, i)/* + getSmoothValue(lfo1Values, numSamples, i)*/;
            voicePtrL[i] += output * getPanGain(Channel::LEFT, pan);
            voicePtrR[i] += output * getPanGain(Channel::RIGHT, pan);
        }

        /*if (id == "osc1" && voiceId == 0) {
            DBG(id + " - " + std::to_string(voiceId) + " - " + std::to_string(maxFreq));
        }*/

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