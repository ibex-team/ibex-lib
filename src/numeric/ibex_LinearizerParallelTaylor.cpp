/*
 * ibex_LinearizerParallelTaylor.cpp
 *
 *  Created on: Aug 7, 2019
 *      Author: iaraya
 */

#include "ibex_LinearizerParallelTaylor.h"
#include "ibex_ExtendedSystem.h"
#include "ibex_Random.h"
#include "ibex_Exception.h"
#include "ibex_NormalizedSystem.h"
#include "ibex_BxpSystemCache.h"

#include <vector>

using namespace std;

namespace ibex {

namespace{

class Unsatisfiability : public Exception { };

}

LinearizerParallelTaylor::LinearizerParallelTaylor(const System& _sys, approx_mode _mode, slope_formula _slope):
			Linearizer(_sys.nb_var), sys(_sys),
			m(sys.f_ctrs.image_dim()), goal_ctr(-1 /*tmp*/),
			slope(_slope), lp_solver(NULL),mode(_mode) {

	if (dynamic_cast<const ExtendedSystem*>(&sys)) {
		((int&) goal_ctr)=((const ExtendedSystem&) sys).goal_ctr();
	}

}

void LinearizerParallelTaylor::add_property(const IntervalVector& init_box, BoxProperties& prop) {
	//--------------------------------------------------------------------------
	/* Using system cache seems not interesting. */
	//	if (/*mode==RELAX && slope==TAYLOR && */!prop[BxpSystemCache::get_id(sys)]) {
	//		prop.add(new BxpSystemCache(sys,BxpSystemCache::default_update_ratio));
	//	}
	//--------------------------------------------------------------------------
}

int LinearizerParallelTaylor::linearize(const IntervalVector& box, LPSolver& _lp_solver)  {
	BoxProperties prop(box);
	return linearize(box, _lp_solver, prop);
}

int LinearizerParallelTaylor::linearize(const IntervalVector& box, LPSolver& _lp_solver, BoxProperties& prop) {
	lp_solver = &_lp_solver;

	// ========= get active constraints ===========
	BitSet* active;

	active = new BitSet(sys.active_ctrs(box));
	// ============================================


	if (mode==RELAX){
		bool r=linear_relax(box,*active);
		return r;
	}

	else ibex_error("LinearizerParallelTaylor: inner extraction is not implemented");

	return 0;
}

int LinearizerParallelTaylor::linear_relax(const IntervalVector& box, const BitSet& active)  {
	int count=0; // total number of added constraint

	if (active.empty()) return 0;
	int ma=active.size();

	IntervalMatrix Df(ma,n); // derivatives over the box

	IntervalMatrix Df_mid(ma,n); // derivatives over the midpoint

	Df_mid = sys.f_ctrs.jacobian(box.mid(),active);
	if (Df_mid.is_empty()) return 0;
	if (slope == TAYLOR)  // compute derivatives once for all
		Df=sys.f_ctrs.jacobian(box,active);

	else if (slope == HANSEN)
		sys.f_ctrs.hansen_matrix(box,box.mid(),Df,active);

	if (Df.is_empty()) return 0;

	IntervalVector f(sys.f_ctrs.eval_vector(box.mid(),active));
	if (f.is_empty()) return 0;

	int c;
	for (int i=0; i<active.size(); i++) {
		c=(i==0? active.min() : active.next(c));

		Vector a(n); // vector of coefficients
		if (Df_mid[i].max_diam() > LPSolver::max_box_diam) {
			// we also also avoid this way to deal with infinite bounds (see below)
			throw LPException();
		}
		for (int j=0; j<n; j++)
			a[j] = Df_mid[i][j].mid();


		Interval rhs = - (f[i] - a*box.mid() );

		rhs -= (Df[i]-Df_mid[i]) * (box-box.mid());

		try {

		if (sys.ops[c]==LEQ || sys.ops[c]==LT || sys.ops[c]==EQ){
			double b = rhs.ub();
			count +=check_and_add_constraint(box, a, b, LEQ);
		}

		if (sys.ops[c]==GEQ || sys.ops[c]==GT || sys.ops[c]==EQ){ // && c!=goal_ctr)){
			double b = rhs.lb();
			count +=check_and_add_constraint(box, a, b, GEQ);
		}

		} catch (LPException&) {
			continue;  // just skip this constraint
		} catch (Unsatisfiability&) {
			return -1;
		}


	}


	return count;
}

int LinearizerParallelTaylor::check_and_add_constraint(const IntervalVector& box, const Vector& a, double b, CmpOp op) {

	Interval ax=a*box; // for fast (in)feasibility check

	// ======= Quick (in)feasibility checks
	//                 a*[x] <= rhs ?
	if ( (op==LEQ && ax.lb()>b) || (op==GEQ && ax.ub()<b))
		// the constraint is not satisfied
		throw Unsatisfiability();
	else if ( (op==LEQ && ax.ub()<=b) || (op==GEQ && ax.lb()>=b)) {
		// the (linear) constraint is satisfied for any point in the box
		return 0;
	} else {
		//cout << "add constraint " << a << "*x<=" << b << endl;
		lp_solver->add_constraint(a, op, b); // note: may throw LPException
		return 1;
	}
}







} /* namespace ibex */
