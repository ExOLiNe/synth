//
// Created by Aleksandr on 19-Feb-23.
//

#include "TopPanel.h"
#include "../other/Grid.h"

TopPanel::TopPanel() {
    addAndMakeVisible(logoComponent);
    addAndMakeVisible(menuComponent);
    addAndMakeVisible(presetComponent);
}

TopPanel::~TopPanel() noexcept {

}

void TopPanel::paint(juce::Graphics &g) {
    g.fillAll(juce::Colours::black);
}

void TopPanel::resized() {
    /*juce::FlexBox fb;
    fb.flexWrap = juce::FlexBox::Wrap::wrap;
    fb.justifyContent = juce::FlexBox::JustifyContent::center;
    fb.alignContent = juce::FlexBox::AlignContent::stretch;
    fb.items.add(juce::FlexItem(voices).withMinWidth(200.0f).withMinHeight(200.0f));
    fb.performLayout(getLocalBounds().toFloat());*/
    Grid grid({ 1 }, { 1, 2, 3});
    grid.setItems({&logoComponent, &menuComponent, &presetComponent});
    grid.performLayout(getLocalBounds());
}