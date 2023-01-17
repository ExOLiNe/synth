/*
  ==============================================================================

    EnvelopeComponent.cpp
    Created: 7 Jan 2023 3:13:41pm
    Author:  Aleksandr

  ==============================================================================
*/

#include <JuceHeader.h>
#include "EnvelopeComponent.h"

//==============================================================================
EnvelopeComponent::EnvelopeComponent(std::string firstId, std::string secondId, AudioProcessorValueTreeState& apvts) :
    adsrPanel1(firstId, apvts),
    adsrPanel2(secondId, apvts),
    modAdsrPanel("mod", apvts) //TODO remove hardcode
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    setSize(540, 200);

    addAndMakeVisible(adsrPanel1);
    addChildComponent(adsrPanel2);
    addChildComponent(modAdsrPanel);

    env1.setRadioGroupId(1);
    env2.setRadioGroupId(1);
    modEnv.setRadioGroupId(1);
    env1.setSize(50, 50);
    env2.setSize(50, 50);
    modEnv.setSize(50, 50);
    env1.onClick = [this] { toggleAdsrPanel(AdsrToggle::ADSR1); };
    env2.onClick = [this] { toggleAdsrPanel(AdsrToggle::ADSR2); };
    modEnv.onClick = [this] { toggleAdsrPanel(AdsrToggle::MOD_ADSR); };

    
    addAndMakeVisible(env1);
    addAndMakeVisible(env2);
    addAndMakeVisible(modEnv);
}

EnvelopeComponent::~EnvelopeComponent()
{
}

void EnvelopeComponent::paint (juce::Graphics& g)
{
}

void EnvelopeComponent::resized()
{
    adsrPanel1.setBounds(0, 80, adsrPanel1.getWidth(), getHeight());
    adsrPanel2.setBounds(0, 80, adsrPanel2.getWidth(), getHeight());
    modAdsrPanel.setBounds(0, 80, modAdsrPanel.getWidth(), getHeight());

    env1.setBounds(0, 0, env1.getWidth(), 20);
    env2.setBounds(55, 0, env2.getWidth(), 20);
    modEnv.setBounds(110, 0, modEnv.getWidth(), 20);
}

void EnvelopeComponent::toggleAdsrPanel(AdsrToggle adsr)
{
    switch (adsr)
    {
    case AdsrToggle::ADSR1:
        adsrPanel1.setVisible(true);
        adsrPanel2.setVisible(false);
        modAdsrPanel.setVisible(false);
        break;
    case AdsrToggle::ADSR2:
        adsrPanel1.setVisible(false);
        adsrPanel2.setVisible(true);
        modAdsrPanel.setVisible(false);
        break;
    case AdsrToggle::MOD_ADSR:
        adsrPanel1.setVisible(false);
        adsrPanel2.setVisible(false);
        modAdsrPanel.setVisible(true);
    }
}