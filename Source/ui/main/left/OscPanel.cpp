//
// Created by Aleksandr on 19-Feb-23.
//

#include "OscPanel.h"
#include "../../../other/Grid.h"

OscPanel::OscPanel(juce::AudioProcessorValueTreeState& treeState, juce::String oscId) :
controls(treeState, oscId), waveComponent(treeState, oscId) {
    addAndMakeVisible(waveComponent);
    addAndMakeVisible(controls);
}

OscPanel::~OscPanel() noexcept {

}

void OscPanel::paint(juce::Graphics &g) {
    g.fillAll(juce::Colour::fromString("0xff191919"));
}

void OscPanel::resized() {
    Grid grid({ 1 }, { 2, 1 });
    grid.setItems({&waveComponent, &controls});
    grid.performLayout(getLocalBounds());
}