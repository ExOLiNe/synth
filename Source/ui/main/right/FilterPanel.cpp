//
// Created by Aleksandr on 19-Feb-23.
//

#include "FilterPanel.h"
#include "../../../Typedefs.h"
#include "../../../Constants.h"

namespace ui {
    FilterPanel::FilterPanel(juce::AudioProcessorValueTreeState& treeState) : controls(treeState) {
        addAndMakeVisible(controls);

        selector.addListener(&controls);
        selector.addItem("A", 1);
        selector.addItem("B", 2);
        selector.setSelectedItemIndex(0);
        addAndMakeVisible(selector);
    }

    FilterPanel::~FilterPanel() noexcept {

    }

    void FilterPanel::paint(juce::Graphics &g) {
        g.fillAll(juce::Colour::fromString("0xff191919"));
    }

    void FilterPanel::resized() {
        using namespace juce;
        Grid grid;
        grid.templateRows = { Track(Fr(1)) };
        grid.templateColumns = {
                Track(Fr(2)),
                Track(Fr(6))
        };
        grid.items = {
                GridItem(selector).withHeight(50.0f)
                        .withAlignSelf(juce::GridItem::AlignSelf::center),
                GridItem(controls)
        };
        grid.performLayout(getLocalBounds());
    }



    FilterControls::FilterControls(juce::AudioProcessorValueTreeState& treeState, const juce::String& filterId) {
        frequencyLabel.setText("Freq", juce::NotificationType::dontSendNotification);
        frequencyLabel.setJustificationType(juce::Justification::centredRight);
        addAndMakeVisible(frequencyLabel);

        setKnob(frequency);
        addAndMakeVisible(frequency);

        resonanceLabel.setText("Reso", juce::NotificationType::dontSendNotification);
        resonanceLabel.setJustificationType(juce::Justification::centredRight);
        addAndMakeVisible(resonanceLabel);

        setKnob(resonance);
        addAndMakeVisible(resonance);

        mixLabel.setText("Mix", juce::NotificationType::dontSendNotification);
        mixLabel.setJustificationType(juce::Justification::centredRight);
        addAndMakeVisible(mixLabel);

        setKnob(mix);
        addAndMakeVisible(mix);

        addAndMakeVisible(enabled);

        bindLayoutsToTree(treeState, filterId);
    }

    FilterControls::~FilterControls() noexcept {}

    void FilterControls::paint(juce::Graphics &) {}
    void FilterControls::resized() {
        using namespace juce;
        Grid grid;
        grid.templateRows = { Track(Fr(1)) };
        grid.templateColumns = {
                Track(Fr(1)),
                Track(Fr(1)),
                Track(Fr(1)),
                Track(Fr(1)),
                Track(Fr(1))
        };
        grid.items = {
                GridItem(frequencyLabel),
                GridItem(frequency),
                GridItem(resonanceLabel),
                GridItem(resonance),
                GridItem(enabled).withWidth(100.0f).withHeight(100.0f)
        };
        grid.performLayout(getLocalBounds());
    }
    void FilterControls::setKnob(juce::Slider &knob) {
        knob.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
        knob.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 0.0f, 0.0f);
    }

    void FilterControls::bindLayoutsToTree(juce::AudioProcessorValueTreeState &apvts, const juce::String& filterId) {
        frequencyValue = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
                apvts, filterId + params::filter::freq.name, frequency
        );
        resonanceValue = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
                apvts, filterId + params::filter::reso.name, resonance
        );
        enabledValue = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(
                apvts, filterId + params::filter::enabled.name, enabled
                );
    }


    FilterControlsHolder::FilterControlsHolder(juce::AudioProcessorValueTreeState& treeState) :
    controlsA(treeState, FILTER_A), controlsB(treeState, FILTER_B) {
        addAndMakeVisible(controlsA);
        addChildComponent(controlsB);
    }
    FilterControlsHolder::~FilterControlsHolder() noexcept {}

    void FilterControlsHolder::paint(juce::Graphics &) {}

    void FilterControlsHolder::resized() {
        controlsA.setBounds(getLocalBounds());
        controlsB.setBounds(getLocalBounds());
    }

    void FilterControlsHolder::comboBoxChanged(juce::ComboBox *comboBoxThatHasChanged) {
        if (comboBoxThatHasChanged->getSelectedItemIndex() == 0) {
            controlsA.setVisible(true);
            controlsB.setVisible(false);
        } else if (comboBoxThatHasChanged->getSelectedItemIndex() == 1) {
            controlsA.setVisible(false);
            controlsB.setVisible(true);
        }
    }
}