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
 * \brief Q-intersection - EXACT - Jaulin-Golsztejn algorithm
 */
IntervalVector qinter(const Array<IntervalVector>& boxes, int q);

/**
 * \ingroup combinatorial
 * \brief Q-intersection - HEURISTIC - Jaulin's algorithm
 */
IntervalVector qinter_jaulin(const Array<IntervalVector>& _boxes, int q);

/**
 * \ingroup combinatorial
 * \brief Q-intersection - HEURISTIC - improved Gutowski algorithm
 */
IntervalVector qinter_gutow(const Array<IntervalVector>& _boxes, int q);

/**
 * \ingroup combinatorial
 * \brief Q-intersection - EXACT - Cliquer-based approach without filtering
 */
IntervalVector qinter_chabs(const Array<IntervalVector>& _boxes, int q);

/**
 * \ingroup combinatorial
 * \brief Q-intersection - EXACT - Cliquer-based approach without filtering, but with nogoods recording
 */
IntervalVector qinter_chabs_nogoods(const Array<IntervalVector>& _boxes, int q);

/**
 * \ingroup combinatorial
 * \brief Q-intersection - EXACT - Cliquer-based approach with filtering
 */
IntervalVector qinter_chabs_gutow(const Array<IntervalVector>& _boxes, int q);

/**
 * \ingroup combinatorial
 * \brief Q-intersection existence using Cliquer
 */
IntervalVector qinterex_cliquer(const vector<IntervalVector *>& boxes, int q);

} // end namespace ibex
#endif // __IBEX_Q_INTER_H__
