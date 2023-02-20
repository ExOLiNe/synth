#pragma once

#include "juce_gui_basics/juce_gui_basics.h"
#include "osc/Controls.h"
#include "osc/WaveComponent.h"

class OscPanel : public juce::Component {
public:
    OscPanel();
    ~OscPanel();

    void paint(juce::Graphics&) override;
    void resized() override;
private:
    WaveComponent wave;
    Controls controls;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(OscPanel)
};