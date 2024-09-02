/*
  ==============================================================================

    waveNet.h
    Created: 18 Jan 2024 5:38:52pm
    Author:  Manuel Freitas

  ==============================================================================
*/

#pragma once
#include "RTNeural/RTNeural.h"
#include <JuceHeader.h>
#include "../Source/modules/Eigen/Eigen/Eigen"


#include <iostream>

#define N_CHANNELS 16
#define N_SAMPLES 2045
#define KERNEL_SIZE 3
#define GROUPS 1
#define DYN_STATE true



std::unique_ptr<RTNeural::Model<float>> initWaveNet();
using namespace RTNeural;
using namespace std;

class waveNet {
    
public:
    waveNet();
    ~waveNet();
    void loadLayers(const String fileName);
    float predict(Eigen::Matrix<float,1,1> x);
    void reDimension(std::vector<std::vector<std::vector<float>>> source,
                              std::vector<std::vector<std::vector<float>>> &destination);
   
    void processWavFile(const juce::String& inputFilePath, const juce::String& outputFilePath);
    
private:
    int samplesProcessed = 0;
    void loadHiddenLayers(int &iLayer,nlohmann::json layersJson);
    void resetLayers();
    void hiddenLayerForward (uint index);
    
    static constexpr int hiddenLayerDepth=18; //hidden layer size
                                          // filters_per_group = 1 = in_size/num_groups => num_groups = in_size
    
    
    // Conv1DT<float, input_channels, output_channels, kernel_size, dilation, groups = >

    RTNeural::Conv1DT<float,1,N_CHANNELS,1,1,GROUPS, DYN_STATE> inputLayer;
    RTNeural::SigmoidActivationT<float, N_CHANNELS> hiddenGateActivation[18];
    RTNeural::TanhActivationT<float, N_CHANNELS> hiddenFilterActivation[18];
    RTNeural::Conv1DT<float,N_CHANNELS,N_CHANNELS,1,1, GROUPS, DYN_STATE> hiddenLayerResidual[17];

    RTNeural::Conv1DT<float, 288,1,1,1,1> outputLayer;
   // Conv1DT<<#typename T#>, <#int in_sizet#>, <#int out_sizet#>, <#int kernel_size#>, <#int dilation_rate#>>
    Conv1DT<float,N_CHANNELS,N_CHANNELS,3,1,GROUPS, DYN_STATE> gate_1[2];   // channels_per_group = out_size/num_groups = 16
    Conv1DT<float,N_CHANNELS,N_CHANNELS,3,2,GROUPS, DYN_STATE> gate_2[2];
    Conv1DT<float,N_CHANNELS,N_CHANNELS,3,4,GROUPS, DYN_STATE> gate_4[2];   // Conv1FT < Type, in_size,out_size,kernel_size,dilation,groups>
    Conv1DT<float,N_CHANNELS,N_CHANNELS,3,8,GROUPS, DYN_STATE> gate_8[2];
    Conv1DT<float,N_CHANNELS,N_CHANNELS,3,16,GROUPS, DYN_STATE> gate_16[2];
    Conv1DT<float,N_CHANNELS,N_CHANNELS,3,32,GROUPS, DYN_STATE> gate_32[2];
    Conv1DT<float,N_CHANNELS,N_CHANNELS,3,64,GROUPS, DYN_STATE> gate_64[2];
    Conv1DT<float,N_CHANNELS,N_CHANNELS,3,128,GROUPS, DYN_STATE> gate_128[2];
    Conv1DT<float,N_CHANNELS,N_CHANNELS,3,256,GROUPS, DYN_STATE> gate_256[2];

    Conv1DT<float,N_CHANNELS,N_CHANNELS,3,1,GROUPS, DYN_STATE> filter_1[2];
    Conv1DT<float,N_CHANNELS,N_CHANNELS,3,2,GROUPS, DYN_STATE> filter_2[2];
    Conv1DT<float,N_CHANNELS,N_CHANNELS,3,4,GROUPS, DYN_STATE> filter_4[2];
    Conv1DT<float,N_CHANNELS,N_CHANNELS,3,8,GROUPS, DYN_STATE> filter_8[2];
    Conv1DT<float,N_CHANNELS,N_CHANNELS,3,16,GROUPS, DYN_STATE> filter_16[2];
    Conv1DT<float,N_CHANNELS,N_CHANNELS,3,32,GROUPS, DYN_STATE> filter_32[2];
    Conv1DT<float,N_CHANNELS,N_CHANNELS,3,64,GROUPS, DYN_STATE> filter_64[2];
    Conv1DT<float,N_CHANNELS,N_CHANNELS,3,128,GROUPS, DYN_STATE> filter_128[2];
    Conv1DT<float,N_CHANNELS,N_CHANNELS,3,256,GROUPS, DYN_STATE> filter_256[2];
   
    
    Eigen::Matrix<float,N_CHANNELS,1> outGateBuffer;//16
    Eigen::Matrix<float,N_CHANNELS,1> outFilterBuffer;//16
    
    Eigen::Matrix<float,hiddenLayerDepth*N_CHANNELS,1> zout;
};


