#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include "ADSRWave.h"

class ADSRPanel : public juce::Component {
public:
    ADSRPanel();
    ~ADSRPanel();

    void paint(juce::Graphics&) override;
    void resized() override;
private:
    ADSRWave wave;
    juce::Label attackLabel, decayLabel, sustainLabel, releaseLabel;
    juce::Slider attack, decay, sustain, release;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ADSRPanel)
};