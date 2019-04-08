/* ============================================================================
 * I B E X - ibex_RelaxationLinearizerSIP.cpp
 * ============================================================================
 * Copyright   : IMT Atlantique (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Antoine Marendet, Gilles Chabert
 * Created     : May 4, 2018
 * ---------------------------------------------------------------------------- */
 
#include "ibex_RelaxationLinearizerSIP.h"

#include "ibex_utils.h"
#include "ibex_SIConstraint.h"
#include "ibex_SIConstraintCache.h"

#include "ibex_CmpOp.h"
#include "ibex_Interval.h"

#include <cmath>
#include <cstdlib>

using namespace std;

namespace ibex {

RelaxationLinearizerSIP::RelaxationLinearizerSIP(const SIPSystem& system, CornerPolicy corner_policy, bool opposite) :
		Linearizer(system.ext_nb_var), system_(system), corner_policy_(corner_policy), opposite_(opposite), box_(
				nb_var()) {

}

int RelaxationLinearizerSIP::linearize(const IntervalVector& box, LPSolver& lp_solver) {
	ibex_error("RelaxationLinearizerSIP::linearize: called with no box_properties");
	return -1;
}

int RelaxationLinearizerSIP::linearize(const IntervalVector& box, LPSolver& lp_solver, BoxProperties& prop) {
	/*if(box.is_unbounded()) {
		return -1;
	}*/
	BxpNodeData* node_data = (BxpNodeData*) prop[BxpNodeData::id];
	if(node_data == nullptr) {
		ibex_error("RelaxationLinearizerSIP::linearize: BxpNodeData must be set");
	}

	int added_count = 0;
	box_ = box;
	setCornersAndAlphas();
	// NLConstraints
	std::vector<Vector> lhs;
	std::vector<double> rhs;
	for (const auto& nlc : system_.normal_constraints_) {
		added_count += linearizeNLC(nlc, lhs, rhs);
	}
	int sic_index = 0;
	for (const auto& sic : system_.sic_constraints_) {
		added_count += linearizeSIC(sic, lhs, rhs, node_data->sic_constraints_caches[sic_index]);
		sic_index++;
	}
	for (int i = 0; i < rhs.size(); ++i) {
		if (lhs[i].max() < 1e10 && lhs[i].min() > -1e10 && isfinite(lhs[i]) && std::isfinite(rhs[i])) {
			lp_solver.add_constraint(lhs[i], CmpOp::LEQ, rhs[i]);
		} else {
			added_count -= 1;
		}
	}
	return added_count;
}

int RelaxationLinearizerSIP::linearizeNLC(const NLConstraint& nlc, std::vector<Vector>& lhs,
		std::vector<double>& rhs) const {
	int added_count = 0;
	for (int i = 0; i < alphas_.size(); ++i) {
		Interval function_value = nlc.evaluate(corners_[i]);
		IntervalVector gradient = nlc.gradient(box_);
		double rhs_param = -function_value.lb();
		Vector lhs_param(nb_var());
		for (int j = 0; j < nb_var(); ++j) {
			lhs_param[j] = alphas_[i][j] == 0 ? gradient[j].lb() : gradient[j].ub();
			rhs_param += (Interval(lhs_param[j]) * corners_[i][j]).lb();
		}
		added_count += 1;
		lhs.emplace_back(lhs_param);
		rhs.emplace_back(rhs_param);
	}
	return added_count;
}

int RelaxationLinearizerSIP::linearizeSIC(const SIConstraint& constraint, std::vector<Vector>& lhs,
		std::vector<double>& rhs, SIConstraintCache& cache) const {
	int added_count = 0;
	for (int i = 0; i < alphas_.size(); ++i) {
		for (const auto& mem_box : cache.parameter_caches_) {
			Interval function_value = constraint.evaluate(corners_[i], mem_box.parameter_box.mid());
			IntervalVector gradient = constraint.gradient(box_, mem_box.parameter_box.mid());
			double rhs_param = -function_value.lb();
			//std::cout << function_value << std::endl;
			Vector lhs_param(nb_var());
			for (int j = 0; j < nb_var(); ++j) {
				lhs_param[j] = alphas_[i][j] == 0 ? gradient[j].lb() : gradient[j].ub();
				rhs_param += (Interval(lhs_param[j]) * corners_[i][j]).lb();
			}
			added_count += 1;
			lhs.emplace_back(lhs_param);
			rhs.emplace_back(rhs_param);
		}
		for (const auto& parameter_point : cache.best_blankenship_points_) {
			Interval function_value = constraint.evaluate(corners_[i], parameter_point);
			IntervalVector gradient = constraint.gradient(box_, parameter_point);
			double rhs_param = -function_value.lb();
			Vector lhs_param(nb_var());
			for (int j = 0; j < nb_var(); ++j) {
				lhs_param[j] = alphas_[i][j] == 0 ? gradient[j].lb() : gradient[j].ub();
				rhs_param += (Interval(lhs_param[j]) * corners_[i][j]).lb();
			}
			added_count += 1;
			lhs.emplace_back(lhs_param);
			rhs.emplace_back(rhs_param);
		}
	}
	return added_count;
}

void RelaxationLinearizerSIP::setCornersAndAlphas() {
	corners_.clear();
	alphas_.clear();
	std::vector<int> alpha(nb_var());
	Vector corner(nb_var());
	for (int i = 0; i < nb_var(); ++i) {
		// First, check only the corner policy for the first corner
		switch (corner_policy_) {
		case up:
			alpha[i] = 1;
			corner[i] = box_[i].ub();
			break;
		case down:
			alpha[i] = 0;
			corner[i] = box_[i].lb();
			break;
		case random:
			alpha[i] = rand() % 2;
			corner[i] = (1 - alpha[i]) * box_[i].lb() + alpha[i] * box_[i].ub();
			break;
		}
	}
	alphas_.emplace_back(alpha);
	corners_.emplace_back(corner);
	// Then, check if the opposite flag is set
	if (opposite_) {
		std::vector<int> beta(nb_var());
		Vector beta_corner(nb_var());
		for (int i = 0; i < nb_var(); ++i) {
			beta[i] = (alpha[i] + 1) % 2;
			beta_corner[i] = (1 - beta[i]) * box_[i].lb() + beta[i] * box_[i].ub();
		}
		alphas_.emplace_back(beta);
		corners_.emplace_back(beta_corner);
	}
}
} // end namespace ibex
