//
// Created by Aleksandr on 19-Feb-23.
//

#include "LogoComponent.h"

namespace ui {
    LogoComponent::LogoComponent() {
    }

    LogoComponent::~LogoComponent() noexcept {

    }

    void LogoComponent::paint(juce::Graphics &g) {
        g.fillAll(juce::Colours::black);
        g.setColour(juce::Colours::white);
        g.setFont(15.0f);
        g.drawFittedText("LOGO", getLocalBounds(), juce::Justification::centred, 1);
    }

    void LogoComponent::resized() {
        /*juce::FlexBox fb;
        fb.flexWrap = juce::FlexBox::Wrap::wrap;
        fb.justifyContent = juce::FlexBox::JustifyContent::center;
        fb.alignContent = juce::FlexBox::AlignContent::stretch;
        fb.items.add(juce::FlexItem(voices).withMinWidth(200.0f).withMinHeight(200.0f));
        fb.performLayout(getLocalBounds().toFloat());*/
    }
}