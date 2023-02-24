#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

namespace ui {
    class PresetComponent : public juce::Component {
    public:
        PresetComponent();
        ~PresetComponent();

        void paint(juce::Graphics&) override;
        void resized() override;
    private:
        juce::ComboBox presets;
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PresetComponent)
    };
}