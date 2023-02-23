//
// Created by Aleksandr on 19-Feb-23.
//

#include "Controls.h"
#include "../../../../Typedefs.h"
#include "../../../../Constants.h"

Controls::Controls(juce::AudioProcessorValueTreeState& treeState, const juce::String& oscId) {
    voicesLabel.setText("voices", juce::NotificationType::dontSendNotification);
    voicesLabel.setJustificationType(juce::Justification::Flags::centred);
    addAndMakeVisible(voicesLabel);

    setKnob(voices);
    addAndMakeVisible(voices);

    detuneLabel.setText("detune", juce::NotificationType::dontSendNotification);
    detuneLabel.setJustificationType(juce::Justification::Flags::centred);
    addAndMakeVisible(detuneLabel);

    setKnob(detune);
    addAndMakeVisible(detune);

    phaseLabel.setText("phase", juce::NotificationType::dontSendNotification);
    phaseLabel.setJustificationType(juce::Justification::Flags::centred);
    addAndMakeVisible(phaseLabel);

    setKnob(phase);
    addAndMakeVisible(phase);

    wtPosLabel.setText("wtPos", juce::NotificationType::dontSendNotification);
    wtPosLabel.setJustificationType(juce::Justification::Flags::centred);
    addAndMakeVisible(wtPosLabel);

    setKnob(wtPos);
    addAndMakeVisible(wtPos);

    panLabel.setText("pan", juce::NotificationType::dontSendNotification);
    panLabel.setJustificationType(juce::Justification::Flags::centred);
    addAndMakeVisible(panLabel);

    setKnob(pan);
    addAndMakeVisible(pan);

    levelLabel.setText("level", juce::NotificationType::dontSendNotification);
    levelLabel.setJustificationType(juce::Justification::Flags::centred);
    addAndMakeVisible(levelLabel);

    setKnob(level);
    addAndMakeVisible(level);

    semitoneLabel.setText("semitone", juce::NotificationType::dontSendNotification);
    semitoneLabel.setJustificationType(juce::Justification::Flags::centred);
    addAndMakeVisible(semitoneLabel);

    setKnob(semitone);
    addAndMakeVisible(semitone);

    fineLabel.setText("fine", juce::NotificationType::dontSendNotification);
    fineLabel.setJustificationType(juce::Justification::Flags::centred);
    addAndMakeVisible(fineLabel);

    setKnob(fine);
    addAndMakeVisible(fine);

    bindLayoutsToTree(treeState, oscId);
}

Controls::~Controls() noexcept {

}

void Controls::paint(juce::Graphics &g) {
    g.fillAll(juce::Colour::fromString("0xff191919"));
}

void Controls::resized() {
    int knobFraction = 7;
    int labelFraction = 1;
    using namespace juce;
    Grid grid;
    grid.alignItems = Grid::AlignItems::center;
    grid.templateRows = {
            Track(Fr(knobFraction)),
            Track(Fr(labelFraction)),
            Track(Fr(knobFraction)),
            Track(Fr(labelFraction)),
            Track(Fr(knobFraction)),
            Track(Fr(labelFraction)),
            Track(Fr(knobFraction)),
            Track(Fr(labelFraction))
    };
    grid.templateColumns = {
            Track(Fr(1)),
            Track(Fr(1))
    };
    grid.items = {
           voices,
           detune,
           voicesLabel,
           detuneLabel,
           phase,
           wtPos,
           phaseLabel,
           wtPosLabel,
           pan,
           level,
           panLabel,
           levelLabel,
           semitone,
           fine,
           semitoneLabel,
           fineLabel
    };
    grid.performLayout(getLocalBounds());
}

void Controls::setKnob(juce::Slider &knob) {
    knob.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    knob.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 0.0f, 0.0f);
}

void Controls::bindLayoutsToTree(juce::AudioProcessorValueTreeState& apvts, const juce::String& oscId) {
    voicesValue = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
            apvts, oscId + params::osc::voices.name, voices);
    detuneValue = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
            apvts, oscId + params::osc::detune.name, detune);
    phaseValue = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
            apvts, oscId + params::osc::phase.name, phase);
    wtPosValue = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
            apvts, oscId + params::osc::wtPos.name, wtPos);
    panValue = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
            apvts, oscId + params::osc::pan.name, pan);
    levelValue = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
            apvts, oscId + params::osc::level.name, level);
    semitoneValue = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
            apvts, oscId + params::osc::semitone.name, semitone);
    fineValue = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
            apvts, oscId + params::osc::fine.name, fine);
}