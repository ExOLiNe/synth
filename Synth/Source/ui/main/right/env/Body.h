#pragma once

#include <juce_gui_basics.h>
#include "adsr/ADSRPanel.h"
#include "lfo/LFOPanel.h"

namespace ui {
    class Body : public juce::Component, public juce::ActionListener {
    public:
        Body(juce::AudioProcessorValueTreeState& treeState);
        ~Body() noexcept;

        void paint(juce::Graphics&) override;
        void resized() override;
        void actionListenerCallback (const juce::String& message) override;
    private:
        ADSRPanel volumeAdsr, adsr1, adsr2;
        LFOPanel lfo1, lfo2;
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Body)
    };
}