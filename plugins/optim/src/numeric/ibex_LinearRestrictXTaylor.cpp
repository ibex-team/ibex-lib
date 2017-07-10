//============================================================================
//                                  I B E X
// File        : ibex_LinearRestrictXTaylor.cpp
// Author      : Gilles Chabert, Ignacio Araya, Bertrand Neveu
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Last Update : Jun 22, 2017
//============================================================================

#include "ibex_LinearRestrictXTaylor.h"
#include "ibex_Random.h"

#include <stdlib.h>
#include <cassert>

using namespace std;

namespace ibex {

namespace {

class NoCornerPoint : public Exception { };

class Unsatisfiability : public Exception { };

}

LinearRestrictXTaylor::LinearRestrictXTaylor(const System& sys) : LinearRestrict(sys.nb_var), sys(sys), n(sys.nb_var), inf(new bool[n]) {

}

LinearRestrictXTaylor::~LinearRestrictXTaylor() {
	delete [] inf;
}

IntervalVector LinearRestrictXTaylor::get_corner_point(const IntervalVector& box) {
	IntervalVector corner(n);

	for (int j=0; j<n; j++) {
		if (inf[j]) {
			if (box[j].lb()>NEG_INFINITY)
				corner[j]=box[j].lb() ;
			else if (box[j].ub()<POS_INFINITY)
				corner[j]=box[j].ub() ;
			else
				throw NoCornerPoint();
		}
		else {
			if (box[j].ub()<POS_INFINITY)
				corner[j]=box[j].ub() ;
			else if (box[j].lb()>NEG_INFINITY)
				corner[j]=box[j].lb() ;
			else
				throw NoCornerPoint();
		}
	}
	return corner;
}

int LinearRestrictXTaylor::linearization(const IntervalVector& box, LinearSolver& lp_solver)  {
	int count=0; // total number of added constraint

	BitSet active=sys.active_ctrs(box);

	int ma=(int) active.size();

	if (ma==0) return 0; //The box is inner

	//cout << "[x-Taylor] box=" << box << endl;

	// random corner choice
	for (int i=0; i<n ; i++) {
		inf[i]=(RNG::rand()%2);
	}

	// the corner used -> typed IntervalVector just to have guaranteed computations
	IntervalVector x_corner = get_corner_point(box);

	Vector row(n);

	// the evaluation of the constraints in the corner x_corner
	IntervalVector g_corner(sys.f_ctrs.eval_vector(x_corner,active));

	IntervalMatrix J=sys.active_ctrs_jacobian(box);

	int c; // constraint number

	for (int i=0; i<ma; i++) {

		// TODO: replace with lp_solver.get_limit_diam_box() ?
		if (J[i].max_diam() > lp_solver.default_limit_diam_box.ub()) {
			return -1;
		}

		c=(i==0? active.min() : active.next(c));

		if (sys.ops[c]==EQ) {
			// in principle we could deal with linear constraints
			return -1;
		}

		bool leq=(sys.ops[c]==LEQ || sys.ops[c]==LT);

		//The contraints i is generated:
		// c_i:  inf([g_i]([x])) + sup(dg_i/dx_1) * xl_1 + ... + sup(dg_i/dx_n) * xl_n  <= -eps_error
		for (int j=0; j<n; j++) {
			if ((inf[j] && leq) || (!inf[j] && !leq))
				row[j]=J[i][j].ub();
			else
				row[j]=J[i][j].lb();
		}

		double rhs = (-g_corner[i] + row*x_corner).lb();

		try {
			if (leq) {
				rhs -= lp_solver.get_epsilon();
				lp_solver.add_constraint(row,LEQ,rhs);
			} else {
				rhs += lp_solver.get_epsilon();
				lp_solver.add_constraint(row,GEQ,rhs);
			}
			count++;
		} catch (LPException&) {
			return -1;
		}
	}

	assert(count==ma);

	return count;
}

} /* namespace ibex */
