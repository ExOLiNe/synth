#pragma once

#include <juce_audio_processors.h>
#include <vector>

#include "lfo/LFO.h"
#include "../Constants.h"
#include "osc/SynthVoice.h"
#include "osc/SynthSound.h"

#ifdef PROFILING_ENABLED
#include <Tracy.hpp>
#endif

using juce::OwnedArray;
using juce::String;
using juce::StringRef;
using juce::AudioBuffer;
using juce::MidiBuffer;
using juce::Synthesiser;
using ValueTree = juce::AudioProcessorValueTreeState;
using ParameterLayout = ValueTree::ParameterLayout;
using juce::AudioProcessor;

using std::vector;

namespace audio {

    template<
        typename SampleType = float,
        size_t Channels = 2
    >
    class Engine {
    public:

        Engine(AudioProcessor& audioProcessor) :
        processor(audioProcessor),
        valueTree(processor, nullptr, "Params", createLayout()),
        lfo1(valueTree.getRawParameterValue(LFO_1), valueTree.getRawParameterValue(juce::String(LFO_1) + params::lfo::freq.name)),
        lfo2(valueTree.getRawParameterValue(LFO_2), valueTree.getRawParameterValue(juce::String(LFO_2) + params::lfo::freq.name)) {
            oscOutputBuffers.resize(oscIds.size());

            for (int oscIndex = 0; oscIndex < oscIds.size(); ++oscIndex) {
                oscs.add(new juce::Synthesiser());
                oscs[oscIndex]->clearVoices();

                for (size_t j = 0; j < TOTAL_SYNTH_VOICES; ++j) {
                    oscs[oscIndex]->addVoice(new audio::SynthVoice(valueTree, oscIds[static_cast<size_t>(oscIndex)], j));
                }

                oscs[oscIndex]->clearSounds();
                oscs[oscIndex]->addSound(new audio::SynthSound());
            }
        }

