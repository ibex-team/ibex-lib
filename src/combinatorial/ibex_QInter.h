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
#include "ibex_KCoreGraph.h"
#include "ibex_Array.h"
#include "ibex_IntStack.h"

using namespace std;

namespace ibex {

/**
 * \defgroup combinatorial Combinatorial Algorithms
 */

/**
 * \ingroup combinatorial
 * \brief Q-intersection - EXACT - Jaulin-Goldsztejn algorithm
 */
IntervalVector qinter(const Array<IntervalVector>& boxes, int q);

/**
 * \ingroup combinatorial
 * \brief Q-intersection - HEURISTIC - Luc Jaulin's algorithm
 */
IntervalVector qinter_projf(const Array<IntervalVector>& _boxes, int q);

/**
 * \ingroup combinatorial
 * \brief Q-intersection - HEURISTIC - k-core filtering + greedy coloring
 */
IntervalVector qinter_coref(const Array<IntervalVector>& _boxes, int q);

/**
 * \ingroup combinatorial
 * \brief Q-intersection - EXACT - Cliquer-based approach
 */
IntervalVector qinter_chabs_nogoods(const Array<IntervalVector>& _boxes, int q);

/**
 * \ingroup combinatorial
 * \brief Checks for nonempty Q-intersection (Cliquer)
 */
IntervalVector qinterex_cliquer(const vector<IntervalVector *>& boxes, const vector<int>& indices, int q, KCoreGraph* origin);

} // end namespace ibex
#endif // __IBEX_Q_INTER_H__
