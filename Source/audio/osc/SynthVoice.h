#pragma once

#include "juce_audio_processors/juce_audio_processors.h"
#include "../wave/WaveTables.h"
#include <juce_dsp/juce_dsp.h>

namespace audio {

    enum Channel {
        LEFT, RIGHT
    };

class SynthVoice : public juce::SynthesiserVoice {
public:
    SynthVoice(const juce::AudioProcessorValueTreeState& apvts, const juce::String id);
    bool canPlaySound(juce::SynthesiserSound*) override;
    void startNote(int midiNoteNumber,
                   float velocity,
                   juce::SynthesiserSound* sound,
                   int currentPitchWheelPosition) override;
    void stopNote(float velocity, bool allowTailOff) override;
    void pitchWheelMoved(int newPitchWheelValue) override;
    void controllerMoved(int controllerNumber, int newControllerValue) override;
    void renderNextBlock(juce::AudioBuffer<float>& outputBuffer,
                         int startSample,
                         int numSamples) override;
    void prepareToPlay(double sampleRate, int samplesPerBlock, int outputChannels);
private:
    float getPanGain(Channel channel);
    float getFloatWaveTablePos();

    const juce::AudioProcessorValueTreeState& apvts;
    const juce::String id;

    juce::ADSR gainADSR;
    juce::ADSR::Parameters gainADSRParams;

    std::vector<WaveTable> waveTables;

    const std::atomic<float> *gainAtomic, *panAtomic;

    const std::atomic<float> *waveTableIndex, *waveTablePos;

    int currentWaveTableIndex = -1;
    WaveTable& currentWaveTable;

    double frequency = 0.0;
};

}
