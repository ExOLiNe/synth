//
// Created by Aleksandr on 19-Feb-23.
//

#include "MatrixPanel.h"
#include "../other/Grid.h"

MatrixPanel::MatrixPanel() {
}

MatrixPanel::~MatrixPanel() noexcept {

}

void MatrixPanel::paint(juce::Graphics &g) {
    g.fillAll(juce::Colours::beige);
    g.setColour(juce::Colours::white);
    g.setFont(15.0f);
    g.drawFittedText("Matrix panel!", getLocalBounds(), juce::Justification::centred, 1);
}

void MatrixPanel::resized() {
}