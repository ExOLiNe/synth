//
// Created by Aleksandr on 19-Feb-23.
//

#include "FmOscPanel.h"
#include "../../../defs.h"
#include "../../../Constants.h"

namespace ui {
    FmOscPanel::FmOscPanel(juce::AudioProcessorValueTreeState& treeState)
    : controls(treeState) {
        addAndMakeVisible(controls);

        selector.addListener(&controls);
        selector.setSize(50, 10);
        selector.addItem("A", 1);
        selector.addItem("B", 2);
        selector.setSelectedItemIndex(0);
        addAndMakeVisible(selector);
    }

    FmOscPanel::~FmOscPanel() noexcept {

    }

    void FmOscPanel::paint(juce::Graphics &g) {
        g.fillAll(juce::Colour::fromString("0xff191919"));
    }

    void FmOscPanel::resized() {
        using namespace juce;
        Grid grid;
        grid.templateRows = { Track(Fr(1)) };
        grid.templateColumns = {
                Track(Fr(2)),
                Track(Fr(5))
        };
        grid.items = {
                GridItem(selector).withHeight(50.0f)
                        .withAlignSelf(juce::GridItem::AlignSelf::center),
                GridItem(controls)
        };
        grid.performLayout(getLocalBounds());
    }

    //=============

    FmOscControlsHolder::FmOscControlsHolder(juce::AudioProcessorValueTreeState& treeState) :
        controlsA(treeState, FM_A), controlsB(treeState, FM_B) {
        addAndMakeVisible(controlsA);
        addChildComponent(controlsB);
    }

    FmOscControlsHolder::~FmOscControlsHolder() noexcept {

    }

    void FmOscControlsHolder::paint(juce::Graphics &) {

    }

    void FmOscControlsHolder::resized() {
        controlsA.setBounds(getLocalBounds());
        controlsB.setBounds(getLocalBounds());
    }

    void FmOscControlsHolder::comboBoxChanged (juce::ComboBox* comboBoxThatHasChanged) {
        if (comboBoxThatHasChanged->getSelectedItemIndex() == 0) {
            controlsA.setVisible(true);
            controlsB.setVisible(false);
        } else if (comboBoxThatHasChanged->getSelectedItemIndex() == 1) {
            controlsA.setVisible(false);
            controlsB.setVisible(true);
        }
    }

    //=============

    FmOscControls::FmOscControls(juce::AudioProcessorValueTreeState& treeState, const juce::String& fmId) {
        frequencyLabel.setText("Freq", juce::NotificationType::dontSendNotification);
        frequencyLabel.setJustificationType(juce::Justification::centredRight);
        addAndMakeVisible(frequencyLabel);

        setKnob(frequency);
        addAndMakeVisible(frequency);

        mixLabel.setText("Amp", juce::NotificationType::dontSendNotification);
        mixLabel.setJustificationType(juce::Justification::centredRight);
        addAndMakeVisible(mixLabel);

        setKnob(mix);
        addAndMakeVisible(mix);

        addAndMakeVisible(enabled);

        bindLayoutsToTree(treeState, fmId);
    }

    FmOscControls::~FmOscControls() {

    }

    void FmOscControls::bindLayoutsToTree(juce::AudioProcessorValueTreeState &apvts, const juce::String& fmId) {
        frequencyValue = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
                apvts, fmId + params::fm::freq.name, frequency
                );
        mixValue = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
                apvts, fmId + params::fm::mix.name, mix
        );
        enabledValue = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(
                apvts, fmId + params::fm::enabled.name, enabled
                );

    }

    void FmOscControls::paint(juce::Graphics &g) {
        g.fillAll(juce::Colour::fromString("0xff191919"));
    }

    void FmOscControls::resized() {
        using namespace juce;
        Grid grid;
        grid.templateRows = { Track(Fr(1)) };
        grid.templateColumns = {
                Track(Fr(2)),
                Track(Fr(1)),
                Track(Fr(1)),
                Track(Fr(1)),
                Track(Fr(1))
        };
        grid.items = {
                GridItem(frequencyLabel),
                GridItem(frequency),
                GridItem(mixLabel),
                GridItem(mix),
                GridItem(enabled).withWidth(100.0f).withHeight(100.0f)
        };
        grid.performLayout(getLocalBounds());
    }

    void FmOscControls::setKnob(juce::Slider &knob) {
        knob.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
        knob.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 0, 0);
    }

}