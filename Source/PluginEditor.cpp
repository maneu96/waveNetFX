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
    setSize (800, 600);
   
    loadButton.setButtonText("LOAD MODEL");
    addAndMakeVisible(&loadButton);
    loadButton.addListener(this);
    // set graphic settings of knobs
    mainVolume.setSliderStyle(juce::Slider::Rotary);
    mainVolume.setRotaryParameters( juce::MathConstants<float>::pi + juce::MathConstants<float>::pi/6,  juce::MathConstants<float>::pi * 3  - juce::MathConstants<float>::pi/6, true);
    mainVolume.setRange(-18.0, 18.0);
    mainVolume.setTextBoxStyle(juce::Slider::TextBoxBelow, false, mainVolume.getWidth()/2, mainVolume.getHeight()/8);
    //mainVolume.setPopupDisplayEnabled(true, false, this);
    
    //mainVolume.setTextValueSuffix(" dB");
    mainVolume.setValue(0.0);
    // set visibility on the GUI
    addAndMakeVisible(&mainVolume);
    //add listener to the slider
    mainVolume.addListener(this);
    
    // Set up the volume label
    addAndMakeVisible(mainVolumeLabel);
    mainVolumeLabel.setText("Volume [dB]", juce::dontSendNotification);
    mainVolumeLabel.setJustificationType(juce::Justification::centred);
    mainVolumeLabel.attachToComponent(&mainVolume, false);
    
    
    
    //add discrete gain knob
    discreteGain.setSliderStyle(juce::Slider::Rotary);
    //discreteGain.setRotaryParameters(<#RotaryParameters newParameters#>)
    discreteGain.setRange(0,2,1);
    discreteGain.setRotaryParameters( juce::MathConstants<float>::pi + juce::MathConstants<float>::pi/6,  juce::MathConstants<float>::pi * 3  - juce::MathConstants<float>::pi/6, true);
    discreteGain.setValue(0);
    discreteGain.setTextBoxStyle(juce::Slider::TextBoxBelow, false, discreteGain.getWidth()/2, discreteGain.getHeight()/8);
   // discreteGain.setPopupDisplayEnabled(true, false, this);

    addAndMakeVisible(&discreteGain);
    discreteGain.addListener(this);
   // addAndMakeVisible(threePointKnob);
    
    
    addAndMakeVisible(discreteGainLabel);
    discreteGainLabel.setText("Gain", juce::dontSendNotification);
    discreteGainLabel.setJustificationType(juce::Justification::centred);
    discreteGainLabel.attachToComponent(&discreteGain, false);
    
    //add blend knob
    blendGain.setSliderStyle(juce::Slider::Rotary);
    blendGain.setRange(0,1);
    blendGain.setRotaryParameters(juce::MathConstants<float>::pi + juce::MathConstants<float>::pi/6,  juce::MathConstants<float>::pi * 3  - juce::MathConstants<float>::pi/6, true);
    blendGain.setValue(0);
    blendGain.setTextBoxStyle(juce::Slider::TextBoxBelow, false, blendGain.getWidth()/2, blendGain.getHeight()/8);
    //blendGain.setPopupDisplayEnabled(true, false, this);
    blendGain.setName("Dry/Wet"); // Set the description
    
    addAndMakeVisible(&blendGain);
    blendGain.addListener(this);

    
    addAndMakeVisible(blendGainLabel);
    blendGainLabel.setText("Dry/Wet", juce::dontSendNotification);
    blendGainLabel.setJustificationType(juce::Justification::centred);
    blendGainLabel.attachToComponent(&blendGain, false);
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
   // g.drawFittedText ("Main Volume", 200,100,200,20,juce::Justification::centred, 1);
}

void DistFxWaveNetAudioProcessorEditor::resized()
{

    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    const int border = 5 ;
    const int rotaryWidth = getWidth()/3;
    const int rotaryHeight = getHeight()/3 ;
   
    mainVolume.setBounds(rotaryWidth/2 + border, rotaryHeight/2 + border, rotaryWidth,rotaryHeight);
    discreteGain.setBounds(getWidth()/2 + border , rotaryHeight/2 + border, rotaryWidth, rotaryHeight);
    blendGain.setBounds(getWidth()/2 - rotaryWidth/2 ,getHeight()/2 + border, rotaryWidth, rotaryHeight);
    
    const int loadButtonWidth = getWidth()/2;
    const int loadButtonHeight = getHeight()/15;
    loadButton.setBounds(getWidth()/2 - loadButtonWidth/2 , loadButtonHeight/2 , loadButtonWidth, loadButtonHeight );
    
}


void DistFxWaveNetAudioProcessorEditor::sliderValueChanged(juce::Slider* slider)
{
    audioProcessor.mainVolDb = mainVolume.getValue();
    audioProcessor.discreteGainSelector = discreteGain.getValue();
    audioProcessor.blendGain = blendGain.getValue();
}


void DistFxWaveNetAudioProcessorEditor::buttonClicked(juce::Button *button)
{
    if (button == &loadButton)
        loadButtonClicked();
}

void DistFxWaveNetAudioProcessorEditor::loadButtonClicked()
{
    myChooser = std::make_unique<FileChooser> ("Load a trained WaveNet model...",
                         File::getSpecialLocation(File::userHomeDirectory),
                                     "*.json");
    
   // auto folderChooserFlags = FileBrowserComponent::openMode | FileBrowserComponent::canSelectFiles;
    auto folderChooserFlags = FileBrowserComponent::openMode | FileBrowserComponent::canSelectDirectories;
    myChooser->launchAsync (folderChooserFlags, [this] (const FileChooser& chooser)
    {
        File file = chooser.getResult();
        audioProcessor.loadConfig(file.getFullPathName());
    });
    
    //File file("D:\Tese\WaveNetVA\Models\WaveNet3-muff.json");
   
}
