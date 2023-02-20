//
// Created by Aleksandr on 19-Feb-23.
//

#include "MasterPanel.h"
#include "../other/Grid.h"

MasterPanel::MasterPanel() {
}

MasterPanel::~MasterPanel() noexcept {

}

void MasterPanel::paint(juce::Graphics &g) {
    g.fillAll(juce::Colours::beige);
    g.setColour(juce::Colours::white);
    g.setFont(15.0f);
    g.drawFittedText("Master panel!", getLocalBounds(), juce::Justification::centred, 1);
}

void MasterPanel::resized() {
}