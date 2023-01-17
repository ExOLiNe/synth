/*
  ==============================================================================

    OscControlJPanel.cpp
    Created: 7 Jan 2023 3:13:21pm
    Author:  Aleksandr

  ==============================================================================
*/

#include <JuceHeader.h>
#include "OscControlJPanel.h"

//==============================================================================
OscControlJPanel::OscControlJPanel(const string& id, AudioProcessorValueTreeState& apvts)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    setSize(524, 206);

    detune.setSize(100, 100);
    detune.setSliderStyle(Slider::SliderStyle::Rotary);
    detune.setHelpText("detune");
    detune.setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxBelow, false, 80, 15);
    addAndMakeVisible(detune);

    detuneLabel.setText("Detune", NotificationType::dontSendNotification);
    detuneLabel.setSize(50, 20);
    addAndMakeVisible(detuneLabel);

    phase.setSize(100, 100);
    phase.setSliderStyle(Slider::SliderStyle::Rotary);
    phase.setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxBelow, false, 80, 15);
    addAndMakeVisible(phase);

    phaseLabel.setText("Phase", NotificationType::dontSendNotification);
    phaseLabel.setSize(50, 20);
    addAndMakeVisible(phaseLabel);

    voices.setSize(50, 30);
    for (int i = __OSC_VOICES_MIN_VALUE; i <= __OSC_VOICES_MAX_VALUE; i++)
    {
        voices.addItem(String(i), i);
    }
    voices.setSelectedId(__OSC_VOICES_DEFAULT);
    addAndMakeVisible(voices);

    voicesLabel.setText("voices", NotificationType::dontSendNotification);
    voicesLabel.setSize(50, 20);
    addAndMakeVisible(voicesLabel);

    level.setSize(100, 100);
    level.setSliderStyle(Slider::SliderStyle::Rotary);
    level.setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxBelow, false, 80, 15);
    addAndMakeVisible(level);

    levelLabel.setText("Level", NotificationType::dontSendNotification);
    levelLabel.setSize(50, 20);
    addAndMakeVisible(levelLabel);


    bindLayoutsToTree(id, apvts);
}

OscControlJPanel::~OscControlJPanel()
{
}

void OscControlJPanel::bindLayoutsToTree(const string& id, AudioProcessorValueTreeState& apvts)
{
    detuneValue = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(
        apvts, id + __OSC_DETUNE_ID_POSTFIX, detune);
    phaseValue = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(
        apvts, id + __OSC_PHASE_ID_POSTFIX, phase);
    levelValue = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(
        apvts, id + __OSC_VOLUME_ID_POSTFIX, level);
    voicesValue = std::make_unique<AudioProcessorValueTreeState::ComboBoxAttachment>(
        apvts, id + __OSC_VOICES_ID_POSTFIX, voices);

}

void OscControlJPanel::paint (juce::Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */
}

void OscControlJPanel::resized()
{
    auto bottomButtonLabelOffset = 182;
    auto bounds = getLocalBounds();

    voices.setBounds(borderSize, borderSize, voices.getWidth(), voices.getHeight());
    voicesLabel.setBounds(borderSize, borderSize + 32, voicesLabel.getWidth(), voicesLabel.getHeight());

    detune.setBounds(borderSize, bounds.getHeight() / 2 - 20, detune.getWidth(), detune.getHeight());
    detuneLabel.setBounds(borderSize + 22, bottomButtonLabelOffset, detuneLabel.getWidth(), detuneLabel.getHeight());
    
    phase.setBounds(bounds.getWidth() / 3, borderSize, phase.getWidth(), phase.getHeight());
    phaseLabel.setBounds(bounds.getWidth() / 3 + 24, borderSize + 98, phaseLabel.getWidth(), phaseLabel.getHeight());

    level.setBounds(bounds.getWidth() / 3 * 2, bounds.getHeight() / 2 - 20, level.getWidth(), level.getHeight());
    levelLabel.setBounds(bounds.getWidth() / 3 * 2 + 26, bottomButtonLabelOffset, levelLabel.getWidth(), levelLabel.getHeight());
}
