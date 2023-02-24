//
// Created by Aleksandr on 19-Feb-23.
//

#include "Body.h"
#include "../../../../Constants.h"

namespace ui {
    Body::Body() {
        addAndMakeVisible(volumeAdsr);
        addChildComponent(adsr1);
        addChildComponent(adsr2);
        addChildComponent(lfo1);
        addChildComponent(lfo2);
        juce::MessageManager::getInstance()->registerBroadcastListener(this);
    }

    Body::~Body() noexcept {

    }

    void Body::paint(juce::Graphics &g) {
        g.fillAll(juce::Colour::fromString("0xff195555"));
    }

    void Body::resized() {
        volumeAdsr.setBounds(getLocalBounds());
        adsr1.setBounds(getLocalBounds());
        adsr2.setBounds(getLocalBounds());
        lfo1.setBounds(getLocalBounds());
        lfo2.setBounds(getLocalBounds());
    }

    void Body::actionListenerCallback (const juce::String& message) {
        if (message == VOLUME_ADSR) {
            volumeAdsr.setVisible(true);
            adsr1.setVisible(false);
            adsr2.setVisible(false);
            lfo1.setVisible(false);
            lfo2.setVisible(false);
        } else if (message == ADSR1) {
            volumeAdsr.setVisible(false);
            adsr1.setVisible(true);
            adsr2.setVisible(false);
            lfo1.setVisible(false);
            lfo2.setVisible(false);
        } else if (message == ADSR2) {
            volumeAdsr.setVisible(false);
            adsr1.setVisible(false);
            adsr2.setVisible(true);
            lfo1.setVisible(false);
            lfo2.setVisible(false);
        } else if (message == LFO1) {
            volumeAdsr.setVisible(false);
            adsr1.setVisible(false);
            adsr2.setVisible(false);
            lfo1.setVisible(true);
            lfo2.setVisible(false);
        } else if (message == LFO2) {
            volumeAdsr.setVisible(false);
            adsr1.setVisible(false);
            adsr2.setVisible(false);
            lfo1.setVisible(false);
            lfo2.setVisible(true);
        }
    }
}