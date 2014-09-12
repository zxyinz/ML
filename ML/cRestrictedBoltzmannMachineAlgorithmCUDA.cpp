#ifndef __CRESTRICTEDBOLTZMANNMACHINEALGORITHMCUDA_CORE_H__
#define __CRESTRICTEDBOLTZMANNMACHINEALGORITHMCUDA_CORE_H__

struct RBMDESC_CUDA
{
	float* pHiddenLayerBias;
	float* pVisibleLayerBias;
	float* pWeightArray;
	float* pTrainingDataArray;
	float* pRandomTable;
	unsigned int HiddenLayerSize;
	unsigned int VisibleLayerSize;
	unsigned int WeightArraySize;
	unsigned int TrainingDataSize;

	//unsigned int CurrentRandomVal;
	unsigned int RandomTableSize;

	float LearningRate;

	float* pHiddenProbability;
	float* pHiddenSample;
	float* pNewVisibleProbability;
	float* pNewVisibleSample;
	float* pNewHiddenProbability;
	float* pNewHiddenSample;
};

__constant__ static struct RBMDESC_CUDA DevDesc;
__constant__ static float pDevRandomTable[1024 * 3];

__device__ float _CalcBinomialValue(float N, float Probability, int RandomIndex, int RandomSize)
{
	if ((Probability<0.0) || (Probability>1.0))
	{
		return 0;
	}
	float Res = 0;
	float RandVal = 0.0;
#pragma unroll
	for (uint32 seek = 0; seek < N; seek = seek + 1)
	{
		RandVal = pDevRandomTable[RandomIndex % RandomSize];
		if (RandVal < Probability)
		{
			Res = Res + 1.0;
		}
	}
	return Res;
};
__device__ float _CalcSigmoidValue(float Net)
{
	return 1.0 / (1.0 + exp(-Net));
};
__device__ float _CalcProbabilityUp(int HiddenIndex, float* pSample)
{
	/*P(Hi | v£© = sig(sum(Vj * Wij) + Bi)*/
	float Probability = 0.0;
	int VisibleLayerSize = DevDesc.VisibleLayerSize;
	int HiddenLayerSize = DevDesc.HiddenLayerSize;

	for (int seek = 0; seek < VisibleLayerSize; seek = seek + 1)
	{
		Probability = Probability + pSample[seek] * DevDesc.pWeightArray[HiddenIndex*VisibleLayerSize + seek];
	}
	Probability = Probability + DevDesc.pHiddenLayerBias[HiddenIndex];
	return _CalcSigmoidValue(Probability);
};
__device__ float _CalcProbabilityDown(int VisibleIndex, float* pSample)
{
	/*P(Vi | h) = sig(sum(Hj*Wij) + Ai)*/
	float Probability = 0.0;
	int VisibleLayerSize = DevDesc.VisibleLayerSize;
	int HiddenLayerSize = DevDesc.HiddenLayerSize;

	for (int seek = 0; seek < HiddenLayerSize; seek = seek + 1)
	{
		Probability = Probability + pSample[seek] * DevDesc.pWeightArray[seek*VisibleLayerSize + VisibleIndex];
	}
	Probability = Probability + DevDesc.pVisibleLayerBias[VisibleIndex];
	return _CalcSigmoidValue(Probability);
};
__global__ void _CD_K(int TrainingTimes int KTimes)
{
	extern __shared__ float Data[];
	float* pHiddenProbability = Data;
	float* pHiddenSample = &Data[DevDesc.HiddenLayerSize];
	float* pNewHiddenProbability = &Data[DevDesc.HiddenLayerSize * 2];
	float* pNewHiddenSample = &Data[DevDesc.HiddenLayerSize * 3];
	float* pNewVisibleProbability = &Data[DevDesc.HiddenLayerSize * 4];
	float* pNewVisibleSample = &Data[DevDesc.HiddenLayerSize * 4 + DevDesc.VisibleLayerSize];

	int HiddenLayerSize = DevDesc.HiddenLayerSize;
	int VisibleLayerSize = DevDesc.VisibleLayerSize;
	int SetSize = DevDesc.TrainingDataSize;
	int RandomTableSize = DevDesc.RandomTableSize;
	int CurrentRandomOffset = 0;

	int Index = 0;
	Index = threadIdx.x + blockIdx.x*blockDim.x;

	float Probability = 0.0;
	int Offset = 0;

	for (int seek_iter = 0; seek_iter< TrainingTimes; seek_iter = seek_iter + 1)
	{
		for (int seek_ins = 0; seek_ins < SetSize; seek_ins = seek_ins + 1)
		{
			if (Index < HiddenLayerSize)
			{
				Offset = Index*VisibleLayerSize;
				for (int seek = 0; seek < VisibleLayerSize; seek = seek + 1)
				{
					Probability = Probability + pSample[seek] * DevDesc.pWeightArray[Offset + seek];
				}
				Probability = Probability + DevDesc.pHiddenLayerBias[HiddenIndex];
				pHiddenProbability[Index] = _CalcSigmoidValue(Probability);
				pHiddenSample[Index] = _CalcBinomialValue(1, pHiddenProbability[Index], CurrentRandomOffset + Index, RandomTableSize);
			}
			CurrentRandomOffset = CurrentRandomOffset + HiddenLayerSize;
			_syncthreads();

			if (Index <  VisibleLayerSize)
			{
				Offset = 0;
				for (int seek = 0; seek < HiddenLayerSize; seek = seek + 1)
				{
					Probability = Probability + pHiddenSample[seek] * DevDesc.pWeightArray[Offset + VisibleIndex];
					Offset = Offset + VisibleLayerSize;
				}
				Probability = Probability + DevDesc.pVisibleLayerBias[VisibleIndex];
				pNewVisibleProbability[Index] = _CalcSigmoidValue(Probability);
				pNewVisibleSample[Index] = _CalcBinomialValue(1, pNewVisibleProbability[Index], CurrentRandomOffset + Index, RandomTableSize);
			}
			CurrentRandomOffset = CurrentRandomOffset + VisibleLayerSize;
			_syncthreads();

			if (Index < HiddenLayerSize)
			{
				Offset = Index*VisibleLayerSize;
				for (int seek = 0; seek < VisibleLayerSize; seek = seek + 1)
				{
					Probability = Probability + pNewVisibleSample[seek] * DevDesc.pWeightArray[Offset + seek];
				}
				Probability = Probability + DevDesc.pHiddenLayerBias[HiddenIndex];
				pNewHiddenProbability[Index] = _CalcSigmoidValue(Probability);
				pNewHiddenSample[Index] = _CalcBinomialValue(1, pNewHiddenProbability[Index], CurrentRandomOffset + Index, RandomTableSize);
			}
			CurrentRandomOffset = CurrentRandomOffset + HiddenLayerSize;
			_syncthreads();
		}
	}


	/*Update RBM*/
	float LRate = DevDesc.LearningRate;

	if (Index <HiddenLayerSize)
	{
		int WeightIndex = VisibleLayerSize*Index;

		float Delta = 0.0;
		for (int seek_visible = 0; seek_visible < VisibleLayerSize; seek_visible = seek_visible + 1)
		{
			Delta = pHiddenProbability[Index] * pSample[seek_visible] - pNewHiddenProbability * pNewVisibleSample[seek_visible];
			Delta = Delta / (float)(SetSize);
			DevDesc.pWeightArray[WeightIndex + seek_visible] = DevDesc.pWeightArray[WeightIndex + seek_visible] + Delta*LRate;
		}

		Delta = LRate*(pHiddenSample[Index] - pNewHiddenProbability) / (float)SetSize;
		DevDesc.pHiddenLayerBias[Index] = DevDesc.pHiddenLayerBias[Index] + Delta;
	}

	if (Index < VisibleLayerSize)
	{
		float Delta = LRate*(pSample[Index] - pNewVisibleSample[Index]) / (float)SetSize;
		DevDesc.pVisibleLayerBias[Index] = DevDesc.pVisibleLayerBias[Index] + Delta;
	}
}
__global__ void _SampleHV(float* pSample, float* pHiddenNodeProbability, float* pHiddenNodeSample)
{
	int Index = 0;
	Index = threadIdx.x + blockIdx.x*blockDim.x;
	if (Index < DevDesc.HiddenLayerSize)
	{
		pHiddenNodeProbability[Index] = _CalcProbabilityUp(Index, pSample);
		pHiddenNodeSample[Index] = _CalcBinomialValue(1, pHiddenNodeProbability[Index], Index, DevDesc.RandomTableSize);
	}
};
__global__ void _SampleVH(float* pSample, float* pVisibleNodeProbability, float* pVisibleNodeSample)
{
	int Index = threadIdx.x + blockIdx.x*blockDim.x;
	if (Index <  DevDesc.VisibleLayerSize)
	{
		pVisibleNodeProbability[Index] = _CalcProbabilityDown(Index, pSample);
		pVisibleNodeSample[Index] = _CalcBinomialValue(1, pVisibleNodeProbability[Index], Index, DevDesc.RandomTableSize);
	}
};
__global__ void _UpdateRBM(float* pSample, int InstanceIndex)
{
	int Index = threadIdx.x + blockIdx.x*blockDim.x;

	if (Index < DevDesc.HiddenLayerSize)
	{
		int HiddenLayerSize = DevDesc.HiddenLayerSize;
		int VisibleLayerSize = DevDesc.VisibleLayerSize;
		int WeightIndex = VisibleLayerSize*Index;

		float NewHiddenProb = DevDesc.pNewHiddenProbability[Index];
		float LRate = DevDesc.LearningRate;

		float Delta = 0.0;

		for (int seek_visible = 0; seek_visible < VisibleLayerSize; seek_visible = seek_visible + 1)
		{
			Delta = DevDesc.pHiddenProbability[Index] * pSample[seek_visible] - NewHiddenProb * DevDesc.pNewVisibleSample[seek_visible];
			Delta = Delta / (float)(DevDesc.TrainingDataSize);
			DevDesc.pWeightArray[WeightIndex + seek_visible] = DevDesc.pWeightArray[WeightIndex + seek_visible] + Delta*LRate;
		}

		Delta = LRate*(DevDesc.pHiddenSample[Index] - NewHiddenProb) / (float)DevDesc.TrainingDataSize;
		DevDesc.pHiddenLayerBias[Index] = DevDesc.pHiddenLayerBias[Index] + Delta;
	}

	if (Index < DevDesc.VisibleLayerSize)
	{
		float Delta = DevDesc.LearningRate*(pSample[Index] - DevDesc.pNewVisibleSample[Index]) / (float)DevDesc.TrainingDataSize;
		DevDesc.pVisibleLayerBias[Index] = DevDesc.pVisibleLayerBias[Index] + Delta;
	}
};
#endif