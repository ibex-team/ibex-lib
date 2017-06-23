/*
 * ibex_InXTaylor.cpp
 *
 *  Created on: Jun 22, 2017
 *      Author: gilles
 */

#include "ibex_InXTaylor.h"

namespace ibex {

InXTaylor::InXTaylor(const NormalizedSystem& sys) : sys(sys), lr(sys), lp_solver(sys.nb_var+1, std::max(sys.nb_var*3,LinearSolver::default_max_iter)) {

	// TODO: warning, INXTaylor actually works on the extended system!!
}

inline double InXTaylor::goal(const Vector& x) const {
	Interval fx=sys.goal->eval(x);
	if (fx.is_empty())  // means: outside of the definition domain of the function
		return POS_INFINITY;
	else
		return fx.ub();

}

bool InXTaylor::is_inner(const IntervalVector& box) {
	//	cout << " box " << box << endl;
	for (int j=0; j<sys.nb_ctr; j++) {

		//TODO
		//if (entailed->normalized(j)) continue;
		//

		Interval ev=sys.ctrs[j].f.eval(box);
		if (ev.is_empty()) return false;
		if (ev.ub()>0) return false;

	}
	return true;
}

std::pair<Vector, double> InXTaylor::find(const IntervalVector& box, const Vector& x0, double current_loup) {

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

		// "res" will contain an upper bound of the criterion
		double res = goal(loup_point);

		// check if f(x) is below the "loup" (the current upper bound).
		//
		// The "loup" and the corresponding "loup_point" (the current minimizer)
		// will be updated if the constraints are satisfied.
		// The test of the constraints is done only when the evaluation of the criterion
		// is better than the loup (a cheaper test).

		//        cout << " res " <<  res << " loup " <<  pseudo_loup <<  " is_inner " << _is_inner << endl;
		if (res<current_loup) {
			if (is_inner(loup_point)) {
				return std::make_pair(loup_point,res);
			}
		}
	}

	throw NotFound();
}

InXTaylor::~InXTaylor() {
	// TODO Auto-generated destructor stub
}

} /* namespace ibex */
