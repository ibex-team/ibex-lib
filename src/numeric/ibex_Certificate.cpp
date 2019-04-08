//============================================================================
//                                  I B E X
// File        : ibex_Certificate.cpp
// Author      : Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Sep 21, 2017
// Last Update : Nov 13, 2018
//============================================================================

#include "ibex_Certificate.h"

using namespace std;

namespace ibex {

Certificate::Certificate(const IntervalVector& box, const IntervalVector* unicity, const VarSet* varset) :
		varset(varset? new VarSet(*varset) : NULL),
		_existence(box),
		_unicity(unicity? new IntervalVector(*unicity) : NULL) {

}

ostream& operator<<(ostream& os, const Certificate& c) {
	os.precision(12);
	os << c.existence() << " ";

	if (c.varset!=NULL) {
		os << " ";
		for (int i=0; i<c.varset->nb_param; i++) {
			if (i>0) os << ',';
			os << c.varset->param(i) +1;
		}
		os << "->";
		for (int i=0; i<c.varset->nb_var; i++) {
			if (i>0) os << ',';
			os << c.varset->var(i) +1;
		}
	}
	os << "]";

	return os;
}

} /* namespace ibex */
