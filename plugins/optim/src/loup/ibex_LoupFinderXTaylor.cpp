//============================================================================
//                                  I B E X
// File        : ibex_LoupFinderXTaylor.cpp
// Author      : Gilles Chabert, Ignacio Araya, Bertrand Neveu
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Jul 12, 2012
// Last Update : Jul 09, 2017
//============================================================================

#include "ibex_LoupFinderXTaylor.h"

namespace ibex {

LoupFinderXTaylor::LoupFinderXTaylor(const /*Normalized*/System& sys) : sys(sys), lr(sys), lp_solver(sys.nb_var+1, std::max(sys.nb_var*3,LinearSolver::default_max_iter)) {
//	nb_simplex=0;
//	diam_simplex=0;

	// TODO: warning, INXTaylor actually works on the extended system!!
}

std::pair<Vector, double> LoupFinderXTaylor::find(const IntervalVector& box, const Vector&, double current_loup) {

	int n=sys.nb_var;

	lr.linearization(box,lp_solver);

	LinearSolver::Status_Sol stat = lp_solver.solve();

	//std::cout << " stat " << stat << std::endl;
	if (stat == LinearSolver::OPTIMAL) {
		//the linear solution is mapped to intervals and evaluated
		Vector prim(n+1);
		lp_solver.get_primal_sol(prim);

		Vector loup_point = prim.subvector(1,n);

		//std::cout << " simplex result " << prim[0] << " " << loup_point << std::endl;

		if (!box.contains(loup_point)) throw NotFound();

		double new_loup=current_loup;
		if (check(sys,loup_point,new_loup,false)) {
			return std::make_pair(loup_point,new_loup);
		}
	}

	throw NotFound();
}

} /* namespace ibex */
