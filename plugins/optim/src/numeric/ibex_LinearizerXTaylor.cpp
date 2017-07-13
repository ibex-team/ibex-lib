/* ============================================================================
 * I B E X - X-Taylor linear relaxation/restriction
 * ============================================================================
 * Copyright   : IMT Atlantique (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert, Ignacio Araya, Bertrand Neveu
 * Created     : July 01th, 2012
 * Updated     : July 13th, 2017
 * ---------------------------------------------------------------------------- */

#include "ibex_ExtendedSystem.h"
#include "ibex_ExprDiff.h"
#include "ibex_Random.h"
#include "ibex_Exception.h"
#include "ibex_LinearizerXTaylor.h"
#include "ibex_NormalizedSystem.h"

#include <vector>

using namespace std;

namespace ibex {

namespace {

class NoCornerPoint : public Exception { };

class Unsatisfiability : public Exception { };

}

// TODO: merge with default_diam_limit
const double LinearizerXTaylor::default_max_diam_deriv =1e6;

LinearizerXTaylor::LinearizerXTaylor(const System& _sys, approx_mode mode, corner_policy policy,
		linear_mode lmode1, double max_diam_deriv1):
			Linearizer(_sys.nb_var), sys(_sys),
			m(sys.f_ctrs.image_dim()), goal_ctr(-1 /*tmp*/),
			mode(mode), lmode(lmode1),
			inf(new bool[n]), max_diam_deriv(max_diam_deriv1) {

	if (dynamic_cast<const ExtendedSystem*>(&sys)) {
		((int&) goal_ctr)=((const ExtendedSystem&) sys).goal_ctr();
	} else if (!dynamic_cast<const NormalizedSystem*>(&sys)) {
		not_implemented("LinearizerXTyalor with non-normalized systems");
	}


	switch(policy) {
	case INF:    corners.push_back(INF_X); break;
	case SUP:    corners.push_back(SUP_X); break;
	case RANDOM: corners.push_back(RANDOM_X); break;
	case RANDOM_OPP:
		if (mode==RESTRICT) ibex_error("LinearizerXTaylor: cannot create inner restriction with multiple points");
		else {
			corners.push_back(RANDOM_X);
			corners.push_back(OPPOSITE);
		}
		break;
	}
}

LinearizerXTaylor::~LinearizerXTaylor() {
	delete[] inf;
}

void LinearizerXTaylor::get_corner(corner_id id) {

	for (int j=0; j<n; j++) {
		switch (id) {
		case INF_X:
			inf[j] = true;
			break;
		case SUP_X:
			inf[j] = false;
			break;
		case RANDOM_X:
			inf[j] = (RNG::rand() % 2 == 0);
			break;
		case OPPOSITE:
			inf[j] = !inf[j];
			break;
		default:
			assert(false);
			break;
		}
	}
}

IntervalVector LinearizerXTaylor::get_corner_point(const IntervalVector& box) {
	IntervalVector pt(n);

	for (int j=0; j<n; j++) {
		if (inf[j]) {
			if (box[j].lb()>NEG_INFINITY)
				pt[j]=box[j].lb();
			else if (box[j].ub()<POS_INFINITY) {
				pt[j]=box[j].ub();
				inf[j]=false; // force change
			} else
				throw NoCornerPoint();
		}
		else {
			if (box[j].ub()<POS_INFINITY)
				pt[j]=box[j].ub();
			else if (box[j].lb()>NEG_INFINITY) {
				inf[j]=true; // force change
				pt[j]=box[j].lb();
			} else
				throw NoCornerPoint();
		}
	}
	return pt;
}

bool LinearizerXTaylor::check_and_add_constraint(const IntervalVector& box, const Vector& a, double b, LinearSolver& lp_solver) {

	Interval ax=a*box; // for fast (in)feasibility check

	// ======= Quick (in)feasibility checks
	//                 a*[x] <= rhs ?
	if (ax.lb()>b)
		// the constraint is not satisfied
		throw Unsatisfiability();
	else if (ax.ub()<=b) {
		// the (linear) constraint is satisfied for any point in the box
		return false;
	} else {
		try {
			//cout << "add constraint " << a << "*x<=" << b << endl;
			lp_solver.add_constraint(a, LEQ, b);
			return true;
		} catch (LPException&) {
			return false;
		}
	}
}

int LinearizerXTaylor::linearize(const IntervalVector& box, LinearSolver& lp_solver)  {
	if (mode==RELAX)
		return linear_relax(box,lp_solver);
	else
		return linear_restrict(box,lp_solver);
}

int LinearizerXTaylor::linear_relax(const IntervalVector& box, LinearSolver& lp_solver)  {

	int count=0; // total number of added constraint

	// ========= get active constraints ===========

	// ------------------------------------------------
	// TODO: requires sys.active_ctrs_hansen_matrix !!!
	//	BitSet b=sys.active_ctrs(box);

	BitSet active=BitSet::all(m); // tmp.................
	// ------------------------------------------------

	int ma=active.size();

	IntervalMatrix Df(ma,n); // derivatives over the box
	Matrix coeffs(ma,n);     // coefficients for the LP

	if (lmode == TAYLOR) // compute derivatives once for all
		// ------------------------------------------------
		// TODO: requires sys.active_ctrs_hansen_matrix !!!
		//Df=sys.active_ctrs_jacobian(box);

		Df=sys.f_ctrs.jacobian(box); // tmp................

	try {
		for(unsigned int k=0; k<corners.size(); k++) {

			// TODO: only one corner for a linear constraint!

			// ============ get the corner point =================
			get_corner(corners[k]);

			IntervalVector corner=get_corner_point(box);

			//cout << "========== corner=" << corner << "=========" << endl;
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
			IntervalVector rhs=-sys.f_ctrs.eval_vector(corner,active) + coeffs*corner;

			// ================== add constraints ==================
			int c; // constraint number

			for (int i=0; i<ma; i++) {

				c=(i==0? active.min() : active.next(c));

				//cout << " add ctr n°" << c << endl;
				if (k>0 && sys.f_ctrs.deriv_calculator().is_linear[c]) {
					//cout << "ctr " << c << " is linear!\n";
					continue;
				}

				// only one corner for a linear constraint
				if (Df[i].max_diam() > max_diam_deriv) {
					continue; // To avoid problems with LP solvers (like SoPleX)
				}


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

	return count;
}

int LinearizerXTaylor::linear_restrict(const IntervalVector& box, LinearSolver& lp_solver)  {
	int count=0; // total number of added constraint

	BitSet active=sys.active_ctrs(box);

	int ma=(int) active.size();

	if (ma==0) return 0; //The box is inner

	//cout << "[x-Taylor] box=" << box << endl;

	get_corner(corners.back());

	// the corner used -> typed IntervalVector just to have guaranteed computations
	IntervalVector corner = get_corner_point(box);

	Vector row(n);

	// the evaluation of the constraints in the corner x_corner
	IntervalVector g_corner(sys.f_ctrs.eval_vector(corner,active));

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

		double rhs = (-g_corner[i] + row*corner).lb();

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

} // end namespace
