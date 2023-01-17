/*
  ==============================================================================

    Filter.cpp
    Created: 12 Jan 2023 12:49:19pm
    Author:  Aleksandr

  ==============================================================================
*/

#include <JuceHeader.h>
#include "FilterComponent.h"

//==============================================================================
FilterComponent::FilterComponent(AudioProcessorValueTreeState& apvts)
{
    cutOffSlider.setSize(100, 100);
    cutOffSlider.setSliderStyle(Slider::SliderStyle::Rotary);
    cutOffSlider.setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxBelow, false, 80, 15);
    addAndMakeVisible(cutOffSlider);

    cutOffLabel.setText("cutOff", NotificationType::dontSendNotification);
    cutOffLabel.setSize(50, 20);
    addAndMakeVisible(cutOffLabel);

    resonanceSlider.setSize(100, 100);
    resonanceSlider.setSliderStyle(Slider::SliderStyle::Rotary);
    resonanceSlider.setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxBelow, false, 80, 15);
    addAndMakeVisible(resonanceSlider);

    resonanceLabel.setText("reso", NotificationType::dontSendNotification);
    resonanceLabel.setSize(50, 20);
    addAndMakeVisible(resonanceLabel);

    mixSlider.setSize(100, 100);
    mixSlider.setSliderStyle(Slider::SliderStyle::Rotary);
    mixSlider.setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxBelow, false, 80, 15);
    addAndMakeVisible(mixSlider);

    mixLabel.setText("mix", NotificationType::dontSendNotification);
    mixLabel.setSize(50, 20);
    addAndMakeVisible(mixLabel);

    setSize(540, 200);

    bindLayoutsToTree(apvts);
}

FilterComponent::~FilterComponent()
{
}

void FilterComponent::bindLayoutsToTree(AudioProcessorValueTreeState& apvts)
{
    cutOffValue = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(apvts, __FILTER_CUTOFF_ID, cutOffSlider);
    resonanceValue = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(apvts, __FILTER_RESONANSE_ID, resonanceSlider);
    mixValue = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(apvts, __FILTER_MIX_ID, mixSlider);
}

void FilterComponent::paint (juce::Graphics& g)
{
}

void FilterComponent::resized()
{
    auto bounds = getLocalBounds();

    auto labelTopOffset = borderSize + cutOffSlider.getHeight();

    cutOffSlider.setBounds(borderSize, 0, cutOffSlider.getWidth(), cutOffSlider.getHeight());
    cutOffLabel.setBounds(borderSize, labelTopOffset, cutOffLabel.getWidth(), cutOffLabel.getHeight());

    resonanceSlider.setBounds(borderSize * 2 + cutOffSlider.getWidth(), 0, resonanceSlider.getWidth(), resonanceSlider.getHeight());
    resonanceLabel.setBounds(borderSize * 2 + cutOffSlider.getWidth(), labelTopOffset, resonanceLabel.getWidth(), resonanceLabel.getHeight());

    mixSlider.setBounds(borderSize * 3 + cutOffSlider.getWidth() * 2, 0, mixSlider.getWidth(), mixSlider.getHeight());
    mixLabel.setBounds(borderSize * 3 + cutOffSlider.getWidth() * 2, labelTopOffset, mixLabel.getWidth(), mixLabel.getHeight());
}
