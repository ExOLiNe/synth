/*
  ==============================================================================

    AdsrPanel.cpp
    Created: 9 Jan 2023 9:45:33pm
    Author:  Aleksandr

  ==============================================================================
*/

#include <JuceHeader.h>
#include "AdsrPanel.h"

//==============================================================================
AdsrPanel::AdsrPanel(std::string id, AudioProcessorValueTreeState& apvts)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    setSize(540, 180);
    attackSlider.setSize(100, 100);
    attackSlider.setSliderStyle(Slider::SliderStyle::Rotary);
    attackSlider.setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxBelow, false, 80, 15);
    addAndMakeVisible(attackSlider);

    attackLabel.setText("Attack", NotificationType::dontSendNotification);
    attackLabel.setSize(50, 25);
    addAndMakeVisible(attackLabel);

    decaySlider.setSize(100, 100);
    decaySlider.setSliderStyle(Slider::SliderStyle::Rotary);
    decaySlider.setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxBelow, false, 80, 15);
    addAndMakeVisible(decaySlider);

    decayLabel.setText("Decay", NotificationType::dontSendNotification);
    decayLabel.setSize(50, 25);
    addAndMakeVisible(decayLabel);

    sustainSlider.setSize(100, 100);
    sustainSlider.setSliderStyle(Slider::SliderStyle::Rotary);
    sustainSlider.setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxBelow, false, 80, 15);
    addAndMakeVisible(sustainSlider);

    sustainLabel.setText("Sustain", NotificationType::dontSendNotification);
    sustainLabel.setSize(50, 20);
    addAndMakeVisible(sustainLabel);

    releaseSlider.setSize(100, 100);
    releaseSlider.setSliderStyle(Slider::SliderStyle::Rotary);
    releaseSlider.setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxBelow, false, 80, 15);
    addAndMakeVisible(releaseSlider);

    releaseLabel.setText("Release", NotificationType::dontSendNotification);
    releaseLabel.setSize(50, 20);
    addAndMakeVisible(releaseLabel);

    bindLayoutsToTree(id, apvts);
}

AdsrPanel::~AdsrPanel()
{
}

void AdsrPanel::bindLayoutsToTree(std::string id, AudioProcessorValueTreeState& apvts)
{
    attackValue = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(
        apvts, id + __ADSR_ATTACK_ID_POSTFIX, attackSlider);

    decayValue = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(
        apvts, id + __ADSR_DECAY_ID_POSTFIX, decaySlider);

    sustainValue = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(
        apvts, id + __ADSR_SUSTAIN_ID_POSTFIX, sustainSlider);

    releaseValue = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(
        apvts, id + __ADSR_RELEASE_ID_POSTFIX, releaseSlider);
}

void AdsrPanel::paint (juce::Graphics& g)
{
    
}

void AdsrPanel::resized()
{
    auto bounds = getLocalBounds();


    attackSlider.setBounds(
        borderSize, 
        borderSize, 
        attackSlider.getWidth(), 
        attackSlider.getHeight());

    attackLabel.setBounds(
        borderSize + attackSlider.getWidth() / 4,
        borderSize + attackSlider.getHeight(),
        attackLabel.getWidth(),
        attackLabel.getHeight()
    );

    decaySlider.setBounds(
        borderSize * 2 + 120,
        borderSize,
        attackSlider.getWidth(),
        attackSlider.getHeight());

    decayLabel.setBounds(
        borderSize * 2 + 120 + decaySlider.getWidth() / 4,
        borderSize + decaySlider.getHeight(),
        decayLabel.getWidth(),
        decayLabel.getHeight()
    );

    sustainSlider.setBounds(
        borderSize * 3 + 120 * 2,
        borderSize,
        sustainSlider.getWidth(),
        sustainSlider.getHeight());

    sustainLabel.setBounds(
        borderSize * 3 + 120 * 2 + sustainSlider.getWidth() / 4,
        borderSize + sustainSlider.getHeight(),
        sustainLabel.getWidth(),
        sustainLabel.getHeight()
    );

    releaseSlider.setBounds(
        borderSize * 4 + 120 * 3,
        borderSize,
        sustainSlider.getWidth(),
        sustainSlider.getHeight());

    releaseLabel.setBounds(
        borderSize * 4 + 120 * 3 + releaseSlider.getWidth() / 4,
        borderSize + releaseSlider.getHeight(),
        releaseLabel.getWidth(),
        releaseLabel.getHeight()
    );
}