        void process(AudioBuffer<SampleType> &buffer, MidiBuffer &midiMessages) {
#ifdef PROFILING_ENABLED
            ZoneScoped;
#endif
            buffer.clear();

            //measurement.start();

            for (int i = 0; i < oscs.size(); ++i) {
                juce::AudioBuffer<float>& oscOutputBuffer = oscOutputBuffers[i];
                oscOutputBuffer.clear();
                oscOutputBuffer.setSize(buffer.getNumChannels(), buffer.getNumSamples());
                oscs[i]->renderNextBlock(oscOutputBuffer, midiMessages, 0, oscOutputBuffer.getNumSamples());
            }

            // Mix together
            for (int channel = 0; channel < buffer.getNumChannels(); ++channel) {
                float* writePtr = buffer.getWritePointer(channel);
                for (const auto & oscBuffer : oscOutputBuffers) {
                    const float* readPtr = oscBuffer.getReadPointer(channel);
                    for (int j = 0; j < buffer.getNumSamples(); ++j) {
                        writePtr[j] += readPtr[j];
                    }
                }
            }

            lfo1.shiftPhase(buffer.getNumSamples(), processor.getSampleRate());
            lfo2.shiftPhase(buffer.getNumSamples(), processor.getSampleRate());

            //measurement.end();
#ifdef PROFILING_ENABLED
            FrameMark;
#endif
        }
        void prepareToPlay (double sampleRate, int samplesPerBlock) {
            jassert((size_t)oscs.size() == oscOutputBuffers.size());

            for (int i = 0; i < oscs.size(); ++i) {
                oscOutputBuffers[i].setSize(Channels, samplesPerBlock);

                oscs[i]->setCurrentPlaybackSampleRate(sampleRate);

                for (int j = 0; j < oscs[i]->getNumVoices(); ++j) {
                    ((audio::SynthVoice*)oscs[i]->getVoice(j))->
                        prepareToPlay(sampleRate, samplesPerBlock, Channels);
                }
            }
        }
        ValueTree& getTreeState() {
            return valueTree;
        }
        [[nodiscard]] const vector<String>& getParamNamesAbleToModulate() const {
            return paramNamesAbleToModulate;
        }
        [[nodiscard]] const vector<String>& getModulatorNames() const {
            return modulatorNames;
        }
    private:
        ParameterLayout createLayout() {
            using namespace std;
            using Param = juce::RangedAudioParameter;
            using Param_f = juce::AudioParameterFloat;
            using Param_i = juce::AudioParameterInt;
            using Param_b = juce::AudioParameterBool;
            vector<unique_ptr<Param>> params;

            vector<string> oscillators = { OSC1, OSC2 };

            using namespace params;

            size_t waveTableTypesTotal = audio::WaveTables::getInstance()->getTotal();
            jassert(waveTableTypesTotal);
            for (auto const &oscId : oscillators) {
                string waveTableTypeName = oscId + osc::waveTableTypeName;
                string voicesName = oscId + osc::voices.name;
                string detune = oscId + osc::detune.name;
                string phase = oscId + osc::phase.name;
                string wtPos = oscId + osc::wtPos.name;
                string pan = oscId + osc::pan.name;
                string level = oscId + osc::level.name;
                string semitone = oscId + osc::semitone.name;
                string fine = oscId + osc::fine.name;

                params.emplace_back(
                    make_unique<Param_i> (
                        voicesName,
                        voicesName,
                        osc::voices.minValue,
                        osc::voices.maxValue,
                        osc::voices.defaultValue));
                params.emplace_back(
                    make_unique<Param_f> (
                        detune,
                        detune,
                        osc::voices.minValue,
                        osc::voices.maxValue,
                        osc::voices.defaultValue ));
                params.emplace_back(
                    make_unique<Param_i> (
                        phase,
                        phase,
                        osc::phase.minValue,
                        osc::phase.maxValue,
                        osc::phase.defaultValue
                    ));
                paramNamesAbleToModulate.emplace_back(phase);
                params.emplace_back(
                    make_unique<Param_f> (
                        wtPos,
                        wtPos,
                        osc::wtPos.minValue,
                        osc::wtPos.maxValue,
                        osc::wtPos.defaultValue));
                paramNamesAbleToModulate.emplace_back(wtPos);
                params.emplace_back(
                    make_unique<Param_f> (
                        pan,
                        pan,
                        osc::pan.minValue,
                        osc::pan.maxValue,
                        osc::pan.defaultValue
                    )
                );
                paramNamesAbleToModulate.emplace_back(level);
                params.emplace_back(
                    make_unique<Param_i> (
                        level,
                        level,
                        osc::level.minValue,
                        osc::level.maxValue,
                        osc::level.defaultValue
                    )
                );
                params.emplace_back(
                    make_unique<Param_i> (
                        semitone,
                        semitone,
                        osc::semitone.minValue,
                        osc::semitone.maxValue,
                        osc::semitone.defaultValue
                    )
                );
                paramNamesAbleToModulate.emplace_back(fine);
                params.emplace_back(
                    make_unique<Param_f> (
                        fine,
                        fine,
                        osc::fine.minValue,
                        osc::fine.maxValue,
                        osc::fine.defaultValue
                    )
                );
                params.emplace_back(
                    make_unique<Param_i>(
                        waveTableTypeName,
                        waveTableTypeName,
                        0,
                        waveTableTypesTotal,
                        0
                    )
                );

                // Modulators
                for (const auto modulatorId : std::array { LFO_1, LFO_2, ADSR_1, ADSR_2 }) {
                    params.emplace_back(
                        make_unique<Param_f>(
                            oscId + params::osc::level.name + modulatorId,
                            oscId + params::osc::level.name + modulatorId,
                            0.f,
                            1.f,
                            0.f
                        )
                    );
                    params.emplace_back(
                        make_unique<Param_f>(
                            oscId + params::osc::pan.name + modulatorId,
                            oscId + params::osc::pan.name + modulatorId,
                            0.f,
                            1.f,
                            0.f
                        )
                    );
                    params.emplace_back(
                        make_unique<Param_f>(
                            oscId + params::osc::phase.name + modulatorId,
                            oscId + params::osc::phase.name + modulatorId,
                            0.f,
                            1.f,
                            0.f
                        )
                    );
                    params.emplace_back(
                        make_unique<Param_f>(
                            oscId + params::osc::fine.name + modulatorId,
                            oscId + params::osc::fine.name + modulatorId,
                            0.f,
                            1.f,
                            0.f
                        )
                    );
                    params.emplace_back(
                        make_unique<Param_f>(
                            oscId + params::osc::wtPos.name + modulatorId,
                            oscId + params::osc::wtPos.name + modulatorId,
                            0.f,
                            1.f,
                            0.f
                        )
                    );
                    params.emplace_back(
                        make_unique<Param_f>(
                            oscId + filter::freq.name + modulatorId,
                            oscId + filter::freq.name + modulatorId,
                            0.f,
                            1.f,
                            0.f
                        )
                    );
                }
            }

            vector<string> fms = { FM_A, FM_B };

            for (auto const &fmId : fms) {
                string freq = fmId + fm::freq.name;
                string mix = fmId + fm::mix.name;
                string enabled = fmId + fm::enabled.name;
                params.emplace_back(
                    make_unique<Param_f> (
                        freq,
                        freq,
                        fm::freq.minValue,
                        fm::freq.maxValue,
                        fm::freq.defaultValue
                    )
                );
                paramNamesAbleToModulate.emplace_back(freq);
                params.emplace_back(
                    make_unique<Param_i> (
                        mix,
                        mix,
                        fm::mix.minValue,
                        fm::mix.maxValue,
                        fm::mix.defaultValue
                    )
                );
                paramNamesAbleToModulate.emplace_back(enabled);
                params.emplace_back(
                    make_unique<Param_b> (
                        enabled,
                        enabled,
                        fm::enabled.defaultValue
                    )
                );
            }

            vector<string> filters = { FILTER_A, FILTER_B };
            for (auto const &osc : oscillators) {
                auto filterId = "filter" + osc;

                string freq = filterId + filter::freq.name;
                string reso = filterId + filter::reso.name;
                string enabled = filterId + filter::enabled.name;
                params.emplace_back(
                    make_unique<Param_f> (
                        freq,
                        freq,
                        filter::freq.minValue,
                        filter::freq.maxValue,
                        filter::freq.defaultValue
                    )
                );
                paramNamesAbleToModulate.emplace_back(freq);
                params.emplace_back(
                    make_unique<Param_f> (
                        reso,
                        reso,
                        filter::reso.minValue,
                        filter::reso.maxValue,
                        filter::reso.defaultValue
                    )
                );
                paramNamesAbleToModulate.emplace_back(enabled);
                params.emplace_back(
                    make_unique<Param_b> (
                        enabled,
                        enabled,
                        filter::enabled.defaultValue
                    )
                );

                for (const auto modulatorId : std::array { ADSR_1, ADSR_2, LFO_1, LFO_2 }) {
                    params.emplace_back(
                        make_unique<Param_f>(
                            filterId + params::filter::freq.name + modulatorId,
                            filterId + params::filter::freq.name + modulatorId,
                            0.f,
                            1.f,
                            0.f
                        )
                    );
                }
            }

            vector<string> adsrs = { ADSR_1, ADSR_2 };
            for (auto const &adsrId : adsrs) {
                modulatorNames.push_back(adsrId);

                string attack = adsrId + adsr::attack.name;
                string decay = adsrId + adsr::decay.name;
                string sustain = adsrId + adsr::sustain.name;
                string release = adsrId + adsr::release.name;

                params.emplace_back(
                    make_unique<Param_f> (
                        attack,
                        attack,
                        adsr::attack.minValue,
                        adsr::attack.maxValue,
                        adsr::attack.defaultValue
                    )
                );
                params.emplace_back(
                    make_unique<Param_f> (
                        decay,
                        decay,
                        adsr::decay.minValue,
                        adsr::decay.maxValue,
                        adsr::decay.defaultValue
                    )
                );
                params.emplace_back(
                    make_unique<Param_i> (
                        sustain,
                        sustain,
                        adsr::sustain.minValue,
                        adsr::sustain.maxValue,
                        adsr::sustain.defaultValue
                    )
                );

                params.emplace_back(
                    make_unique<Param_i> (
                        release,
                        release,
                        adsr::release.minValue,
                        adsr::release.maxValue,
                        adsr::release.defaultValue
                    )
                );

                // adsr value calculated from adsr object
                /*params.emplace_back(
                        make_unique<Param_f> (
                                adsrId,
                                adsrId,
                                -1.f,
                                1.f,
                                0.f
                        )
                );*/
            }

            {
                string attack = params::volumeADSRName + adsr::attack.name;
                string decay = params::volumeADSRName + adsr::decay.name;
                string sustain = params::volumeADSRName + adsr::sustain.name;
                string release = params::volumeADSRName + adsr::release.name;

                params.emplace_back(
                    make_unique<Param_f> (
                        attack,
                        attack,
                        adsr::attack.minValue,
                        adsr::attack.maxValue,
                        adsr::attack.defaultValue
                    )
                );
                params.emplace_back(
                    make_unique<Param_f> (
                        decay,
                        decay,
                        adsr::decay.minValue,
                        adsr::decay.maxValue,
                        adsr::decay.defaultValue
                    )
                );
                params.emplace_back(
                    make_unique<Param_i> (
                        sustain,
                        sustain,
                        adsr::sustain.minValue,
                        adsr::sustain.maxValue,
                        adsr::sustain.defaultValue
                    )
                );

                params.emplace_back(
                    make_unique<Param_i> (
                        release,
                        release,
                        adsr::release.minValue,
                        adsr::release.maxValue,
                        adsr::release.defaultValue
                    )
                );
            }

            vector<string> lfos = { LFO_1, LFO_2 };
            for (auto const& lfoId : lfos) {
                modulatorNames.push_back(lfoId);

                string freq = lfoId + lfo::freq.name;
                string amp = lfoId + lfo::amp.name;
                params.emplace_back(
                    make_unique<Param_f> (
                        freq,
                        freq,
                        lfo::freq.minValue,
                        lfo::freq.maxValue,
                        lfo::freq.defaultValue
                    )
                );
                params.emplace_back(
                    make_unique<Param_f> (
                        amp,
                        amp,
                        lfo::amp.minValue,
                        lfo::amp.maxValue,
                        lfo::amp.defaultValue
                    )
                );

                // lfo value calculated from lfo object
                params.emplace_back(
                    make_unique<Param_f> (
                        lfoId,
                        lfoId,
                        -1.f,
                        1.f,
                        0.f
                    )
                );
            }

            for (auto const& param : params) {
                DBG(param->getParameterID());
            }

            return {params.begin(), params.end()};
        }

        AudioProcessor& processor;
        OwnedArray<Synthesiser> oscs;
        vector<String> oscIds { OSC1, OSC2 };
        vector<AudioBuffer<SampleType>> oscOutputBuffers;
        vector<String> paramNamesAbleToModulate;
        vector<String> modulatorNames;

        ValueTree valueTree;
        LFO lfo1, lfo2;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Engine)
    };

}