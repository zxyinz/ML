#include"ctime"
#include"..\Core\SanContainer.h"
#include"..\Core\SanMathematics.h"
#pragma once
using namespace std;
using namespace San::Mathematics;
namespace San
{
	namespace MachineLearning
	{
#ifndef __CLINEARLEARNINGALGORITHM_H__
#define __CLINEARLEARNINGALGORITHM_H__

		/*Feature Template Structure Define*/
		template<typename ST>
		struct stFEATURE
		{
			SString	str_name; // Feature Name
			sfloat	x;
			sfloat	weight;
			SHANDLE	param; //User define feature detect function param, be passed to feature_detect_func_ptr when update the x value
			sfloat(*feature_detect_func_ptr)(const ST&, const SHANDLE); // Feature Detect Function Pointer, ST: current state - b, SHANDLE: Param
		};

		template<typename ST>
		class cLinearLearningAlgorithm
		{
		private:
			vector<stFEATURE<ST>>			m_FeatureArray; // V(b) = M0 + M1*X1 + ... + Mn*Xn;
			//vector<vector<stFEATURE<ST>>>	m_StateStack;
			vector<stFEATURE<ST>>			m_HistoryFeatureList;
			list<pair<uint32, ST>>			m_AvailableStateList; // The State learner can choose to make decision, in this case, thie state is the board position
			sfloat							m_Step; // update step, default 0.1
			sfloat							m_Boundary; //convergence boundary, because in this case the feature didn't convergence, so this value currently not use
			//bool							m_bIsLearningState;
			uint32							m_CurrentAvailableStateID; // Current min-value available ID to assign
		public:
			cLinearLearningAlgorithm()
				:m_Step(0.01),
				m_Boundary(0.1),
				//m_bIsLearningState(true),
				m_CurrentAvailableStateID(1)
			{
			};
			~cLinearLearningAlgorithm()
			{
				this->m_FeatureArray.clear();
				this->m_HistoryFeatureList.clear();
				this->m_AvailableStateList.clear();
				this->m_Step = 0.0;
				this->m_Boundary = 0.0;
				//this->m_bIsLearningState = true;
				this->m_CurrentAvailableStateID = 1;
			};
			bool iCreateFeature(const SString &strName, const sfloat InitWeight, const SHANDLE Param, sfloat(*FeatureDetectFunc)(const ST&, const SHANDLE))
			{
				if (strName.empty() || (FeatureDetectFunc == nullptr))
				{
					return false;
				}
				uint32 size = this->m_FeatureArray.size();
				for (uint32 seek = 0; seek < size; seek = seek + 1)
				{
					if (this->m_FeatureArray[seek].str_name == strName)
					{
						return false;
					}
				}
				stFEATURE<ST> Feature;
				Feature.str_name = strName;
				Feature.weight = InitWeight;
				Feature.param = Param;
				Feature.feature_detect_func_ptr = FeatureDetectFunc;
				this->m_FeatureArray.push_back(Feature);
				return true;
			};
			void iReleaseFeature(const SString &strName)
			{
				if (strName.empty())
				{
					return;
				}
				vector<stFEATURE<ST>>::iterator pFeature = this->m_FeatureArray.begin();
				while (pFeature != this->m_FeatureArray.end())
				{
					if (pFeature->str_name == strName)
					{
						pFeature = this->m_FeatureArray.erase(pFeature);
						return;
					}
					pFeature++;
				}
			};
			uint32 iAddAvailableState(const ST& State)
			{
				this->m_AvailableStateList.push_back(make_pair(this->m_CurrentAvailableStateID, State));
				this->m_CurrentAvailableStateID = this->m_CurrentAvailableStateID + 1;
				return this->m_CurrentAvailableStateID - 1;
			};
			void iRemoveAvailableStateByID(const uint32 StateID)
			{
				list<pair<uint32, ST>>::iterator pState = this->m_AvailableStateList.begin();
				while (pState != this->m_AvailableStateList.end())
				{
					if (pState->first == StateID)
					{
						pState = this->m_AvailableStateList.erase(pState);
						return;
					}
					pState++;
				}
			};
			void iRemoveAvailableStateByValue(const ST StateValue)
			{
				list<pair<uint32, ST>>::iterator pState = this->m_AvailableStateList.begin();
				while (pState != this->m_AvailableStateList.end())
				{
					if (pState->second == StateValue)
					{
						pState = this->m_AvailableStateList.erase(pState);
						return;
					}
					pState++;
				}
			};
			void iSetStep(const sfloat Step)
			{
				if (::gloIsFloatEqual(Step, 0.0))
				{
					return;
				}
				this->m_Step = Step;
			};
			void iSetBoundary(const sfloat Boundary)
			{
				if (::gloIsFloatEqual(Step, 0.0))
				{
					return;
				}
				this->m_Boundary = Boundary;
			};
			sfloat iGetStep() const
			{
				return this->m_Step;
			};
			sfloat iGetBoundary() const
			{
				return this->m_Boundary;
			};
			void iFeedback(const sfloat &VTrain)
			{
				if (this->m_HistoryFeatureList.empty())
				{
					return;
				}
				/*Debug Print*/
				::cout << "V(b) = ";
				for (uint32 seek_feature = 0; seek_feature < this->m_HistoryFeatureList.size(); seek_feature = seek_feature + 1)
				{
					::wcout << "\n        " << this->m_FeatureArray[seek_feature].weight << "       \t* " << this->m_FeatureArray[seek_feature].str_name.c_str() << L" +";
				}
				::cout << "\b \n\n";

				::cout << "V-Train(b): " << VTrain << "\n";

				/*Get the V(b) value for current feature list*/
				sfloat Vb = 0.0;
				for (uint32 seek_feature = 0; seek_feature < this->m_HistoryFeatureList.size(); seek_feature = seek_feature + 1)
				{
					Vb = Vb + this->m_HistoryFeatureList[seek_feature].x*this->m_HistoryFeatureList[seek_feature].weight;
				}

				/*Update the weight of V(b), if the current stack feature didn't match the real feature list, it may cause problem*/
				/* Wi = Wi + n( V-train(b) - V(b) ) * Xi */
				::cout << "Updated V(b) =\n";
				for (uint32 seek_feature = 0; seek_feature < this->m_HistoryFeatureList.size(); seek_feature = seek_feature + 1)
				{
					this->m_FeatureArray[seek_feature].weight = this->m_FeatureArray[seek_feature].weight + this->m_Step*(VTrain - Vb)*this->m_HistoryFeatureList[seek_feature].x;
					::wcout << "\t" << this->m_FeatureArray[seek_feature].weight << L"       \t* " << this->m_HistoryFeatureList[seek_feature].x << L" +\n";
				}
				::cout << "\b\b  \n\n";
				this->m_HistoryFeatureList.clear();
			};
			ST iSupervisedLearning(const ST &TrainingData)
			{
				/*Create a feature array which is a copy of the origional feature array, for learnning*/
				vector<stFEATURE<ST>> FeatureArray;
				uint32 FeatureArraySize = this->m_FeatureArray.size();
				for (uint32 seek = 0; seek < FeatureArraySize; seek = seek + 1)
				{
					FeatureArray.push_back(this->m_FeatureArray[seek]);
				}

				/*Find the state*/
				list<pair<uint32, ST>>::iterator pState = this->m_AvailableStateList.begin();
				while (pState != this->m_AvailableStateList.end())
				{
					if (pState->second == TrainingData)
					{
						break;
					}
					pState++;
				}
				if (pState == this->m_AvailableStateList.end())
				{
					::cout << "Error: can not find training data in available data set\n";
					::system("pause");
					::exit(0);
					return TrainingData;
				}
				/*Call Feedback to upsate weight, if there is no history, not update*/
				if (this->m_HistoryFeatureList.empty())
				{
					for (uint32 seek = 0; seek < FeatureArraySize; seek = seek + 1)
					{
						FeatureArray[seek].x = FeatureArray[seek].feature_detect_func_ptr(pState->second, FeatureArray[seek].param);
					}
				}
				else
				{
					/*Get current board value V(Successor(b)) as VTrain to update weight*/
					sfloat VTrain = 0.0;
					for (uint32 seek = 0; seek < FeatureArraySize; seek = seek + 1)
					{
						FeatureArray[seek].x = FeatureArray[seek].feature_detect_func_ptr(pState->second, FeatureArray[seek].param);
						VTrain = VTrain + this->m_FeatureArray[seek].weight * FeatureArray[seek].x;
					}
					this->iFeedback(VTrain);
				}
				for (uint32 seek = 0; seek < FeatureArraySize; seek = seek + 1)
				{
					this->m_HistoryFeatureList.push_back(FeatureArray[seek]);
				}
				return pState->second;
			};
			ST iPredict(const bool bIsLearningMode)
			{
				/*Create a feature array which is a copy of the origional feature array, for learnning*/
				vector<stFEATURE<ST>> FeatureArray;
				uint32 FeatureArraySize = this->m_FeatureArray.size();
				for (uint32 seek = 0; seek < FeatureArraySize; seek = seek + 1)
				{
					FeatureArray.push_back(this->m_FeatureArray[seek]);
				}

				/*Calcuate each available state and select the one has max value as predict result to return*/
				list<pair<uint32, ST>>::iterator pState = this->m_AvailableStateList.begin();
				vector<list<pair<uint32, ST>>::iterator> MaxStateArray;

				sfloat VMax = -FLT_MAX;
				sfloat Vb = 0.0;

				while (pState != this->m_AvailableStateList.end())
				{
					Vb = 0.0;
					for (uint32 seek = 0; seek < FeatureArraySize; seek = seek + 1)
					{
						FeatureArray[seek].x = FeatureArray[seek].feature_detect_func_ptr(pState->second, FeatureArray[seek].param);
						Vb = Vb + this->m_FeatureArray[seek].weight*FeatureArray[seek].x;
					}
					//::cout << "Feature Value: " << CurrentStateValue << "\n";
					if (::gloIsFloatEqual(Vb, VMax))
					{
						MaxStateArray.push_back(pState);
						//MaxStateValue = CurrentStateValue;
					}
					else
					{
						if (Vb > VMax)
						{
							MaxStateArray.clear();
							MaxStateArray.push_back(pState);
							VMax = Vb;
						}
					}
					pState++;
				}

				/*Choose a MaxState*/
				::srand(time(0));
				uint32 StateIndex = ::rand() % MaxStateArray.size();

				/*If in learning mode, update weight by history(if have) and save the current feature state (to history)*/
				if (bIsLearningMode)
				{
					if (this->m_HistoryFeatureList.empty())
					{
						for (uint32 seek = 0; seek < FeatureArraySize; seek = seek + 1)
						{
							FeatureArray[seek].x = FeatureArray[seek].feature_detect_func_ptr(MaxStateArray[StateIndex]->second, FeatureArray[seek].param);
						}
					}
					else
					{
						/*Get current board value V(Successor(b)) as VTrain to update weight*/
						sfloat VTrain = 0.0;
						for (uint32 seek = 0; seek < FeatureArraySize; seek = seek + 1)
						{
							FeatureArray[seek].x = FeatureArray[seek].feature_detect_func_ptr(MaxStateArray[StateIndex]->second, FeatureArray[seek].param);
							VTrain = VTrain + this->m_FeatureArray[seek].weight * FeatureArray[seek].x;
						}
						this->iFeedback(VTrain);
					}
					for (uint32 seek = 0; seek < FeatureArraySize; seek = seek + 1)
					{
						this->m_HistoryFeatureList.push_back(FeatureArray[seek]);
					}
					return MaxStateArray[StateIndex]->second;
				}
				return MaxStateArray[StateIndex]->second;
			};
			void iClearStateStack()
			{
				this->m_HistoryFeatureList.clear();
			};
			void iClearAvailableStateList()
			{
				this->m_AvailableStateList.clear();
			};
		};
#endif
	}
}