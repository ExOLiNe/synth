#include "BodyPanel.h"
#include "../other/Grid.h"
#include "../Constants.h"

BodyPanel::BodyPanel() {
    addAndMakeVisible(mainPanel);
    addChildComponent(matrixPanel);
    addChildComponent(masterPanel);
    juce::MessageManager::getInstance()->registerBroadcastListener(this);
}

BodyPanel::~BodyPanel() noexcept {

}

void BodyPanel::paint(juce::Graphics &g) {
    g.fillAll(juce::Colour::fromString("0xff191919"));
}

void BodyPanel::resized() {
    mainPanel.setBounds(getLocalBounds());
    matrixPanel.setBounds(getLocalBounds());
    masterPanel.setBounds(getLocalBounds());
}

void BodyPanel::actionListenerCallback (const juce::String& message)
{
    if (message == MAIN_PANEL) {
        mainPanel.setVisible(true);
        matrixPanel.setVisible(false);
        masterPanel.setVisible(false);
    } else if (message == MATRIX_PANEL) {
        mainPanel.setVisible(false);
        matrixPanel.setVisible(true);
        masterPanel.setVisible(false);
    } else if (message == MASTER_PANEL) {
        mainPanel.setVisible(false);
        matrixPanel.setVisible(true);
        masterPanel.setVisible(false);
    }
}