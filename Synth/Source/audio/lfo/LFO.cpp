//
// Created by Aleksandr on 19-Mar-23.
//

#include "LFO.h"
#include <juce_core.h>

namespace audio {
    LFO::LFO(std::atomic<float> *lfoValue, const std::atomic<float>* frequency) : lfoValue(lfoValue), frequency(frequency) {
    }
    void LFO::shiftPhase(int numSamples, double sampleRate) {
        phase += numSamples;
        float samplesPerPhase = static_cast<float>(sampleRate) / *frequency;
        auto factor = (float)phase / samplesPerPhase;
        auto value = std::sin((factor - static_cast<float>(static_cast<int>(factor))) * juce::MathConstants<float>::twoPi);
        lfoValue->store(value);
    }
}