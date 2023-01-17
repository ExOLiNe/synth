/*
  ==============================================================================

    SynthVoice.cpp
    Created: 8 Jan 2023 5:59:17pm
    Author:  Aleksandr

  ==============================================================================
*/

#include "SynthVoice.h"
#include "SynthSound.h"

static const float jump = 0.5f;

SynthVoice::SynthVoice(std::string id, AudioProcessorValueTreeState& apvts, int i) : 
    apvts(apvts),
    id(id),
    mod(MOD),
    modAttack(apvts.getRawParameterValue(mod + __ADSR_ATTACK_ID_POSTFIX)),
    modDecay(apvts.getRawParameterValue(mod + __ADSR_DECAY_ID_POSTFIX)),
    modSustain(apvts.getRawParameterValue(mod + __ADSR_SUSTAIN_ID_POSTFIX)),
    modRelease(apvts.getRawParameterValue(mod + __ADSR_RELEASE_ID_POSTFIX)),
    attack(apvts.getRawParameterValue(id + __ADSR_ATTACK_ID_POSTFIX)),
    decay(apvts.getRawParameterValue(id + __ADSR_DECAY_ID_POSTFIX)),
    sustain(apvts.getRawParameterValue(id + __ADSR_SUSTAIN_ID_POSTFIX)),
    release(apvts.getRawParameterValue(id + __ADSR_RELEASE_ID_POSTFIX)),
    waveTypeFloat(apvts.getParameterAsValue(id + __WAVE_SELECTOR_ID_POSTFIX)),
    voicesAtomic(apvts.getRawParameterValue(id + __OSC_VOICES_ID_POSTFIX)),
    detuneAtomic(apvts.getRawParameterValue(id + __OSC_DETUNE_ID_POSTFIX)),
    phase(apvts.getRawParameterValue(id + __OSC_PHASE_ID_POSTFIX)),
    gainAtomic(apvts.getRawParameterValue(id + __OSC_VOLUME_ID_POSTFIX)),
    cutOffFrequency(apvts.getRawParameterValue( __FILTER_CUTOFF_ID)),
    resonance(apvts.getRawParameterValue(__FILTER_RESONANSE_ID)),
    filterMix(apvts.getRawParameterValue(__FILTER_MIX_ID))
{
}

void SynthVoice::prepareToPlay(double sampleRate, int samplesPerBlock, int ouputChannels)
{
    for (int i = 0; i < __OSC_VOICES_MAX_VALUE; ++i)
    {
        waves.emplace_back(sampleRate);
    }
    dsp::ProcessSpec spec = { sampleRate, samplesPerBlock, ouputChannels };

    gainAdsr.setSampleRate(sampleRate);
    frequencyAdsr.setSampleRate(sampleRate);

    gain.prepare(spec);
    gain.reset();
    lowPassFilter.prepare(spec);
    lowPassFilter.reset();
}

