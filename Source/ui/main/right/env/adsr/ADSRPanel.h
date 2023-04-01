#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_audio_processors/juce_audio_processors.h>
#include "ADSRWave.h"

namespace ui {
    class ADSRPanel : public juce::Component {
    public:
        ADSRPanel(juce::AudioProcessorValueTreeState& treeState, const juce::String adsrId);
        ~ADSRPanel() noexcept;

        void paint(juce::Graphics&) override;
        void resized() override;
    private:
        void bindLayoutsToTree(juce::AudioProcessorValueTreeState &apvts, const juce::String& adsrId);
        ADSRWave wave;
        juce::Label attackLabel, decayLabel, sustainLabel, releaseLabel;
        juce::Slider attack, decay, sustain, release;
        std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>
            attackValue, decayValue, sustainValue, releaseValue;
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ADSRPanel)
    };
}