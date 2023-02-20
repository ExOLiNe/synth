//
// Created by Aleksandr on 19-Feb-23.
//

#include "FilterPanel.h"
#include "../../../Typedefs.h"

FilterPanel::FilterPanel() {
    setKnob(frequency);
    addAndMakeVisible(frequency);
    setKnob(resonance);
    addAndMakeVisible(resonance);
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
            Track(Fr(1))
    };
    grid.items = {
            GridItem(selector).withHeight(50.0f),
            GridItem(frequency),
            GridItem(resonance),
            GridItem(mix)
    };
    grid.performLayout(getLocalBounds());
}

void FilterPanel::setKnob(juce::Slider& knob) {
    knob.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    knob.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 0.0f, 0.0f);
}