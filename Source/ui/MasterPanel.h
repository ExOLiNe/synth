#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

namespace ui {
    class MasterPanel : public juce::Component {
    public:
        MasterPanel();
        ~MasterPanel();

        void paint(juce::Graphics&) override;
        void resized() override;
    private:
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MasterPanel)
    };
}