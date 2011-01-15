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

#include "IbexSharedObject.h"
#include <assert.h>


namespace ibex {

SharedObject::SharedObject() {
  refs[id=ids++]=pair<SharedObject*, int>(this,1);
}

SharedObject::~SharedObject() { }

void SharedObject::incr_ref(int id) {
  assert(has_ref(id));
  refs[id].second++;
}
  
void SharedObject::decr_ref(int id) {
  assert(has_ref(id));
  int k=--refs[id].second;
  if (k==0) {
    SharedObject* o=refs[id].first;
    refs.erase(id);
    delete o;
  }
}
  
int SharedObject::nb_ref(int id) {
  if (refs.find(id)==refs.end()) return 0;
  else return refs[id].second;
}

bool SharedObject::has_ref(int id) {
  return (refs.find(id)!=refs.end());
}

hash_map<int,pair<SharedObject*,int> > SharedObject::refs;
int SharedObject::ids=0;

} // end namespace
