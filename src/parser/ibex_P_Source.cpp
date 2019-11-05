//============================================================================
//                                  I B E X                                   
// File        : ibex_P_Source.cpp
// Author      : Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : May 22, 2019
// Last Update : Jun 12, 2012
//============================================================================

#include "ibex_Expr.h"
#include "ibex_P_Source.h"

using namespace std;

namespace ibex {
namespace parser {

P_Source::P_Source() : goal(NULL), ctrs(NULL) { //, cleanup_func(true) {

}

P_Source::~P_Source() {

	if (goal) delete goal;

	if (ctrs) // may be NULL if only a single function is loaded
		      // or if the system is unconstrained.
		delete ctrs; // will recursively delete all the constraints

	// Note: Now that there is no more application nodes (ExprApply),
	// we can safely destroy auxiliary functions.
	//if (cleanup_func)
	for (vector<Function*>::iterator it=func.begin(); it!=func.end(); ++it)
		delete *it;
}

ostream& operator<<(ostream& os, const P_Source& source) {
	if (source.goal)
		os << "minimize " << *source.goal << endl;
	if (source.ctrs) {
		os << "constraints" << endl;
		os << *source.ctrs;
	}
	return os;
}

} // end namespace parser

} // end namespace ibex
