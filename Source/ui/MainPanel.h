#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_audio_processors/juce_audio_processors.h>
#include "main/LeftPanel.h"
#include "main/RightPanel.h"

namespace ui {
    class MainPanel : public juce::Component {
    public:
        MainPanel(juce::AudioProcessorValueTreeState&);
        ~MainPanel();

        void paint(juce::Graphics&) override;
        void resized() override;
    private:
        LeftPanel leftPanel;
        RightPanel rightPanel;
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainPanel)
    };
}