bool SynthVoice::canPlaySound(SynthesiserSound* sound)
{
    return dynamic_cast<SynthSound*>(sound) != nullptr;
}
void SynthVoice::startNote(int midiNoteNumber,
    float velocity,
    SynthesiserSound* sound,
    int currentPitchWheelPosition)
{
    frequency = MidiMessage::getMidiNoteInHertz(midiNoteNumber);
    gainAdsr.noteOn();
    frequencyAdsr.noteOn();
}
void SynthVoice::stopNote(float velocity, bool allowTailOff)
{
    gainAdsr.noteOff();
    frequencyAdsr.noteOff();
}
void SynthVoice::pitchWheelMoved(int newPitchWheelValue)
{

}
void SynthVoice::controllerMoved(int controllerNumber, int newControllerValue)
{

}
void SynthVoice::renderNextBlock(AudioBuffer<float>& outputBuffer, int startSample, int numSamples)
{
    tempBuffer.clear();

    if (tempBuffer.getNumSamples() < numSamples)
    {
        tempBuffer.setSize(outputBuffer.getNumChannels(), numSamples);
        frequencyBuffer.setSize(outputBuffer.getNumChannels(), numSamples);
    }

    updateParams();

    frequencyAdsr.applyEnvelopeToBuffer(tempBuffer, startSample, numSamples);

    WaveType waveType = WaveType(static_cast<int>(waveTypeFloat.getValue()));

    auto detune = detuneAtomic->load();
    auto phaseOffsetToApply = phase->load() - phaseOffset;
    phaseOffset = phaseOffset + phaseOffsetToApply;

    auto block = dsp::AudioBlock<float>(tempBuffer);
    auto pcr = dsp::ProcessContextReplacing<float>(block);

    auto tempPtrLeft = tempBuffer.getWritePointer(0);
    auto tempPtrRight = tempBuffer.getWritePointer(1);

    auto outputPtrLeft = outputBuffer.getWritePointer(0);
    auto outputPtrRight = outputBuffer.getWritePointer(1);

    LOG_JUMPS(outputPtrLeft, 0, numSamples, -1);
    LOG_JUMPS(outputPtrRight, 1, numSamples, -1);

    LOG_JUMPS(tempPtrLeft, 0, numSamples, 0);
    LOG_JUMPS(tempPtrRight, 1, numSamples, 0);

    for (int i = 0; i < numSamples; i++)
    {
        double wave = 0.0;

        auto voices = voicesAtomic->load();
        for (int j = 0; j < voices; j++)
        {
            double factor = pow(exp(log(2) / 1200), detune * (voices / 2 - j));
            waves[j].phaseShift(phaseOffsetToApply);
            switch (waveType)
            {
            case WaveType::SINE:
                wave += waves[j].generateSine(frequency * factor);
                break;
            case WaveType::SQUARE:
                wave += waves[j].generateSquare(frequency * factor);
                break;
            case WaveType::TRIANGLE:
                wave += waves[j].generateTriangle(frequency * factor);
                break;
            case WaveType::SAW:
                wave += waves[j].generateSaw(frequency * factor);
                break;
            case WaveType::NOISE:
                wave += waves[j].generateNoise();
                break;
            }
        }

        wave /= voicesAtomic->load();

        tempPtrLeft[i] = wave;
        tempPtrRight[i] = wave;
    }

    LOG_JUMPS(tempPtrLeft, 0, numSamples, 1);
    LOG_JUMPS(tempPtrRight, 1, numSamples, 1);

    
    cutOffFrequencyFactor = frequencyAdsr.getNextSample();
    if (cutOffFrequencyFactor >= 1.0f)
    {
        cutOffFrequencyFactor = 0.9f;
    }
    else if(cutOffFrequencyFactor <= 0.0f)
    {
        cutOffFrequencyFactor = 0.0f;
    }
    auto blabla = cutOffFrequency->load() * (1.0 - cutOffFrequencyFactor);
    lowPassFilter.setCutoffFrequencyHz(blabla );
    lowPassFilter.setResonance(resonance->load());
    lowPassFilter.setDrive(filterMix->load());

    lowPassFilter.process(pcr);

    LOG_JUMPS(tempPtrLeft, 0, numSamples, 2);
    LOG_JUMPS(tempPtrRight, 1, numSamples, 2);

    gain.process(pcr);

    LOG_JUMPS(tempPtrLeft, 0, numSamples, 3);
    LOG_JUMPS(tempPtrRight, 1, numSamples, 3);

    gainAdsr.applyEnvelopeToBuffer(tempBuffer, 0, numSamples);

    LOG_JUMPS(tempPtrLeft, 0, numSamples, 4);
    LOG_JUMPS(tempPtrRight, 1, numSamples, 4);

    // output
    

    for (int i = 0; i < numSamples; i++)
    {
        outputPtrLeft[i + startSample] += tempPtrLeft[i];
        outputPtrRight[i + startSample] += tempPtrRight[i];
    }

    LOG_JUMPS(outputPtrLeft, 0, numSamples, 5);
    LOG_JUMPS(outputPtrRight, 1, numSamples, 5);

}

void SynthVoice::updateParams()
{
    gain.setGainLinear(
        Decibels::decibelsToGain<float>(gainAtomic->load()) * GAIN_FACTOR
    );
    gainAdsrParams.attack = attack->load();
    gainAdsrParams.decay = decay->load();
    gainAdsrParams.sustain = sustain->load();
    gainAdsrParams.release = release->load();
    gainAdsr.setParameters(gainAdsrParams);

    frequencyAdsrParams.attack = modAttack->load();
    frequencyAdsrParams.decay = modDecay->load();
    frequencyAdsrParams.sustain = modSustain->load();
    frequencyAdsrParams.release = modRelease->load();
    frequencyAdsr.setParameters(frequencyAdsrParams);
}