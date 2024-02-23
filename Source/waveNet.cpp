/*
  ==============================================================================

    waveNet.cpp
    Created: 18 Jan 2024 5:38:52pm
    Author:  Manuel Freitas

  ==============================================================================
*/

#include "waveNet.h"
#include <stdio.h>

using namespace RTNeural;

#define _POSIX_SOURCE
#include <unistd.h>

void waveNet::loadLayers(){
   /* std::ifstream jsonStream("/Users/maneu/Downloads/updated_conv1d_network_parameters.json"); */
    /* RTNeural::Conv1DT<float, 8, 4, 3, 2 > conv1D;*/
    std::ifstream jsonStream("/Users/maneu/Downloads/ModelKeras.json");
    nlohmann::json j ;
    
    inputLayer.reset();
    outputLayer.reset();
    hiddenLayerGate[0].reset();
    hiddenLayerFilter[0].reset();
    hiddenLayerGate[1].reset();
    hiddenLayerFilter[1].reset();
    hiddenGateActivation[0].reset();
    hiddenFilterActivation[0].reset();
    hiddenGateActivation[1].reset();
    hiddenFilterActivation[1].reset();
    hiddenLayerResidual.reset();
    if(jsonStream.is_open()){

        jsonStream >> j;
        auto layersJson = j.at("layers");
        
        
        // LOAD weights and Biases Input layer
        auto inputLayerJson = layersJson.at(0);
        std::vector<std::vector<std::vector<float> > > weightsInputLayer (16,std::vector<std::vector<float> >(1,std::vector <float>(1,0)));

        reDimension(inputLayerJson.at("weights").at(0).get<std::vector<std::vector<std::vector<float>>>>(), weightsInputLayer);
    
        inputLayer.setWeights(weightsInputLayer);
        inputLayer.setBias( inputLayerJson.at("weights").at(1).get<std::vector<float>>());
        
        // Load HIDDEN LAYER WEIGHTS AND BIASES
        
        // Gate
        auto hiddenLayerGateJson = layersJson.at(1);
        std::vector<std::vector<std::vector<float> > > weightsHiddenGateLayer (16,std::vector<std::vector<float> >(16,std::vector <float>(3,0)));
    
        
        reDimension(hiddenLayerGateJson.at("weights").at(0).get<std::vector<std::vector<std::vector<float>>>>(), weightsHiddenGateLayer);
        hiddenLayerGate[0].setWeights(weightsHiddenGateLayer);
        hiddenLayerGate[0].setBias( hiddenLayerGateJson.at("weights").at(1).get<std::vector<float>>());
    
        // Filter
        auto hiddenLayerFilterJson = layersJson.at(2);
        std::vector<std::vector<std::vector<float> > > weightsHiddenFilterLayer (16,std::vector<std::vector<float> >(16,std::vector <float>(3,0)));
        reDimension(hiddenLayerFilterJson.at("weights").at(0).get<std::vector<std::vector<std::vector<float>>>>(), weightsHiddenFilterLayer);
        hiddenLayerFilter[0].setWeights(weightsHiddenFilterLayer);
        hiddenLayerFilter[0].setBias( hiddenLayerFilterJson.at("weights").at(1).get<std::vector<float>>());

        
        // Hidden
        auto hiddenLayerResidualJson = layersJson.at(3);
        std::vector<std::vector<std::vector<float> > > weightsHiddenLayerResidual (16,std::vector<std::vector<float> >(16,std::vector <float>(1,0)));
        reDimension(hiddenLayerResidualJson.at("weights").at(0).get<std::vector<std::vector<std::vector<float>>>>(), weightsHiddenLayerResidual);
        hiddenLayerResidual.setWeights(weightsHiddenLayerResidual);
        hiddenLayerResidual.setBias( hiddenLayerResidualJson.at("weights").at(1).get<std::vector<float>>());
        
        
        
        //Hidden 2
        // Gate
        hiddenLayerGateJson = layersJson.at(4);

        reDimension(hiddenLayerGateJson.at("weights").at(0).get<std::vector<std::vector<std::vector<float>>>>(), weightsHiddenGateLayer);
        hiddenLayerGate[1].setWeights(weightsHiddenGateLayer);
        hiddenLayerGate[1].setBias( hiddenLayerGateJson.at("weights").at(1).get<std::vector<float>>());
    
        // Filter
        hiddenLayerFilterJson = layersJson.at(5);
        reDimension(hiddenLayerFilterJson.at("weights").at(0).get<std::vector<std::vector<std::vector<float>>>>(), weightsHiddenFilterLayer);
        hiddenLayerFilter[1].setWeights(weightsHiddenFilterLayer);
        hiddenLayerFilter[1].setBias( hiddenLayerFilterJson.at("weights").at(1).get<std::vector<float>>());
        
        // Output
        auto outputLayerJson = layersJson.at(6);
        
        std::vector<std::vector<std::vector<float> > > weightsOutputLayer (1,std::vector<std::vector<float> >(16,std::vector <float>(1,0)));

        reDimension(outputLayerJson.at("weights").at(0).get<std::vector<std::vector<std::vector<float>>>>(), weightsOutputLayer);
        outputLayer.setWeights(weightsOutputLayer);
        outputLayer.setBias(outputLayerJson.at("weights").at(1).get<std::vector<float>>());
        
   
    }
    else{
        std::cout << "Error opening File"<< std::endl;
    }
    
}

