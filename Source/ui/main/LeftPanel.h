#pragma once

#include "juce_gui_basics/juce_gui_basics.h"
#include "left/OscPanel.h"

class LeftPanel : public juce::Component {
public:
    LeftPanel();
    ~LeftPanel();

    void paint(juce::Graphics&) override;
    void resized() override;
private:
    OscPanel oscPanelA, oscPanelB;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LeftPanel)
};