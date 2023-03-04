//
// Created by Aleksandr on 26-Feb-23.
//

#include "SynthVoice.h"
#include "SynthSound.h"
#include "../../Constants.h"
#include <cmath>

namespace audio {
    SynthVoice::SynthVoice(const juce::AudioProcessorValueTreeState& apvts, const juce::String id):
    apvts(apvts),
    id(id), waveTables(WaveTables::getInstance()->getWaveTables()),
    waveTablePos(apvts.getRawParameterValue(id + params::osc::wtPos.name)),
    waveTableIndex(apvts.getRawParameterValue(id + params::osc::waveTableTypeName)),
    gainAtomic(apvts.getRawParameterValue(id + params::osc::level.name)),
    panAtomic(apvts.getRawParameterValue(id + params::osc::pan.name)) {
        gainADSRParams = {0.01f, 10.0f, 0.0f, 0.01f};
    }

    bool SynthVoice::canPlaySound(juce::SynthesiserSound* sound) {
        return dynamic_cast<SynthSound*>(sound) != nullptr;
    }

    void SynthVoice::startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound *sound,
                               int currentPitchWheelPosition) {
        frequency = juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber);
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
        auto &table = waveTables[(size_t)waveTableIndex->load()];
        auto waveIndex = static_cast<int>(std::round(waveTablePos->load() / 100 * (table.waveTable.size() - 1)));
        auto wave = table.waveTable[waveIndex];
        jassert(wave != nullptr);
        auto writePtrL = outputBuffer.getWritePointer(0);
        auto writePtrR = outputBuffer.getWritePointer(1);
        for (unsigned int i = startSample; i < numSamples; ++i) {
            //TODO implement multivoice => for (int j = 0; j < voices; ++j)
            float output = wave->generate(frequency, getSampleRate()) * 0.4f * (gainAtomic->load() / 100.0f);
            writePtrL[i] = output * getPanGain(Channel::LEFT);
            writePtrR[i] = output * getPanGain(Channel::RIGHT);
        }
        gainADSR.applyEnvelopeToBuffer(outputBuffer, startSample, numSamples);
    }

    float SynthVoice::getPanGain(Channel channel) {
        float direction = channel == Channel::LEFT ? -1.0f : 1.0f;
        return (direction * panAtomic->load() + 1) / 2.0f;
    }
}