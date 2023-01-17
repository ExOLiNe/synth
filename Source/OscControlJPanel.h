/*
  ==============================================================================

    OscControlJPanel.h
    Created: 7 Jan 2023 3:13:21pm
    Author:  Aleksandr

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "Params.h"

using namespace juce;

using SliderAttachment = std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment>;
using ComboBoxAttachment = std::unique_ptr<AudioProcessorValueTreeState::ComboBoxAttachment>;

//==============================================================================
/*
*/
class OscControlJPanel  : public juce::Component
{
public:
    OscControlJPanel(std::string id, AudioProcessorValueTreeState& apvts);
    ~OscControlJPanel() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    void bindLayoutsToTree(std::string id, AudioProcessorValueTreeState& apvts);

    Slider detune;
    SliderAttachment detuneValue;
    Label detuneLabel;

    Slider phase;
    SliderAttachment phaseValue;
    Label phaseLabel;

    Slider level;
    SliderAttachment levelValue;
    Label levelLabel;

    ComboBox voices;
    ComboBoxAttachment voicesValue;
    Label voicesLabel;


    int borderSize = 5;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OscControlJPanel)
};
