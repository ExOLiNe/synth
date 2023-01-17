/*
  ==============================================================================

    OscWaveTable.cpp
    Created: 7 Jan 2023 3:49:25pm
    Author:  Aleksandr

  ==============================================================================
*/

#include <JuceHeader.h>
#include "OscWaveTable.h"

//==============================================================================
OscWaveTable::OscWaveTable(SpectrumAnalyzer& analyzer) : 
    scopeData(analyzer.scopeData), scopeSize(analyzer.scopeSize), analyzer(analyzer)
{
    analyzer.addChangeListener(this);
    setSize(524, 230);
}

OscWaveTable::~OscWaveTable()
{
    analyzer.removeChangeListener(this);
}

void OscWaveTable::paint (juce::Graphics& g)
{
    g.fillAll(juce::Colours::aqua);

    g.setOpacity(1.0f);
    g.setColour(juce::Colours::blueviolet);

    for (int i = 1; i < scopeSize; ++i)
    {
        auto width = getLocalBounds().getWidth();
        auto height = getLocalBounds().getHeight();
        g.drawLine({ (float)juce::jmap(i - 1, 0, scopeSize - 1, 0, width),
                                  jmap(scopeData[i - 1], 0.0f, 1.0f, (float)height, 0.0f),
                          (float)juce::jmap(i,     0, scopeSize - 1, 0, width),
                                  jmap(scopeData[i],     0.0f, 1.0f, (float)height, 0.0f) });
    }
}

void OscWaveTable::resized()
{
}

void OscWaveTable::changeListenerCallback(ChangeBroadcaster* source)
{
    repaint();
}