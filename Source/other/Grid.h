//
// Created by Aleksandr on 19-Feb-23.
//
#include <juce_gui_basics/juce_gui_basics.h>

class Grid {
public:
    Grid(juce::Array<int> rows, juce::Array<int> cols);
    ~Grid();
    void setItems(std::vector<juce::Component*> items);
    void performLayout(juce::Rectangle<int> bounds);
private:
    juce::Grid j_grid;
};