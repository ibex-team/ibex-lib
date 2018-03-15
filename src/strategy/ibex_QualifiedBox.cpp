//============================================================================
//                                  I B E X
// File        : ibex_QualifiedBox.cpp
// Author      : Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Sep 19, 2017
// Last update : Mar 15, 2018
//============================================================================

#include <stdlib.h>
#include "ibex_QualifiedBox.h"

using namespace std;

namespace ibex {

QualifiedBox::QualifiedBox(const IntervalVector& box, QualifiedBox::sol_status status, const IntervalVector* unicity, const VarSet* varset) :
		status(status),
		varset(varset? new VarSet(*varset) : NULL),
		_existence(box),
		_unicity(unicity? new IntervalVector(*unicity) : NULL) {

}

ostream& operator<<(ostream& os, const QualifiedBox& sol) {
	os.precision(12);
	os << sol.existence() << " ";
	switch(sol.status) {

	case QualifiedBox::INNER:
		os << "[inner";
		break;
	case QualifiedBox::BOUNDARY:
		os << "[boundary";
		break;
	case QualifiedBox::UNKNOWN :
		os << "[unknown]";
		break;
	case QualifiedBox::PENDING:
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
