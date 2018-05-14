//============================================================================
//                                  I B E X                                   
// File        : ibex_LoupFinderDefault.cpp
// Author      : Antoine Marendet, Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : May 4, 2018
// Last Update : May 4, 2018
//============================================================================

#include "ibex_LoupFinderDefault.h"

#include "ibex_Cell.h"
#include "ibex_Vector.h"


using namespace std;

namespace ibex {

LoupFinderDefault::LoupFinderDefault(
		const SIPSystem& system) :
		system_(system) {
}

LoupFinderDefault::~LoupFinderDefault() {
}

std::pair<IntervalVector, double> LoupFinderDefault::find(
		const Cell& cell, const IntervalVector& loup_point,
		double loup) {
    Vector loup_point_plus_goal(cell.box.mid());
	if(check(system_, loup_point_plus_goal, loup, false)) {
		return std::make_pair(loup_point_plus_goal.subvector(0, cell.box.size()-2), loup);
	}
	throw NotFound();
}

} // end namespace ibex
