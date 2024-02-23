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


std::unique_ptr<RTNeural::Model<float>> initWaveNet();

class waveNet {
    RTNeural::Conv1DT<float,1,16,1,1> inputLayer;
    
    RTNeural::Conv1DT<float,16,16,3,2>  hiddenLayerGate[2];
    RTNeural::Conv1DT<float,16,16,3,2>  hiddenLayerFilter[2];
    
    
    // Activations!!!
    RTNeural::SigmoidActivationT<float, 16> hiddenGateActivation[2];
    RTNeural::TanhActivationT<float, 16> hiddenFilterActivation[2] ;
    
    RTNeural::Conv1DT<float,16,16,1,1>  hiddenLayerResidual;
    // Output layer

    RTNeural::Conv1DT<float, 32,1,1,1> outputLayer;
    
public:
    void loadLayers();
    void predict(float x);
    
    void reDimension(std::vector<std::vector<std::vector<float>>> source,
                              std::vector<std::vector<std::vector<float>>> &destination);
};


