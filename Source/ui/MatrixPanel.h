#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_audio_processors/juce_audio_processors.h>

namespace ui {

#define SOURCE "source"
#define DESTINATION "destination"

    int parseComponentID(const char* prefix, juce::ComboBox* combo);

    using SourceCombo = std::unique_ptr<juce::ComboBox>;
    using DestinationCombo = std::unique_ptr<juce::ComboBox>;
    using Intensity = std::unique_ptr<juce::Slider>;
    using ParameterID = juce::String;
    using Attachment = std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>;
    //using RowElement = std::tuple<SourceCombo, DestinationCombo, Intensity, Attachment>;
    struct RowElement {
        SourceCombo source;
        DestinationCombo destination;
        ParameterID parameterID;
        Intensity intensity;
        Attachment attachment;

        void makeVisible(juce::Component* component) const {
            component->addAndMakeVisible(*source);
            component->addAndMakeVisible(*destination);
            component->addAndMakeVisible(*intensity);
        }
    };

class MatrixPanel : public juce::Component, public juce::ComboBox::Listener {
    public:
        MatrixPanel(
                juce::AudioProcessorValueTreeState& apvts,
                const std::vector<juce::String>& paramNamesAbleToModulate,
                const std::vector<juce::String>& modulatorNames);
        ~MatrixPanel();

        void paint(juce::Graphics&) override;
        void resized() override;
        void comboBoxChanged (juce::ComboBox* comboBoxThatHasChanged) override;
    private:
        juce::AudioProcessorValueTreeState& apvts;
        juce::Label sourceLabel, destinationLabel, intensityLabel;

        juce::OwnedArray<RowElement> rowElements;

        const std::vector<juce::String>& paramNamesAbleToModulate;
        juce::OwnedArray<juce::Label> sourceLabels;
        juce::OwnedArray<juce::ComboBox> sources;
        juce::OwnedArray<juce::ComboBox> destinations;
        juce::OwnedArray<juce::Slider> intensityKnobs;
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MatrixPanel)
    };
}