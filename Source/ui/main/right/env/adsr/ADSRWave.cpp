//
// Created by Aleksandr on 19-Feb-23.
//

#include "ADSRWave.h"
#include "../../../../../Constants.h"

namespace ui {
    ADSRWave::ADSRWave(juce::AudioProcessorValueTreeState& treeState, const juce::String adsrId) :
        treeState(treeState), adsrId(adsrId) {

    }

    ADSRWave::~ADSRWave() noexcept {

    }

    /*void ADSRWave::setDefaultValues(const juce::Slider &attackSlider, const juce::Slider &decaySlider,
                                    const juce::Slider &sustainSlider, const juce::Slider &releaseSlider) {
        attackValue = getNormalisedValue(attackSlider);
        decayValue = getNormalisedValue(decaySlider);
        sustainValue = getNormalisedValue(sustainSlider);
        releaseValue = getNormalisedValue(releaseSlider);
    }*/

    void ADSRWave::parameterChanged (const juce::String& parameterID, float newValue) {
        repaint();
    }

    void ADSRWave::paint(juce::Graphics &g) {
        const int fr = 100;
        auto attackValue = treeState.getRawParameterValue(adsrId + params::adsr::attack.name)->load() / fr;
        auto decayValue = treeState.getRawParameterValue(adsrId + params::adsr::decay.name)->load() / fr;
        auto sustainValue = treeState.getRawParameterValue(adsrId + params::adsr::sustain.name)->load() / fr;
        auto releaseValue = treeState.getRawParameterValue(adsrId + params::adsr::release.name)->load() / fr;

        auto pointColour = juce::Colours::white.withAlpha(0.8f);
        auto lineColour = juce::Colour(juce::PixelARGB(120, 107, 140, 175));
        float pointThickness = 0.01f;

        auto height = getLocalBounds().getHeight();
        g.fillAll(juce::Colours::black);
        g.addTransform(juce::AffineTransform::verticalFlip(height));
        g.addTransform(juce::AffineTransform::scale(height));

        auto widthNormalised = getLocalBounds().toFloat().getAspectRatio();

        float attackValueWithRatio = (float) attackValue * widthNormalised;
        float decayValueWithRatio = attackValueWithRatio + (float) decayValue * widthNormalised;
        float releaseValueWithRatio = decayValueWithRatio + (float) releaseValue * widthNormalised;

        if (releaseValueWithRatio >= widthNormalised) {
            auto factor = widthNormalised / releaseValueWithRatio;
            attackValueWithRatio *= factor;
            decayValueWithRatio *= factor;
            releaseValueWithRatio = widthNormalised;
        }

        juce::Path path;
        g.setColour(lineColour);
        path.startNewSubPath({pointThickness, pointThickness});
        path.lineTo({attackValueWithRatio, 1.0f - pointThickness});
        path.closeSubPath();

        path.startNewSubPath({attackValueWithRatio, 1.0f - pointThickness});
        path.lineTo({decayValueWithRatio, 1.0f - pointThickness});
        path.closeSubPath();

        path.startNewSubPath({decayValueWithRatio, 1.0f - pointThickness});
        path.lineTo({releaseValueWithRatio, pointThickness});
        path.closeSubPath();

        path.startNewSubPath({decayValueWithRatio, 1.0f - pointThickness});
        path.lineTo({releaseValueWithRatio, pointThickness});
        path.closeSubPath();

        g.strokePath(path, juce::PathStrokeType(0.01f));

        g.setColour(pointColour);
        g.drawEllipse({pointThickness,
                       pointThickness, pointThickness, pointThickness}, pointThickness);
        g.drawEllipse({attackValueWithRatio, 1.0f - pointThickness, pointThickness, pointThickness}, pointThickness);
        g.drawEllipse({decayValueWithRatio, 1.0f - pointThickness, pointThickness, pointThickness}, pointThickness);
        g.drawEllipse({releaseValueWithRatio, pointThickness, pointThickness, pointThickness}, pointThickness);
    }

    void ADSRWave::resized() {
    }
}