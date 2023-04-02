#pragma once

#include "juce_audio_processors/juce_audio_processors.h"
#include "../wave/WaveTables.h"
#include "../../other/polymorphic_readonly_array.h"
#include <juce_dsp/juce_dsp.h>
#include "../../Constants.h"
#include <cstdarg>
#include "../../other/HighFrequencyLogger.h"

#define LOAD_CURRENT_LFO_VALUE(param_name) \
    lfo1##param_name##AmpValues.current = lfo1##param_name##Amp->load(); \
    lfo2##param_name##AmpValues.current = lfo2##param_name##Amp->load()

/*#define LOAD_CURRENT_VALUES(...) \
    int params[] = { __VA_ARGS__ }; \
    int numfos = sizeof(params); \
    static_assert(false, numfos);*/

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
        // if value is the same(floating point errors)
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
    SynthVoice(const juce::AudioProcessorValueTreeState& apvts, const juce::String id, const size_t voiceId);
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
    template<typename ...T>
    void updatePreviousValues(EffectValues<T>&... effectValues) {
        (effectValues.updatePrevious(), ...);
    }
    float getPanGain(const Channel& channel, float pan) const;
    float getFloatWaveTablePos(const WaveTable& waveTable) const;
    void updateSemitone();
    void updateWaveTableIndex();
    float getFine() const;
    void updateADSR();

    const juce::String id;
    [[maybe_unused]]
    const size_t voiceId;
    const juce::String lfo1Id = LFO_1;
    const juce::String lfo2Id = LFO_2;

    juce::ADSR volumeADSR;
    juce::ADSR::Parameters volumeADSRParams;

    const std::atomic<float> *waveTableIndex, *waveTablePos, *gainAtomic, *panAtomic,
        *voicesAtomic, *detuneAtomic, *phaseAtomic, *semitoneAtomic, *fineAtomic,
        *volumeAttack, *volumeDecay, *volumeSustain, *volumeRelease;

    //TODO add common amp lfo value
    const std::atomic<float> *lfo1, *lfo2;

    const std::atomic<float> *lfo1GainAmp, *lfo1PanAmp, *lfo1PhaseAmp, *lfo1FineAmp;
    const std::atomic<float> *lfo2GainAmp, *lfo2PanAmp, *lfo2PhaseAmp, *lfo2FineAmp;

    int currentWaveTableIndex = -1;
    std::vector<WaveTable> waveTables;

    int midiNote;
    int previousSemitoneOffset = 0;

    EffectValues<float> fineValues, phaseValues, detuneValues, gainValues, panValues;

    EffectValues<float> lfo1Values, lfo2Values;

    EffectValues<float> lfo1GainAmpValues, lfo2GainAmpValues;
    EffectValues<float> lfo1PanAmpValues, lfo2PanAmpValues;
    EffectValues<float> lfo1PhaseAmpValues, lfo2PhaseAmpValues;
    EffectValues<float> lfo1FineAmpValues, lfo2FineAmpValues;

    double frequency = 0.0;

    juce::AudioBuffer<float> currentVoiceBuffer;
    HighFrequencyLogger logger;
};

}
