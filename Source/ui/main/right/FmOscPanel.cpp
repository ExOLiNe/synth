//
// Created by Aleksandr on 19-Feb-23.
//

#include "FmOscPanel.h"
#include <juce_gui_extra/juce_gui_extra.h>
#include "../../../Typedefs.h"

FmOscPanel::FmOscPanel() {
    setKnob(frequency);
    addAndMakeVisible(frequency);
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
    g.fillAll(juce::Colours::burlywood);
}

void FmOscPanel::resized() {
    juce::Grid grid;
    grid.templateRows = { Track(Fr(1)) };
    grid.templateColumns = {
            Track(Fr(2)),
            Track(Fr(1)),
            Track(Fr(1)),
            Track(Fr(1))
    };
    grid.items = {
            juce::GridItem(selector).withHeight(50.0f),
            juce::GridItem(frequency),
            juce::GridItem(intensity),
            juce::GridItem(enabled).withWidth(100.0f).withHeight(100.0f)
    };
    grid.performLayout(getLocalBounds());
}

void FmOscPanel::setKnob(juce::Slider& knob) {
    knob.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    knob.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 0.0f, 0.0f);
}