#include"ProblemUIFunc.h"
using namespace std;
using namespace San;
using namespace San::Device;
using namespace San::MachineLearning;
void TicTacToeLearningProblem(cSanTerminalDevice* pTerminal, SString* pstrOutputString)
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
	TicTacToePrintBoard(Game, 1, 2, pTerminal);

	pTerminal->iOutputString(_SSTR("Please Enter Supervise Learning Times: "));
	::cin >> Times;

	SupervisetLearning(Times, _SSTR("A1DataSet.txt"), Game, Learner, pTerminal);

	pTerminal->iOutputString(_SSTR("Please Enter Self Learning Times: "));
	::cin >> Times;

	SelfLearning(Times, Game, Learner, pTerminal);

	pTerminal->iOutputString(_SSTR("Please Enter AI - AI Context Times: "));
	::cin >> Times;

	ContextFuncAIAI(Times, Game, Learner, pTerminal);

	::system("pause");

	ContextFunc(Game, Learner, pTerminal);

	::system("pause");
}