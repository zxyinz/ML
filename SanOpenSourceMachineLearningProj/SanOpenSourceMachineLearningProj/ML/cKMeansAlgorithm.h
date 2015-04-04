#include"..\Core\SanContainer.h"
#include"..\Core\SanMathematics.h"
using namespace std;
namespace San
{
	namespace MachineLearning
	{
#ifndef __CKMEANSALGORITHM_H__
#define __CKMEANSALGORITHM_H__

		template<class DataType>
		using CLUSTERSET = _sstream<_sstream<DataType>>;

		template<class DataType>
		class cKMeansAlgorithm
		{
		private:
			CLUSTERSET<uint32> m_ClusterIndex;
		protected:
			sfloat _CalcInstanceSimilarity(const DataType &Data1, const DataType Data2) const = 0;
			DataType _CalcClusterCenter(const vector<uint32> &ClusterArray) const = 0;
			DataType _GenerateRandomInstance() const = 0;
		public:
			cKMeansAlgorithm()
			{
				this->m_ClusterIndex.clear();
			};
			~cKMeansAlgorithm()
			{
				this->m_ClusterIndex.clear();
			};

			bool iClusterIndex(const vector<DataType> &DataSet, const uint32 ClusterNum, const sfloat ConvergeBoundary = 0.0)
			{
				if (DataSet.empty()){ this->m_ClusterIndex.clear(); return false; }
				if (ClusterNum == 0){ this->m_ClusterIndex.clear(); return false; }

				vector<DataType> ClusterCenter;
				this->m_ClusterIndex.resize(ClusterNum);

				//Random Initialize all the center data point
				for (uint32 seek = 0; seek < ClusterNum; seek = seek + 1)
				{
					ClusterCenter.push_back(this->_GenerateRandomInstance());
				}

				sfloat MaxChange = MAX_FLOAT32_VALUE;
				uint32 DataSetSize = DataSet.size();

				while (MaxChange > ConvergeBoundary)
				{
					//Clear the cluster set
					for (uint32 seek = 0; seek < ClusterNum; seek = seek + 1)
					{
						this->m_ClusterIndex[seek].clear();
					}

					for (uint32 seek = 0; seek < DataSetSize; seek = seek + 1)
					{
						sfloat MinDis = MAX_FLOAT32_VALUE;
						int32 MinDisIndex = -1;

						//Assign each data point to the nearest cluster
						for (uint32 seek_cluster = 0; seek_cluster < ClusterNum; seek_cluster = seek_cluster + 1)
						{
							sfloat Distance = this->_CalcInstanceSimilarity(DataSet[seek], ClusterCenter[seek_cluster]);

							if (Distance < MinDis)
							{
								MinDis = Distance;
								MinDisIndex = seek_cluster;
							}
						}

						if (MinDisIndex == -1){ return CLUSTERSET<uint32>(); }

						this->m_ClusterIndex[MinDisIndex].push_back(seek);
					}

					//Recalculate each cluster center
					DataType NewClusterCenter;
					MaxChange = -MAX_FLOAT32_VALUE;

					for (uint32 seek = 0; seek < ClusterNum; seek = seek + 1)
					{
						NewClusterCenter = this->_CalcClusterCenter(ClusterSet[seek]);

						sfloat CurrentChange = this->_CalcInstanceSimilarity(ClusterCenter[seek], NewClusterCenter);

						ClusterCenter[seek] = NewClusterCenter;

						MaxChange = MaxChange>CurrentChange ? MaxChange : CurrentChange;
					}
				}

				return true;
			};
			CLUSTERSET<uint32> iGetClusterIndex() const
			{
				return this->m_ClusterIndex;
			};
			CLUSTERSET<DataType> iGetClusterSet(const vector<DataSet> &DataSet) const
			{
				if (this->m_ClusterIndex.empty()){ return CLUSTERSET<DataType>(); }

				CLUSTERSET<DataType> ClusterArray(this->m_ClusterIndex.size());

				for (uint32 seek = 0; seek < this->m_ClusterIndex.size(); seek = seek + 1)
				{
					uint32 SetSize = this->m_ClusterIndex[seek].size();

					ClusterArray[seek].resize(SetSize);

					for (uint32 seek_data = 0; seek_data < SetSize; seek = seek + 1)
					{
						ClusterArray[seek][seek_data] = DataSet[this->m_ClusterIndex[seek][seek_data]];
					}
				}

				return ClusterArray;
			};
		};

		template<class DataType, class SimilarFunc, class CenterFunc, class RandomFunc>
		CLUSTERSET<uint32> KMeansClusterAlgorithm(const vector<DataType> &DataSet, const uint32 ClusterNum, const sfloat ConvergeBoundary = 0.0)
		{
			if (DataSet.empty()){ return CLUSTERSET<uint32>(); }
			if (ClusterNum == 0){ return CLUSTERSET<uint32>(); }

			vector<DataType> ClusterCenter;
			CLUSTERSET<uint32> ClusterSet(ClusterNum);

			//Random Initialize all the center data point
			for (uint32 seek = 0; seek < ClusterNum; seek = seek + 1)
			{
				ClusterCenter.push_back(RandomFunc());
			}

			sfloat MaxChange = MAX_FLOAT32_VALUE;
			uint32 DataSetSize = DataSet.size();

			while (MaxChange > ConvergeBoundary)
			{
				//Clear the cluster set
				for (uint32 seek = 0; seek < ClusterNum; seek = seek + 1)
				{
					ClusterSet[seek].clear();
				}

				for (uint32 seek = 0; seek < DataSetSize; seek = seek + 1)
				{
					sfloat MinDis = MAX_FLOAT32_VALUE;
					int32 MinDisIndex = -1;

					//Assign each data point to the nearest cluster
					for (uint32 seek_cluster = 0; seek_cluster < ClusterNum; seek_cluster = seek_cluster + 1)
					{
						sfloat Distance = SimilarFunc(DataSet[seek], ClusterCenter[seek_cluster]);

						if (Distance < MinDis)
						{
							MinDis = Distance;
							MinDisIndex = seek_cluster;
						}
					}

					if (MinDisIndex == -1){ return CLUSTERSET<uint32>(); }

					ClusterSet[MinDisIndex].push_back(seek);
				}

				//Recalculate each cluster center
				DataType NewClusterCenter;
				MaxChange = -MAX_FLOAT32_VALUE;

				for (uint32 seek = 0; seek < ClusterNum; seek = seek + 1)
				{
					NewClusterCenter = CenterFunc(ClusterSet[seek]);

					sfloat CurrentChange = SimilarFunc(ClusterCenter[seek], NewClusterCenter);

					ClusterCenter[seek] = NewClusterCenter;

					MaxChange = MaxChange>CurrentChange ? MaxChange : CurrentChange;
				}
			}

			return ClusterSet;
		};
#endif
	}
}