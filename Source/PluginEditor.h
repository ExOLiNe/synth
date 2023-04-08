/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <juce_gui_basics.h>
#include "PluginProcessor.h"
#include "ui/TopPanel.h"
#include "ui/BodyPanel.h"
#include "other/Grid.h"

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
    ui::TopPanel topPanel;
    ui::BodyPanel bodyPanel;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SynthAudioProcessorEditor)
};
