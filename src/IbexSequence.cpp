/*---------------------------------------------------------------------------------
 * 
 * Sequence contractor (constraint satisfaction problem)
 * ------------------------------------------------------
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

#include "IbexSequence.h"
#include "IbexEnv.h"
#include "IbexCSP.h"
#include "IbexHC4Revise.h"

namespace ibex {


Sequence::Sequence(const ContractorList& ctc, Space& space, bool logic_op) :
  Operator(space), list(ctc), logic_op(logic_op) { 

  if (list.empty()) throw NonRecoverableException("Empty composite contractor");
  for (int i=0; i<list.size(); i++) {
    this->list(i).contract_floor=0;
  }
}

Sequence::Sequence(const CSP& csp) : Operator(csp.space), list(ToHC4Revise(), csp), logic_op(true) { 
  for (int i=0; i<list.size(); i++) {
    this->list(i).contract_floor=0;
  }
}

Sequence::Sequence(const ContractorList& ctc) :
  Operator(ctc.empty()? *new Space(*new Env()) : ctc(0).space), list(ctc), logic_op(logic_op) { 

  if (list.empty()) {
    const Env& env=space.env;
    delete &space;
    delete &env;
    throw NonRecoverableException("Empty composite contractor");
  }
  for (int i=0; i<list.size(); i++) {
    this->list(i).contract_floor=-1;
  }
}

Sequence::Sequence(const Sequence& seq) : 
  Operator(seq.space), list(seq.list), logic_op(seq.logic_op) {
}

/** Generic constraint copy (spaces are passed by reference).*/
Sequence* Sequence::copy() const {
  return new Sequence(*this); //SequenceCopy(*this).build_ctr();
}

Sequence::Sequence(const Contractor& c1, const Contractor& c2, bool logic_op) : 
  Operator(c1.space), list(c1,c2), logic_op(logic_op) {
   this->list(0).contract_floor = -1;
   this->list(1).contract_floor = -1;

}

Sequence operator&(const Contractor& c1, const Contractor& c2) {
  return Sequence(c1,c2,true);
}

Sequence operator|(const Contractor& c1, const Contractor& c2) {
  return Sequence(c1,c2,false);
}

void Sequence::contract() {  

  const Indicators* seq_indic=current_indic();

  /* ------------- logical and ---------------------*/
  if (logic_op) {
  
    bool reduced=false;

    for (int i=0; i<list.size(); i++) {

      if (seq_indic) {

	// if no contraction happened so far and the
	// next subcontractor shares the same space with the 
	// Sequence, then the "impact" indicator can be 
	// transmitted to the subcontractor.	
	/*
 	int impact;
	
	if (!reduced && (&this->space==&list(i).space)) 
	  impact=seq_indic->impact;
	else 
	  impact=ALL_VARS; // if the space is different, we don't know its impact
	

 	
	// remark 1: even if the spaces do not coincide, the "scope" indicator
	// can be transmitted.

	// remark 2: when contract_floor=0 for all subcontractors of the list
	// (which is the case by default), only the first subcontractor can 
	// get the "impact" indicator (since reduced=true after the first contraction)
	//	reduced |= list(i).contract(space, Indicators(impact, seq_indic->scope));
	*/
	reduced |= list(i).contract(space, *seq_indic);
      }

      else {
	list(i).contract(space);
      }
    }

  }
  
  /* ------------- logical or ----------------------*/
  else {

    INTERVAL_VECTOR initbox(space.box);
    INTERVAL_VECTOR initepx(space.epx);
    INTERVAL_VECTOR hull(space.box);
    
    hull.set_empty();

    for (int i=0; i<list.size(); i++) {

      space.box = initbox; // restore domains 

      try {

	bool reduced;
	
	if (seq_indic) {

	  // if the subcontractor shares the same space with
	  // the sequence, both indicators can be transmitted
	  // (otherwise, only the "scope").
	  int impact;
	  if (&this->space==&list(i).space) 
	    impact=seq_indic->impact;
	  else 
	    impact=ALL_VARS; // if the space is different, we don't know its impact

	  reduced = list(i).contract(space, Indicators(impact, seq_indic->scope));

	} else {

	  reduced = list(i).contract(space);
	  
	}

	if (reduced)
	  hull |= space.box;		
	else {
	  /* if one contractor did not reduce anything: */
	  /* restore domains  */
	  space.box = initbox; 
	  /* useless to go on  */
	  return;
	}
      } catch(EmptyBoxException) { }
    }    

    if (hull.empty()) throw EmptyBoxException();

    space.box = hull;        
    space.epx = initepx;
  }
}



} // end namespace
