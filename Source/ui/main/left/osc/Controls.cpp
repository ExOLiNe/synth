//
// Created by Aleksandr on 19-Feb-23.
//

#include "Controls.h"
#include "../../../../other/Grid.h"

Controls::Controls() {
    setKnob(voices);
    addAndMakeVisible(voices);
    setKnob(detune);
    addAndMakeVisible(detune);
    setKnob(phase);
    addAndMakeVisible(phase);
    setKnob(wtPos);
    addAndMakeVisible(wtPos);
    setKnob(pan);
    addAndMakeVisible(pan);
    setKnob(level);
    addAndMakeVisible(level);
    setKnob(semitone);
    addAndMakeVisible(semitone);
    setKnob(fine);
    addAndMakeVisible(fine);

}

Controls::~Controls() noexcept {

}

void Controls::paint(juce::Graphics &g) {
    g.fillAll(juce::Colours::cyan);
    g.setColour(juce::Colours::white);
    g.setFont(15.0f);
    g.drawFittedText("Controls", getLocalBounds(), juce::Justification::centred, 1);
}

void Controls::resized() {
    float minSize = 80.0f;

    Grid grid({1, 1, 1, 1}, {1, 1});

    grid.setItems({&detune, &voices, &phase, &wtPos, &level, &pan, &semitone, &fine});
    grid.performLayout(getLocalBounds());

    /*juce::FlexBox fb;
    fb.flexWrap = juce::FlexBox::Wrap::wrap;
    fb.justifyContent = juce::FlexBox::JustifyContent::center;
    fb.alignContent = juce::FlexBox::AlignContent::stretch;
    fb.items = {
            juce::FlexItem(detune).withMinWidth(minSize).withMinHeight(minSize),
            juce::FlexItem(voices).withMinWidth(minSize).withMinHeight(minSize),
            juce::FlexItem(phase).withMinWidth(minSize).withMinHeight(minSize),
            juce::FlexItem(wtPos).withMinWidth(minSize).withMinHeight(minSize),
            juce::FlexItem(level).withMinWidth(minSize).withMinHeight(minSize),
            juce::FlexItem(pan).withMinWidth(minSize).withMinHeight(minSize),
            juce::FlexItem(semitone).withMinWidth(minSize).withMinHeight(minSize),
            juce::FlexItem(fine).withMinWidth(minSize).withMinHeight(minSize),
    };
    fb.performLayout(getLocalBounds().toFloat());*/
}

void Controls::setKnob(juce::Slider &knob) {
    knob.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    knob.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 0.0f, 0.0f);
}