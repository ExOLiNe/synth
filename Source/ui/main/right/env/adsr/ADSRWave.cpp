//
// Created by Aleksandr on 19-Feb-23.
//

#include "ADSRWave.h"

namespace ui {
    ADSRWave::ADSRWave() {
    }

    ADSRWave::~ADSRWave() noexcept {

    }

    void ADSRWave::setDefaultValues(const juce::Slider &attackSlider, const juce::Slider &decaySlider,
                                    const juce::Slider &sustainSlider, const juce::Slider &releaseSlider) {
        attackValue = getNormalisedValue(attackSlider);
        decayValue = getNormalisedValue(decaySlider);
        sustainValue = getNormalisedValue(sustainSlider);
        releaseValue = getNormalisedValue(releaseSlider);
    }

    void ADSRWave::paint(juce::Graphics &g) {
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

        g.setColour(lineColour);
        path.startNewSubPath({attackValueWithRatio, 1.0f - pointThickness});
        path.lineTo({decayValueWithRatio, 1.0f - pointThickness});
        path.closeSubPath();
        g.strokePath(path, juce::PathStrokeType(0.01f));

        juce::Path path2;
        g.setColour(lineColour);
        path2.startNewSubPath({decayValueWithRatio, 1.0f - pointThickness});

        path2.lineTo({releaseValueWithRatio, pointThickness});
        path2.applyTransform(
                juce::AffineTransform::rotation(10.0f, decayValueWithRatio,
                                                1.0f - pointThickness)
        );
        g.strokePath(path2,
                     juce::PathStrokeType(
                             0.01f,
                             juce::PathStrokeType::JointStyle::curved,
                             juce::PathStrokeType::EndCapStyle::rounded
                     ));
        //path2.closeSubPath();


        drawSin(g, pointThickness, {decayValueWithRatio, 1.0f - pointThickness}, {releaseValueWithRatio, pointThickness});

        g.setColour(pointColour);
        g.drawEllipse({pointThickness,
                       pointThickness, pointThickness, pointThickness}, pointThickness);
        g.drawEllipse({attackValueWithRatio, 1.0f - pointThickness, pointThickness, pointThickness}, pointThickness);
        g.drawEllipse({decayValueWithRatio, 1.0f - pointThickness, pointThickness, pointThickness}, pointThickness);
        g.drawEllipse({releaseValueWithRatio, pointThickness, pointThickness, pointThickness}, pointThickness);
    }

    void ADSRWave::resized() {
    }

    void ADSRWave::drawSin(juce::Graphics &g, float pointThickness, juce::Point<float> p1, juce::Point<float> p2) {
        float width = p2.x - p1.x;
        float height = p2.y - p1.y;
        float ratio = width / height;
        float length = p1.getDistanceFrom(p2);
        juce::Line<float> line(p1, p2);
        float angle = line.getAngle() - juce::MathConstants<float>::pi / 2.0f;
        const unsigned int n = 200;
        const float xStep = length / n;
        const float intensity = -0.06f;

        const float yFactor = juce::MathConstants<float>::pi / n;

        juce::Path path;
        bool started = false;
        float middle = length / 2;
        for (int i = 0; i < n; ++i) {
            float x = (float)i * xStep;
            float asymptoteY = x > middle ? length - x : x;
            float asymptoteY2 = -asymptoteY;
            auto y = std::sin((float)i * yFactor) * intensity;
            if (y > asymptoteY) {
                y = asymptoteY;
            } else if (y < asymptoteY2) {
                y = asymptoteY2;
            }
            if (!started) {
                path.startNewSubPath({x, y});
                started = true;
            } else {
                path.lineTo({x, y});
            }
        }

        path.applyTransform(juce::AffineTransform::translation(p1.x, p1.y));
        path.applyTransform(juce::AffineTransform::rotation(angle, p1.x, p1.y));
        g.strokePath(path, juce::PathStrokeType(pointThickness * 0.5f));
    }

    void ADSRWave::sliderValueChanged(juce::Slider *slider) {
        auto normalisedValue = getNormalisedValue(*slider);
        auto id = slider->getComponentID();

        if (id == "attack") {
            attackValue = normalisedValue;
        } else if (id == "decay") {
            decayValue = normalisedValue;
        } else if (id == "sustain") {
            sustainValue = normalisedValue;
        } else if (id == "release") {
            releaseValue = normalisedValue;
        }

        repaint();
    }

    double ADSRWave::getNormalisedValue(const juce::Slider &slider) {
        auto range = slider.getRange().getLength();
        return (slider.getValue() - slider.getRange().getStart()) / range;
    }
}