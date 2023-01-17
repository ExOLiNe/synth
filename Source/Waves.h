/*
  ==============================================================================

    Waves.h
    Created: 11 Jan 2023 5:42:59pm
    Author:  Aleksandr

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"

using namespace juce;

#define TWO_PI MathConstants<float>::twoPi

class Wave
{
public:
    Wave(double sampleRate)
    {
        this->sampleRate = sampleRate;
    }
    void reset()
    {
        phase = 0;
    }
    void phaseShift(double offset)
    {
        phase += offset;
    }
    double generateSine(double frequency)
    {
        output = sin(phase * (TWO_PI));
        if (phase >= 1.0) phase -= 1.0;
        phase += (1. / (sampleRate / (frequency)));
        return output;
    }
    double generateTriangle(double frequency)
    {
        if (phase >= 1.0) phase -= 1.0;
        phase += (1. / (sampleRate / (frequency)));
        if (phase <= 0.5) {
            output = (phase - 0.25) * 4;
        }
        else {
            output = ((1.0 - phase) - 0.25) * 4;
        }
        return output;
    }
    double generateSaw(double frequency)
    {
        output = phase;
        if (phase >= 1.0) phase -= 2.0;
        phase += (1. / (sampleRate / (frequency))) * 2.0;
        return output;
    }
    double generateNoise()
    {
        float r = random.nextFloat();
        output = r * 2 - 1;
        return output;
    }
    double generateSquare(double frequency)
    {
        if (phase < 0.5) output = -1;
        if (phase > 0.5) output = 1;
        if (phase >= 1.0) phase -= 1.0;
        phase += (1. / (sampleRate / (frequency)));
        return output;
    }
private:
    double sampleRate;
    double phase = 0;
    double output;
    Random random;
};

/*#define WAVE_HOLDER_FACTORY(NAME, BODY) \
struct NAME##Holder { \
    static double generateSample(Random random, double output, double phase, double sampleRate, double frequency) \
    { \
        BODY \
    } \
}; \
typedef Wave<NAME##Holder> NAME;

WAVE_HOLDER_FACTORY(SineWave, {
        output = sin(phase * (TWO_PI));
        if (phase >= 1.0) phase -= 1.0;
        phase += (1. / (sampleRate / (frequency)));
        return output;
    });

WAVE_HOLDER_FACTORY(SquareWave, {
    output = sin(phase * (TWO_PI));
    if (phase >= 1.0) phase -= 1.0;
    phase += (1. / (sampleRate / (frequency)));
    return output;
    });

WAVE_HOLDER_FACTORY(NoiseWave, {
    float r = random.nextFloat();
    output = r * 2 - 1;
    return output;
    });

WAVE_HOLDER_FACTORY(SawWave, {
    output = phase;
    if (phase >= 1.0) phase -= 2.0;
    phase += (1. / (sampleRate / (frequency))) * 2.0;
    return output;
    });

WAVE_HOLDER_FACTORY(TriangleWave, {
    if (phase >= 1.0) phase -= 1.0;
    phase += (1. / (sampleRate / (frequency)));
    if (phase <= 0.5) {
        output = (phase - 0.25) * 4;
    }
    else {
        output = ((1.0 - phase) - 0.25) * 4;
    }
    return output;
    });
    */