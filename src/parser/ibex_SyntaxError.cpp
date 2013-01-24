//============================================================================
//                                  I B E X                                   
// File        : ibex_SyntaxError.cpp
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Jun 12, 2012
// Last Update : Jun 12, 2012
//============================================================================

#include "ibex_SyntaxError.h"

namespace ibex {

std::ostream& operator<< (std::ostream& os, const SyntaxError& e) {
  os << "Syntax error";
  if (e.token) os << " near \"" << e.token << "\"";
  if (e.line!=-1) os << " line " << e.line;
  os << ": " << e.msg;
  return os;
}

} // end namespace ibex
