//
//  ThreePointKnob.hpp
//  waveNetFx
//
//  Created by Manuel Freitas on 28/09/2024.
//
#pragma once
#include <JuceHeader.h>

class ThreePointKnob : public juce::Component
{
public:
    // Constructor
    ThreePointKnob();

    // Paint method to draw the knob
    void paint(juce::Graphics& g) override;

    // Mouse drag event to handle knob position changes
    void mouseDrag(const juce::MouseEvent& event) override;

    // Getter for the current knob position
    int getPosition() const;


};
