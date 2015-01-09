#include"..\FileIO\SanFileIO.h"
#include"..\Device\cSanTerminalDeviceWin.h"
#include"..\ML\cLinearLearningAlgorithm.h"
#include"cTicTacToeGame.h"
#include"TicTacToeDecetorFunc.h"
using namespace std;
using namespace San;
using namespace San::FileIOStream;
using namespace San::Device;
using namespace San::MachineLearning;
#pragma once
#ifndef __TICTACTOELINEARLEARNINGPROBLEM_H__
#define __TICTACTOELINEARLEARNINGPROBLEM_H__

static const sfloat WinFeedBack = 100.0;
static const sfloat LoseFeedBack = -100.0;
static const sfloat DrawFeedBack = -50.0;

void		TicTacToePrintBoard(const cTicTacToeGame &GameObj, const uint32 LearnerID, const uint32 SuperviserID, cSanTerminalDevice* pTerminal);
POSITION2D	TicTacToeCreatePlayer(cTicTacToeGame &Game, SHANDLE LearnerParam, SHANDLE SuperviserParam, const uint32 Mode);

/*Actuall Learning Process Functions*/
void	SupervisetLearning(const uint32 Times, const SString strFilePath, cTicTacToeGame &Game, cLinearLearningAlgorithm<POSITION2D> &Learner, cSanTerminalDevice* pTerminal);
void	SelfLearning(const uint32 Times, cTicTacToeGame &Game, cLinearLearningAlgorithm<POSITION2D> &Learner, cSanTerminalDevice* pTerminal);
void	ContextFunc(cTicTacToeGame &Game, cLinearLearningAlgorithm<POSITION2D> &Learner, cSanTerminalDevice* pTerminal);
void	ContextFuncAIAI(const uint32 Times, cTicTacToeGame &Game, cLinearLearningAlgorithm<POSITION2D> &Learner, cSanTerminalDevice* pTerminal);

POSITION2D	GetUserMove(SHANDLE lParam, SHANDLE wParam, cSanTerminalDevice* pTerminal);
POSITION2D	GetAIMove(SHANDLE lParam, SHANDLE wParam, cSanTerminalDevice* pTerminal);

#endif