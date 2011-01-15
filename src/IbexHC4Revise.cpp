/*---------------------------------------------------------------------------------
 * 
 * HC4Revise
 * --------------------------------------
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

#include "IbexHC4Revise.h"

namespace ibex {

HC4Revise::HC4Revise(const Constraint& ctr, bool only_multi_occ) : 
  Operator(ConstraintSpaceFactory(ctr, only_multi_occ)), ctr(ctr) { }

HC4Revise::HC4Revise(const Constraint& ctr, Space& space) : 
  Operator(space), ctr(ctr) { }

HC4Revise::HC4Revise(const HC4Revise& hc4) : Operator(hc4.space), Contractor(hc4), ctr(hc4.ctr) { }

void HC4Revise::contract() {        
  const Indicators* indic=current_indic();
  if (indic!=NULL && indic->impact==NO_VAR) return;

  ctr.forward(space);
  ctr.backward(space);
}


} // end namespace
