//
// Created by Aleksandr on 26-Feb-23.
//

#include "SynthSound.h"

namespace audio {
    bool SynthSound::appliesToNote(int)
    {
        return true;
    }
    bool SynthSound::appliesToChannel(int)
    {
        return true;
    }
}