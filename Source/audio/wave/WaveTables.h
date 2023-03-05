#pragma once

#include <vector>
#include "juce_core/juce_core.h"
#include <cmath>

namespace audio {
    class Wave {
    public:
        void shiftPhase(float offset);
        virtual float generate(float frequency, float sampleRate, long long phaseOffset) = 0;
        virtual Wave* clone() = 0;
        virtual ~Wave();
    protected:
        float output = 0.f;
        float phase = 0.f;
    };


    class SinWave : public Wave {
    public:
        SinWave();
        float generate(float frequency, float sampleRate, long long phaseOffset) override;
        Wave* clone() override;
        ~SinWave();
    private:
        SinWave(const SinWave& other) = default;
    };

    class SawWave : public Wave {
    public:
        SawWave();
        float generate(float frequency, float sampleRate, long long phaseOffset) override;
        Wave* clone() override;
        ~SawWave();
    private:
        SawWave(const SawWave& other) = default;
    };

    using TableVector = std::vector<std::vector<float>>;

    struct WaveTable {
        WaveTable(juce::String name, std::function<TableVector()> presentation)
                : name(name), getWaveTablePresentation(presentation) {}
        WaveTable(const WaveTable& other) : name(other.name), getWaveTablePresentation(other.getWaveTablePresentation) {
            waveTable.clear();
            waveTable.reserve(other.waveTable.size());
            for (auto* wave : other.waveTable) {
                waveTable.push_back(wave->clone());
            }
        }

        void resetPhaseOffset() {
            phaseOffset = 0;
        }

        void shiftPhase() {
            ++phaseOffset;
        }

        float generateSample(double frequency, double sampleRate, int waveIndex) {
            return waveTable[waveIndex]->generate(frequency, sampleRate, phaseOffset);
        }

        long long phaseOffset = 0;

        juce::String name;
        std::function<TableVector()> getWaveTablePresentation;
        std::vector<Wave*> waveTable;
    };

    class WaveTables {
    public:
        WaveTables(WaveTables& other) = delete;
        void operator=(const WaveTables &) = delete;
        static WaveTables* getInstance();
        std::vector<WaveTable> getWaveTables();
        ~WaveTables();
    private:
        static WaveTables* instance;
        static std::vector<WaveTable> waveTables;
        WaveTables();
    };
}