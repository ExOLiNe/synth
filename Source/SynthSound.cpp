/*
  ==============================================================================

    SynthSound.cpp
    Created: 8 Jan 2023 5:59:08pm
    Author:  Aleksandr

  ==============================================================================
*/

#include "SynthSound.h"

bool SynthSound::appliesToNote(int midiNoteNumber)
{
    return true;
}
bool SynthSound::appliesToChannel(int midiChannel)
{
    return true;
}