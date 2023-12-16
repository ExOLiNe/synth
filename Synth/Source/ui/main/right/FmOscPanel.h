#pragma once

#include <juce_gui_basics.h>
#include <juce_audio_processors.h>

namespace ui {
class FmOscControls : public juce::Component {
    public:
        FmOscControls(juce::AudioProcessorValueTreeState& treeState, const juce::String& fmId);
        ~FmOscControls();

        void paint(juce::Graphics&) override;
        void resized() override;
    private:
        void bindLayoutsToTree(juce::AudioProcessorValueTreeState& apvts, const juce::String& fmId);
        void setKnob(juce::Slider& knob);
        juce::Label frequencyLabel, mixLabel;
        juce::Slider frequency, mix;
        juce::ToggleButton enabled;
        std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>
            frequencyValue, mixValue;
        std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> enabledValue;
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FmOscControls)
    };

    class FmOscControlsHolder : public juce::Component, public juce::ComboBox::Listener {
    public:
        FmOscControlsHolder(juce::AudioProcessorValueTreeState& treeState);
        ~FmOscControlsHolder() noexcept;
        void paint(juce::Graphics&) override;
        void resized() override;
    private:
        void comboBoxChanged (juce::ComboBox* comboBoxThatHasChanged) override;
        FmOscControls controlsA, controlsB;
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FmOscControlsHolder)
    };

    class FmOscPanel : public juce::Component {
    public:
        FmOscPanel(juce::AudioProcessorValueTreeState& treeState);
        ~FmOscPanel() noexcept;

        void paint(juce::Graphics&) override;
        void resized() override;
    private:
        juce::ComboBox selector;
        FmOscControlsHolder controls;
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FmOscPanel)
    };
}