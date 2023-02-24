//
// Created by Aleksandr on 19-Feb-23.
//

#include "MenuComponent.h"
#include "../../Constants.h"
#include "../../other/Grid.h"

namespace ui {
    MenuComponent::MenuComponent() {
        main.setButtonText("main");
        main.addListener(this);
        addAndMakeVisible(main);
        matrix.setButtonText("matrix");
        matrix.addListener(this);
        addAndMakeVisible(matrix);
        master.setButtonText("master");
        master.addListener(this);
        addAndMakeVisible(master);
    }

    MenuComponent::~MenuComponent() noexcept {

    }

    void MenuComponent::paint(juce::Graphics &g) {
        g.fillAll(juce::Colours::black);
        g.setColour(juce::Colours::white);
        g.setFont(15.0f);
        g.drawFittedText("MENU", getLocalBounds(), juce::Justification::centred, 1);
    }

    void MenuComponent::resized() {
        Grid grid({ 1 }, { 1, 1, 1});
        grid.setItems({&main, &matrix, &master});
        grid.performLayout(getLocalBounds());
    }

    void MenuComponent::buttonStateChanged (juce::Button*){}

    void MenuComponent::buttonClicked(juce::Button *button) {
        auto messageManager = juce::MessageManager::getInstance();

        main.setToggleState(false, juce::NotificationType::dontSendNotification);
        matrix.setToggleState(false, juce::NotificationType::dontSendNotification);
        master.setToggleState(false, juce::NotificationType::dontSendNotification);

        if (button == &main) {
            messageManager->deliverBroadcastMessage(MAIN_PANEL);
        } else if (button == &matrix) {
            messageManager->deliverBroadcastMessage(MATRIX_PANEL);
        } else if (button == &master) {
            messageManager->deliverBroadcastMessage(MASTER_PANEL);
        }
        button->setToggleState(true, juce::NotificationType::dontSendNotification);
    }
}