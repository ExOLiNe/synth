#pragma once

#include <atomic>

namespace audio {
    class LFO {
    public:
        LFO(std::atomic<float>* lfoValue, const std::atomic<float>* frequency);
        void shiftPhase(int numSamples, float sampleRate);
    private:
        long long phase = 0;
        std::atomic<float>* lfoValue;
        const std::atomic<float>* frequency;
    };
}
