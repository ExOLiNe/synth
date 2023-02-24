#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

namespace ui {
class FmOscControls : public juce::Component {
    public:
        FmOscControls();
        ~FmOscControls();

        void paint(juce::Graphics&) override;
        void resized() override;
    private:
        void setKnob(juce::Slider& knob);
        juce::Label frequencyLabel, intensityLabel;
        juce::ComboBox selector;
        juce::Slider frequency, intensity;
        juce::ToggleButton enabled;
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FmOscControls)
    };

    class FmOscControlsHolder : public juce::Component, public juce::ComboBox::Listener {
    public:
        FmOscControlsHolder();
        ~FmOscControlsHolder();
        void paint(juce::Graphics&) override;
        void resized() override;
    private:
        void comboBoxChanged (juce::ComboBox* comboBoxThatHasChanged) override;
        FmOscControls controlsA, controlsB;
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FmOscControlsHolder)
    };

    class FmOscPanel : public juce::Component {
    public:
        FmOscPanel();
        ~FmOscPanel();

        void paint(juce::Graphics&) override;
        void resized() override;
    private:
        juce::ComboBox selector;
        FmOscControlsHolder controls;
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FmOscPanel)
    };
}