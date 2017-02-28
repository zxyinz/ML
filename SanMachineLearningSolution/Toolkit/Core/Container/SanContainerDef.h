//Project: San Lab Game Engine
//Version: 2.1.1
//Debug State: Add pair
#include"vector"
#include"list"
#include"queue"
#include"stack"
#include"set"
#include"map"
#include"unordered_set"
#include"unordered_map"
#include"functional"
#include"algorithm"
#include"bitset"
#include"mutex"
#include"atomic"
#include"../SanMemory.h"
#include"../SanTypes.h"
#pragma once
using namespace std;
namespace San
{
#ifndef __STDSANCONTAINERDEF_H__
#define __STDSANCONTAINERDEF_H__

	template<class ST1, class ST2, class _Allocator = cSanSystemAllocator>
	struct _SANUNKNOWN : public _smemobj<_Allocator>
	{
	public:
		ST1 first;
		ST2 second;
		/*union{ ST1 description; ST1 first; };
		union{ ST2 value; ST2 second; };*/
	public:
		_SANUNKNOWN();
		_SANUNKNOWN(const ST1 &First, const ST2 &Second);
		~_SANUNKNOWN();

		bool operator==(const _SANUNKNOWN<ST1, ST2> &Variable) const;
		bool operator!=(const _SANUNKNOWN<ST1, ST2> &Variable) const;
	};

#pragma region _SANUNKNOWN CPP file
	template<class ST1, class ST2, class _Allocator>
	_SANUNKNOWN<ST1, ST2, _Allocator>::_SANUNKNOWN()
	{
	}
	template<class ST1, class ST2, class _Allocator>
	_SANUNKNOWN<ST1, ST2, _Allocator>::_SANUNKNOWN(const ST1 &First, const ST2 &Second)
		:first(First), second(Second)
	{
	}
	template<class ST1, class ST2, class _Allocator>
	_SANUNKNOWN<ST1, ST2, _Allocator>::~_SANUNKNOWN()
	{
	}
	template<class ST1, class ST2, class _Allocator>
	bool _SANUNKNOWN<ST1, ST2, _Allocator>::operator==(const _SANUNKNOWN<ST1, ST2> &Variable) const
	{
		return (this->first == Variable.first) && (this->second == Variable.second);
	}
	template<class ST1, class ST2, class _Allocator>
	bool _SANUNKNOWN<ST1, ST2, _Allocator>::operator!=(const _SANUNKNOWN<ST1, ST2> &Variable) const
	{
		return (this->first != Variable.first) || (this->second != Variable.second);
	}
#pragma endregion

	template<class _First, class _Second, class _Allocator = cSanSystemAllocator>
	using _spair = _SANUNKNOWN<_First, _Second, _Allocator>;

#endif
}