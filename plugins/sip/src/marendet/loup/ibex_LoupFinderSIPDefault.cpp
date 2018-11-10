//============================================================================
//                                  I B E X                                   
// File        : ibex_LoupFinderDefault.cpp
// Author      : Antoine Marendet, Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : May 4, 2018
// Last Update : May 4, 2018
//============================================================================

#include "ibex_Cell.h"
#include "ibex_LoupFinderSIPDefault.h"
#include "ibex_Vector.h"


using namespace std;

namespace ibex {

LoupFinderSIPDefault::LoupFinderSIPDefault(
		const SIPSystem& system) :
		system_(system) {
}

LoupFinderSIPDefault::~LoupFinderSIPDefault() {
}

std::pair<IntervalVector, double> LoupFinderSIPDefault::find(const IntervalVector& box, const IntervalVector& loup_point, double loup) {
	ibex_warning("LoupFinderSIPDefault: called with no BoxProperties");
	return make_pair(loup_point, loup);
}

std::pair<IntervalVector, double> LoupFinderSIPDefault::find(
		const IntervalVector& box, const IntervalVector& loup_point, double loup, BoxProperties& prop) {
    Vector loup_point_plus_goal(box.mid());
	if(check(system_, loup_point_plus_goal, loup, false, prop)) {
		return std::make_pair(loup_point_plus_goal.subvector(0, box.size()-2), loup);
	}
	throw NotFound();
}

} // end namespace ibex
