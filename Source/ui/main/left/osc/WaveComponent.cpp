//
// Created by Aleksandr on 19-Feb-23.
//

#include "WaveComponent.h"
#include "../../../../other/Grid.h"

WaveComponent::WaveComponent(juce::AudioProcessorValueTreeState& treeState, juce::String oscId)
: waveTables(audio::WaveTables::getInstance()->copyWaveTables()), wave(treeState, oscId) {

    {
        int index = 0;
        for (const auto &waveTable: waveTables) {
            selector.addItem(waveTable.presentation.name, ++index);
        }
    }

    selectWaveTable(1, true);

    addAndMakeVisible(wave);

    selector.addListener(this);
    addAndMakeVisible(selector);

    bindLayoutsToTree(treeState, oscId);
}

WaveComponent::~WaveComponent() noexcept {

}

void WaveComponent::bindLayoutsToTree(juce::AudioProcessorValueTreeState &apvts, const juce::String &oscId) {
    selectorValue = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(
            apvts, oscId + params::osc::waveTableTypeName, selector);
}

void WaveComponent::comboBoxChanged (juce::ComboBox* comboBoxThatHasChanged) {
    selectWaveTable(comboBoxThatHasChanged->getSelectedItemIndex());
}

void WaveComponent::selectWaveTable(int index, bool reselect) {
    wave.setData(waveTables.at(index).presentation.function());
    if (reselect) {
        selector.setSelectedItemIndex(index, juce::NotificationType::dontSendNotification);
    }
}

void WaveComponent::paint(juce::Graphics &g) {
    g.fillAll(juce::Colour::fromString("0xff191919"));
}

void WaveComponent::resized() {
    Grid grid({ 1, 7 }, { 1 });
    grid.setItems({&selector, &wave});
    grid.performLayout(getLocalBounds());
}