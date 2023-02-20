#pragma once

#include "juce_gui_basics/juce_gui_basics.h"

class EnvPanel : public juce::Component {
public:
    EnvPanel();
    ~EnvPanel();

    void paint(juce::Graphics&) override;
    void resized() override;
private:
    juce::Slider slider;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(EnvPanel)
};