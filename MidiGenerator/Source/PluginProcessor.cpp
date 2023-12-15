/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"

//==============================================================================
MidiGeneratorProcessor::MidiGeneratorProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
        : AudioProcessor (BusesProperties()
#if ! JucePlugin_IsMidiEffect
#if ! JucePlugin_IsSynth
                                  .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
#endif
                                  .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
#endif
)
#endif
{
}

MidiGeneratorProcessor::~MidiGeneratorProcessor() = default;

//==============================================================================
const juce::String MidiGeneratorProcessor::getName() const
{
    return JucePlugin_Name;
}

bool MidiGeneratorProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
    return true;
#else
    return false;
#endif
}

bool MidiGeneratorProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
    return true;
#else
    return false;
#endif
}

bool MidiGeneratorProcessor::isMidiEffect() const
{
#if JucePlugin_IsMidiEffect
    return true;
#else
    return false;
#endif
}

double MidiGeneratorProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int MidiGeneratorProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
    // so this should be at least 1, even if you're not really implementing programs.
}

int MidiGeneratorProcessor::getCurrentProgram()
{
    return 0;
}

void MidiGeneratorProcessor::setCurrentProgram (int)
{
}

const juce::String MidiGeneratorProcessor::getProgramName (int)
{
    return {};
}

void MidiGeneratorProcessor::changeProgramName (int, const juce::String&)
{
}

//==============================================================================
void MidiGeneratorProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    //arpeggioIterator = std::make_unique<arpeggiator_type::Iterator>(arpeggiator.getIterator(0));
}

void MidiGeneratorProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool MidiGeneratorProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void MidiGeneratorProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages) {
    playMidi(midiMessages);
    //arpeggiator.tryPlay();
}

void MidiGeneratorProcessor::playMidi(juce::MidiBuffer& midiMessages) {
    auto now = high_resolution_clock::now();
    auto duration = duration_cast<std::chrono::milliseconds>(now - lastMidiNoteTime);
    if (duration.count() > 1000) {
        times++;
        if (times == 2) {
            if (currentChordIndex == 2) {
                currentChordIndex = 0;
            } else {
                currentChordIndex++;
            }
            times = 0;
        }
        lastMidiNote = 40;
        auto noteOffset = chordProgression[currentChordIndex];
        midiMessages.addEvent(juce::MidiMessage::noteOff(1, lastMidiNote + noteOffset), counter);
        midiMessages.addEvent(juce::MidiMessage::noteOff(1, lastMidiNote + noteOffset + 4), counter);
        midiMessages.addEvent(juce::MidiMessage::noteOff(1, lastMidiNote + noteOffset + 7), counter);
        midiMessages.addEvent(juce::MidiMessage::noteOff(1, lastMidiNote + noteOffset + 10), counter);
        lastMidiNoteTime = now;
        counter++;
        midiMessages.addEvent(juce::MidiMessage::noteOn(1, lastMidiNote + noteOffset, (::uint8_t)100), counter);
        midiMessages.addEvent(juce::MidiMessage::noteOn(1, lastMidiNote + noteOffset + 4, (::uint8_t)100), counter);
        midiMessages.addEvent(juce::MidiMessage::noteOn(1, lastMidiNote + noteOffset + 7, (::uint8_t)100), counter);
        midiMessages.addEvent(juce::MidiMessage::noteOn(1, lastMidiNote + noteOffset + 10, (::uint8_t)100), counter);
    }
}

//==============================================================================
bool MidiGeneratorProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* MidiGeneratorProcessor::createEditor()
{
    return nullptr;
}

//==============================================================================
void MidiGeneratorProcessor::getStateInformation (juce::MemoryBlock& destData)
{
}

void MidiGeneratorProcessor::setStateInformation (const void* data, int sizeInBytes)
{
}

/*int MidiGeneratorProcessor::getScaleNote(note_type randomInt) {
    if (notesInArpeggioPlayed == maxNotesInArpeggio) {
        auto noteIndexWithinScale = randomInt % scaleLen;
        auto noteWithinScale = scale[noteIndexWithinScale];
        notesInArpeggioPlayed = 0;
        arpeggioIterator = std::make_unique<arpeggiator_type::Iterator>(arpeggiator.getIterator(noteWithinScale));
    }

    notesInArpeggioPlayed++;
    auto finalNote = **arpeggioIterator;
    ++(*arpeggioIterator);
    return finalNote;
}*/

//std::array<MidiGeneratorProcessor::note_type, MidiGeneratorProcessor::scaleLen> MidiGeneratorProcessor::scale = arpeggiator_type::scale;

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new MidiGeneratorProcessor();
}