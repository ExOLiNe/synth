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
), treeState(*this, nullptr, "Params", createLayout())
#endif
{
}

SynthAudioProcessor::~SynthAudioProcessor()
{
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
     for (auto const &oscId : oscillators) {
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
                         osc::voices.maxValue,
                         osc::voices.defaultValue
                         ));
         params.emplace_back(
                 make_unique<Param_f> (
                         wtPos,
                         wtPos,
                         osc::wtPos.minValue,
                         osc::wtPos.maxValue,
                         osc::wtPos.defaultValue));
         params.emplace_back(
                 make_unique<Param_i> (
                         pan,
                         pan,
                         osc::pan.minValue,
                         osc::pan.maxValue,
                         osc::pan.defaultValue
                         )
                 );
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
         params.emplace_back(
                 make_unique<Param_f> (
                         fine,
                         fine,
                         osc::fine.minValue,
                         osc::fine.maxValue,
                         osc::fine.defaultValue
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
         params.emplace_back(
                 make_unique<Param_i> (
                         mix,
                         mix,
                         fm::mix.minValue,
                         fm::mix.maxValue,
                         fm::mix.defaultValue
                         )
                 );
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
         params.emplace_back(
                 make_unique<Param_f> (
                         reso,
                         reso,
                         filter::reso.minValue,
                         filter::reso.maxValue,
                         filter::reso.defaultValue
                         )
                 );
         params.emplace_back(
                 make_unique<Param_b> (
                         enabled,
                         enabled,
                         filter::enabled.defaultValue
                         )
                 );
     }

     vector<string> adsrs = { ADSR_VOLUME, ADSR_1, ADSR_2 };
     for (auto const &adsrId : adsrs) {
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

     vector<string> lfos = { LFO_1, LFO_2 };
     for (auto const& lfoId : lfos) {
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
     }



    return {params.begin(), params.end()};
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

void SynthAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String SynthAudioProcessor::getProgramName (int index)
{
    return {};
}

void SynthAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void SynthAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
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
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);

        // ..do something to the data...
    }

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
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void SynthAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
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
