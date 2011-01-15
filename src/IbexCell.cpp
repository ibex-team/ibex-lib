/*---------------------------------------------------------------------------------
 * 
 * Cells - Representation of the search space.
 * -------------------------------------------
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

#include "IbexCell.h"

namespace ibex {

Cell::Cell(Space& space, const BacktrackableMap& m) : space(space), last_split_var(ROOT_CELL), last_contractor(-1), box(space.box), epx(space.epx) { 
  m.create(*this);
}

Cell::Cell(Bisection& bis, bool side, Cell& c, const BacktrackableMap& m) :  space(c.space), last_split_var(bis.var), last_contractor(c.last_contractor),
									     box(side?bis.box2:bis.box1), epx(c.epx) { 
  m.create_child(*this, c, side);
}
    
const int Cell::ROOT_CELL = -1;

std::ostream& operator<<(std::ostream& os, const BacktrackableMap& m) {
  for (BacktrackableMap::const_iterator it=m.begin(); it!=m.end(); it++) {
    os << it->first << " ";
  }
  return os;
}

} // end namespace
