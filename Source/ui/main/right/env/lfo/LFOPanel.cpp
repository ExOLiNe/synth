//
// Created by Aleksandr on 19-Feb-23.
//

#include "LFOPanel.h"
#include "../../../../../Typedefs.h"
#include "../../../../../Constants.h"

namespace ui {
    LFOPanel::LFOPanel(juce::AudioProcessorValueTreeState& treeState, const juce::String lfoId) :
    wave(treeState, lfoId) {
        frequencyLabel.setText("Freq", juce::NotificationType::dontSendNotification);
        frequencyLabel.setJustificationType(juce::Justification::centredRight);
        addAndMakeVisible(frequencyLabel);

        frequency.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
        frequency.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
        addAndMakeVisible(frequency);

        intensityLabel.setText("Amp", juce::NotificationType::dontSendNotification);
        intensityLabel.setJustificationType(juce::Justification::centredRight);
        addAndMakeVisible(intensityLabel);

        intensity.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
        intensity.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
        addAndMakeVisible(intensity);

        treeState.addParameterListener(lfoId + params::lfo::freq.name, &wave);
        treeState.addParameterListener(lfoId + params::lfo::amp.name, &wave);

        addAndMakeVisible(wave);

        bindLayoutsToTree(treeState, lfoId);
    }

    LFOPanel::~LFOPanel() noexcept {

    }

    void LFOPanel::bindLayoutsToTree(juce::AudioProcessorValueTreeState &apvts, const juce::String& lfoId) {
        frequencyValue = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (
                apvts, lfoId + params::lfo::freq.name, frequency
                );
        intensityValue = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (
                apvts, lfoId + params::lfo::amp.name, intensity
        );
    }

    void LFOPanel::paint(juce::Graphics &g) {
        g.fillAll(juce::Colour::fromString("0xff191919"));
    }

    void LFOPanel::resized() {
        using namespace juce;
        Grid grid;
        grid.templateRows = { Track(Fr(4)), Track(Fr(1)) };
        grid.templateColumns = {
                Track(Fr(1)),
                Track(Fr(1)),
                Track(Fr(1)),
                Track(Fr(1))
        };
        grid.items = {
                GridItem(wave).withArea(
                        GridItem::Property(1),
                        GridItem::Property(1),
                        GridItem::Property(1),
                        GridItem::Property(7)
                ),
                GridItem(frequencyLabel),
                GridItem(frequency),
                GridItem(intensityLabel),
                GridItem(intensity)
        };
        grid.performLayout(getLocalBounds());
    }
}