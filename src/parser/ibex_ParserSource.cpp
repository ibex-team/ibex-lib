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
#include "ibex_Expr.h"

using namespace std;

namespace ibex {
namespace parser {

P_Source::P_Source() : goal(NULL), ctrs(NULL) {

}

void P_Source::cleanup() {
	func.clear();

	if (goal) ibex::cleanup(*goal,false);

	if (ctrs) // may be NULL if only a single function is loaded
		      // or if the system is unconstrained.
		delete ctrs; // will recursively delete all the constraints
	                 // but not the symbols

	for (vector<Entity*>::iterator it=vars.begin(); it!=vars.end(); it++) {
		delete (ExprSymbol*) &((*it)->symbol);
		delete *it;
	}
	vars.clear();

	// important! re-init to NULL for the next file to be parsed:

	goal=NULL;
	ctrs=NULL;
}

} // end namespace parser
} // end namespace ibex
