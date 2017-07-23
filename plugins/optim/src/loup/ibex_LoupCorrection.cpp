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

LoupCorrection::LoupCorrection(const System& sys, LoupFinder& finder, bool trace) : sys(sys), has_equality(false), finder(finder), trace(trace) {
	// ==== check if the system contains equalities ====
	for (int i=0; i<sys.ctrs.size(); i++) {
		if (sys.ctrs[i].op==EQ) {
			(bool&) has_equality = true;
			break;
		}
	}
}

//pair<IntervalVector, double> LoupCorrection::find(double loup, const Vector& loup_point, double pseudo_loup) {
std::pair<IntervalVector, double> LoupCorrection::find(const IntervalVector& box, const IntervalVector& loup_point, double loup) {

	// may throw NotFound
	pair<IntervalVector,double> p=finder.find(box,loup_point,loup);

	if (!has_equality)
		return p;

	// todo : change hard-coded value
	FncActivation af(sys,p.first.lb(),1e-8,trace);

	if (af.image_dim()==0) {
		return p;
	}

	IntervalVector epsbox(p.first);

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
					if (!af.activated()[j])
						if (((sys.ops[j]==LEQ || sys.ops[j]==LT)
							  && sys.ctrs[j].f.eval(pdc.solution()).ub()>0)
								||
							((sys.ops[j]==GEQ || sys.ops[j]==GT)
							  && sys.ctrs[j].f.eval(pdc.solution()).lb()<0)) {

						/* TODO: && !entailed->original(j)*/

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
