//============================================================================
//                                  I B E X                                   
// File        : Q-intersection
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Apr 25, 2012
// Last Update : Apr 25, 2012
//============================================================================

#ifndef __IBEX_Q_INTER_H__
#define __IBEX_Q_INTER_H__

#include "ibex_IntervalVector.h"

// ignored warning : comparison between signed and unsigned integer expressions in this file
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wsign-compare"

#include "ibex_KCoreGraph.h"
#include "ibex_Array.h"
#include "ibex_IntStack.h"

namespace ibex {

/**
 * \defgroup combinatorial Combinatorial Algorithms
 */

/**
 * \ingroup combinatorial
 * \brief Q-intersection - EXACT - Grid algorithm
 */
IntervalVector qinter(const Array<IntervalVector>& boxes, int q);

/**
 * \ingroup combinatorial
 * \brief Q-intersection - HEURISTIC - Projective filtering
 */
IntervalVector qinter_projf(const Array<IntervalVector>& _boxes, int q);

/**
 * \ingroup combinatorial
 * \brief Q-intersection - HEURISTIC - k-core filtering + greedy coloring
 */
IntervalVector qinter_coref(const Array<IntervalVector>& _boxes, int q);

/**
 * \ingroup combinatorial
 * \brief Q-intersection - EXACT - QInter2 : Cliquer-based solver
 */
IntervalVector qinter2(const Array<IntervalVector>& _boxes, int q);

/**
 * \ingroup combinatorial
 * \brief Checks for nonempty Q-intersection (Cliquer)
 */
IntervalVector qinterex_cliquer(const std::vector<IntervalVector *>& boxes, const std::vector<int>& indices, int q, KCoreGraph* origin);

} // end namespace ibex


// Restore warning : comparison between signed and unsigned integer expressions in this file
#pragma GCC diagnostic pop
#endif // __IBEX_Q_INTER_H__
