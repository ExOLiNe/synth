/*
  ==============================================================================

    Oscillator.cpp
    Created: 7 Jan 2023 3:12:57pm
    Author:  Aleksandr

  ==============================================================================
*/

#include <JuceHeader.h>
#include "Oscillator.h"

//==============================================================================
Oscillator::Oscillator(std::string name, std::string id, 
    AudioProcessorValueTreeState& apvts, SpectrumAnalyzer& analyzer)
    : name(name), 
    controlPanel(id, apvts),
    waveTable(analyzer)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    setSize(540, 500);
    addAndMakeVisible(controlPanel);
    addAndMakeVisible(waveTable);

    waveSelector.addItem("sine", WaveType::SINE);
    waveSelector.addItem("square", WaveType::SQUARE);
    waveSelector.addItem("triangle", WaveType::TRIANGLE);
    waveSelector.addItem("saw", WaveType::SAW);
    waveSelector.addItem("noise", WaveType::NOISE);
    waveSelector.setSize(524, 40);
    addAndMakeVisible(waveSelector);

    bindLayoutsToTree(id, apvts);
}

Oscillator::~Oscillator()
{
}

void Oscillator::bindLayoutsToTree(std::string id, AudioProcessorValueTreeState& apvts)
{
    waveSelectorAttachment = make_unique<AudioProcessorValueTreeState::ComboBoxAttachment>(
        apvts, id + __WAVE_SELECTOR_ID_POSTFIX, waveSelector);

    waveSelectorAttachment;
}

/*void Oscillator::comboBoxChanged(ComboBox* comboBoxThatHasChanged)
{
    Logger::getCurrentLogger()->writeToLog("I have changed!");
}*/

void Oscillator::paint (juce::Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (juce::Colours::white);
    g.setFont (14.0f);
    g.drawText (name, getLocalBounds(),
                juce::Justification::centred, true);   // draw some placeholder text
}

void Oscillator::resized()
{
    auto bounds = getLocalBounds();

    waveSelector.setBounds(borderSize, borderSize, waveSelector.getWidth(), waveSelector.getHeight());

    waveTable.setBounds(borderSize, borderSize * 2 + waveSelector.getHeight(), waveTable.getWidth(), waveTable.getHeight());

    controlPanel.setBounds(borderSize, bounds.getHeight() - borderSize - controlPanel.getHeight(), controlPanel.getWidth(), controlPanel.getHeight());
}
