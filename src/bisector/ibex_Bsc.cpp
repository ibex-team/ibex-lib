//============================================================================
//                                  I B E X                                   
// File        : ibex_Bsc.cpp
// Author      : Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : May 8, 2012
// Last Update : Dec 25, 2017
//============================================================================

#include "ibex_Bsc.h"
#include "ibex_Exception.h"
#include "ibex_Id.h"
#include "ibex_BisectedVar.h"

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

void Bsc::add_property(Map<SearchNodeProp>& map) {
	if (!map.found(BisectedVar::prop_id))
		map.insert_new(BisectedVar::prop_id,new BisectedVar());
}

pair<IntervalVector,IntervalVector> Bsc::bisect(const IntervalVector& box) {
	Cell cell(box);
	add_property(cell.prop);
	pair<Cell*,Cell*> p=bisect(cell);
	pair<IntervalVector,IntervalVector> boxes=make_pair(p.first->box,p.second->box);
	delete p.first;
	delete p.second;
	return boxes;
}

const long BisectedVar::prop_id = next_id();

} // end namespace ibex
