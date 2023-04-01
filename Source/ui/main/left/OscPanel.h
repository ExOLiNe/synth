#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_audio_processors/juce_audio_processors.h>
#include "osc/Controls.h"
#include "osc/WaveComponent.h"

class OscPanel : public juce::Component {
public:
    OscPanel(juce::AudioProcessorValueTreeState& treeState, juce::String oscId);
    ~OscPanel() noexcept;

    void paint(juce::Graphics&) override;
    void resized() override;
private:
    WaveComponent waveComponent;
    Controls controls;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(OscPanel)
};