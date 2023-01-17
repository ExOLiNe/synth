/*
  ==============================================================================

    Oscillator.h
    Created: 7 Jan 2023 3:12:57pm
    Author:  Aleksandr

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <string>
#include "OscControlJPanel.h"
#include "OscWaveTable.h"
#include "SpectrumAnalyzer.h"

using namespace juce;
using namespace std;
//==============================================================================
/*
*/
class Oscillator  : public Component
{
public:
    Oscillator(const string name, const string& id, AudioProcessorValueTreeState& apvts, SpectrumAnalyzer& analyzer);
    ~Oscillator() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    const string name;
    OscControlJPanel controlPanel;
    OscWaveTable waveTable;

    ComboBox waveSelector;
    unique_ptr<AudioProcessorValueTreeState::ComboBoxAttachment> waveSelectorAttachment;

    const int borderSize = 8;

    void bindLayoutsToTree(const string& id, AudioProcessorValueTreeState& apvts);
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Oscillator)
};
