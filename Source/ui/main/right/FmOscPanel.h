#pragma once

#include "juce_gui_basics/juce_gui_basics.h"

class FmOscPanel : public juce::Component {
public:
    FmOscPanel();
    ~FmOscPanel();

    void paint(juce::Graphics&) override;
    void resized() override;
private:
    void setKnob(juce::Slider& knob);
    juce::Label frequencyLabel, intensityLabel;
    juce::ComboBox selector;
    juce::Slider frequency, intensity;
    juce::ToggleButton enabled;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FmOscPanel)
};