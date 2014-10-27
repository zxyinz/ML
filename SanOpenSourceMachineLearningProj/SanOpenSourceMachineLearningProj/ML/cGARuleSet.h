#include"iostream"
#include"cGeneticAlgorithmHepothesis.h"
using namespace std;
using namespace San;
#ifndef __CGARULTSET_H__
#define __CGARULTSET_H__
struct GARULEDESC
{
	uint32 RuleSetSize;
	uint32 PerRuleLength;
	uint32 AttributeLength;
	vector<pair<SString, vector<SString>>> AttributeArray;
	vector<SString> ResultArray;
	SString DefaultPredict;
};

class cGARuleSet : public cGeneticAlgorithmHypothesis<SString, SString, GARULEDESC>
{
private:
	SANBITSTREAM m_RuleSet;
	GARULEDESC m_Desc;
protected:
	static bool _CrossoverSP(const cGeneticAlgorithmHypothesis<SString, SString, GARULEDESC> &H1, const cGeneticAlgorithmHypothesis<SString, SString, GARULEDESC> &H2, vector<cGeneticAlgorithmHypothesis<SString, SString, GARULEDESC>*> &OffspringSet);
	static bool _InvaildCheck(const cGARuleSet &Hypothesis, const SANBITSTREAM &RuleSet);
	bool _PredictByOneRule(const uint32 InstanceIndex, const uint32 RuleIndex, const GAINSTANCESET<SString, SString> &InstanceSpace, SString &Result);
	void _CalcDefaultPredict();
public:
	cGARuleSet();
	~cGARuleSet();
	bool iInitialize(const uint32 Index, const GARULEDESC &Desc, const GAINSTANCESET<SString, SString> &InstanceSpace);
	void iRelease();
	bool iPredict(const GAINSTANCESET<SString, SString> &InstanceSpace, vector<SString> &Result);
	bool iTraining(const GAINSTANCESET<SString, SString> &TrainingSet);
	bool iCrossoverSP(const cGeneticAlgorithmHypothesis<SString, SString, GARULEDESC>* pHypothesis, vector<cGeneticAlgorithmHypothesis<SString, SString, GARULEDESC>*> &OffspringSet);
	bool iCrossoverTP(const cGeneticAlgorithmHypothesis<SString, SString, GARULEDESC>* pHypothesis, vector<cGeneticAlgorithmHypothesis<SString, SString, GARULEDESC>*> &OffspringSet);
	bool iCrossoverUNI(const cGeneticAlgorithmHypothesis<SString, SString, GARULEDESC>* pHypothesis, vector<cGeneticAlgorithmHypothesis<SString, SString, GARULEDESC>*> &OffspringSet);
	bool iMutation();
	sfloat iEvaluate(const GAINSTANCESET<SString, SString> &InstanceSpace);
	sfloat iAccuracy(const GAINSTANCESET<SString, SString> &InstanceSpace);
	SString iPrintHypothesisSpace() const;
	SString iPrintDebugInformtion() const;
	sfloat iGetHypothesisSize() const;
};
#endif