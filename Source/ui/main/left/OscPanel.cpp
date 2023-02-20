//
// Created by Aleksandr on 19-Feb-23.
//

#include "OscPanel.h"
#include "../../../other/Grid.h"

OscPanel::OscPanel() {
    addAndMakeVisible(wave);
    addAndMakeVisible(controls);
}

OscPanel::~OscPanel() noexcept {

}

void OscPanel::paint(juce::Graphics &g) {
    g.fillAll(juce::Colours::chocolate);
    g.setColour(juce::Colours::white);
    g.setFont(15.0f);
    g.drawFittedText("Osc panel!", getLocalBounds(), juce::Justification::centred, 1);
}

void OscPanel::resized() {
    /*juce::FlexBox fb;
    fb.flexWrap = juce::FlexBox::Wrap::wrap;
    fb.justifyContent = juce::FlexBox::JustifyContent::center;
    fb.alignContent = juce::FlexBox::AlignContent::stretch;
    fb.items.add(juce::FlexItem(voices).withMinWidth(200.0f).withMinHeight(200.0f));
    fb.performLayout(getLocalBounds().toFloat());*/
    Grid grid({ 1 }, { 2, 1 });
    grid.setItems({&wave, &controls});
    grid.performLayout(getLocalBounds());
}