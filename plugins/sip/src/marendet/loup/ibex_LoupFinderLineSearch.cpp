//============================================================================
//                                  I B E X                                   
// File        : ibex_LoupFinderLineSearch.cpp
// Author      : Antoine Marendet, Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : May 4, 2018
// Last Update : May 4, 2018
//============================================================================

#include "ibex_LoupFinderLineSearch.h"

#include "system/ibex_SIConstraint.h"
#include "system/ibex_SIConstraintCache.h"

#include "ibex_Cell.h"
#include "ibex_Interval.h"
#include "ibex_Linear.h"
#include "ibex_LinearException.h"
#include "ibex_Matrix.h"
#include "ibex_Vector.h"

#include <vector>

using namespace std;

namespace ibex {

LoupFinderLineSearch::LoupFinderLineSearch(SIPSystem& system) :
		system_(system), linearizer_(system, RelaxationLinearizerSIP::CornerPolicy::random, false), lp_solver_(
				system.ext_nb_var, 10000, 10000) {

}

std::pair<IntervalVector, double> LoupFinderLineSearch::find(const Cell& cell,
		const IntervalVector& loup_point, double loup) {
	lp_solver_.clean_ctrs();
	lp_solver_.set_bounds(cell.box);
	lp_solver_.set_obj_var(system_.ext_nb_var - 1, 1.0);
	lp_solver_.set_sense(LPSolver::MINIMIZE);
	linearizer_.linearize(cell.box, lp_solver_);
	lp_solver_.write_file();

	auto return_code = lp_solver_.solve();
	if (return_code != LPSolver::Status_Sol::OPTIMAL) {
		throw NotFound();
	}
	//Vector sol(cell.box.mid());
	Vector sol = lp_solver_.get_primal_sol();
	Vector sol_without_goal = sol.subvector(0, system_.nb_var - 1);
	//Vector dual(lp_solver_.get_nb_rows());
	Vector dual = lp_solver_.get_dual_sol();
	//IntervalVector rhs(dual.size());
	IntervalVector rhs = lp_solver_.get_lhs_rhs();
	//Matrix A(lp_solver_.get_nb_rows(), system_.ext_nb_var);
	Matrix A = lp_solver_.get_rows();
	vector<Vector> active_constraints;

	// After variables and linearized goal
	for (int i = system_.ext_nb_var + 1; i < A.nb_rows(); ++i) {
		if (!Interval(dual[i]).inflate(1e-10).contains(0)) {
			active_constraints.emplace_back(A.row(i).subvector(0, system_.nb_var-1));
		 }
		/*Interval cst_eval = A.row(i) * sol - rhs[i].ub();
		if (cst_eval.inflate(1e-10).contains(0)) {
			active_constraints.emplace_back(A.row(i).subvector(0, system_.nb_var-1));
		}*/
	}

	for (int i = 0; i < system_.nb_var; ++i) {
		if (Interval(cell.box[i].lb()).inflate(1e-10).contains(sol[i])) {
			Vector cst(system_.nb_var, 0.0);
			//cst[cell.box.size()] = 1;
			cst[i] = 1;
			active_constraints.emplace_back(cst);
		} else if (Interval(cell.box[i].ub()).inflate(1e-10).contains(sol[i])) {
			Vector cst(system_.nb_var, 0.0);
			//cst[cell.box.size()] = 1;
			cst[i] = -1;
			active_constraints.emplace_back(cst);
		}
	}
	if(active_constraints.size() == 0) {
		// That happens when the linear solver does not return a point in a corner of the relaxation
		throw NotFound();
	}
	Matrix G(system_.nb_var, active_constraints.size());
	for(int i = 0; i < active_constraints.size(); ++i) {
		G.set_col(i, active_constraints[i]);
	}
	/*cout << "box=" << cell.box << endl;
	cout << "sol=" << sol << endl;
	cout << "dual=" << dual << endl;
	cout << G.nb_cols() << endl;*/
	/*cout << G << endl;
	 Matrix invG(G.nb_cols(), G.nb_rows());
	 real_inverse(G, invG);
	 Vector eps(invG.nb_cols(), 1);
	 Vector direction = invG*eps;¨*/

	Matrix GtG = G.transpose() * G;
	int* pr = new int[GtG.nb_rows()];
	int* pc = new int[GtG.nb_cols()];
	Matrix LU(GtG.nb_rows(), GtG.nb_rows());
	IntervalVector eps_box(GtG.nb_rows(), Interval(0, 1));
	try {
		real_LU(GtG, LU, pr, pc);
	} catch (SingularMatrixException &e) {
//		std::cerr
//				<< "Node::_getImprovingVectorFromOuterLinearizations: real_LU fail"
//				<< std::endl;
		throw NotFound();
	}
	double best_loup = POS_INFINITY;
	Vector best_loup_point(system_.nb_var);
	bool loup_found = false;
	for (int i = 0; i < 10; ++i) {
		Vector rhs = -eps_box.random();
		Vector u(GtG.nb_rows());
		try {
			real_LU_solve(LU, pr, rhs, u);
		} catch (SingularMatrixException &e) {
			continue;
		}
		Vector direction = G * u;
		//cout << "direction=" << direction << endl;
		//cout << "point=" << sol << endl;
		Interval t = Interval::POS_REALS;
		for (const auto& constraint : system_.sic_constraints_) {
			const auto& cache = constraint.cache_->parameter_caches_;
			for (const auto& mem_box : cache) {
				Interval eval = constraint.evaluate(sol, mem_box.parameter_box);
				/*IntervalVector gradient_x = constraint.gradient(cell.box, mem_box.parameter_box).subvector(0,
						system_.nb_var - 1);*/
				IntervalVector gradient_x = mem_box.full_gradient.subvector(0, system_.nb_var-1);
				t &= (Interval::NEG_REALS - eval.ub()) / (gradient_x * direction).ub();

			}
		}
		// -1 to exclude goal
		for (int i = 0; i < system_.normal_constraints_.size() - 1; ++i) {
			const auto& constraint = system_.normal_constraints_[i];
			IntervalVector gradient_x = constraint.gradient(cell.box).subvector(0, system_.nb_var - 1);
			t &= (Interval::NEG_REALS - constraint.evaluate(sol).ub()) / (gradient_x * direction).ub();
		}

		if (!t.is_empty()) {
			Vector point = sol + t.lb() * direction;
			Vector point_plus_goal(system_.ext_nb_var);
			point_plus_goal.put(0, point);
			if (cell.box.contains(point_plus_goal)) {
				double new_loup = loup;
				if (check(system_, point, new_loup, true) && new_loup < best_loup) {
					best_loup_point = point;
					best_loup = new_loup;
					loup_found = true;
				}
			}
		}
	}
	if (loup_found) {
		return make_pair(best_loup_point, best_loup);
	}
	/*if (t.is_empty())
	 throw NotFound();
	 Vector point = sol + t.lb() * direction;
	 if (!cell.box.contains(point))
	 throw NotFound();
	 double new_loup = loup;
	 if (check(system_, point, new_loup, false)) {
	 Vector loup_point = point.subvector(0, system_.nb_var - 1);
	 return std::make_pair(loup_point, new_loup);
	 }*/
	throw NotFound();
}

LoupFinderLineSearch::~LoupFinderLineSearch() {
	// TODO Auto-generated destructor stub
}

} // end namespace ibex
