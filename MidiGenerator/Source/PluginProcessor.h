/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include "juce_audio_processors/juce_audio_processors.h"
#include <chrono>
#include <random>
//#include "Arpeggiator.h"
//#include "juce_audio_basics/audio_play_head/juce_AudioPlayHead.h"

using namespace std::chrono;

//==============================================================================
/**
*/
class MidiGeneratorProcessor  : public juce::AudioProcessor
#if JucePlugin_Enable_ARA
    , public juce::AudioProcessorARAExtension
#endif
{
public:
    //==============================================================================
    MidiGeneratorProcessor();
    ~MidiGeneratorProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

#ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
#endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
private:
    void playMidi(juce::MidiBuffer& midiMessages);
    using note_type = unsigned int;
/*  using arpeggiator_type = Arpeggiator<note_type>;
    arpeggiator_type arpeggiator;
    static constexpr unsigned int maxNotesInArpeggio = 4;
    unsigned int notesInArpeggioPlayed = 0;
    static constexpr unsigned int scaleLen = arpeggiator_type::notesLen;
    static std::array<note_type, scaleLen> scale;
    std::unique_ptr<arpeggiator_type::Iterator> arpeggioIterator;
    int getScaleNote(note_type randomInt);
    */
    time_point<high_resolution_clock> lastMidiNoteTime = high_resolution_clock::now();
    note_type lastMidiNote = 1;
    long long counter = 0L;
    int times = 0;
    int currentChordIndex = 0;
    std::array<int, 3> chordProgression = {0, 5, 7};
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MidiGeneratorProcessor)
};
