//============================================================================
//                                  I B E X
// File        : ibex_LoupFinderCertify.cpp
// Author      : Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Jul 20, 2017
//============================================================================

#include "ibex_PdcHansenFeasibility.h"
#include "ibex_FncActiveCtrs.h"
#include "ibex_LoupFinderCertify.h"
#include "ibex_NormalizedSystem.h"

namespace ibex {

using namespace std;

LoupFinderCertify::LoupFinderCertify(const System& sys, LoupFinder& finder) : sys(sys), has_equality(false), finder(finder) {

	if (sys.nb_ctr>0)
		// ==== check if the system contains equalities ====
		for (int i=0; i<sys.f_ctrs.image_dim(); i++) {
			if (sys.ops[i]==EQ) {
				(bool&) has_equality = true;
				break;
			}
		}
}

//pair<IntervalVector, double> LoupCorrection::find(double loup, const Vector& loup_point, double pseudo_loup) {
std::pair<IntervalVector, double> LoupFinderCertify::find(const IntervalVector& box, const IntervalVector& loup_point, double loup) {

	IntervalVector epsbox(box.size());
	bool pseudo_loup_found=true;
	pair<IntervalVector,double> p;

	try {
		p=finder.find(box,loup_point,loup);
		epsbox = p.first;
	} catch(NotFound&) {
		pseudo_loup_found=false;
		epsbox=box.mid();
	}

	if (pseudo_loup_found && !has_equality)
		return p;

	// Loop while the number of active constraints is less
	// than the number of variables (we cannot make proofs
	// with overconstrained sytems)

	FncActiveCtrs *af = NULL;

	// TODO : how to fix initial detection threshold in a more adaptative way?
	double eps_h = NormalizedSystem::default_eps_h;

	do {
		try {
			// try with a smaller threshold...
			if (af) delete af;
			af = new FncActiveCtrs(sys,epsbox.lb(),eps_h,false);
		} catch(FncActiveCtrs::NothingActive&) {
			if (pseudo_loup_found)
				return p;
			else
				throw NotFound();
		}
		eps_h /= 2.;
	} while (af->image_dim() > sys.nb_var && eps_h>min_activity_thershold);
	if (af->image_dim() > sys.nb_var) {
		ibex_warning("too many active constraints, cannot prove feasibility -> loup lost!");
	}

	// ====================================================
	// solution #1: we inflate the loup-point and
	//              call Hansen test in contracting mode.
	//	epsbox.inflate(NormalizedSystem::default_eps_h);
	//	PdcHansenFeasibility pdc(af, false);
	// ====================================================

	// ====================================================
	// solution #2: we call Hansen test in inflating mode.
	PdcHansenFeasibility pdc(*af, true);
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

				// we must check the inequalities that are not active (for
				// the latters, feasibility is already OK).
				// It is unlikely that they are violated but still possible.
				if (!sys.box.intersects(pdc.solution()))
					satisfy_inequalities=false;
				else if (sys.nb_ctr>0) {
					for (int j=0; j<sys.f_ctrs.image_dim(); j++) {
						if (!af->active_ctr[j])
							if (((sys.ops[j]==LEQ || sys.ops[j]==LT)
									&& sys.f_ctrs.eval(j,pdc.solution()).ub()>0)
									||
									((sys.ops[j]==GEQ || sys.ops[j]==GT)
											&& sys.f_ctrs.eval(j,pdc.solution()).lb()<0)) {


								satisfy_inequalities=false;
								break;
							}
					}
				}
				if (satisfy_inequalities) {
					delete af;
					return make_pair(pdc.solution(), res);
				}
			}
		}
	}

	//===========================================================
	delete af;
	throw NotFound();
}

} /* namespace ibex */
