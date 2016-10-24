#include"FileIO\SanFileIO.h"
#include"Device\cSanTerminalDeviceWin.h"
#include"Device\cSanTerminalDeviceWin.h"
#include"ML\cArtificialNeuralNetworkAlgorithm.h"
using namespace std;
using namespace San;
using namespace San::Device;
using namespace San::MachineLearning;
#pragma once
#ifndef __ARTIFICIALNEURALNETWORKLEARNINGPROBLEM_H__
#define __ARTIFICIALNEURALNETWORKLEARNINGPROBLEM_H__

SString ByteANN(cSanTerminalDevice* pTerminal);
SString TennisANN(cSanTerminalDevice* pTerminal);
SString TennisSANN(cSanTerminalDevice* pTerminal);
SString IrisANN(cSanTerminalDevice* pTerminal);
SString IrisPANN(cSanTerminalDevice* pTerminal);
SString GeneralANN(cSanTerminalDevice* pTerminal);
SString GeneralNoiseANN(cSanTerminalDevice* pTerminal);

sfloat CalcAccuracy(const vector<sfloat> &PredictVector, const vector<sfloat> &TrainOutputVector);
sfloat CalcDistance(const vector<sfloat> &Vec1, const vector<sfloat> &Vec2);
void PrintInstance(const pair<vector<sfloat>, vector<sfloat>> &Instance, cSanTerminalDevice* pTerminal);
sfloat CalcSetAccuracy(cArtificialNeuralNetworkAlgorithm &ANNNetwork, const ANNTRAININGSET &Set);

#endif