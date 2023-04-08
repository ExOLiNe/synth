#pragma once

#include <juce_gui_basics.h>
#include <juce_audio_processors.h>

namespace ui {
    class FilterControls : public juce::Component {
    public:
        FilterControls(juce::AudioProcessorValueTreeState& treeState, const juce::String& filterId);
        ~FilterControls() noexcept;

        void paint(juce::Graphics&) override;
        void resized() override;
    private:
        void bindLayoutsToTree(juce::AudioProcessorValueTreeState &apvts, const juce::String& filterId);
        void setKnob(juce::Slider& knob);
        juce::Label frequencyLabel, resonanceLabel, mixLabel;
        juce::Slider frequency, resonance, mix;
        juce::ToggleButton enabled;
        juce::ComboBox selector;
        std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> frequencyValue, resonanceValue;
        std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> enabledValue;
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FilterControls)
    };

    class FilterControlsHolder : public juce::Component, public juce::ComboBox::Listener {
    public:
        FilterControlsHolder(juce::AudioProcessorValueTreeState& treeState);
        ~FilterControlsHolder() noexcept;
        void paint(juce::Graphics&) override;
        void resized() override;
    private:
        void comboBoxChanged(juce::ComboBox* comboBoxThatHasChanged) override;
        FilterControls controlsA, controlsB;
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FilterControlsHolder)
    };

    class FilterPanel : public juce::Component {
    public:
        FilterPanel(juce::AudioProcessorValueTreeState& treeState);
        ~FilterPanel() noexcept;

        void paint(juce::Graphics&) override;
        void resized() override;
    private:
        juce::ComboBox selector;
        FilterControlsHolder controls;
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FilterPanel)
    };
}