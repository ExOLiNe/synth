//
// Created by Aleksandr on 19-Feb-23.
//

#include "WaveComponent.h"
#include "../../../../other/Grid.h"
#include "../../../../Constants.h"

WaveComponent::WaveComponent(juce::AudioProcessorValueTreeState& treeState, juce::String oscId) {
    waveTables = audio::WaveTables::getWaveTables();

    {
        int index = 0;
        for (const auto &waveTable: waveTables) {
            selector.addItem(waveTable.name, ++index);
        }
    }

    selectWaveTable(0, true);

    treeState.addParameterListener(oscId + params::osc::wtPos.name, &wave);
    addAndMakeVisible(wave);

    selector.addListener(this);
    addAndMakeVisible(selector);
}

WaveComponent::~WaveComponent() noexcept {

}

void WaveComponent::comboBoxChanged (juce::ComboBox* comboBoxThatHasChanged) {
    selectWaveTable(comboBoxThatHasChanged->getSelectedItemIndex());
}

void WaveComponent::selectWaveTable(int index, bool reselect) {
    wave.setData(waveTables.at(index).getWaveTablePresentation());
    wave.setZHighlight(0);
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