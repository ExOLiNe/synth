#pragma once

#include <juce_gui_basics.h>
#include <juce_audio_processors.h>

namespace ui {

#define SOURCE "source"
#define DESTINATION "destination"
    enum struct ComboBoxType { source, destination, other };

    int parseComponentID(juce::String prefix, const juce::ComboBox* combo);

    using Combo = std::unique_ptr<juce::ComboBox>;
    using SourceCombo = Combo;
    using DestinationCombo = Combo;
    using Intensity = std::unique_ptr<juce::Slider>;
    using ParameterID = juce::String;
    using Attachment = std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>;
    //using RowElement = std::tuple<SourceCombo, DestinationCombo, Intensity, Attachment>;
    struct RowElement {
        SourceCombo source;
        DestinationCombo destination;
        ParameterID parameterID; // this is for deletion of attachment because SliderAttachment doesn't have getter for its paramID
        Intensity intensity;
        Attachment attachment;

        void makeVisible(juce::Component* component) const {
            component->addAndMakeVisible(*source);
            component->addAndMakeVisible(*destination);
            component->addAndMakeVisible(*intensity);
        }

        juce::String getParameterID() {
            return source->getText() + destination->getText();
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
    /*
     *  comboBoxThatHasChanged = source combobox,  anotherComboBox = destination combobox
     *  OR
     *  comboBoxThatHasChanged = destination combobox,  anotherComboBox = source combobox
     */
    void updateAttachment(
            const Combo& comboBoxThatHasChanged,
            const Combo& anotherComboBox,
            RowElement* rowElement
    );
    ComboBoxType getComboBoxType(const juce::ComboBox* comboBox);
    int getComponentID(const ComboBoxType type, const juce::ComboBox* comboBox) const;
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