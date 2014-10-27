#include"cGAContinueRuleSet.h"
using namespace std;
using namespace San;
using namespace San::MachineLearning;
cGAContinueRuleSet::cGAContinueRuleSet()
	:cGeneticAlgorithmHypothesis<SString, SString, GACONTINUERULEDESC>(GAHT_RULESET_CONTINUE)
{
}
cGAContinueRuleSet::~cGAContinueRuleSet()
{
}
bool cGAContinueRuleSet::_CrossoverSP(const cGeneticAlgorithmHypothesis<SString, SString, GACONTINUERULEDESC> &H1, const cGeneticAlgorithmHypothesis<SString, SString, GACONTINUERULEDESC> &H2, vector<cGeneticAlgorithmHypothesis<SString, SString, GACONTINUERULEDESC>*> &OffspringSet)
{
	if (H1.iGetType() != H2.iGetType())
	{
		return false;
	}

	cGAContinueRuleSet* pH1 = (cGAContinueRuleSet*) &H1;
	cGAContinueRuleSet* pH2 = (cGAContinueRuleSet*) &H2;

	uint32 H1Index = ::rand() % pH1->m_Desc.RuleSetSize;
	uint32 H2Index = ::rand() % pH2->m_Desc.RuleSetSize;
	uint32 Offset = ::rand() % pH1->m_Desc.AttributeLength;

	if (OffspringSet[0] == nullptr)
	{
		OffspringSet[0] = (cGeneticAlgorithmHypothesis<SString, SString, GACONTINUERULEDESC>*)new cGAContinueRuleSet();
	}
	if (OffspringSet[1] == nullptr)
	{
		OffspringSet[1] = (cGeneticAlgorithmHypothesis<SString, SString, GACONTINUERULEDESC>*)new cGAContinueRuleSet();
	}

	cGAContinueRuleSet* pO1 = (cGAContinueRuleSet*) OffspringSet[0];
	cGAContinueRuleSet* pO2 = (cGAContinueRuleSet*) OffspringSet[1];

	pO1->m_Desc = pH1->m_Desc;
	pO2->m_Desc = pH1->m_Desc;

	pO1->m_Desc.RuleSetSize = H1Index + pH2->m_Desc.RuleSetSize - H2Index;
	pO2->m_Desc.RuleSetSize = H2Index + pH1->m_Desc.RuleSetSize - H1Index;

	pO1->m_RuleSet.iReSizeStream(pO1->m_Desc.RuleSetSize*pO1->m_Desc.PerRuleLength);
	pO2->m_RuleSet.iReSizeStream(pO2->m_Desc.RuleSetSize*pO2->m_Desc.PerRuleLength);

	H1Index = H1Index*pH1->m_Desc.PerRuleLength + Offset;
	H2Index = H2Index*pH1->m_Desc.PerRuleLength + Offset;

	pO1->m_RuleSet.iSetStream(0, pH1->m_RuleSet.pStream, H1Index);
	pO1->m_RuleSet.iSetStream(H1Index, pH2->m_RuleSet.pStream + H2Index, pH2->m_RuleSet.Size - H2Index);

	pO2->m_RuleSet.iSetStream(0, pH2->m_RuleSet.pStream, H2Index);
	pO2->m_RuleSet.iSetStream(H2Index, pH1->m_RuleSet.pStream + H1Index, pH1->m_RuleSet.Size - H1Index);

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
bool cGAContinueRuleSet::_InvaildCheck(const cGAContinueRuleSet &Hypothesis, const SANBITSTREAM &RuleSet)
{
	for (uint32 seek = 0; seek <Hypothesis.m_Desc.RuleSetSize; seek = seek + 1)
	{
		uint32 Counter = 0;
		uint32 Index = seek*Hypothesis.m_Desc.PerRuleLength + Hypothesis.m_Desc.AttributeLength;
		for (uint32 seek_res = 0; seek_res < Hypothesis.m_Desc.ResultArray.size(); seek_res = seek_res + 1)
		{
			Counter = Counter + (uint32) (RuleSet[Index + seek_res]);
		}
		if (Counter != 1)
		{
			return false;
		}
	}
	return true;
}
bool cGAContinueRuleSet::_PredictByOneRule(const uint32 InstanceIndex, const uint32 RuleIndex, const GAINSTANCESET<SString, SString> &InstanceSpace, SString &Result)
{
	uint32 AttribIndex = 0;
	for (uint32 seek_attrib = 0; seek_attrib < this->m_Desc.AttributeArray.size(); seek_attrib = seek_attrib + 1)
	{
		uint8 Operator = 0;
		uint8 Data[2];
		this->m_RuleSet.iGetBitStream(AttribIndex, Operator, 0, 2);
		this->m_RuleSet.iGetBitStream(AttribIndex + 2, Data[0], 0, 8);
		this->m_RuleSet.iGetBitStream(AttribIndex + 10, Data[1], 0, 8);

		uint8 IData;
		if (Data[0] > Data[1])
		{
			IData = Data[1];
			Data[1] = Data[0];
			Data[0] = IData;
		}

		uint8 AttribVal = this->_ConvertFloatToInt(::gloSToF(InstanceSpace[InstanceIndex].first[seek_attrib]));
		switch (Operator)
		{
		case 0:
			// All data acceptable
			break;
		case 1:
			// Small or equal than Val <= Num
			if (AttribVal > Data[1])
			{
				return false;
			}
			break;
		case 2: 
			// Greater or equal than Num <= Val
			if (AttribVal < Data[0])
			{
				return false;
			}
			break;
		case 3: 
			// Range Num1 <= Val <= Num2
			if ((AttribVal<Data[0]) || (AttribVal>Data[1]))
			{
				return false;
			}
			break;
		default:
			return false;
			break;
		}
		AttribIndex = AttribIndex + 18;
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
void cGAContinueRuleSet::_CalcDefaultPredict()
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
sfloat cGAContinueRuleSet::_ConvertIntToFloat(const uint32 Data) const
{
	sfloat FData = this->m_Desc.ValBoundary[0];
	sfloat FDistance = this->m_Desc.ValBoundary[1] - this->m_Desc.ValBoundary[0];
	sfloat IDistance = this->m_Desc.BitBoundary[1] - this->m_Desc.BitBoundary[0];
	return FData + FDistance*(((sfloat) Data - (sfloat)this->m_Desc.BitBoundary[0]) / IDistance);
}
uint32 cGAContinueRuleSet::_ConvertFloatToInt(const sfloat Data) const
{
	sfloat IData = this->m_Desc.BitBoundary[0];
	sfloat IDistance = this->m_Desc.BitBoundary[1] - this->m_Desc.BitBoundary[0];
	sfloat FDistance = this->m_Desc.ValBoundary[1] - this->m_Desc.ValBoundary[0];
	return IData + IDistance*(((sfloat) Data - (sfloat)this->m_Desc.ValBoundary[0]) / FDistance);
}
bool cGAContinueRuleSet::iInitialize(const uint32 Index, const GACONTINUERULEDESC &Desc, const GAINSTANCESET<SString, SString> &InstanceSpace)
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
	if (Desc.ValBoundary[0] >= Desc.ValBoundary[1])
	{
		return false;
	}
	if (Desc.BitBoundary[0] >= Desc.BitBoundary[1])
	{
		return false;
	}
	this->m_Desc = Desc;
	this->m_Desc.AttributeLength = 18 * this->m_Desc.AttributeArray.size(); // 2bit operator and 8*2 bit number
	this->m_Desc.PerRuleLength = this->m_Desc.AttributeLength + this->m_Desc.ResultArray.size();
	this->m_RuleSet.iReSizeStream(this->m_Desc.RuleSetSize*this->m_Desc.PerRuleLength);
	this->m_RuleSet.iClear(false);
	for (uint32 seek = 0; seek < this->m_Desc.RuleSetSize; seek = seek + 1)
	{
		uint32 Offset = seek*this->m_Desc.PerRuleLength;
		uint32 AttribOffset = 0;
		if (false)//(Index*this->m_Desc.RuleSetSize + seek) < InstanceSpace.size())
		{
			/*From training set*/
			for (uint32 seek_attrib = 0; seek_attrib < this->m_Desc.AttributeArray.size(); seek_attrib = seek_attrib + 1)
			{
				sfloat FloatData = ::gloSToF(InstanceSpace[Index*this->m_Desc.RuleSetSize + seek].first[seek_attrib]);
				uint8 Val = this->_ConvertFloatToInt(FloatData);
				this->m_RuleSet[Offset + AttribOffset] = true;
				this->m_RuleSet[Offset + AttribOffset + 1] = true;
				this->m_RuleSet.iSetBitStream(Offset + AttribOffset + 2, Val, 0, 8);
				this->m_RuleSet.iSetBitStream(Offset + AttribOffset + 10, Val, 0, 8);
				AttribOffset = AttribOffset + 18;
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
				uint32 Op = ::rand() % 4;
				switch (Op)
				{
				case 0:
					// All accept
					this->m_RuleSet[Offset + AttribOffset] = false;
					this->m_RuleSet[Offset + AttribOffset + 1] = false;
					break;
				case 1:
					// Num <= Val
					this->m_RuleSet[Offset + AttribOffset] = true;
					this->m_RuleSet[Offset + AttribOffset + 1] = false;
					break;
				case 2:
					// Val <= Num
					this->m_RuleSet[Offset + AttribOffset] = false;
					this->m_RuleSet[Offset + AttribOffset + 1] = true;
					break;
				case 3:
					// Num <=  Val <= Num
					this->m_RuleSet[Offset + AttribOffset] = true;
					this->m_RuleSet[Offset + AttribOffset + 1] = true;
					break;
				default:
					this->m_RuleSet[Offset + AttribOffset] = false;
					this->m_RuleSet[Offset + AttribOffset + 1] = false;
					break;
				}
				uint8 Data = ::rand() % 256;
				this->m_RuleSet.iSetBitStream(Offset + AttribOffset + 2, Data, 0, 8);
				this->m_RuleSet.iSetBitStream(Offset + AttribOffset + 10, Data, 0, 8);
				AttribOffset = AttribOffset + 18;
			}
			uint32 Res = ::rand() % this->m_Desc.ResultArray.size();
			this->m_RuleSet[Offset + AttribOffset + Res] = true;
		}
	}
}
void cGAContinueRuleSet::iRelease()
{
	this->m_Desc.RuleSetSize = 0;
	this->m_Desc.PerRuleLength = 0;
	this->m_Desc.AttributeArray.clear();
	this->m_Desc.ResultArray.clear();
	this->m_RuleSet.iReSizeStream(0);
}
bool cGAContinueRuleSet::iPredict(const GAINSTANCESET<SString, SString> &InstanceSpace, vector<SString> &Result)
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
bool cGAContinueRuleSet::iTraining(const GAINSTANCESET<SString, SString> &TrainingSet)
{
	return true;
}
bool cGAContinueRuleSet::iCrossoverSP(const cGeneticAlgorithmHypothesis<SString, SString, GACONTINUERULEDESC>* pHypothesis, vector<cGeneticAlgorithmHypothesis<SString, SString, GACONTINUERULEDESC>*> &OffspringSet)
{
	return this->_CrossoverSP(*this, *pHypothesis, OffspringSet);
}
bool cGAContinueRuleSet::iCrossoverTP(const cGeneticAlgorithmHypothesis<SString, SString, GACONTINUERULEDESC>* pHypothesis, vector<cGeneticAlgorithmHypothesis<SString, SString, GACONTINUERULEDESC>*> &OffspringSet)
{
	return false;
}
bool cGAContinueRuleSet::iCrossoverUNI(const cGeneticAlgorithmHypothesis<SString, SString, GACONTINUERULEDESC>* pHypothesis, vector<cGeneticAlgorithmHypothesis<SString, SString, GACONTINUERULEDESC>*> &OffspringSet)
{
	return false;
}
bool cGAContinueRuleSet::iMutation()
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
	this->m_RuleSet[Index] = !this->m_RuleSet[Index];
	return true;
}
sfloat cGAContinueRuleSet::iEvaluate(const GAINSTANCESET<SString, SString> &InstanceSpace)
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
sfloat cGAContinueRuleSet::iAccuracy(const GAINSTANCESET<SString, SString> &InstanceSpace)
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
SString cGAContinueRuleSet::iPrintHypothesisSpace() const
{
	SString strOutput = _SSTR("\r\n");
	for (uint32 seek = 0; seek < this->m_Desc.RuleSetSize; seek = seek + 1)
	{
		strOutput = strOutput + _SSTR("< ");
		uint32 AttribOffset = 0;
		for (uint32 seek_attrib = 0; seek_attrib < this->m_Desc.AttributeArray.size(); seek_attrib = seek_attrib + 1)
		{
			uint8 Op = 0;
			uint8 Data[2];
			uint32 Index = seek*this->m_Desc.PerRuleLength + seek_attrib;
			this->m_RuleSet.iGetBitStream(Index, Op, 0, 2);
			this->m_RuleSet.iGetBitStream(Index + 2, Data[0], 0, 8);
			this->m_RuleSet.iGetBitStream(Index + 10, Data[1], 0, 8);

			uint8 IData;
			if (Data[0] > Data[1])
			{
				IData = Data[1];
				Data[1] = Data[0];
				Data[0] = IData;
			}

			switch (Op)
			{
			case 0:
				strOutput = strOutput + this->m_Desc.AttributeArray[seek_attrib] + _SSTR(": ACCEPT ALL");
				break;
			case 1:
				strOutput = strOutput + this->m_Desc.AttributeArray[seek_attrib] + _SSTR(" <= ") + ::gloFToS(this->_ConvertIntToFloat((uint32) Data[1]));
				break;
			case 2:
				strOutput = strOutput + ::gloFToS(this->_ConvertIntToFloat((uint32) Data[0])) + _SSTR(" <= ") + this->m_Desc.AttributeArray[seek_attrib];
				break;
			case 3:
				strOutput = strOutput + ::gloFToS(this->_ConvertIntToFloat((uint32) Data[0])) + _SSTR(" <= ") + this->m_Desc.AttributeArray[seek_attrib] + _SSTR(" <= ") + ::gloFToS(this->_ConvertIntToFloat((uint32) Data[1])) + _SSTR(", ");
				break;
			default:
				strOutput = strOutput + this->m_Desc.AttributeArray[seek_attrib] + _SSTR(": ACCEPT ALL");
				break;
			}
			strOutput = strOutput + _SSTR(", ");
			seek_attrib = seek_attrib + 18;
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
SString cGAContinueRuleSet::iPrintDebugInformtion() const
{
	SString strOutput;
	for (uint32 seek = 0; seek < this->m_Desc.RuleSetSize; seek = seek + 1)
	{
		uint32 AttribOffset = 0;
		for (uint32 seek_attrib = 0; seek_attrib < this->m_Desc.AttributeArray.size(); seek_attrib = seek_attrib + 1)
		{
			if (this->m_RuleSet[seek*this->m_Desc.PerRuleLength + AttribOffset])
			{
				strOutput = strOutput + _SSTR("1");
			}
			else
			{
				strOutput = strOutput + _SSTR("0");
			}
			if (this->m_RuleSet[seek*this->m_Desc.PerRuleLength + AttribOffset+1])
			{
				strOutput = strOutput + _SSTR("1 ");
			}
			else
			{
				strOutput = strOutput + _SSTR("0 ");
			}
			uint32 Index = seek*this->m_Desc.PerRuleLength + AttribOffset + 2;
			for (uint32 seek_val = 0; seek_val < 2; seek_val = seek_val + 1)
			{
				for (uint32 seek_bit = 0; seek_bit < 8; seek_bit = seek_bit + 1)
				{
					if (this->m_RuleSet[Index + seek_val * 8 + seek_bit])
					{
						strOutput = strOutput + _SSTR("1");
					}
					else
					{
						strOutput = strOutput + _SSTR("0");
					}
				}
				strOutput = strOutput + _SSTR(" ");
			}
			strOutput = strOutput + _SSTR("\n");
			AttribOffset = AttribOffset + 18;
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
sfloat cGAContinueRuleSet::iGetHypothesisSize() const
{
	return this->m_Desc.RuleSetSize;
}