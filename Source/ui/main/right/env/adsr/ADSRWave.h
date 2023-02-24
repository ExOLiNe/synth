#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_audio_processors/juce_audio_processors.h>

namespace ui {
    class ADSRWave : public juce::Component, public juce::AudioProcessorValueTreeState::Listener {
    public:
        ADSRWave(juce::AudioProcessorValueTreeState& treeState, const juce::String adsrId);
        ~ADSRWave();

        void paint(juce::Graphics&) override;
        void resized() override;
    void parameterChanged (const juce::String& parameterID, float newValue) override;
    private:
        juce::AudioProcessorValueTreeState& treeState;
        const juce::String adsrId;
        //double getNormalisedValue(const juce::Slider& slider);
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ADSRWave)
    };
}