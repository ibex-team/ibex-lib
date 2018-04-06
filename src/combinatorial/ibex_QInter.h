//============================================================================
//                                  I B E X                                   
// File        : Q-intersection
// Author      : Gilles Chabert Bertrand Neveu
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Apr 25, 2012
// Last Update : Sep 11, 2015
//============================================================================

#ifndef __IBEX_Q_INTER_H__
#define __IBEX_Q_INTER_H__

#include "ibex_IntervalVector.h"
//#include "ibex_KCoreGraph.h"
#include "ibex_Array.h"
#include "ibex_IntStack.h"
#include "ibex_IntervalMatrix.h"
#include<list>

 // ignored warning : comparison between signed and unsigned integer expressions in this file
 #pragma GCC diagnostic push
 #pragma GCC diagnostic ignored "-Wsign-compare"
 



using namespace std;

namespace ibex {

typedef enum { QINTERPROJ,QINTERCORE,QINTERFULL,QINTERGRID} qintermethod;
/**
 * \defgroup combinatorial Combinatorial Algorithms
 */
 IntervalVector qintermeth(const Array<IntervalVector>& _boxes, int q, qintermethod meth);
/**
 * \ingroup combinatorial
 * \brief Q-intersection - EXACT - Grid algorithm
 */
IntervalVector qinter(const Array<IntervalVector>& _boxes, int q);
IntervalVector qinter(IntervalMatrix& _boxes, int q , int p, list<int>* points);

/**
 * \ingroup combinatorial
 * \brief Q-intersection - HEURISTIC - Projective filtering
 */

// IntervalVector qinter_projf( IntervalMatrix& _boxes, int q, int & qmax, int& p,  int & varbiss, list<int>* points, int n0, int n1=0);
 IntervalVector qinter_projf( IntervalMatrix& _boxes, int q, int & qmax, int& p, list<int>* points, int n0, int n1);
/**
 * \ingroup combinatorial
 * \brief Q-intersection - HEURISTIC - k-core filtering + greedy coloring
 */

// IntervalVector qinter_coref(IntervalMatrix& _boxes, int q, int& p,list<int>* points , int n0);

/**
 * \ingroup combinatorial
 * \brief Q-intersection - EXACT - QInter2 : Cliquer-based solver
 */

// IntervalVector qinter2(const Array<IntervalVector>& _boxes, int q);
// IntervalVector qinter2(IntervalMatrix& _boxes, int q , int p, list<int>* points);
/**
 * \ingroup combinatorial
 * \brief Checks for nonempty Q-intersection (Cliquer)
 */
//IntervalVector qinterex_cliquer(const vector<IntervalVector *>& boxes, const vector<int>& indices, int q, KCoreGraph* origin);

} // end namespace ibex


// Restore warning : comparison between signed and unsigned integer expressions in this file
#pragma GCC diagnostic pop
#endif // __IBEX_Q_INTER_H__
