//
// Created by Aleksandr on 19-Feb-23.
//

#include "OscPanel.h"
#include "../../../other/Grid.h"

OscPanel::OscPanel() {
    addAndMakeVisible(wave);
    addAndMakeVisible(controls);
}

OscPanel::~OscPanel() noexcept {

}

void OscPanel::paint(juce::Graphics &g) {
    g.fillAll(juce::Colour::fromString("0xff191919"));
}

void OscPanel::resized() {
    Grid grid({ 1 }, { 2, 1 });
    grid.setItems({&wave, &controls});
    grid.performLayout(getLocalBounds());
}