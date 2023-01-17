/*
  ==============================================================================

    SpectrumAnalyzer.h
    Created: 12 Jan 2023 6:21:41am
    Author:  Aleksandr

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"

using namespace juce;

static constexpr auto fftOrder = 11;
static constexpr auto fftSize = 1 << fftOrder;
static constexpr auto _scopeSize = 512;

class SpectrumAnalyzer : public ChangeBroadcaster
{
public:
    SpectrumAnalyzer();
    SpectrumAnalyzer(const SpectrumAnalyzer&);
    void processBlock(const AudioBuffer<float>&);
    static constexpr int scopeSize = _scopeSize;
    float scopeData[scopeSize];
private:
    Random random;

    dsp::FFT forwardFFT;
    dsp::WindowingFunction<float> window;
    float fifo[fftSize];
    float fftData[2 * fftSize];
    unsigned int fifoIndex = 0;
    bool nextFFTBlockReady = false;

    void pushNextSampleIntoFifo(const float sample);

    double currAngle = 0;
    int phase = 10;
    int maxPhase = 10;
    int frequency = 440;
    double angleDelta = 0.3;
    int angleNum = 0;
};