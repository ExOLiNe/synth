//
// Created by Aleksandr on 19-Feb-23.
//

#include "LFOWave.h"
#include "../../../../../Constants.h"

namespace ui {
    LFOWave::LFOWave(juce::AudioProcessorValueTreeState& treeState, const juce::String lfoId) :
            treeState(treeState), lfoId(lfoId) {
    }

    LFOWave::~LFOWave() noexcept {

    }

    void LFOWave::parameterChanged (const juce::String& parameterID, float newValue) {
        repaint();
    }

    void LFOWave::paint(juce::Graphics &g) {
        auto rate = treeState.getRawParameterValue(lfoId + params::lfo::freq.name)->load();
        auto amp = treeState.getRawParameterValue(lfoId + params::lfo::amp.name)->load() / 100;

        auto lineColour = juce::Colour(juce::PixelARGB(120, 107, 140, 175));
        float pointThickness = 3.0f;

        auto height = getLocalBounds().getHeight();
        auto width = getLocalBounds().getWidth();
        g.fillAll(juce::Colours::black);
        g.addTransform(juce::AffineTransform::verticalFlip(height));
        g.addTransform(juce::AffineTransform::translation(0.0f, (float)height / 2.0f));

        auto pointsTotal = width;
        auto yFactor = (float)height / 2.0f * amp;

        float yStep = juce::MathConstants<float>::twoPi / pointsTotal * rate;

        juce::Path path;
        g.setColour(lineColour);

        bool started = false;

        for (int i = 0; i < pointsTotal; ++i) {
            float y = std::sin(i * yStep) * yFactor;
            juce::Point<float> point({(float)i, y});
            if (!started) {
                path.startNewSubPath(point);
                started = true;
            } else {
                path.lineTo(point);
            }
        }
        g.strokePath(path, juce::PathStrokeType(pointThickness));
    }

    void LFOWave::resized() {

    }
}