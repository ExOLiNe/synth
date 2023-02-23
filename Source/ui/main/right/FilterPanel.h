#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

class FilterPanel : public juce::Component {
public:
    FilterPanel();
    ~FilterPanel();

    void paint(juce::Graphics&) override;
    void resized() override;
private:
    void setKnob(juce::Slider& knob);
    juce::Label frequencyLabel, resonanceLabel, mixLabel;
    juce::Slider frequency, resonance, mix;
    juce::ToggleButton enabled;
    juce::ComboBox selector;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FilterPanel)
};