/*
  ==============================================================================

    Filter.h
    Created: 12 Jan 2023 12:49:19pm
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
class FilterComponent  : public juce::Component
{
public:
    FilterComponent(AudioProcessorValueTreeState& apvts);
    ~FilterComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    void bindLayoutsToTree(AudioProcessorValueTreeState& apvts);

    Slider cutOffSlider, resonanceSlider, mixSlider;
    SliderAttachment cutOffValue, resonanceValue, mixValue;
    Label cutOffLabel, resonanceLabel, mixLabel;

    const int borderSize = 5;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FilterComponent)
};
