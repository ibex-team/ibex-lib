/* ============================================================================
 * I B E X - X-Taylor linear relaxation/restriction
 * ============================================================================
 * Copyright   : IMT Atlantique (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert, Ignacio Araya, Bertrand Neveu
 * Created     : July 01th, 2012
 * Updated     : July 18th, 2017
 * ---------------------------------------------------------------------------- */

#include "ibex_LinearizerXTaylor.h"
#include "ibex_ExtendedSystem.h"
#include "ibex_Random.h"
#include "ibex_Exception.h"
#include "ibex_NormalizedSystem.h"
#include "ibex_BxpSystemCache.h"

#include <vector>

using namespace std;

namespace ibex {

namespace {

class NoCornerPoint : public Exception { };

class Unsatisfiability : public Exception { };

}

LinearizerXTaylor::LinearizerXTaylor(const System& _sys, approx_mode _mode, corner_policy policy,
		slope_formula _slope):
			Linearizer(_sys.nb_var), sys(_sys),
			m(sys.f_ctrs.image_dim()), goal_ctr(-1 /*tmp*/),
			mode(_mode), slope(_slope),
			inf(new bool[n]), lp_solver(NULL), cache(NULL) {

	if (dynamic_cast<const ExtendedSystem*>(&sys)) {
		((int&) goal_ctr)=((const ExtendedSystem&) sys).goal_ctr();
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

void LinearizerXTaylor::add_property(const IntervalVector& init_box, BoxProperties& prop) {
	//--------------------------------------------------------------------------
	/* Using system cache seems not interesting. */
	//	if (/*mode==RELAX && slope==TAYLOR && */!prop[BxpSystemCache::get_id(sys)]) {
	//		prop.add(new BxpSystemCache(sys,BxpSystemCache::default_update_ratio));
	//	}
	//--------------------------------------------------------------------------
}

int LinearizerXTaylor::linearize(const IntervalVector& box, LPSolver& _lp_solver)  {
	BoxProperties prop(box);
	return linearize(box, _lp_solver, prop);
}

int LinearizerXTaylor::linearize(const IntervalVector& box, LPSolver& _lp_solver, BoxProperties& prop) {
	lp_solver = &_lp_solver;

	// ========= get active constraints ===========
	BitSet* active;

	//--------------------------------------------------------------------------
	/* Using system cache seems not interesting. */
	//cache=(BxpSystemCache*) prop[BxpSystemCache::get_id(sys)];
	cache=NULL;
	//--------------------------------------------------------------------------

	if (cache!=NULL) {
		active = &cache->active_ctrs();
//			if (active->size()<box.size()) {
//				cout << "[xtaylor] inactive constraint!\n";
//			}
	} else {
		active = new BitSet(sys.active_ctrs(box));
	}
	// ============================================

	int n = mode==RELAX ? linear_relax(box,*active) : linear_restrict(box,*active);

	if (cache==NULL) delete active;
	return n;
}

int LinearizerXTaylor::linear_relax(const IntervalVector& box, const BitSet& active)  {

	int count=0; // total number of added constraint

	if (active.empty()) return 0;

	int ma=active.size();

	IntervalMatrix Df(ma,n); // derivatives over the box

	if (slope == TAYLOR) { // compute derivatives once for all
		Df=cache? cache->active_ctrs_jacobian() : sys.f_ctrs.jacobian(box,active);

		if (Df.is_empty()) return -1;
	}

	for(unsigned int k=0; k<corners.size(); k++) {

		// ============ get the corner point =================
		get_corner(corners[k]);

		try {
			IntervalVector corner=get_corner_point(box);

			// the evaluation of the constraints in the corner x_corner
			IntervalVector g_corner(sys.f_ctrs.eval_vector(corner,active));

			if (g_corner.is_empty()) continue; // skip this corner

			//cout << "========== corner=" << corner << "=========" << endl;

			// ========= update derivatives (Hansen mode) ========
			if (slope == HANSEN) {
				sys.f_ctrs.hansen_matrix(box,corner,Df,active);
				if (Df.is_empty()) continue; // skip this corner
			}

			int c; // constraint number

			for (int i=0; i<active.size(); i++) {
				c=(i==0? active.min() : active.next(c));

				//cout << " add ctr n°" << c << endl;

				// only one corner for a linear constraint
				if (k>0 && sys.f_ctrs.deriv_calculator().is_linear[c]) {
					//cout << "ctr " << c << " is linear!\n";
					continue;
				}

				try {
					if (sys.ops[c]==LEQ || sys.ops[c]==LT || sys.ops[c]==EQ)
						count += linearize_leq_corner(box,corner,Df[i],g_corner[i]);

					// note: in case of equality g(x)=0, we also add a linear relaxation for
					// g(x)>=0, except if this is the "goal constraint" y=f(x).
					if (sys.ops[c]==GEQ || sys.ops[c]==GT || sys.ops[c]==EQ) // && c!=goal_ctr))
						count += linearize_leq_corner(box,corner,-Df[i],-g_corner[i]);

				} catch (LPException&) {
					continue;  // just skip this constraint
				} catch (Unsatisfiability&) {
					return -1;
				}
			}
		} catch(NoCornerPoint&) {
			continue; // skip this corner
		}
	}

	return count;
}

int LinearizerXTaylor::linear_restrict(const IntervalVector& box, const BitSet& active) {

	if (active.empty()) return 0;

	get_corner(corners.back());

	try {
		// the corner used -> typed IntervalVector just to have guaranteed computations
		IntervalVector corner = get_corner_point(box);
		//IntervalMatrix J=cache? cache->active_ctrs_jacobian() : sys.f_ctrs.jacobian(box,active);
		//IntervalMatrix J=sys.f_ctrs.jacobian(box,active);
		IntervalMatrix J(active.size(),n); // derivatives over the box
		sys.f_ctrs.hansen_matrix(box,corner,J,active);

		// note: no way to inform that the box is actually infeasible
		// (we are in "restrict" mode)
		if (J.is_empty()) return -1;

		// the evaluation of the constraints in the corner x_corner
		IntervalVector g_corner(sys.f_ctrs.eval_vector(corner,active));
		if (g_corner.is_empty()) return -1;

		// total number of added constraint
		// may be less than active.size() if
		// a constraint was not detected as inactive
		int count=0;
		int c; // constraint number

		for (int i=0; i<active.size(); i++) {
			c=(i==0? active.min() : active.next(c));

			try {
				if (sys.ops[c]==EQ && c!=goal_ctr)
					// in principle we could deal with linear constraints
					return -1;
				else if (c==goal_ctr || sys.ops[c]==LEQ || sys.ops[c]==LT)
					count += linearize_leq_corner(box,corner,J[i],g_corner[i]);
				else
					count += linearize_leq_corner(box,corner,-J[i],-g_corner[i]);
			} catch (LPException&) {
				return -1;
			} catch (Unsatisfiability&) {
				return -1;
			}
		}
		return count;
	} catch(NoCornerPoint&) {
		return -1;
	}

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
				pt[j]=box[j].lb();
				inf[j]=true; // force change
			} else
				throw NoCornerPoint();
		}
	}
	return pt;
}

