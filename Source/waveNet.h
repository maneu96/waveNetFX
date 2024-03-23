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

std::unique_ptr<RTNeural::Model<float>> initWaveNet();
using namespace RTNeural;
using namespace std;

//typedef Eigen::Matrix<float, 16, 1> Vector16f;

/*using LayerVariant = std::variant<
    Conv1DT<float, 16, 16, 3, 1>,
    Conv1DT<float, 16, 16, 3, 2>,
    Conv1DT<float, 16, 16, 3, 4>,
    Conv1DT<float, 16, 16, 3, 8>,
    Conv1DT<float, 16, 16, 3, 16>,
    Conv1DT<float, 16, 16, 3, 32>,
    Conv1DT<float,16,16,3,64>,
    Conv1DT<float,16,16,3,128>,
    Conv1DT<float,16,16,3,256>
    >; */

/*class BaseLayer {
public:
    virtual void forward(float* input) = 0;
     virtual void setWeights(vector<vector<vector<float>>>  weights) = 0;
     virtual void setBias(std::vector<float> bias) = 0;// Provide a method to access outs
     virtual ~BaseLayer() {}
};

template<int N1, int N2, int N3, int N4>
class Conv1DLayer : public BaseLayer {
   

public:
    Conv1DT<float, N1, N2, N3, N4> layer;
    void forward(float input[16]) override {
       //layer.forward(input);
    }
    void setWeights(vector<vector<vector<float>>>  weights) override{
        layer.setWeights(weights);
    }
    void setBias(const std::vector<float> biases) override{
        layer.setBias(biases);
    }
    
    float getOuts() override {
        return layer.outs;  // Assuming outs is public or there's an accessor in Conv1DT
    }
};
*/
// Define specific layers
/*using Layer_16_16_3_1 = Conv1DLayer<16, 16, 3, 1> ;
using Layer_16_16_3_2 = Conv1DLayer<16, 16, 3, 2> ;
using Layer_16_16_3_4 = Conv1DLayer<16, 16, 3, 4> ;
using Layer_16_16_3_8 = Conv1DLayer<16, 16, 3, 8> ;
using Layer_16_16_3_16 = Conv1DLayer<16, 16, 3, 16> ;
using Layer_16_16_3_32 = Conv1DLayer<16, 16, 3, 32>;
using Layer_16_16_3_64 = Conv1DLayer<16, 16, 3, 64> ;
using Layer_16_16_3_128 = Conv1DLayer<16, 16, 3, 128> ;
using Layer_16_16_3_256 = Conv1DLayer<16, 16, 3, 256> ;
using Layer_16_16_1_1 =  Conv1DLayer<16, 16, 1, 1> ;

*/
class waveNet {

    RTNeural::Conv1DT<float,1,16,1,1> inputLayer;
    RTNeural::SigmoidActivationT<float, 16> hiddenGateActivation[18];
    RTNeural::TanhActivationT<float, 16> hiddenFilterActivation[18];
    RTNeural::Conv1DT<float,16,16,1,1> hiddenLayerResidual[17];
    float zout[288];
    RTNeural::Conv1DT<float, 288,1,1,1> outputLayer;

   // std::array<std::unique_ptr<BaseLayer>, 18> layersGate;
   // std::array<std::unique_ptr<BaseLayer>, 18> layersFilter;
    
public:
    waveNet();
    ~waveNet();
    void loadLayers();
    float predict(float x);
    void reDimension(std::vector<std::vector<std::vector<float>>> source,
                              std::vector<std::vector<std::vector<float>>> &destination);
    void hiddenLayerForward (uint index);
    
private:
    void loadHiddenLayers(int &iLayer,nlohmann::json layersJson);
    void resetLayers();

    
    static constexpr int hiddenLayerDepth=18; //hidden layer size
    
    Conv1DT<float,16,16,3,1> gate_1[2];
    Conv1DT<float,16,16,3,2> gate_2[2];
    Conv1DT<float,16,16,3,4> gate_4[2];
    Conv1DT<float,16,16,3,8> gate_8[2];
    Conv1DT<float,16,16,3,16> gate_16[2];
    Conv1DT<float,16,16,3,32> gate_32[2];
    Conv1DT<float,16,16,3,64> gate_64[2];
    Conv1DT<float,16,16,3,128> gate_128[2];
    Conv1DT<float,16,16,3,256> gate_256[2];
    
    Conv1DT<float,16,16,3,1> filter_1[2];
    Conv1DT<float,16,16,3,2> filter_2[2];
    Conv1DT<float,16,16,3,4> filter_4[2];
    Conv1DT<float,16,16,3,8> filter_8[2];
    Conv1DT<float,16,16,3,16> filter_16[2];
    Conv1DT<float,16,16,3,32> filter_32[2];
    Conv1DT<float,16,16,3,64> filter_64[2];
    Conv1DT<float,16,16,3,128> filter_128[2];
    Conv1DT<float,16,16,3,256> filter_256[2];
   
    
    Eigen::Matrix<float,16,1> outGateBuffer;//16
    Eigen::Matrix<float,16,1> outFilterBuffer;//16
    float inputHiddenLayer[16];
   // Eigen::Map<Eigen::VectorXf> zOutBuffer; //size 32
    
    //Vector
    // std::array<LayerVariant, 18> layersGate;
    //std::array<LayerVariant, 18> layersFilter;

};


