/*
  ==============================================================================

    AdsrPanel.h
    Created: 9 Jan 2023 9:45:33pm
    Author:  Aleksandr

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "Params.h"

using namespace juce;

using SliderAttachment = std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment>;

//==============================================================================
/*
*/
class AdsrPanel  : public juce::Component
{
public:
    AdsrPanel(std::string id, AudioProcessorValueTreeState& apvts);
    ~AdsrPanel() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    Slider attackSlider, decaySlider, sustainSlider, releaseSlider;
    Label attackLabel, decayLabel, sustainLabel, releaseLabel;
    SliderAttachment attackValue, decayValue, sustainValue, releaseValue;

    int borderSize = 5;

    void bindLayoutsToTree(std::string id, AudioProcessorValueTreeState& apvts);
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AdsrPanel)
};
