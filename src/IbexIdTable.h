/*---------------------------------------------------------------------------------
 * 
 * A mapping table for identifiers (symbols)
 * ----------------------------------------
 *
 * Copyright (C) 2007-2009 Gilles Chabert
 * 
 * This file is part of IBEX.
 *
 * IBEX is free software; you can redistribute it and/or modify it under the terms of 
 * the GNU General Public License as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later version.
 *
 * IBEX is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; 
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
 * PURPOSE. See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with IBEX; 
 * if not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, 
 * Boston, MA 02110-1301, USA 
 *
 --------------------------------------------------------------------------------*/

#ifndef _IBEX_ID_TABLE_H
#define _IBEX_ID_TABLE_H

#include <stdlib.h>
#include <iostream>
#include "IbexException.h"

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

/* \ingroup miscgroup
 * \brief Identifier table.
 *
 * \sa #IdException.
 * \author Gilles Chabert
 * \date March 2007
 */
template <typename T>
class IdTable {

 public:

  /** Delete this instance. */
  ~IdTable() {    
    clean();
  }

  /** Clean the hash map (erases all the elements). */
  void clean() {
    if (map.size()>0) { // "this" to force the compiler to look for "size" when the template is instanciated.
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

  IdTable<T>() { }

  /** Duplicate the table */
/*  IdTable<T>(const IdTable<T>& tab) {
    typename IdTable::iterator it=tab.begin();
k
    for(; it!=tab.end(); it++) {
      map.insert_new(it->first, it->second);
    }
  }
  */

  /** Return true if \a id is the key of one element. */
  bool used(const char* id) const { 
    return map.find(id)!=map.end(); 
  }
  
  /** Insert a new identifier \a id with associated data \a data.
   * The key is a copy of the string pointed to by \a id, not \a id itself.
   * \throw RedeclaredIdException - if \a id already exists.
   * \return - a pointer to the copy of the string used as key. */
  const char* insert_new(const char* id, T data) {
    if (used(id)) throw NonRecoverableException(std::string("Redeclared symbol \"")+id+"\"");
    
    char* copy = strdup(id);
    map.insert(std::pair<const char*,T>(copy, data));
    return copy;
  }
  
  void erase(const char* id) {
    if (used(id)) {
      typename IBEXMAP(T)::iterator it = map.find (id);      
      const char* name=it->first;	
      map.erase(it);
      free((char*) name);
    }
  }

  /** Return the data associated to the key \a id.
   * \throw UnknownIdException - if no element could be found with key \a id. */
  const T& data(const char* id) const {
    typename IBEXMAP(T)::const_iterator it = map.find (id);
    if (it == map.end()) throw NonRecoverableException(std::string("Unknown symbol \"")+id+"\"");
    return it->second;
  }

  /** Return the data associated to the key \a id.
   * \throw UnknownIdException - if no element could be found with key \a id. */
  T& data(const char* id) {
    return (T&) ((const IdTable<T>*) this)->data(id);
  }
  
  /** Output the hash table */
  friend std::ostream& operator<<(std::ostream& os, const IdTable<T>& idtab) {
    int i=0;
    for (typename IBEXMAP(T)::const_iterator it=idtab.begin(); it!=idtab.end(); it++) {
      os << it->first << " " << it->second << "    ";
      if (++i%8==0) os << std::endl;
    }
    return os;
  }

 //private:
  IBEXMAP(T) map;
     
};

} // end namespace
  
#endif
