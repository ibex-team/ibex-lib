//============================================================================
//                                  I B E X
// File        : ibex_LoupFinder.cpp
// Author      : Gilles Chabert, Ignacio Araya, Bertrand Neveu
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Jul 09, 2017
//============================================================================

#include "ibex_LoupFinder.h"

using namespace std;

namespace ibex {

LoupFinder::~LoupFinder() {

}

void LoupFinder::add_property(const IntervalVector& init_box, BoxProperties& prop) {

}

bool LoupFinder::check(const System& sys, const Vector& pt, double& loup, bool _is_inner) {

	// "res" will contain an upper bound of the criterion
	double res = sys.goal_ub(pt);

	// check if f(x) is below the "loup" (the current upper bound).
	//
	// The "loup" and the corresponding "loup_point" (the current minimizer)
	// will be updated if the constraints are satisfied.
	// The test of the constraints is done only when the evaluation of the criterion
	// is better than the loup (a cheaper test).

	//        cout << " res " <<  res << " loup " <<  pseudo_loup <<  " is_inner " << _is_inner << endl;
	if (res<loup) {
		if (_is_inner || sys.is_inner(pt)) {
			loup = res;
			return true;
		}
	}

	return false;
}

void LoupFinder::monotonicity_analysis(const System& sys, IntervalVector& box, bool is_inner) {

	int n=sys.nb_var;

	if (!is_inner && sys.f_ctrs.used_vars.size()==n)
		// if there is no inner box and all the variables appear
		// in the constraints, nothing can be done
		return;

	IntervalVector g(n);
	sys.goal->gradient(box,g);

	for (int j=0; j<n; j++) {
		if (is_inner || !sys.f_ctrs.used(j)) {
			if (g[j].lb()>=0 && box[j].lb()!=NEG_INFINITY) box[j]=box[j].lb();
			if (g[j].ub()<=0 && box[j].ub()!=POS_INFINITY) box[j]=box[j].ub();
		}
	}
}

} // namespace ibex
