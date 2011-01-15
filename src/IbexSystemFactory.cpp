/*---------------------------------------------------------------------------------
 * 
 * System factories
 * ----------------
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

#include "IbexSystemFactory.h"
#include "IbexEnv.h"
#include "IbexIdTable.h"
#include "IbexContractor.h"

namespace ibex {

SystemSpaceFactory::SystemSpaceFactory(const Space& base_space, const vector<const Constraint*>& ctrs, bool syb_by_default) :
  SpaceFactory(base_space.env), base_space(base_space) {
  for (int i=0; i<base_space.env.nb_keys(); i++) {
    if (base_space.used(i)) {
      for (unsigned int j=0; j<ctrs.size(); j++)
	if (ctrs[j]->adj.find(i)!=ctrs[j]->adj.end()) {
	  const Entity& e(base_space.entity(i));
	  SpaceFactory::set_entity(i, syb_by_default? IBEX_SYB : e.type, *e.domain);
	}
    }
  }
}
  
void SystemSpaceFactory::set_entity(EntityType basetype, int index, EntityType newtype) {
  SpaceFactory::set_entity(base_space.key(basetype,index), newtype, base_space.domain(basetype,index)); 
}

void SystemSpaceFactory::set_entity(const char* symbol_name, EntityType newtype) {  
  const Entity& e(base_space.entity(symbol_name));
  set_entity(e.type, base_space.component(e.key), newtype);
}


} // end namespace
