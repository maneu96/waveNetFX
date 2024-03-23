/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

#include <chrono>

//==============================================================================
DistFxWaveNetAudioProcessor::DistFxWaveNetAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::mono(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::mono(), true)
                     #endif
                       )
#endif
{
}

DistFxWaveNetAudioProcessor::~DistFxWaveNetAudioProcessor()
{
}

//==============================================================================
const juce::String DistFxWaveNetAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool DistFxWaveNetAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool DistFxWaveNetAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool DistFxWaveNetAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double DistFxWaveNetAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int DistFxWaveNetAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int DistFxWaveNetAudioProcessor::getCurrentProgram()
{
    return 0;
}

void DistFxWaveNetAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String DistFxWaveNetAudioProcessor::getProgramName (int index)
{
    return {};
}

void DistFxWaveNetAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void DistFxWaveNetAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    auto start = std::chrono::high_resolution_clock::now();
    for(int i= 0; i< 512; i++)
        cNN.predict(1.0);
    // Stop measuring time
     auto stop = std::chrono::high_resolution_clock::now();
     
     // Calculate the duration
     auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
     
    std::cout << "Time taken by function: " << duration.count() << " microseconds" << std::endl;

    cout << cNN.predict(1.0) << endl;
    //Use this method as the place to do any pre-playback
    // initialisation that you need..\
    //std::cout << cNN.outputLayer.outs << std::endl;
    std::cout << "Model Initialized" << std::endl;
    
}

void DistFxWaveNetAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool DistFxWaveNetAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void DistFxWaveNetAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    
    float volLinear = pow(10,mainVolDb/20); // set the volume into linear scale
    //printf("%f\n", volLinear);
    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    //for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
    //    buffer.clear (i, 0, buffer.getNumSamples());
    
    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.

    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto * x = buffer.getReadPointer  (channel);
        auto * y = buffer.getWritePointer (channel);
        for (auto n = 0; n < buffer.getNumSamples(); n++)
        {
           
            y[n] = volLinear * cNN.predict(x[n]);
            //y[n] = x[n];
            //cout<< x[n]<<endl;
        }
    }
}

//==============================================================================
bool DistFxWaveNetAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* DistFxWaveNetAudioProcessor::createEditor()
{
    return new DistFxWaveNetAudioProcessorEditor (*this);
}

//==============================================================================
void DistFxWaveNetAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void DistFxWaveNetAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new DistFxWaveNetAudioProcessor();
}
