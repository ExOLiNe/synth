#pragma once

#include <vector>
#include "juce_core/juce_core.h"
//#include "../../other/polymorphic_readonly_array.h"
#include <cmath>

namespace audio {
    class Wave {
    public:
        virtual float generate(float frequency, float sampleRate, long long phaseShiftSamples, float phaseOffsetPercentage = 0.f) = 0;
        virtual Wave* clone() = 0;
        virtual ~Wave();
    };


    class SinWave : public Wave {
    public:
        SinWave();
        float generate(float frequency, float sampleRate, long long phaseShiftSamples, float phaseOffsetPercentage = 0.f) override;
        Wave* clone() override;
        ~SinWave();
    private:
        SinWave(const SinWave& other) = default;
    };

    class SawWave : public Wave {
    public:
        SawWave();
        float generate(float frequency, float sampleRate, long long phaseShiftSamples, float phaseOffsetPercentage = 0.f) override;
        Wave* clone() override;
        ~SawWave();
    private:
        SawWave(const SawWave& other) = default;
    };

    using TableVector = std::vector<std::vector<float>>;

    struct WaveTable {
        struct WaveTablePresentation {
            juce::String name;
            std::function<TableVector()> function;
        };
        /*WaveTable(juce::String name, std::function<TableVector()> function)
                : name(name), getWaveTablePresentation(function) {}*/
        WaveTable(juce::String name, std::function<TableVector()> function, std::vector<Wave*>&& table)
                : presentation { name, function }, waveTable(table) {}
        WaveTable(const WaveTable& other) : presentation(other.presentation) {
            waveTable.clear();
            waveTable.reserve(other.waveTable.size());
            for (auto* wave : other.waveTable) {
                waveTable.push_back(wave->clone());
            }
            jassertfalse;
        }
        /*WaveTable(const WaveTable& other) {
            for (auto* wave : other.waveTable) {
                waveTable.push_back(wave->clone());
            }
            DBG("CALL!\n");
        }*/

        void resetPhaseOffset() {
            phaseOffset = 0;
        }

        void shiftPhase() {
            ++phaseOffset;
        }

        float generateSample(double frequency, double sampleRate, int waveIndex, float phaseOffsetPercentage) {
            return waveTable[waveIndex]->generate(frequency, sampleRate, phaseOffset, phaseOffsetPercentage);
        }

        WaveTablePresentation presentation;
        //TODO CREATE SEPARATE CLASS FOR waveTable WITH generateSample and so on ENCAPSULATED IN IT
        std::vector<Wave*> waveTable;

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