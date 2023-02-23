#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include "wave/Wave.h"

class WaveComponent : public juce::Component {
public:
    WaveComponent();
    ~WaveComponent();

    void paint(juce::Graphics&) override;
    void resized() override;
private:
    juce::ComboBox selector;
    Wave wave;
    std::vector<std::vector<float>> getSinWaveTable() {
        std::vector<std::vector<float>> points;
        float sinFactor = 5.0f * 0.01f;
        for (unsigned int z = 0; z < zSize; ++z) {
            points.push_back(std::vector<float>());
            for (int x = 0; x < 200; ++x) {
                float y = std::sin((float)x * sinFactor);
                juce::Logger::getCurrentLogger()->writeToLog(std::to_string(y));
                points.at(z).push_back(y);
            }
        }
        return points;
    }
    int zSize = 15;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(WaveComponent)
};