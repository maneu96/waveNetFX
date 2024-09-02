/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class DistFxWaveNetAudioProcessorEditor  : public juce::AudioProcessorEditor,
                                           private juce::Slider::Listener, //inheritance of slider listener class functionalities
                                           private juce::Button::Listener
{
public:
    DistFxWaveNetAudioProcessorEditor (DistFxWaveNetAudioProcessor&);
    ~DistFxWaveNetAudioProcessorEditor() override;
    
    
    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    
    void sliderValueChanged(juce::Slider* slider) override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    DistFxWaveNetAudioProcessor& audioProcessor;
    juce::Slider mainVolume;
    juce::TextButton loadButton;
    
    std::unique_ptr<FileChooser> myChooser;
    
    virtual void buttonClicked(juce::Button *button) override;
    void loadButtonClicked();
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DistFxWaveNetAudioProcessorEditor)
};
