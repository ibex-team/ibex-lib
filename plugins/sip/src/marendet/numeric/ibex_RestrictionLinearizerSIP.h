/* ============================================================================
 * I B E X - ibex_RestrictionLinearizerSIP.h
 * ============================================================================
 * Copyright   : IMT Atlantique (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Antoine Marendet, Gilles Chabert
 * Created     : May 4, 2018
 * ---------------------------------------------------------------------------- */
 
#ifndef __SIP_IBEX_RESTRICTIONLINEARIZERSIP_H__
#define __SIP_IBEX_RESTRICTIONLINEARIZERSIP_H__

#include "ibex_NLConstraint.h"
#include "ibex_SIPSystem.h"

#include "ibex_IntervalVector.h"
#include "ibex_Linearizer.h"
#include "ibex_LPSolver.h"
#include "ibex_Vector.h"

#include <vector>

namespace ibex {
class RestrictionLinearizerSIP: public Linearizer {

public:
	enum CornerPolicy {
		up, down, random
	};

	const static Interval default_limit_val_box;

	RestrictionLinearizerSIP(const SIPSystem& system,
			CornerPolicy corner_policy);
	int linearize(const IntervalVector& box, LPSolver& lp_solver);
	int linearize(const IntervalVector& box, LPSolver& lp_solver, BoxProperties& prop);
	int linearizeNLC(const NLConstraint& constraint, Vector& lhs,
			double& rhs) const;
	int linearizeSIC(const SIConstraint& constraint,
			std::vector<Vector>& lhs, std::vector<double>& rhs, SIConstraintCache& cache) const;
private:
	void setCornerAndAlpha();

	const SIPSystem& system_;
	const CornerPolicy corner_policy_;
	IntervalVector box_;
	Vector corner_;
	std::vector<int> alpha_;

};
} // end namespace ibex

#endif // __SIP_IBEX_RESTRICTIONLINEARIZERSIP_H__
