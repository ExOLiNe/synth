/*
  ==============================================================================

    OscWaveTable.h
    Created: 7 Jan 2023 3:49:25pm
    Author:  Aleksandr

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "SpectrumAnalyzer.h"

using namespace juce;
//==============================================================================
/*
*/
class OscWaveTable  : public juce::Component, public juce::ChangeListener
{
public:
    OscWaveTable(SpectrumAnalyzer& analyzer);
    ~OscWaveTable() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    void changeListenerCallback(ChangeBroadcaster* source) override;
private:
    int scopeSize;
    float* scopeData;
    SpectrumAnalyzer& analyzer;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OscWaveTable)
};
