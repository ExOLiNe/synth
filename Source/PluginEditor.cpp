/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SynthAudioProcessorEditor::SynthAudioProcessorEditor(SynthAudioProcessor &p)
        : AudioProcessorEditor(&p), audioProcessor(p) {
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize(1200, 700);
    setResizable(true, true);
    addAndMakeVisible(topPanel);
    addAndMakeVisible(bodyPanel);
}

SynthAudioProcessorEditor::~SynthAudioProcessorEditor() {
}

//==============================================================================
void SynthAudioProcessorEditor::paint(juce::Graphics &g) {
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
}

void SynthAudioProcessorEditor::resized() {
    Grid grid({1, 8}, { 1 });
    grid.setItems({ &topPanel, &bodyPanel });
    grid.performLayout(getLocalBounds());
}
