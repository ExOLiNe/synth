/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <memory>
#include <vector>
#include "Constants.h"
#include "audio/osc/SynthVoice.h"
#include "audio/osc/SynthSound.h"
#include "audio/wave/WaveTables.h"

//==============================================================================
SynthAudioProcessor::SynthAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
        : AudioProcessor (BusesProperties()
#if ! JucePlugin_IsMidiEffect
#if ! JucePlugin_IsSynth
                                  .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
#endif
                                  .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
#endif
), treeState(*this, nullptr, "Params", createLayout()),
lfo(treeState.getRawParameterValue(LFO_1), lfoFreq)
#endif
{
    oscOutputBuffers.resize(oscIds.size());

    for (size_t oscIndex = 0; oscIndex < oscIds.size(); ++oscIndex) {
        oscillators.add(new juce::Synthesiser());
        oscillators[oscIndex]->clearVoices();

        for (size_t j = 0; j < TOTAL_SYNTH_VOICES; ++j) {
            oscillators[oscIndex]->addVoice(new audio::SynthVoice(treeState, oscIds[oscIndex], j));
        }

        oscillators[oscIndex]->clearSounds();
        oscillators[oscIndex]->addSound(new audio::SynthSound());
    }
}

SynthAudioProcessor::~SynthAudioProcessor()
{
    delete audio::WaveTables::getInstance();
}

//==============================================================================
const juce::String SynthAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool SynthAudioProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
    return true;
#else
    return false;
#endif
}

bool SynthAudioProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
    return true;
#else
    return false;
#endif
}

bool SynthAudioProcessor::isMidiEffect() const
{
#if JucePlugin_IsMidiEffect
    return true;
#else
    return false;
#endif
}

double SynthAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int SynthAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
    // so this should be at least 1, even if you're not really implementing programs.
}

int SynthAudioProcessor::getCurrentProgram()
{
    return 0;
}

void SynthAudioProcessor::setCurrentProgram (int)
{
}

const juce::String SynthAudioProcessor::getProgramName (int)
{
    return {};
}

void SynthAudioProcessor::changeProgramName (int, const juce::String&)
{
}

//==============================================================================
void SynthAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    jassert((size_t)oscillators.size() == oscOutputBuffers.size());

    for (int i = 0; i < oscillators.size(); ++i) {
        oscOutputBuffers[i].setSize(getTotalNumOutputChannels(), samplesPerBlock);

        oscillators[i]->setCurrentPlaybackSampleRate(sampleRate);

        for (int j = 0; j < oscillators[i]->getNumVoices(); ++j) {
            ((audio::SynthVoice*)oscillators[i]->getVoice(j))->
                    prepareToPlay(sampleRate, samplesPerBlock, getTotalNumOutputChannels());
        }
    }
}

void SynthAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool SynthAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
#if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
#else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
        && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
#if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
#endif

    return true;
#endif
}
#endif

void SynthAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    buffer.clear();

    auto start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < oscillators.size(); ++i) {
        juce::AudioBuffer<float>& oscOutputBuffer = oscOutputBuffers[i];
        oscOutputBuffer.clear();
        oscOutputBuffer.setSize(buffer.getNumChannels(), buffer.getNumSamples());
        oscillators[i]->renderNextBlock(oscOutputBuffer, midiMessages, 0, oscOutputBuffer.getNumSamples());
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

    lfo.shiftPhase(buffer.getNumSamples(), getSampleRate());

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
    timeAccum += duration;
    times++;
}

//==============================================================================
bool SynthAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* SynthAudioProcessor::createEditor()
{
    return new SynthAudioProcessorEditor (*this);
}

//==============================================================================
void SynthAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    auto state = treeState.copyState();
    std::unique_ptr<juce::XmlElement> xml (state.createXml());
    copyXmlToBinary (*xml, destData);
}

void SynthAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xmlState (getXmlFromBinary (data, sizeInBytes));

    if (xmlState.get() != nullptr)
        if (xmlState->hasTagName (treeState.state.getType()))
            treeState.replaceState (juce::ValueTree::fromXml (*xmlState));
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SynthAudioProcessor();
}

juce::AudioProcessorValueTreeState& SynthAudioProcessor::getTreeState() {
    return treeState;
}

const std::vector<juce::String>& SynthAudioProcessor::getParamNamesAbleToModulate() const {
    return paramNamesAbleToModulate;
}

const std::vector<juce::String>& SynthAudioProcessor::getModulatorNames() const {
    return modulatorNames;
}

juce::AudioProcessorValueTreeState::ParameterLayout SynthAudioProcessor::createLayout() {
    using namespace std;
    using Param = juce::RangedAudioParameter;
    using Param_f = juce::AudioParameterFloat;
    using Param_i = juce::AudioParameterInt;
    using Param_b = juce::AudioParameterBool;
    vector<unique_ptr<Param>> params;

    vector<string> oscillators = { OSC1, OSC2 };

    using namespace params;

    size_t waveTableTypesTotal = audio::WaveTables::getInstance()->getTotal();
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
        paramNamesAbleToModulate.emplace_back(voicesName);
        params.emplace_back(
                make_unique<Param_f> (
                        detune,
                        detune,
                        osc::voices.minValue,
                        osc::voices.maxValue,
                        osc::voices.defaultValue ));
        paramNamesAbleToModulate.emplace_back(detune);
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
        paramNamesAbleToModulate.emplace_back(semitone);
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

        // LFO Modulators
        params.emplace_back(
                make_unique<Param_f>(
                        oscId + LFO_1 + params::osc::level.name,
                        oscId + LFO_1 + params::osc::level.name,
                        0.f,
                        1.f,
                        0.f
                        )
                );
        params.emplace_back(
                make_unique<Param_f>(
                        oscId + LFO_2 + params::osc::level.name,
                        oscId + LFO_2 + params::osc::level.name,
                        0.f,
                        1.f,
                        0.f
                )
        );
        params.emplace_back(
                make_unique<Param_f>(
                        oscId + LFO_1 + params::osc::pan.name,
                        oscId + LFO_1 + params::osc::pan.name,
                        0.f,
                        1.f,
                        0.f
                )
        );
        params.emplace_back(
                make_unique<Param_f>(
                        oscId + LFO_2 + params::osc::pan.name,
                        oscId + LFO_2 + params::osc::pan.name,
                        0.f,
                        1.f,
                        0.f
                )
        );
        params.emplace_back(
                make_unique<Param_f>(
                        oscId + LFO_1 + params::osc::phase.name,
                        oscId + LFO_1 + params::osc::phase.name,
                        0.f,
                        1.f,
                        0.f
                )
        );
        params.emplace_back(
                make_unique<Param_f>(
                        oscId + LFO_2 + params::osc::phase.name,
                        oscId + LFO_2 + params::osc::phase.name,
                        0.f,
                        1.f,
                        0.f
                )
        );
        params.emplace_back(
                make_unique<Param_f>(
                        oscId + LFO_1 + params::osc::fine.name,
                        oscId + LFO_1 + params::osc::fine.name,
                        0.f,
                        1.f,
                        0.f
                )
        );
        params.emplace_back(
                make_unique<Param_f>(
                        oscId + LFO_2 + params::osc::fine.name,
                        oscId + LFO_2 + params::osc::fine.name,
                        0.f,
                        1.f,
                        0.f
                )
        );
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
    for (auto const &filterId : filters) {

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

        string freq = lfoId;
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
    }

    //TODO remove temporary
    params.emplace_back(
            make_unique<Param_f>(
            "lfo",
            "lfo",
            -1.f,
            1.f,
            0.f
            ));

    return {params.begin(), params.end()};
}