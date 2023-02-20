#pragma once

#include "juce_gui_basics/juce_gui_basics.h"
#include "MainPanel.h"
#include "MatrixPanel.h"
#include "MasterPanel.h"

class BodyPanel : public juce::Component, public juce::ActionListener {
public:
    BodyPanel();
    ~BodyPanel();

    void paint(juce::Graphics&) override;
    void resized() override;
    void actionListenerCallback (const juce::String& message) override;
private:
    MainPanel mainPanel;
    MatrixPanel matrixPanel;
    MasterPanel masterPanel;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BodyPanel)
};