#include "MainPanel.h"
#include "../other/Grid.h"

namespace ui {
    MainPanel::MainPanel(juce::AudioProcessorValueTreeState& treeState) : leftPanel(treeState), rightPanel(treeState) {
        addAndMakeVisible(leftPanel);
        addAndMakeVisible(rightPanel);
    }

    MainPanel::~MainPanel() noexcept {

    }

    void MainPanel::paint(juce::Graphics &g) {
        g.fillAll(juce::Colour::fromString("0xff191919"));
    }

    void MainPanel::resized() {
        Grid grid({ 1 }, { 4, 3 });
        grid.setItems({&leftPanel, &rightPanel});
        grid.performLayout(getLocalBounds());
        /*juce::FlexBox fb;
        fb.flexWrap = juce::FlexBox::Wrap::wrap;
        fb.justifyContent = juce::FlexBox::JustifyContent::center;
        fb.alignContent = juce::FlexBox::AlignContent::center;
        fb.items.add(juce::FlexItem(voices).withMinWidth(200.0f).withMinHeight(200.0f));
        fb.performLayout(getLocalBounds().toFloat());*/
    }
}