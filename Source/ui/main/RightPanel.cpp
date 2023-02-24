//
// Created by Aleksandr on 19-Feb-23.
//

#include "RightPanel.h"
#include "../../other/Grid.h"

namespace ui {
    RightPanel::RightPanel(juce::AudioProcessorValueTreeState& treeState) {
        addAndMakeVisible(fmOscPanel);
        addAndMakeVisible(filterPanel);
        addAndMakeVisible(envPanel);
    }

    RightPanel::~RightPanel() noexcept {

    }

    void RightPanel::paint(juce::Graphics &g) {
        g.fillAll(juce::Colour::fromString("0xff191919"));
    }

    void RightPanel::resized() {
        Grid grid({ 2, 2, 6}, { 1 });
        grid.setItems({&fmOscPanel, &filterPanel, &envPanel});
        grid.performLayout(getLocalBounds());
    }
}