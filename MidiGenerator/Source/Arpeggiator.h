#pragma once

#include "RingIndex.h"

using type = short;

template<type bottomLimit, type upperLimit, type number, type ...numbers>
struct sequence_in_within {
    constexpr static bool value =
        number > bottomLimit &&
        number < upperLimit && sequence_in_within<bottomLimit, upperLimit, numbers...>::value;
};

template<type bottomLimit, type upperLimit, type number>
struct sequence_in_within<bottomLimit, upperLimit, number> {
    constexpr static bool value = number > bottomLimit && number < upperLimit;
};


template<typename type>
class Arpeggiator {
private:
    juce::SortedSet<juce::MidiMessage> notes;
public:
    void tryPlay(juce::MidiBuffer& midiMessages) {
        for (const auto& midi : midiMessages) {
            auto message = midi.getMessage();
            if (message.isNoteOn()) {
                notes.add(message);
            } else if (message.isNoteOff()) {
                notes.remove();
            }
        }
    }
};