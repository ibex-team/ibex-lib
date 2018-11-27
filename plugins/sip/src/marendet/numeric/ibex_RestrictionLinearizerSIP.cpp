/* ============================================================================
 * I B E X - ibex_RestrictionLinearizerSIP.cpp
 * ============================================================================
 * Copyright   : IMT Atlantique (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Antoine Marendet, Gilles Chabert
 * Created     : May 4, 2018
 * ---------------------------------------------------------------------------- */
 
#include "ibex_RestrictionLinearizerSIP.h"

#include "ibex_utils.h"
#include "ibex_SIConstraint.h"
#include "ibex_SIConstraintCache.h"

#include "ibex_CmpOp.h"
#include "ibex_Function.h"
#include "ibex_Interval.h"

#include <cmath>
#include <cstdlib>

using namespace std;

namespace ibex {

const Interval default_limit_val_box(1.e-6, 1.e6);

RestrictionLinearizerSIP::RestrictionLinearizerSIP(const SIPSystem& system, CornerPolicy corner_policy)
: Linearizer(system.nb_var), system_(system), corner_policy_(corner_policy), box_(nb_var()), corner_(nb_var()+1), alpha_(nb_var()+1) {
    
}

int RestrictionLinearizerSIP::linearize(const IntervalVector& box, LPSolver& lp_solver) {
	ibex_error("RestrictionLinearizerSIP::linearize: called with no box_properties");
	return -1;
}

int RestrictionLinearizerSIP::linearize(const IntervalVector& box, LPSolver& lp_solver, BoxProperties& prop) {
    /*if(box.is_unbounded()) {
        return -1;
    }*/
    BxpNodeData* node_data = (BxpNodeData*) prop[BxpNodeData::id];
	if(node_data == nullptr) {
		ibex_error("RelaxationLinearizerSIP::linearize: BxpNodeData must be set");
	}
    
    int added_count = 0;
    box_ = box;
    setCornerAndAlpha();
    Vector lhs(nb_var());
    double rhs;
    // -1 to remove the linearization constraint of the objective
    for(int i = 0; i < system_.normal_constraints_.size()-1; ++i) {
        added_count += linearizeNLC(system_.normal_constraints_[i], lhs, rhs);
        if(lhs.max() > 1e10 || lhs.min() < -1e10 || !isfinite(lhs) || !std::isfinite(rhs)) {
            return -1;
        }
        lp_solver.add_constraint(lhs, CmpOp::LEQ, rhs);
    }
    std::vector<Vector> lhs_sic;
    std::vector<double> rhs_sic;
    for(int i = 0; i < system_.sic_constraints_.size(); ++i) {
        added_count += linearizeSIC(system_.sic_constraints_[i], lhs_sic, rhs_sic, node_data->sic_constraints_caches[i]);
    }
    for(int i = 0; i < rhs_sic.size(); ++i) {
        if(lhs_sic[i].max() > 1e10 || lhs_sic[i].min() < -1e10 || !isfinite(lhs_sic[i]) || !std::isfinite(rhs_sic[i])) {
            return -1;
        }
        lp_solver.add_constraint(lhs_sic[i], CmpOp::LEQ, rhs_sic[i]);
    }
    return added_count;
}

int RestrictionLinearizerSIP::linearizeNLC(const NLConstraint& nlc, Vector& lhs, double& rhs) const {
    Interval function_value = nlc.evaluate(corner_);
    IntervalVector gradient = nlc.gradient(box_);
    rhs = -function_value.ub();

    for(int i = 0; i < nb_var(); ++i) {
        lhs[i] = alpha_[i] == 0 ? gradient[i].ub() : gradient[i].lb();
        rhs += (Interval(lhs[i])*corner_[i]).ub();
    }
    return 1;
}

int RestrictionLinearizerSIP::linearizeSIC(const SIConstraint& constraint, std::vector<Vector>& lhs, std::vector<double>& rhs, SIConstraintCache& cache) const {
    int added_count = 0;
    // TODO : Sale. Peut etre une fonction getUpdatedCache dans SIConstraint ?
    cache.update_cache(*constraint.function_, box_);
    for(const auto& mem_box : cache.parameter_caches_) {
    	Interval function_value = constraint.evaluate(corner_, mem_box.parameter_box);
        //Interval function_value = mem_box.evaluation;
        IntervalVector gradient = mem_box.full_gradient;
        double rhs_param = -function_value.ub();
        Vector lhs_param(nb_var());
        for(int i = 0; i < nb_var(); ++i) {
            lhs_param[i] = alpha_[i] == 0 ? gradient[i].ub() : gradient[i].lb();
            rhs_param += (Interval(lhs_param[i])*corner_[i]).ub();
        }
        added_count += 1;
        lhs.emplace_back(lhs_param);
        rhs.emplace_back(rhs_param);
    }
    return added_count;
}

void RestrictionLinearizerSIP::setCornerAndAlpha() {
	// nb_var + 1 to allow evaluation, which require the goal variable
    for(int i = 0; i < nb_var() + 1; ++i) {
        switch (corner_policy_) {
            case up:
                alpha_[i] = 1;
                corner_[i] = box_[i].ub();
                break;
            case down:
                alpha_[i] = 0;
                corner_[i] = box_[i].lb();
                break;
            case random:
                alpha_[i] = rand() % 2;
                corner_[i] = (1-alpha_[i])*box_[i].lb() + alpha_[i]*box_[i].ub();
                break;
        }
    }
}
} // end namespace ibex
