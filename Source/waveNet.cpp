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

waveNet::waveNet()
 /*  : outGateBuffer(hiddenGateActivation[0].outs, 16),
     outFilterBuffer(hiddenFilterActivation[0].outs, 16)*/{
    // Initialize other layers
    loadLayers();
}
waveNet::~waveNet(){
};


void waveNet::loadLayers(){
   /* std::ifstream jsonStream("/Users/maneu/Downloads/updated_conv1d_network_parameters.json"); */
    /* RTNeural::Conv1DT<float, 8, 4, 3, 2 > conv1D;*/
    std::ifstream jsonStream("/Users/maneu/Downloads/ModelKeras.json");
    nlohmann::json j ;
    resetLayers();
    
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
        //auto hiddenLayerGateJson = layersJson.at(1);
        int index = 1;
        loadHiddenLayers(index,layersJson);
      /*  std::vector<std::vector<std::vector<float> > > weightsHiddenGateLayer (16,std::vector<std::vector<float> >(16,std::vector <float>(3,0)));
    
        
       reDimension(hiddenLayerGateJson.at("weights").at(0).get<std::vector<std::vector<std::vector<float>>>>(), weightsHiddenGateLayer);
        loadHiddenLayers(weightsHiddenGateLayer,hiddenLayerGateJson.at("weights").at(1).get<std::vector<float>>());*/
       /* hiddenLayerGate[0].setWeights(weightsHiddenGateLayer);
        hiddenLayerGate[0].setBias( hiddenLayerGateJson.at("weights").at(1).get<std::vector<float>>());*/
    
        // Filter
   /*     auto hiddenLayerFilterJson = layersJson.at(2);
        std::vector<std::vector<std::vector<float> > > weightsHiddenFilterLayer (16,std::vector<std::vector<float> >(16,std::vector <float>(3,0)));
        reDimension(hiddenLayerFilterJson.at("weights").at(0).get<std::vector<std::vector<std::vector<float>>>>(), weightsHiddenFilterLayer);
        hiddenLayerFilter[0].setWeights(weightsHiddenFilterLayer);
        hiddenLayerFilter[0].setBias( hiddenLayerFilterJson.at("weights").at(1).get<std::vector<float>>()); */

        
 /*       // Hidden
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
  */
        // Output
        auto outputLayerJson = layersJson.at(index);
        
        std::vector<std::vector<std::vector<float> > > weightsOutputLayer (1,std::vector<std::vector<float> >(288,std::vector <float>(1,0)));

        reDimension(outputLayerJson.at("weights").at(0).get<std::vector<std::vector<std::vector<float>>>>(), weightsOutputLayer);
        outputLayer.setWeights(weightsOutputLayer);
        outputLayer.setBias(outputLayerJson.at("weights").at(1).get<std::vector<float>>());
        
   
    }
    else{
        std::cout << "Error opening File"<< std::endl;
    }

}

