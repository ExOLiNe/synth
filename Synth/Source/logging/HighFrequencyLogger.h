#pragma once

#include <chrono>
#include "juce_core.h"

using namespace std::chrono;

static auto& defaultLogger = std::cout;

template<typename LoggerType = decltype(defaultLogger)>
class HighFrequencyLogger {
public:
    using T = double;
    explicit HighFrequencyLogger(int durationMillis = 1000, LoggerType logger = defaultLogger)
    : durationMillis(durationMillis), logger(logger) {
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
            logger << averageText + std::to_string(averageValue) << "\n";
            logger << maxText + std::to_string(maxValue) << "\n";
            logger << minText + std::to_string(minValue) << "\n";
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
    LoggerType& logger;
};