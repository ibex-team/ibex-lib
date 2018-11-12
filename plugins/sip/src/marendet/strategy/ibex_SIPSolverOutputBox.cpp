/* ============================================================================
 * I B E X - ibex_SIPSolverOutputBox.cpp
 * ============================================================================
 * Copyright   : IMT Atlantique (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Antoine Marendet, Gilles Chabert
 * Created     : May 4, 2018
 * ---------------------------------------------------------------------------- */
 
#include "ibex_SIPSolverOutputBox.h"

using namespace std;

namespace ibex {

ostream& operator<<(ostream& os, const SIPSolverOutputBox& sol) {
	os.precision(12);
	os << sol.existence() << " ";
	switch(sol.status) {

	case SIPSolverOutputBox::INNER:
		os << "[inner";
		break;
	case SIPSolverOutputBox::BOUNDARY:
		os << "[boundary";
		break;
	case SIPSolverOutputBox::UNKNOWN :
		os << "[unknown]";
		break;
	case SIPSolverOutputBox::PENDING:
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
} // end namespace ibex
