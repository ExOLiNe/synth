//
// Created by Aleksandr on 19-Feb-23.
//

#include "WaveComponent.h"
#include "../../../../other/Grid.h"

WaveComponent::WaveComponent() {
    wave.setData(getSinWaveTable());
    wave.setZHighlight(4);
    addAndMakeVisible(wave);

    selector.addItem("sin", 1);
    selector.addItem("square", 2);
    selector.addItem("triangle", 3);
    selector.addItem("saw", 4);
    selector.setSelectedId(1, juce::NotificationType::dontSendNotification);
    addAndMakeVisible(selector);
}

WaveComponent::~WaveComponent() noexcept {

}

void WaveComponent::paint(juce::Graphics &g) {
    g.fillAll(juce::Colour::fromString("0xff191919"));
}

void WaveComponent::resized() {
    Grid grid({ 1, 7 }, { 1 });
    grid.setItems({&selector, &wave});
    grid.performLayout(getLocalBounds());
}