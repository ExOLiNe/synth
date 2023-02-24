//
// Created by Aleksandr on 19-Feb-23.
//

#include "TopPanel.h"
#include "../other/Grid.h"

namespace ui {
    TopPanel::TopPanel() {
        addAndMakeVisible(logoComponent);
        addAndMakeVisible(menuComponent);
        addAndMakeVisible(presetComponent);
    }

    TopPanel::~TopPanel() noexcept {

    }

    void TopPanel::paint(juce::Graphics &g) {
        g.fillAll(juce::Colour::fromString("0xff191919"));
    }

    void TopPanel::resized() {
        Grid grid({ 1 }, { 1, 2, 3});
        grid.setItems({&logoComponent, &menuComponent, &presetComponent});
        grid.performLayout(getLocalBounds());
    }
}