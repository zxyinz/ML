#include"cGARuleSet.h"
using namespace std;
using namespace San;
using namespace San::MachineLearning;
cGARuleSet::cGARuleSet()
	:cGeneticAlgorithmHypothesis<SString, SString, GARULEDESC>(GAHT_RULESET)
{
}
cGARuleSet::~cGARuleSet()
{
}
bool cGARuleSet::_CrossoverSP(const cGeneticAlgorithmHypothesis<SString, SString, GARULEDESC> &H1, const cGeneticAlgorithmHypothesis<SString, SString, GARULEDESC> &H2, vector<cGeneticAlgorithmHypothesis<SString, SString, GARULEDESC>*> &OffspringSet)
{
	if (H1.iGetType() != H2.iGetType())
	{
		return false;
	}

	cGARuleSet* pH1 = (cGARuleSet*) &H1;
	cGARuleSet* pH2 = (cGARuleSet*) &H2;

	uint32 H1Index = ::rand() % pH1->m_Desc.RuleSetSize;
	uint32 H2Index = ::rand() % pH2->m_Desc.RuleSetSize;
	uint32 Offset = ::rand() % pH1->m_Desc.AttributeLength;

	if (OffspringSet[0] == nullptr)
	{
		OffspringSet[0] = (cGeneticAlgorithmHypothesis<SString, SString, GARULEDESC>*)new cGARuleSet();
	}
	if (OffspringSet[1] == nullptr)
	{
		OffspringSet[1] = (cGeneticAlgorithmHypothesis<SString, SString, GARULEDESC>*)new cGARuleSet();
	}

	cGARuleSet* pO1 = (cGARuleSet*) OffspringSet[0];
	cGARuleSet* pO2 = (cGARuleSet*) OffspringSet[1];

	pO1->m_Desc = pH1->m_Desc;
	pO2->m_Desc = pH1->m_Desc;

	pO1->m_Desc.RuleSetSize = H1Index + pH2->m_Desc.RuleSetSize - H2Index;
	pO2->m_Desc.RuleSetSize = H2Index + pH1->m_Desc.RuleSetSize - H1Index;

	pO1->m_RuleSet.iReSizeStream(pO1->m_Desc.RuleSetSize*pO1->m_Desc.PerRuleLength);
	pO2->m_RuleSet.iReSizeStream(pO2->m_Desc.RuleSetSize*pO2->m_Desc.PerRuleLength);

	H1Index = H1Index*pH1->m_Desc.PerRuleLength + Offset;
	H2Index = H2Index*pH1->m_Desc.PerRuleLength + Offset;

	pO1->m_RuleSet.iSetStream(0, pH1->m_RuleSet.iGetPtr(), H1Index);
	pO1->m_RuleSet.iSetStream(H1Index, pH2->m_RuleSet.iGetPtr() + H2Index, pH2->m_RuleSet.iGetSize() - H2Index);

	pO2->m_RuleSet.iSetStream(0, pH2->m_RuleSet.iGetPtr(), H2Index);
	pO2->m_RuleSet.iSetStream(H2Index, pH1->m_RuleSet.iGetPtr() + H1Index, pH1->m_RuleSet.iGetSize() - H1Index);

	pO1->_CalcDefaultPredict();
	pO2->_CalcDefaultPredict();

	//::wcout << pO1->iPrintDebugInformtion().c_str();
	//::wcout << pO2->iPrintDebugInformtion().c_str();

	if (pH1->_InvaildCheck(*pO1, pO1->m_RuleSet) && pH1->_InvaildCheck(*pO2, pO2->m_RuleSet))
	{
		return true;
	}
	return false;
}
bool cGARuleSet::_InvaildCheck(const cGARuleSet &Hypothesis, const SANBITSTREAM &RuleSet)
{
	for (uint32 seek = 0; seek <Hypothesis.m_Desc.RuleSetSize; seek = seek + 1)
	{
		uint32 AttribOffset = 0;
		uint32 Counter = 0;
		for (uint32 seek_attrib = 0; seek_attrib < Hypothesis.m_Desc.AttributeArray.size(); seek_attrib = seek_attrib + 1)
		{
			Counter = 0;
			for (uint32 seek_val = 0; seek_val < Hypothesis.m_Desc.AttributeArray[seek_attrib].second.size(); seek_val = seek_val + 1)
			{
				if (RuleSet[seek*Hypothesis.m_Desc.PerRuleLength + AttribOffset + seek_val] == false)
				{
					Counter = Counter + 1;
				}
			}
			if (Counter == Hypothesis.m_Desc.AttributeArray[seek_attrib].second.size())
			{
				return false;
			}
			AttribOffset = AttribOffset + Hypothesis.m_Desc.AttributeArray[seek_attrib].second.size();
		}
		Counter = 0;
		for (uint32 seek_res = 0; seek_res < Hypothesis.m_Desc.ResultArray.size(); seek_res = seek_res + 1)
		{
			Counter = Counter + (uint32) (RuleSet[seek*Hypothesis.m_Desc.PerRuleLength + AttribOffset + seek_res]);
		}
		if (Counter != 1)
		{
			return false;
		}
	}
	return true;
}
bool cGARuleSet::_PredictByOneRule(const uint32 InstanceIndex, const uint32 RuleIndex, const GAINSTANCESET<SString, SString> &InstanceSpace, SString &Result)
{
	uint32 AttribIndex = 0;
	for (uint32 seek_attrib = 0; seek_attrib < this->m_Desc.AttributeArray.size(); seek_attrib = seek_attrib + 1)
	{
		for (uint32 seek_val = 0; seek_val < this->m_Desc.AttributeArray[seek_attrib].second.size(); seek_val = seek_val + 1)
		{
			if (InstanceSpace[InstanceIndex].first[seek_attrib] == this->m_Desc.AttributeArray[seek_attrib].second[seek_val])
			{
				if (this->m_RuleSet[RuleIndex + AttribIndex + seek_val] == false)
				{
					return false;
				}
				break;
			}
		}
		AttribIndex = AttribIndex + this->m_Desc.AttributeArray[seek_attrib].second.size();
	}
	for (uint32 seek_res = 0; seek_res < this->m_Desc.ResultArray.size(); seek_res = seek_res + 1)
	{
		if (this->m_RuleSet[RuleIndex + this->m_Desc.AttributeLength + seek_res] == true)
		{
			Result = this->m_Desc.ResultArray[seek_res];
			return true;
		}
	}
	return false;
}
void cGARuleSet::_CalcDefaultPredict()
{
	vector<uint32> Count(this->m_Desc.ResultArray.size());
	for (uint32 seek = 0; seek < this->m_Desc.ResultArray.size(); seek = seek + 1)
	{
		Count[seek] = 0;
	}
	for (uint32 seek = 0; seek < this->m_Desc.RuleSetSize; seek = seek + 1)
	{
		uint32 Index = seek*this->m_Desc.PerRuleLength + this->m_Desc.AttributeLength;
		for (uint32 seek_res = 0; seek_res < this->m_Desc.ResultArray.size(); seek_res = seek_res + 1)
		{
			Count[seek_res] = Count[seek_res] + (uint8)this->m_RuleSet[Index + seek_res];
		}
	}
	uint32 MaxIndex = 0;
	for (uint32 seek = 0; seek < this->m_Desc.ResultArray.size(); seek = seek + 1)
	{
		if (Count[MaxIndex] < Count[seek])
		{
			MaxIndex = seek;
		}
	}
	this->m_Desc.DefaultPredict = this->m_Desc.ResultArray[MaxIndex];
}
bool cGARuleSet::iInitialize(const uint32 Index, const GARULEDESC &Desc, const GAINSTANCESET<SString, SString> &InstanceSpace)
{
	if (Desc.RuleSetSize == 0)
	{
		return false;
	}
	if (Desc.PerRuleLength < 2)
	{
		return false;
	}
	if (Desc.AttributeArray.empty() || Desc.ResultArray.empty())
	{
		return false;
	}
	this->m_Desc = Desc;
	this->m_Desc.PerRuleLength = 0;
	for (uint32 seek = 0; seek < this->m_Desc.AttributeArray.size(); seek = seek + 1)
	{
		this->m_Desc.PerRuleLength = this->m_Desc.PerRuleLength + this->m_Desc.AttributeArray[seek].second.size();
	}
	this->m_Desc.AttributeLength = this->m_Desc.PerRuleLength;
	this->m_Desc.PerRuleLength = this->m_Desc.PerRuleLength + this->m_Desc.ResultArray.size();
	this->m_RuleSet.iReSizeStream(this->m_Desc.RuleSetSize*this->m_Desc.PerRuleLength);
	this->m_RuleSet.iSetStream(false);

	for (uint32 seek = 0; seek < this->m_Desc.RuleSetSize; seek = seek + 1)
	{
		uint32 Offset = seek*this->m_Desc.PerRuleLength;
		uint32 AttribOffset = 0;
		if ((Index*this->m_Desc.RuleSetSize + seek) < InstanceSpace.size())
		{
			/*From training set*/
			for (uint32 seek_attrib = 0; seek_attrib < this->m_Desc.AttributeArray.size(); seek_attrib = seek_attrib + 1)
			{
				uint32 Val = 0;
				for (uint32 seek_val = 0; seek_val < this->m_Desc.AttributeArray[seek_attrib].second.size(); seek_val = seek_val + 1)
				{
					if (InstanceSpace[seek].first[seek_attrib] == this->m_Desc.AttributeArray[seek_attrib].second[seek_val])
					{
						Val = seek_val;
						break;
					}
				}
				this->m_RuleSet[Offset + AttribOffset + Val] = true;
				AttribOffset = AttribOffset + this->m_Desc.AttributeArray[seek_attrib].second.size();
			}
			uint32 Res = 0;
			for (uint32 seek_res = 0; seek_res < this->m_Desc.ResultArray.size(); seek_res = seek_res + 1)
			{
				if (InstanceSpace[seek].second[0] == this->m_Desc.ResultArray[seek_res])
				{
					Res = seek_res;
					break;
				}
			}
			this->m_RuleSet[Offset + AttribOffset + Res] = true;
		}
		else
		{
			/*Random*/
			for (uint32 seek_attrib = 0; seek_attrib < this->m_Desc.AttributeArray.size(); seek_attrib = seek_attrib + 1)
			{
				uint32 Val = ::rand() % this->m_Desc.AttributeArray[seek_attrib].second.size();
				this->m_RuleSet[Offset + AttribOffset + Val] = true;
				AttribOffset = AttribOffset + this->m_Desc.AttributeArray[seek_attrib].second.size();
			}
			uint32 Res = ::rand() % this->m_Desc.ResultArray.size();
			this->m_RuleSet[Offset + AttribOffset + Res] = true;
		}
	}
}
void cGARuleSet::iRelease()
{
	this->m_Desc.RuleSetSize = 0;
	this->m_Desc.PerRuleLength = 0;
	this->m_Desc.AttributeArray.clear();
	this->m_Desc.ResultArray.clear();
	this->m_RuleSet.iReSizeStream(0);
}
bool cGARuleSet::iPredict(const GAINSTANCESET<SString, SString> &InstanceSpace, vector<SString> &Result)
{
	Result.resize(InstanceSpace.size());
	for (uint32 seek = 0; seek < InstanceSpace.size(); seek = seek + 1)
	{
		bool bFitRule = false;
		for (uint32 seek_rule = 0; seek_rule < this->m_Desc.RuleSetSize; seek_rule = seek_rule + 1)
		{
			uint32 RuleIndex = seek_rule*this->m_Desc.PerRuleLength;
			SString Res;
			if (this->_PredictByOneRule(seek, RuleIndex, InstanceSpace, Res))
			{
				bFitRule = true;
				Result[seek] = Res;
				break;
			}
		}
		if (!bFitRule)
		{
			Result[seek] = this->m_Desc.DefaultPredict;
		}
	}
	return true;
}
bool cGARuleSet::iTraining(const GAINSTANCESET<SString, SString> &TrainingSet)
{
	return true;
}
bool cGARuleSet::iCrossoverSP(const cGeneticAlgorithmHypothesis<SString, SString, GARULEDESC>* pHypothesis, vector<cGeneticAlgorithmHypothesis<SString, SString, GARULEDESC>*> &OffspringSet)
{
	return this->_CrossoverSP(*this, *pHypothesis, OffspringSet);
}
bool cGARuleSet::iCrossoverTP(const cGeneticAlgorithmHypothesis<SString, SString, GARULEDESC>* pHypothesis, vector<cGeneticAlgorithmHypothesis<SString, SString, GARULEDESC>*> &OffspringSet)
{
	return false;
}
bool cGARuleSet::iCrossoverUNI(const cGeneticAlgorithmHypothesis<SString, SString, GARULEDESC>* pHypothesis, vector<cGeneticAlgorithmHypothesis<SString, SString, GARULEDESC>*> &OffspringSet)
{
	return false;
}
bool cGARuleSet::iMutation()
{
	uint32 Index = ::rand() % (this->m_Desc.RuleSetSize*this->m_Desc.PerRuleLength);
	uint32 HIndex = Index / this->m_Desc.PerRuleLength;
	uint32 Offset = Index%this->m_Desc.PerRuleLength;
	if (Offset >= this->m_Desc.AttributeLength)
	{
		uint32 Res = 0;
		for (uint32 seek = this->m_Desc.AttributeLength; seek < this->m_Desc.PerRuleLength; seek = seek + 1)
		{
			if (this->m_RuleSet[HIndex*this->m_Desc.PerRuleLength + seek] == true)
			{
				Res = seek;
				this->m_RuleSet[HIndex*this->m_Desc.PerRuleLength + seek] = false;
				break;
			}
		}
		uint32 MutationRes = 0;
		while (true)
		{
			MutationRes = ::rand() % this->m_Desc.ResultArray.size() + this->m_Desc.AttributeLength;
			if (Res != MutationRes)
			{
				break;
			}
		}
		this->m_RuleSet[HIndex*this->m_Desc.PerRuleLength + MutationRes] = true;
		return true;
	}
	uint32 AttribOffset = 0;
	for (uint32 seek = 0; seek < this->m_Desc.AttributeArray.size(); seek = seek + 1)
	{
		if (Offset < (AttribOffset + this->m_Desc.AttributeArray[seek].second.size()))
		{
			Offset = Offset - AttribOffset;
			while (true)
			{
				bool bTrue = false;
				uint32 CurrentIndex = HIndex*this->m_Desc.PerRuleLength + AttribOffset + Offset;
				this->m_RuleSet[CurrentIndex] = !this->m_RuleSet[CurrentIndex];
				for (uint32 seek_val = 0; seek_val < this->m_Desc.AttributeArray[seek].second.size();seek_val=seek_val+1)
				{
					if (this->m_RuleSet[HIndex*this->m_Desc.PerRuleLength + AttribOffset + seek_val] == true)
					{
						bTrue = true;
					}
				}
				if (bTrue)
				{
					break;
				}
				this->m_RuleSet[CurrentIndex] = !this->m_RuleSet[CurrentIndex];
				Offset = ::rand() % this->m_Desc.AttributeArray[seek].second.size();
			}
			break;
		}
		AttribOffset = AttribOffset + this->m_Desc.AttributeArray[seek].second.size();
	}
	return true;
}
sfloat cGARuleSet::iEvaluate(const GAINSTANCESET<SString, SString> &InstanceSpace)
{
	sfloat SpaceSize = InstanceSpace.size();
	vector<sfloat> Accuracy(this->m_Desc.RuleSetSize);
	for (uint32 seek = 0; seek < this->m_Desc.RuleSetSize; seek = seek + 1)
	{
		Accuracy[seek] = 0.0;
	}
	for (uint32 seek = 0; seek < this->m_Desc.RuleSetSize; seek = seek + 1)
	{
		for (uint32 seek_ins = 0; seek_ins < SpaceSize; seek_ins = seek_ins + 1)
		{
			SString Res;
			uint32 Index = seek*this->m_Desc.PerRuleLength;
			if (this->_PredictByOneRule(seek_ins, Index, InstanceSpace, Res))
			{
				if (Res == InstanceSpace[seek_ins].second[0])
				{
					Accuracy[seek] = Accuracy[seek] + 1.0;
				}
			}
		}
	}

	/*Sort*/

	/*Evaluate*/
	sfloat Fitness = 0.0;
	vector<SString> ResList;
	this->iPredict(InstanceSpace, ResList);
	for (uint32 seek = 0; seek < ResList.size(); seek = seek + 1)
	{
		if (ResList[seek] == InstanceSpace[seek].second[0])
		{
			Fitness = Fitness + 1.0;
		}
	}
	Fitness = 0.01 + (Fitness / SpaceSize)*0.99;
	if (this->m_Desc.RuleSetSize > InstanceSpace.size())
	{
		sfloat Rate = this->m_Desc.RuleSetSize - InstanceSpace.size();
		Rate = Rate <= 1.0 ? 1.0 : Rate;
		Fitness = Fitness / sqrt(sqrt(Rate));
	}
	/*else
	{
		Fitness = Fitness - (sfloat)this->m_Desc.RuleSetSize / (sfloat) InstanceSpace.size();
	}*/
	return Fitness;
}
sfloat cGARuleSet::iAccuracy(const GAINSTANCESET<SString, SString> &InstanceSpace)
{
	sfloat SpaceSize = InstanceSpace.size();
	sfloat Accuracy = 0.0;
	vector<SString> ResList;
	this->iPredict(InstanceSpace, ResList);
	for (uint32 seek = 0; seek < ResList.size(); seek = seek + 1)
	{
		if (ResList[seek] == InstanceSpace[seek].second[0])
		{
			Accuracy = Accuracy + 1.0;
		}
	}
	Accuracy = Accuracy / SpaceSize;
	return Accuracy;
}
SString cGARuleSet::iPrintHypothesisSpace() const
{
	SString strOutput=_SSTR("\r\n");
	for (uint32 seek = 0; seek < this->m_Desc.RuleSetSize; seek = seek + 1)
	{
		strOutput = strOutput + _SSTR("< ");
		uint32 AttribOffset = 0;
		for (uint32 seek_attrib = 0; seek_attrib < this->m_Desc.AttributeArray.size(); seek_attrib = seek_attrib + 1)
		{
			strOutput = strOutput + this->m_Desc.AttributeArray[seek_attrib].first + _SSTR(" = ");
			for (uint32 seek_val = 0; seek_val < this->m_Desc.AttributeArray[seek_attrib].second.size(); seek_val = seek_val + 1)
			{
				uint32 Index = seek*this->m_Desc.PerRuleLength + AttribOffset + seek_val;
				if (this->m_RuleSet[Index])
				{
					strOutput = strOutput + this->m_Desc.AttributeArray[seek_attrib].second[seek_val] + _SSTR(" U ");
				}
			}
			strOutput[strOutput.size() - 2] = _SSTR(',');
			//strOutput[strOutput.size() - 2] = _SSTR(' ');
			AttribOffset = AttribOffset + this->m_Desc.AttributeArray[seek_attrib].second.size();
		}
		strOutput[strOutput.size() - 2] = _SSTR('|');
		for (uint32 seek_res = 0; seek_res < this->m_Desc.ResultArray.size(); seek_res = seek_res + 1)
		{
			uint32 Index = seek*this->m_Desc.PerRuleLength + AttribOffset + seek_res;
			if (this->m_RuleSet[Index])
			{
				strOutput = strOutput + this->m_Desc.ResultArray[seek_res];
			}
		}
		strOutput = strOutput + _SSTR(" >\r\n");
	}
	strOutput = strOutput + _SSTR("\r\n\r\n");
	strOutput = strOutput + _SSTR("Default Predict: ") + this->m_Desc.DefaultPredict;
	strOutput = strOutput + _SSTR("\r\n\r\n");
	return strOutput;
}
SString cGARuleSet::iPrintDebugInformtion() const
{
	SString strOutput;
	for (uint32 seek = 0; seek < this->m_Desc.RuleSetSize; seek = seek + 1)
	{
		uint32 AttribOffset = 0;
		for (uint32 seek_attrib = 0; seek_attrib < this->m_Desc.AttributeArray.size(); seek_attrib = seek_attrib + 1)
		{
			for (uint32 seek_val = 0; seek_val < this->m_Desc.AttributeArray[seek_attrib].second.size(); seek_val = seek_val + 1)
			{
				uint32 Index = seek*this->m_Desc.PerRuleLength + AttribOffset + seek_val;
				if (this->m_RuleSet[Index])
				{
					strOutput = strOutput + _SSTR("1");
				}
				else
				{
					strOutput = strOutput + _SSTR("0");
				}
			}
			strOutput = strOutput + _SSTR(" ");
			AttribOffset = AttribOffset + this->m_Desc.AttributeArray[seek_attrib].second.size();
		}
		strOutput = strOutput + _SSTR("| ");
		for (uint32 seek_res = 0; seek_res < this->m_Desc.ResultArray.size(); seek_res = seek_res + 1)
		{
			uint32 Index = seek*this->m_Desc.PerRuleLength + AttribOffset + seek_res;
			if (this->m_RuleSet[Index])
			{
				strOutput = strOutput + _SSTR("1");
			}
			else
			{
				strOutput = strOutput + _SSTR("0");
			}
		}
		strOutput = strOutput + _SSTR("\n");
	}
	strOutput = strOutput + _SSTR("\n\n");
	return strOutput;
}
sfloat cGARuleSet::iGetHypothesisSize() const
{
	return this->m_Desc.RuleSetSize;
}