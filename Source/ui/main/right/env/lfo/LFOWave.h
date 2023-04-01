#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_audio_processors/juce_audio_processors.h>

namespace ui {
class LFOWave : public juce::Component, public juce::AudioProcessorValueTreeState::Listener {
    public:
        LFOWave(juce::AudioProcessorValueTreeState& treeState, const juce::String lfoId);
        ~LFOWave() noexcept;

        void paint(juce::Graphics&) override;
        void resized() override;
    private:
        void parameterChanged (const juce::String& parameterID, float newValue) override;
        juce::AudioProcessorValueTreeState& treeState;
        const juce::String lfoId;
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LFOWave)
    };
}