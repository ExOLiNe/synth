#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

namespace ui {
    class LogoComponent : public juce::Component {
    public:
        LogoComponent();
        ~LogoComponent();

        void paint(juce::Graphics&) override;
        void resized() override;
    private:
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LogoComponent)
    };
}