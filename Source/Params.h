/*
  ==============================================================================

    Params.h
    Created: 7 Jan 2023 6:40:52pm
    Author:  Aleksandr

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

using ParameterLayoutVector = std::vector<std::unique_ptr<juce::RangedAudioParameter>>;

enum WaveType
{
    EMPTY, // not usable! only for reserved 0 index
    SINE,
    SQUARE,
    TRIANGLE,
    SAW,
    NOISE
};

#define GAIN_FACTOR 0.2f

#define __WAVE_SELECTOR_ID_POSTFIX "_waveSelector"
#define __WAVE_SELECTOR_DEFAULT_WAVE WaveType::SINE

#define __OSC_VOLUME_ID_POSTFIX "_volume"
#define __OSC_VOLUME_MIN_VALUE -50
#define __OSC_VOLUME_MAX_VALUE 0
#define __OSC_VOLUME_DEFAULT 0

#define __OSC_DETUNE_ID_POSTFIX "_detune"
#define __OSC_DETUNE_MIN_VALUE 0
#define __OSC_DETUNE_MAX_VALUE 30
#define __OSC_DETUNE_DEFAULT __OSC_DETUNE_MIN_VALUE

#define __OSC_PHASE_ID_POSTFIX "_phase"
#define __OSC_PHASE_MIN_VALUE 0
#define __OSC_PHASE_MAX_VALUE 180
#define __OSC_PHASE_DEFAULT __OSC_PHASE_MIN_VALUE

#define __OSC_VOICES_ID_POSTFIX "_voices"
#define __OSC_VOICES_MIN_VALUE 1
#define __OSC_VOICES_MAX_VALUE 5
#define __OSC_VOICES_DEFAULT __OSC_VOICES_MIN_VALUE


#define __ADSR_ATTACK_ID_POSTFIX "_attack"
#define __ADSR_ATTACK_MIN_VALUE 0.0f
#define __ADSR_ATTACK_MAX_VALUE 3.0f
#define __ADSR_ATTACK_DEFAULT __ADSR_ATTACK_MIN_VALUE

#define __ADSR_DECAY_ID_POSTFIX "_decay"
#define __ADSR_DECAY_MIN_VALUE 0.0f
#define __ADSR_DECAY_MAX_VALUE 1.0f
#define __ADSR_DECAY_DEFAULT __ADSR_DECAY_MIN_VALUE + 0.01f

#define __ADSR_SUSTAIN_ID_POSTFIX "_sustain"
#define __ADSR_SUSTAIN_MIN_VALUE 0.0f
#define __ADSR_SUSTAIN_MAX_VALUE 10.0f
#define __ADSR_SUSTAIN_DEFAULT __ADSR_SUSTAIN_MIN_VALUE

#define __ADSR_RELEASE_ID_POSTFIX "_release"
#define __ADSR_RELEASE_MIN_VALUE 0.0f
#define __ADSR_RELEASE_MAX_VALUE 10.0f
#define __ADSR_RELEASE_DEFAULT __ADSR_RELEASE_MIN_VALUE


#define __FILTER_CUTOFF_ID "lowpass"
#define __FILTER_CUTOFF_MIN_VALUE 20.0f
#define __FILTER_CUTOFF_MAX_VALUE 20000.0f
#define __FILTER_CUTOFF_DEFAULT __FILTER_CUTOFF_MAX_VALUE

#define __FILTER_RESONANSE_ID "reso"
#define __FILTER_RESONANSE_MIN_VALUE 0.0f
#define __FILTER_RESONANSE_MAX_VALUE 1.0f
#define __FILTER_RESONANSE_DEFAULT 0.0f

#define __FILTER_MIX_ID "mix"
#define __FILTER_MIX_MIN_VALUE 1.0
#define __FILTER_MIX_MAX_VALUE 100.0
#define __FILTER_MIX_DEFAULT __FILTER_MIX_MIN_VALUE

// ======= CONCRETE VALUES =======

#define OSC1_ID "osc1"
#define OSC1_NAME OSC1_ID


#define OSC2_ID "osc2"
#define OSC2_NAME OSC2_ID

#define MOD "mod"

#define ADSR1_ID "adsr1"

#include <sstream>
#include "JuceHeader.h"

#ifdef LOG_JUMPS_ENABLED

#define LOG_JUMPS(PTR, CHANNEL, NUM_SAMPLES, MSG) {\
    for (int i = 0; i < NUM_SAMPLES; ++i)\
    {\
        if (PTR[i] > 1)\
        {\
            std::stringstream ss;\
            ss << "JUMP channel: " << CHANNEL << " value: " << PTR[i] << " flat: " << MSG;\
            juce::Logger::getCurrentLogger()->writeToLog(ss.str());\
        }\
    }\
}

#else

#define LOG_JUMPS(PTR, CHANNEL, NUM_SAMPLES, MSG)

#endif // DEBUG