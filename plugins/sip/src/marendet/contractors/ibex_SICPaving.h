//============================================================================
//                                  I B E X                                   
// File        : ibex_SICPaving.h
// Author      : Antoine Marendet
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : July 13, 2018
// Last Update : July 13, 2018
//============================================================================

#ifndef __SIP_IBEX_IBEX_SIC_PAVING_H__
#define __SIP_IBEX_IBEX_SIC_PAVING_H__

#include "ibex_SIConstraint.h"
#include "ibex_SIConstraintCache.h"

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

}

#endif  // __SIP_IBEX_IBEX_SIC_PAVING_H__