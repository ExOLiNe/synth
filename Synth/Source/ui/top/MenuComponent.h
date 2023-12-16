#pragma once

#include <juce_gui_basics.h>

namespace ui {
    class MenuComponent : public juce::Component, public juce::Button::Listener {
    public:
        MenuComponent();
        ~MenuComponent() noexcept;

        void paint(juce::Graphics&) override;
        void resized() override;
        void buttonStateChanged (juce::Button*) override;
        void buttonClicked (juce::Button*) override;
    private:
        juce::TextButton main, matrix, master;
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MenuComponent)
    };
}