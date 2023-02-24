#include "WaveTables.h"

namespace audio {
    void Wave::shiftPhase(float offset) {
        phase += offset;
    }

    Wave::~Wave() noexcept {

    }

    float SinWave::generate(float frequency, float sampleRate) {
        output = std::sin(phase * juce::MathConstants<float>::twoPi);
        if (phase >= 1.f) phase -= 1.f;
        phase += (1.f / (sampleRate / (frequency)));
        return output;
    }

    SinWave::~SinWave() noexcept {

    }

    float SawWave::generate(float frequency, float sampleRate) {
        output = phase;
        if (phase >= 1.f) phase -= 2.f;
        phase += (1.f / (sampleRate / (frequency))) * 2.f;
        return output;
    }
    SawWave::~SawWave() noexcept {

    }

    //TODO cache this shit
    std::vector<WaveTable> WaveTables::getWaveTables() {
        std::vector<WaveTable> waveTables;
        waveTables.emplace_back(WaveTable("sin", []() -> TableVector {
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
        }, { new SinWave(), new SinWave(), new SinWave(), new SinWave() }));
        waveTables.emplace_back(WaveTable("saw", []() -> TableVector {
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
            }, { new SawWave(), new SawWave(), new SawWave(), new SawWave() }));
        return waveTables;
    }


    /*WaveTable::WaveTable(WaveTable &&other)  {
        this->name = std::move(other.name);
        this->getWaveTablePresentation = other.getWaveTablePresentation;
        this->waveTable = std::move(other.waveTable);
    }*/

    WaveTable::WaveTable(juce::String name, std::function<TableVector()> presentation, const std::initializer_list<Wave*>& waves)
            : name(name), getWaveTablePresentation(presentation), waveTable(waves) {}
}