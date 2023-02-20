//
// Created by Aleksandr on 19-Feb-23.
//

#include "EnvPanel.h"

EnvPanel::EnvPanel() {
    slider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    slider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 0.0f, 0.0f);
    addAndMakeVisible(slider);
}

EnvPanel::~EnvPanel() noexcept {

}

void EnvPanel::paint(juce::Graphics &g) {
    g.fillAll(juce::Colours::blanchedalmond);
    g.setColour(juce::Colours::white);
    g.setFont(15.0f);
    g.drawFittedText("Env panel!", getLocalBounds(), juce::Justification::centred, 1);
}

void EnvPanel::resized() {
    juce::FlexBox fb;
    fb.flexWrap = juce::FlexBox::Wrap::wrap;
    fb.justifyContent = juce::FlexBox::JustifyContent::center;
    fb.alignContent = juce::FlexBox::AlignContent::stretch;
    fb.items.add(juce::FlexItem(slider).withMinWidth(200.0f).withMinHeight(200.0f));
    fb.performLayout(getLocalBounds().toFloat());
}