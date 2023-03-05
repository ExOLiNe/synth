#include "WaveTables.h"

namespace audio {
    void Wave::shiftPhase(float offset) {
        phase += offset;
    }

    Wave::~Wave() noexcept {

    }

    float SinWave::generate(float frequency, float sampleRate, long long phaseOffset) {
        auto factor = (1.0 / (sampleRate / (frequency))) * phaseOffset;
        factor = factor - (int)factor;
        output = std::sin(factor * juce::MathConstants<float>::twoPi);
        return output;
    }

    SinWave::SinWave() {

    }

    SinWave::~SinWave() noexcept {

    }

    Wave *SinWave::clone() {
        return new SinWave(*this);
    }

    SawWave::SawWave() {

    }

    float SawWave::generate(float frequency, float sampleRate, long long phaseOffset) {
        auto factor = (1.0 / (sampleRate / (frequency))) * phaseOffset;
        factor = (factor - (int)factor - 1.0) * 2.0;
        return (float)factor;
    }
    SawWave::~SawWave() noexcept {

    }

    Wave *SawWave::clone() {
        return new SawWave(*this);
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
        {
            WaveTable waveTable = WaveTable("sin", []() -> TableVector {
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
            });
            waveTable.waveTable.push_back(new SinWave());
            waveTable.waveTable.push_back(new SinWave());
            waveTable.waveTable.push_back(new SinWave());
            waveTable.waveTable.push_back(new SawWave());
            waveTables.push_back(waveTable);
        }
        {
            WaveTable waveTable = WaveTable("saw", []() -> TableVector {
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
            });
            waveTable.waveTable.push_back(new SawWave());
            waveTable.waveTable.push_back(new SawWave());
            waveTable.waveTable.push_back(new SawWave());
            waveTable.waveTable.push_back(new SawWave());
            waveTables.push_back(waveTable);
        }
    }

    //TODO cache this shit
    std::vector<WaveTable> WaveTables::getWaveTables() {
        return waveTables;
    }

    WaveTables* WaveTables::instance = nullptr;

    std::vector<WaveTable> WaveTables::waveTables;


    /*WaveTable::WaveTable(juce::String name, std::function<TableVector()> presentation, Ts... wavesConstructor)
            : name(name), getWaveTablePresentation(presentation), waveTable(wavesConstructor) {}*/

    /*WaveTable::WaveTable(WaveTable &other) : name(other.name), getWaveTablePresentation(other.getWaveTablePresentation) {
        waveTable.clear();
    }

    WaveTable::WaveTable(WaveTable&& other)
        : waveTable(std::move(other.waveTable)), name(other.name), getWaveTablePresentation(other.getWaveTablePresentation) {
    }*/
}