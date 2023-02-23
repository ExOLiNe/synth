#include "WaveTables.h"

std::vector<std::vector<float>> getSinWaveTable() {
    std::vector<std::vector<float>> points;
    float sinFactor = 5.0f * 0.01f;
    for (unsigned int z = 0; z < 15; ++z) {
        points.emplace_back(std::vector<float>());
        for (int x = 0; x < 200; ++x) {
            float y = std::sin((float)x * sinFactor);
            points.at(z).push_back(y);
        }
    }
    return points;
}

std::vector<WaveTable> WaveTables::getWaveTables() {
    WaveTable sin = {
            "sin",
            getSinWaveTable(),
            [](unsigned int waveIndex, unsigned int some) -> float {

            }
    };
}