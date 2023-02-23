#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_audio_processors/juce_audio_processors.h>

class Controls : public juce::Component {
public:
    Controls(juce::AudioProcessorValueTreeState& treeState, const juce::String& oscId);
    ~Controls();

    void paint(juce::Graphics&) override;
    void resized() override;
private:
    void bindLayoutsToTree(juce::AudioProcessorValueTreeState& apvts, const juce::String& oscId);
    void setKnob(juce::Slider& knob);
    juce::Slider voices, detune, phase, wtPos, pan, level, semitone, fine;
    juce::Label voicesLabel, detuneLabel, phaseLabel, wtPosLabel, panLabel, levelLabel, semitoneLabel, fineLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> voicesValue, detuneValue, phaseValue,
    wtPosValue, panValue, levelValue, semitoneValue, fineValue;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Controls)
};