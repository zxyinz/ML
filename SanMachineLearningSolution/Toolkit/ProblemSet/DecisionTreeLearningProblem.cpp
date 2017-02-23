#include"DecisionTreeLearningProblem.h"
using namespace std;
using namespace San;
using namespace San::FileIOStream;
using namespace San::Device;
using namespace San::MachineLearning;
bool LoadAttributeArray(const SString &strFilePath, vector<ATTRIBUTE> &AttributeArray, vector<SString> &ResultArray)
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
		ATTRIBUTE Attribute;
		Attribute.strName = SubItems[0];
		if (SubItems[1] == _SSTR("continuous"))
		{
			Attribute.bIsContinuesValue = true;
			Attribute.Boundary.push_back(-1024.0);
			Attribute.Boundary.push_back(1024.0);
		}
		else
		{
			SubItems.erase(SubItems.begin());
			Attribute.bIsContinuesValue = false;
			Attribute.ValueArray = SubItems;
		}
		AttributeArray.push_back(Attribute);
	}
	vector<SString> ResItems = ::gloGetStringItems(Items.back());
	ResItems.erase(ResItems.begin());
	ResultArray = ResItems;
	return true;
}

bool LoadInstanceSpace(const SString &strFilePath, vector<INSTANCE> &TrainingInstanceSpace, vector<INSTANCE> &ValidationInstanceSpace, const sfloat ValidationFactor)
{
	SStringA strData;
	if (!gloLoadFile(strFilePath, strData))
	{
		return false;
	}
	//::srand(time(0));
	vector<SString> Items = ::gloGetStringItems(::gloAToT(strData), _SSTR("\n\t\r"));

	for (uint32 seek = 0; seek < Items.size(); seek = seek + 1)
	{
		sfloat CurrentVal = ::rand() % 1000;
		CurrentVal = CurrentVal / 1000.0;
		if (CurrentVal >= ValidationFactor)
		{
			TrainingInstanceSpace.push_back(make_pair(::gloGetStringItems(Items[seek]), 1.0));
		}
		else
		{
			ValidationInstanceSpace.push_back(make_pair(::gloGetStringItems(Items[seek]), 1.0));
		}
	}

	return true;
}
sfloat PredictInstanceSpace(cDecisionTreeAlgorithm &DecisionTree, vector<INSTANCE> &InstanceArray, SString &strOutput, bool bUseRule)
{
	if (InstanceArray.size() == 0)
	{
		strOutput = strOutput + _SSTR(" predect accuracy: INSTANCE SPACE EMPTY\r\n");
		return 0.0;
	}
	sfloat SuccessCount = 0;
	for (uint32 seek = 0; seek < InstanceArray.size(); seek = seek + 1)
	{
		SString strRes = InstanceArray[seek].first.back();
		SString strPred = (!bUseRule) ? DecisionTree.iPredict(InstanceArray[seek]) : DecisionTree.iPrediceByRuleSet(InstanceArray[seek]);
		if (strRes == strPred)
		{
			SuccessCount = SuccessCount + 1;
		}
		/*else
		{
		for (uint32 seek_str = 0; seek_str < InstanceArray[seek].first.size(); seek_str = seek_str + 1)
		{
		strOutput = strOutput + InstanceArray[seek].first[seek_str] + _SSTR(" ");
		}
		strOutput = strOutput + strRes + _SSTR("\r\n");
		}*/
	}
	strOutput = strOutput + _SSTR(" predect accuracy:\t") + ::gloFToS(SuccessCount / (sfloat) (InstanceArray.size())) + _SSTR("\r\n");
	return SuccessCount / (sfloat) (InstanceArray.size());
}
SString GenerateDecisionTree(cDecisionTreeAlgorithm &DecisionTree, vector<ATTRIBUTE> &AttributeArray, vector<INSTANCE> &TrainInstanceArray, vector<SString> &ResultArray)
{
	const SString strLineN = _SSTR("----------------------------------------\r\n\r\n");
	const SString strLine = _SSTR("----------------------------------------\r\n");

	for (uint32 seek = 0; seek < AttributeArray.size(); seek = seek + 1)
	{
		if (!AttributeArray[seek].bIsContinuesValue)
		{
			DecisionTree.iCreateAttribute(AttributeArray[seek].strName, AttributeArray[seek].ValueArray);
		}
		else
		{
			DecisionTree.iCreateAttribute(AttributeArray[seek].strName, AttributeArray[seek].Boundary);
		}
	}

	DecisionTree.iUpdateResultAttribute(_SSTR("RESULT"), ResultArray);

	DecisionTree.iGenerateTree(TrainInstanceArray);
	SString strOutput = SString(_SSTR("\r\n\r\nDecision Tree Structutr:\r\n")) + strLineN + DecisionTree.iPrintTree().c_str();

	strOutput = SString(_SSTR("Inner Attribute Array:\r\n")) + strLineN + DecisionTree.iPrintInnerAttribute() + _SSTR("\r\n") + strOutput;

	DecisionTree.iGenerateRule();
	strOutput = strOutput + _SSTR("\r\n\r\n");
	strOutput = strOutput + SString(_SSTR("Rule Array(before purning):\r\n")) + strLineN + DecisionTree.iPrintRule();

	strOutput = strOutput + _SSTR("\r\n\r\n");
	return strOutput;
}