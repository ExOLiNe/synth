#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

namespace ui {
    class ADSRWave : public juce::Component, public juce::Slider::Listener {
    public:
        ADSRWave();
        ~ADSRWave();

        void paint(juce::Graphics&) override;
        void resized() override;
        void setDefaultValues(
                const juce::Slider& attackSlider,
                const juce::Slider& decaySlider,
                const juce::Slider& sustainSlider,
                const juce::Slider& releaseSlider);
        void sliderValueChanged(juce::Slider *) override;
    private:
        void drawSin(juce::Graphics&, float pointThickness, juce::Point<float> p1, juce::Point<float> p2);
        double getNormalisedValue(const juce::Slider& slider);
        double attackValue, decayValue, sustainValue, releaseValue;
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ADSRWave)
    };
}