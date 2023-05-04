#pragma once

#include "juce_audio_processors.h"
#include "../wave/WaveTables.h"
#include "../../other/polymorphic_readonly_array.h"
#include <juce_dsp.h>
#include "../../Constants.h"
#include <cstdarg>
#include "../../logging/HighFrequencyLogger.h"
#include <Tracy.hpp>

#define LOAD_CURRENT_LFO_VALUE(param_name) \
    lfo1##param_name##AmpValues.current = lfo1##param_name##Amp->load(); \
    lfo2##param_name##AmpValues.current = lfo2##param_name##Amp->load()

/*#define LOAD_CURRENT_VALUES(...) \
    int params[] = { __VA_ARGS__ }; \
    int numfos = sizeof(params); \
    static_assert(false, numfos);*/

namespace audio {
    struct OscParams {
        OscParams(const juce::AudioProcessorValueTreeState& apvts, const juce::String& id):
                waveTableIndex(apvts.getRawParameterValue(id + params::osc::waveTableTypeName)),
                waveTablePos(apvts.getRawParameterValue(id + params::osc::wtPos.name)),
                gain(apvts.getRawParameterValue(id + params::osc::level.name)),
                pan(apvts.getRawParameterValue(id + params::osc::pan.name)),
                voices(apvts.getRawParameterValue(id + params::osc::voices.name)),
                detune(apvts.getRawParameterValue(id + params::osc::detune.name)),
                phase(apvts.getRawParameterValue(id + params::osc::phase.name)),
                semitone(apvts.getRawParameterValue(id + params::osc::semitone.name)),
                fine(apvts.getRawParameterValue(id + params::osc::fine.name)) {}

        std::atomic<float>* waveTableIndex;
        std::atomic<float>* waveTablePos;
        std::atomic<float>* gain;
        std::atomic<float>* pan;
        std::atomic<float>* voices;
        std::atomic<float>* detune;
        std::atomic<float>* phase;
        std::atomic<float>* semitone;
        std::atomic<float>* fine;
    };

    struct ADSRParams {
        ADSRParams(const juce::AudioProcessorValueTreeState& apvts, const juce::String& adsrName):
                attack(apvts.getRawParameterValue(adsrName + params::adsr::attack.name)),
                decay(apvts.getRawParameterValue(adsrName + params::adsr::decay.name)),
                sustain(apvts.getRawParameterValue(adsrName + params::adsr::sustain.name)),
                release(apvts.getRawParameterValue(adsrName + params::adsr::release.name))
        {

        }
        std::atomic<float>* attack;
        std::atomic<float>* decay;
        std::atomic<float>* sustain;
        std::atomic<float>* release;
    };

    struct ModulatorParamAmps {
        ModulatorParamAmps(const juce::AudioProcessorValueTreeState& apvts, const juce::String& oscId, const juce::String& modulatorId):
            gainAmp(apvts.getRawParameterValue(oscId + params::osc::level.name + modulatorId)),
            panAmp(apvts.getRawParameterValue(oscId + params::osc::pan.name + modulatorId)),
            phaseAmp(apvts.getRawParameterValue(oscId + params::osc::phase.name + modulatorId)),
            fineAmp(apvts.getRawParameterValue(oscId + params::osc::fine.name + modulatorId)) {}

        std::atomic<float>* gainAmp;
        std::atomic<float>* panAmp;
        std::atomic<float>* phaseAmp;
        std::atomic<float>* fineAmp;
    };


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
        ZoneScoped;
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
    const juce::String adsr1Id = ADSR_1;
    const juce::String adsr2Id = ADSR_2;

    juce::ADSR volumeADSR;
    juce::ADSR ADSR1;
    juce::ADSR ADSR2;

    OscParams oscParams;

    ADSRParams volumeAdsrParams;
    ADSRParams adsr1Params;
    ADSRParams adsr2Params;


    ModulatorParamAmps adsr1Amps;
    ModulatorParamAmps adsr2Amps;
    ModulatorParamAmps lfo1Amps;
    ModulatorParamAmps lfo2Amps;

    //TODO add common amp lfo value
    const std::atomic<float> *lfo1, *lfo2;

    int currentWaveTableIndex = -1;
    std::vector<WaveTable> waveTables;

    int midiNote;
    int previousSemitoneOffset = 0;

    EffectValues<float> fineValues {}, phaseValues {}, detuneValues {}, gainValues {}, panValues {};

    EffectValues<float> lfo1Values, lfo2Values;

    double frequency = 0.0;

    juce::AudioBuffer<float> currentVoiceBuffer;
    juce::AudioBuffer<float> ADSR1Buffer;
    juce::AudioBuffer<float> ADSR2Buffer;

    HighFrequencyLogger logger;
};

}
