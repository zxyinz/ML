#include"FileIO\SanFileIO.h"
#include"Device\cSanTerminalDeviceWin.h"
#include"ML\cDecisionTreeAlgorithm.h"
using namespace std;
using namespace San;
using namespace San::MachineLearning;
#pragma once
#ifndef __DECISIONTREELEARNINGPROBLEM_H__
#define __DECISIONTREELEARNINGPROBLEM_H__

bool LoadAttributeArray(const SString &strFilePath, vector<ATTRIBUTE> &AttributeArray, vector<SString> &ResultArray);
bool LoadInstanceSpace(const SString &strFilePath, vector<INSTANCE> &TrainingInstanceSpace, vector<INSTANCE> &ValidationInstanceSpace, const sfloat ValidationFactor);
sfloat PredictInstanceSpace(cDecisionTreeAlgorithm &DecisionTree, vector<INSTANCE> &InstanceArray, SString &strOutput, bool bUseRule);
SString GenerateDecisionTree(cDecisionTreeAlgorithm &DecisionTree, vector<ATTRIBUTE> &AttributeArray, vector<INSTANCE> &TrainInstanceArray, vector<SString> &ResultArray);

#endif