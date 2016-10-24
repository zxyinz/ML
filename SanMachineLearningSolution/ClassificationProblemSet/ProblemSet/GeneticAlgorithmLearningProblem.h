#include"FileIO\SanFileIO.h"
#include"Device\cSanTerminalDeviceWin.h"
#include"Device\cSanTerminalDeviceWin.h"
#include"ML\cGeneticAlgorithmHepothesis.h"
#include"ML\cGARuleSet.h"
#include"ML\cGAContinueRuleSet.h"
#include"ML\cGeneticAlgorithm.h"
using namespace std;
using namespace San;
using namespace San::Device;
using namespace San::MachineLearning;
#pragma once
#ifndef __GENETICALGORITHMLEARNINGPROBLEM_H__
#define __GENETICALGORITHMLEARNINGPROBLEM_H__

SString TennisGA(cSanTerminalDevice* pTerminal);
SString IrisGA(cSanTerminalDevice* pTerminal);
SString IrisSGA(cSanTerminalDevice* pTerminal);
SString IrisRGA(cSanTerminalDevice* pTerminal);

bool LoadAttribute(SString strFilePath, vector<pair<SString, vector<SString>>> &AttributeArray, vector<SString> &ResultArray);
bool LoadInstanceSet(const SString &strFilePath, GAINSTANCESET<SString, SString> &InstanceSet);

#endif