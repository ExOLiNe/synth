//
// Created by Aleksandr on 19-Feb-23.
//

#include "Grid.h"

Grid::Grid(juce::Array<int> rows, juce::Array<int> cols) {
    using Track = juce::Grid::TrackInfo;
    using Fr = juce::Grid::Fr;
    juce::Array<Track> gridRows;
    juce::Array<Track> gridCols;

    for (auto row : rows) {
        gridRows.insert(-1, Track(Fr(row)));
    }

    for (auto col : cols) {
        gridCols.insert(-1, Track(Fr(col)));
    }

    j_grid.templateRows = gridRows;
    j_grid.templateColumns = gridCols;
}

Grid::~Grid() {

}

void Grid::setItems(std::vector<juce::Component*> items) {
    for (auto *item : items) {
        j_grid.items.add(juce::GridItem(item));
    }
}

void Grid::performLayout(juce::Rectangle<int> bounds) {
    j_grid.performLayout(bounds);
}