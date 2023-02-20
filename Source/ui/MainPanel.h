#pragma once

#include "juce_gui_basics/juce_gui_basics.h"
#include "main/LeftPanel.h"
#include "main/RightPanel.h"

class MainPanel : public juce::Component {
public:
    MainPanel();
    ~MainPanel();

    void paint(juce::Graphics&) override;
    void resized() override;
private:
    LeftPanel leftPanel;
    RightPanel rightPanel;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainPanel)
};