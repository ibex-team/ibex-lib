//============================================================================
//                                  I B E X
// File        : ibex_LPSolver.cpp
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : May 15, 2013 (Jordan Ninin)
// Last update : Feb 13, 2025 (Gilles Chabert)
//============================================================================

#include "ibex_LPSolver.h"

namespace ibex {

constexpr double LPSolver::default_tolerance;

constexpr double LPSolver::default_timeout;

constexpr int LPSolver::default_max_iter;

/** \brief Stream out \a x. */
std::ostream& operator<<(std::ostream& os, const LPSolver::Status x){

	switch(x) {
	case(LPSolver::Status::Optimal) :{
			os << "OPTIMAL";
			break;
	}
	case(LPSolver::Status::Infeasible) :{
			os << "INFEASIBLE";
			break;
	}
	case(LPSolver::Status::OptimalProved) :{
			os << "OPTIMAL_PROVED";
			break;
	}
	case(LPSolver::Status::InfeasibleProved) :{
			os << "INFEASIBLE_PROVED";
			break;
	}
	case(LPSolver::Status::Timeout) :{
			os << "TIME_OUT";
			break;
	}
	case(LPSolver::Status::MaxIter) :{
			os << "MAX_ITER";
			break;
	}
	case(LPSolver::Status::Unbounded) :{
			os << "UNBOUNDED";
			break;
	}
	case(LPSolver::Status::Unknown) :{
		os << "UNKNOWN";
		break;
	}
	}
	return os;
}

namespace {

// Computes lambda'*b (in interval arithmetic) and A'*lambda (in floating
// point, as the dense product Matrix*Vector did) row by row, skipping the
// rows with a null multiplier. The simplex multipliers are sparse (only the
// rows active in the basis are nonzero), so this avoids building rows() and
// its transpose, two dense matrices, at every call.
void sparse_dual_products(const LPSolver& lp, const Vector& lambda, Interval& lambda_b, Vector& At_lambda) {
	const int n = lp.nb_vars();
	const int m = lp.nb_rows();
	lambda_b = Interval::zero();
	At_lambda = Vector::zeros(n);
	for (int i = 0; i < m; ++i) {
		// A row with lhs > rhs makes the polytope empty. The dense product
		// propagated its empty interval to the bound even with a null
		// multiplier (and CtcPolytopeHull then empties the box): keep that.
		if (lp.lhs_rhs(i).is_empty()) { lambda_b.set_empty(); return; }
		const double li = lambda[i];
		if (li == 0.0) continue;
		lambda_b += li*lp.lhs_rhs(i);
		const Vector r = lp.row(i);
		for (int j = 0; j < n; ++j)
			if (r[j] != 0.0) At_lambda[j] += li*r[j];
	}
}

} // anonymous namespace

bool LPSolver::neumaier_shcherbina_postprocessing() {
	Interval lambda_b;
	Vector At_lambda(nb_vars());
	sparse_dual_products(*this, uncertified_dual_, lambda_b, At_lambda);
	IntervalVector rest(At_lambda);
	rest -= cost();
	obj_ = lambda_b - rest*ivec_bounds_;
	return true;
}

bool LPSolver::neumaier_shcherbina_infeasibility_test() {
    Vector lambda(1);
	// It is possible that the solver does not find an infeasible direction
	// even when the problem is infeasible.
    bool infeasible_dir_found = uncertified_infeasible_dir(lambda);
    if(!infeasible_dir_found) {
        return false;
    }

	Interval lambda_b;
	Vector At_lambda(nb_vars());
	sparse_dual_products(*this, lambda, lambda_b, At_lambda);
    IntervalVector rest(At_lambda);
    Interval d = rest * ivec_bounds_ - lambda_b;

    // if 0 does not belong to d, the infeasibility is proved
    return !d.contains(0.0);
}


void LPSolver::invalidate() {
    status_ = LPSolver::Status::Unknown;
    has_solution_ = false;
    has_infeasible_dir_ = false;
    bound_at_stop_ = NEG_INFINITY;
}

double LPSolver::proved_bound_at_stop() const {
    return bound_at_stop_;
}

bool LPSolver::is_feasible() const {
	return (status_ == LPSolver::Status::Optimal && mode_ == LPSolver::Mode::NotCertified)
	|| (status_ == LPSolver::Status::OptimalProved && mode_ == LPSolver::Mode::Certified);
}

void LPSolver::enable_statistics(Statistics& sts, const std::string& prefix) {
	if (!statistics) 
		sts.add(statistics = new StsLPSolver(prefix + "/LP Solver"));
}

}  // end namespace ibex
