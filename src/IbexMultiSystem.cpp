/*---------------------------------------------------------------------------------
 * 
 * Multi-System (system of systems of equations)
 * --------------------------------------------
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

#include "IbexMultiSystem.h"
#include "IbexSystemFactory.h"

namespace ibex {

MultiSystem::MultiSystem(const vector<const System*> syslist) : nb(syslist.size()) {
 
  _subsys = new System*[syslist.size()]; 

  int i=0;
  for (vector<const System*>::const_iterator it=syslist.begin(); it!=syslist.end(); it++) 
    _subsys[i++]=new System(**it);
}

MultiSystem::MultiSystem(const System& sys, const BlockSet& bs) : nb(bs.nb_blk) {

  vector<const ArithConstraint*> sublist[bs.nb_blk];
  
  for (int ctr=0; ctr<sys.nb_ctr(); ctr++) {
    int max_blk=-1;                           // the greatest blk number of the variables involved in the constraint
    const hash_map<int,int>& adj=sys.ctr(ctr).adj;
    for (hash_map<int,int>::const_iterator it = adj.begin(); it!=adj.end(); it++) {
      int var = it->first; 
      if (sys.space.entity(var).type!=IBEX_VAR) continue;
      if (bs.noblk(sys.space.component(var))) { max_blk = bs.nb_blk; break; } // if var is not in a block, we set blk to "past the end".
      int blk = bs.blk(sys.space.component(var)); 
      if (blk>max_blk) max_blk=blk;
    }
    if (max_blk<bs.nb_blk  && max_blk!=-1) {
      sublist[max_blk].push_back(&sys.ctr(ctr));
    }
  }      
  
  _subsys = new System*[bs.nb_blk];

  for (int blk=0; blk<bs.nb_blk; blk++) {
    if (bs.empty(blk)) {
      delete[] _subsys;
      throw BlockSetException(BlockSetException::EMPTY_BLK);
    }
    
    SystemSpaceFactory fac(sys.space, (const vector<const Constraint*>&) sublist[blk], true);

    int var=bs.first(blk); 
    do {
      fac.set_entity(IBEX_VAR, var, IBEX_VAR);
      var=bs.next(var);
    } while(var!=bs.first(blk));
    
    _subsys[blk] = new System(sublist[blk],fac);
  }

}

MultiSystem::~MultiSystem() { 

  for (int i=0; i<size(); i++) 
    delete _subsys[i];
  delete[] _subsys; 
}

} // end namespace
