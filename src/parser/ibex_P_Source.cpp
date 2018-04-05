//============================================================================
//                                  I B E X                                   
// File        : ibex_ParserSource.cpp
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Jun 12, 2012
// Last Update : Jun 12, 2012
//============================================================================

#include "ibex_Expr.h"
#include "ibex_P_Source.h"

using namespace std;

namespace ibex {
namespace parser {

P_Source::P_Source() : goal(NULL), ctrs(NULL) {

}

void P_Source::cleanup() {

	if (goal) delete goal;

	if (ctrs) // may be NULL if only a single function is loaded
		      // or if the system is unconstrained.
		delete ctrs; // will recursively delete all the constraints

	// important! re-init to NULL for the next file to be parsed:

	// Note: Now that there is no more application nodes (ExprApply),
	// we could copy auxiliary functions in MainGenerator
	// and safely destroy them here.
	func.clear();

	goal=NULL;
	ctrs=NULL;
}

} // end namespace parser
} // end namespace ibex
