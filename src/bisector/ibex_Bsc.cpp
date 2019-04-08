//============================================================================
//                                  I B E X                                   
// File        : ibex_Bsc.cpp
// Author      : Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : May 8, 2012
// Last Update : Jul 6, 2018
//============================================================================

#include "ibex_Bsc.h"
#include "ibex_Exception.h"
#include "ibex_Id.h"

using namespace std;

namespace ibex {

double Bsc::default_ratio() {
	return 0.45;
}

Bsc::Bsc(double prec) : _prec(1,prec) {
	if (prec<0) ibex_error("precision must be a nonnegative number");
	// note: prec==0 allowed with, e.g., LargestFirst
}

Bsc::Bsc(const Vector& prec) : _prec(prec) {
	for (int i=0; i<prec.size(); i++)
		if (prec[i]<=0) ibex_error("precision must be a nonnegative number");
}

void Bsc::add_property(const IntervalVector& init_box, BoxProperties& map) {

}

pair<IntervalVector,IntervalVector> Bsc::bisect(const IntervalVector& box) {
	Cell cell(box);
	pair<Cell*,Cell*> p=bisect(cell);
	pair<IntervalVector,IntervalVector> boxes=make_pair(p.first->box,p.second->box);
	delete p.first;
	delete p.second;
	return boxes;
}

} // end namespace ibex
