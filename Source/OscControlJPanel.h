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
using namespace std;

using SliderAttachment = std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment>;
using ComboBoxAttachment = std::unique_ptr<AudioProcessorValueTreeState::ComboBoxAttachment>;

//==============================================================================
/*
*/
class OscControlJPanel  : public juce::Component
{
public:
    OscControlJPanel(const string& id, AudioProcessorValueTreeState& apvts);
    ~OscControlJPanel() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    void bindLayoutsToTree(const string& id, AudioProcessorValueTreeState& apvts);

    Slider detune, phase, level;
    SliderAttachment detuneValue, phaseValue, levelValue;
    Label detuneLabel, phaseLabel, levelLabel, voicesLabel;

    ComboBox voices;
    ComboBoxAttachment voicesValue;

    const int borderSize = 5;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OscControlJPanel)
};
