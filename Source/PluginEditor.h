/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "Oscillator.h"
#include "EnvelopeComponent.h"
#include "FilterComponent.h"


//==============================================================================
/**
*/
class SynthAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    SynthAudioProcessorEditor (SynthAudioProcessor&);
    ~SynthAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    SynthAudioProcessor& audioProcessor;

    Oscillator osc1;
    Oscillator osc2;

    EnvelopeComponent envComponent;

    FilterComponent filterComponent;

    MidiKeyboardState keyboardState;
    MidiKeyboardComponent keyboard;

    int borderSize = 30;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SynthAudioProcessorEditor)
};
