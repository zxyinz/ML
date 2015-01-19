#include"..\FileIO\SanFileIO.h"
#include"..\Device\cSanTerminalDeviceWin.h"
#include"..\Device\cSanTerminalDeviceWin.h"
#include"..\ML\cArtificialNeuralNetworkAlgorithm.h"
using namespace std;
using namespace San;
using namespace San::Device;
using namespace San::MachineLearning;
#pragma once
#ifndef __ARTIFICIALNEURALNETWORKLEARNINGPROBLEM_H__
#define __ARTIFICIALNEURALNETWORKLEARNINGPROBLEM_H__

void ByteANN(cSanTerminalDevice* pTerminal);
void TennisANN(cSanTerminalDevice* pTerminal);
void TennisSANN(cSanTerminalDevice* pTerminal);
void IrisANN(cSanTerminalDevice* pTerminal);
void IrisPANN(cSanTerminalDevice* pTerminal);

sfloat CalcAccuracy(const vector<sfloat> &PredictVector, const vector<sfloat> &TrainOutputVector);
sfloat CalcDistance(const vector<sfloat> &Vec1, const vector<sfloat> &Vec2);
void PrintInstance(const pair<vector<sfloat>, vector<sfloat>> &Instance);
sfloat CalcSetAccuracy(cArtificialNeuralNetworkAlgorithm &ANNNetwork, const ANNTRAININGSET &Set);

#endif