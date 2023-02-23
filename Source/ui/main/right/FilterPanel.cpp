//
// Created by Aleksandr on 19-Feb-23.
//

#include "FilterPanel.h"
#include "../../../Typedefs.h"

FilterPanel::FilterPanel() {
    frequencyLabel.setText("Freq", juce::NotificationType::dontSendNotification);
    frequencyLabel.setJustificationType(juce::Justification::centredRight);
    addAndMakeVisible(frequencyLabel);

    setKnob(frequency);
    addAndMakeVisible(frequency);

    resonanceLabel.setText("Reso", juce::NotificationType::dontSendNotification);
    resonanceLabel.setJustificationType(juce::Justification::centredRight);
    addAndMakeVisible(resonanceLabel);

    setKnob(resonance);
    addAndMakeVisible(resonance);

    mixLabel.setText("Mix", juce::NotificationType::dontSendNotification);
    mixLabel.setJustificationType(juce::Justification::centredRight);
    addAndMakeVisible(mixLabel);

    setKnob(mix);
    addAndMakeVisible(mix);

    addAndMakeVisible(enabled);

    selector.addItem("A", 1);
    selector.addItem("B", 2);
    addAndMakeVisible(selector);
}

FilterPanel::~FilterPanel() noexcept {

}

void FilterPanel::paint(juce::Graphics &g) {
    g.fillAll(juce::Colour::fromString("0xff191919"));
}

void FilterPanel::resized() {
    using namespace juce;
    Grid grid;
    grid.templateRows = { Track(Fr(1)) };
    grid.templateColumns = {
            Track(Fr(2)),
            Track(Fr(1)),
            Track(Fr(1)),
            Track(Fr(1)),
            Track(Fr(1)),
            Track(Fr(1)),
            Track(Fr(1))
    };
    grid.items = {
            GridItem(selector).withHeight(50.0f)
                    .withAlignSelf(juce::GridItem::AlignSelf::center),
                    GridItem(frequencyLabel),
                    GridItem(frequency),
                    GridItem(resonanceLabel),
                    GridItem(resonance),
                    GridItem(mixLabel),
                    GridItem(mix)
    };
    grid.performLayout(getLocalBounds());
}

void FilterPanel::setKnob(juce::Slider& knob) {
    knob.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    knob.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 0.0f, 0.0f);
}