void waveNet::loadHiddenLayers(int &iLayer,nlohmann::json layersJson ){

    for (int index = 0; index < hiddenLayerDepth; ++index){
        int repeat = 2*index/hiddenLayerDepth;
        //int dilationSelector = index%(hiddenLayerDepth/2);
        auto layerJson = layersJson.at(iLayer);
        int dilationSelector = layerJson.at("dilation").at(0).get<int>();
        
        std::vector<std::vector<std::vector<float> > > weights (16,std::vector<std::vector<float> >(16,std::vector <float>(3,0)));
        reDimension(layerJson.at("weights").at(0).get<std::vector<std::vector<std::vector<float>>>>(), weights);
        
        
        switch (dilationSelector) {
            case 1:
                gate_1[repeat].setWeights(weights);
                gate_1[repeat].setBias(layerJson.at("weights").at(1).get<std::vector<float>>());
                ++iLayer;
                
                layerJson = layersJson.at(iLayer);
                reDimension(layerJson.at("weights").at(0).get<std::vector<std::vector<std::vector<float>>>>(), weights);
                filter_1[repeat].setWeights(weights);
                filter_1[repeat].setBias(layerJson.at("weights").at(1).get<std::vector<float>>());
                ++iLayer;
                break;
            
            case 2:
                gate_2[repeat].setWeights(weights);
                gate_2[repeat].setBias(layerJson.at("weights").at(1).get<std::vector<float>>());
                ++iLayer;
                
                layerJson = layersJson.at(iLayer);
                reDimension(layerJson.at("weights").at(0).get<std::vector<std::vector<std::vector<float>>>>(), weights);
                filter_2[repeat].setWeights(weights);
                filter_2[repeat].setBias(layerJson.at("weights").at(1).get<std::vector<float>>());
                ++iLayer;
                break;
            
            case 4:
                gate_4[repeat].setWeights(weights);
                gate_4[repeat].setBias(layerJson.at("weights").at(1).get<std::vector<float>>());
                ++iLayer;
                
                layerJson = layersJson.at(iLayer);
                reDimension(layerJson.at("weights").at(0).get<std::vector<std::vector<std::vector<float>>>>(), weights);
                filter_4[repeat].setWeights(weights);
                filter_4[repeat].setBias(layerJson.at("weights").at(1).get<std::vector<float>>());
                ++iLayer;
                break;
            
            case 8:
                gate_8[repeat].setWeights(weights);
                gate_8[repeat].setBias(layerJson.at("weights").at(1).get<std::vector<float>>());
                ++iLayer;
                
                layerJson = layersJson.at(iLayer);
                reDimension(layerJson.at("weights").at(0).get<std::vector<std::vector<std::vector<float>>>>(), weights);
                filter_8[repeat].setWeights(weights);
                filter_8[repeat].setBias(layerJson.at("weights").at(1).get<std::vector<float>>());
                ++iLayer;
                break;
            
            case 16:
                gate_16[repeat].setWeights(weights);
                gate_16[repeat].setBias(layerJson.at("weights").at(1).get<std::vector<float>>());
                ++iLayer;
                
                layerJson = layersJson.at(iLayer);
                reDimension(layerJson.at("weights").at(0).get<std::vector<std::vector<std::vector<float>>>>(), weights);
                filter_16[repeat].setWeights(weights);
                filter_16[repeat].setBias(layerJson.at("weights").at(1).get<std::vector<float>>());
                ++iLayer;
                break;
            
            case 32:
                gate_32[repeat].setWeights(weights);
                gate_32[repeat].setBias(layerJson.at("weights").at(1).get<std::vector<float>>());
                ++iLayer;
                
                layerJson = layersJson.at(iLayer);
                reDimension(layerJson.at("weights").at(0).get<std::vector<std::vector<std::vector<float>>>>(), weights);
                filter_32[repeat].setWeights(weights);
                filter_32[repeat].setBias(layerJson.at("weights").at(1).get<std::vector<float>>());
                ++iLayer;
                break;
            
            case 64:
                gate_64[repeat].setWeights(weights);
                gate_64[repeat].setBias(layerJson.at("weights").at(1).get<std::vector<float>>());
                ++iLayer;
                
                layerJson = layersJson.at(iLayer);
                reDimension(layerJson.at("weights").at(0).get<std::vector<std::vector<std::vector<float>>>>(), weights);
                filter_64[repeat].setWeights(weights);
                filter_64[repeat].setBias(layerJson.at("weights").at(1).get<std::vector<float>>());
                ++iLayer;
                break;
            
            case 128:
                gate_128[repeat].setWeights(weights);
                gate_128[repeat].setBias(layerJson.at("weights").at(1).get<std::vector<float>>());
                ++iLayer;
                
                layerJson = layersJson.at(iLayer);
                reDimension(layerJson.at("weights").at(0).get<std::vector<std::vector<std::vector<float>>>>(), weights);
                filter_128[repeat].setWeights(weights);
                filter_128[repeat].setBias(layerJson.at("weights").at(1).get<std::vector<float>>());
                ++iLayer;
                break;
            
            case 256:
                gate_256[repeat].setWeights(weights);
                gate_256[repeat].setBias(layerJson.at("weights").at(1).get<std::vector<float>>());
                ++iLayer;
                
                layerJson = layersJson.at(iLayer);
                reDimension(layerJson.at("weights").at(0).get<std::vector<std::vector<std::vector<float>>>>(), weights);
                filter_256[repeat].setWeights(weights);
                filter_256[repeat].setBias(layerJson.at("weights").at(1).get<std::vector<float>>());
                ++iLayer;
                break;
            
        }

    if (index == hiddenLayerDepth -1) // last hidden layer does not have residual
        return;
    // Hidden
         auto hiddenLayerResidualJson = layersJson.at(iLayer);
         std::vector<std::vector<std::vector<float> > > weightsHiddenLayerResidual (16,std::vector<std::vector<float> >(16,std::vector <float>(1,0)));
         reDimension(hiddenLayerResidualJson.at("weights").at(0).get<std::vector<std::vector<std::vector<float>>>>(), weightsHiddenLayerResidual);
         hiddenLayerResidual[index].setWeights(weightsHiddenLayerResidual);
         hiddenLayerResidual[index].setBias( hiddenLayerResidualJson.at("weights").at(1).get<std::vector<float>>());
    ++iLayer;
}

}
float waveNet::predict(float x){
    
    float input[1] = {x};
    inputLayer.forward(input);
    for (int i = 0; i <= hiddenLayerDepth; ++i) {
        hiddenLayerForward(i);
    }
    outputLayer.forward(zout);
    return outputLayer.outs[0];
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


void waveNet::hiddenLayerForward (uint index){
    //float inputHiddenLayer[16];
    int repeat  = 2 * index/hiddenLayerDepth;
    Eigen::Map<Eigen::Matrix<float,16,1>> inputLayerOut(inputLayer.outs,16);
    if (index == 0){
        
       gate_1[0].forward(inputLayer.outs);
       hiddenGateActivation[0].forward(gate_1[0].outs);
       filter_1[0].forward(inputLayer.outs);
       hiddenFilterActivation[0].forward(filter_1[0].outs);
    }
    else{
        
        //Mapping the gate and filter outputs
        new(&outGateBuffer) Eigen::Map<Eigen::Matrix<float,16,1>> (hiddenGateActivation[index-1].outs,16);
        new(&outFilterBuffer) Eigen::Map<Eigen::Matrix<float,16,1>>(hiddenFilterActivation[index-1].outs,16);
        //Elementwise multiplication
        outGateBuffer = outGateBuffer.cwiseProduct(outFilterBuffer);
        //Saving the output
        Eigen::Map<Eigen::Matrix<float,16,1>> (zout + 16*(index-1),16)= outGateBuffer;
        //cout<< outGateBuffer<< endl;
        
        if (index == hiddenLayerDepth){
            return;
        }
        //Casting to float
        
        Eigen::Map<Eigen::Matrix<float,16,1>>(inputHiddenLayer,16) = outGateBuffer;
        
        // Performing forward inference in the residual layer
        hiddenLayerResidual[index-1].forward(inputHiddenLayer); //hiddenLayerResidual[index-1].forward
        //Mapping,16
        // Mapping the output of the residual layer
        Eigen::Map<Eigen::Matrix<float,16,1>> outResidual(hiddenLayerResidual[index-1].outs,16); //hiddenLayerResidual[index-1].outs
        // Residual operation to derive this layer's input
        Eigen::Map<Eigen::Matrix<float,16,1>>(inputHiddenLayer,16) = outResidual + inputLayerOut ;
        
        switch (index) {
            case 1:
            case 10:// dilation = 2
                gate_2[repeat].forward(inputHiddenLayer);
                hiddenGateActivation[index].forward(gate_2[repeat].outs);
                filter_2[repeat].forward(inputHiddenLayer);
                hiddenFilterActivation[index].forward(filter_2[repeat].outs);
                break;
            case 2:
            case 11:
                gate_4[repeat].forward(inputHiddenLayer);
                hiddenGateActivation[index].forward(gate_4[repeat].outs);
                filter_4[repeat].forward(inputHiddenLayer);
                hiddenFilterActivation[index].forward(filter_4[repeat].outs);
                break;
            case 3:
            case 12:
                gate_8[repeat].forward(inputHiddenLayer);
                hiddenGateActivation[index].forward(gate_8[repeat].outs);
                filter_8[repeat].forward(inputHiddenLayer);
                hiddenFilterActivation[index].forward(filter_8[repeat].outs);
                break;
            case 4:
            case 13:
                gate_16[repeat].forward(inputHiddenLayer);
                hiddenGateActivation[index].forward(gate_16[repeat].outs);
                filter_16[repeat].forward(inputHiddenLayer);
                hiddenFilterActivation[index].forward(filter_16[repeat].outs);
                break;
            case 5:
            case 14:
                gate_32[repeat].forward(inputHiddenLayer);
                hiddenGateActivation[index].forward(gate_32[repeat].outs);
                filter_32[repeat].forward(inputHiddenLayer);
                hiddenFilterActivation[index].forward(filter_32[repeat].outs);
                break;
            case 6:
            case 15:
                gate_64[repeat].forward(inputHiddenLayer);
                hiddenGateActivation[index].forward(gate_64[repeat].outs);
                filter_64[repeat].forward(inputHiddenLayer);
                hiddenFilterActivation[index].forward(filter_64[repeat].outs);
                break;
            case 7:
            case 16:
                gate_128[repeat].forward(inputHiddenLayer);
                hiddenGateActivation[index].forward(gate_128[repeat].outs);
                filter_128[repeat].forward(inputHiddenLayer);
                hiddenFilterActivation[index].forward(filter_128[repeat].outs);
                break;
            case 8:
            case 17:
                gate_256[repeat].forward(inputHiddenLayer);
                hiddenGateActivation[index].forward(gate_256[repeat].outs);
                filter_256[repeat].forward(inputHiddenLayer);
                hiddenFilterActivation[index].forward(filter_256[repeat].outs);
                break;
            case 9:
                gate_1[repeat].forward(inputHiddenLayer);
                hiddenGateActivation[index].forward(gate_1[repeat].outs);
                filter_1[repeat].forward(inputHiddenLayer);
                hiddenFilterActivation[index].forward(filter_1[repeat].outs);
                break;
        }
        
    }
}


void waveNet::resetLayers(){
    inputLayer.reset();
    outputLayer.reset();
    for(int i= 0; i < hiddenLayerDepth; i++){
        hiddenGateActivation[i].reset();
        hiddenFilterActivation[i].reset();
        hiddenLayerResidual[i].reset();
    }
    
    for(int i = 0 ; i < 2; i++){
        gate_1[i].reset();
        gate_2[i].reset();
        gate_4[i].reset();
        gate_8[i].reset();
        gate_16[i].reset();
        gate_32[i].reset();
        gate_64[i].reset();
        gate_128[i].reset();
        gate_256[i].reset();
        filter_1[i].reset();
        filter_2[i].reset();
        filter_4[i].reset();
        filter_8[i].reset();
        filter_16[i].reset();
        filter_32[i].reset();
        filter_64[i].reset();
        filter_128[i].reset();
        filter_256[i].reset();
    }

}

