//============================================================================
//                                  I B E X
// File        : ibex_LoupFinderDuality.cpp
// Author      : Gilles Chabert, Alexandre Goldsztejn
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Mar 20, 2019
// Last Update : Mar 20, 2019
//============================================================================

#include "ibex_LoupFinderDuality.h"

using namespace std;

namespace ibex {

LoupFinderDuality::LoupFinderDuality(const System& sys) : sys(sys) { }

void LoupFinderDuality::add_property(const IntervalVector& init_box, BoxProperties& prop) {
	//--------------------------------------------------------------------------
	/* Using system cache seems not interesting. */
	//	if (/*slope==TAYLOR && */!prop[BxpSystemCache::get_id(sys)]) {
	//		prop.add(new BxpSystemCache(sys,BxpSystemCache::default_update_ratio));
	//	}
	//--------------------------------------------------------------------------
}


LPSolver* LoupFinderDuality::linearize(const IntervalVector& box) {
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

	size_t n_total = n +  active->size()*n;

	int LP_max_iter = LPSolver::default_max_iter;
	if  (n_total*3>LP_max_iter) LP_max_iter = n_total*3; //TODO: remove this recipe

	LPSolver* lp_solver = new LPSolver(n_total, LP_max_iter);

	IntervalVector point=box.mid();

	if (!active->empty()) {

		IntervalVector dual_box(active->size()*n, Interval::POS_REALS);

		IntervalVector full_box = cart_prod(box,dual_box);

		lp_solver->set_bounds(full_box);

		//IntervalMatrix J=cache? cache->active_ctrs_jacobian() : sys.f_ctrs.jacobian(box,active);
		//IntervalMatrix J=sys.f_ctrs.jacobian(box,active);
		IntervalMatrix J(active->size(),n); // derivatives over the box
		sys.f_ctrs.hansen_matrix(box,point,J,*active);
		if (J.is_empty()) {
			delete lp_solver;
			throw NotFound(); // note: no way to inform that the box is actually infeasible
		}

		// the evaluation of the constraints in the point
		IntervalVector gx(sys.f_ctrs.eval_vector(point,*active));
		if (gx.is_empty()) {
			delete lp_solver;
			throw NotFound();
		}

		int i=0; // counter of active constraints
		for (BitSet::const_iterator c=active->begin(); c!=active->end(); ++c) {
			for (int j=0; j<n; j++) {
				Vector row(n_total,0.0);
				row[j]=1;
				row[n + i*n +j]=1;

				double rhs = point[j].lb();

				lp_solver->add_constraint(row, LEQ, rhs);
			}

			{
				Vector row(n_total,0.0);
				row.put(0,J[c].lb());

				IntervalVector Gl(J[c].lb());

				Vector diam_correctly_rounded = (IntervalVector(J[c].ub())-Gl).lb();

				row.put(n + c*n,-diam_correctly_rounded);

				double rhs = (-gx[i] + (Gl*point)).lb();

				lp_solver->add_constraint(row, LEQ, rhs);
			}

		}
	} else {
		lp_solver->set_bounds(box);
	}

	IntervalVector igoal=sys.goal->gradient(point);
	if (igoal.is_empty()) { // unfortunately, at the midpoint the function is not differentiable
		delete lp_solver;
		throw NotFound(); // not a big deal: wait for another box...
	}

	Vector goal=igoal.mid();

	// set the objective coefficient
	// TODO: replace with lp_solver.set_obj(goal) when implemented
	for (int j=0; j<n; j++)
		lp_solver->set_obj_var(j,goal[j]);
	for (int i=0; i<active->size(); i++)
		lp_solver->set_obj_var(n+i,0);

	return lp_solver;
}

std::pair<IntervalVector, double> LoupFinderDuality::find(const IntervalVector& box, const IntervalVector&, double current_loup, BoxProperties& prop) {

	double d=box.max_diam();
	if (d < LPSolver::min_box_diam || d > LPSolver::max_box_diam)
		throw NotFound();

	int n=sys.nb_var;

	LPSolver* lp_solver = linearize(box); // may throw NotFound --> forwarded

	LPSolver::Status_Sol stat = lp_solver->solve();

	if (stat == LPSolver::OPTIMAL) {
		//the linear solution is mapped to intervals and evaluated
		Vector loup_point = lp_solver->get_primal_sol().subvector(0,n-1);

		//std::cout << " simplex result " << prim[0] << " " << loup_point << std::endl;

		if (!box.contains(loup_point)) {
			delete lp_solver;
			throw NotFound();
		}

		double new_loup=current_loup;

		if (check(sys,loup_point,new_loup,false)) {
			delete lp_solver;
			return std::make_pair(loup_point,new_loup);
		}
	}

	delete lp_solver;
	throw NotFound();
}

} /* namespace ibex */
