//============================================================================
//                                  I B E X
// File        : ibex_Map.h
// Author      : Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Jun 18, 2018
//============================================================================

#ifndef __IBEX_MAP_H__
#define __IBEX_MAP_H__

#include "ibex_Exception.h"

#ifdef __GNUC__
#include <ciso646> // just to initialize _LIBCPP_VERSION
#ifdef _LIBCPP_VERSION
#include <unordered_map>
#define __ibex_map__(T) std::unordered_map<long, T>
#else
#include <tr1/unordered_map>
#define __ibex_map__(T) std::tr1::unordered_map<long, T>
#endif
#else
#if (_MSC_VER >= 1600)
#include <unordered_map>
#define __ibex_map__(T) std::unordered_map<long, T>
#else
#include <unordered_map>
#define __ibex_map__(T) std::tr1::unordered_map<long, T>
#endif // (_MSC_VER >= 1600)
#endif

#include <utility>
#include <stdlib.h>
#include <type_traits>
#include <cassert>

namespace ibex {

/**
 * \ingroup tools
 *
 * \brief Map a number to something, either by reference or copy.
 *
 * Generic map structure where the key is a number (long)
 * and the value a reference/copy to an object of class T.
 *
 */
template<class T, bool REF=true>
class Map {
public:
	// T& -> by reference or T -> by copy
	typedef typename std::conditional<REF,T&,T>::type RefOrCopy;
	typedef typename std::conditional<REF,T*,T>::type StorageType;

	/**
	 * \brief Iterator
	 *
	 * If REF==true, the iterator points to a pair <long,T*>
	 * If REF==false, the iterator points to a pair <long,T>
	 */
	typedef typename __ibex_map__(StorageType)::iterator iterator;

	/**
	 * \brief Const iterator.
	 *
	 * If REF==true, the iterator points to a pair <long,T*>
	 * If REF==false, the iterator points to a pair <long,T>
	 */
	typedef typename __ibex_map__(StorageType)::const_iterator const_iterator;

	__ibex_map__(StorageType) map;

	/**
	 * \brief Thrown when the identifier does not exist.
	 */
	class NotFound : Exception { };

	/**
	 * \brief True if id is in the map
	 */
	bool found(long id) const {
		return map.find(id)!=map.end();
	}

	/**
	 * \brief Insert a new pair <id,p>.
	 *
	 * If REF==true, p is passed by reference (T&).
	 * If REF==false, p is passed by copy (T).
	 */
	void insert_new(long id, RefOrCopy p) {
		assert(!found(id));

		map.insert(std::make_pair(id,
				pointer_or_ref(p, typename std::conditional<REF,std::true_type,std::false_type>::type())));
	}

	/**
	 * \brief Return a reference to the data associated to the key \a id.
	 *
	 * \throw - A NotFound exception if no element with \id exists.
	 */
	const T& operator[](long id) const {
		const_iterator it = map.find (id);
		if (it == map.end()) throw NotFound();
		return deref_or_ref(it->second, typename std::conditional<REF,std::true_type,std::false_type>::type());
	}

	/**
	 * \brief Return a reference to the data associated to the key \a id.
	 *
	 * \throw - A NotFound exception if no element with \id exists.
	 */
	T& operator[](long id) {
		return (RefOrCopy&) ((const Map*) this)->operator[](id);
	}

	/**
	 * \brief Iterator pointing to the first pair <key,T&>.
	 */
	iterator begin() {
		return map.begin();
	}

	/**
	 * \brief Iterator pointing to past-the-end element.
	 */
	iterator end() {
		return map.end();
	}

	/**
	 * \brief Const iterator pointing to the first pair <key,T&>.
	 */
	const_iterator begin() const {
		return map.begin();
	}

	/**
	 * \brief Const iterator pointing to past-the-end element.
	 */
	const_iterator end() const {
		return map.end();
	}

private:
	T* pointer_or_ref(T& x, std::true_type) { // called when REF=true
		return &x;
	}

	T& pointer_or_ref(T& x, std::false_type) { // called when REF=false
		return x;
	}

	const T& deref_or_ref(const T* x, std::true_type) const { // called when REF=true
		return *x;
	}

	const T& deref_or_ref(const T& x, std::false_type) const { // called when REF=false
		return x;
	}

};

} /* namespace ibex */

#endif /* __IBEX_MAP_H__ */
