#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

namespace ui {
    class LFOWave : public juce::Component {
    public:
        LFOWave();
        ~LFOWave();

        void paint(juce::Graphics&) override;
        void resized() override;
    private:
        float rate = 1.0f, amp = 1.0f;
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LFOWave)
    };
}