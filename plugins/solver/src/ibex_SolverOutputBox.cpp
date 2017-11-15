//============================================================================
//                                  I B E X
// File        : ibex_Solution.cpp
// Author      : Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Last update : Sep 19, 2017
//============================================================================

#include "ibex_SolverOutputBox.h"

#include <stdlib.h>

using namespace std;

namespace ibex {

ostream& operator<<(ostream& os, const SolverOutputBox& sol) {
	os.precision(12);
	os << sol.existence() << " ";
	switch(sol.status) {

	case SolverOutputBox::INNER:
		os << "[inner";
		break;
	case SolverOutputBox::BOUNDARY:
		os << "[boundary";
		break;
	case SolverOutputBox::UNKNOWN :
		os << "[unknown]";
		break;
	case SolverOutputBox::PENDING:
		os << "[pending]";
		break;
	}

	if (sol.varset!=NULL) {
		os << " ";
		for (int i=0; i<sol.varset->nb_param; i++) {
			if (i>0) os << ',';
			os << sol.varset->param(i) +1;
		}
		os << "->";
		for (int i=0; i<sol.varset->nb_var; i++) {
			if (i>0) os << ',';
			os << sol.varset->var(i) +1;
		}
	}
	os << "]";

	return os;
}

} /* namespace ibex */
