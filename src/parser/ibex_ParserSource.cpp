//============================================================================
//                                  I B E X                                   
// File        : ibex_ParserSource.cpp
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Jun 12, 2012
// Last Update : Jun 12, 2012
//============================================================================

#include "ibex_ParserSource.h"

namespace ibex {
namespace parser {

void P_Source::cleanup() {
	func.clear();

	for (vector<Entity*>::iterator it=vars.begin(); it!=vars.end(); it++) {
		delete (ExprSymbol*) &((*it)->symbol);
		delete *it;
	}
	vars.clear();

	//delete ctrs; // will recursively delete all the constraints
}

} // end namespace parser
} // end namespace ibex
