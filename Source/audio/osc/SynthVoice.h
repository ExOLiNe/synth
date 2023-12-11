#pragma once

#include "juce_audio_processors.h"
#include "../wave/WaveTables.h"
#include "../../other/polymorphic_readonly_array.h"
#include <juce_dsp.h>
#include "../../Constants.h"
#include <cstdarg>
#include "../../logging/HighFrequencyLogger.h"
#ifdef PROFILING_ENABLED
#include <Tracy.hpp>
#endif

#define LOAD_CURRENT_LFO_VALUE(param_name) \
    lfo1##param_name##AmpValues.current = lfo1##param_name##Amp->load(); \
    lfo2##param_name##AmpValues.current = lfo2##param_name##Amp->load()

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

    struct FilterParams {
        FilterParams(const juce::AudioProcessorValueTreeState& apvts, const juce::String& id):
            filterFreq(apvts.getRawParameterValue(juce::String("filter") + id + params::filter::freq.name)),
            filterReso(apvts.getRawParameterValue(juce::String("filter") + id + params::filter::reso.name)),
            filterEnabled(apvts.getRawParameterValue(juce::String("filter") + id + params::filter::enabled.name)) {
                juce::Logger::writeToLog("ID");
                juce::Logger::writeToLog(juce::String("filter") + id + params::filter::freq.name);
        }
        std::atomic<float>* filterFreq;
        std::atomic<float>* filterReso;
        std::atomic<float>* filterEnabled;
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
            fineAmp(apvts.getRawParameterValue(oscId + params::osc::fine.name + modulatorId)),
            wtPos(apvts.getRawParameterValue(oscId + params::osc::wtPos.name + modulatorId)),
            filterFreqAmp(apvts.getRawParameterValue("filter" + oscId + params::filter::freq.name + modulatorId)){}

        std::atomic<float>* gainAmp;
        std::atomic<float>* panAmp;
        std::atomic<float>* phaseAmp;
        std::atomic<float>* fineAmp;
        std::atomic<float>* wtPos;
        std::atomic<float>* filterFreqAmp;
    };


    template<typename T>
    struct EffectValues {
        T previous;
        T current;
        void updatePrevious() {
            previous = current;
        }
    };

    struct MorphingWave {
        struct Wave {
            size_t index;
            float volume;
        };
        Wave bottom;
        Wave top;
    };

    struct ModulatorCalculatedValues {
        float lfo1, lfo2, adsr1, adsr2;
    };

    template<typename T>
    T getSmoothValue(const EffectValues<T>& values, int bufSize, int step) {
#ifdef PROFILING_ENABLED
        ZoneScoped;
#endif
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
    void updateFilter();
    float getFloatWaveTablePos(const WaveTable& waveTable, const ModulatorCalculatedValues& modulators) const;
    double getFrequency(int i, int numSamples, const ModulatorCalculatedValues& modulators);
    void updateSemitone();
    void updateWaveTableIndex();
    float getFine() const;
    void updateADSR();
    static float getPanGain(const Channel& channel, float pan);
    static MorphingWave getMorphingWave(float waveTablePos);

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
    const std::atomic<float> *lfo1Atomic, *lfo2Atomic;

    int currentWaveTableIndex = -1;
    std::vector<WaveTable> waveTables;

    int midiNote;
    int previousSemitoneOffset = 0;

    EffectValues<float> fineValues {1.f, 1.f};
    EffectValues<float> phaseValues {0.f, 0.f};
    EffectValues<float> detuneValues {0.f, 0.f};
    EffectValues<float> gainValues {1.f, 1.f};
    EffectValues<float> panValues {1.f, 1.f};

    EffectValues<float> lfo1Values, lfo2Values;

    double frequency = 0.0;

    juce::AudioBuffer<float> currentVoiceBuffer;
    juce::AudioBuffer<float> ADSR1Buffer;
    juce::AudioBuffer<float> ADSR2Buffer;

    FilterParams filterParams;
    juce::dsp::LadderFilter<float> filter;

    HighFrequencyLogger<> logger;
};

}
