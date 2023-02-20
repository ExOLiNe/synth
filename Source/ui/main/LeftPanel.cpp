//
// Created by Aleksandr on 19-Feb-23.
//

#include "LeftPanel.h"
#include "../../other/Grid.h"

LeftPanel::LeftPanel() {
    addAndMakeVisible(oscPanelA);
    addAndMakeVisible(oscPanelB);
}

LeftPanel::~LeftPanel() noexcept {

}

void LeftPanel::paint(juce::Graphics &g) {
    g.fillAll(juce::Colours::black);
    g.setColour(juce::Colours::white);
    g.setFont(15.0f);
    g.drawFittedText("Left panel!", getLocalBounds(), juce::Justification::centred, 1);
}

void LeftPanel::resized() {
    /*juce::FlexBox fb;
    fb.flexWrap = juce::FlexBox::Wrap::wrap;
    fb.justifyContent = juce::FlexBox::JustifyContent::center;
    fb.alignContent = juce::FlexBox::AlignContent::stretch;
    fb.items.add(juce::FlexItem(voices).withMinWidth(200.0f).withMinHeight(200.0f));
    fb.performLayout(getLocalBounds().toFloat());*/

    Grid grid({1, 1}, {1});
    grid.setItems({&oscPanelA, &oscPanelB});
    grid.performLayout(getLocalBounds());
}