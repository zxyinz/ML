#include"iostream"
#include"time.h"
#include"FileIO\SanFileIO.h"
#include"ML\cArtificialNeuralNetworkAlgorithm.h"
#include"ML\cDeepNeuralNetworkAlgorithm.h"
using namespace std;
using namespace San;
using namespace San::FileIOStream;
#ifndef __TRAININGDATAPREPROCESSING_H__
#define __TRAININGDATAPREPROCESSING_H__
bool GetTrainingSet(const SString strTrainingImageFilePath, const SString strTrainingLabelFilePath,const uint32 TrainingDataSize, ANNTRAININGSET &TrainingSet,ANNTRAININGSET &ValidationSet,const sfloat ValidationRate)
{
	const uint32 ImageSize = 28 * 28;

	ANNTRAININGSET InstanceSet;

	SANSTREAM DataSet;
	SANSTREAM LabelSet;

	if (!::gloLoadFile(strTrainingImageFilePath, DataSet))
	{
		::cout << "Error: Can not find training image data file\n";
		return false;
	}
	if (!::gloLoadFile(strTrainingLabelFilePath, LabelSet))
	{
		::cout << "Error: Can not find training label data file\n";
		return false;
	}

#pragma region analysis file
	/*Image Data Set File Format:									*/
	/*[offset] [type]          [value]          [description]		*/
	/*0000     32 bit integer  0x00000803(2051) magic number		*/
	/*0004     32 bit integer  60000            number of images	*/
	/*0008     32 bit integer  28               number of rows		*/
	/*0012     32 bit integer  28               number of columns	*/
	/*0016     unsigned byte   ??               pixel				*/
	/*0017     unsigned byte   ??               pixel				*/
	/*........														*/
	/*xxxx     unsigned byte   ??               pixel				*/
	/*Pixels are organized row-wise. Pixel values are 0 to 255.		*/
	/*0 means background (white), 255 means foreground (black).		*/
	/*Reference: http://yann.lecun.com/exdb/mnist/					*/

	/*Label Data Set File Format:											*/
	/*[offset] [type]          [value]          [description]				*/
	/*0000     32 bit integer  0x00000801(2049) magic number (MSB first)	*/
	/*0004     32 bit integer  60000            number of items				*/
	/*0008     unsigned byte   ??               label						*/
	/*0009     unsigned byte   ??               label						*/
	/*........																*/
	/*xxxx     unsigned byte   ??               label						*/
	/*The labels values are 0 to 9.											*/
	/*Reference: http://yann.lecun.com/exdb/mnist/							*/

	InstanceSet.resize(TrainingDataSize);

	/*Load each instance*/
	for (uint32 seek = 0; seek < TrainingDataSize; seek = seek + 1)
	{
		uint32 Offset = 16 + seek*ImageSize;

		InstanceSet[seek].first.resize(ImageSize);
		InstanceSet[seek].second.resize(10);

		for (uint32 seek_pix = 0; seek_pix < ImageSize; seek_pix = seek_pix + 1)
		{
			/*Set normalized pixel 0.0 ~ 1.0*/
			InstanceSet[seek].first[seek_pix] = (sfloat) (DataSet[Offset + seek_pix]) / 255.0;
		}
		for (uint32 seek_res = 0; seek_res < 9; seek_res = seek_res + 1)
		{
			InstanceSet[seek].second[seek_res] = 0.0;
		}

		/*Set Label*/
		InstanceSet[seek].second[LabelSet[8 + seek]] = 1.0;
	}
#pragma end region

	if (ValidationRate <= 0.0)
	{
		TrainingSet = InstanceSet;
		return true;
	}
	sfloat RandomVal = 0.0;

	for (uint32 seek = 0; seek < TrainingDataSize; seek = seek + 1)
	{
		RandomVal = ::rand() % 1000;
		RandomVal = RandomVal / 1000.0;

		if (RandomVal < ValidationRate)
		{
			ValidationSet.push_back(InstanceSet[seek]);
		}
		else
		{
			TrainingSet.push_back(InstanceSet[seek]);
		}
	}
	return true;
}
bool GetTestSet(const SString strTestImageFilePath, const SString strTestLabelFilePath, const uint32 TestDataSize, ANNTRAININGSET &TestSet)
{
	const uint32 ImageSize = 28 * 28;

	SANSTREAM DataSet;
	SANSTREAM LabelSet;

	if (!::gloLoadFile(strTestImageFilePath, DataSet))
	{
		::cout << "Error: Can not find test image data file\n";
		return false;
	}
	if (!::gloLoadFile(strTestLabelFilePath, LabelSet))
	{
		::cout << "Error: Can not find test label data file\n";
		return false;
	}

	TestSet.resize(TestDataSize);

	/*Load each instance*/
	for (uint32 seek = 0; seek < TestDataSize; seek = seek + 1)
	{
		uint32 Offset = 16 + seek*ImageSize;

		TestSet[seek].first.resize(ImageSize);
		TestSet[seek].second.resize(10);

		for (uint32 seek_pix = 0; seek_pix < ImageSize; seek_pix = seek_pix + 1)
		{
			/*Set normalized pixel 0.0 ~ 1.0*/
			TestSet[seek].first[seek_pix] = (sfloat) (DataSet[Offset + seek_pix]) / 255.0;
		}
		for (uint32 seek_res = 0; seek_res < 9; seek_res = seek_res + 1)
		{
			TestSet[seek].second[seek_res] = 0.0;
		}

		/*Set Label*/
		TestSet[seek].second[LabelSet[8 + seek]] = 1.0;
	}
	return true;
}
bool GetCIFARTrainingSet(const SString strTrainingImageFilePath, const uint32 TrainingDataSize, ANNTRAININGSET &TrainingSet, ANNTRAININGSET &ValidationSet, const sfloat ValidationRate)
{
	const uint32 ImageSize = 32 * 32 * 3;
	const uint32 PerChannelSize = 32 * 32;

	ANNTRAININGSET InstanceSet;

	SANSTREAM DataSet;

	if (!::gloLoadFile(strTrainingImageFilePath, DataSet))
	{
		::cout << "Error: Can not find training image data file\n";
		return false;
	}

#pragma region analysis file

	InstanceSet.resize(TrainingDataSize);

	/*Load each instance*/
	for (uint32 seek = 0; seek < TrainingDataSize; seek = seek + 1)
	{
		uint32 Offset = seek*(ImageSize + 1);

		InstanceSet[seek].first.resize(PerChannelSize);
		InstanceSet[seek].second.resize(10);

		for (uint32 seek_pix = 0; seek_pix < PerChannelSize; seek_pix = seek_pix + 1)
		{
			uint32 PixIndex = seek_pix * 3;
			/*Set normalized pixel 0.0 ~ 1.0*/
			/*InstanceSet[seek].first[PixIndex] = (sfloat)(DataSet[Offset + seek_pix]) / 255.0;
			InstanceSet[seek].first[PixIndex + 1] = (sfloat)(DataSet[Offset + PerChannelSize + seek_pix]) / 255.0;
			InstanceSet[seek].first[PixIndex + 2] = (sfloat)(DataSet[Offset + PerChannelSize * 2 + seek_pix]) / 255.0;*/

			sfloat Pixel = (sfloat)DataSet[Offset + seek_pix] * 0.2989;
			Pixel = Pixel + (sfloat)DataSet[Offset + PerChannelSize + seek_pix] * 0.5870;
			Pixel = Pixel + (sfloat)DataSet[Offset + PerChannelSize*2 + seek_pix] * 0.1140;

			InstanceSet[seek].first[seek_pix] = Pixel / 255.0;
		}
		for (uint32 seek_res = 0; seek_res < 9; seek_res = seek_res + 1)
		{
			InstanceSet[seek].second[seek_res] = 0.0;
		}

		/*Set Label*/
		InstanceSet[seek].second[DataSet[Offset]] = 1.0;
	}
#pragma end region

	if (ValidationRate <= 0.0)
	{
		TrainingSet = InstanceSet;
		return true;
	}
	sfloat RandomVal = 0.0;

	for (uint32 seek = 0; seek < TrainingDataSize; seek = seek + 1)
	{
		RandomVal = ::rand() % 1000;
		RandomVal = RandomVal / 1000.0;

		if (RandomVal < ValidationRate)
		{
			ValidationSet.push_back(InstanceSet[seek]);
		}
		else
		{
			TrainingSet.push_back(InstanceSet[seek]);
		}
	}
	return true;
}
sfloat CalcVectorDistance(const vector<sfloat> Vec1, const vector<sfloat> Vec2)
{
	if (Vec1.size() != Vec2.size())
	{
		return MAX_FLOAT32_VALUE;
	}
	sfloat Distance = 0.0;
	uint32 VecSize = Vec1.size();
	for (uint32 seek = 0; seek < VecSize; seek = seek + 1)
	{
		sfloat SubDis = Vec1[seek] - Vec2[seek];
		Distance = Distance + SubDis*SubDis;
	}
	return sqrt(Distance);
}
sfloat CalcAccuracy(const vector<sfloat> Vec1, const vector<sfloat> Vec2)
{
	if (Vec1.size() != Vec2.size())
	{
		return MAX_FLOAT32_VALUE;
	}
	uint32 Max = 0;
	uint32 VecSize = Vec1.size();
	for (uint32 seek = 0; seek < VecSize; seek = seek + 1)
	{
		if (Vec1[Max] <= Vec1[seek])
		{
			Max = seek;
		}
	}
	if (Vec2[Max] >= 0.7)
	{
		return 0.0;
	}
	return MAX_FLOAT32_VALUE;
}
bool WriteTrainingData(const SString strTrainingImageFilePath, const SString strTrainingLabelFilePath)
{
	const uint32 ImageSize = 28 * 28;

	const uint32 DataSetSize = 60000;

	SANSTREAM DataSet;
	SANSTREAM LabelSet;

	if (!::gloLoadFile(strTrainingImageFilePath, DataSet))
	{
		::cout << "Error: Can not find training image data file\n";
		return false;
	}
	if (!::gloLoadFile(strTrainingLabelFilePath, LabelSet))
	{
		::cout << "Error: Can not find training label data file\n";
		return false;
	}

	SANSTREAM NewSet(16 + DataSetSize*ImageSize);
	SANSTREAM NewLabel(8 + DataSetSize);

	NewSet.iSetStream(0, DataSet.pStream, 16 + DataSetSize*ImageSize);
	NewLabel.iSetStream(0, LabelSet.pStream, 8 + DataSetSize);

	::gloWriteFile(_SSTR("Training-") + ::gloIToS(DataSetSize) + _SSTR("-image.data"), NewSet);
	::gloWriteFile(_SSTR("Training-") + ::gloIToS(DataSetSize) + _SSTR("-label.data"), NewLabel);

	return true;
}
sfloat CalcAccuracy(cArtificialNeuralNetworkAlgorithm &ANNNet,const ANNTRAININGSET &TestSet)
{
	uint32 SetSize = TestSet.size();
	uint32 Count = 0;
	vector<sfloat> Result;
	for (uint32 seek_instance = 0; seek_instance < SetSize; seek_instance = seek_instance + 1)
	{
		ANNNet.iPredict(TestSet[seek_instance].first);
		Result = ANNNet.iGetOutputArray();

		if (CalcAccuracy(Result, TestSet[seek_instance].second) <= 0.3)
		{
			Count = Count + 1;
		}
	}

	sfloat Accuracy = Count;
	return Accuracy / (sfloat) SetSize;
}
void MultiANN(cDeepNeuralNetworkAlgorithm &DNN, const uint32 Size, const ANNTRAININGSET &TrainingSet, const ANNTRAININGSET &TestSet)
{
	vector<cDeepNeuralNetworkAlgorithm> DNNArray(Size);
	for (uint32 seek = 0; seek < Size; seek = seek + 1)
	{
		DNNArray[seek] = DNN;
	}

	vector<float> True(1);
	vector<float> False(1);

	True[0] = 1;
	False[0] = 0;


}
SString PrintTime(const clock_t StartTime)
{
	clock_t ClockOffset = ::clock() - StartTime;

	SString strOutput;

	uint32 ms = (uint32) ClockOffset % 1000;
	uint32 sec = (ClockOffset / 1000) % 60;
	uint32 min = (ClockOffset / (60 * 1000)) % 60;
	uint32 hour = ClockOffset / (60 * 60 * 1000);

	return ::gloIToS(hour) + _SSTR(" hour ") + ::gloIToS(min) + _SSTR(" min ") + ::gloIToS(sec) + _SSTR(" sec ") + ::gloIToS(ms) + _SSTR(" ms\r\n");
}
#endif