#include "BodyPanel.h"
#include "../other/Grid.h"
#include "../Constants.h"

namespace ui {
    BodyPanel::BodyPanel(juce::AudioProcessorValueTreeState& treeState,
                         const std::vector<juce::String>& paramNamesAbleToModulate,
                         const std::vector<juce::String>& modulatorNames) :
    mainPanel(treeState)/*, matrixPanel(paramNamesAbleToModulate, modulatorNames)*/ {
        addAndMakeVisible(mainPanel);
        auto matrixPanel = new MatrixPanel(paramNamesAbleToModulate, modulatorNames);
        matrixPanel->setBounds(getLocalBounds());
        matrixViewPort.setViewedComponent(matrixPanel);
        addChildComponent(matrixViewPort);
        //addChildComponent(matrixPanel);
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
        matrixViewPort.setBounds(getLocalBounds());
        matrixViewPort.getViewedComponent()->setBounds(getLocalBounds().withHeight(getLocalBounds().getHeight() * 2));
        masterPanel.setBounds(getLocalBounds());
    }

    void BodyPanel::actionListenerCallback (const juce::String& message)
    {
        if (message == MAIN_PANEL) {
            mainPanel.setVisible(true);
            //matrixPanel.setVisible(false);
            matrixViewPort.setVisible(false);
            masterPanel.setVisible(false);
        } else if (message == MATRIX_PANEL) {
            mainPanel.setVisible(false);
            matrixViewPort.setVisible(true);
            masterPanel.setVisible(false);
        } else if (message == MASTER_PANEL) {
            mainPanel.setVisible(false);
            matrixViewPort.setVisible(false);
            masterPanel.setVisible(true);
        }
    }
}