int LinearizerXTaylor::linearize_leq_corner(const IntervalVector& box, IntervalVector& corner, const IntervalVector& dg_box, const Interval& g_corner) {
	Vector a(n); // vector of coefficients

	if (dg_box.max_diam() > LPSolver::max_box_diam) {
		// we also also avoid this way to deal with infinite bounds (see below)
		throw LPException();
	}

	// ========= compute matrix of coefficients ===========
	// Fix each coefficient to the lower/upper bound of the
	// constraint gradient, depending on the position of the
	// corresponding component of the corner and the
	// linearization mode.
	for (int j=0; j<n; j++) {
		if ((mode==RELAX && !inf[j]) || (mode==RESTRICT && inf[j]))
			a[j]=dg_box[j].ub();
		else
			a[j]=dg_box[j].lb();
	}
	// =====================================================

	Interval rhs = -g_corner + a*corner;

	double b = mode==RESTRICT? rhs.lb() - lp_solver->get_epsilon() : rhs.ub();

	// may throw Unsatisfiability and LPException
	return check_and_add_constraint(box,a,b);
}

int LinearizerXTaylor::check_and_add_constraint(const IntervalVector& box, const Vector& a, double b) {

	Interval ax=a*box; // for fast (in)feasibility check

	// ======= Quick (in)feasibility checks
	//                 a*[x] <= rhs ?
	if (ax.lb()>b)
		// the constraint is not satisfied
		throw Unsatisfiability();
	else if (ax.ub()<=b) {
		// the (linear) constraint is satisfied for any point in the box
		return 0;
	} else {
		//cout << "add constraint " << a << "*x<=" << b << endl;
		lp_solver->add_constraint(a, LEQ, b); // note: may throw LPException
		return 1;
	}
}

} // end namespace
