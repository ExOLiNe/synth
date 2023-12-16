//
// Created by Aleksandr on 19-Feb-23.
//

#include "LeftPanel.h"
#include "../../other/Grid.h"
#include "../../Constants.h"

namespace ui {
    LeftPanel::LeftPanel(juce::AudioProcessorValueTreeState& treeState)
            : oscPanelA(treeState, OSC1), oscPanelB(treeState, OSC2) {
        addAndMakeVisible(oscPanelA);
        addAndMakeVisible(oscPanelB);
    }

    LeftPanel::~LeftPanel() noexcept {

    }

    void LeftPanel::paint(juce::Graphics &g) {
        g.fillAll(juce::Colour::fromString("0xff191919"));
    }

    void LeftPanel::resized() {
        Grid grid({1, 1}, {1});
        grid.setItems({&oscPanelA, &oscPanelB});
        grid.performLayout(getLocalBounds());
    }
}