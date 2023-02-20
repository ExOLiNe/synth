#pragma once

#include "juce_gui_basics/juce_gui_basics.h"
#include "wave/Wave.h"

class WaveComponent : public juce::Component {
public:
    WaveComponent();
    ~WaveComponent();

    void paint(juce::Graphics&) override;
    void resized() override;
private:
    Wave wave;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(WaveComponent)
};