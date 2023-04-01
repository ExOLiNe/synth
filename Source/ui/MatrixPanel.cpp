//
// Created by Aleksandr on 19-Feb-23.
//

#include "MatrixPanel.h"
#include "../Typedefs.h"

namespace ui {
    MatrixPanel::MatrixPanel(const std::vector<juce::String>& paramNamesAbleToModulate,
                             const std::vector<juce::String>& modulatorNames)
    : paramNamesAbleToModulate(paramNamesAbleToModulate) {
        sourceLabel.setText("Source", juce::NotificationType::dontSendNotification);
        sourceLabel.setFont(juce::Font().boldened());
        addAndMakeVisible(sourceLabel);

        destinationLabel.setText("Destination", juce::NotificationType::dontSendNotification);
        destinationLabel.setFont(juce::Font().boldened());
        addAndMakeVisible(destinationLabel);

        intensityLabel.setText("Intensity", juce::NotificationType::dontSendNotification);
        intensityLabel.setFont(juce::Font().boldened());
        addAndMakeVisible(intensityLabel);

        for (size_t i = 0; i < paramNamesAbleToModulate.size(); ++i) {
            auto source = new juce::Label(paramNamesAbleToModulate[i], paramNamesAbleToModulate[i]);
            source->setText(paramNamesAbleToModulate[i], juce::NotificationType::dontSendNotification);
            addAndMakeVisible(source);
            sourceLabels.add(source);

            auto destination = new juce::ComboBox("combo");
            destination->addItem("---------", i * modulatorNames.size() + 1);
            for (size_t j = 0; j < modulatorNames.size(); ++j) {
                destination->addItem(modulatorNames[j], i * modulatorNames.size() + j + 2);
            }
            destination->setSelectedItemIndex(0);
            addAndMakeVisible(destination);
            destinations.add(destination);

            auto knob = new juce::Slider();
            knob->setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
            addAndMakeVisible(knob);
            intensityKnobs.add(knob);
        }
    }

    MatrixPanel::~MatrixPanel() = default;

    void MatrixPanel::paint(juce::Graphics &g) {
        g.fillAll(juce::Colour::fromString("0xff191919"));
    }

    void MatrixPanel::resized() {
        const float maxHeight = 60.f;
        juce::Array<Track> rows;
        rows.resize(paramNamesAbleToModulate.size() + 1);
        for (size_t i = 0; i <= paramNamesAbleToModulate.size(); ++i) {
            rows.add(Track(Fr(1)));
        }
        using namespace juce;
        Grid grid;
        grid.alignItems = juce::Grid::AlignItems::start;
        grid.alignContent = juce::Grid::AlignContent::center;
        grid.templateRows = rows;
        grid.templateColumns = {
                Track(Fr(1)),
                Track(Fr(1)),
                Track(Fr(1))
        };
        juce::Array<GridItem> gridItems;
        gridItems.add(GridItem(sourceLabel));
        gridItems.add(GridItem(destinationLabel));
        gridItems.add(GridItem(intensityLabel));
        for (size_t i = 1; i < paramNamesAbleToModulate.size(); ++i) {
            gridItems.add(GridItem(sourceLabels[i]).withHeight(maxHeight).withWidth(100.f));
            gridItems.add(GridItem(destinations[i]).withHeight(30.f).withWidth(100.f).withAlignSelf(juce::GridItem::AlignSelf::center));
            gridItems.add(GridItem(intensityKnobs[i]).withHeight(maxHeight));
        }
        grid.items = gridItems;

        grid.performLayout(getLocalBounds().withHeight(maxHeight * paramNamesAbleToModulate.size()));
    }
}