//============================================================================
//                                  I B E X                                   
// File        : ibex_Memory.h
// Author      : Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Jul 05, 2014
// Last Update : Nov 21, 2017
//============================================================================

#ifndef __IBEX_MEMORY_H__
#define __IBEX_MEMORY_H__

#include "ibex_System.h"
#include "ibex_Bsc.h"
#include "ibex_Ctc.h"
#include "ibex_CellBuffer.h"

#ifdef _IBEX_WITH_OPTIM_ // TODO make Memory::Object more generic
#include "ibex_LoupFinder.h"
#endif

#include "ibex_Linearizer.h"

#include <stdlib.h>
#include <list>

#ifdef __GNUC__
#include <ciso646> // just to initialize _LIBCPP_VERSION
#ifdef _LIBCPP_VERSION
#include <unordered_map>
#else
#include <tr1/unordered_map>
#endif
#else
#if (_MSC_VER >= 1600)
#include <unordered_map>
#else
#include <unordered_map>
#endif // (_MSC_VER >= 1600)
#endif

namespace ibex {

/**
 * \ingroup tools
 *
 * This class is used to record the data created dynamically
 * (see, e.g., DefaultSolver, DefaultOptimizer), to ease disallocation.
 *
 * It implements a kind of smart pointer with class scope.
 *
 * Typical data include contractors, a bisector, etc.
 *
 */
class Memory {
public:

	class Object {
	public:
		const void* data;

		/* Note: if we had a root class IbexObject for all classes (Ctc, Bsc,
		 * etc.) the implementation would be more direct.
		 * We need to record the type of the object (because the "delete" operator
		 * requires static type cast).
		 */
		enum { CTC, BSC, SYSTEM, LOUP_FINDER, CELL_BUFFER, LINEARIZER } type;

		Object(const Ctc* obj) : data(obj), type(CTC) { }
		Object(const Bsc* obj) : data(obj), type(BSC) { }
		Object(const System* obj) : data(obj), type(SYSTEM) { }
#ifdef _IBEX_WITH_OPTIM_
		Object(const LoupFinder* obj) : data(obj), type(LOUP_FINDER) { }
#endif
		Object(const CellBuffer* obj) : data(obj), type(CELL_BUFFER) { }
		Object(const Linearizer* obj) : data(obj), type(LINEARIZER) { }

		~Object();
	};


	template<class T>
	T& rec(T* ptr);

	template<class T>
	T& rec(T* ptr, int tag);

	bool found(int tag) const;

	template<class T>
	T& get(int tag) const;

	~Memory();

private:
	std::list<Object*> objs;

#ifdef __GNUC__
#ifdef _LIBCPP_VERSION
std::unordered_map<int, void*> tags;
#else
std::tr1::unordered_map<int, void*> tags;
#endif
#else
#if (_MSC_VER >= 1600)
std::unordered_map<int, void*> tags;
#else
std::tr1::unordered_map<int, void*> tags;
#endif // (_MSC_VER >= 1600)
#endif
};

/*============================================ inline implementation ============================================ */

template<class T>
inline T& Memory::rec(T* ptr) {
	objs.push_front(new Object(ptr));
	return *ptr;
}

template<class T>
inline T& Memory::rec(T* ptr, int tag) {
	tags.insert(std::make_pair(tag,ptr));
	objs.push_front(new Object(ptr));
	return *ptr;
}

inline bool Memory::found(int tag) const {
	return tags.find(tag)!=tags.end();
}

template<class T>
inline T& Memory::get(int tag) const {
	return *((T*) (tags.find(tag)->second));
}

} // end namespace ibex

#endif // __IBEX_MEMORY__
