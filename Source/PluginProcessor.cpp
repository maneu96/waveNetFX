/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

#include <chrono>

const double gainCorrection = 0.5; // -3 dB to compensate for high gain
const float sampleRate = 44100.0f; // Sampling rate in Hz
const float frequency = 500.0f;    // Frequency in Hz
static float t = 0.0f;             // Static time variable to maintain continuity between buffer fills

void fillBuffer(float * buffer) {
    for (int i = 0; i < 2054; ++i) {
        buffer[i] = 0.5f * std::sin(2.0f * M_PI * frequency * t);
        t += 1.0f / sampleRate;

        // Reset t to prevent precision loss over time
        if (t >= 1.0f) t -= 1.0f;
    }
}
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
   /*
    float buffer[2054], results[10];
    fillBuffer(buffer);
    Eigen::Matrix<float,1,1> x;
    auto model = RTNeural::json_parser::parseJson<float>(jsonStream, true);
   
    float x_in[4] = {0,0.03558885,0.07099716,0.10604533};
    std::cout<< model ->forward(&x_in[0])<<std::endl;
    std::cout<< model ->forward(&x_in[1])<<std::endl;
    model->reset();
    std::cout<< model ->forward(&x_in[2])<<std::endl;
    std::cout<< model ->forward(&x_in[3])<<std::endl;
    for(int i= 2044; i< 2054; i++){
      //  cout << buffer[i] <<endl;
    }
    cout<< "END INPUT *****************************************************" <<endl << endl;
    auto start = std::chrono::high_resolution_clock::now();
    for(int i= 0; i< 2054; i++){
        x(0) = 1;
        if( i >=2045)
        {
            results[i%2045] = cNN.predict(x);
            cout << results[i%2045] <<endl;
            
        }
    }
    // Stop measuring time
     auto stop = std::chrono::high_resolution_clock::now();
     
     // Calculate the duration
     auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
     
    std::cout << "Time taken by function: " << duration.count() << " microseconds" << std::endl;
    //Use this method as the place to do any pre-playback
    // initialisation that you need..
    //std::cout << cNN.outputLayer.outs << std::endl; */
    std::cout << "Model Initialized" << std::endl;

 //   cNN.processWavFile(juce::String("/Users/maneu/Downloads/x_true.wav"),juce::String("/Users/maneu/Downloads/y_pred_cpp.wav"));
    
    //double frequency = sampleRate * 0.5 * (1.0 - 0.95); // Calculate the cutoff frequency based on the coefficient
    //sampleRate = 44100;
    float frequency = 40;
    highPassFilter.setCoefficients(juce::IIRCoefficients::makeHighPass(sampleRate, frequency));
    
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
    Eigen::Matrix<double,1,1> input;
    double volLinear = pow(10,mainVolDb/20); //set the volume into linear scale
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
        const float * x = buffer.getReadPointer  (channel);
        //inputBuffer << buffer.getReadPointer(channel);
        highPassFilter.processSamples(buffer.getWritePointer(channel), buffer.getNumSamples());
        auto * y = buffer.getWritePointer (channel);
        for (auto n = 0; n < buffer.getNumSamples(); n++)
        {
            
            //inputBuffer=Eigen::Map<const E igen::Matrix<float, 1, 1>> (x);
            input(0) = x[n];
            //y[n] = gainCorrection * volLinear * cNN[discreteGainSelector].predict(input);//Eigen::Map<Eigen::Matrix<float,1,1>> (x[n]));
            y[n] = (1-blendGain)*input(0) + blendGain * gainCorrection * volLinear *cNN[discreteGainSelector].predict(input);
            //if cNN.samplesProcessed = 512//y[n] = x[n];
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

void DistFxWaveNetAudioProcessor::loadConfig(const String filePath){ //Here the 3 networks are loaded
    // /Users/maneu/Documents/WaveNetModels/ERD/model_0.json
    for(int i = 0; i < 3; i++){
        //std::cout << "Loaded Model: " + filePath + "/model_" + std::to_string(i*50) + ".json" << std::endl;
        cNN[i].loadLayers(filePath + "/model_" + std::to_string(i*50) + ".json");
    }
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new DistFxWaveNetAudioProcessor();
}


