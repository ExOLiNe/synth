//
// Created by Aleksandr on 19-Feb-23.
//

#include "MatrixPanel.h"
#include "../Typedefs.h"

#define SLOTS 20

namespace ui {
    int parseComponentID(const char* prefix, juce::ComboBox* combo)  {
        return atoi(combo->getComponentID().substring(sizeof(prefix) - 1).toRawUTF8());
    }

    MatrixPanel::MatrixPanel(
            juce::AudioProcessorValueTreeState& apvts,
            const std::vector<juce::String>& paramNamesAbleToModulate,
            const std::vector<juce::String>& modulatorNames)
    : apvts(apvts), paramNamesAbleToModulate(paramNamesAbleToModulate) {
        sourceLabel.setText("Source", juce::NotificationType::dontSendNotification);
        sourceLabel.setFont(juce::Font().boldened());
        addAndMakeVisible(sourceLabel);

        destinationLabel.setText("Destination", juce::NotificationType::dontSendNotification);
        destinationLabel.setFont(juce::Font().boldened());
        addAndMakeVisible(destinationLabel);

        intensityLabel.setText("Intensity", juce::NotificationType::dontSendNotification);
        intensityLabel.setFont(juce::Font().boldened());
        addAndMakeVisible(intensityLabel);

        for(size_t i = 0; i < SLOTS; ++i) {
            auto componentID = std::to_string(i);

            auto source = [&]() {
                SourceCombo source(new juce::ComboBox(SOURCE));
                source->setComponentID(juce::String(SOURCE) + componentID);
                int index = 1;
                source->addItem("-------", index);

                if (source->getSelectedItemIndex() == -1) {
                    source->setSelectedItemIndex(0);
                }

                for (const auto& paramName: paramNamesAbleToModulate) {
                    source->addItem(paramName, ++index);
                }
                source->addListener(this);
                return source;
            }();

            auto destination = [&]() {
                DestinationCombo destination(new juce::ComboBox(DESTINATION));
                destination->setComponentID(juce::String(DESTINATION) + componentID);
                int index = 1;
                destination->addItem("------", index);
                if (destination->getSelectedItemIndex() == -1) {
                    destination->setSelectedItemIndex(0);
                }

                for (const auto& modulator : modulatorNames) {
                    destination->addItem(modulator, ++index);
                }
                destination->addListener(this);
                return destination;
            }();

            auto intensity = [&]() {
                Intensity knob(new juce::Slider());
                knob->setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
                return knob;
            }();

            Attachment attachment;
            attachment.reset();

            juce::String parameterID = source->getText() + destination->getText();

            auto rowElement = new RowElement {
                std::move(source),
                std::move(destination),
                std::move(parameterID),
                std::move(intensity),
                std::move(attachment)
            };
            rowElement->makeVisible(this);
            rowElements.add(rowElement);
        }
    }

    void MatrixPanel::comboBoxChanged (juce::ComboBox* comboBoxThatHasChanged) {
        DBG(comboBoxThatHasChanged->getSelectedItemIndex());
        DBG(comboBoxThatHasChanged->getSelectedId());
        if (comboBoxThatHasChanged->getComponentID().startsWith(SOURCE)) {
            auto id = parseComponentID(SOURCE, comboBoxThatHasChanged);
            auto rowElement = rowElements[id];
            if (rowElement->destination->getSelectedItemIndex() > 0
                && comboBoxThatHasChanged->getSelectedItemIndex() > 0) {
                Attachment attachment(
                        new juce::AudioProcessorValueTreeState::SliderAttachment(apvts,
                            rowElement->source->getText() + rowElement->destination->getText(),
                            *rowElement->intensity)
                );
                rowElement->attachment = std::move(attachment);
            } else if (comboBoxThatHasChanged->getSelectedItemIndex() <= 0) {
                rowElement->attachment.reset();
            }
        } else if (comboBoxThatHasChanged->getComponentID().startsWith(DESTINATION)) {
            auto id = parseComponentID(DESTINATION, comboBoxThatHasChanged);
            auto rowElement = rowElements[id];
            auto parameterID = rowElement->source->getText() + rowElement->destination->getText();
            if (rowElement->source->getSelectedItemIndex() > 0
                && comboBoxThatHasChanged->getSelectedItemIndex() > 0) {
                Attachment attachment(
                        new juce::AudioProcessorValueTreeState::SliderAttachment(apvts,
                                                                                 parameterID,
                                                                                 *rowElement->intensity)
                );
                rowElement->attachment = std::move(attachment);
                rowElement->parameterID = std::move(parameterID);
            } else if (comboBoxThatHasChanged->getSelectedItemIndex() <= 0) {
                rowElement->attachment.reset();
                auto valuePtr = apvts.getRawParameterValue(rowElement->parameterID);
                if (valuePtr != nullptr) {
                    valuePtr->store(0.f);
                }
            }
        }
        /*
           // REMOVE PREVIOUS

           // ADD
           auto attachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (
                apvts, paramNamesAbleToModulate[getSelectedItemId()].parameterID);
           attachments.add(attachment);
         */
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
        for (const auto& rowElement : rowElements) {
            gridItems.add(GridItem(*rowElement->source).withHeight(maxHeight).withWidth(100.f));
            gridItems.add(GridItem(*rowElement->destination).withHeight(maxHeight).withWidth(100.f).withAlignSelf(juce::GridItem::AlignSelf::center));
            gridItems.add(GridItem(*rowElement->intensity).withHeight(maxHeight));
        }
        /*for (size_t i = 1; i < paramNamesAbleToModulate.size(); ++i) {
            gridItems.add(GridItem(sourceLabels[i]).withHeight(maxHeight).withWidth(100.f));
            gridItems.add(GridItem(destinations[i]).withHeight(30.f).withWidth(100.f).withAlignSelf(juce::GridItem::AlignSelf::center));
            gridItems.add(GridItem(intensityKnobs[i]).withHeight(maxHeight));
        }*/
        grid.items = gridItems;

        grid.performLayout(getLocalBounds().withHeight(maxHeight * paramNamesAbleToModulate.size()));
    }
}