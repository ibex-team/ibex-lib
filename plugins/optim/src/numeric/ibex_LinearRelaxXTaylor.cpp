/* ============================================================================
 * I B E X - X-Taylor linear relaxation
 * ============================================================================
 * Copyright   : IMT Atlantique (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert, Ignacio Araya, Bertrand Neveu
 * Created     : July 01th, 2012
 * Updated     : July 07th, 2017
 * ---------------------------------------------------------------------------- */

#include "ibex_LinearRelaxXTaylor.h"
#include "ibex_ExtendedSystem.h"
#include "ibex_ExprDiff.h"
#include "ibex_Random.h"
#include "ibex_Exception.h"

using namespace std;

namespace ibex {

namespace {

class NoCornerPoint : public Exception { };

class Unsatisfiability : public Exception { };

}

// TODO: merge with default_diam_limit
const double LinearRelaxXTaylor::default_max_diam_deriv =1e6;

LinearRelaxXTaylor::LinearRelaxXTaylor(const System& _sys, std::vector<corner_point>& cpoints1,
		linear_mode lmode1, double max_diam_deriv1):
			LinearRelax(_sys.nb_var), sys(_sys), n(_sys.nb_var),
			m(sys.f_ctrs.image_dim()), goal_ctr(-1 /*tmp*/), cpoints(cpoints1),
			max_diam_deriv(max_diam_deriv1),
			lmode(lmode1) {

	if (dynamic_cast<const ExtendedSystem*>(&sys)) {
		((int&) goal_ctr)=((const ExtendedSystem&) sys).goal_ctr();
	} else if (!dynamic_cast<const NormalizedSystem*>(&sys)) {
		not_implemented("LinearRelaxXTyalor with non-normalized systems");
	}
}

LinearRelaxXTaylor::~LinearRelaxXTaylor() {

}

void LinearRelaxXTaylor::get_corner(corner_point cpoint, bool *inf) {

	for (int j=0; j<n; j++) {
		switch (cpoint) {
		case INF_X:
			inf[j] = true;
			break;
		case SUP_X:
			inf[j] = false;
			break;
		case RANDOM:
			inf[j] = (RNG::rand() % 2 == 0);
			break;
		case RANDOM_INV:
			inf[j] = !inf[j];
			break;
		default:
			assert(false);
			break;
		}
	}
}

IntervalVector LinearRelaxXTaylor::get_corner_point(const IntervalVector& box, bool* inf) {
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

bool LinearRelaxXTaylor::check_and_add_constraint(const IntervalVector& box, const Vector& a, double b, LinearSolver& lp_solver) {

	Interval check=a*box; // for fast (in)feasibility check

	// ======= Quick (in)feasibility checks
	//                 a*[x] <= rhs ?
	if (check.lb()>b)
		// the constraint is not satisfied
		throw Unsatisfiability();
	else if (check.ub()<=b)
		// the (linear) constraint is satisfied for any point in the box
		return false;
	else {
		try {
			lp_solver.add_constraint(a, LEQ, b);
			return true;
		} catch (LPException&) {
			return false;
		}
	}
}

int LinearRelaxXTaylor::linearization(const IntervalVector& box, LinearSolver& lp_solver)  {

	int count=0; // total number of added constraint

	// ========= get active constraints ===========

	// ------------------------------------------------
	// TODO: requires sys.active_ctrs_hansen_matrix !!!
	//	BitSet b=sys.active_ctrs(box);

	BitSet b=BitSet::all(m); // tmp.................
	// ------------------------------------------------

	int ma=b.size();

	IntervalMatrix Df(ma,n); // derivatives over the box
	Matrix coeffs(ma,n);     // coefficients for the LP
	bool *inf = new bool[n]; // information about which corner it is

	if (lmode == TAYLOR) // compute derivatives once for all
		// ------------------------------------------------
		// TODO: requires sys.active_ctrs_hansen_matrix !!!
		//Df=sys.active_ctrs_jacobian(box);

		Df=sys.f_ctrs.jacobian(box); // tmp................

	try {
		for(unsigned int k=0; k<cpoints.size(); k++) {

			// TODO: only one corner for a linear constraint!

			// ============ get the corner point =================
			get_corner(cpoints[k],inf);
			IntervalVector corner=get_corner_point(box,inf);

			// ========= update derivatives (Hansen mode) ========
			if (lmode == HANSEN) {
				sys.f_ctrs.hansen_matrix(box,corner,Df);
			}
			// ========= compute matrix of coefficients ===========
			// Fix each column of the matrix to its lower/upper
			// bound depending on the position of the corresponding
			// component of the corner
			for (int j=0; j<n; j++) {
				coeffs.set_col(j, inf[j] ? Df.col(j).lb() : Df.col(j).ub());
			}

			// ========= compute right-hand side vector ===========
			IntervalVector rhs=-sys.f_ctrs.eval_vector(corner,b) + coeffs*corner;

			// ================== add constraints ==================
			int c; // constraint number

			for (int i=0; i<ma; i++) {

				if (Df[i].max_diam() > max_diam_deriv) {
					continue; // To avoid problems with LP solvers (like SoPleX)
				}

				c=(i==0? b.min() : b.next(c));

				count += check_and_add_constraint(box,coeffs[i],rhs[i].ub(),lp_solver);

				// in case of equality g(x)=0, also add a linear relaxation for g(x)>=0
				if (sys.ops[c]==EQ && c!=goal_ctr) {
					count += check_and_add_constraint(box,-coeffs[i],-rhs[i].lb(),lp_solver);
				}
			}
		}
	} catch(Unsatisfiability&) {
		return -1;
	} catch(NoCornerPoint&) {
		// impossible to linearize
	}

	delete[] inf;

	return count;
}

} // end namespace
