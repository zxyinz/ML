#include"Device/cSanTerminalDeviceWin.h"
#include"FileIO/SanFileIO.h"
#include"DecisionTreeLearningProblem.h"
#include"ArtificialNeuralNetworkLearningProblem.h"
#include"GeneticAlgorithmLearningProblem.h"
using namespace std;
#pragma once
#ifndef __PROBLEMUIFUNC_H__
#define __PROBLEMUIFUNC_H__

void DecisionTreeLearningProblem(cSanTerminalDevice* pTerminal, SString* pstrOutputString);
void ArtificialNeuralNetworkLearningProblem(cSanTerminalDevice* pTerminal, SString* pstrOutputString);
void GeneticAlgorithmLearningProblem(cSanTerminalDevice* pTerminal, SString* pstrOutputString);

#endif