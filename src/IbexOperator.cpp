/*---------------------------------------------------------------------------------
 * 
 * Generic Operator for system solving.
 * ------------------------------------
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

#include "IbexOperator.h"

namespace ibex {

Operator::Operator(Space& space) : space(space) {  
  SharedObject::incr_ref(space.id);
  SharedObject::incr_ref(space.env.id);
} 

Operator::Operator(const SpaceFactory& fac) : space(*new Space(fac)) { 
  SharedObject::incr_ref(fac.env.id);
} 

Operator::~Operator() {
  const Env& env=space.env;
  SharedObject::decr_ref(space.id); // may delete space  
  SharedObject::decr_ref(env.id); // may delete env
}


void Operator::accept_printer(const OperatorPrinter& p) const {
  p.os << "operator (no name)";
}

std::ostream& operator<<(std::ostream& os, const Operator& op) {
  op.accept_printer(OperatorPrinter(os));
  return os;
}

} // end namespace

