#include"GeneticAlgorithmLearningProblem.h"
using namespace std;
using namespace San;
using namespace San::FileIOStream;
using namespace San::Device;
using namespace San::MachineLearning;

const SString strTennisAttrPath = _SSTR("DataSet\\tennis-attr.txt");
const SString strTennisTrainPath = _SSTR("DataSet\\tennis-train.txt");
const SString strTennisTestPath = _SSTR("DataSet\\tennis-test.txt");

const SString strIrisAttrPath = _SSTR("DataSet\\iris-attr.txt");
const SString strIrisTrainPath = _SSTR("DataSet\\iris-train.txt");
const SString strIrisTestPath = _SSTR("DataSet\\iris-test.txt");


const SString strOutputFilePath = _SSTR("Output.txt");
const SString strLine = _SSTR("------------------------------\r\n\r\n");

SString TennisGA(cSanTerminalDevice* pTerminal)
{
	::cout << "GENETIC ALGORITHM:\n\n";

	SString strOutput = _SSTR("GENETIC ALGORITHM:\r\n") + strLine;

	vector<pair<SString, vector<SString>>> AttributeArray;
	vector<SString> ResultArray;

	LoadAttribute(strTennisAttrPath, AttributeArray, ResultArray);

	GAINSTANCESET<SString, SString> TrainingSet;
	LoadInstanceSet(strTennisTrainPath, TrainingSet);

	GAINSTANCESET<SString, SString> TestSet;
	LoadInstanceSet(strTennisTestPath, TestSet);

	cGeneticAlgorithm<SString, SString, GARULEDESC, cGARuleSet> GA;

	GARULEDESC Desc;
	Desc.RuleSetSize = 2;
	Desc.AttributeArray = AttributeArray;
	Desc.ResultArray = ResultArray;

	GA.iSetAlgorithmParams(30, 0.5, 0.2, GASST_DEFAULT);

	GA.iGeneratePolulation(TrainingSet, Desc, 0.90, 1000);

	strOutput = strOutput + GA.iPrintHypothesisSpace();

	strOutput = strOutput + _SSTR("Training Set Accuracy:\t") + ::gloFToS(GA.iAccuracy(TrainingSet)) + _SSTR("\r\n");

	strOutput = strOutput + _SSTR("Test Set Accuracy:\t") + ::gloFToS(GA.iAccuracy(TestSet)) + _SSTR("\r\n");

	return strOutput;
}
SString IrisGA(cSanTerminalDevice* pTerminal)
{
	::cout << "GENETIC ALGORITHM:\n\n";

	SString strOutput = _SSTR("GENETIC ALGORITHM:\r\n") + strLine;

	vector<pair<SString, vector<SString>>> AttributeArray;
	vector<SString> ResultArray;

	LoadAttribute(strIrisAttrPath, AttributeArray, ResultArray);

	GAINSTANCESET<SString, SString> TrainingSet;
	LoadInstanceSet(strIrisTrainPath, TrainingSet);

	GAINSTANCESET<SString, SString> TestSet;
	LoadInstanceSet(strIrisTestPath, TestSet);

	cGeneticAlgorithm<SString, SString, GACONTINUERULEDESC, cGAContinueRuleSet> GA;

	GACONTINUERULEDESC Desc;
	Desc.RuleSetSize = 4;
	Desc.ValBoundary[0] = 0.0;
	Desc.ValBoundary[1] = 10.0;
	Desc.BitBoundary[0] = 0;
	Desc.BitBoundary[1] = 255;

	for (uint32 seek = 0; seek < AttributeArray.size(); seek = seek + 1)
	{
		Desc.AttributeArray.push_back(AttributeArray[seek].first);
	}
	Desc.ResultArray = ResultArray;

	GA.iSetAlgorithmParams(100, 0.5, 0.01, GASST_DEFAULT);

	GA.iGeneratePolulation(TrainingSet, Desc, 0.9, 1000);

	strOutput = strOutput + GA.iPrintHypothesisSpace();

	strOutput = strOutput + _SSTR("Training Set Accuracy:\t") + ::gloFToS(GA.iAccuracy(TrainingSet)) + _SSTR("\r\n");

	strOutput = strOutput + _SSTR("Test Set Accuracy:\t") + ::gloFToS(GA.iAccuracy(TestSet)) + _SSTR("\r\n");

	return strOutput;
}
SString IrisSGA(cSanTerminalDevice* pTerminal)
{
	::cout << "GENETIC ALGORITHM:\n\n";

	SString strOutput = _SSTR("GENETIC ALGORITHM:\r\n") + strLine;

	vector<pair<SString, vector<SString>>> AttributeArray;
	vector<SString> ResultArray;

	LoadAttribute(strIrisAttrPath, AttributeArray, ResultArray);

	GAINSTANCESET<SString, SString> TrainingSet;
	LoadInstanceSet(strIrisTrainPath, TrainingSet);

	GAINSTANCESET<SString, SString> TestSet;
	LoadInstanceSet(strIrisTestPath, TestSet);

	GACONTINUERULEDESC Desc;
	Desc.RuleSetSize = 4;
	Desc.ValBoundary[0] = 0.0;
	Desc.ValBoundary[1] = 10.0;
	Desc.BitBoundary[0] = 0;
	Desc.BitBoundary[1] = 255;

	for (uint32 seek = 0; seek < AttributeArray.size(); seek = seek + 1)
	{
		Desc.AttributeArray.push_back(AttributeArray[seek].first);
	}
	Desc.ResultArray = ResultArray;

	for (uint32 seek = 10; seek < 50; seek = seek + 10)
	{
		::cout << "Generation Num: " << seek << "\n";
		for (uint32 seek_method = 0; seek_method < 3; seek_method = seek_method + 1)
		{
			cGeneticAlgorithm<SString, SString, GACONTINUERULEDESC, cGAContinueRuleSet> GA;

			SString strMethod = _SSTR("Fitness   ");

			if (seek_method == 1)
			{
				strMethod = _SSTR("Tournament");
			}
			if (seek_method == 2)
			{
				strMethod = _SSTR("Rank      ");
			}

			::wcout << strMethod.c_str();

			GA.iSetAlgorithmParams(100, 0.5, 0.01, eGASELECTMETHOD(seek_method));
			GA.iGeneratePolulation(TrainingSet, Desc, 0.9, seek);

			sfloat Accuracy = GA.iAccuracy(TestSet);

			::cout << " Accuracy: " << Accuracy << "\n";

			strOutput = strOutput + _SSTR("Generations: ") + ::gloIToS(seek) + _SSTR("\r\n");
			strOutput = strOutput + _SSTR("Selection Method: ") + strMethod + _SSTR("\tTest Set Accuracy:\t") + ::gloFToS(Accuracy) + _SSTR("\r\n");

		}
	}
	
	return strOutput;
}
SString IrisRGA(cSanTerminalDevice* pTerminal)
{
	::cout << "GENETIC ALGORITHM:\n\n";

	SString strOutput = _SSTR("GENETIC ALGORITHM:\r\n") + strLine;

	vector<pair<SString, vector<SString>>> AttributeArray;
	vector<SString> ResultArray;

	LoadAttribute(strIrisAttrPath, AttributeArray, ResultArray);

	GAINSTANCESET<SString, SString> TrainingSet;
	LoadInstanceSet(strIrisTrainPath, TrainingSet);

	GAINSTANCESET<SString, SString> TestSet;
	LoadInstanceSet(strIrisTestPath, TestSet);

	GACONTINUERULEDESC Desc;
	Desc.RuleSetSize = 4;
	Desc.ValBoundary[0] = 0.0;
	Desc.ValBoundary[1] = 10.0;
	Desc.BitBoundary[0] = 0;
	Desc.BitBoundary[1] = 255;

	for (uint32 seek = 0; seek < AttributeArray.size(); seek = seek + 1)
	{
		Desc.AttributeArray.push_back(AttributeArray[seek].first);
	}
	Desc.ResultArray = ResultArray;

	for (uint32 seek = 1; seek <= 9; seek = seek + 1)
	{
		cGeneticAlgorithm<SString, SString, GACONTINUERULEDESC, cGAContinueRuleSet> GA;

		sfloat Rate = (sfloat) seek / 10.0;
		::cout << "Replacement Rate: " << Rate << "\n";

		GA.iSetAlgorithmParams(100, Rate, 0.01, GASST_DEFAULT);
		GA.iGeneratePolulation(TrainingSet, Desc, 0.6, 50);

		sfloat Accuracy = GA.iAccuracy(TestSet);
		::cout << "Accuracy: " << Accuracy << "\n";

		strOutput = strOutput + _SSTR("Replacement Rate: ") + ::gloFToS(Rate) + _SSTR("\tTest Set Accuracy:\t") + ::gloFToS(Accuracy) + _SSTR("\r\n");

	}

	return strOutput;
}
bool LoadAttribute(SString strFilePath, vector<pair<SString, vector<SString>>> &AttributeArray, vector<SString> &ResultArray)
{
	SStringA strData;
	if (!gloLoadFile(strFilePath, strData))
	{
		return false;
	}
	vector<SString> Items = ::gloGetStringItems(::gloAToT(strData), _SSTR("\n\t\r"));
	for (uint32 seek = 0; seek < (Items.size() - 1); seek = seek + 1)
	{
		vector<SString> SubItems = ::gloGetStringItems(Items[seek]);
		pair<SString, vector<SString>> Attribute;
		Attribute.first = SubItems[0];
		SubItems.erase(SubItems.begin());
		Attribute.second = SubItems;
		AttributeArray.push_back(Attribute);
	}
	vector<SString> ResItems = ::gloGetStringItems(Items.back());
	ResItems.erase(ResItems.begin());
	ResultArray = ResItems;
	return true;
}
bool LoadInstanceSet(const SString &strFilePath, GAINSTANCESET<SString, SString> &InstanceSet)
{
	SStringA strData;
	if (!gloLoadFile(strFilePath, strData))
	{
		return false;
	}
	vector<SString> Items = ::gloGetStringItems(::gloAToT(strData), _SSTR("\n\t\r"));
	for (uint32 seek = 0; seek < Items.size(); seek = seek + 1)
	{
		vector<SString> SubItems = ::gloGetStringItems(Items[seek]);
		pair<vector<SString>, vector<SString>> Instance;
		Instance.second.push_back(SubItems.back());
		SubItems.resize(SubItems.size() - 1);
		Instance.first = SubItems;
		InstanceSet.push_back(Instance);
	}
	return true;
}