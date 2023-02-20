#pragma once

#include "juce_gui_basics/juce_gui_basics.h"
#include "right/FmOscPanel.h"
#include "right/FilterPanel.h"
#include "right/EnvPanel.h"

class RightPanel : public juce::Component {
public:
    RightPanel();
    ~RightPanel();

    void paint(juce::Graphics&) override;
    void resized() override;
private:
    FmOscPanel fmOscPanel;
    FilterPanel filterPanel;
    EnvPanel envPanel;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(RightPanel)
};