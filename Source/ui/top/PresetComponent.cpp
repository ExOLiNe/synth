//
// Created by Aleksandr on 19-Feb-23.
//

#include "PresetComponent.h"

PresetComponent::PresetComponent() {
    presets.addItem("Preset 1", 1);
    presets.addItem("Preset 2", 2);
    presets.addItem("Preset 3", 3);
    addAndMakeVisible(presets);
}

PresetComponent::~PresetComponent() noexcept {

}

void PresetComponent::paint(juce::Graphics &g) {
    g.fillAll(juce::Colours::black);
    g.setColour(juce::Colours::white);
    g.setFont(15.0f);
    g.drawFittedText("PRESETS", getLocalBounds(), juce::Justification::centred, 1);
}

void PresetComponent::resized() {
    juce::FlexBox fb;
    fb.flexWrap = juce::FlexBox::Wrap::wrap;
    fb.justifyContent = juce::FlexBox::JustifyContent::center;
    fb.alignContent = juce::FlexBox::AlignContent::stretch;
    fb.items.add(juce::FlexItem(presets).withMinWidth(200.0f).withMinHeight(30.0f));
    fb.performLayout(getLocalBounds().toFloat());
}