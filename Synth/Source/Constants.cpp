#include "Constants.h"
#include "defs.h"

namespace params {
    CLANG_NO_DESTROY std::string volumeADSRName(ADSR_VOLUME);
    namespace osc {
        Str waveTableTypeName = "waveTableType";
        Param<unsigned int> voices {"voices", 1, 12, 1};
        Param<float> detune {"detune", 0.f, static_cast<float>(TOTAL_DETUNE_VOICES), 0.f };
        Param<unsigned int> phase { "phase", 0, 100, 0 };
        Param<float> wtPos { "wtPos", 0.f, 100.0f, 50.0f };
        Param<float> pan { "pan", -1.0f, 1.0f, 0 };
        Param<unsigned int> level { "level", 0, 100, 0 };
        Param<int> semitone { "semitone", -24, 24, 0 };
        Param<float> fine { "fine", -100.f, 100.f, 0.0f };
    }

    namespace fm {
        Param<float> freq { "freq", 20.f, 16000.f, 800.f };
        Param<unsigned int> mix { "mix", 0, 100, 0 };
        BoolParam enabled { "enabled", false};
    }

    namespace filter {
        Param<float> freq { "freq", 0.001f, 100.f, 100.f };
        Param<float> reso { "reso", 0.f, 100.f, 0.f };
        BoolParam enabled { "enabled", false };
    }

    namespace adsr {
        Param<float> attack { "attack", 0.1f, 100.f, 0.1f };
        Param<float> decay { "decay", 0.1f, 100.f, 30.0f };
        Param<float> sustain { "sustain", 1.f, 20.f, 1.f };
        Param<float> release { "release", 1.f, 100.f, 10.0f };

        float attackFactor { 0.03f };
        float decayFactor { 0.03f };
        float sustainFactor { 1.0f };
        float releaseFactor { 0.01f };
    }

    namespace lfo {
        Param<float> freq { "freq", 0.5f, 32.f, 4.f };
        Param<float> amp { "amp", 0.f, 100.f, 20.f };
    }
}

#ifdef PROFILING_ENABLED
void* operator new(std::size_t count) {
    auto ptr = malloc(count);
    TracyAlloc(ptr, count);
    return ptr;
}

void operator delete(void* ptr) noexcept {
    TracyFree (ptr);
    free(ptr);
}
#endif
