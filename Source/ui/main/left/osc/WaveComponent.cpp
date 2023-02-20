//
// Created by Aleksandr on 19-Feb-23.
//

#include "WaveComponent.h"
#include "../../../../other/Grid.h"

WaveComponent::WaveComponent() {
    addAndMakeVisible(wave);
}

WaveComponent::~WaveComponent() noexcept {

}

void WaveComponent::paint(juce::Graphics &g) {
    g.fillAll(juce::Colours::chocolate);
    g.setColour(juce::Colours::white);
    g.setFont(15.0f);
    g.drawFittedText("WaveComponent", getLocalBounds(), juce::Justification::centred, 1);
}

void WaveComponent::resized() {
    Grid grid({ 1 }, { 1, 1});
    grid.setItems({&wave});
    grid.performLayout(getLocalBounds());
}