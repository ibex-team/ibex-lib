/* ============================================================================
 * I B E X - ibex_SICPaving.h
 * ============================================================================
 * Copyright   : IMT Atlantique (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Antoine Marendet
 * Created     : July 13, 2018
 * ---------------------------------------------------------------------------- */
 
#ifndef __SIP_IBEX_IBEX_SIC_PAVING_H__
#define __SIP_IBEX_IBEX_SIC_PAVING_H__

#include "ibex_SIConstraint.h"
#include "ibex_SIConstraintCache.h"
#include "ibex_SIPSystem.h"

namespace ibex {

/*class FncGradientSubset {
public:
    FncGradientSubset(const Function& f, const BitSet& bitset);
    IntervalVector eval_vector(const IntervalVector& box, const BitSet& components) const;
    void jacobian(const IntervalVector& x, IntervalMatrix& J, const BitSet& components, int v=-1) const;

private:
    BitSet components_;
    const Function& f_;
}*/

void bisect_paving(SIConstraintCache& cache);
bool is_feasible_with_paving(const SIConstraint& constraint, const SIConstraintCache& cache, const IntervalVector& box);
void simplify_paving(const SIConstraint& constraint, SIConstraintCache& cache, const IntervalVector& box, bool with_newton=false);
void monotonicity_filter(const SIConstraint& constraint, SIConstraintCache& cache, const IntervalVector& box);
void evaluation_filter(const SIConstraint& constraint, SIConstraintCache& cache, const IntervalVector& box);
void newton_filter(const SIConstraint& constraint, SIConstraintCache& cache, const IntervalVector& box);
void blankenship(const IntervalVector& box, const SIPSystem& sys, BxpNodeData* node_data);
}

#endif  // __SIP_IBEX_IBEX_SIC_PAVING_H__