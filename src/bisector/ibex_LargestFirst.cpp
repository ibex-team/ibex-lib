//============================================================================
//                                  I B E X                                   
// File        : ibex_LargestFirst.cpp
// Author      : Bertrand Neveu
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : May 19, 2012
// Last Update : May 19, 2012
//============================================================================

#include "ibex_LargestFirst.h"
#include "ibex_NoBisectableVariableException.h"
using std::pair;

namespace ibex {

  LargestFirst::LargestFirst( double ratio1) :  ratio(ratio1) {

}

pair<IntervalVector,IntervalVector> LargestFirst::bisect(const IntervalVector& box) {

 int var =-1;
  for (int i=0; i< box.size(); i++)	{
    if (box[i].is_bisectable()){
      if (var==-1) var=i;
      else {
    	  if (box[i].diam()>box[var].diam())  var = i;
      }
    }
  }
  
  if (var !=-1){
    return box.bisect(var,ratio);
  }
  else {
	  throw NoBisectableVariableException();
  }

}





} // end namespace ibex
