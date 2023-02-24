//
// Created by Aleksandr on 19-Feb-23.
//

#include "MatrixPanel.h"
#include "../other/Grid.h"

namespace ui {
    MatrixPanel::MatrixPanel() {
    }

    MatrixPanel::~MatrixPanel() noexcept {

    }

    void MatrixPanel::paint(juce::Graphics &g) {
        g.fillAll(juce::Colour::fromString("0xff191919"));
    }

    void MatrixPanel::resized() {
    }
}