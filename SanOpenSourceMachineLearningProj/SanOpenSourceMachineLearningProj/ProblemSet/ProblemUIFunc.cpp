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

	SupervisetLearning(Times, _SSTR("DataSet\\TicTacToeTrainingDataSet.txt"), Game, Learner, pTerminal);

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
void DecisionTreeLearningProblem(cSanTerminalDevice* pTerminal, SString* pstrOutputString)
{
	::system("cls");

	std::ios::sync_with_stdio(false);

	SString strTitle;
	strTitle = strTitle + _SSTR("/*--------------- CSE5693 Machine Learning HW2 ---------------*/\n");
	strTitle = strTitle + _SSTR("/*Project: Decision Tree Algorithm                            */\n");
	strTitle = strTitle + _SSTR("/*Author:  Wang Kai                                           */\n");
	strTitle = strTitle + _SSTR("/*------------------------------------------------------------*/\n");

	pTerminal->iSetTerminalTittle(_SSTR("San Machine Learning Terminal"));

	vector<ATTRIBUTE>	AttributeArray;
	vector<INSTANCE>	TrainInstanceArray;
	vector<INSTANCE>	ValidationInstanceArray;
	vector<INSTANCE>	TestInstanceArray;
	vector<SString>		ResultArray;

	SString strOutput;

	char Buffer[1024];

	pTerminal->iOutputString(strTitle);
	pTerminal->iOutputString(_SSTR("Please choose module (1 or 2):"));
	::cin >> Buffer;

	SString strCommand = ::gloAToT(SStringA(Buffer));

	/*Load DataSet*/
	::cin.getline(Buffer, 1024);

	pTerminal->iClearScreen();

	pTerminal->iOutputString(strTitle);
	pTerminal->iOutputString(_SSTR("DECISION TREE ALGORITHM:\n"));
	pTerminal->iOutputString(_SSTR("Please enter data set file name:\r\n"), STC_GREY);
	pTerminal->iOutputString(_SSTR("EXAMPLE INPUT: restaurant     for restaurant data set\r\n"), STC_GREY);
	pTerminal->iOutputString(_SSTR("EXAMPLE INPUT: ids            for ids data set\r\n"), STC_GREY);
	pTerminal->iOutputString(_SSTR("EXAMPLE INPUT: ttt            for tic-tac-toe data set\r\n"), STC_GREY);

	pTerminal->iOutputString(_SSTR("Command>"));
	::cin.getline(Buffer, 1024);

	SStringA strPath = Buffer;

	SString strAttrPath = ::gloAToT(strPath) + _SSTR("-attr.txt");
	SString strTrainPath = ::gloAToT(strPath) + _SSTR("-train.txt");
	SString strTestPath = ::gloAToT(strPath) + _SSTR("-test.txt");

	pTerminal->iOutputString(_SSTR("\r\n"));
	pTerminal->iOutputString(_SSTR("Attribute defination file path: ") + strAttrPath + _SSTR("\r\n"), STC_GREY);
	pTerminal->iOutputString(_SSTR("Training data set file path:    ") + strTrainPath + _SSTR("\r\n"), STC_GREY);
	pTerminal->iOutputString(_SSTR("Test data set file path:        ") + strTestPath + _SSTR("\r\n"), STC_GREY);

	pTerminal->iOutputString(_SSTR("\r\nPlease enter validation dataset size (percentage, 0.0 - 1.0): "), STC_GREY);
	::cin.getline(Buffer, 1024);

	sfloat ValidationRate = ::gloSToF(Buffer);

	::system("pause");

	LoadAttributeArray(strAttrPath, AttributeArray, ResultArray);
	LoadInstanceSpace(strTrainPath, TrainInstanceArray, ValidationInstanceArray, ValidationRate);
	LoadInstanceSpace(strTestPath, TestInstanceArray, ValidationInstanceArray, ValidationRate);

	if (strCommand == _SSTR("1"))
	{
		cDecisionTreeAlgorithm DecisionTree;
		strOutput = GenerateDecisionTree(DecisionTree, AttributeArray, TrainInstanceArray, ResultArray);
		strOutput = strOutput + SString(_SSTR("Training Set\tby Decision tree"));
		PredictInstanceSpace(DecisionTree, TrainInstanceArray, strOutput, false);
		strOutput = strOutput + SString(_SSTR("Test Set\tby Decision tree"));
		PredictInstanceSpace(DecisionTree, TestInstanceArray, strOutput, false);
	}

	/*Noise Mode*/
	if (strCommand == _SSTR("2"))
	{
		cDecisionTreeAlgorithm DecisionTree;
		strOutput = GenerateDecisionTree(DecisionTree, AttributeArray, TrainInstanceArray, ResultArray);
		strOutput = strOutput + SString(_SSTR("Training Set\tby Decision tree"));
		PredictInstanceSpace(DecisionTree, TrainInstanceArray, strOutput, false);
		strOutput = strOutput + SString(_SSTR("Test Set\tby Decision tree"));
		PredictInstanceSpace(DecisionTree, TestInstanceArray, strOutput, false);
	}

	if (strCommand == _SSTR("t"))
	{
	}

	::wcout << strOutput.c_str();

	::gloWriteFile(_SSTR("Output.txt"), strOutput);
	::system("notepad Output.txt");

	::system("pause");
}