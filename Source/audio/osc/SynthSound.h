#pragma once

#include <juce_audio_processors.h>

namespace audio {

class SynthSound : public juce::SynthesiserSound {
public:
    bool appliesToNote(int midiNoteNumber) override;
    bool appliesToChannel(int midiChannel) override;
};

}
