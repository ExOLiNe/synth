/*
  ==============================================================================

    EnvelopeComponent.h
    Created: 7 Jan 2023 3:13:41pm
    Author:  Aleksandr

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "AdsrPanel.h"

using namespace juce;

enum AdsrToggle
{
    ADSR1,
    ADSR2,
    MOD_ADSR
};
//==============================================================================
/*
*/
class EnvelopeComponent  : public juce::Component
{
public:
    EnvelopeComponent(std::string firstId, std::string secondId, AudioProcessorValueTreeState& apvts);
    ~EnvelopeComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    AdsrPanel adsrPanel1, adsrPanel2, modAdsrPanel;
    TextButton env1 { "env1" }, env2 { "evn2" }, modEnv { "mod env" }; //TODO remove hardcode

    void toggleAdsrPanel(AdsrToggle adsr);
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (EnvelopeComponent)
};
