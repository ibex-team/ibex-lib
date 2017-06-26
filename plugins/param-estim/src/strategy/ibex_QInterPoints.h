//============================================================================
//                                  I B E X
// File        : ibex_QInterPoints.h
// Author      : Bertrand Neveu
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Apr 15, 2015
// Last Update : Apr 15, 2015
//============================================================================

#ifndef __IBEX_QINTERPOINTS_H__
#define __IBEX_QINTERPOINTS_H__

#include "ibex_Backtrackable.h"
#include <list>
#include <iostream>
#include <stdlib.h>
using namespace std;
namespace ibex {


  // A backtrackable structure used by SolverOptQInter 
  // the points storage  is only used by DFS and diving search.

class QInterPoints : public Backtrackable {
public:
  list<int> points;
  int qmax; // the maximum number of inliers in the node  (used for best first search)
  int depth;
  QInterPoints();
  ~QInterPoints();
  pair<Backtrackable*,Backtrackable*> down();
  QInterPoints(const QInterPoints&);
};










} // end namespace ibex

#endif // __IBEX_QINTERPOINTS_H__
