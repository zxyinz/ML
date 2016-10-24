#include"ProblemUIFunc.h"
using namespace std;
using namespace San;
using namespace San::Device;
using namespace San::FileIOStream;
using namespace San::MachineLearning;
void DecisionTreeLearningProblem(cSanTerminalDevice* pTerminal, SString* pstrOutputString)
{
	const SString strLineN = _SSTR("----------------------------------------\r\n\r\n");
	const SString strLine = _SSTR("----------------------------------------\r\n");

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

	//::system("pause");

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

		if (!::gloIsFloatEqual(ValidationRate, 0.0))
		{
			DecisionTree.iPruningTree(ValidationInstanceArray);

			strOutput = strOutput + SString(_SSTR("\r\n\r\nRule Array(after purning):\r\n")) + strLine + DecisionTree.iPrintRule();

			strOutput = strOutput + SString(_SSTR("\r\n\r\nTraining Set\tby Role Array"));
			PredictInstanceSpace(DecisionTree, TrainInstanceArray, strOutput, true);
			strOutput = strOutput + SString(_SSTR("Test Set\tby Role array"));
			PredictInstanceSpace(DecisionTree, TestInstanceArray, strOutput, true);
		}
	}

	/*Noise Mode*/
	if (strCommand == _SSTR("2"))
	{
		sfloat MaxNoise = 0.3;
		sfloat NoiseStep = 0.02;

		while (true)
		{
			pTerminal->iOutputString(_SSTR("\r\nPlease enter maximum noise rate (percentage, 0.0 - 1.0): "), STC_GREY);
			::cin.getline(Buffer, 1024);

			MaxNoise = ::gloSToF(Buffer);

			if (MaxNoise < 0.0){ pTerminal->iOutputString(_SSTR("Error: Invalid value\r\n"), STC_WHITE, STC_RED); continue; }
			if (MaxNoise > 1.0){ pTerminal->iOutputString(_SSTR("Error: Invalid value\r\n"), STC_WHITE, STC_RED); continue; }

			break;
		}

		while (true)
		{
			pTerminal->iOutputString(_SSTR("\r\nPlease enter noise increase step (percentage, 0.0 - 1.0): "), STC_GREY);
			::cin.getline(Buffer, 1024);

			NoiseStep = ::gloSToF(Buffer);

			if (NoiseStep < 0.0){ pTerminal->iOutputString(_SSTR("Error: Invalid value\r\n"), STC_WHITE, STC_RED); continue; }
			if (NoiseStep > MaxNoise){ pTerminal->iOutputString(_SSTR("Error: Invalid value, step greater than maximum noise rate\r\n"), STC_WHITE, STC_RED); continue; }

			break;
		}

		uint32 NoiseTimes = MaxNoise / NoiseStep + 1;

		const uint32 SampleTimes = 5;

		vector<sfloat> TrainAccDecisionTree(NoiseTimes);
		vector<sfloat> TestAccDecisionTree(NoiseTimes);
		vector<sfloat> TrainAccRuleArray(NoiseTimes);
		vector<sfloat> TestAccRuleArray(NoiseTimes);

		for (uint32 seek = 0; seek < NoiseTimes; seek = seek + 1)
		{
			TrainAccDecisionTree[seek] = 0.0;
			TestAccDecisionTree[seek] = 0.0;
			TrainAccRuleArray[seek] = 0.0;
			TestAccRuleArray[seek] = 0.0;
		}

		for (sfloat seek = 0.0; seek <= MaxNoise; seek = seek + NoiseStep)
		{
			pTerminal->iOutputString(_SSTR("Current noise rate: ") + ::gloFToS(seek * 100.0,_SSTR("5.2")) + _SSTR("%\r\n"));

			//#pragma omp parallel for
			for (uint32 seek_times = 0; seek_times < SampleTimes; seek_times = seek_times + 1)
			{
				//strOutput = strOutput + _SSTR("Noise rate:") + ::gloIToS(seek*100.0) + _SSTR("%\r\n");
				vector<INSTANCE> NoiseTrainSpace;
				vector<INSTANCE> NoiseVaildationSpace;
				vector<INSTANCE> AccVaildationSpace;
				for (uint32 seek_instance = 0; seek_instance < TrainInstanceArray.size(); seek_instance = seek_instance + 1)
				{
					INSTANCE Instance = TrainInstanceArray[seek_instance];

					sfloat RandomVal = rand() % 10000;
					RandomVal = RandomVal / 10000.0;

					/*Generate Noise*/
					if (RandomVal < seek)
					{
						while (true)
						{
							uint32 Index = ::rand() % ResultArray.size();
							if (Instance.first.back() != ResultArray[Index])
							{
								Instance.first.back() = ResultArray[Index];
								Instance.second = 1.0;
								break;
							}
						}
					}
					/*Generate Vaildation Set*/
					RandomVal = ::rand() % 10000;
					RandomVal = RandomVal / 10000.0;
					if (RandomVal >= ValidationRate)
					{
						NoiseTrainSpace.push_back(Instance);
					}
					else
					{
						NoiseVaildationSpace.push_back(Instance);
						AccVaildationSpace.push_back(TrainInstanceArray[seek]);
					}
				}

				/*Generate Tree*/
				cDecisionTreeAlgorithm DecisionTree;
				SString strAcc;

				GenerateDecisionTree(DecisionTree, AttributeArray, NoiseTrainSpace, ResultArray);

				uint32 Index = seek / NoiseStep;
				TrainAccDecisionTree[Index] = TrainAccDecisionTree[Index] + PredictInstanceSpace(DecisionTree, NoiseTrainSpace, strAcc, true);
				TestAccDecisionTree[Index] = TestAccDecisionTree[Index] + PredictInstanceSpace(DecisionTree, TestInstanceArray, strAcc, true);

				/*Pruning*/
				if (!::gloIsFloatEqual(ValidationRate, 0.0))
				{
					DecisionTree.iPruningTree(NoiseVaildationSpace);

					TrainAccRuleArray[Index] = TrainAccRuleArray[Index] + PredictInstanceSpace(DecisionTree, NoiseTrainSpace, strAcc, true);
					TestAccRuleArray[Index] = TestAccRuleArray[Index] + PredictInstanceSpace(DecisionTree, TestInstanceArray, strAcc, true);
				}
			}
		}

		strOutput = strOutput + SString(_SSTR("\r\n\r\nTest Set\tby Decision tree:\r\n")) + strLine;
		for (uint32 seek = 0; seek < TestAccDecisionTree.size(); seek = seek + 1)
		{
			strOutput = strOutput + ::gloIToS((TestAccDecisionTree[seek] / SampleTimes) * 100.0) + _SSTR("% ");
		}

		strOutput = strOutput + SString(_SSTR("\r\n\r\nTest Set\tby Rule Array:\r\n")) + strLine;
		for (uint32 seek = 0; seek < TestAccRuleArray.size(); seek = seek + 1)
		{
			strOutput = strOutput + ::gloIToS((TestAccRuleArray[seek] / SampleTimes) * 100.0) + _SSTR("% ");
		}
		strOutput = strOutput + _SSTR("\r\n");
	}

	if (strCommand == _SSTR("t"))
	{
	}

	::wcout << strOutput.c_str();

	::gloWriteFile(_SSTR("Output.txt"), strOutput);
	::system("notepad Output.txt");

	::system("pause");
}
void ArtificialNeuralNetworkLearningProblem(cSanTerminalDevice* pTerminal, SString* pstrOutputString)
{
	SString strTitle;
	strTitle = strTitle + _SSTR("/*--------------- CSE5693 Machine Learning HW2 ---------------*/\n");
	strTitle = strTitle + _SSTR("/*Project: Artificial Neural Network Algorithm                */\n");
	strTitle = strTitle + _SSTR("/*Author:  Wang Kai                                           */\n");
	strTitle = strTitle + _SSTR("/*------------------------------------------------------------*/\n");

	pTerminal->iSetTerminalTittle(_SSTR("San Machine Learning Terminal"));

	pTerminal->iClearScreen();

	pTerminal->iOutputString(strTitle);
	pTerminal->iOutputString(_SSTR("ARTIFICIAL NEURAL NETWORK ALGORITHM : \n\n"), STC_GREY);
	pTerminal->iOutputString(_SSTR("Mode:\n\t0 - Byte\n\t1 - Tennis\n\t2 - Tennis Sepatate\n\t3 - Iris\n\t4 - Iris Validation\n\t5 - General Validation\n\t6 - General Noise Validation\n\n"), STC_GREY);
	pTerminal->iOutputString(_SSTR("Please Select Mode(0 - 6) :"), STC_GREY);

	uint32 Mode;

	::cin >> Mode;

	Mode = Mode <= 6 ? Mode : 0;

	pTerminal->iClearScreen();

	SString strOutput;

	switch (Mode)
	{
	case 0:strOutput = ByteANN(pTerminal); break;
	case 1:strOutput = TennisANN(pTerminal); break;
	case 2:strOutput = TennisSANN(pTerminal); break;
	case 3:strOutput = IrisANN(pTerminal); break;
	case 4:strOutput = IrisPANN(pTerminal); break;
	case 5:strOutput = GeneralANN(pTerminal); break;
	case 6:strOutput = GeneralNoiseANN(pTerminal); break;
	default:break;
	}

	if (pstrOutputString != nullptr){ *pstrOutputString = strOutput; }

	::system("pause");
}
void GeneticAlgorithmLearningProblem(cSanTerminalDevice* pTerminal, SString* pstrOutputString)
{
	SString strTitle;
	strTitle = strTitle + _SSTR("/*--------------- CSE5693 Machine Learning HW2 ---------------*/\n");
	strTitle = strTitle + _SSTR("/*Project: Genetic Algorithm                                  */\n");
	strTitle = strTitle + _SSTR("/*Author:  Wang Kai                                           */\n");
	strTitle = strTitle + _SSTR("/*------------------------------------------------------------*/\n");

	pTerminal->iSetTerminalTittle(_SSTR("San Machine Learning Terminal"));

	pTerminal->iClearScreen();

	pTerminal->iOutputString(strTitle);
	pTerminal->iOutputString(_SSTR("GENETIC ALGORITHM:\n\n"), STC_GREY);
	pTerminal->iOutputString(_SSTR("Mode:\n\t0 - Tennis\n\t1 - Iris\n\t2 - Iris Seletion\n\t3 - Iris Replacement\n\n"), STC_GREY);
	pTerminal->iOutputString(_SSTR("Please Select Mode (0 - 3):"), STC_GREY);

	uint32 Mode;

	::cin >> Mode;

	Mode = Mode <= 6 ? Mode : 0;

	pTerminal->iClearScreen();

	SString strOutput;

	switch (Mode)
	{
	case 0:strOutput = TennisGA(pTerminal); break;
	case 1:strOutput = IrisGA(pTerminal); break;
	case 2:strOutput = IrisSGA(pTerminal); break;
	case 3:strOutput = IrisRGA(pTerminal); break;
	default:break;
	}

	if (pstrOutputString != nullptr){ *pstrOutputString = strOutput; }

	::system("pause");
}