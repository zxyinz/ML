#include"iostream"
#include"memory"
#include"unordered_map"
#include"../Core/SanMemory.h"
#include"../Core/SanContainer.h"
#pragma once
using namespace std;
namespace San
{
	namespace MachineLearning
	{
#ifndef __CSANDATACONTAINER_H__
#define __CSANDATACONTAINER_H__
		template<class _data, class _label>
		class cSanDataContainer : public _smemobj<>
		{
		private:
			typedef list<shared_ptr<_data>> _data_list;
		private:
			unordered_map<_label, _data_list >> m_Container;
			unordered_map<uint32, _data_list::iterator> m_LookupTable;
		public:
			cSanDataContainer();
			~cSanDataContainer();

			uint32 iInsert(const _data &Data, const _label &Label);
		};
#endif
	}
}