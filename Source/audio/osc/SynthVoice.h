#pragma once

#include "juce_audio_processors/juce_audio_processors.h"
#include "../wave/WaveTables.h"
#include "../../other/polymorphic_readonly_array.h"
#include <juce_dsp/juce_dsp.h>

namespace audio {
    template<typename T>
    struct EffectValues {
        T previous;
        T current;
        void updatePrevious() {
            previous = current;
        }
    };

    template<typename T>
    T getSmoothValue(const EffectValues<T>& values, int bufSize, int step) {
        jassert(bufSize > 0);
        if (std::abs(values.previous - values.current) <= 0.001f) {
            return values.current;
        } else {
            auto stepSize = (values.current - values.previous) / bufSize;
            return values.previous + stepSize * step;
        }
    }

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
    float getFloatWaveTablePos(const WaveTable& waveTable) const;
    void updateSemitone();
    void updateWaveTableIndex();
    float getFine() const;
    void updateADSR();

    const juce::String id;

    juce::ADSR volumeADSR;
    juce::ADSR::Parameters volumeADSRParams;

    const std::atomic<float> *waveTableIndex, *waveTablePos, *gainAtomic, *panAtomic,
        *voicesAtomic, *detuneAtomic, *phaseAtomic, *semitoneAtomic, *fineAtomic;

    std::atomic<float> *volumeAttack, *volumeDecay, *volumeSustain, *volumeRelease;

    int currentWaveTableIndex = -1;
    std::vector<WaveTable> waveTables;

    int midiNote;
    int previousSemitoneOffset = 0;

    EffectValues<float> fineValues, phaseValues, detuneValues, gainValues, panValues;

    double frequency = 0.0;

    juce::AudioBuffer<float> currentVoiceBuffer;
};

}
