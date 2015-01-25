#include"ArtificialNeuralNetworkLearningProblem.h"
using namespace std;
using namespace San;
using namespace San::FileIOStream;
using namespace San::Device;
using namespace San::MachineLearning;

static const SString strLine = _SSTR("------------------------------\r\n\r\n");

static const sfloat ErrorBoundary = 0.05;
SString ByteANN(cSanTerminalDevice* pTerminal)
{
	pTerminal->iOutputString(_SSTR("ARTIFICIAL NEURAL NETWORK ALGORITHM:\n\n"), STC_GREY);

	SString strOutput;

#pragma region Generate training set
	/*Generate training set*/
	ANNTRAININGSET TrainingSet(8);
	for (uint32 seek = 0; seek < 8; seek = seek + 1)
	{
		TrainingSet[seek].first.resize(8);
		TrainingSet[seek].second.resize(8);
		for (uint32 seek_bit = 0; seek_bit < 8; seek_bit = seek_bit + 1)
		{
			TrainingSet[seek].first[seek_bit] = 0.0;
			TrainingSet[seek].second[seek_bit] = 0.0;
		}
		TrainingSet[seek].first[seek] = 1.0;
		TrainingSet[seek].second[seek] = 1.0;
	}
#pragma endregion

	cArtificialNeuralNetworkAlgorithm ANNNetwork(0.3, 0.1, -0.1, 0.1);

	SANSTREAM UserStream;

	ANNNetwork.iCreateFeatureNode(_SSTR("0"), UserStream, nullptr);
	ANNNetwork.iCreateFeatureNode(_SSTR("1"), UserStream, nullptr);
	ANNNetwork.iCreateFeatureNode(_SSTR("2"), UserStream, nullptr);
	ANNNetwork.iCreateFeatureNode(_SSTR("3"), UserStream, nullptr);
	ANNNetwork.iCreateFeatureNode(_SSTR("4"), UserStream, nullptr);
	ANNNetwork.iCreateFeatureNode(_SSTR("5"), UserStream, nullptr);
	ANNNetwork.iCreateFeatureNode(_SSTR("6"), UserStream, nullptr);
	ANNNetwork.iCreateFeatureNode(_SSTR("7"), UserStream, nullptr);

	ANNNetwork.iCreateLayer(8);
	ANNNetwork.iCreateLayer(8);

#pragma region Debug output
	strOutput = strOutput + _SSTR("INITIALIZED NETWORK:\r\n") + strLine + ANNNetwork.iPrintNeuralNetwork();
	pTerminal->iOutputString(ANNNetwork.iPrintNeuralNetwork());

	::system("pause");

	pTerminal->iClearScreen();
	pTerminal->iOutputString(_SSTR("ARTIFICIAL NEURAL NETWORK ALGORITHM : \n\nTraining...\n\n"));
#pragma endregion

	ANNNetwork.iTraining(TrainingSet, 100000);
	strOutput = strOutput + _SSTR("\r\nTRAINNED NETWORK:\r\n") + strLine + ANNNetwork.iPrintNeuralNetwork();

	for (uint32 seek = 0; seek < 8; seek = seek + 1)
	{
		ANNNetwork.iPredict(TrainingSet[seek].first);
		pTerminal->iClearScreen();
		pTerminal->iOutputString(_SSTR("ARTIFICIAL NEURAL NETWORK ALGORITHM:\n\n") + ANNNetwork.iPrintNeuralNetwork());

		::system("pause");
	}

	/*::gloWriteFile(strOutputFilePath, strOutput, true);
	::system(SStringA(SStringA("notepad ") + ::gloTToA(strOutputFilePath)).c_str());
	::system("pause");*/

	return strOutput;
}
SString TennisANN(cSanTerminalDevice* pTerminal)
{
	const SString strTennisAttrPath = _SSTR("DataSet\\tennis-attr.txt");
	const SString strTennisTrainPath = _SSTR("DataSet\\tennis-train.txt");
	const SString strTennisTestPath = _SSTR("DataSet\\tennis-test.txt");

	::cout << "ARTIFICIAL NEURAL NETWORK ALGORITHM:\n\n";

	SString strOutput;

	cArtificialNeuralNetworkAlgorithm ANNNetwork(0.6, 0.15, -0.1, 0.1);

	vector<vector<SString>> AttributeTable;
	vector<SString> ResultTable;

	SStringA strData;

#pragma region Load attributes and result table
	/*Load attributes and result table*/
	if (!::gloLoadFile(strTennisAttrPath, strData))
	{
		return SString(_SSTR("Error: Load file failed\r\n"));
	}
	vector<SString> strItems = ::gloGetStringItems(::gloAToT(strData), _SSTR("\r\n"));
	AttributeTable.resize(strItems.size() - 1);
	for (uint32 seek = 0; seek < (strItems.size() - 1); seek = seek + 1)
	{
		AttributeTable[seek] = ::gloGetStringItems(strItems[seek], _SSTR(" "));
	}
	ResultTable = ::gloGetStringItems(strItems[strItems.size() - 1], _SSTR(""));
#pragma endregion

	/*Generate ANN network structure*/
	SANSTREAM UserStream;

	for (uint32 seek = 0; seek < AttributeTable.size(); seek = seek + 1)
	{
		ANNNetwork.iCreateFeatureNode(AttributeTable[seek][0], UserStream, nullptr);
	}
	ANNNetwork.iCreateLayer(3);
	ANNNetwork.iCreateLayer(ResultTable.size() - 1);

#pragma region Debug output
	strOutput = strOutput + _SSTR("INITIALIZED NETWORK:\r\n") + strLine + ANNNetwork.iPrintNeuralNetwork();
	::wcout << ANNNetwork.iPrintNeuralNetwork().c_str();
	::system("pause");
	::system("cls");
	::wcout << L"ARTIFICIAL NEURAL NETWORK ALGORITHM:\n\nTraining...\n\n";
#pragma endregion

#pragma region Load training instance space
	/*Load training instance space*/
	if (!::gloLoadFile(strTennisTrainPath, strData))
	{
		return SString(_SSTR("Error: Load file failed\r\n"));
	}

	ANNTRAININGSET TrainingSet;
	strItems = ::gloGetStringItems(::gloAToT(strData), _SSTR("\r\n"));
	TrainingSet.resize(strItems.size());
	for (uint32 seek = 0; seek < strItems.size(); seek = seek + 1)
	{
		TrainingSet[seek].first.resize(AttributeTable.size());
		TrainingSet[seek].second.resize(ResultTable.size() - 1);
		vector<SString> strSubItems = ::gloGetStringItems(strItems[seek], _SSTR(" "));
		for (uint32 seek_attrib = 0; seek_attrib < (strSubItems.size() - 1); seek_attrib = seek_attrib + 1)
		{
			TrainingSet[seek].first[seek_attrib] = 0.0;
			for (uint32 seek_val = 1; seek_val < AttributeTable[seek_attrib].size(); seek_val = seek_val + 1)
			{
				if (strSubItems[seek_attrib] == AttributeTable[seek_attrib][seek_val])
				{
					TrainingSet[seek].first[seek_attrib] = seek_val;
				}
			}
		}
		for (uint32 seek_val = 0; seek_val < (ResultTable.size() - 1); seek_val = seek_val + 1)
		{
			TrainingSet[seek].second[seek_val] = 0.0;
		}
		for (uint32 seek_val = 1; seek_val < ResultTable.size(); seek_val = seek_val + 1)
		{
			if (strSubItems.back() == ResultTable[seek_val])
			{
				TrainingSet[seek].second[seek_val - 1] = 1.0;
			}
		}
	}
#pragma endregion

#pragma region Load test instance space
	/*Load test instance space*/
	if (!::gloLoadFile(strTennisTestPath, strData))
	{
		return SString(_SSTR("Error: Load file failed\r\n"));
	}

	ANNTRAININGSET TestSet;
	strItems = ::gloGetStringItems(::gloAToT(strData), _SSTR("\r\n"));
	TestSet.resize(strItems.size());
	for (uint32 seek = 0; seek < strItems.size(); seek = seek + 1)
	{
		TestSet[seek].first.resize(AttributeTable.size());
		TestSet[seek].second.resize(ResultTable.size() - 1);
		vector<SString> strSubItems = ::gloGetStringItems(strItems[seek], _SSTR(" "));
		for (uint32 seek_attrib = 0; seek_attrib < (strSubItems.size() - 1); seek_attrib = seek_attrib + 1)
		{
			TestSet[seek].first[seek_attrib] = 0.0;
			for (uint32 seek_val = 1; seek_val < AttributeTable[seek_attrib].size(); seek_val = seek_val + 1)
			{
				if (strSubItems[seek_attrib] == AttributeTable[seek_attrib][seek_val])
				{
					TestSet[seek].first[seek_attrib] = seek_val;
				}
			}
		}
		for (uint32 seek_val = 0; seek_val < (ResultTable.size() - 1); seek_val = seek_val + 1)
		{
			TestSet[seek].second[seek_val] = 0.0;
		}
		for (uint32 seek_val = 1; seek_val < ResultTable.size(); seek_val = seek_val + 1)
		{
			if (strSubItems.back() == ResultTable[seek_val])
			{
				TestSet[seek].second[seek_val - 1] = 1.0;
			}
		}
	}
#pragma endregion

	ANNNetwork.iTraining(TrainingSet, 20000);
	strOutput = strOutput + _SSTR("\r\nTRAINNED NETWORK:\r\n") + strLine + ANNNetwork.iPrintNeuralNetwork();

#pragma region Evaluate accuracy

	sfloat Accuracy = 0.0;

	/*Training Set Accuracy*/
	Accuracy = CalcSetAccuracy(ANNNetwork, TrainingSet);
	::cout << "Training Set Accuracy: " << Accuracy << "\n";
	strOutput = strOutput + _SSTR("\r\nTRAINING SET ACCURACY:\t") + ::gloFToS(Accuracy) + _SSTR("\r\n");

	/*Test Set Accuracy*/
	Accuracy = CalcSetAccuracy(ANNNetwork, TestSet);
	::cout << "Test Set Accuracy: " << Accuracy << "\n";
	strOutput = strOutput + _SSTR("\r\nTEST SET ACCURACY:\t") + ::gloFToS(Accuracy) + _SSTR("\r\n");
#pragma endregion

	/*::gloWriteFile(strOutputFilePath, strOutput, true);
	::system(SStringA(SStringA("notepad ") + ::gloTToA(strOutputFilePath)).c_str());
	::system("pause");*/

	return strOutput;
}
SString TennisSANN(cSanTerminalDevice* pTerminal)
{
	const SString strTennisAttrPath = _SSTR("DataSet\\tennis-attr.txt");
	const SString strTennisTrainPath = _SSTR("DataSet\\tennis-train.txt");
	const SString strTennisTestPath = _SSTR("DataSet\\tennis-test.txt");

	::cout << "ARTIFICIAL NEURAL NETWORK ALGORITHM:\n\n";

	SString strOutput;

	cArtificialNeuralNetworkAlgorithm ANNNetwork(0.6, 0.15, -0.1, 0.1);

	vector<vector<SString>> AttributeTable;
	vector<SString> ResultTable;
	uint32 FeatureArraySize = 0;

	SStringA strData;

#pragma region Load attributes and result table
	/*Load attributes and result table*/
	if (!::gloLoadFile(strTennisAttrPath, strData))
	{
		return SString(_SSTR("Error: Load file failed\r\n"));
	}
	vector<SString> strItems = ::gloGetStringItems(::gloAToT(strData), _SSTR("\r\n"));
	AttributeTable.resize(strItems.size() - 1);
	for (uint32 seek = 0; seek < (strItems.size() - 1); seek = seek + 1)
	{
		AttributeTable[seek] = ::gloGetStringItems(strItems[seek], _SSTR(" "));
		FeatureArraySize = FeatureArraySize + AttributeTable[seek].size() - 1;
	}
	ResultTable = ::gloGetStringItems(strItems[strItems.size() - 1], _SSTR(""));
#pragma endregion

	/*Generate ANN network structure*/
	SANSTREAM UserStream;

	for (uint32 seek = 0; seek < AttributeTable.size(); seek = seek + 1)
	{
		for (uint32 seek_val = 1; seek_val < AttributeTable[seek].size(); seek_val = seek_val + 1)
		{
			ANNNetwork.iCreateFeatureNode(AttributeTable[seek][0] + _SSTR("_") + AttributeTable[seek][seek_val], UserStream, nullptr);
		}
	}

	ANNNetwork.iCreateLayer(3);
	ANNNetwork.iCreateLayer(ResultTable.size() - 1);

#pragma region Debug output
	strOutput = strOutput + _SSTR("INITIALIZED NETWORK:\r\n") + strLine + ANNNetwork.iPrintNeuralNetwork();
	::wcout << ANNNetwork.iPrintNeuralNetwork().c_str();
	::system("pause");
	::system("cls");
	::wcout << L"ARTIFICIAL NEURAL NETWORK ALGORITHM:\n\nTraining...\n\n";
#pragma endregion

#pragma region Load training instance space
	/*Load training instance space*/
	if (!::gloLoadFile(strTennisTrainPath, strData))
	{
		return SString(_SSTR("Error: Load file failed\r\n"));
	}

	ANNTRAININGSET TrainingSet;
	strItems = ::gloGetStringItems(::gloAToT(strData), _SSTR("\r\n"));
	TrainingSet.resize(strItems.size());
	for (uint32 seek = 0; seek < strItems.size(); seek = seek + 1)
	{
		TrainingSet[seek].first.resize(FeatureArraySize);
		TrainingSet[seek].second.resize(ResultTable.size() - 1);
		for (uint32 seek_feature = 0; seek_feature < FeatureArraySize; seek_feature = seek_feature + 1)
		{
			TrainingSet[seek].first[seek_feature] = 0.0;
		}
		vector<SString> strSubItems = ::gloGetStringItems(strItems[seek], _SSTR(" "));
		uint32 AttribOffset = 0;
		for (uint32 seek_attrib = 0; seek_attrib < (strSubItems.size() - 1); seek_attrib = seek_attrib + 1)
		{
			for (uint32 seek_val = 1; seek_val < AttributeTable[seek_attrib].size(); seek_val = seek_val + 1)
			{
				if (strSubItems[seek_attrib] == AttributeTable[seek_attrib][seek_val])
				{
					TrainingSet[seek].first[AttribOffset + seek_val - 1] = 1.0;
				}
			}
			AttribOffset = AttribOffset + AttributeTable[seek_attrib].size() - 1;
		}
		for (uint32 seek_val = 0; seek_val < (ResultTable.size() - 1); seek_val = seek_val + 1)
		{
			TrainingSet[seek].second[seek_val] = 0.0;
		}
		for (uint32 seek_val = 1; seek_val < ResultTable.size(); seek_val = seek_val + 1)
		{
			if (strSubItems.back() == ResultTable[seek_val])
			{
				TrainingSet[seek].second[seek_val - 1] = 1.0;
			}
		}
	}
#pragma endregion

#pragma region Load test instance space
	/*Load test instance space*/
	if (!::gloLoadFile(strTennisTestPath, strData))
	{
		return SString(_SSTR("Error: Load file failed\r\n"));
	}

	ANNTRAININGSET TestSet;
	strItems = ::gloGetStringItems(::gloAToT(strData), _SSTR("\r\n"));
	TestSet.resize(strItems.size());
	for (uint32 seek = 0; seek < strItems.size(); seek = seek + 1)
	{
		TestSet[seek].first.resize(FeatureArraySize);
		TestSet[seek].second.resize(ResultTable.size() - 1);
		for (uint32 seek_feature = 0; seek_feature < FeatureArraySize; seek_feature = seek_feature + 1)
		{
			TestSet[seek].first[seek_feature] = 0.0;
		}
		vector<SString> strSubItems = ::gloGetStringItems(strItems[seek], _SSTR(" "));
		uint32 AttribOffset = 0;
		for (uint32 seek_attrib = 0; seek_attrib < (strSubItems.size() - 1); seek_attrib = seek_attrib + 1)
		{
			for (uint32 seek_val = 1; seek_val < AttributeTable[seek_attrib].size(); seek_val = seek_val + 1)
			{
				if (strSubItems[seek_attrib] == AttributeTable[seek_attrib][seek_val])
				{
					TestSet[seek].first[AttribOffset + seek_val - 1] = 1.0;
				}
			}
			AttribOffset = AttribOffset + AttributeTable[seek_attrib].size() - 1;
		}
		for (uint32 seek_val = 0; seek_val < (ResultTable.size() - 1); seek_val = seek_val + 1)
		{
			TestSet[seek].second[seek_val] = 0.0;
		}
		for (uint32 seek_val = 1; seek_val < ResultTable.size(); seek_val = seek_val + 1)
		{
			if (strSubItems.back() == ResultTable[seek_val])
			{
				TestSet[seek].second[seek_val - 1] = 1.0;
			}
		}
	}
#pragma endregion

	ANNNetwork.iTraining(TrainingSet, 20000);
	strOutput = strOutput + _SSTR("\r\nTRAINNED NETWORK:\r\n") + strLine + ANNNetwork.iPrintNeuralNetwork();

#pragma region Evaluate accuracy

	sfloat Accuracy = 0.0;

	/*Training Set Accuracy*/
	Accuracy = CalcSetAccuracy(ANNNetwork, TrainingSet);
	::cout << "Training Set Accuracy: " << Accuracy << "\n";
	strOutput = strOutput + _SSTR("\r\nTRAINING SET ACCURACY:\t") + ::gloFToS(Accuracy) + _SSTR("\r\n");

	/*Test Set Accuracy*/
	Accuracy = CalcSetAccuracy(ANNNetwork, TestSet);
	::cout << "Test Set Accuracy: " << Accuracy << "\n";
	strOutput = strOutput + _SSTR("\r\nTEST SET ACCURACY:\t") + ::gloFToS(Accuracy) + _SSTR("\r\n");
#pragma endregion

	/*::gloWriteFile(strOutputFilePath, strOutput, true);
	::system(SStringA(SStringA("notepad ") + ::gloTToA(strOutputFilePath)).c_str());
	::system("pause");*/

	return strOutput;
}
SString IrisANN(cSanTerminalDevice* pTerminal)
{
	const SString strIrisAttrPath = _SSTR("DataSet\\iris-attr.txt");
	const SString strIrisTrainPath = _SSTR("DataSet\\iris-train.txt");
	const SString strIrisTestPath = _SSTR("DataSet\\iris-test.txt");

	::cout << "ARTIFICIAL NEURAL NETWORK ALGORITHM:\n\n";

	SString strOutput;

	cArtificialNeuralNetworkAlgorithm ANNNetwork(0.9, 0.3, -0.1, 0.1);

	vector<vector<SString>> AttributeTable;
	vector<SString> ResultTable;

	SStringA strData;

#pragma region Load attributes and result table
	/*Load attributes and result table*/
	if (!::gloLoadFile(strIrisAttrPath, strData))
	{
		return SString(_SSTR("Error: Load file failed\r\n"));
	}
	vector<SString> strItems = ::gloGetStringItems(::gloAToT(strData), _SSTR("\r\n"));
	AttributeTable.resize(strItems.size() - 1);
	for (uint32 seek = 0; seek < (strItems.size() - 1); seek = seek + 1)
	{
		AttributeTable[seek] = ::gloGetStringItems(strItems[seek], _SSTR(" "));
	}
	ResultTable = ::gloGetStringItems(strItems[strItems.size() - 1], _SSTR(""));
#pragma endregion

	/*Generate ANN network structure*/
	SANSTREAM UserStream;

	for (uint32 seek = 0; seek < AttributeTable.size(); seek = seek + 1)
	{
		ANNNetwork.iCreateFeatureNode(AttributeTable[seek][0], UserStream, nullptr);
	}

	ANNNetwork.iCreateLayer(5);
	ANNNetwork.iCreateLayer(ResultTable.size() - 1);

#pragma region Debug output
	strOutput = strOutput + _SSTR("INITIALIZED NETWORK:\r\n") + strLine + ANNNetwork.iPrintNeuralNetwork();
	::wcout << ANNNetwork.iPrintNeuralNetwork().c_str();
	::system("pause");
	::system("cls");
	::wcout << L"ARTIFICIAL NEURAL NETWORK ALGORITHM:\n\nTraining...\n\n";
#pragma endregion

#pragma region Load training instance space
	/*Load training instance space*/
	if (!::gloLoadFile(strIrisTrainPath, strData))
	{
		return SString(_SSTR("Error: Load file failed\r\n"));
	}

	ANNTRAININGSET TrainingSet;
	strItems = ::gloGetStringItems(::gloAToT(strData), _SSTR("\r\n"));
	TrainingSet.resize(strItems.size());
	for (uint32 seek = 0; seek < strItems.size(); seek = seek + 1)
	{
		TrainingSet[seek].first.resize(AttributeTable.size());
		TrainingSet[seek].second.resize(ResultTable.size() - 1);
		vector<SString> strSubItems = ::gloGetStringItems(strItems[seek], _SSTR(" "));
		for (uint32 seek_attrib = 0; seek_attrib < (strSubItems.size() - 1); seek_attrib = seek_attrib + 1)
		{
			TrainingSet[seek].first[seek_attrib] = ::gloSToF(strSubItems[seek_attrib]);
		}
		for (uint32 seek_val = 0; seek_val < (ResultTable.size() - 1); seek_val = seek_val + 1)
		{
			TrainingSet[seek].second[seek_val] = 0.0;
		}
		for (uint32 seek_val = 1; seek_val < ResultTable.size(); seek_val = seek_val + 1)
		{
			if (strSubItems.back() == ResultTable[seek_val])
			{
				TrainingSet[seek].second[seek_val - 1] = 1.0;
			}
		}
	}
#pragma endregion

#pragma region Load test instance space
	/*Load test instance space*/
	if (!::gloLoadFile(strIrisTestPath, strData))
	{
		return SString(_SSTR("Error: Load file failed\r\n"));
	}

	ANNTRAININGSET TestSet;
	strItems = ::gloGetStringItems(::gloAToT(strData), _SSTR("\r\n"));
	TestSet.resize(strItems.size());
	for (uint32 seek = 0; seek < strItems.size(); seek = seek + 1)
	{
		TestSet[seek].first.resize(AttributeTable.size());
		TestSet[seek].second.resize(ResultTable.size() - 1);
		vector<SString> strSubItems = ::gloGetStringItems(strItems[seek], _SSTR(" "));
		for (uint32 seek_attrib = 0; seek_attrib < (strSubItems.size() - 1); seek_attrib = seek_attrib + 1)
		{
			TestSet[seek].first[seek_attrib] = ::gloSToF(strSubItems[seek_attrib]);
		}
		for (uint32 seek_val = 0; seek_val < (ResultTable.size() - 1); seek_val = seek_val + 1)
		{
			TestSet[seek].second[seek_val] = 0.0;
		}
		for (uint32 seek_val = 1; seek_val < ResultTable.size(); seek_val = seek_val + 1)
		{
			if (strSubItems.back() == ResultTable[seek_val])
			{
				TestSet[seek].second[seek_val - 1] = 1.0;
			}
		}
	}
#pragma endregion

	ANNNetwork.iTraining(TrainingSet, 20000);
	strOutput = strOutput + _SSTR("\r\nTRAINNED NETWORK:\r\n") + strLine + ANNNetwork.iPrintNeuralNetwork();

#pragma region Evaluate accuracy

	sfloat Accuracy = 0.0;

	/*Training Set Accuracy*/
	Accuracy = CalcSetAccuracy(ANNNetwork, TrainingSet);
	::cout << "Training Set Accuracy: " << Accuracy << "\n\n";
	strOutput = strOutput + _SSTR("\r\nTRAINING SET ACCURACY:\t") + ::gloFToS(Accuracy) + _SSTR("\r\n");

	/*Test Set Accuracy*/
	Accuracy = CalcSetAccuracy(ANNNetwork, TestSet);
	::cout << "Test Set Accuracy: " << Accuracy << "\n";
	strOutput = strOutput + _SSTR("\r\nTEST SET ACCURACY:\t") + ::gloFToS(Accuracy) + _SSTR("\r\n");
#pragma endregion

	/*::gloWriteFile(strOutputFilePath, strOutput, true);
	::system(SStringA(SStringA("notepad ") + ::gloTToA(strOutputFilePath)).c_str());
	::system("pause");*/

	return strOutput;
}
SString IrisPANN(cSanTerminalDevice* pTerminal)
{
	const SString strIrisAttrPath = _SSTR("DataSet\\iris-attr.txt");
	const SString strIrisTrainPath = _SSTR("DataSet\\iris-train.txt");
	const SString strIrisTestPath = _SSTR("DataSet\\iris-test.txt");

	::cout << "ARTIFICIAL NEURAL NETWORK ALGORITHM:\n\n";

	SString strOutput;

	const sfloat ValidationSetRate = 1.0 / 3.0;

	vector<vector<SString>> AttributeTable;
	vector<SString> ResultTable;

	ANNTRAININGSET* pTrainingSet = new ANNTRAININGSET;
	ANNTRAININGSET* pTestSet = new ANNTRAININGSET;

	SStringA strData;

#pragma region Load attributes and result table
	/*Load attributes and result table*/
	if (!::gloLoadFile(strIrisAttrPath, strData))
	{
		return SString(_SSTR("Error: Load file failed\r\n"));
	}
	vector<SString> strItems = ::gloGetStringItems(::gloAToT(strData), _SSTR("\r\n"));
	AttributeTable.resize(strItems.size() - 1);
	for (uint32 seek = 0; seek < (strItems.size() - 1); seek = seek + 1)
	{
		AttributeTable[seek] = ::gloGetStringItems(strItems[seek], _SSTR(" "));
	}
	ResultTable = ::gloGetStringItems(strItems[strItems.size() - 1], _SSTR(""));
#pragma endregion

#pragma region Load training instance space
	/*Load training instance space*/
	if (!::gloLoadFile(strIrisTrainPath, strData))
	{
		return SString(_SSTR("Error: Load file failed\r\n"));
	}

	strItems = ::gloGetStringItems(::gloAToT(strData), _SSTR("\r\n"));
	pTrainingSet->resize(strItems.size());
	for (uint32 seek = 0; seek < strItems.size(); seek = seek + 1)
	{
		(*pTrainingSet)[seek].first.resize(AttributeTable.size());
		(*pTrainingSet)[seek].second.resize(ResultTable.size() - 1);
		vector<SString> strSubItems = ::gloGetStringItems(strItems[seek], _SSTR(" "));
		for (uint32 seek_attrib = 0; seek_attrib < (strSubItems.size() - 1); seek_attrib = seek_attrib + 1)
		{
			(*pTrainingSet)[seek].first[seek_attrib] = ::gloSToF(strSubItems[seek_attrib]);
		}
		for (uint32 seek_val = 0; seek_val < (ResultTable.size() - 1); seek_val = seek_val + 1)
		{
			(*pTrainingSet)[seek].second[seek_val] = 0.0;
		}
		for (uint32 seek_val = 1; seek_val < ResultTable.size(); seek_val = seek_val + 1)
		{
			if (strSubItems.back() == ResultTable[seek_val])
			{
				(*pTrainingSet)[seek].second[seek_val - 1] = 1.0;
			}
		}
	}
#pragma endregion

#pragma region Load test instance space
	/*Load test instance space*/
	if (!::gloLoadFile(strIrisTestPath, strData))
	{
		return SString(_SSTR("Error: Load file failed\r\n"));
	}

	strItems = ::gloGetStringItems(::gloAToT(strData), _SSTR("\r\n"));
	pTestSet->resize(strItems.size());
	for (uint32 seek = 0; seek < strItems.size(); seek = seek + 1)
	{
		(*pTestSet)[seek].first.resize(AttributeTable.size());
		(*pTestSet)[seek].second.resize(ResultTable.size() - 1);
		vector<SString> strSubItems = ::gloGetStringItems(strItems[seek], _SSTR(" "));
		for (uint32 seek_attrib = 0; seek_attrib < (strSubItems.size() - 1); seek_attrib = seek_attrib + 1)
		{
			(*pTestSet)[seek].first[seek_attrib] = ::gloSToF(strSubItems[seek_attrib]);
		}
		for (uint32 seek_val = 0; seek_val < (ResultTable.size() - 1); seek_val = seek_val + 1)
		{
			(*pTestSet)[seek].second[seek_val] = 0.0;
		}
		for (uint32 seek_val = 1; seek_val < ResultTable.size(); seek_val = seek_val + 1)
		{
			if (strSubItems.back() == ResultTable[seek_val])
			{
				(*pTestSet)[seek].second[seek_val - 1] = 1.0;
			}
		}
	}
#pragma endregion

	const uint32 NoiseMinLevel = 0;
	const uint32 NoiseMaxLevel = 30;
	const uint32 NoiseIncreaseStep = 2;
	const uint32 IterationTimes = 5;

	uint32 NoiseTimes = (NoiseMaxLevel - NoiseMinLevel) / NoiseIncreaseStep;

	vector<sfloat>* pAccuracyArray = new vector<sfloat>[6];
	pAccuracyArray[0].resize(NoiseTimes);
	pAccuracyArray[1].resize(NoiseTimes);
	pAccuracyArray[2].resize(NoiseTimes);
	pAccuracyArray[3].resize(NoiseTimes);
	pAccuracyArray[4].resize(NoiseTimes);
	pAccuracyArray[5].resize(NoiseTimes);
	for (uint32 seek = 0; seek < 6; seek = seek + 1)
	{
		for (uint32 seek_item = 0; seek_item < NoiseTimes; seek_item = seek_item + 1)
		{
			pAccuracyArray[seek][seek_item] = 0.0;
		}
	}

	for (uint32 seek_iteration = 0; seek_iteration < IterationTimes; seek_iteration = seek_iteration + 1)
	{
		//#pragma omp parallel for
		for (int32 seek_noise = NoiseMinLevel; seek_noise <= NoiseMaxLevel; seek_noise = seek_noise + NoiseIncreaseStep)
		{
			::cout << "\nNoise Rate: " << seek_noise << "\n";
			ANNTRAININGSET NoiseTrainingSet;
			ANNTRAININGSET NoiseValidationSet;

#pragma region Add noise and create noise training set, noise validation set
			/*Add noise and seprate the instance to training set or validation set*/
			sfloat RandomNum = 0.0;
			uint32 InstanceOutputIndex = 0;
			uint32 ResultArraySize = (*pTrainingSet)[0].second.size();
			uint32 Count = 0;
			for (uint32 seek = 0; seek < pTrainingSet->size(); seek = seek + 1)
			{
				pair<vector<sfloat>, vector<sfloat>> Instance = (*pTrainingSet)[seek];
				RandomNum = ::rand() % 1000;
				if (RandomNum < (seek_noise * 10))
				{
					//PrintInstance(Instance);
					for (uint32 seek_res = 0; seek_res < ResultArraySize; seek_res = seek_res + 1)
					{
						if (::gloIsFloatEqual(Instance.second[seek_res], 1.0))
						{
							InstanceOutputIndex = seek_res;
						}
						Instance.second[seek_res] = 0.0;
					}
					RandomNum = ::rand() % ResultArraySize;
					while (::gloIsFloatEqual(RandomNum, InstanceOutputIndex))
					{
						RandomNum = ::rand() % ResultArraySize;
					}
					Instance.second[(uint32) RandomNum] = 1.0;
					//PrintInstance(Instance);
					//::cout << "\n";
					//Count = Count + 1;
				}
				RandomNum = ::rand() % 1000;
				RandomNum = RandomNum / 1000.0;
				if (RandomNum < ValidationSetRate)
				{
					NoiseValidationSet.push_back(Instance);
				}
				else
				{
					NoiseTrainingSet.push_back(Instance);
				}
			}
			::cout << "Training Set Size:\t" << NoiseTrainingSet.size() << "\n";
			::cout << "Validation Set Size:\t" << NoiseValidationSet.size() << "\n";
#pragma endregion

			cArtificialNeuralNetworkAlgorithm ANNNetwork(0.9, 0.3, -0.1, 0.1);

			/*Generate ANN network structure*/
			SANSTREAM UserStream;

			for (uint32 seek = 0; seek < AttributeTable.size(); seek = seek + 1)
			{
				ANNNetwork.iCreateFeatureNode(AttributeTable[seek][0], UserStream, nullptr);
			}

			ANNNetwork.iCreateLayer(5);
			ANNNetwork.iCreateLayer(ResultTable.size() - 1);

#pragma region Debug output
			/*strOutput = strOutput + _SSTR("INITIALIZED NETWORK:\r\n") + strLine + ANNNetwork.iPrintNeuralNetwork();
			::wcout << ANNNetwork.iPrintNeuralNetwork().c_str();
			::system("pause");
			::system("cls");
			::wcout << L"ARTIFICIAL NEURAL NETWORK ALGORITHM:\n\nTraining...\n\n";*/
#pragma endregion

			ANNNetwork.iTraining(NoiseTrainingSet, 20000);
			//strOutput = strOutput + _SSTR("\r\nTRAINNED NETWORK:\r\n") + strLine + ANNNetwork.iPrintNeuralNetwork();

			sfloat Accuracy = 0.0;

#pragma region Evaluate accuracy before pruning
			uint32 NoiseLevelIndex = seek_noise / NoiseIncreaseStep;

			/*Training Set Accuracy Before Pruning*/
			Accuracy = CalcSetAccuracy(ANNNetwork, NoiseTrainingSet);
			::cout << "Training Set\tAcc - BP: " << Accuracy << "\n";
			pAccuracyArray[0][NoiseLevelIndex] = pAccuracyArray[0][NoiseLevelIndex] + Accuracy;

			/*Validation Set Accuracy Before Pruning*/
			Accuracy = CalcSetAccuracy(ANNNetwork, NoiseValidationSet);
			::cout << "Validation Set\tAcc - BP: " << Accuracy << "\n";
			pAccuracyArray[1][NoiseLevelIndex] = pAccuracyArray[1][NoiseLevelIndex] + Accuracy;

			/*Test Set Accuracy Before Pruning*/
			Accuracy = CalcSetAccuracy(ANNNetwork, *pTestSet);
			::cout << "Test Set\tAcc - BP: " << Accuracy << "\n";
			pAccuracyArray[2][NoiseLevelIndex] = pAccuracyArray[2][NoiseLevelIndex] + Accuracy;
#pragma endregion

			//ANNNetwork.iTrainingWithValidation(NoiseTrainingSet, NoiseTrainingSet, 50000);
			ANNNetwork.iPruning(NoiseValidationSet);

#pragma region Evaluate accuracy after pruning
			/*Training Set Accuracy Before Pruning*/
			Accuracy = CalcSetAccuracy(ANNNetwork, NoiseTrainingSet);
			::cout << "Training Set\tAcc - AP: " << Accuracy << "\n";
			pAccuracyArray[3][NoiseLevelIndex] = pAccuracyArray[3][NoiseLevelIndex] + Accuracy;

			/*Validation Set Accuracy Before Pruning*/
			Accuracy = CalcSetAccuracy(ANNNetwork, NoiseValidationSet);
			::cout << "Validation Set\tAcc - AP: " << Accuracy << "\n";
			pAccuracyArray[4][NoiseLevelIndex] = pAccuracyArray[4][NoiseLevelIndex] + Accuracy;

			/*Test Set Accuracy Before Pruning*/
			Accuracy = CalcSetAccuracy(ANNNetwork, *pTestSet);
			::cout << "Test Set\tAcc - AP: " << Accuracy << "\n";
			pAccuracyArray[5][NoiseLevelIndex] = pAccuracyArray[5][NoiseLevelIndex] + Accuracy;
#pragma endregion
			//NoiseLevelIndex = NoiseLevelIndex + 1;
		}
	}

#pragma region Accuracy output
	SString strAccuracyType[6];
	strAccuracyType[0] = _SSTR("\r\nBefore Pruning\r\nTraining   Set: ");
	strAccuracyType[1] = _SSTR("Validation Set: ");
	strAccuracyType[2] = _SSTR("Test       Set: ");
	strAccuracyType[3] = _SSTR("\r\nAfter Pruning\r\nTraining   Set: ");
	strAccuracyType[4] = _SSTR("Validation Set: ");
	strAccuracyType[5] = _SSTR("Test       Set: ");
	strOutput = strOutput + _SSTR("\r\n\r\nNoise Level:    ");
	for (int32 seek = NoiseMinLevel; seek <= NoiseMaxLevel; seek = seek + NoiseIncreaseStep)
	{
		strOutput = strOutput + ::gloFToS((sfloat) seek / 100.0) + _SSTR(" ");
	}
	for (uint32 seek = 0; seek < 6; seek = seek + 1)
	{
		strOutput = strOutput + _SSTR("\r\n") + strAccuracyType[seek];
		for (int32 seek_noise = NoiseMinLevel; seek_noise <= NoiseTimes; seek_noise = seek_noise + 1)
		{
			strOutput = strOutput + ::gloFToS(pAccuracyArray[seek][seek_noise] * (1.0 / (sfloat) IterationTimes)) + _SSTR(" ");
		}
	}
#pragma endregion

	/*::gloWriteFile(strOutputFilePath, strOutput, true);
	::system(SStringA(SStringA("notepad ") + ::gloTToA(strOutputFilePath)).c_str());*/

	delete pTrainingSet;
	pTrainingSet = nullptr;
	delete pTestSet;
	pTestSet = nullptr;
	delete[] pAccuracyArray;
	pAccuracyArray = nullptr;

	/*::system("pause");*/

	return strOutput;
}
SString GeneralANN(cSanTerminalDevice* pTerminal)
{
	pTerminal->iOutputString(_SSTR("ARTIFICIAL NEURAL NETWORK ALGORITHM:\r\n\r\n"));

	SString strAttrPath = _SSTR("-attr.txt");
	SString strTrainPath = _SSTR("-train.txt");
	SString strTestPath = _SSTR("-test.txt");

	SString strOutput;

	sfloat ValidationSetRate = 1.0 / 3.0;

	vector<vector<SString>> AttributeTable;
	vector<SString> ResultTable;

	ANNTRAININGSET* pTrainingSet = new ANNTRAININGSET;
	ANNTRAININGSET* pTestSet = new ANNTRAININGSET;

	SStringA strData;

	char Buffer[1024];

	::cin.getline(Buffer, 1024);

#pragma region Get user input, data set file and validation set rate
	//Get user input, data set file and validation set rate
	while (true)
	{
		pTerminal->iOutputString(_SSTR("Please enter data set file name: "), STC_GREY);
		::cin.getline(Buffer, 1024);

		SStringA strFileName = Buffer;

		if (strFileName.empty()){ pTerminal->iOutputString(_SSTR("Error: Invalid file name\r\n"), STC_WHITE, STC_RED); continue; }

		strAttrPath = strFileName + strAttrPath;
		strTrainPath = strFileName + strTrainPath;
		strTestPath = strFileName + strTestPath;

		break;
	}

	while (true)
	{
		pTerminal->iOutputString(_SSTR("Please enter validation dataset size (percentage, 0.0 - 1.0): "), STC_GREY);
		::cin.getline(Buffer, 1024);

		SStringA strCommand = Buffer;

		if (strCommand.empty()){ continue; }
		sfloat Rate = ::gloSToF(strCommand);

		if ((Rate<0.0) || (Rate>1.0)){ pTerminal->iOutputString(_SSTR("Error: Invalid value\r\n"), STC_WHITE, STC_RED); continue; }

		ValidationSetRate = Rate;

		break;
	}
#pragma endregion

#pragma region Load attributes and result table
	/*Load attributes and result table*/
	if (!::gloLoadFile(strAttrPath, strData)){ return SString(_SSTR("Error: Load file failed\r\n")); }

	vector<SString> strItems = ::gloGetStringItems(::gloAToT(strData), _SSTR("\r\n"));
	AttributeTable.resize(strItems.size() - 1);
	for (uint32 seek = 0; seek < (strItems.size() - 1); seek = seek + 1)
	{
		AttributeTable[seek] = ::gloGetStringItems(strItems[seek], _SSTR(" "));
	}
	ResultTable = ::gloGetStringItems(strItems[strItems.size() - 1], _SSTR(""));
#pragma endregion

#pragma region Load training instance space
	/*Load training instance space*/
	if (!::gloLoadFile(strTrainPath, strData)){ return SString(_SSTR("Error: Load file failed\r\n")); }

	strItems = ::gloGetStringItems(::gloAToT(strData), _SSTR("\r\n"));
	pTrainingSet->resize(strItems.size());
	for (uint32 seek = 0; seek < strItems.size(); seek = seek + 1)
	{
		(*pTrainingSet)[seek].first.resize(AttributeTable.size());
		(*pTrainingSet)[seek].second.resize(ResultTable.size() - 1);
		vector<SString> strSubItems = ::gloGetStringItems(strItems[seek], _SSTR(" "));
		for (uint32 seek_attrib = 0; seek_attrib < (strSubItems.size() - 1); seek_attrib = seek_attrib + 1)
		{
			(*pTrainingSet)[seek].first[seek_attrib] = ::gloSToF(strSubItems[seek_attrib]);
		}
		for (uint32 seek_val = 0; seek_val < (ResultTable.size() - 1); seek_val = seek_val + 1)
		{
			(*pTrainingSet)[seek].second[seek_val] = 0.0;
		}
		for (uint32 seek_val = 1; seek_val < ResultTable.size(); seek_val = seek_val + 1)
		{
			if (strSubItems.back() == ResultTable[seek_val])
			{
				(*pTrainingSet)[seek].second[seek_val - 1] = 1.0;
			}
		}
	}
#pragma endregion

#pragma region Load test instance space
	/*Load test instance space*/
	if (!::gloLoadFile(strTestPath, strData)){ return SString(_SSTR("Error: Load file failed\r\n")); }

	strItems = ::gloGetStringItems(::gloAToT(strData), _SSTR("\r\n"));
	pTestSet->resize(strItems.size());
	for (uint32 seek = 0; seek < strItems.size(); seek = seek + 1)
	{
		(*pTestSet)[seek].first.resize(AttributeTable.size());
		(*pTestSet)[seek].second.resize(ResultTable.size() - 1);
		vector<SString> strSubItems = ::gloGetStringItems(strItems[seek], _SSTR(" "));
		for (uint32 seek_attrib = 0; seek_attrib < (strSubItems.size() - 1); seek_attrib = seek_attrib + 1)
		{
			(*pTestSet)[seek].first[seek_attrib] = ::gloSToF(strSubItems[seek_attrib]);
		}
		for (uint32 seek_val = 0; seek_val < (ResultTable.size() - 1); seek_val = seek_val + 1)
		{
			(*pTestSet)[seek].second[seek_val] = 0.0;
		}
		for (uint32 seek_val = 1; seek_val < ResultTable.size(); seek_val = seek_val + 1)
		{
			if (strSubItems.back() == ResultTable[seek_val])
			{
				(*pTestSet)[seek].second[seek_val - 1] = 1.0;
			}
		}
	}
#pragma endregion

	sfloat LearningRate = 0.9;
	sfloat MomentumValue = 0.3;

#pragma region Get user input, learning rate and momentum value
	//Get user input, learning rate and momentum value
	while (true)
	{
		pTerminal->iOutputString(_SSTR("Please enter learning rate (greater than 0.0): "), STC_GREY);
		::cin.getline(Buffer, 1024);

		SStringA strCommand = Buffer;

		if (strCommand.empty()){ continue; }

		sfloat Rate = ::gloSToF(strCommand);

		if (Rate <= 0.0){ pTerminal->iOutputString(_SSTR("Error: Invalid value\r\n"), STC_WHITE, STC_RED); continue; }

		LearningRate = Rate;

		break;
	}

	while (true)
	{
		pTerminal->iOutputString(_SSTR("Please enter momentum value (greater than 0.0): "), STC_GREY);
		::cin.getline(Buffer, 1024);

		SStringA strCommand = Buffer;

		if (strCommand.empty()){ continue; }

		sfloat Rate = ::gloSToF(strCommand);

		if (Rate <= 0.0){ pTerminal->iOutputString(_SSTR("Error: Invalid value\r\n"), STC_WHITE, STC_RED); continue; }

		MomentumValue = Rate;

		break;
	}
#pragma endregion

#pragma region Generate artificial neural network
	/*Generate artificial neural network*/
	cArtificialNeuralNetworkAlgorithm ANN(LearningRate, MomentumValue, -0.1, 0.1);

	SANSTREAM UserStream;

	for (uint32 seek = 0; seek < AttributeTable.size(); seek = seek + 1)
	{
		ANN.iCreateFeatureNode(AttributeTable[seek][0], UserStream, nullptr);
	}

	while (true)
	{
		int32 HiddenLayerSize = 1;

		while (true)
		{
			pTerminal->iOutputString(_SSTR("Please enter artificial neural network layer size: "), STC_GREY);
			::cin.getline(Buffer, 1024);

			SStringA strCommand = Buffer;

			if (strCommand.empty()){ continue; }

			HiddenLayerSize = ::gloSToI(strCommand);

			if (HiddenLayerSize <= 0){ pTerminal->iOutputString(_SSTR("Error: Invalid value\r\n"), STC_WHITE, STC_RED); continue; }

			break;
		}

		for (int32 seek_layer = 0; seek_layer < HiddenLayerSize; seek_layer = seek_layer + 1)
		{
			uint32 NodeNumber = 1;

			while (true)
			{
				pTerminal->iOutputString(_SSTR("Please enter layer #") + ::gloIToS(seek_layer + 1) + _SSTR(" node size: "), STC_GREY);
				::cin.getline(Buffer, 1024);

				SStringA strCommand = Buffer;

				if (strCommand.empty()){ continue; }

				NodeNumber = ::gloSToI(strCommand);

				if (NodeNumber == 0){ pTerminal->iOutputString(_SSTR("Error: Invalid value\r\n"), STC_WHITE, STC_RED); continue; }

				break;
			}

			ANN.iCreateLayer(NodeNumber);
		}

		ANN.iCreateLayer(ResultTable.size() - 1);

		break;
	}
#pragma endregion

	sfloat NoiseMinLevel = 0.0;
	sfloat NoiseMaxLevel = 0.3;
	sfloat NoiseIncreaseStep = 0.02;

#pragma region Get user input, noise min level, noise max level and increase step
	//Get user input, noise min level, noise max level and increase step
	while (true)
	{
		pTerminal->iOutputString(_SSTR("Please enter noise minimum level (percentage, 0.0 - 1.0): "), STC_GREY);
		::cin.getline(Buffer, 1024);

		SStringA strCommand = Buffer;

		if (strCommand.empty()){ continue; }

		sfloat Rate = ::gloSToF(strCommand);

		if ((Rate<0.0) || (Rate>1.0)){ pTerminal->iOutputString(_SSTR("Error: Invalid value\r\n"), STC_WHITE, STC_RED); continue; }

		NoiseMinLevel = Rate;

		break;
	}

	while (true)
	{
		pTerminal->iOutputString(_SSTR("Please enter noise maximum level (percentage, noise minimum level - 1.0): "), STC_GREY);
		::cin.getline(Buffer, 1024);

		SStringA strCommand = Buffer;

		if (strCommand.empty()){ continue; }

		sfloat Rate = ::gloSToF(strCommand);

		if ((Rate<0.0) || (Rate>1.0)){ pTerminal->iOutputString(_SSTR("Error: Invalid value\r\n"), STC_WHITE, STC_RED); continue; }
		if (Rate < NoiseMinLevel){ pTerminal->iOutputString(_SSTR("Error: Invalid value\r\n"), STC_WHITE, STC_RED); continue; }

		NoiseMaxLevel = Rate;

		break;
	}

	while (true)
	{
		pTerminal->iOutputString(_SSTR("Please enter noise increase step (percentage, 0.0 - 1.0): "), STC_GREY);
		::cin.getline(Buffer, 1024);

		SStringA strCommand = Buffer;

		if (strCommand.empty()){ continue; }

		sfloat Rate = ::gloSToF(strCommand);

		if ((Rate <= 0.0) || (Rate > 1.0)){ pTerminal->iOutputString(_SSTR("Error: Invalid value\r\n"), STC_WHITE, STC_RED); continue; }

		NoiseIncreaseStep = Rate;

		break;
	}
#pragma endregion

	const uint32 IterationTimes = 5;

	uint32 NoiseTimes = (NoiseMaxLevel*1000.0 - NoiseMinLevel*1000.0) / (NoiseIncreaseStep*1000.0);

	vector<vector<sfloat>> AccuracyArray(6);// = new vector<sfloat>[IterationTimes + 1];

	for (uint32 seek = 0; seek <= IterationTimes; seek = seek + 1)
	{
		AccuracyArray[seek].resize(NoiseTimes);
		for (uint32 seek_item = 0; seek_item < NoiseTimes; seek_item = seek_item + 1)
		{
			AccuracyArray[seek][seek_item] = 0.0;
		}
	}

	for (uint32 seek_iteration = 0; seek_iteration < IterationTimes; seek_iteration = seek_iteration + 1)
	{
		for (sfloat seek_noise = NoiseMinLevel; seek_noise <= NoiseMaxLevel; seek_noise = seek_noise + NoiseIncreaseStep)
		{
			pTerminal->iOutputString(_SSTR("\nNoise Rate: ") + ::gloFToS(seek_noise) + _SSTR("\r\n"));
			ANNTRAININGSET NoiseTrainingSet;
			ANNTRAININGSET NoiseValidationSet;

#pragma region Add noise and create noise training set, noise validation set
			/*Add noise and seprate the instance to training set or validation set*/
			sfloat RandomNum = 0.0;
			uint32 InstanceOutputIndex = 0;
			uint32 ResultArraySize = (*pTrainingSet)[0].second.size();
			uint32 Count = 0;
			for (uint32 seek = 0; seek < pTrainingSet->size(); seek = seek + 1)
			{
				pair<vector<sfloat>, vector<sfloat>> Instance = (*pTrainingSet)[seek];
				RandomNum = ::rand() % 1000;
				if (RandomNum < (seek_noise * 10))
				{
					//PrintInstance(Instance);
					for (uint32 seek_res = 0; seek_res < ResultArraySize; seek_res = seek_res + 1)
					{
						if (::gloIsFloatEqual(Instance.second[seek_res], 1.0))
						{
							InstanceOutputIndex = seek_res;
						}
						Instance.second[seek_res] = 0.0;
					}
					RandomNum = ::rand() % ResultArraySize;
					while (::gloIsFloatEqual(RandomNum, InstanceOutputIndex))
					{
						RandomNum = ::rand() % ResultArraySize;
					}
					Instance.second[(uint32) RandomNum] = 1.0;
				}

				RandomNum = ::rand() % 1000;
				RandomNum = RandomNum / 1000.0;
				if (RandomNum < ValidationSetRate)
				{
					NoiseValidationSet.push_back(Instance);
				}
				else
				{
					NoiseTrainingSet.push_back(Instance);
				}
			}
			pTerminal->iOutputString(_SSTR("Training Set Size:\t") + ::gloIToS(NoiseTrainingSet.size()) + _SSTR("\n"));
			pTerminal->iOutputString(_SSTR("Validation Set Size:\t") + ::gloIToS(NoiseValidationSet.size()) + _SSTR("\n"));
#pragma endregion

			cArtificialNeuralNetworkAlgorithm ANNNetwork = ANN;

			ANNNetwork.iReset();

			ANNNetwork.iTraining(NoiseTrainingSet, 20000);

			sfloat Accuracy = 0.0;

#pragma region Evaluate accuracy before pruning
			uint32 NoiseLevelIndex = seek_noise / NoiseIncreaseStep;

			/*Training Set Accuracy Before Pruning*/
			Accuracy = CalcSetAccuracy(ANNNetwork, NoiseTrainingSet);
			pTerminal->iOutputString(_SSTR("Training Set\tAcc - BP: ") + ::gloFToS(Accuracy, _SSTR("5.3")) + _SSTR("\n"));
			AccuracyArray[0][NoiseLevelIndex] = AccuracyArray[0][NoiseLevelIndex] + Accuracy;

			/*Validation Set Accuracy Before Pruning*/
			Accuracy = CalcSetAccuracy(ANNNetwork, NoiseValidationSet);
			pTerminal->iOutputString(_SSTR("Validation Set\tAcc - BP: ") + ::gloFToS(Accuracy, _SSTR("5.3")) + _SSTR("\n"));
			AccuracyArray[1][NoiseLevelIndex] = AccuracyArray[1][NoiseLevelIndex] + Accuracy;

			/*Test Set Accuracy Before Pruning*/
			Accuracy = CalcSetAccuracy(ANNNetwork, *pTestSet);
			pTerminal->iOutputString(_SSTR("Test Set\tAcc - BP: ") + ::gloFToS(Accuracy, _SSTR("5.3")) + _SSTR("\n"));
			AccuracyArray[2][NoiseLevelIndex] = AccuracyArray[2][NoiseLevelIndex] + Accuracy;
#pragma endregion

			//ANNNetwork.iTrainingWithValidation(NoiseTrainingSet, NoiseTrainingSet, 50000);
			ANNNetwork.iPruning(NoiseValidationSet);

#pragma region Evaluate accuracy after pruning
			/*Training Set Accuracy Before Pruning*/
			Accuracy = CalcSetAccuracy(ANNNetwork, NoiseTrainingSet);
			pTerminal->iOutputString(_SSTR("Training Set\tAcc - AP: ") + ::gloFToS(Accuracy, _SSTR("5.3")) + _SSTR("\n"));
			AccuracyArray[3][NoiseLevelIndex] = AccuracyArray[3][NoiseLevelIndex] + Accuracy;

			/*Validation Set Accuracy Before Pruning*/
			Accuracy = CalcSetAccuracy(ANNNetwork, NoiseValidationSet);
			pTerminal->iOutputString(_SSTR("Validation Set\tAcc - AP: ") + ::gloFToS(Accuracy, _SSTR("5.3")) + _SSTR("\n"));
			AccuracyArray[4][NoiseLevelIndex] = AccuracyArray[4][NoiseLevelIndex] + Accuracy;

			/*Test Set Accuracy Before Pruning*/
			Accuracy = CalcSetAccuracy(ANNNetwork, *pTestSet);
			pTerminal->iOutputString(_SSTR("Test Set\tAcc - AP: ") + ::gloFToS(Accuracy, _SSTR("5.3")) + _SSTR("\n"));
			AccuracyArray[5][NoiseLevelIndex] = AccuracyArray[5][NoiseLevelIndex] + Accuracy;
#pragma endregion
			//NoiseLevelIndex = NoiseLevelIndex + 1;
		}
	}

#pragma region Accuracy output
	SString strAccuracyType[6];
	strAccuracyType[0] = _SSTR("\r\nBefore Pruning\r\nTraining   Set: ");
	strAccuracyType[1] = _SSTR("Validation Set: ");
	strAccuracyType[2] = _SSTR("Test       Set: ");
	strAccuracyType[3] = _SSTR("\r\nAfter Pruning\r\nTraining   Set: ");
	strAccuracyType[4] = _SSTR("Validation Set: ");
	strAccuracyType[5] = _SSTR("Test       Set: ");
	strOutput = strOutput + _SSTR("\r\n\r\nNoise Level:    ");
	for (int32 seek = NoiseMinLevel; seek <= NoiseMaxLevel; seek = seek + NoiseIncreaseStep)
	{
		strOutput = strOutput + ::gloFToS((sfloat) seek / 100.0) + _SSTR(" ");
	}
	for (uint32 seek = 0; seek < 6; seek = seek + 1)
	{
		strOutput = strOutput + _SSTR("\r\n") + strAccuracyType[seek];
		for (int32 seek_noise = NoiseMinLevel; seek_noise <= NoiseTimes; seek_noise = seek_noise + 1)
		{
			strOutput = strOutput + ::gloFToS(AccuracyArray[seek][seek_noise] * (1.0 / (sfloat) IterationTimes)) + _SSTR(" ");
		}
	}
#pragma endregion

	/*::gloWriteFile(strOutputFilePath, strOutput, true);
	::system(SStringA(SStringA("notepad ") + ::gloTToA(strOutputFilePath)).c_str());*/

	delete pTrainingSet;
	pTrainingSet = nullptr;
	delete pTestSet;
	pTestSet = nullptr;
	/*delete[] pAccuracyArray;
	pAccuracyArray = nullptr;*/

	/*::system("pause");*/

	return strOutput;
}
sfloat CalcAccuracy(const vector<sfloat> &PredictVector, const vector<sfloat> &TrainOutputVector)
{
	sfloat Distance = 0.0;
	//Distance = CalcDistance(PredictVector, TrainOutputVector);
	uint32 MaxID = 0;
	for (uint32 seek = 0; seek < PredictVector.size(); seek = seek + 1)
	{
		if (PredictVector[MaxID] < PredictVector[seek])
		{
			MaxID = seek;
		}
	}
	return TrainOutputVector[MaxID] == 1.0 ? 0.0 : FLT_MAX;
}
sfloat CalcDistance(const vector<sfloat> &Vec1, const vector<sfloat> &Vec2)
{
	if (Vec1.size() != Vec2.size())
	{
		return -1.0;
	}
	sfloat dis = 0.0;
	for (uint32 seek = 0; seek < Vec1.size(); seek = seek + 1)
	{
		dis = dis + (Vec1[seek] - Vec2[seek])*(Vec1[seek] - Vec2[seek]);
	}
	return ::sqrt(dis);
}
void PrintInstance(const pair<vector<sfloat>, vector<sfloat>> &Instance, cSanTerminalDevice* pTerminal)
{
	SString strOutput = _SSTR("Instance: ");
	uint32 AttribSize = Instance.first.size();
	uint32 ResSize = Instance.second.size();
	for (uint32 seek = 0; seek < AttribSize; seek = seek + 1)
	{
		strOutput = strOutput + ::gloFToS(Instance.first[seek]) + _SSTR(" ");
	}
	strOutput = strOutput + _SSTR("\t");
	for (uint32 seek = 0; seek < ResSize; seek = seek + 1)
	{
		strOutput = strOutput + ::gloFToS(Instance.second[seek]) + _SSTR(" ");
	}
	pTerminal->iOutputString(strOutput + _SSTR("\r\n"));
}
sfloat CalcSetAccuracy(cArtificialNeuralNetworkAlgorithm &ANNNetwork, const ANNTRAININGSET &Set)
{
	sfloat Accuracy = 0.0;
	sfloat SetSize = Set.size();
	for (uint32 seek = 0; seek < SetSize; seek = seek + 1)
	{
		ANNNetwork.iPredict(Set[seek].first);
		if (CalcAccuracy(ANNNetwork.iGetOutputArray(), Set[seek].second) < ErrorBoundary)
		{
			Accuracy = Accuracy + 1.0;
		}
	}
	return Accuracy / SetSize;
}