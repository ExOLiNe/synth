#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_audio_processors/juce_audio_processors.h>

namespace ui {


    class MatrixPanel : public juce::Component {
    public:
        MatrixPanel(const std::vector<juce::String>& paramNamesAbleToModulate,
                    const std::vector<juce::String>& modulatorNames);
        ~MatrixPanel();

        void paint(juce::Graphics&) override;
        void resized() override;
    private:
        juce::Label sourceLabel, destinationLabel, intensityLabel;
        const std::vector<juce::String>& paramNamesAbleToModulate;
        juce::OwnedArray<juce::Label> sourceLabels;
        juce::OwnedArray<juce::ComboBox> destinations;
        juce::OwnedArray<juce::Slider> intensityKnobs;
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MatrixPanel)
    };
}