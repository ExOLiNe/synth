#pragma once

#include <juce_gui_basics.h>
#include "top/LogoComponent.h"
#include "top/MenuComponent.h"
#include "top/PresetComponent.h"

namespace ui {
    class TopPanel : public juce::Component {
    public:
        TopPanel();
        ~TopPanel() noexcept;

        void paint(juce::Graphics&) override;
        void resized() override;
    private:
        LogoComponent logoComponent;
        MenuComponent menuComponent;
        PresetComponent presetComponent;
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TopPanel)
    };
}