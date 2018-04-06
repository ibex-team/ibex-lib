//============================================================================
//                                  I B E X
// File        : ibex_ValidPoint.h
// Author      : Bertrand Neveu
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : May 27, 2015
// Last Update : May 27, 2015
//============================================================================

#ifndef __IBEX_VALIDPOINT_H__
#define __IBEX_VALIDPOINT_H__

#include "ibex_Backtrackable.h"
#include "ibex_IntervalVector.h"


#include <list>
#include <iostream>
#include <stdlib.h>
using namespace std;
namespace ibex {

/**
 * \ingroup strategy
 *
 * \brief  Solver.
 *
 * This class implements a backtrackable datastructure used by QInterSolver : it stores the point where a set 
 * of measures has been validated and the number (the size of the validated measures set)
 * the set itself is not stored (it is recomputed when a solution is found).
 */


class ValidPoint : public Backtrackable {
public:
  Vector* point;
  int validpoints_number;
  
  ValidPoint();
  ~ValidPoint();
  pair<Backtrackable*,Backtrackable*> down(const BisectionPoint& b);
  ValidPoint(const ValidPoint&);
  Backtrackable* copy() const;
};










} // end namespace ibex

#endif // __IBEX_VALIDPOINT_H__
