//
// Created by Aleksandr on 19-Feb-23.
//

#include "EnvPanel.h"
#include "../../../Typedefs.h"
#include "../../../Constants.h"

namespace ui {
    EnvPanel::EnvPanel() {
        volumeAdsrButton.setToggleState(true, juce::NotificationType::dontSendNotification);
        volumeAdsrButton.setButtonText("Main ADSRWave");
        volumeAdsrButton.addListener(this);
        addAndMakeVisible(volumeAdsrButton);

        adsr1Button.setButtonText("ADSR1");
        adsr1Button.addListener(this);
        addAndMakeVisible(adsr1Button);

        adsr2Button.setButtonText("ADSR2");
        adsr2Button.addListener(this);
        addAndMakeVisible(adsr2Button);

        lfo1Button.setButtonText("LFO1");
        lfo1Button.addListener(this);
        addAndMakeVisible(lfo1Button);

        lfo2Button.setButtonText("LFO2");
        lfo2Button.addListener(this);
        addAndMakeVisible(lfo2Button);

        addAndMakeVisible(body);
    }

    EnvPanel::~EnvPanel() noexcept {

    }

    void EnvPanel::paint(juce::Graphics &g) {
        g.fillAll(juce::Colour::fromString("0xff191919"));
    }

    void EnvPanel::buttonClicked (juce::Button* button) {
        volumeAdsrButton.setToggleState(false, juce::NotificationType::dontSendNotification);
        adsr1Button.setToggleState(false, juce::NotificationType::dontSendNotification);
        adsr2Button.setToggleState(false, juce::NotificationType::dontSendNotification);
        lfo1Button.setToggleState(false, juce::NotificationType::dontSendNotification);
        lfo2Button.setToggleState(false, juce::NotificationType::dontSendNotification);

        auto messageManager = juce::MessageManager::getInstance();
        if (button == &volumeAdsrButton) {
            messageManager->deliverBroadcastMessage(VOLUME_ADSR);
        } else if (button == &adsr1Button) {
            messageManager->deliverBroadcastMessage(ADSR1);
        }  else if (button == &adsr2Button) {
            messageManager->deliverBroadcastMessage(ADSR2);
        } else if (button == &lfo1Button) {
            messageManager->deliverBroadcastMessage(LFO1);
        } else if (button == &lfo2Button) {
            messageManager->deliverBroadcastMessage(LFO2);
        }
        button->setToggleState(true, juce::NotificationType::dontSendNotification);
    }

    void EnvPanel::resized() {
        using namespace juce;
        Grid grid;
        grid.templateRows = {Track(Fr(1)), Track(Fr(7)) };
        grid.templateColumns = {
                Track(Fr(1)),
                Track(Fr(1)),
                Track(Fr(1)),
                Track(Fr(1)),
                Track(Fr(1))
        };
        grid.items = {
                GridItem(volumeAdsrButton),
                GridItem(adsr1Button),
                GridItem(adsr2Button),
                GridItem(lfo1Button),
                GridItem(lfo2Button),
                GridItem(body).withArea(
                        GridItem::Property(2),
                        GridItem::Property(1),
                        GridItem::Property(2),
                        GridItem::Property(6)
                )
        };

        grid.performLayout(getLocalBounds());
    }
}