#pragma once

#include <chrono>
#include <juce_core/juce_core.h>

using namespace std::chrono;
class HighFrequencyLogger {
public:
    using T = double;
    explicit HighFrequencyLogger(int durationMillis = 1000) : durationMillis(durationMillis) {
        resetMetrics();
    }
    HighFrequencyLogger(const HighFrequencyLogger&) = delete;
    HighFrequencyLogger(HighFrequencyLogger&&) = delete;

    void log(T value) {
        lastTimeCalled = high_resolution_clock::now();

        if (value > maxValue) {
            maxValue = value;
        }
        if (value < minValue) {
            minValue = value;
        }

        averageValue = (averageValue * callsBeforeLog + value) / (callsBeforeLog + 1);

        ++callsBeforeLog;

        if (duration_cast<milliseconds>(lastTimeCalled - lastTimeLogged).count() > durationMillis) {
            DBG(averageText + std::to_string(averageValue));
            DBG(maxText + std::to_string(maxValue));
            DBG(minText + std::to_string(minValue));
            resetMetrics();
        }
    }

    void resetMetrics() {
        maxValue = std::numeric_limits<T>::min();
        minValue = std::numeric_limits<T>::max();
        averageValue = T();
        callsBeforeLog = 0;
        lastTimeLogged = lastTimeCalled;
    }
private:
    juce::String averageText = "average: ";
    juce::String maxText = "max: ";
    juce::String minText = "min: ";
    using Time = time_point<steady_clock>;
    int durationMillis;
    Time lastTimeCalled = high_resolution_clock::now();
    Time lastTimeLogged;
    T maxValue;
    T minValue;
    T averageValue;
    long callsBeforeLog = 0;
};