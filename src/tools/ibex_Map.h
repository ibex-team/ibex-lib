/*
 * ibex_PropertyMap.h
 *
 *  Created on: Jun 18, 2018
 *      Author: gilles
 */

#ifndef __IBEX_MAP_H__
#define __IBEX_MAP_H__

#ifdef __GNUC__
#include <ciso646> // just to initialize _LIBCPP_VERSION
#ifdef _LIBCPP_VERSION
#include <unordered_map>
#define __ibex_map__(T) std::unordered_map<long, T*>
#else
#include <tr1/unordered_map>
#define __ibex_map__(T) std::tr1::unordered_map<long, T*>
#endif
#else
#if (_MSC_VER >= 1600)
#include <unordered_map>
#define __ibex_map__(T) std::unordered_map<long, T*>
#else
#include <unordered_map>
#define __ibex_map__(T) std::tr1::unordered_map<long, T*>
#endif // (_MSC_VER >= 1600)
#endif

#include <utility>
#include <stdlib.h>
#include <cassert>

namespace ibex {

template<class T>
class Map : public __ibex_map__(T) {
public:
	/**
	 * \brief True if e is in the map
	 */
	bool found(long id) const {
		return __ibex_map__(T)::find(id)!=__ibex_map__(T)::end();
	}

	/**
	 * \brief Insert a new pair <id,property>.
	 */
	void insert_new(long id, T* p) {
		assert(!found(id));
		__ibex_map__(T)::insert(std::make_pair(id,p));
	}

	/**
	 * \brief Return the data associated to the symbol \a id.
	 *
	 * \pre - an element with key \a id must exist. */
	const T* operator[](long id) const {
		typename __ibex_map__(T)::const_iterator it = __ibex_map__(T)::find (id);
		if (it == __ibex_map__(T)::end()) return NULL;
		return it->second;
	}

	/**
	 * \brief Return the data associated to the symbol \a id.
	 *
	 * \pre - an element with key \a id must exist.
	 */
	T* operator[](long id) {
		return (T*) ((const Map*) this)->operator[](id);
	}

};

} /* namespace ibex */

#endif /* __IBEX_MAP_H__ */
