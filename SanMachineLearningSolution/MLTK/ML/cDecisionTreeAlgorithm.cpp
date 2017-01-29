#include"cDecisionTreeAlgorithm.h"
using namespace std;
using namespace San;
using namespace San::MachineLearning;
cDecisionTreeAlgorithm::cDecisionTreeAlgorithm()
	:m_pTreeRoot(nullptr)
{
	this->m_OuterArributeArray.clear();
	this->m_InnerAttributeArray.clear();
	this->m_ResultArray.clear();
}
cDecisionTreeAlgorithm::~cDecisionTreeAlgorithm()
{
	this->m_OuterArributeArray.clear();
	this->m_InnerAttributeArray.clear();
	this->m_ResultArray.clear();
	if (this->m_pTreeRoot != nullptr)
	{
		delete this->m_pTreeRoot;
		this->m_pTreeRoot = nullptr;
	}
}
vector<ATTRIBUTE> cDecisionTreeAlgorithm::_GenerateInnerAttribute(const ATTRIBUTE &Attribute, const vector<INSTANCE> &InstanceSpace) const
{
	// Continues value: [a,b) "a <= X < b"
	vector<ATTRIBUTE> InnerAttributeArray;
	if (!Attribute.bIsContinuesValue)
	{
		InnerAttributeArray.push_back(Attribute);
		return InnerAttributeArray;
	}
	vector<pair<int32,SString>> ValueSet;
	for (uint32 seek = 0; seek < InstanceSpace.size(); seek = seek + 1)
	{
		ValueSet.push_back(make_pair(::gloSToF(InstanceSpace[seek].first[Attribute.ID])*100.0, InstanceSpace[seek].first.back()));
	}
	/*Sort*/
	pair<int32, SString> Buffer;
	for (uint32 seek = 0; seek < ValueSet.size(); seek = seek + 1)
	{
		sfloat MinIndex = seek;
		for (uint32 seek_min = seek; seek_min < ValueSet.size(); seek_min = seek_min + 1)
		{
			if (ValueSet[MinIndex].first>ValueSet[seek_min].first)
			{
				MinIndex = seek_min;
			}
		}
		Buffer = ValueSet[seek];
		ValueSet[seek] = ValueSet[MinIndex];
		ValueSet[MinIndex] = Buffer;
	}

	/*Add boundary point*/
	vector<int32> BoundarySet;
	SString PrevResult = _SSTR("");
	BoundarySet.push_back(Attribute.Boundary[0]);
	for (uint32 seek = 0; seek < ValueSet.size(); seek = seek + 1)
	{
		if (ValueSet[seek].second != PrevResult)
		{
			BoundarySet.push_back((sfloat)(ValueSet[seek].first));
			PrevResult = ValueSet[seek].second;
		}
	}
	BoundarySet.push_back(Attribute.Boundary[1]);

	/*Convert the boundary point to node - [a,b), with TURE and FALSE*/
	vector<SString> BinValueSet;
	BinValueSet.push_back(_SSTR("TRUE"));
	BinValueSet.push_back(_SSTR("FALSE"));
	ATTRIBUTE BinAttrib;
	BinAttrib.strName = _SSTR("");
	BinAttrib.ID = 0;
	BinAttrib.ValueArray = BinValueSet;
	BinAttrib.bIsContinuesValue = false;
	BinAttrib.Boundary.clear();
	BinAttrib.bIsTaken = false;
	for (uint32 seek = 1; seek < BoundarySet.size(); seek = seek + 1)
	{
		bool bFound = false;
		SString strName = Attribute.strName + _SSTR("_") + ::gloIToS(BoundarySet[seek - 1]) + _SSTR("_") + ::gloIToS(BoundarySet[seek]);
		uint32 AttribArraySize = InnerAttributeArray.size();
		for (uint32 seek_c = 0; seek_c < AttribArraySize; seek_c = seek_c + 1)
		{
			if (InnerAttributeArray[seek_c].strName == strName)
			{
				bFound = true;
				break;
			}
		}
		if (!bFound)
		{
			BinAttrib.strName = strName;
			InnerAttributeArray.push_back(BinAttrib);
		}
	}
	return InnerAttributeArray;
}
INSTANCE cDecisionTreeAlgorithm::_GenerateInnerInstance(const INSTANCE &Instance) const
{
	INSTANCE InnerInstance;
	InnerInstance.second = Instance.second;
	uint32 AttributeSize = this->m_OuterArributeArray.size();
	for (uint32 seek = 0; seek < AttributeSize; seek = seek + 1)
	{
		if (!this->m_OuterArributeArray[seek].bIsContinuesValue)
		{
			InnerInstance.first.push_back(Instance.first[seek]);
			continue;
		}
		/*Find the boundary of continues attribute		*/
		/*InnerAttribID:	0 - start id	1 - end id	*/
		int32 InnerAttribID[2] = { -1, -1 };
		for (uint32 seek_attrib = 0; seek_attrib < this->m_InnerAttributeArray.size(); seek_attrib = seek_attrib + 1)
		{
			vector<SString> Items = ::gloGetStringItems(this->m_InnerAttributeArray[seek_attrib].strName, _SSTR("_"));
			if (Items.size() != 3)
			{
				continue;
			}
			if (Items[0] == this->m_OuterArributeArray[seek].strName)
			{
				if (InnerAttribID[0] == -1)
				{
					InnerAttribID[0] = seek_attrib;
				}
				InnerAttribID[1] = seek_attrib;
			}
		}
		if (InnerAttribID[0] == -1)
		{
			::cout << "ERROR: CAN NOT FIND THE CONTINUES ATTRIBUTE";
		}
		/*Generate the continues attribute to several boolean attributes*/
		sfloat CurrentValue = ::gloSToF(Instance.first[seek]);
		sfloat Boundary[2];
		for (uint32 seek_attrib = InnerAttribID[0]; seek_attrib <= InnerAttribID[1]; seek_attrib = seek_attrib + 1)
		{
			vector<SString> Items = ::gloGetStringItems(this->m_InnerAttributeArray[seek_attrib].strName,_SSTR("_"));
			Boundary[0] = ::gloSToF(Items[1]) / 100.0;
			Boundary[1] = ::gloSToF(Items[2]) / 100.0;
			if ((CurrentValue >= Boundary[0]) && (CurrentValue < Boundary[1]))
			{
				InnerInstance.first.push_back(_SSTR("TRUE"));
			}
			else
			{
				InnerInstance.first.push_back(_SSTR("FALSE"));
			}
		}
	}
	InnerInstance.first.push_back(Instance.first.back()); // Push the result value
	return InnerInstance;
}
sfloat cDecisionTreeAlgorithm::_CalcInformationGain(const uint32 AttributeID, vector<INSTANCE> &InstanceSpace, const sfloat PreviousEntropy)
{
	uint32 ValSize = this->m_InnerAttributeArray[AttributeID].ValueArray.size();
	uint32 ResSize = this->m_ResultArray.size();
	uint32 AttribSize = this->m_InnerAttributeArray.size();
	vector<vector<sfloat>> ValCount(ValSize);

	/*Initialize the count space*/
	for (uint32 seek_val = 0; seek_val < ValSize; seek_val = seek_val + 1)
	{
		ValCount[seek_val].resize(ResSize);
		for (uint32 seek_res = 0; seek_res < ResSize; seek_res = seek_res + 1)
		{
			ValCount[seek_val][seek_res] = 0.0;
		}
	}

	/*Count the instance distribution*/
	uint32 InstanceSize = InstanceSpace.size();
	sfloat InstanceSum = 0.0;
	for (uint32 seek = 0; seek < InstanceSize; seek = seek + 1)
	{
		for (uint32 seek_val = 0; seek_val < ValSize; seek_val = seek_val + 1)
		{
			if (InstanceSpace[seek].first[AttributeID] == this->m_InnerAttributeArray[AttributeID].ValueArray[seek_val])
			{
				for (uint32 seek_res = 0; seek_res < ResSize; seek_res = seek_res + 1)
				{
					if (InstanceSpace[seek].first[AttribSize] == this->m_ResultArray[seek_res])
					{
						ValCount[seek_val][seek_res] = ValCount[seek_val][seek_res] + InstanceSpace[seek].second;
					}
				}
			}
		}
		InstanceSum = InstanceSum + InstanceSpace[seek].second;
	}

	/*InstanceSum equal 0*/

	/*Calculate current attriibute entropy*/
	sfloat Entropy = PreviousEntropy;
	for (uint32 seek_val = 0; seek_val < ValSize; seek_val = seek_val + 1)
	{
		sfloat SubEntropy = 0.0;
		sfloat ValueWeight = 0.0;
		sfloat SubWeight = 0.0;
		for (uint32 seek_res = 0; seek_res < ResSize; seek_res = seek_res + 1)
		{
			ValueWeight = ValueWeight + ValCount[seek_val][seek_res];
		}
		ValueWeight = ValueWeight + 0.001;
		for (uint32 seek_res = 0; seek_res < ResSize; seek_res = seek_res + 1)
		{
			SubWeight = ValCount[seek_val][seek_res] / ValueWeight;
			sfloat LogB2 = log2(SubWeight + 0.001);
			SubEntropy = SubEntropy - SubWeight*LogB2;
		}
		ValueWeight = ValueWeight / InstanceSum;
		Entropy = Entropy - ValueWeight*SubEntropy;
	}
	//::wcout << this->m_InnerAttributeArray[AttributeID].strName.c_str() << L"   \t" << PreviousEntropy << "   \t" << Entropy << "\n";
	return Entropy;
}
bool cDecisionTreeAlgorithm::_GenerateDecisionTree(vector<INSTANCE> InstanceSpace, vector<ATTRIBUTE> AttributeSpace, lpDECISIONTREENODE pNode)
{
	if (pNode == nullptr)
	{
		return false;
	}
	/*Initialize the value count array*/
	sfloat TotalInstanceWeight = 0.0;
	uint32 ResultSize = this->m_ResultArray.size();
	pNode->iGetData().InstanceCount.resize(ResultSize);
	for (uint32 seek = 0; seek < ResultSize; seek = seek + 1)
	{
		pNode->iGetData().InstanceCount[seek] = 0.0;
	}
	for (uint32 seek = 0; seek < InstanceSpace.size(); seek = seek + 1)
	{
		for (uint32 seek_res = 0; seek_res < ResultSize; seek_res = seek_res + 1)
		{
			if (InstanceSpace[seek].first.back() == this->m_ResultArray[seek_res])
			{
				pNode->iGetData().InstanceCount[seek_res] = pNode->iGetData().InstanceCount[seek_res] + InstanceSpace[seek].second;
				break;
			}
		}
		TotalInstanceWeight = TotalInstanceWeight + InstanceSpace[seek].second;
	}
	sfloat MaxResultWeight = -FLT_MAX;
	int32 MaxResultWeightID = -1;
	/*If all examples are has same result, return the single node tree, with current result for name*/
	for (uint32 seek = 0; seek < ResultSize; seek = seek + 1)
	{
		if (pNode->iGetData().InstanceCount[seek] == TotalInstanceWeight)
		{
			pNode->iGetData().strName = this->m_ResultArray[seek];
			return true;
		}
		if (MaxResultWeight < pNode->iGetData().InstanceCount[seek])
		{
			MaxResultWeight = pNode->iGetData().InstanceCount[seek];
			MaxResultWeightID = seek;
		}
	}
	/*Pick attribute*/
	uint32 AttribSize = AttributeSpace.size();
	sfloat MaxInformationGain = -FLT_MAX;
	int32 MaxInformationGainID = -1;
	sfloat CurrentEntropy = 0.0;
	sfloat CurrentWeight;
	/*Calculate the current node entropy*/
	for (uint32 seek = 0; seek < ResultSize; seek = seek + 1)
	{
		CurrentWeight = pNode->iGetData().InstanceCount[seek] / (TotalInstanceWeight + 0.001);
		CurrentEntropy = CurrentEntropy - CurrentWeight*log2(CurrentWeight + 0.001);
	}
	/*Find max information gain*/
	for (uint32 seek = 0; seek < AttribSize; seek = seek + 1)
	{
		if (AttributeSpace[seek].bIsTaken)
		{
			continue;
		}
		sfloat CurrentInformationGain = this->_CalcInformationGain(seek, InstanceSpace, CurrentEntropy);
		if (MaxInformationGain <= CurrentInformationGain)
		{
			MaxInformationGain = CurrentInformationGain;
			MaxInformationGainID = seek;
		}
	}
	if (MaxInformationGainID == -1)
	{
		if (MaxResultWeightID == -FLT_MAX)
		{
			::cout << "ERROR: INVALID VALUE";
			return false;
		}
		pNode->iGetData().strName = this->m_ResultArray[MaxResultWeightID];
		return true;
	}
	/*Add node*/
	pNode->iGetData().strName = this->m_InnerAttributeArray[MaxInformationGainID].strName;
	pNode->iGetData().Index = MaxInformationGainID;
	pNode->iGetData().Extropy = CurrentEntropy;
	/*Remove the current attribute*/
	AttributeSpace[MaxInformationGainID].bIsTaken = true;
	/*Divide the instance space*/
	uint32 ValueSize = this->m_InnerAttributeArray[MaxInformationGainID].ValueArray.size();
	vector<vector<INSTANCE>> InstanceSpaceArray;
	InstanceSpaceArray.resize(ValueSize);
	for (uint32 seek = 0; seek < InstanceSpace.size(); seek = seek + 1)
	{
		for (uint32 seek_val = 0; seek_val < ValueSize; seek_val = seek_val + 1)
		{
			if (InstanceSpace[seek].first[MaxInformationGainID] == this->m_InnerAttributeArray[MaxInformationGainID].ValueArray[seek_val])
			{
				InstanceSpaceArray[seek_val].push_back(InstanceSpace[seek]);
			}
		}
	}
	ATTRIBUTENODE Node;
	bool bResult = true;
	for (uint32 seek = 0; seek < ValueSize; seek = seek + 1)
	{
		lpDECISIONTREENODE pNextNode = pNode->iCreateNode(Node);
		if (InstanceSpaceArray[seek].empty())
		{
			/*if (MaxResultWeightID == -1)
			{
				::cout << "ERROR: INVALID VALUE";
				return false;
			}*/
			pNextNode->iGetData().strName = this->m_ResultArray[MaxResultWeightID];
			//return true;
		}
		else
		{
			bResult = this->_GenerateDecisionTree(InstanceSpaceArray[seek], AttributeSpace, pNextNode);
			if (bResult == false)
			{
				return false;
			}
		}
	}
	return true;
}
SString cDecisionTreeAlgorithm::_SearchDecisionTree(const INSTANCE &Instance, const lpDECISIONTREENODE pNode) const
{
	if (pNode == nullptr)
	{
		return _SSTR("ERROR: THE DECISION TREE NODE IS EMPTY");
	}
	if (pNode->iIsLeaf())
	{
		return pNode->iGetData().strName;
	}
	uint32 Index = pNode->iGetData().Index;
	SString strValue = Instance.first[Index];
	uint32 ValueArraySize = this->m_InnerAttributeArray[Index].ValueArray.size();
	for (uint32 seek = 0; seek < ValueArraySize; seek = seek + 1)
	{
		if (strValue == this->m_InnerAttributeArray[Index].ValueArray[seek])
		{
			return this->_SearchDecisionTree(Instance, (*pNode)[seek]);
		}
	}
	return _SSTR("ERROR: CAN NOT FIND THE SUITABLE ATTRIBUTE VALUE");
}
void cDecisionTreeAlgorithm::_PrintTree(const lpDECISIONTREENODE pNode, const SString &PrevString, SString &strOutputString) const
{
	if (pNode == nullptr)
	{
		return;
	}
	strOutputString = strOutputString + PrevString + _SSTR("{");
	if (pNode->iIsRoot())
	{
		strOutputString = strOutputString + _SSTR("ROOT");
	}
	else
	{
		uint32 Index = pNode->iGetTrunkPtr()->iGetData().Index;
		uint32 ValID = pNode->iGetID();
		strOutputString = strOutputString + pNode->iGetTrunkPtr()->iGetData().strName + _SSTR("_") + this->m_InnerAttributeArray[Index].ValueArray[ValID];
	}
	strOutputString = strOutputString + _SSTR("} - ");
	if (pNode->iIsLeaf())
	{
		strOutputString = strOutputString + _SSTR("LEAF: ") + pNode->iGetData().strName;
	}
	else
	{
		strOutputString = strOutputString + _SSTR("NODE: ") + pNode->iGetData().strName;
	}
	strOutputString = strOutputString + _SSTR(" [");
	uint32 Size = pNode->iGetData().InstanceCount.size();
	for (uint32 seek = 0; seek < Size; seek = seek + 1)
	{
		strOutputString = strOutputString + this->m_ResultArray[seek] + _SSTR(":");
		strOutputString = strOutputString + ::gloIToS(pNode->iGetData().InstanceCount[seek]) + _SSTR("; ");
	}
	strOutputString = strOutputString + _SSTR("] Entropy: ") + ::gloFToS(pNode->iGetData().Extropy) + _SSTR("\r\n\r\n");
	if (!pNode->iIsLeaf())
	{
		uint32 NodeSize = pNode->iGetSize();
		for (uint32 seek = 0; seek < NodeSize; seek = seek + 1)
		{
			this->_PrintTree((*pNode)[seek], PrevString + _SSTR("\t"), strOutputString);
		}
	}
}
bool cDecisionTreeAlgorithm::_GenerateRuleArray(const lpDECISIONTREENODE pNode, RULE CurrentRule)
{
	if (pNode == nullptr)
	{
		return false;
	}
	if (pNode->iIsLeaf())
	{
		CurrentRule.second = pNode->iGetData().strName;
		this->m_RuleArray.push_back(CurrentRule);
		return true;
	}
	for (uint32 seek = 0; seek < pNode->iGetSize(); seek = seek + 1)
	{
		RULE SubRule = CurrentRule;
		SubRule.first.push_back(pNode->iGetData().strName);
		SubRule.first.push_back((this->m_InnerAttributeArray[pNode->iGetData().Index].ValueArray[seek]));
		/*SubRule.first[pNode->iGetData().Index * 2] = pNode->iGetData().strName;
		SubRule.first[pNode->iGetData().Index * 2 + 1] = (this->m_InnerAttributeArray[pNode->iGetData().Index].ValueArray[seek]);*/
		this->_GenerateRuleArray((*pNode)[seek], SubRule);
	}
}
bool cDecisionTreeAlgorithm::_RulePredict(const RULE &Rule, const INSTANCE Instance, SString &strResult) const
{
	for (uint32 seek = 0; seek < Rule.first.size(); seek = seek + 2)
	{
		if (Rule.first[seek].empty())
		{
			continue;
		}
		int32 Index = -1;
		for (uint32 seek_res = 0; seek_res < this->m_InnerAttributeArray.size(); seek_res = seek_res + 1)
		{
			if (this->m_InnerAttributeArray[seek_res].strName == Rule.first[seek])
			{
				Index = seek_res;
				break;
			}
		}
		/*if (Index == -1)
		{
			return false;
		}*/
		if (Rule.first[seek + 1] != Instance.first[Index])
		{
			return false;
		}
	}
	strResult = Rule.second;
	return true;
}
bool cDecisionTreeAlgorithm::_RuleCompare(const RULE &Rule1, const RULE &Rule2) const
{
	if (Rule1.first.size() != Rule2.first.size())
	{
		return false;
	}
	if (Rule1.second != Rule2.second)
	{
		return false;
	}
	for (uint32 seek = 0; seek < Rule1.first.size(); seek = seek + 1)
	{
		if (Rule1.first[seek] != Rule2.first[seek])
		{
			return false;
		}
	}
	return true;
}
bool cDecisionTreeAlgorithm::iCreateAttribute(const SString &strAttributeName, vector<SString> &ValueArray)
{
	if (strAttributeName.empty() || ValueArray.empty())
	{
		return false;
	}
	for (uint32 seek = 0; seek < this->m_OuterArributeArray.size(); seek = seek + 1)
	{
		if (this->m_OuterArributeArray[seek].strName == strAttributeName)
		{
			return false;
		}
	}
	ATTRIBUTE Attribute;
	Attribute.strName = strAttributeName;
	Attribute.ID = this->m_OuterArributeArray.size();
	Attribute.ValueArray = ValueArray;
	Attribute.bIsContinuesValue = false;
	Attribute.Boundary.clear();
	Attribute.bIsTaken = false;
	this->m_OuterArributeArray.push_back(Attribute);
	return true;
}
bool cDecisionTreeAlgorithm::iCreateAttribute(const SString &strAttributeName, vector<sfloat> &BoundaryArray)
{
	if (strAttributeName.empty() || BoundaryArray.empty())
	{
		return false;
	}
	for (uint32 seek = 0; seek < this->m_OuterArributeArray.size(); seek = seek + 1)
	{
		if (this->m_OuterArributeArray[seek].strName == strAttributeName)
		{
			return false;
		}
	}
	ATTRIBUTE Attribute;
	Attribute.strName = strAttributeName;
	Attribute.ID = this->m_OuterArributeArray.size();
	Attribute.ValueArray.clear();
	Attribute.bIsContinuesValue = true;
	Attribute.Boundary = BoundaryArray;
	Attribute.bIsTaken = false;
	this->m_OuterArributeArray.push_back(Attribute);
	return true;
}
void cDecisionTreeAlgorithm::iDeleteAttribute(const SString &strAttributeName)
{
	if (strAttributeName.empty())
	{
		return;
	}
	bool bDeleted = false;
	vector<ATTRIBUTE>::iterator pAttribute = this->m_OuterArributeArray.begin();
	while (pAttribute!=this->m_OuterArributeArray.end())
	{
		if (pAttribute->strName == strAttributeName)
		{
			pAttribute = this->m_OuterArributeArray.erase(pAttribute);
			bDeleted = true;
			continue;
		}
		if (bDeleted)
		{
			pAttribute->ID = pAttribute->ID - 1;
		}
		pAttribute++;
	}
}
void cDecisionTreeAlgorithm::iClearAttributeArray()
{
	this->m_OuterArributeArray.clear();
	this->m_InnerAttributeArray.clear();
}
bool cDecisionTreeAlgorithm::iUpdateResultAttribute(const SString &strResultName, vector<SString> &ValArray)
{
	if (ValArray.empty())
	{
		return false;
	}
	this->m_ResultArray = ValArray;
	return true;
}
bool cDecisionTreeAlgorithm::iGenerateTree(const vector<INSTANCE> &InstanceSpace)
{
	if (this->m_OuterArributeArray.empty() || InstanceSpace.empty() || this->m_ResultArray.empty())
	{
		return false;
	}

	if (this->m_pTreeRoot != nullptr)
	{
		this->m_pTreeRoot->iClear();
		delete this->m_pTreeRoot;
	}
	ATTRIBUTENODE Node;
	this->m_pTreeRoot = new TREE<ATTRIBUTENODE>(0,Node,nullptr);

	this->m_InnerAttributeArray.clear();

	vector<INSTANCE> InnerInstanceSpace;

	/*Geneate the inner attribute space - devide the continues attribute to several indivual attributes*/
	uint32 CurrentIndex = 0;
	for (uint32 seek = 0; seek < this->m_OuterArributeArray.size(); seek = seek + 1)
	{
		vector<ATTRIBUTE> AttributeArray = this->_GenerateInnerAttribute(this->m_OuterArributeArray[seek], InstanceSpace);
		for (uint32 seek_attribute = 0; seek_attribute < AttributeArray.size(); seek_attribute = seek_attribute + 1)
		{
			this->m_InnerAttributeArray.push_back(AttributeArray[seek_attribute]);
			this->m_InnerAttributeArray[CurrentIndex + seek_attribute].ID = CurrentIndex + seek_attribute;
		}
		CurrentIndex = CurrentIndex + AttributeArray.size();
	}

	/*Calculate the probabilaty of unknown attribute and devide the hole instance into several instances with defferent weight*/
	//this->m_InnerInstanceSpace = this->_CalcUnknownInstance(this->m_OuterInstanceSpace);

	/*Generate the inner instance space - classify the continues attribute to several true - false binary based attributes*/
	INSTANCE Instance;
	for (uint32 seek = 0; seek < InstanceSpace.size(); seek = seek + 1)
	{
		Instance = this->_GenerateInnerInstance(InstanceSpace[seek]);
		InnerInstanceSpace.push_back(Instance);
	}

	/*Generate the actual decision tree with modified instance space*/
	vector<uint32> InstanceCount(this->m_ResultArray.size());
	return this->_GenerateDecisionTree(InnerInstanceSpace, this->m_InnerAttributeArray,this->m_pTreeRoot);
}
bool cDecisionTreeAlgorithm::iGenerateRule()
{
	RULE Rule;
	/*Rule.first.resize(this->m_InnerAttributeArray.size() * 2);
	for (uint32 seek = 0; seek < (this->m_InnerAttributeArray.size() * 2); seek = seek + 1)
	{
		Rule.first[seek].clear();
	}*/
	this->m_RuleArray.clear();
	return this->_GenerateRuleArray(this->m_pTreeRoot, Rule);;
}
bool cDecisionTreeAlgorithm::iPruningTree(const vector<INSTANCE> &InstanceSpace)
{
	if (this->m_RuleArray.empty() || InstanceSpace.empty() || this->m_ResultArray.empty()||this->m_InnerAttributeArray.empty())
	{
		return false;
	}

	sfloat DecisionTreeWeight = InstanceSpace.size();
	sfloat RightCount = 0.0;
	for (uint32 seek = 0; seek < InstanceSpace.size(); seek = seek + 1)
	{
		if (InstanceSpace[seek].first.back() == this->iPrediceByRuleSet(InstanceSpace[seek]))
		{
			RightCount = RightCount + 1.0;
		}
	}
	DecisionTreeWeight = RightCount / DecisionTreeWeight;

	vector<INSTANCE> InnerInstanceSpace;

	/*Calculate the probabilaty of unknown attribute and devide the hole instance into several instances with defferent weight*/
	//this->m_InnerInstanceSpace = this->_CalcUnknownInstance(this->m_OuterInstanceSpace);

	/*Generate the inner instance space - classify the continues attribute to several true - false binary based attributes*/
	INSTANCE Instance;
	for (uint32 seek = 0; seek < InstanceSpace.size(); seek = seek + 1)
	{
		Instance = this->_GenerateInnerInstance(InstanceSpace[seek]);
		InnerInstanceSpace.push_back(Instance);
	}

	/*Post - pruning the rule space by the validation space*/
	vector<uint32> InstanceCount(this->m_ResultArray.size());

	uint32 RuleArraySize = this->m_RuleArray.size();
	uint32 InstanceSize = InnerInstanceSpace.size();

	/*For sort*/
	vector<sfloat> AccuracyArray;
	AccuracyArray.resize(this->m_RuleArray.size());

	/*Generate the rule space*/
	for (uint32 seek = 0; seek < RuleArraySize; seek = seek + 1)
	{
		for (int32 seek_attrib = this->m_RuleArray[seek].first.size() - 2; seek_attrib >= 0; seek_attrib = seek_attrib - 2)
		{
			if (this->m_RuleArray[seek].first[seek_attrib].empty())
			{
				continue;
			}
			RULE CurrentRule = this->m_RuleArray[seek];
			/*Calculate the previous accuracy*/
			sfloat TotalCount = 0.0;
			sfloat RightCount = 0.0;
			sfloat PrevAccuracy = 0.0;
			for (uint32 seek_instance = 0; seek_instance < InstanceSize; seek_instance = seek_instance + 1)
			{
				SString strResult;
				if (this->_RulePredict(CurrentRule, InnerInstanceSpace[seek_instance], strResult))
				{
					TotalCount = TotalCount + 1.0;
					if (strResult == InnerInstanceSpace[seek_instance].first.back())
					{
						RightCount = RightCount + 1.0;
					}
				}
			}
			if (TotalCount == 0.0)
			{
				PrevAccuracy = DecisionTreeWeight;
			}
			else
			{
				PrevAccuracy = RightCount / TotalCount;
			}
			AccuracyArray[seek] = PrevAccuracy;
			//::cout << "---" << PrevAccuracy;

			/*Remove attribute*/
			vector<SString>::iterator pSubAttribute = CurrentRule.first.begin();
			for (uint32 seek_sub_attrib = 0; seek_sub_attrib < seek_attrib; seek_sub_attrib = seek_sub_attrib + 1)
			{
				pSubAttribute++;
			}
			//pSubAttribute = CurrentRule.first.erase(pSubAttribute);
			//pSubAttribute = CurrentRule.first.erase(pSubAttribute);
			CurrentRule.first[seek_attrib].clear();
			CurrentRule.first[seek_attrib + 1].clear();

			/*Caluculate the current accuracy*/
			TotalCount = 0.0;
			RightCount = 0.0;
			sfloat CurrentAccuriry = 0.0;
			for (uint32 seek_instance = 0; seek_instance < InstanceSize; seek_instance = seek_instance + 1)
			{
				SString strResult;
				if (this->_RulePredict(CurrentRule, InnerInstanceSpace[seek_instance], strResult))
				{
					TotalCount = TotalCount + 1.0;
					if (strResult == InnerInstanceSpace[seek_instance].first.back())
					{
						RightCount = RightCount + 1.0;
					}
				}
			}
			if (TotalCount == 0.0)
			{
				continue;
			}
			CurrentAccuriry = RightCount / TotalCount;
			//::cout << "---" << PrevAccuracy << " " << CurrentAccuriry << "\n";
			if (CurrentAccuriry > PrevAccuracy)
			{
				this->m_RuleArray[seek] = CurrentRule;
				AccuracyArray[seek] = CurrentAccuriry;
			}
		}
	}
	/*Sort*/
	sfloat AccBuffer;
	RULE RuleBuffer;
	for (uint32 seek = 0; seek < RuleArraySize; seek = seek + 1)
	{
		int32 MaxIndex = -1;
		for (uint32 seek_max = seek; seek_max < RuleArraySize; seek_max = seek_max + 1)
		{
			if (AccuracyArray[seek] < AccuracyArray[seek_max])
			{
				MaxIndex = seek_max;
			}
		}
		if (MaxIndex == -1)
		{
			continue;
		}
		AccBuffer = AccuracyArray[seek];
		RuleBuffer = this->m_RuleArray[seek];
		AccuracyArray[seek] = AccuracyArray[MaxIndex];
		this->m_RuleArray[seek] = this->m_RuleArray[MaxIndex];
		AccuracyArray[MaxIndex] = AccBuffer;
		this->m_RuleArray[MaxIndex] = RuleBuffer;
	}
	return true;
}
SString cDecisionTreeAlgorithm::iPredict(const INSTANCE &Instance) const
{
	if (this->m_pTreeRoot == nullptr)
	{
		return _SSTR("ERROR: DECISION TREE IS EMPTY");
	}
	if (this->m_InnerAttributeArray.empty())
	{
		return _SSTR("ERROR: INNER ATTRIBUTE ARRAY IS EMPTY");
	}
	if ((Instance.first.size() - 1) != (this->m_OuterArributeArray.size()))
	{
		return _SSTR("ERROR: INSTANCE SIZE IS MISMATCH");
	}
	INSTANCE InnerInstance;
	InnerInstance = this->_GenerateInnerInstance(Instance);
	return this->_SearchDecisionTree(InnerInstance, this->m_pTreeRoot);
}
SString cDecisionTreeAlgorithm::iPrediceByRuleSet(const INSTANCE &Instance) const
{
	if (this->m_RuleArray.empty())
	{
		return _SSTR("ERROR: DECISION TREE IS EMPTY");
	}
	if (this->m_InnerAttributeArray.empty())
	{
		return _SSTR("ERROR: INNER ATTRIBUTE ARRAY IS EMPTY");
	}
	if ((Instance.first.size() - 1) != (this->m_OuterArributeArray.size()))
	{
		return _SSTR("ERROR: INSTANCE SIZE IS MISMATCH");
	}
	INSTANCE InnerInstance;
	InnerInstance = this->_GenerateInnerInstance(Instance);
	for (uint32 seek = 0; seek < this->m_RuleArray.size(); seek = seek + 1)
	{
		SString strResult;
		if (this->_RulePredict(this->m_RuleArray[seek], InnerInstance, strResult))
		{
			return strResult;
		}
	}
	return _SSTR("");
}
SString cDecisionTreeAlgorithm::iPrintTree() const
{
	SString strOutputString;
	this->_PrintTree(this->m_pTreeRoot, _SSTR(""), strOutputString);
	return strOutputString;
}
SString cDecisionTreeAlgorithm::iPrintInnerAttribute() const
{
	SString strOutputString;
	for (uint32 seek = 0; seek < this->m_InnerAttributeArray.size(); seek = seek + 1)
	{
		strOutputString = strOutputString + this->m_InnerAttributeArray[seek].strName + _SSTR("\r\n");
	}
	return strOutputString;
}
SString cDecisionTreeAlgorithm::iPrintRule() const
{
	SString strOutputString;
	for (uint32 seek = 0; seek < this->m_RuleArray.size(); seek = seek + 1)
	{
		SString strString = _SSTR("<");
		for (uint32 seek_val = 0; seek_val < this->m_RuleArray[seek].first.size(); seek_val = seek_val + 2)
		{
			if (this->m_RuleArray[seek].first[seek_val].empty())
			{
				continue;
			}
			strString = strString + _SSTR(" ") + this->m_RuleArray[seek].first[seek_val] + _SSTR(" = ") + this->m_RuleArray[seek].first[seek_val + 1] + _SSTR(", ");
		}
		strString = strString + _SSTR("| ") + this->m_RuleArray[seek].second + _SSTR(" >\r\n");
		strOutputString = strOutputString + strString;
	}
	return strOutputString;
}