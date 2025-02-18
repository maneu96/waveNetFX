/*
  ==============================================================================

    waveNet.h
    Created: 18 Jan 2024 5:38:52pm
    Author:  Manuel Freitas

  ==============================================================================
*/

#pragma once
#include "RTNeural/RTNeural.h"
#include "/Users/maneu/Desktop/Tese/Wavenet2023/waveNetFX/waveNetFx/JuceLibraryCode/JuceHeader.h"
#include "./modules/Eigen/Eigen/Eigen"
#include <iostream>
#include <string>

#define N_CHANNELS 16
#define N_SAMPLES 1
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
    void loadLayers(String fileName);
    float predict(Eigen::Matrix<float,N_SAMPLES,1> x);
    void reDimension(std::vector<std::vector<std::vector<float>>> source,
                              std::vector<std::vector<std::vector<float>>> &destination);
    //void processWavFile(const juce::String& inputFilePath, const juce::String& outputFilePath);
    
private:
    int samplesProcessed = 0;
    void loadHiddenLayers(int &iLayer,nlohmann::json layersJson);
    void resetLayers();
    void hiddenLayerForward (uint index);
    
    static constexpr int hiddenLayerDepth=18; //hidden layer size
    
    Conv1DT<float,N_SAMPLES,N_SAMPLES*N_CHANNELS,3,1,GROUPS, DYN_STATE> inputLayer;
    SigmoidActivationT<float, N_CHANNELS*N_SAMPLES> hiddenGateActivation[18];
    TanhActivationT<float, N_CHANNELS*N_SAMPLES> hiddenFilterActivation[18];
 
    // usage of Conv1D layer: Conv1DT<<#typename T#>, <#int in_sizet#>, <#int out_sizet#>, <#int kernel_size#>, <#int dilation_rate#>>
    // Hidden Layer declarations
    Conv1DT<float,N_CHANNELS*N_SAMPLES,N_CHANNELS*N_SAMPLES,1,1, GROUPS, DYN_STATE> hiddenLayerResidual[17];
    // Gate ie: gate_n corresponds to a Conv1D gate layer with dilation factor n
    //          gate_n[r] is the r ocurrence of a gate with n dilation factor. r = {0,1}, in this case.
    Conv1DT<float,N_CHANNELS*N_SAMPLES,N_CHANNELS*N_SAMPLES,3,1,GROUPS, DYN_STATE> gate_1[2];
    Conv1DT<float,N_CHANNELS*N_SAMPLES,N_CHANNELS*N_SAMPLES,3,2,GROUPS, DYN_STATE> gate_2[2];
    Conv1DT<float,N_CHANNELS*N_SAMPLES,N_CHANNELS*N_SAMPLES,3,4,GROUPS, DYN_STATE> gate_4[2];
    Conv1DT<float,N_CHANNELS*N_SAMPLES,N_CHANNELS*N_SAMPLES,3,8,GROUPS, DYN_STATE> gate_8[2];
    Conv1DT<float,N_CHANNELS*N_SAMPLES,N_CHANNELS*N_SAMPLES,3,16,GROUPS, DYN_STATE> gate_16[2];
    Conv1DT<float,N_CHANNELS*N_SAMPLES,N_CHANNELS*N_SAMPLES,3,32,GROUPS, DYN_STATE> gate_32[2];
    Conv1DT<float,N_CHANNELS*N_SAMPLES,N_CHANNELS*N_SAMPLES,3,64,GROUPS, DYN_STATE> gate_64[2];
    Conv1DT<float,N_CHANNELS*N_SAMPLES,N_CHANNELS*N_SAMPLES,3,128,GROUPS, DYN_STATE> gate_128[2];
    Conv1DT<float,N_CHANNELS*N_SAMPLES,N_CHANNELS*N_SAMPLES,3,256,GROUPS, DYN_STATE> gate_256[2];
    // Filter ie:  filter_n corresponds to a Conv1D gate layer with dilation factor n
    Conv1DT<float,N_CHANNELS*N_SAMPLES,N_CHANNELS*N_SAMPLES,3,1,GROUPS, DYN_STATE> filter_1[2];
    Conv1DT<float,N_CHANNELS*N_SAMPLES,N_CHANNELS*N_SAMPLES,3,2,GROUPS, DYN_STATE> filter_2[2];
    Conv1DT<float,N_CHANNELS*N_SAMPLES,N_CHANNELS*N_SAMPLES,3,4,GROUPS, DYN_STATE> filter_4[2];
    Conv1DT<float,N_CHANNELS*N_SAMPLES,N_CHANNELS*N_SAMPLES,3,8,GROUPS, DYN_STATE> filter_8[2];
    Conv1DT<float,N_CHANNELS*N_SAMPLES,N_CHANNELS*N_SAMPLES,3,16,GROUPS, DYN_STATE> filter_16[2];
    Conv1DT<float,N_CHANNELS*N_SAMPLES,N_CHANNELS*N_SAMPLES,3,32,GROUPS, DYN_STATE> filter_32[2];
    Conv1DT<float,N_CHANNELS*N_SAMPLES,N_CHANNELS*N_SAMPLES,3,64,GROUPS, DYN_STATE> filter_64[2];
    Conv1DT<float,N_CHANNELS*N_SAMPLES,N_CHANNELS*N_SAMPLES,3,128,GROUPS, DYN_STATE> filter_128[2];
    Conv1DT<float,N_CHANNELS*N_SAMPLES,N_CHANNELS*N_SAMPLES,3,256,GROUPS, DYN_STATE> filter_256[2];
   
    //Output Layer
    Conv1DT<float, hiddenLayerDepth*N_CHANNELS*N_SAMPLES,N_SAMPLES,1,1,1,DYN_STATE> outputLayer;
    //Auxiliary Buffers
    Eigen::Matrix<float,N_SAMPLES*N_CHANNELS,1> outGateBuffer;//16
    Eigen::Matrix<float,N_SAMPLES*N_CHANNELS,1> hiddenInBuffer;//16
    Eigen::Matrix<float,hiddenLayerDepth*N_CHANNELS*N_SAMPLES,1> zout;


};
