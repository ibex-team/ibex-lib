/* ============================================================================
 * I B E X - Structure to map symbols to data of any type T
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Jan 05, 2012
 * ---------------------------------------------------------------------------- */

#ifndef _IBEX_SYMBOL_MAP_H
#define _IBEX_SYMBOL_MAP_H

#include <stdlib.h>
#include <string.h>
#include <iostream>
#include "ibex_NonRecoverableException.h"

#ifdef __GNUC__
// === deprecated headers ===
#include <ext/hash_map>
using __gnu_cxx::hash;
using __gnu_cxx::hash_map;

/* #include <unordered_map> */
/* using std::unordered_map; */

#else
#ifdef __WIN32__
#include <hash_map>
using stdext::hash_compare;
using stdext::hash_map;
#endif
#endif

#ifdef __GNUC__
// deprecated header
#define IBEXMAP(T) hash_map<const char*, T, hash<const char*>, equal_string>
//#define IBEXMAP(T) unordered_map<const char*, T, hash<const char*>, equal_string>
#else
#define IBEXMAP(T) hash_map<const char*, T, hash_compare<const char*,equal_string> >
#endif

namespace ibex {

struct equal_string {
	bool operator() (const char* s1, const char* s2) const {
		return strcmp (s1, s2) == 0;
	}
};

/** \ingroup toolsgroup
 * \brief Structure to map symbol to any data.
 */
template <typename T>
class SymbolMap {
public:

	/** \brief Copy constructor.
	 *
	 * Duplicate all symbols of the table \a tab. */
	SymbolMap(const SymbolMap<T>& tab) {
		typename IBEXMAP(T)::const_iterator it=tab.map.begin();
		for(; it!=tab.map.end(); it++) {
			map.insert_new(it->first, it->second);
		}
	}

	/** \brief Delete this instance. */
	~SymbolMap() {
		clean();
	}

	/** \brief Size of the structure.
	 *
	 * Return the number of elements in the structure */
	size_t size() const {
		return map.size();
	}

	/** \brief Clean the structure
	 *
	 * Erase all the elements. */
	void clean() {
		if (map.size()>0) { // "this" to force the compiler to look for "size" when the template is instantiated.
			typename IBEXMAP(T)::const_iterator it=map.begin();
			typename IBEXMAP(T)::const_iterator it2=map.begin();
			it++;
			for(; it!=map.end(); it++) {
				free((char*) it2->first);; // it2 was introduced to avoid deleting the key [it] is pointing to
				it2 = it;
			}
			free((char*) it2->first);
		}
		map.clear(); // erases all the elements
	}

	/** \brief Return true if the symbol \a id exists in the map. */
	bool used(const char* id) const {
		return map.find(id)!=map.end();
	}

	/** \brief Insert a new identifier \a id with associated data \a data.
	 *
	 * The key is a copy of the string pointed to by \a id, not \a id itself.
	 * \throw  NonRecoverableException- if \a id already exists.
	 * \return - a pointer to the copy of the string used as key. */
	const char* insert_new(const char* id, T data) {
		if (used(id)) throw NonRecoverableException(std::string("Redeclared symbol \"")+id+"\"");

		char* copy = strdup(id);
		map.insert(std::pair<const char*,T>(copy, data));
		return copy;
	}

	/** \brief Remove the pair <key,data> where key is the symbol \a id.
	 */
	void erase(const char* id) {
		if (used(id)) {
			typename IBEXMAP(T)::iterator it = map.find (id);
			const char* name=it->first;
			map.erase(it);
			free((char*) name);
		}
	}

	/** \brief Return the data associated to the symbol \a id.
	 *
	 * \throw NonRecoverableException - if no element could be found with key \a id. */
	const T& data(const char* id) const {
		typename IBEXMAP(T)::const_iterator it = map.find (id);
		if (it == map.end()) throw NonRecoverableException(std::string("Unknown symbol \"")+id+"\"");
		return it->second;
	}

	/** \brief Return the data associated to the symbol \a id.
	 *
	 * \throw NonRecoverableException - if no element could be found with key \a id. */
	T& data(const char* id) {
		return (T&) ((const SymbolMap<T>*) this)->data(id);
	}

	/** \brief Output the hash table. */
	friend std::ostream& operator<<(std::ostream& os, const SymbolMap<T>& idtab) {
		int i=0;
		for (typename IBEXMAP(T)::const_iterator it=idtab.begin(); it!=idtab.end(); it++) {
			os << it->first << " " << it->second << "    ";
			if (++i%8==0) os << std::endl;
		}
		return os;
	}

private:
	IBEXMAP(T) map;

};

} // end namespace

#endif // end _IBEX_SYMBOL_MAP_
