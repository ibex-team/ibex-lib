/* ============================================================================
 * I B E X - ibex_RelaxationLinearizerSIP.h
 * ============================================================================
 * Copyright   : IMT Atlantique (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Antoine Marendet, Gilles Chabert
 * Created     : May 4, 2018
 * ---------------------------------------------------------------------------- */
 
#ifndef __SIP_IBEX_RELAXATIONLINEARIZERSIP_H__
#define __SIP_IBEX_RELAXATIONLINEARIZERSIP_H__

#include "ibex_NLConstraint.h"
#include "ibex_SIPSystem.h"

#include "ibex_IntervalVector.h"
#include "ibex_Linearizer.h"
#include "ibex_LPSolver.h"
#include "ibex_Vector.h"

#include <vector>

namespace ibex {
/**
 * \brief RelaxationLinearizerSIP
 *
 * Compute linear relaxation for a SIPSystem.
 */
class RelaxationLinearizerSIP: public Linearizer {

public:
	enum CornerPolicy{
		up = 1, down = 2, random = 4
	};
	RelaxationLinearizerSIP(const SIPSystem& system,
			CornerPolicy corner_policy, bool opposite);
	int linearize(const IntervalVector& box, LPSolver& lp_solver);
	int linearize(const IntervalVector& box, LPSolver& lp_solver, BoxProperties& prop);
	int linearizeNLC(const NLConstraint& constraint,
			std::vector<Vector>& lhs, std::vector<double>& rhs) const;
	int linearizeSIC(const SIConstraint& constraint,
			std::vector<Vector>& lhs, std::vector<double>& rhs, SIConstraintCache& cache) const;
private:
	void setCornersAndAlphas();
	const SIPSystem& system_;
	const CornerPolicy corner_policy_;
	const bool opposite_;
	IntervalVector box_;
	std::vector<Vector> corners_;
	std::vector<std::vector<int>> alphas_;

};

} // end namespace ibex

#endif // __SIP_IBEX_RELAXATIONLINEARIZERSIP_H__
