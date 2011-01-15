/*---------------------------------------------------------------------------------
 * 
 * Generic Contraction for system solving.
 * --------------------------------------------------------------------------
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

#include "IbexContractor.h"

namespace ibex {

const int Contractor::ALL_VARS = -1;

const int Contractor::NO_VAR = -2;

bool Contractor::contract(Space& space) {
  INTERVAL_VECTOR* tmpbox;

  if (contract_floor>0) tmpbox = new INTERVAL_VECTOR(space.box);  // we will need to measure the reduction performed.

  /* we must not apply the contract_floor on the space of the contractor (this.space) 
   * but on the spcae in argument.
   * Because if the contractor has contracted significantly only entities that
   * are sybs in the space in argument, then, it will return "true" while no
   * contraction are actually performed (see sync below: sybs are not updated). 
   * This may cause, e.g., an infinite loop at the paver level. */

  this->space.sync(space);

  try {
    contract();
  } catch(EmptyBoxException e) {
    if (contract_floor>0) delete tmpbox;
    throw e;
  }

  space.sync(this->space,255); // 255 => only the domain of variables and eprs can be modified!

  if (contract_floor>0) {
    bool reduced=tmpbox->maxratiodelta(space.box) > contract_floor; 
    if (!reduced) space.box = *tmpbox;  // in case cell.space and space coincide
    delete tmpbox;
    if (!reduced) return false;
  }
  
  return true;
}


bool Contractor::contract(Space& space, const Indicators& p) {
  bool res;
  this->indic = &p;

  try {

    res=contract(space);

  } catch (EmptyBoxException e) {
    this->indic = NULL;
    throw e;
  }

  this->indic = NULL;
  return res;
}

bool Contractor::contract(Cell& cell, const Indicators& p) {
  bool res;
  this->cell = &cell;
  this->indic = &p;

  try {

    res=contract(cell.space);

  } catch(EmptyBoxException e) {
    this->cell = NULL;
    this->indic = NULL;
    throw e;
  }

  this->cell = NULL;
  this->indic = NULL;
  return res;
}

const REAL Contractor::default_contract_floor = 0.02;

}
