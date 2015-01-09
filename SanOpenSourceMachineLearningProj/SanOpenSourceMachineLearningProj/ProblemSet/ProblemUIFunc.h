#include"../Device/cSanTerminalDeviceWin.h"
#include"../FileIO/SanFileIO.h"
#include"TicTacToeLinearLearningProblem.h"
using namespace std;
using namespace San;
using namespace San::Device;
using namespace San::MachineLearning;
#pragma once
#ifndef __PROBLEMUIFUNC_H__
#define __PROBLEMUIFUNC_H__

/*TicTacToe linear learning main funciton, process order: supervisor learning - self learning - AI,AI test - AI,User test*/
void TicTacToeLearningProblem(cSanTerminalDevice* pTerminal, SString* pstrOutputString);

#endif