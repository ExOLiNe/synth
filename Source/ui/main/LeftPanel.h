#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_audio_processors/juce_audio_processors.h>
#include "left/OscPanel.h"

namespace ui {
    class LeftPanel : public juce::Component {
    public:
        LeftPanel(juce::AudioProcessorValueTreeState& treeState);
        ~LeftPanel();

        void paint(juce::Graphics&) override;
        void resized() override;
    private:
        OscPanel oscPanelA, oscPanelB;
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LeftPanel)
    };
}