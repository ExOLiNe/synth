#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_audio_processors/juce_audio_processors.h>
#include "wave/Wave.h"
#include "../../../../audio/wave/WaveTables.h"

class WaveComponent : public juce::Component, public juce::ComboBox::Listener {
public:
    WaveComponent(juce::AudioProcessorValueTreeState& treeState, juce::String oscId);
    ~WaveComponent();

    void paint(juce::Graphics&) override;
    void resized() override;

    void comboBoxChanged (juce::ComboBox* comboBoxThatHasChanged) override;
private:
    void selectWaveTable(int index, bool reselect = false);

    juce::ComboBox selector;
    Wave wave;
    std::vector<audio::WaveTable>& waveTables;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(WaveComponent)
};