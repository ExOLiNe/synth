#pragma once

#if defined(__clang__)
#define CLANG_NO_DESTROY [[clang::no_destroy]]
#else
#define CLANG_NO_DESTROY
#endif

#include <juce_gui_basics.h>

using Track = juce::Grid::TrackInfo;
using Fr = juce::Grid::Fr;