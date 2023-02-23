//
// Created by Aleksandr on 19-Feb-23.
//

#include "FmOscPanel.h"
#include <juce_gui_extra/juce_gui_extra.h>
#include "../../../Typedefs.h"

FmOscPanel::FmOscPanel() {
    frequencyLabel.setText("Freq", juce::NotificationType::dontSendNotification);
    frequencyLabel.setJustificationType(juce::Justification::centredRight);
    addAndMakeVisible(frequencyLabel);

    setKnob(frequency);
    addAndMakeVisible(frequency);

    intensityLabel.setText("Reso", juce::NotificationType::dontSendNotification);
    intensityLabel.setJustificationType(juce::Justification::centredRight);
    addAndMakeVisible(intensityLabel);

    setKnob(intensity);
    addAndMakeVisible(intensity);

    addAndMakeVisible(enabled);

    selector.setSize(50.0f, 10.0f);
    selector.addItem("A", 1);
    selector.addItem("B", 2);
    addAndMakeVisible(selector);
}

FmOscPanel::~FmOscPanel() noexcept {

}

void FmOscPanel::paint(juce::Graphics &g) {
    g.fillAll(juce::Colour::fromString("0xff191919"));
}

void FmOscPanel::resized() {
    using namespace juce;
    Grid grid;
    grid.templateRows = { Track(Fr(1)) };
    grid.templateColumns = {
            Track(Fr(2)),
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
            GridItem(intensityLabel),
            GridItem(intensity),
            GridItem(enabled).withWidth(100.0f).withHeight(100.0f)
    };
    grid.performLayout(getLocalBounds());
}

void FmOscPanel::setKnob(juce::Slider& knob) {
    knob.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    knob.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 0.0f, 0.0f);
}