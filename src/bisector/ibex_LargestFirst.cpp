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

LargestFirst::LargestFirst(double prec, double ratio1) : Bsc(prec), ratio(ratio1) {

}

LargestFirst::LargestFirst(const Vector& prec, double ratio1) : Bsc(prec), ratio(ratio1) {

}

pair<IntervalVector,IntervalVector> LargestFirst::bisect(const IntervalVector& box) {

	int var =-1;
	double l=0.0;
	for (int i=0; i< box.size(); i++)	{
		if (!too_small(box,i)){
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
		return box.bisect(var,ratio);
	}
	else {
		throw NoBisectableVariableException();
	}

}





} // end namespace ibex
