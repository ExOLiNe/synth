/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

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
     ), treeState(*this, nullptr, "PARAMETERS", createLayout())
#endif
{
    analyzers.resize(oscIds.size());
    synthBuffers.resize(oscIds.size());
    for (int j = 0; j < oscIds.size(); j++)
    {
        analyzers.emplace_back();
        synths.add(new Synthesiser());
        synths[j]->clearVoices();

        for (int i = 0; i < 12; i++)
        {
            synths[j]->addVoice(new SynthVoice(oscIds[j], treeState));
        }

        synths[j]->clearSounds();
        synths[j]->addSound(new SynthSound());
    }
}

SynthAudioProcessor::~SynthAudioProcessor()
{

}

AudioProcessorValueTreeState::ParameterLayout SynthAudioProcessor::createLayout()
{
    vector<unique_ptr<RangedAudioParameter>> params;
    vector<string> oscillators = { OSC1_ID, OSC2_ID };
    vector<string> adsrEnvelopes{ ADSR1_ID };

    for (auto oscId : oscillators)
    {
        params.push_back(
            make_unique<AudioParameterFloat>(
                oscId + __OSC_DETUNE_ID_POSTFIX, 
                oscId + __OSC_DETUNE_ID_POSTFIX,
                __OSC_DETUNE_MIN_VALUE,
                __OSC_DETUNE_MAX_VALUE,
                __OSC_DETUNE_DEFAULT
                )
        );

        params.push_back(
            make_unique<AudioParameterFloat>(
                oscId + __OSC_PHASE_ID_POSTFIX,
                oscId + __OSC_PHASE_ID_POSTFIX,
                __OSC_PHASE_MIN_VALUE,
                __OSC_PHASE_MAX_VALUE,
                __OSC_PHASE_DEFAULT
                )
        );

        if (oscId == OSC2_ID)
        {
            params.push_back(
                make_unique<AudioParameterFloat>(
                    oscId + __OSC_VOLUME_ID_POSTFIX,
                    oscId + __OSC_VOLUME_ID_POSTFIX,
                    __OSC_VOLUME_MIN_VALUE,
                    __OSC_VOLUME_MAX_VALUE,
                    __OSC_VOLUME_MIN_VALUE
                    )
            );
        }
        else {
            params.push_back(
                make_unique<AudioParameterFloat>(
                    oscId + __OSC_VOLUME_ID_POSTFIX,
                    oscId + __OSC_VOLUME_ID_POSTFIX,
                    __OSC_VOLUME_MIN_VALUE,
                    __OSC_VOLUME_MAX_VALUE,
                    __OSC_VOLUME_DEFAULT
                    )
            );
        }

        params.push_back(
            make_unique<AudioParameterFloat>(
                oscId + __OSC_VOICES_ID_POSTFIX,
                oscId + __OSC_VOICES_ID_POSTFIX,
                __OSC_VOICES_MIN_VALUE,
                __OSC_VOICES_MAX_VALUE,
                __OSC_VOICES_DEFAULT
                )
        );


        params.push_back(
            make_unique<AudioParameterFloat>(
                oscId + __ADSR_ATTACK_ID_POSTFIX,
                oscId + __ADSR_ATTACK_ID_POSTFIX,
                __ADSR_ATTACK_MIN_VALUE,
                __ADSR_ATTACK_MAX_VALUE,
                __ADSR_ATTACK_DEFAULT
                )
        );

        params.push_back(
            make_unique<AudioParameterFloat>(
                oscId + __ADSR_DECAY_ID_POSTFIX,
                oscId + __ADSR_DECAY_ID_POSTFIX,
                __ADSR_DECAY_MIN_VALUE,
                __ADSR_DECAY_MAX_VALUE,
                __ADSR_DECAY_DEFAULT
                )
        );

        params.push_back(
            make_unique<AudioParameterFloat>(
                oscId + __ADSR_SUSTAIN_ID_POSTFIX,
                oscId + __ADSR_SUSTAIN_ID_POSTFIX,
                __ADSR_SUSTAIN_MIN_VALUE,
                __ADSR_SUSTAIN_MAX_VALUE,
                __ADSR_SUSTAIN_DEFAULT
                )
        );

        params.push_back(
            make_unique<AudioParameterFloat>(
                oscId + __ADSR_RELEASE_ID_POSTFIX,
                oscId + __ADSR_RELEASE_ID_POSTFIX,
                __ADSR_RELEASE_MIN_VALUE,
                __ADSR_RELEASE_MAX_VALUE,
                __ADSR_RELEASE_DEFAULT
                )
        );
        params.push_back(
            make_unique<AudioParameterInt>(
                oscId + __WAVE_SELECTOR_ID_POSTFIX,
                oscId + __WAVE_SELECTOR_ID_POSTFIX,
                WaveType::SINE,
                WaveType::NOISE,
                __WAVE_SELECTOR_DEFAULT_WAVE
                )
        );
    }

    params.push_back(
        make_unique<AudioParameterFloat>(
            __FILTER_CUTOFF_ID,
            __FILTER_CUTOFF_ID,
            __FILTER_CUTOFF_MIN_VALUE,
            __FILTER_CUTOFF_MAX_VALUE,
            __FILTER_CUTOFF_DEFAULT
            )
    );

    params.push_back(
        make_unique<AudioParameterFloat>(
            __FILTER_RESONANSE_ID,
            __FILTER_RESONANSE_ID,
            __FILTER_RESONANSE_MIN_VALUE,
            __FILTER_RESONANSE_MAX_VALUE,
            __FILTER_RESONANSE_DEFAULT
            )
    );

    params.push_back(
        make_unique<AudioParameterFloat>(
            __FILTER_MIX_ID,
            __FILTER_MIX_ID,
            __FILTER_MIX_MIN_VALUE,
            __FILTER_MIX_MAX_VALUE,
            __FILTER_MIX_DEFAULT
            )
    );

    const auto mod = string(MOD);
    //MOD
    params.push_back(
        make_unique<AudioParameterFloat>(
            mod + __ADSR_ATTACK_ID_POSTFIX,
            mod + __ADSR_ATTACK_ID_POSTFIX,
            __ADSR_ATTACK_MIN_VALUE,
            __ADSR_ATTACK_MAX_VALUE,
            __ADSR_ATTACK_DEFAULT
            )
    );

    params.push_back(
        make_unique<AudioParameterFloat>(
            mod + __ADSR_DECAY_ID_POSTFIX,
            mod + __ADSR_DECAY_ID_POSTFIX,
            __ADSR_DECAY_MIN_VALUE,
            __ADSR_DECAY_MAX_VALUE,
            __ADSR_DECAY_DEFAULT
            )
    );

    params.push_back(
        make_unique<AudioParameterFloat>(
            mod + __ADSR_SUSTAIN_ID_POSTFIX,
            mod + __ADSR_SUSTAIN_ID_POSTFIX,
            __ADSR_SUSTAIN_MIN_VALUE,
            __ADSR_SUSTAIN_MAX_VALUE,
            __ADSR_SUSTAIN_DEFAULT
            )
    );

    params.push_back(
        make_unique<AudioParameterFloat>(
            mod + __ADSR_RELEASE_ID_POSTFIX,
            mod + __ADSR_RELEASE_ID_POSTFIX,
            __ADSR_RELEASE_MIN_VALUE,
            __ADSR_RELEASE_MAX_VALUE,
            __ADSR_RELEASE_DEFAULT
            )
    );

    return { params.begin(), params.end() };
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
    ignoreUnused(samplesPerBlock);

    for (int j = 0; j < synths.size(); j++)
    {
        synthBuffers[j].setSize(getNumOutputChannels(), samplesPerBlock);

        synths[j]->setCurrentPlaybackSampleRate(sampleRate);

        for (int i = 0; i < synths[j]->getNumVoices(); i++)
        {
            ((SynthVoice*)synths[j]->getVoice(i))->prepareToPlay(sampleRate, samplesPerBlock, getNumOutputChannels());
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
    for (int i = 0; i < synths.size(); i++)
    {
        synthBuffers[i].clear();
        synthBuffers[i].setSize(buffer.getNumChannels(), buffer.getNumSamples());
        synths[i]->renderNextBlock(synthBuffers[i], midiMessages, 0, buffer.getNumSamples());
        analyzers[i].processBlock(synthBuffers[i]);
    }

    for (int channel = 0; channel < buffer.getNumChannels(); ++channel)
    {
        auto writePtr = buffer.getWritePointer(channel);
        for (int j = 0; j < synthBuffers.size(); j++)
        {
            auto readPtr = synthBuffers[j].getReadPointer(channel);
            LOG_JUMPS(readPtr, channel, buffer.getNumSamples(), 6);
            for (int i = 0; i < buffer.getNumSamples(); ++i)
            {
                writePtr[i] += readPtr[i];
            }
        }
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
