#pragma once

#include "juce_audio_processors/juce_audio_processors.h"
#include "../wave/WaveTables.h"
#include "../../other/polymorphic_readonly_array.h"
#include <juce_dsp/juce_dsp.h>

namespace audio {
    struct EffectValues {
        float previous;
        float current;
        void updatePrevious() {
            previous = current;
        }
    };

    enum Channel {
        LEFT = 0, RIGHT = 1
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
    float getPanGain(const Channel& channel, float pan) const;
    float getFloatWaveTablePos() const;
    void updateSemitone();
    void updateWaveTable();
    float getFine() const;
    float getSmoothValue(const EffectValues& values, int bufSize, int step) const;

    const juce::String id;

    juce::ADSR gainADSR;
    juce::ADSR::Parameters gainADSRParams;

    const std::atomic<float> *waveTableIndex, *waveTablePos, *gainAtomic, *panAtomic,
        *voicesAtomic, *detuneAtomic, *phaseAtomic, *semitoneAtomic, *fineAtomic;

    int currentWaveTableIndex = -1;
    std::vector<WaveTable> waveTables;
    WaveTable& currentWaveTable;

    int midiNote;
    int previousSemitoneOffset = 0;

    EffectValues fineValues, phaseValues, detuneValues, gainValues, panValues;

    double frequency = 0.0;

    juce::AudioBuffer<float> currentVoiceBuffer;
};

}
