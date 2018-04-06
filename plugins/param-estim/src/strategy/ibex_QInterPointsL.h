//============================================================================
//                                  I B E X
// File        : ibex_QInterPointsL.h
// Author      : Bertrand Neveu
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Apr 15, 2015
// Last Update : Apr 15, 2015
//============================================================================

#ifndef __IBEX_QINTERPOINTSL_H__
#define __IBEX_QINTERPOINTSL_H__

#include "ibex_Backtrackable.h"
#include <list>
#include <iostream>
#include <stdlib.h>
using namespace std;
namespace ibex {


  /* A backtrackable structure used by SolverQInter (dfs algorithm for searching all solutions) */

class QInterPointsL : public Backtrackable {
public:
  list<int>* points; // the list of measurements 
  int qmax; // the maximum number of inliers in the node  (used for best first search)
  int depth;
  
  QInterPointsL();
  ~QInterPointsL();
  pair<Backtrackable*,Backtrackable*> down(const BisectionPoint& b);
  QInterPointsL(const QInterPointsL&);
  Backtrackable* copy() const;
};










} // end namespace ibex

#endif // __IBEX_QINTERPOINTS_H__