void waveNet::predict(float x){
    float hiddenLayerInput [16];
    float input[1] = {x} ; // Cast the sample into the correct format
    inputLayer.forward(input);//input); // Process it trough the input layer
    
    hiddenLayerFilter[0].forward(inputLayer.outs); // Process it through the Filter and Gate layers, in parallel
    hiddenLayerGate[0].forward(inputLayer.outs);
    
    hiddenGateActivation[0].forward(hiddenLayerGate[0].outs); // Activate the outputs of the Filter and Gate layers
    hiddenFilterActivation[0].forward(hiddenLayerFilter[0].outs);
    
    float outGatedActivation[16] = {                                        // Multiply the outputs
        hiddenGateActivation[0].outs[0]*hiddenFilterActivation[0].outs[0],
        hiddenGateActivation[0].outs[1]*hiddenFilterActivation[0].outs[1],
        hiddenGateActivation[0].outs[2]*hiddenFilterActivation[0].outs[2],
        hiddenGateActivation[0].outs[3]*hiddenFilterActivation[0].outs[3],
        hiddenGateActivation[0].outs[4]*hiddenFilterActivation[0].outs[4],
        hiddenGateActivation[0].outs[5]*hiddenFilterActivation[0].outs[5],
        hiddenGateActivation[0].outs[6]*hiddenFilterActivation[0].outs[6],
        hiddenGateActivation[0].outs[7]*hiddenFilterActivation[0].outs[7],
        hiddenGateActivation[0].outs[8]*hiddenFilterActivation[0].outs[8],
        hiddenGateActivation[0].outs[9]*hiddenFilterActivation[0].outs[9],
        hiddenGateActivation[0].outs[10]*hiddenFilterActivation[0].outs[10],
        hiddenGateActivation[0].outs[11]*hiddenFilterActivation[0].outs[11],
        hiddenGateActivation[0].outs[12]*hiddenFilterActivation[0].outs[12],
        hiddenGateActivation[0].outs[13]*hiddenFilterActivation[0].outs[13],
        hiddenGateActivation[0].outs[14]*hiddenFilterActivation[0].outs[14],
        hiddenGateActivation[0].outs[15]*hiddenFilterActivation[0].outs[15]
    };// OPTIMIZE THIS
    hiddenLayerResidual.forward(outGatedActivation);
    for(uint i = 0; i< 16; ++i )
        hiddenLayerInput[i] = inputLayer.outs[i] + hiddenLayerResidual.outs[i];
    
    hiddenLayerFilter[1].forward(hiddenLayerInput);
    hiddenLayerGate[1].forward(hiddenLayerInput);
    
    hiddenGateActivation[1].forward(hiddenLayerGate[1].outs); // Activate the outputs of the Filter and Gate layers
    hiddenFilterActivation[1].forward(hiddenLayerFilter[1].outs);
    
    float outGatedActivation2 [16] = {                                        // Multiply the outputs
        hiddenGateActivation[1].outs[0]*hiddenFilterActivation[1].outs[0],
        hiddenGateActivation[1].outs[1]*hiddenFilterActivation[1].outs[1],
        hiddenGateActivation[1].outs[2]*hiddenFilterActivation[1].outs[2],
        hiddenGateActivation[1].outs[3]*hiddenFilterActivation[1].outs[3],
        hiddenGateActivation[1].outs[4]*hiddenFilterActivation[1].outs[4],
        hiddenGateActivation[1].outs[5]*hiddenFilterActivation[1].outs[5],
        hiddenGateActivation[1].outs[6]*hiddenFilterActivation[1].outs[6],
        hiddenGateActivation[1].outs[7]*hiddenFilterActivation[1].outs[7],
        hiddenGateActivation[1].outs[8]*hiddenFilterActivation[1].outs[8],
        hiddenGateActivation[1].outs[9]*hiddenFilterActivation[1].outs[9],
        hiddenGateActivation[1].outs[10]*hiddenFilterActivation[1].outs[10],
        hiddenGateActivation[1].outs[11]*hiddenFilterActivation[1].outs[11],
        hiddenGateActivation[1].outs[12]*hiddenFilterActivation[1].outs[12],
        hiddenGateActivation[1].outs[13]*hiddenFilterActivation[1].outs[13],
        hiddenGateActivation[1].outs[14]*hiddenFilterActivation[1].outs[14],
        hiddenGateActivation[1].outs[15]*hiddenFilterActivation[1].outs[15]
    };// OPTIMIZE THIS
    uint j = 32;
    float outGatedActivations[32];
    for (uint i = 0; i< 32;++i){
        j--;
        if(i<16)
        {
            outGatedActivations[i] = outGatedActivation[i];
        }
        else
        {
            outGatedActivations[i] = outGatedActivation2[i - 16];
        }
    
    }
    outputLayer.forward(outGatedActivations);  // Process it trough the final layer
    std::cout << "The output is " << outputLayer.outs[0] << std::endl;
}


void waveNet::reDimension(std::vector<std::vector<std::vector<float>>> source,
                          std::vector<std::vector<std::vector<float>>> &destination) {
    
    // Assuming source dimensions are 1x15x16
    size_t sourceDepth = source.size();
    size_t sourceRows = source[0].size();
    size_t sourceCols = source[0][0].size();
    
    // Resize destination to match 16x15x1 dimensions
    destination.resize(sourceCols); // New depth is the old cols (16)
    for (size_t i = 0; i < sourceCols; ++i) { // For each new "depth" layer
        destination[i].resize(sourceRows); // Each layer has 15 rows
        for (size_t j = 0; j < sourceRows; ++j) { // For each row
            destination[i][j].resize(sourceDepth); // Each cell now becomes a vector of depth 1
        }
    }
    
    // Copy data from source to destination
    size_t i_inv = sourceDepth;
    for (size_t i = 0; i < sourceDepth; ++i) {
        i_inv -- ;
        for (size_t j = 0; j < sourceRows; ++j) {
            for (size_t k = 0; k < sourceCols; ++k) {
                destination[k][j][i] = source[i_inv][j][k]; // Adjusted indexing for correct mapping
            }
        }
    }
    
}
