/*
  ==============================================================================

    SynthVoice.h
    Created: 8 Jan 2023 5:59:17pm
    Author:  Aleksandr

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"
#include "Params.h"
#include "Waves.h"

using namespace juce;
using namespace std;

class SynthVoice : public SynthesiserVoice
{
public:
    SynthVoice(const string id, const AudioProcessorValueTreeState& apvts);
    bool canPlaySound(SynthesiserSound*) override;
    void startNote(int midiNoteNumber,
        float velocity,
        SynthesiserSound* sound,
        int currentPitchWheelPosition) override;
    void stopNote(float velocity, bool allowTailOff) override;
    void pitchWheelMoved(int newPitchWheelValue) override;
    void controllerMoved(int controllerNumber, int newControllerValue) override;
    void renderNextBlock(AudioBuffer<float>& outputBuffer,
        int startSample,
        int numSamples) override;
    void prepareToPlay(double sampleRate, int samplesPerBlock, int ouputChannels);
private:
    void updateParams();

    const string mod;
    //apvts params
    const atomic<float> *attack, *decay, *sustain, *release, *voicesAtomic, *detuneAtomic, 
        *phase, *gainAtomic, *cutOffFrequency, * resonance, *filterMix, 
        *modAttack, *modDecay, *modSustain, *modRelease;
    Value waveTypeFloat;

    double level;
    double frequency = 0;
    double phaseOffset = 0;
    double cutOffFrequencyFactor = 0;

    ADSR gainAdsr, frequencyAdsr;
    ADSR::Parameters gainAdsrParams, frequencyAdsrParams;
    dsp::Gain<float> gain;
    dsp::LadderFilter<float> lowPassFilter;

    vector<Wave> waves;

    const string id;
    
    const AudioProcessorValueTreeState& apvts;

    AudioBuffer<float> tempBuffer;
    AudioBuffer<float> frequencyBuffer;
};