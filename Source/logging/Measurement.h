#pragma once

#include <chrono>
#include <juce_core.h>

using namespace std::chrono;

template<int durationSeconds = 1>
class Measurement {
public:
    explicit Measurement() {
        resetMetrics();
    }
    Measurement(const Measurement&) = delete;
    Measurement(Measurement&&) = delete;

    void start() {
        startTime = now();
    }

    void end() {
        lastTimeCalled = now();

        Duration executionTime = lastTimeCalled - startTime;

        if (executionTime > maxExecutionTime) {
            maxExecutionTime = executionTime;
        }
        if (executionTime < minExecutionTime) {
            minExecutionTime = executionTime;
        }

        averageExecutionTime = (averageExecutionTime * callsBeforeLog + executionTime) / (callsBeforeLog + 1);

        ++callsBeforeLog;

        if (duration_cast<seconds>(lastTimeCalled - lastTimeLogged).count() > durationSeconds) {
            DBG(averageText + std::to_string(getNanos(averageExecutionTime)));
            DBG(maxText + std::to_string(getNanos(maxExecutionTime)));
            DBG(minText + std::to_string(getNanos(minExecutionTime)));
            DBG("\n");
            resetMetrics();
        }
    }

    void resetMetrics() {
        minExecutionTime = maxExecutionTime * std::numeric_limits<::uint8_t>::max();
        maxExecutionTime = zeroExecutionTime;
        averageExecutionTime = zeroExecutionTime;
        callsBeforeLog = 0;
        lastTimeLogged = now();
    }
private:
    using Time = time_point<steady_clock>;
    using Duration = Time::duration;//decltype(std::declval<Time>() - std::declval<Time>());
    using Unit = nanoseconds;
    using CountType = Duration::rep;//decltype(duration_cast<Unit>(std::declval<Duration>()).count());

    CountType getNanos(const Duration& duration) {
        return duration_cast<Unit>(duration).count();
    }

    static Time now() {
        return high_resolution_clock::now();
    }

    juce::String averageText = "average: ";
    juce::String maxText = "max: ";
    juce::String minText = "min: ";
    Time startTime = now();
    Time lastTimeCalled = now();
    Time lastTimeLogged;
    Duration maxExecutionTime;
    Duration minExecutionTime;
    Duration averageExecutionTime;
    const Duration zeroExecutionTime = startTime - lastTimeCalled;
    long callsBeforeLog = 0;
};