//============================================================================
//                                  I B E X                                   
// File        : ibex_LinearizerDuality.cpp
// Author      : Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Mar 29, 2019
//============================================================================

#include "ibex_LinearizerDuality.h"
#include "ibex_BxpSystemCache.h"
#include "ibex_BxpLinearRelaxArgMin.h"

using namespace std;

namespace ibex {

LinearizerDuality::LinearizerDuality(const NormalizedSystem& sys, slope_formula slope) :
			Linearizer(sys.nb_var), sys(sys), slope(slope), pt(sys.nb_var) {

}

int LinearizerDuality::linearize(const IntervalVector& box, LPSolver& lp_solver)  {
	BoxProperties prop(box);
	return linearize(box, lp_solver, prop);
}

int LinearizerDuality::linearize(const IntervalVector& box, LPSolver& lp_solver, BoxProperties& prop)  {
	// ========= get active constraints ===========
	/* Using system cache seems not interesting. */
	//BxpSystemCache* cache=(BxpSystemCache*) prop[BxpSystemCache::get_id(sys)];
	BxpSystemCache* cache=NULL;
	//--------------------------------------------------------------------------

	BitSet* active;

	if (cache!=NULL) {
		active = &cache->active_ctrs();
	} else {
		active = new BitSet(sys.active_ctrs(box));
	}
	// ============================================

	size_t n = sys.nb_var;
	size_t m = sys.f_ctrs.image_dim();
	size_t n_total = n +  m*n;

	int nb_ctr=0; // number of inequalities added in the LP solver

//	BxpLinearRelaxArgMin* argmin=(BxpLinearRelaxArgMin*) prop[BxpLinearRelaxArgMin::get_id(sys)];
//
//	if (argmin && argmin->argmin()) {
//		pt=*argmin->argmin();
//	} else
		pt=box.mid();

	if (!active->empty()) {

		//IntervalMatrix J=cache? cache->active_ctrs_jacobian() : sys.f_ctrs.jacobian(box,active);
		//IntervalMatrix J=sys.f_ctrs.jacobian(box,*active);
		IntervalMatrix J(active->size(),n); // derivatives over the box
		sys.f_ctrs.hansen_matrix(box,pt,J,*active);

		if (J.is_empty()) {
			if (cache==NULL) delete active;
			return -1;
		}

		// the evaluation of the constraints in the point
		IntervalVector gx(sys.f_ctrs.eval_vector(pt,*active));
		if (gx.is_empty()) {
			if (cache==NULL) delete active;
			return 0;
		}


		int i=0; // counter of active constraints
		for (BitSet::iterator c=active->begin(); c!=active->end(); ++c, i++)  {

			if (!sys.f_ctrs.deriv_calculator().is_linear[c]) {
				for (int j=0; j<n; j++) {
					Vector row(n_total,0.0);
					row[j]=1;
					row[n + c*n +j]=1;

					double rhs = pt[j] - lp_solver.get_epsilon();

					lp_solver.add_constraint(row, LEQ, rhs);
					nb_ctr++;
				}
			}

			Vector row(n_total,0.0);
			row.put(0,J[i].lb());

			IntervalVector gl(J[i].lb());

			Vector diam_correctly_rounded = (IntervalVector(J[i].ub())-gl).lb();

			for (int j=0; j<n; j++) {
				if (diam_correctly_rounded[j]<0)
					ibex_error("negative diameter");
			}

			row.put(n + c*n,-diam_correctly_rounded);

			double rhs = (-gx[i] + (gl*pt)).lb()- lp_solver.get_epsilon();

			lp_solver.add_constraint(row, LEQ, rhs);
			nb_ctr++;
		}
	}

	if (cache==NULL) delete active;
	return nb_ctr;
}


} // namespace ibex
