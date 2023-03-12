#include "WaveTables.h"

namespace audio {

    Wave::~Wave() noexcept {}

    float SinWave::generate(float frequency, float sampleRate, long long phaseShiftSamples, float phaseOffsetPercentage) {
        float samplesPerPhase = sampleRate / frequency;
        float phaseOffsetSamples = samplesPerPhase * (phaseOffsetPercentage / 100.f);
        auto factor = ((float)phaseShiftSamples + phaseOffsetSamples) / samplesPerPhase;
        return std::sin((factor - (int)factor) * juce::MathConstants<float>::twoPi);
    }

    SinWave::SinWave() {}

    SinWave::~SinWave() noexcept {}


    SawWave::SawWave() {}

    float SawWave::generate(float frequency, float sampleRate, long long phaseShiftSamples, float phaseOffsetPercentage) {
        //TODO phaseOffsetPercentage
        auto factor = (1.0 / (sampleRate / (frequency))) * phaseShiftSamples;
        factor = (factor - (int)factor - 1.0) * 2.0;
        return (float)factor;
    }
    SawWave::~SawWave() noexcept {}


    TriangleWave::TriangleWave() {}

    TriangleWave::~TriangleWave() noexcept {}

    float TriangleWave::generate(float frequency, float sampleRate, long long phaseShiftSamples, float phaseOffsetPercentage) {
        float samplesPerPhase = sampleRate / frequency;
        float phaseOffsetSamples = samplesPerPhase * (phaseOffsetPercentage / 100.f);
        auto factor = ((float)phaseShiftSamples + phaseOffsetSamples) / samplesPerPhase;
        return std::sin((factor - (int)factor) * juce::MathConstants<float>::twoPi);
    }


    WaveTables* WaveTables::getInstance() {
        if (instance == nullptr) {
            instance = new WaveTables();
        }
        return instance;
    }

    WaveTables::~WaveTables() {
        waveTables.clear();
    }

    WaveTables::WaveTables() {
        waveTables.reserve(3);
        {
            waveTables.emplace_back("sin", []() -> TableVector {
                TableVector points;
                float sinFactor = 10.0f * 0.01f;
                for (unsigned int z = 0; z < 15; ++z) {
                    points.emplace_back();
                    for (int x = 0; x < 200; ++x) {
                        float y = std::sin((float) x * sinFactor);
                        points.at(z).push_back(y);
                    }
                }
                return points;
            }, std::vector<Wave*> { new SinWave(), new SinWave(), new SinWave(), new SawWave() });
        }
        {
            waveTables.emplace_back("saw", []() -> TableVector {
                TableVector points;
                for (unsigned int z = 0; z < 15; ++z) {
                    points.emplace_back();
                    float y = -1.f;
                    for (int x = 0; x < 200; ++x) {
                        if (y >= 1.f) {
                            y = -1.f;
                        } else {
                            y += 1.f / 25;
                        }
                        points.at(z).push_back(y);
                    }
                }
                return points;
            }, std::vector<Wave*> { new SawWave(), new SawWave(), new SawWave(), new SawWave() });
        }
        {
            waveTables.emplace_back("triangle", []() -> TableVector {
                TableVector points;
                for (unsigned int z = 0; z < 15; ++z) {
                    points.emplace_back();
                    float y = -1.f;
                    for (int x = 0; x < 200; ++x) {
                        if (y >= 1.f) {
                            y = -1.f;
                        } else {
                            y += 1.f / 25;
                        }
                        points.at(z).push_back(y);
                    }
                }
                return points;
            }, std::vector<Wave*> { new TriangleWave(), new TriangleWave(), new TriangleWave(), new TriangleWave() });
        }
    }

    std::vector<WaveTable> WaveTables::copyWaveTables() {
        return waveTables;
    }



    size_t WaveTables::getTotal() const {
        return waveTables.size();
    }

    WaveTables* WaveTables::instance = nullptr;

    std::vector<WaveTable> WaveTables::waveTables;
}