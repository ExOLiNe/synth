#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include "LFOWave.h"

namespace ui {
    class LFOPanel : public juce::Component {
    public:
        LFOPanel();
        ~LFOPanel();

        void paint(juce::Graphics&) override;
        void resized() override;
    private:
        LFOWave wave;
        juce::Label frequencyLabel, intensityLabel, mixLabel;
        juce::Slider frequency, intensity, mix;
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LFOPanel)
    };
}