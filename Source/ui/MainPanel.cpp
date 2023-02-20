#include "MainPanel.h"
#include "../other/Grid.h"

MainPanel::MainPanel() {
    addAndMakeVisible(leftPanel);
    addAndMakeVisible(rightPanel);
}

MainPanel::~MainPanel() noexcept {

}

void MainPanel::paint(juce::Graphics &g) {
    g.fillAll(juce::Colours::indigo);
    g.setColour(juce::Colours::white);
    g.setFont(15.0f);
    g.drawFittedText("Main panel!", getLocalBounds(), juce::Justification::centred, 1);
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