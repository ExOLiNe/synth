#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include "LFOWave.h"

namespace ui {
    class LFOPanel : public juce::Component {
    public:
        LFOPanel(juce::AudioProcessorValueTreeState& treeState, const juce::String lfoId);
        ~LFOPanel() noexcept;

        void paint(juce::Graphics&) override;
        void resized() override;
    private:
        void bindLayoutsToTree(juce::AudioProcessorValueTreeState &apvts, const juce::String& lfoId);
        LFOWave wave;
        juce::Label frequencyLabel, intensityLabel;
        juce::Slider frequency, intensity;
        std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> frequencyValue, intensityValue, mixValue;
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LFOPanel)
    };
}