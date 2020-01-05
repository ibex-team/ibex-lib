//============================================================================
//                                  I B E X                                   
// File        : IBEX_OPTIM_MEMORY.h
// Author      : Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Jul 05, 2014
// Last Update : Oct 18, 2019
//============================================================================

#ifndef __IBEX_OPTIM_MEMORY_H__
#define __IBEX_OPTIM_MEMORY_H__

#include "ibex_Memory.h"
#include "ibex_LoupFinder.h"
#include "ibex_LoupFinderDefault.h"
#include <type_traits>

namespace ibex {

/**
 * \ingroup tools
 *
 * This class is used to record the data created dynamically
 * in the context of optimization, to ease disallocation.
 *
 * It extends the Memory::Object class to record LoupFinder objects.
*/

class OptimObject : public Memory::Object {
public:
	enum { LOUP_FINDER = __NB_TAGS__};

	OptimObject(const LoupFinder* data) : Object(data, LOUP_FINDER) { }

	~OptimObject() { delete (LoupFinder*) data; }
};

template<>
inline LoupFinder& Memory::rec(LoupFinder* ptr) {
	objs.push_front(new OptimObject((LoupFinder*) ptr));
	return *ptr;
}

// note: g++ calls Memory::rec<T> instead of the previous
// function specialization when T is a subclass of LoupFinder
// which is sad. So we need the following declaration:
template<>
inline LoupFinderDefault& Memory::rec(LoupFinderDefault* ptr) {
	objs.push_front(new OptimObject((LoupFinderDefault*) ptr));
	return *ptr;
}

template<>
inline LoupFinder& Memory::rec<LoupFinder>(LoupFinder* ptr, int tag) {
	tags.insert(std::make_pair(tag, (void*) ptr));
	objs.push_front(new OptimObject(ptr));
	return *ptr;
}

} // end namespace ibex

#endif // __IBEX_OPTIM_MEMORY__
