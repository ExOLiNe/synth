//
// Created by Aleksandr on 19-Feb-23.
//

#include "ADSRPanel.h"
#include "../../../../../Typedefs.h"

ADSRPanel::ADSRPanel() {

    attackLabel.setText("Attack", juce::NotificationType::dontSendNotification);
    attackLabel.setJustificationType(juce::Justification::centredRight);
    addAndMakeVisible(attackLabel);

    attack.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    attack.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    addAndMakeVisible(attack);

    decayLabel.setText("Decay", juce::NotificationType::dontSendNotification);
    decayLabel.setJustificationType(juce::Justification::centredRight);
    addAndMakeVisible(decayLabel);

    decay.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    decay.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    addAndMakeVisible(decay);

    sustainLabel.setText("Sustain", juce::NotificationType::dontSendNotification);
    sustainLabel.setJustificationType(juce::Justification::centredRight);
    addAndMakeVisible(sustainLabel);

    sustain.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    sustain.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    addAndMakeVisible(sustain);

    releaseLabel.setText("Release", juce::NotificationType::dontSendNotification);
    releaseLabel.setJustificationType(juce::Justification::centredRight);
    addAndMakeVisible(releaseLabel);

    release.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    release.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    addAndMakeVisible(release);

    wave.setDefaultValues(attack, decay, sustain, release);
    addAndMakeVisible(wave);

}

ADSRPanel::~ADSRPanel() noexcept {

}

void ADSRPanel::paint(juce::Graphics &g) {

}

void ADSRPanel::resized() {
    using namespace juce;
    Grid grid;
    grid.templateRows = { Track(Fr(4)), Track(Fr(1)) };
    grid.templateColumns = {
            Track(Fr(1)),
            Track(Fr(1)),
            Track(Fr(1)),
            Track(Fr(1)),
            Track(Fr(1)),
            Track(Fr(1)),
            Track(Fr(1)),
            Track(Fr(1))
    };
    grid.items = {
            GridItem(wave).withArea(
                    GridItem::Property(1),
                    GridItem::Property(1),
                    GridItem::Property(1),
                    GridItem::Property(9)
            ),
            GridItem(attackLabel),
            GridItem(attack),
            GridItem(decayLabel),
            GridItem(decay),
            GridItem(sustainLabel),
            GridItem(sustain),
            GridItem(releaseLabel),
            GridItem(release)
    };
    grid.performLayout(getLocalBounds());
}