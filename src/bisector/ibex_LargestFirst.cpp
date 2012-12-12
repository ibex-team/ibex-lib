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

using std::pair;

namespace ibex {

  LargestFirst::LargestFirst( double ratio) :  ratio(ratio) {

}

pair<IntervalVector,IntervalVector> LargestFirst::bisect(const IntervalVector& box) {

  int var =0;
  for (int i=1; i< box.size(); i++)
    if (box[i].diam()>box[var].diam() && box[i].mag () < POS_INFINITY  )
      var = i;    

  return box.bisect(var,ratio);
}





} // end namespace ibex
