#pragma once

#include "juce_gui_basics/juce_gui_basics.h"

class Controls : public juce::Component {
public:
    Controls();
    ~Controls();

    void paint(juce::Graphics&) override;
    void resized() override;
private:
    void setKnob(juce::Slider& knob);
    juce::Slider voices, detune, phase, wtPos, pan, level, semitone, fine;
    juce::Label voicesLabel, detuneLabel, phaseLabel, wtPosLabel, panLabel, levelLabel, semitoneLabel, fineLabel;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Controls)
};