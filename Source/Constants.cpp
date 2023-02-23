#include "Constants.h"

namespace params {
    namespace osc {
        Param<unsigned int> voices {"voices", 1, 12, 1};
        Param<float> detune {"detune", 0.f, 12.f, 0.f };
        Param<unsigned int> phase { "phase", 0, 100, 0 };
        Param<float> wtPos { "wtPos", 0.f, 100.0f, 0.0f };
        Param<int> pan { "pan", -50, 50, 0 };
        Param<unsigned int> level { "level", 0, 100, 0 };
        Param<int> semitone { "semitone", -36, 36, 0 };
        Param<float> fine { "fine", -100.f, 100.f, 0.0f };
    }

    namespace fm {
        Param<float> freq { "freq", 20.f, 16000.f, 800.f };
        Param<unsigned int> mix { "mix", 0, 100, 0 };
    }

    namespace filter {
        Param<float> freq { "freq", 20.f, 16000.f, 800.f };
        Param<float> reso { "reso", 0.f, 100.f, 0.f };
    }

    namespace adsr {
        Param<float> freq { "freq", 20.f, 16000.f, 800.f };
        Param<float> amp { "amp", 0.f, 100.f, 0.f };
        Param<unsigned int> mix { "mix", 0, 100, 0 };
    }

    namespace lfo {
        Param<float> freq { "freq", 20.f, 16000.f, 800.f };
        Param<float> amp { "amp", 0.f, 100.f, 0.f };
        Param<unsigned int> mix { "mix", 0, 100, 0 };
    }
}