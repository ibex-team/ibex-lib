//============================================================================
//                                  I B E X                                   
// File        : ibex_LargestFirst.cpp
// Author      : Bertrand Neveu, Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : May 19, 2012
// Last Update : Dec 25, 2017
//============================================================================

#include "ibex_LargestFirst.h"
#include "ibex_NoBisectableVariableException.h"

using namespace std;

namespace ibex {

LargestFirst::LargestFirst(double prec, double ratio1) : Bsc(prec), ratio(ratio1) {

}

LargestFirst::LargestFirst(const Vector& prec, double ratio1) : Bsc(prec), ratio(ratio1) {

}

  bool LargestFirst::nobisectable(const IntervalVector & box, int i) const {
    return too_small (box, i);
  }


BisectionPoint LargestFirst::choose_var(const Cell& cell) {

	const IntervalVector& box=cell.box;

	int var =-1;
	double l=0.0;
	for (int i=0; i< box.size(); i++)	{
	  if (!(nobisectable (box,i))){
			if (var==-1) {
				var=i;
				l = uniform_prec()? box[i].diam() : (box[i].diam()/prec(i));
			}
			else {
				double l_tmp = uniform_prec()? box[i].diam() : (box[i].diam()/prec(i));
				if (l_tmp>l) {
					var = i;
					l = l_tmp;
				}
			}
		}
	}

	if (var !=-1){
		return BisectionPoint(var,ratio,true);
	}
	else {
		throw NoBisectableVariableException();
	}

}

} // end namespace ibex
