/*---------------------------------------------------------------------------------
 * 
 * Shared objects manager (spaces and environments)
 * ------------------------------------------------
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

#ifndef _IBEX_SHARED_OBJECT_H
#define _IBEX_SHARED_OBJECT_H

#include <iostream>
#include <ext/hash_map>
#include <assert.h>

using __gnu_cxx::hash;
using __gnu_cxx::hash_map;
using std::pair;
using std::cout;
using std::endl;

namespace ibex {


/** \ingroup miscgroup
 *
 * \brief Manager for shared objects.
 *
 * Allows to keep track of the references to an object, in all ibex structures.
 *
 * \author Gilles Chabert
 * \date May 2009
 */
class SharedObject {
 public:

  /** Unique identifier for this instance. */
  int id;

  /** Create a new object and assign a new unique
   * identifier to it. */
  SharedObject();

  /** Delete the object. */
  virtual ~SharedObject();

  /** Increment the number of references to the object
   * whose identifying number is n. */
  static void incr_ref(int id);
  
  /** Decrement the number of references to the object
   * whose identifying number is n. If the object has
   * no more reference, it is deleted. */
  static void decr_ref(int id);
  
 private:
  /** Return true if the object is referenced at least
   * once. */
  static  bool has_ref(int id);

  /** For debug */
  static int nb_ref(int id);

  /** Last generated identifier */
  static int ids;

  /** refs[id] is a pair containing an object and the
   * number of references to it */
  static hash_map<int,pair<SharedObject*,int> > refs;
};

} // end namespace

#endif
