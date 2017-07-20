/*
 * ibex_LoupCorrection.cpp
 *
 *  Created on: Jul 20, 2017
 *      Author: gilles
 */

#include "ibex_LoupCorrection.h"
#include "ibex_PdcHansenFeasibility.h"
#include "ibex_FncActivation.h"

namespace ibex {

using namespace std;

LoupCorrection::LoupCorrection(const System& sys, bool trace) : sys(sys), trace(trace) {

}

pair<IntervalVector, double> LoupCorrection::find(double loup, const Vector& loup_point, double pseudo_loup) {

	// todo : change hard-coded value
	FncActivation af(sys,loup_point,1e-8,trace);

	if (af.image_dim()==0) {
		return make_pair(loup_point, pseudo_loup);
	}

	IntervalVector epsbox(loup_point);

	// ====================================================
	// solution #1: we inflate the loup-point and
	//              call Hansen test in contracting mode.
	// TODO: replace default_equ_eps by something else!
	//	epsbox.inflate(default_equ_eps);
	//	PdcHansenFeasibility pdc(af, false);
	// ====================================================

	// ====================================================
	// solution #2: we call Hansen test in inflating mode.
	PdcHansenFeasibility pdc(af, true);
	// ====================================================

	// TODO: maybe we should check first if the epsbox is inner...
	// otherwise the probability to get a feasible point is
	// perhaps too small?

	// TODO: HansenFeasibility uses midpoint
	//       but maybe we should use random

	if (pdc.test(epsbox)==YES) {
		Interval resI = sys.goal->eval(pdc.solution());
		if (!resI.is_empty()) {
			double res=resI.ub();
			if (res<loup) {
				//note: don't call is_inner because it would check again all equalities (which is useless
				// and perhaps wrong as the solution box may violate the relaxed inequality (still, very unlikely))
				bool satisfy_inequalities=true;
				for (int j=0; j<sys.nb_ctr; j++) {
					if (sys.ctrs[j].op!=EQ /* TODO: && !entailed->original(j)*/ &&
						sys.ctrs[j].f.eval(pdc.solution()).ub()>0) {
						satisfy_inequalities=false;
						break;
					}
				}
				if (satisfy_inequalities) {
					return make_pair(pdc.solution(), res);
				}
			}
		}
	}
	//===========================================================
	throw NotFound();
}

} /* namespace ibex */
