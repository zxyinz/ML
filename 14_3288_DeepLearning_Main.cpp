#include"iostream"
#include"process.h"
#include"Device\cSanTerminalDeviceWin.h"
#include"FileIO\SanFileIO.h"
#include"TrainingDataPreProcessing.h"
using namespace std;
using namespace San;
using namespace San::Device;
using namespace San::FileIOStream;

//const SString strTrainingImagePath = _SSTR("train-images.idx3-ubyte");
//const SString strTrainingLabelPath = _SSTR("train-labels.idx1-ubyte");
//const SString strTestImagePath = _SSTR("t10k-images.idx3-ubyte");
//const SString strTestLabelPath = _SSTR("t10k-labels.idx1-ubyte");


const SString strTrainingImagePath = _SSTR("Training-1000-image.data");
const SString strTrainingLabelPath = _SSTR("Training-1000-label.data");
const SString strTestImagePath = _SSTR("Test-10000-image.data");
const SString strTestLabelPath = _SSTR("Test-10000-label.data");

//const SString strTrainingImagePath = _SSTR("data_batch_1.bin");
//const SString strTestImagePath = _SSTR("test_batch.bin");

unsigned int __stdcall TerminalTimerFunc(void* pPtr);
unsigned int TimerThreadID;

int main(int argc, char** argv)
{

	srand(time(0));

	/*vector<vector<float>> Identity(6);
	vector<vector<float>> Output(6);

	for (uint32 seek = 0; seek<6; seek = seek + 1)
	{
		Identity[seek].resize(6);
		Output[seek].resize(6);
		for (uint32 seek_b = 0; seek_b<6; seek_b = seek_b + 1)
		{
			Identity[seek][seek_b] = 0;
		}
		Identity[seek][seek] = 1.0;
	}

	cRestrictedBoltzmannMachineAlgorithm rbm(6, 3);
	rbm.iTrainingRBM(Identity, Output, 1, 6, 0.3);*/

	cSanTerminalDevice Terminal;
	Terminal.iSetTerminalTittle(_SSTR("San Deep Learning Terminal"));

	::_beginthreadex(nullptr, 0, TerminalTimerFunc, &Terminal, 0, &TimerThreadID);

	ANNTRAININGSET TrainingSet;
	ANNTRAININGSET ValidationSet;
	ANNTRAININGSET TestSet;

	SString strOutput;

	::cout << "Load Data...\n";

	GetTrainingSet(strTrainingImagePath, strTrainingLabelPath, 1000, TrainingSet, ValidationSet, 0.0);
	GetTestSet(strTestImagePath, strTestLabelPath, 10000, TestSet);

	/*GetCIFARTrainingSet(strTrainingImagePath, 5000, TrainingSet, ValidationSet, 0.0);
	GetCIFARTrainingSet(strTestImagePath, 1000, TestSet, ValidationSet, 0.0);*/

	/*ANN*/
	::cout << "ANN Initialize...\n";

	const sfloat LearningRate = 0.03;
	const sfloat Momentum = 0.2;

	strOutput = strOutput + _SSTR("Learning Rate: ") + ::gloFToS(LearningRate) + _SSTR("\r\n");
	strOutput = strOutput + _SSTR("Monentum Rate: ") + ::gloFToS(Momentum) + _SSTR("\r\n\r\n");

	strOutput = strOutput + _SSTR("Training Data Set: ") + strTrainingImagePath + _SSTR("\r\n\r\n");

	//cArtificialNeuralNetworkAlgorithm ANNNet(LearningRate, Momentum, -0.05, 0.05);

	cDeepNeuralNetworkAlgorithm ANNNet(LearningRate, Momentum, -0.05, 0.05);

	uint32 FeatureSize = 28 * 28;

	SANSTREAM UserData;
	for (uint32 seek = 0; seek < FeatureSize; seek = seek + 1)
	{
		ANNNet.iCreateFeatureNode(::gloIToS(seek), UserData, nullptr);
	}


	strOutput = strOutput + _SSTR("ANN Structure: CIFAR-2000 ") + _SSTR("24x24 16x16 8x8 4x4 10") + _SSTR("\r\n\r\n");
	ANNNet.iCreateLayer(24*24);
	ANNNet.iCreateLayer(12*12);
	ANNNet.iCreateLayer(8*8);
	ANNNet.iCreateLayer(4*4);
	ANNNet.iCreateLayer(10);

	sfloat Accuracy;

	clock_t StartTime = ::clock();

	::cout << "ANN Start Training...\t";
	::wcout << PrintTime(StartTime).c_str();

	strOutput = strOutput + _SSTR("ANN Start Training...") + PrintTime(StartTime);

	ANNNet.iPreProcessing(TrainingSet, 0.9, 1000);

	for (uint32 seek = 0; seek < 200; seek = seek + 1)
	{
		ANNNet.iTraining(TrainingSet, 10, false);

		Accuracy = CalcAccuracy(ANNNet, TrainingSet);

		::cout << "Iteration: " << seek << " \t";
		::cout << "Accuracy: " << Accuracy << " \t";
		::wcout << PrintTime(StartTime).c_str();

		strOutput = strOutput + _SSTR("Iteration: ") + ::gloIToS(seek) + _SSTR("   \tTest Accuracy : ") + ::gloFToS(Accuracy) + _SSTR("   \t") + PrintTime(StartTime);

		::gloWriteFile(_SSTR("Output.txt"), strOutput);
	}

	::gloWriteFile(_SSTR("Output.txt"), strOutput);
	::system("notepad Output.txt");
	//::system("pause");
}
unsigned int __stdcall TerminalTimerFunc(void* pPtr)
{
	if (pPtr == nullptr)
	{
		return 0;
	}
	cSanTerminalDevice* pTerminal = (cSanTerminalDevice*) pPtr;
	time_t StartTime = ::clock();
	time_t CurrentTime;
	uint32 Offset;
	uint32 Hour, Min, Sec, MS;
	while (true)
	{
		::Sleep(1000);
		CurrentTime = ::clock();
		Offset = CurrentTime - StartTime;
		MS = Offset % 1000;
		Sec = (Offset / 1000) % 60;
		Min = (Offset / (1000 * 60)) % 60;
		Hour = Offset / (1000 * 60 * 60);
		pTerminal->iSetTerminalTittle(_SSTR("San Deep Learning Terminal, Running Time - ") + ::gloIToS(Hour) + _SSTR(" hour ") + ::gloIToS(Min) + _SSTR(" min ") + ::gloIToS(Sec) + _SSTR(" sec ") + ::gloIToS(MS) + _SSTR(" ms "));
	}
	return 0;
}