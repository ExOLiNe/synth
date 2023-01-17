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
using namespace std;

using SliderAttachment = std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment>;

//==============================================================================
/*
*/
class AdsrPanel  : public juce::Component
{
public:
    AdsrPanel(const string& id, AudioProcessorValueTreeState& apvts);
    ~AdsrPanel() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    Slider attackSlider, decaySlider, sustainSlider, releaseSlider;
    Label attackLabel, decayLabel, sustainLabel, releaseLabel;
    SliderAttachment attackValue, decayValue, sustainValue, releaseValue;

    const int borderSize = 5;

    void bindLayoutsToTree(const string& id, AudioProcessorValueTreeState& apvts);
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AdsrPanel)
};
