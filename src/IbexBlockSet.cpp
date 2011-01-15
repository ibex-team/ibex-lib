/*---------------------------------------------------------------------------------
 * 
 * Set of variables block (partition)
  * --------------------------------
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

#include "IbexBlockSet.h"

namespace ibex {

BlockSet::BlockSet(int nb_var, int nb_blk) throw(BlockSetException) : nb_var(nb_var), nb_blk(nb_blk) { 
  
  if (nb_blk<=0 || nb_blk>nb_var) throw BlockSetException(BlockSetException::BAD_NB_BLK); // nothing to clean up

  var2blk = new int[nb_var]; 
  nxt    = new int[nb_var]; 
  prv    = new int[nb_var]; 

  for (int v=0; v<nb_var; v++) { 
    var2blk[v]=-1; 
    nxt[v]=prv[v]=v;
  }

  blk2var = new int[nb_blk];
  blksize = new int[nb_blk];

  for (int b=0; b<nb_blk; b++) { 
    blk2var[b]=-1; 
    blksize[b]=0; 
  }
}

#define copy(x,size) x((int*) memcpy(new int[bs.size], bs.x, bs.size*sizeof(int)))

BlockSet::BlockSet(const BlockSet& bs) : 
  nb_var(bs.nb_var), nb_blk(bs.nb_blk),
  copy(var2blk,nb_var), copy(nxt,nb_var), copy(prv,nb_var),
  copy(blk2var,nb_blk), copy(blksize,nb_blk) { }

BlockSet::~BlockSet() {
  delete[] var2blk;
  delete[] nxt;
  delete[] prv;
  delete[] blk2var;
  delete[] blksize;
}
    
/** remove a variable from the blocks */  
void BlockSet::rem_var(int var) throw(BlockSetException) { 

  int blk=this->blk(var);                  // may throw NO_BLK exception
    
  if (nxt[var]==var) {                     // the variable is the only one of the block
    blk2var[blk]=-1;                       // the block is now empty
  } else {
    if (blk2var[blk]==var) 
      blk2var[blk]=nxt[var];               // the variable is the first of the block
    nxt[prv[var]]=nxt[var];                // skip the variable in the block (forward)
    prv[nxt[var]]=prv[var];                // skip the variable in the block (backward)
  }

  var2blk[var]=-1;
  blksize[blk]--;
}

/* insert var before next */
void BlockSet::insert(int var, int next) { 
  int prev = prv[next];
  nxt[prev]= var;                          // insert the var (forward)
  nxt[var] = next;                         // insert the var (forward)
  prv[var] = prev;                         // insert the var (backward)
  prv[next]= var;                          // insert the var (backward)
}

void BlockSet::set_blk(int var, int blk) throw(BlockSetException) { 
  if (var>=nb_var) throw BlockSetException(BlockSetException::BAD_VAR); 
  if (blk>=nb_blk) throw BlockSetException(BlockSetException::BAD_BLK); 
  
  try { rem_var(var); }
  catch(...) { };

  var2blk[var]=blk; 

  if (empty(blk)) {
    blk2var[blk]=var;                      // if the block was empty, just add the var 
    prv[var]=nxt[var]=var;                 // nxt[var] and prv[var] was already set ONLY if var was 
                                           // the LAST to be removed
  }
  else {
    int first = blk2var[blk];
    int last = prv[first];

    if (var<first) blk2var[blk] = var;     // var becomes the first of the block
    if (var<first || var>last) 
      insert(var,first);                   // insert var between the last and the first
    else {
      while (first<var) first=nxt[first];  // look for the first one smaller than var
      insert(var,first);
    }
  }    

  blksize[blk]++;

}

std::ostream& operator<<(std::ostream& os, const BlockSet& bs) {
  for (int blk=0; blk<bs.nb_blk; blk++) {
    os << blk << ":(";
    if (!bs.empty(blk)) {
      int var;
      for (var=bs.first(blk); var!=bs.last(blk); var=bs.next(var)) 
	os << var << " ";      
      os << var;
    }
    os << ") ";
  }
  return os;
}


std::ostream& operator<<(std::ostream& os, const BlockSetException& e) {

  switch (e.type) {
  case BlockSetException::BAD_NB_BLK: 
    os << "The number of blocks is invalid (typically, greater than the number of variable)"; 
    break;    
  case BlockSetException::BAD_VAR:    
    os << "The variable number is invalid "; 
    break;
  case BlockSetException::BAD_BLK:    
    os << "The block number is invalid"; 
    break;
  case BlockSetException::EMPTY_BLK:  
    os << "The block is empty"; 
    break;
  case BlockSetException::NO_BLK:     
    os << "The variable does not belong to a block"; 
    break;
  case BlockSetException::ALL_EMPTY:     
    os << "All the blocks are empty"; 
    break;
  }
  return os;
}

} // end namespace
