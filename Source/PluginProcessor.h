/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "waveNet.h"

//==============================================================================
/**
*/
void fillBuffer(float * buffer);
class DistFxWaveNetAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    DistFxWaveNetAudioProcessor();
    ~DistFxWaveNetAudioProcessor() override;
    
    float mainVolDb;
    int discreteGainSelector;
    float blendGain;
    String folderPath;
    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;
    
    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    
    void loadConfig(const String filePath);
    
private:
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DistFxWaveNetAudioProcessor)
    waveNet cNN[3];
    
    //Eigen::Matrix<float,N_SAMPLES,1> inputBuffer;
    juce::IIRFilter highPassFilter;
    
    
    
    
};
