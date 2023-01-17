/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "Params.h"

//==============================================================================
SynthAudioProcessorEditor::SynthAudioProcessorEditor (SynthAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p), 
    osc1(OSC1_ID, OSC1_NAME, audioProcessor.treeState, audioProcessor.analyzers[0]),
    osc2(OSC2_ID, OSC2_NAME, audioProcessor.treeState, audioProcessor.analyzers[1]),
    keyboard(keyboardState, MidiKeyboardComponent::horizontalKeyboard),
    envComponent(OSC1_ID, OSC2_ID, audioProcessor.treeState),
    filterComponent(audioProcessor.treeState)
{
    // keyboardState.addListener(audioProcessor);
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (1200, 900);
    addAndMakeVisible(osc1);
    addAndMakeVisible(osc2);

    addAndMakeVisible(envComponent);

    addAndMakeVisible(filterComponent);

    keyboard.setSize(getLocalBounds().getWidth(), 100);
    addAndMakeVisible(keyboard);
}

SynthAudioProcessorEditor::~SynthAudioProcessorEditor()
{
}

//==============================================================================
void SynthAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (15.0f);
}

void SynthAudioProcessorEditor::resized()
{
    auto bounds = getLocalBounds();
    osc1.setBounds(borderSize, borderSize, osc1.getWidth(), osc1.getHeight());

    osc2.setBounds(bounds.getWidth() - borderSize - osc2.getWidth(), borderSize, osc2.getWidth(), osc2.getHeight());

    envComponent.setBounds(borderSize, osc1.getHeight() + borderSize * 2, envComponent.getWidth(), envComponent.getHeight());

    filterComponent.setBounds(bounds.getWidth() - filterComponent.getWidth() - borderSize, osc1.getHeight() + borderSize * 2, filterComponent.getWidth(), filterComponent.getHeight());

    keyboard.setBounds(0, bounds.getHeight() - keyboard.getHeight() - 100, bounds.getWidth(), 200);
}
