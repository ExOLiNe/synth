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
    g.fillAll(juce::Colour::fromString("0xff191919"));
}

void MasterPanel::resized() {
}