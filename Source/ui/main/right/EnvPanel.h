#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include "env/Body.h"

namespace ui {
    class EnvPanel : public juce::Component, public juce::Button::Listener {
    public:
        EnvPanel(juce::AudioProcessorValueTreeState& treeState);
        ~EnvPanel();

        void paint(juce::Graphics&) override;
        void resized() override;
    private:
        void buttonClicked (juce::Button*);
        Body body;
        juce::TextButton volumeAdsrButton, adsr1Button, adsr2Button, lfo1Button, lfo2Button;
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(EnvPanel)
    };
}