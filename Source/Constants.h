#pragma once

#define TOP_MENU_TOPIC "top_menu.panel"
#define MAIN_PANEL TOP_MENU_TOPIC".main"
#define MATRIX_PANEL TOP_MENU_TOPIC".matrix"
#define MASTER_PANEL TOP_MENU_TOPIC".master"

#define ENV_MENU_TOPIC "env_menu.panel"
#define VOLUME_ADSR ENV_MENU_TOPIC".volume_adsr"
#define ADSR1 ENV_MENU_TOPIC".adsr1"
#define ADSR2 ENV_MENU_TOPIC".adsr2"
#define LFO1 ENV_MENU_TOPIC".lfo1"
#define LFO2 ENV_MENU_TOPIC".lfo2"


#define OSC1 "osc1"
#define OSC2 "osc2"

#define FM_A "fmA"
#define FM_B "fmB"

#define FILTER_A "filterA"
#define FILTER_B "filterB"

#define ADSR_VOLUME "volumeADSR"
#define ADSR_1 "ADSR1"
#define ADSR_2 "ADSR2"
#define LFO_1 "LFO1"
#define LFO_2 "LFO2"


using Str = const char*;

namespace params {
    template <typename Type>
    struct Param {
        Str name;
        Type minValue;
        Type maxValue;
        Type defaultValue;
    };

    struct BoolParam {
        Str name;
        bool defaultValue;
    };

    namespace osc {
        extern Str waveTypeName;

        extern Param<unsigned int> voices;
        extern Param<float> detune;
        extern Param<unsigned int> phase;
        extern Param<float> wtPos;
        extern Param<int> pan;
        extern Param<unsigned int> level;
        extern Param<int> semitone;
        extern Param<float> fine;
    }

    namespace fm {
        extern Param<float> freq;
        extern Param<unsigned int> mix;
        extern BoolParam enabled;
    }

    namespace filter {
        extern Param<float> freq;
        extern Param<float> reso;
        extern BoolParam enabled;
    }

    namespace adsr {
        extern Param<float> attack;
        extern Param<float> decay;
        extern Param<float> sustain;
        extern Param<float> release;
    }

    namespace lfo {
        extern Param<float> freq;
        extern Param<float> amp;
    }
}