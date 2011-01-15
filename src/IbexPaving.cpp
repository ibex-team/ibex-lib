/*---------------------------------------------------------------------------------
 * 
 * Paver Paving
 * -------------
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

#include "IbexPaving.h"
#include "IbexIO.h"

namespace ibex {

void PavingPrinter::visit(const PavingNode& n) { 
  if (ctc==-1) {
    if (x) {
      space->box = n.box;
      print_var(*space, *x);
    }		
    else
      os << n.box << " ";
  }
  n.acceptVisitor(*this);
}

void PavingPrinter::visit(const ContractorNode& c) {
  if (ctc==-1) os << "ctc " << c.ctc_number() << endl;
  else if (c.ctc_number()==ctc) {
    if (x) {
      space->box = c.box;
      print_var(*space, *x);
    }		
    else
      os << c.box << " ";
  }
  if (!c.leaf()) visit(c.rejected());
}

void PavingPrinter::visit(const BisectorNode& b) {
  if (ctc==-1) os << "bis " << endl;
  visit(b.left());
  visit(b.right());
}

ostream& operator<<(ostream& os, const PavingNode& n) {
  PavingPrinter(os,n);
  return os;
}

}
