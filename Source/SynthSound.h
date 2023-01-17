/*
  ==============================================================================

    SynthSound.h
    Created: 8 Jan 2023 5:59:08pm
    Author:  Aleksandr

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"

using namespace juce;

class SynthSound : public SynthesiserSound
{
public:
    bool appliesToNote(int midiNoteNumber) override;
    bool appliesToChannel(int midiChannel) override;
};