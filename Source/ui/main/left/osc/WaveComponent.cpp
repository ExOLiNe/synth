//
// Created by Aleksandr on 19-Feb-23.
//

#include "WaveComponent.h"
#include "../../../../other/Grid.h"

WaveComponent::WaveComponent() {
    wave.setData(getSinWaveTable());
    wave.setZHighlight(4);
    addAndMakeVisible(wave);
}

WaveComponent::~WaveComponent() noexcept {

}

void WaveComponent::paint(juce::Graphics &g) {
    g.fillAll(juce::Colour::fromString("0xff191919"));
}

void WaveComponent::resized() {
    Grid grid({ 1 }, { 1, 1});
    grid.setItems({&wave});
    grid.performLayout(getLocalBounds());
}