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

void		TicTacToePrintBoard(const cTicTacToeGame &GameObj, const uint32 LearnerID, const uint32 SuperviserID, cSanTerminalDevice &Terminal);
POSITION2D	TicTacToeCreatePlayer(cTicTacToeGame &Game, SHANDLE LearnerParam, SHANDLE SuperviserParam, const uint32 Mode);

/*Actuall Learning Process Functions*/
void	SupervisetLearning(const uint32 Times, const SString strFilePath, cTicTacToeGame &Game, cLinearLearningAlgorithm<POSITION2D> &Learner);
void	SelfLearning(const uint32 Times, cTicTacToeGame &Game, cLinearLearningAlgorithm<POSITION2D> &Learner);
void	ContextFunc(cTicTacToeGame &Game, cLinearLearningAlgorithm<POSITION2D> &Learner);
void	ContextFuncAIAI(const uint32 Times, cTicTacToeGame &Game, cLinearLearningAlgorithm<POSITION2D> &Learner);

POSITION2D	GetUserMove(SHANDLE lParam, SHANDLE wParam);
POSITION2D	GetAIMove(SHANDLE lParam, SHANDLE wParam);

/*Main funciton, process order: supervisor learning - self learning - AI,AI test - AI,User test*/
inline int32 TicTacToeLinearLearningProblem(cSanTerminalDevice &Terminal)
{
	const uint32	BoardSize = 3;
	uint32			Times = 0;

	/*Create Linear Learning Object*/
	cLinearLearningAlgorithm<POSITION2D> Learner;

	/* Create Game Object and Setup Players*/
	cTicTacToeGame Game(BoardSize);

	/*Setup Linear Learning Object Features and Available State*/
	Learner.iCreateFeature(_SSTR("X0"), 1.0, &Game, OrigionalFeatureFunc);
	Learner.iCreateFeature(_SSTR("ConCol"), 1.0, &Game, ConnectionDetectorCol);
	Learner.iCreateFeature(_SSTR("ConRow"), 1.0, &Game, ConnectionDetectorRow);
	//Learner.iCreateFeature(_SSTR("ConDiag1"), 1.0, &Game, ConnectionDetectorDiag1);
	Learner.iCreateFeature(_SSTR("ConDiag2"), 1.0, &Game, ConnectionDetectorDiag2);
	Learner.iCreateFeature(_SSTR("OpConCol"), 1.0, &Game, OpponentConnectionDetectorCol);
	Learner.iCreateFeature(_SSTR("OpConRow"), 1.0, &Game, OpponentConnectionDetectorRow);
	//Learner.iCreateFeature(_SSTR("OpConDiag1"), 1.0, &Game, OpponentConnectionDetectorDiag1);
	Learner.iCreateFeature(_SSTR("OpConDiag2"), 1.0, &Game, OpponentConnectionDetectorDiag2);
	//Learner.iCreateFeature(_SSTR("Corner"), 1.0, &Game, CornerPosDetector);

	Learner.iSetStep(0.1);

	/*Learning and Context*/
	TicTacToePrintBoard(Game, 1, 2, Terminal);

	Terminal.iOutputString(_SSTR("Please Enter Supervise Learning Times: "));
	::cin >> Times;

	SupervisetLearning(Times, _SSTR("A1DataSet.txt"), Game, Learner);

	Terminal.iOutputString(_SSTR("Please Enter Self Learning Times: "));
	::cin >> Times;

	SelfLearning(Times, Game, Learner);

	Terminal.iOutputString(_SSTR("Please Enter AI - AI Context Times: "));
	::cin >> Times;

	ContextFuncAIAI(Times, Game, Learner);

	::system("pause");

	ContextFunc(Game, Learner);

	::system("pause");

	return 0;
}
#endif