#include"../Device/cSanTerminalDeviceWin.h"
#include"../FileIO/SanFileIO.h"
#include"TicTacToeLinearLearningProblem.h"
#include"DecisionTreeLearningProblem.h"
#include"ArtificialNeuralNetworkLearningProblem.h"
#include"GeneticAlgorithmLearningProblem.h"
using namespace std;
#pragma once
#ifndef __PROBLEMUIFUNC_H__
#define __PROBLEMUIFUNC_H__

/*TicTacToe linear learning main funciton, process order: supervisor learning - self learning - AI,AI test - AI,User test*/
void TicTacToeLearningProblem(cSanTerminalDevice* pTerminal, SString* pstrOutputString);

void DecisionTreeLearningProblem(cSanTerminalDevice* pTerminal, SString* pstrOutputString);

void ArtificialNeuralNetworkLearningProblem(cSanTerminalDevice* pTerminal, SString* pstrOutputString);

void GeneticAlgorithmLearningProblem(cSanTerminalDevice* pTerminal, SString* pstrOutputString);

#endif