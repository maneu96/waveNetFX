/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
DistFxWaveNetAudioProcessorEditor::DistFxWaveNetAudioProcessorEditor (DistFxWaveNetAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 300);
    
    // set graphic settings of knobs
    mainVolume.setSliderStyle(juce::Slider::Rotary);
    mainVolume.setRotaryParameters( juce::MathConstants<float>::pi + juce::MathConstants<float>::pi/6,  juce::MathConstants<float>::pi * 3  - juce::MathConstants<float>::pi/6, true);
    mainVolume.setRange(-18.0, 18.0);
    mainVolume.setTextBoxStyle(juce::Slider::NoTextBox, false, 90, 10);
    mainVolume.setPopupDisplayEnabled(true, false, this);
    mainVolume.setTextValueSuffix(" dB");
    mainVolume.setValue(0.0);
    // set visibility on the GUI
    addAndMakeVisible(&mainVolume);
    
    //add listener to the slider
    mainVolume.addListener(this);
}

DistFxWaveNetAudioProcessorEditor::~DistFxWaveNetAudioProcessorEditor()
{
}

//==============================================================================
void DistFxWaveNetAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (15.0f);
    g.drawFittedText ("Main Volume", 200,100,200,20,juce::Justification::centred, 1);
}

void DistFxWaveNetAudioProcessorEditor::resized()
{

    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    const int border = 20;
    const int rotaryWidth = getWidth()/ 2 - border;
    const int rotaryHeight = getHeight() - border - 100;
    mainVolume.setBounds(border, border, rotaryWidth,rotaryHeight);
}


void DistFxWaveNetAudioProcessorEditor::sliderValueChanged(juce::Slider* slider)
{
    audioProcessor.mainVolDb = mainVolume.getValue();
}
