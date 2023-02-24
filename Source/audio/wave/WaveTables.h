#pragma once

#include <vector>
#include "juce_core/juce_core.h"
#include <cmath>

namespace audio {
    class Wave {
    public:
        void shiftPhase(float offset);

        virtual float generate(float frequency, float sampleRate) = 0;
        virtual ~Wave();

    protected:
        float output = 0.f;
        float phase = 0.f;
    };


    class SinWave : public Wave {
    public:
        float generate(float frequency, float sampleRate) override;
        ~SinWave();
    };

    class SawWave : public Wave {
    public:
        float generate(float frequency, float sampleRate) override;
        ~SawWave();
    };

    using TableVector = std::vector<std::vector<float>>;

    struct WaveTable {

        //WaveTable(WaveTable &&other);

        WaveTable(juce::String name, std::function<TableVector()> presentation, const std::initializer_list<Wave*>& waves);

        juce::String name;
        std::function<TableVector()> getWaveTablePresentation;
        juce::OwnedArray<Wave> waveTable;
    };

    class WaveTables {
    public:
        static std::vector<WaveTable> getWaveTables();
    };
}