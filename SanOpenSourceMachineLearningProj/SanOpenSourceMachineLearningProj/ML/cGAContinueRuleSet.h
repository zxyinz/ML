#include"iostream"
#include"cGeneticAlgorithmHepothesis.h"
using namespace std;
using namespace San;
#ifndef __CGACONTINUERULTSET_H__
#define __CGACONTINUERULTSET_H__
struct GACONTINUERULEDESC
{
	uint32 RuleSetSize;
	uint32 PerRuleLength;
	uint32 AttributeLength;
	uint32 BitBoundary[2];
	sfloat ValBoundary[2];
	vector<SString> AttributeArray;
	vector<SString> ResultArray;
	SString DefaultPredict;
};

class cGAContinueRuleSet : public cGeneticAlgorithmHypothesis<SString, SString, GACONTINUERULEDESC>
{
private:
	SANBITSTREAM m_RuleSet;
	GACONTINUERULEDESC m_Desc;
protected:
	static bool _CrossoverSP(const cGeneticAlgorithmHypothesis<SString, SString, GACONTINUERULEDESC> &H1, const cGeneticAlgorithmHypothesis<SString, SString, GACONTINUERULEDESC> &H2, vector<cGeneticAlgorithmHypothesis<SString, SString, GACONTINUERULEDESC>*> &OffspringSet);
	static bool _InvaildCheck(const cGAContinueRuleSet &Hypothesis, const SANBITSTREAM &Stream);
	bool _PredictByOneRule(const uint32 InstanceIndex, const uint32 RuleIndex, const GAINSTANCESET<SString, SString> &InstanceSpace, SString &Result);
	void _CalcDefaultPredict();
	sfloat _ConvertIntToFloat(const uint32 Data) const;
	uint32 _ConvertFloatToInt(const sfloat Data) const;
public:
	cGAContinueRuleSet();
	~cGAContinueRuleSet();
	bool iInitialize(const uint32 Index, const GACONTINUERULEDESC &Desc, const GAINSTANCESET<SString, SString> &InstanceSpace);
	void iRelease();
	bool iPredict(const GAINSTANCESET<SString, SString> &InstanceSpace, vector<SString> &Result);
	bool iTraining(const GAINSTANCESET<SString, SString> &TrainingSet);
	bool iCrossoverSP(const cGeneticAlgorithmHypothesis<SString, SString, GACONTINUERULEDESC>* pHypothesis, vector<cGeneticAlgorithmHypothesis<SString, SString, GACONTINUERULEDESC>*> &OffspringSet);
	bool iCrossoverTP(const cGeneticAlgorithmHypothesis<SString, SString, GACONTINUERULEDESC>* pHypothesis, vector<cGeneticAlgorithmHypothesis<SString, SString, GACONTINUERULEDESC>*> &OffspringSet);
	bool iCrossoverUNI(const cGeneticAlgorithmHypothesis<SString, SString, GACONTINUERULEDESC>* pHypothesis, vector<cGeneticAlgorithmHypothesis<SString, SString, GACONTINUERULEDESC>*> &OffspringSet);
	bool iMutation();
	sfloat iEvaluate(const GAINSTANCESET<SString, SString> &InstanceSpace);
	sfloat iAccuracy(const GAINSTANCESET<SString, SString> &InstanceSpace);
	SString iPrintHypothesisSpace() const;
	SString iPrintDebugInformtion() const;
	sfloat iGetHypothesisSize() const;
};
#endif