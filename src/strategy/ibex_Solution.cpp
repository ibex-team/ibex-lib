//============================================================================
//                                  I B E X
// File        : ibex_Solution.cpp
// Author      : Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Last update : Sep 19, 2017
//============================================================================

#include "ibex_Solution.h"

#include <stdlib.h>

using namespace std;

namespace ibex {

ostream& operator<<(ostream& os, const Solution& sol) {
	os.precision(12);
	os << sol.existence() << " ";
	switch(sol.status) {

	case Solution::SOLUTION :
		cout << "[certified";
	if (sol.varset!=NULL) {
		cout << " ";
		for (int i=0; i<sol.varset->nb_param; i++) {
			if (i>0) cout << ',';
			cout << sol.varset->param(i);
		}
		cout << "->";
		for (int i=0; i<sol.varset->nb_var; i++) {
			if (i>0) cout << ',';
			cout << sol.varset->var(i);
		}
	}
	cout << ']';
	break;

	case Solution::UNKNOWN :
		cout << "[unknown]"; break;
	}
	return os;
}

} /* namespace ibex */
