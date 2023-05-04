#pragma once

#include <vector>
#include "juce_core.h"
#include "../../other/polymorphic_readonly_array.h"
#include <cmath>
#include <Tracy.hpp>

namespace audio {
    class Wave {
    public:
        virtual float generate(float frequency, float sampleRate, long long phaseShiftSamples, float phaseOffsetPercentage = 0.f) = 0;
        virtual ~Wave() noexcept;
    };


    class SinWave : public Wave {
    public:
        SinWave();
        SinWave(const SinWave& other) = delete;
        float generate(float frequency, float sampleRate, long long phaseShiftSamples, float phaseOffsetPercentage = 0.f) override;
        ~SinWave() noexcept override;
    };

    class SawWave : public Wave {
    public:
        SawWave();
        SawWave(const SawWave& other) = delete;
        float generate(float frequency, float sampleRate, long long phaseShiftSamples, float phaseOffsetPercentage = 0.f) override;
        ~SawWave() noexcept override;
    };

    class TriangleWave : public Wave {
    public:
        TriangleWave();
        TriangleWave(const TriangleWave& other) = delete;
        float generate(float frequency, float sampleRate, long long phaseShiftSamples, float phaseOffsetPercentage = 0.f) override;
        ~TriangleWave() noexcept override;
    };

    using TableVector = std::vector<std::vector<float>>;

    struct WaveTable {
        struct WaveTablePresentation {
            juce::String name;
            std::function<TableVector()> function;
        };

        WaveTable(juce::String name, std::function<TableVector()> function, std::vector<Wave*>&& table)
                : presentation { name, function }, waveTable(std::move(table)) {}

        void resetPhaseOffset() {
            phaseOffset = 0;
        }

        void shiftPhase() {
            ++phaseOffset;
        }

        float generateSample(double frequency, double sampleRate, int waveIndex, float phaseOffsetPercentage) {
            ZoneScoped;
            return waveTable[waveIndex]->generate((float)frequency, (float)sampleRate, phaseOffset, phaseOffsetPercentage);
        }

        WaveTablePresentation presentation;
        polymorphic_readonly_array<Wave, SinWave, SawWave, TriangleWave> waveTable;

    private:
        long long phaseOffset = 0;
    };

    class WaveTables {
    public:
        WaveTables(WaveTables& other) = delete;
        void operator=(const WaveTables &) = delete;
        static WaveTables* getInstance();
        std::vector<WaveTable> copyWaveTables();
        size_t getTotal() const;
        ~WaveTables();
    private:
        static WaveTables* instance;
        static std::vector<WaveTable> waveTables;
        WaveTables();
    };
}