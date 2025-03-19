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

bool LPSolver::neumaier_shcherbina_postprocessing() {
    Matrix A_trans = rows_transposed();
    IntervalVector b = lhs_rhs();
    IntervalVector rest = A_trans*uncertified_dual_;
	rest -= cost();
	//Interval certified_obj_raw = uncertified_dual_*b - rest*ivec_bounds_;
	//certified_obj_ = Interval(certified_obj_raw.lb(), uncertified_obj_.ub());
    obj_ = uncertified_dual_*b - rest*ivec_bounds_;
	return true;
}

bool LPSolver::neumaier_shcherbina_infeasibility_test() {
    ibex::Matrix A_trans = rows_transposed();
    IntervalVector b = lhs_rhs();
    Vector lambda(1);
	// It is possible that the solver does not find an infeasible direction
	// even when the problem is infeasible.
    bool infeasible_dir_found = uncertified_infeasible_dir(lambda);
    if(!infeasible_dir_found) {
        return false;
    }


    IntervalVector rest = A_trans * lambda ;
    Interval d = rest * ivec_bounds_ - lambda * b;

    // if 0 does not belong to d, the infeasibility is proved
    return !d.contains(0.0);
}


void LPSolver::invalidate() {
    status_ = LPSolver::Status::Unknown;
    has_solution_ = false;
    has_infeasible_dir_ = false;
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
