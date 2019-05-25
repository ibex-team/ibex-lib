//============================================================================
//                                  I B E X                                   
// File        : ibex_OptimLargestFirst.cpp
// Author      : Bertrand Neveu, Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : May 19, 2012
// Last Update : Dec 25, 2017
//============================================================================
#include "float.h"
#include "ibex_OptimLargestFirst.h"
#include "ibex_NoBisectableVariableException.h"

using namespace std;

namespace ibex {

  double objectivebisect_ratiolimit=1.e10;

  OptimLargestFirst::OptimLargestFirst(int goal_var,double prec,  double ratio) : LargestFirst(prec, ratio), goal_var(goal_var)  {
}

  OptimLargestFirst::OptimLargestFirst(int goal_var,const Vector& prec,double ratio) :LargestFirst(prec, ratio), goal_var(goal_var) {

}

  
BisectionPoint OptimLargestFirst::choose_var(const Cell& cell) {
  const IntervalVector& box=cell.box;
	max_diam_nobj=0;
	for (int i=0; i< box.size(); i++){
	  if (i != goal_var)
	    max_diam_nobj=std::max(max_diam_nobj,box[i].diam());
	}
	return LargestFirst::choose_var(cell) ;
}
  


  // supplementary sufficient condition for not bisecting the objective (not bounded or too big :max_diam_nobj is the maximum diameter for the other variables)
  bool OptimLargestFirst::nobisectable(const IntervalVector& box, int i) const {
    return (LargestFirst::nobisectable ( box, i) 
	     ||
	    (i == goal_var // to avoid bisecting a no bounded objective
	     && max_diam_nobj < DBL_MAX
	     && box[i].diam()/max_diam_nobj > objectivebisect_ratiolimit)
	    );
  }

} // end namespace ibex
