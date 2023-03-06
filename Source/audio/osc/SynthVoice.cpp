//
// Created by Aleksandr on 26-Feb-23.
//

#include "SynthVoice.h"
#include "SynthSound.h"
#include "../../Constants.h"
#include <juce_gui_extra/juce_gui_extra.h>
#include <cmath>

namespace audio {
    double fineFactor = std::pow(4.0, 1.0 / params::osc::fine.maxValue);

    SynthVoice::SynthVoice(const juce::AudioProcessorValueTreeState& apvts, const juce::String id):
    id(id), waveTables(WaveTables::getInstance()->getWaveTables()),
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
    currentWaveTable(waveTables[(size_t)currentWaveTableIndex]) {
        gainADSRParams = {0.01f, 10.0f, 0.0f, 0.01f};
    }

    bool SynthVoice::canPlaySound(juce::SynthesiserSound* sound) {
        return dynamic_cast<SynthSound*>(sound) != nullptr;
    }

    void SynthVoice::startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound *sound,
                               int currentPitchWheelPosition) {
        midiNote = midiNoteNumber;
        frequency = juce::MidiMessage::getMidiNoteInHertz(midiNote + (int)semitoneAtomic->load());
        currentWaveTable.resetPhaseOffset();
        gainADSR.noteOn();
    }

    void SynthVoice::stopNote(float velocity, bool allowTailOff) {
        gainADSR.noteOff();
    }

    void SynthVoice::pitchWheelMoved(int newPitchWheelValue) {

    }

    void SynthVoice::controllerMoved(int controllerNumber, int newControllerValue) {

    }

    void SynthVoice::prepareToPlay(double sampleRate, int samplesPerBlock, int outputChannels) {
        gainADSR.setSampleRate(sampleRate);
        gainADSR.setParameters(gainADSRParams);
        gainADSR.reset();
    }

    void SynthVoice::renderNextBlock(juce::AudioBuffer<float> &outputBuffer, int startSample, int numSamples) {
        if (gainAtomic->load() == 0.0f) {
            return;
        }

        updateSemitone();
        updateWaveTable();

        const float floatWaveTablePos = getFloatWaveTablePos();
        const int upperWaveIndex = std::ceil(floatWaveTablePos);
        const int lowerWaveIndex = std::floor(floatWaveTablePos);

        // Calculate gain factors of the nearest waves
        const float upperWaveGainFactor = floatWaveTablePos - (float)lowerWaveIndex;
        const float lowerWaveGainFactor = 1.0f - upperWaveGainFactor;

        auto writePtrL = outputBuffer.getWritePointer(Channel::LEFT);
        auto writePtrR = outputBuffer.getWritePointer(Channel::RIGHT);

        fineValues.current = getFine();
        phaseValues.current = phaseAtomic->load();
        detuneValues.current = detuneAtomic->load();
        gainValues.current = gainAtomic->load();
        panValues.current = panAtomic->load();
        const int voices = voicesAtomic->load();

        for (unsigned int i = startSample; i < numSamples; ++i) {
            const double finalFrequency = frequency * getSmoothValue(fineValues, numSamples, i - startSample);
            currentWaveTable.shiftPhase();
            const float phaseOffset = getSmoothValue(phaseValues, numSamples, i - startSample);
            const float detune = getSmoothValue(detuneValues, numSamples, i - startSample);
            float output = 0.0;
            for (int voiceIndex = 0; voiceIndex < voices; ++voiceIndex) {
                const double detuneFactor = std::pow(std::exp(std::log(2) / 1200), detune * (voices / 2 - voiceIndex));

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
            output *= 0.4f * (getSmoothValue(gainValues, numSamples, i - startSample) / 100.0f);
            const float pan = getSmoothValue(panValues, numSamples, i - startSample);
            writePtrL[i] += output * getPanGain(Channel::LEFT, pan);
            writePtrR[i] += output * getPanGain(Channel::RIGHT, pan);
        }
        gainADSR.applyEnvelopeToBuffer(outputBuffer, startSample, numSamples);

        fineValues.updatePrevious();
        phaseValues.updatePrevious();
        detuneValues.updatePrevious();
        gainValues.updatePrevious();
        panValues.updatePrevious();
    }

    float SynthVoice::getFloatWaveTablePos() const {
        return waveTablePos->load() / 100 * (currentWaveTable.waveTable.size() - 1);
    }

    float SynthVoice::getPanGain(const Channel& channel, float pan) const {
        float direction = channel == Channel::LEFT ? -1.0f : 1.0f;
        return (direction * pan + 1) / 2.0f;
    }

    void SynthVoice::updateSemitone() {
        if (previousSemitoneOffset != (int)semitoneAtomic->load()) {
            frequency = juce::MidiMessage::getMidiNoteInHertz(midiNote + (int)semitoneAtomic->load());
            previousSemitoneOffset = (int)semitoneAtomic->load();
        }
    }

    void SynthVoice::updateWaveTable() {
        auto loadedWaveTableIndex = (int)waveTableIndex->load();
        if (currentWaveTableIndex != loadedWaveTableIndex) {
            currentWaveTableIndex = loadedWaveTableIndex;
            currentWaveTable = waveTables[(size_t)currentWaveTableIndex];
        }
    }

    float SynthVoice::getFine() const {
        return (float)std::pow(fineFactor, fineAtomic->load());
    }

    float SynthVoice::getSmoothValue(const EffectValues& values, int bufSize, int step) const {
        jassert(bufSize > 0);
        if (std::abs(values.previous - values.current) <= 0.001f) {
            return values.current;
        } else {
            auto stepSize = (values.current - values.previous) / bufSize;
            return values.previous + stepSize * step;
        }
    }
}