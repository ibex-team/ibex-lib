#include "ibex_LPSolver2.h"

namespace ibex {

/** \brief Stream out \a x. */
std::ostream& operator<<(std::ostream& os, const LPSolver::Status x){

	switch(x) {
	case(LPSolver::Optimal) :{
			os << "OPTIMAL";
			break;
	}
	case(LPSolver::Infeasible) :{
			os << "INFEASIBLE";
			break;
	}
	case(LPSolver::OptimalProved) :{
			os << "OPTIMAL_PROVED";
			break;
	}
	case(LPSolver::InfeasibleProved) :{
			os << "INFEASIBLE_PROVED";
			break;
	}
	case(LPSolver::Timeout) :{
			os << "TIME_OUT";
			break;
	}
	case(LPSolver::MaxIter) :{
			os << "MAX_ITER";
			break;
	}
	case(LPSolver::Unknown) :{
		os << "UNKNOWN";
		break;
	}
	}
	return os;
}

void LPSolver::neumaier_shcherbina_postprocessing() {
    Matrix A_trans = rows_transposed();
    IntervalVector b = lhs_rhs();
    Vector obj = obj();

    IntervalVector rest = A_trans*uncertified_dual_;
    if(sense_ == LPSolver::Sense::Minimize) {
        rest -= obj;
    } else {
        rest += obj;
    }
    certified_obj_ = uncertified_dual_*b - rest*ivec_bounds_;

}

bool LPSolver::neumaier_shcherbina_infeasibility_test() {
    ibex::Matrix A_trans = rows_transposed();
    IntervalVector b = lhs_rhs();
    IntervalVector lambda(1);
    bool b = uncertified_infeasible_dir(lambda);
    if(!b) {
        return false;
    }


    IntervalVector rest = A_trans * lambda ;
    Interval d = rest * ivec_bounds_ - Lambda * b;

    // if 0 does not belong to d, the infeasibility is proved
    return !d.contains(0.0);
}


void LPSolver::invalidate() {
    status_ = LPSolver::Status::Unknown;
    has_solution_ = false;
    delete uncertified_infeasible_dir_;
    delete certified_obj_;
}

}  // end namespace ibex
