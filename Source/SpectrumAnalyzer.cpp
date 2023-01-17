/*
  ==============================================================================

    SpectrumAnalyzer.cpp
    Created: 12 Jan 2023 6:21:41am
    Author:  Aleksandr

  ==============================================================================
*/

#include "SpectrumAnalyzer.h"

SpectrumAnalyzer::SpectrumAnalyzer() : random(juce::Random())
, forwardFFT(fftOrder)
, window(fftSize, dsp::WindowingFunction<float>::hann)
{

}

SpectrumAnalyzer::SpectrumAnalyzer(const SpectrumAnalyzer&) : random(juce::Random())
, forwardFFT(fftOrder)
, window(fftSize, dsp::WindowingFunction<float>::hann)
{

}

void SpectrumAnalyzer::processBlock(const AudioBuffer<float>& buffer)
{
    for (int channel = 0; channel < buffer.getNumChannels(); ++channel)
    {
        auto* channelData = buffer.getReadPointer(channel);

        if (channel == 0)
        {
            for (int i = 0; i < buffer.getNumSamples(); ++i)
            {
                pushNextSampleIntoFifo(channelData[i]);
            }
        }
    }

    if (nextFFTBlockReady)
    {
        window.multiplyWithWindowingTable(fftData, fftSize);
        forwardFFT.performFrequencyOnlyForwardTransform(fftData);

        auto minDb = -100.0f;
        auto maxDb = 0.0f;

        for (int i = 0; i < scopeSize; ++i)
        {
            auto skewedProportionX = 1.0f - exp(log(1.0f - (float)i / (float)scopeSize) * 0.2f);
            auto fftDataIndex = jlimit(0, fftSize / 2, (int)(skewedProportionX * (float)fftSize * 0.5f));
            auto level = jmap(jlimit(minDb, maxDb, Decibels::gainToDecibels(fftData[fftDataIndex]) - Decibels::gainToDecibels((float)fftSize)),
                minDb, maxDb, 0.0f, 1.0f);
            scopeData[i] = level;
        }
        nextFFTBlockReady = false;
        sendChangeMessage();
    }
}

void SpectrumAnalyzer::pushNextSampleIntoFifo(const float sample)
{
    if (fifoIndex == fftSize)
    {
        if (!nextFFTBlockReady)
        {
            juce::zeromem(fftData, sizeof(fftData));
            memcpy(fftData, fifo, sizeof(fifo));
            nextFFTBlockReady = true;
        }

        fifoIndex = 0;
    }

    fifo[fifoIndex++] = sample;
}