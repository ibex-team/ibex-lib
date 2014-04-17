//============================================================================
//                                  I B E X                                   
// File        : ibex_EpsiLargestFirst.cpp
// Author      : Jordan Ninin
// License     : See the LICENSE file
// Created     : Mars 19, 2014
// Last Update : Mars 19, 2012
//============================================================================

#include "ibex_EpsLargestFirst.h"
#include "ibex_NoBisectableVariableException.h"
using std::pair;

namespace ibex {

EpsLargestFirst::EpsLargestFirst(const Vector &init,  double ratio1) :
				  ratio(ratio1), init_eps(init) {
	for (int i=0;i<(init.size());i++) {
		if (init_eps[i]<=0) init_eps[i]=1.e-20;
	}
}

pair<IntervalVector,IntervalVector> EpsLargestFirst::bisect(const IntervalVector& box) {
	assert(box.size()<=init_eps.size());
	int var =-1;
	double l=0.0;
	for (int i=0; i< box.size(); i++)	{
		if (box[i].is_bisectable()){
			if (var==-1) var=i;
			else {
				double l_tmp =box[i].diam();
				if ((l_tmp>init_eps[i]*1.e-2)&&(l_tmp/init_eps[i]>l)) { var = i; l= l_tmp/init_eps[i];}
			}
		}
	}
	if (var !=-1){
		l=0.0;
		for (int i=0; i< box.size(); i++)	{
			if (box[i].is_bisectable()){
				if (var==-1) var=i;
				else {
					double l_tmp =box[i].diam();
					if ((l_tmp/init_eps[i]>l)) { var = i; l= l_tmp/init_eps[i];}
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
