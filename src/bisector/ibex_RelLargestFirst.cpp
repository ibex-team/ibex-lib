//============================================================================
//                                  I B E X                                   
// File        : ibex_RelLargestFirst.cpp
// Author      : Jordan Ninin
// License     : See the LICENSE file
// Created     : Mars 19, 2014
// Last Update : Mars 19, 2012
//============================================================================

#include "ibex_RelLargestFirst.h"
#include "ibex_NoBisectableVariableException.h"
using std::pair;

namespace ibex {

RelLargestFirst::RelLargestFirst( const IntervalVector &init, double ratio1) :
				  ratio(ratio1), init_diam(init.diam()) {
	for (int i=0;i<(init.size());i++) {
		if (init_diam[i]>1.e20) init_diam[i]=1.e20;
	}

}

pair<IntervalVector,IntervalVector> RelLargestFirst::bisect(const IntervalVector& box) {
	assert(box.size()<=init_diam.size());

	int var =-1;
	double l;
	for (int i=0; i< box.size(); i++)	{
		if (box[i].is_bisectable()){
			if (var==-1) {
				var=i; l=box[i].diam()/init_diam[i];
			}
			else {
				if ((box[i].diam()/init_diam[i])>l)  {
					var = i;  l=box[i].diam()/init_diam[i];
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
