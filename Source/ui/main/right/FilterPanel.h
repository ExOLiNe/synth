#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

namespace ui {
    class FilterControls : public juce::Component {
    public:
        FilterControls();
        ~FilterControls();

        void paint(juce::Graphics&) override;
        void resized() override;
    private:
        void setKnob(juce::Slider& knob);
        juce::Label frequencyLabel, resonanceLabel, mixLabel;
        juce::Slider frequency, resonance, mix;
        juce::ToggleButton enabled;
        juce::ComboBox selector;
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FilterControls)
    };

    class FilterControlsHolder : public juce::Component, public juce::ComboBox::Listener {
    public:
        FilterControlsHolder();
        ~FilterControlsHolder();
        void paint(juce::Graphics&) override;
        void resized() override;
    private:
        void comboBoxChanged(juce::ComboBox* comboBoxThatHasChanged) override;
        FilterControls controlsA, controlsB;
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FilterControlsHolder)
    };

    class FilterPanel : public juce::Component {
    public:
        FilterPanel();
        ~FilterPanel();

        void paint(juce::Graphics&) override;
        void resized() override;
    private:
        juce::ComboBox selector;
        FilterControlsHolder controls;
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FilterPanel)
    };
}