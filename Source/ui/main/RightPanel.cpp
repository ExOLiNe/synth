//
// Created by Aleksandr on 19-Feb-23.
//

#include "RightPanel.h"
#include "../../other/Grid.h"

RightPanel::RightPanel() {
    addAndMakeVisible(fmOscPanel);
    addAndMakeVisible(filterPanel);
    addAndMakeVisible(envPanel);
}

RightPanel::~RightPanel() noexcept {

}

void RightPanel::paint(juce::Graphics &g) {
    g.fillAll(juce::Colours::beige);
    g.setColour(juce::Colours::white);
    g.setFont(15.0f);
    g.drawFittedText("Right panel!", getLocalBounds(), juce::Justification::centred, 1);
}

void RightPanel::resized() {
    Grid grid({ 2, 2, 6}, { 1 });
    grid.setItems({&fmOscPanel, &filterPanel, &envPanel});
    grid.performLayout(getLocalBounds());
}