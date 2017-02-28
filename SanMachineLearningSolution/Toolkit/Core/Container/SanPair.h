//Project: San Lab Game Engine
//Version: 2.1.1
//Debug State: Need test
#include"SanContainerDef.h"
#pragma once
using namespace std;
namespace San
{
#ifndef __STDSANPAIR_H__
#define __STDSANPAIR_H__

#define _SAN_PAIR_DEF(__NAME__, __KEY_TYPE__, __KEY__, __VAL_TYPE__, __VAL__, __COMPARATOR__, __ALLOCATOR__) \
	struct __NAME__ : public _smemobj<__ALLOCATOR__>\
	{\
	public:\
		__KEY_TYPE__ __KEY__; \
		__VAL_TYPE__ __VAL__; \
	public:\
		__NAME__(){}; \
		__NAME__(const __KEY_TYPE__ &__KEY__, const __VAL_TYPE__ &__VAL__) :__KEY__(__KEY__), __VAL__(__VAL__){}; \
		~__NAME__(){}; \
		\
		bool operator==(const __NAME__ &Var) const { return (this->__KEY__ == Var.__KEY__) && (this->__VAL__ == Var.__VAL__); }; \
		bool operator!=(const __NAME__ &Var) const { return (this->__KEY__ != Var.__KEY__) || (this->__VAL__ != Var.__VAL__); }; \
		\
		__COMPARATOR__; \
	}\

	template<class ST1, class ST2, class _Allocator = cSanSystemAllocator>
	struct _SANUNKNOWN : public _smemobj<_Allocator>
	{
	public:
		ST1 first;
		ST2 second;
	public:
		_SANUNKNOWN(){};
		_SANUNKNOWN(const ST1 &First, const ST2 &Second) :first(First), second(Second){};
		~_SANUNKNOWN(){};

		bool operator==(const _SANUNKNOWN<ST1, ST2> &Var) const { return (this->first == Var.first) && (this->second == Var.second); };
		bool operator!=(const _SANUNKNOWN<ST1, ST2> &Var) const { return (this->first != Var.first) || (this->second != Var.second); };
	};

	template<class _First, class _Second, class _Allocator = cSanSystemAllocator>
	using _spair = _SANUNKNOWN<_First, _Second, _Allocator>;
#endif
}