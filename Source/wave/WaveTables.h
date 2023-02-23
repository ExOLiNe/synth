#pragma once
#include <vector>
#include <juce_core/juce_core.h>

struct Wave {

};

struct WaveTable {
    juce::String name;
    std::vector<std::vector<float>> waveTablePresentation;
    std::function<float(unsigned int, unsigned int)> wave;
};

class WaveTables {
public:
    static std::vector<WaveTable> getWaveTables();
};