//============================================================================
//                                  I B E X                                   
// File        : ibex_LoupFinderSIP.cpp
// Author      : Antoine Marendet, Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : May 4, 2018
// Last Update : May 4, 2018
//============================================================================

#include "ibex_LoupFinderSIP.h"

using namespace std;

namespace ibex {

LoupFinderSIP::LoupFinderSIP(const SIPSystem& system) : system_(system) {

}

LoupFinderSIP::~LoupFinderSIP() {

}

void LoupFinderSIP::add_property(const IntervalVector& init_box, BoxProperties& prop) {
	if(prop[BxpNodeData::id] == nullptr) {
        prop.add(new BxpNodeData(system_.getInitialNodeCaches()));
    }
}

bool LoupFinderSIP::check(const SIPSystem& sys, const Vector& pt, double& loup, bool _is_inner, BoxProperties& prop) {

	// "res" will contain an upper bound of the criterion
	Vector pt_without_goal(pt.size()-1);
	pt_without_goal.put(0, pt.subvector(0, pt.size()-2));
	double res = sys.goal_ub(pt_without_goal);

	// check if f(x) is below the "loup" (the current upper bound).
	//
	// The "loup" and the corresponding "loup_point" (the current minimizer)
	// will be updated if the constraints are satisfied.
	// The test of the constraints is done only when the evaluation of the criterion
	// is better than the loup (a cheaper test).

	//        cout << " res " <<  res << " loup " <<  pseudo_loup <<  " is_inner " << _is_inner << endl;
	//cout << res << " " << loup << endl;

	BxpNodeData* node_data = (BxpNodeData*) prop[BxpNodeData::id];
	if(node_data == nullptr) {
		ibex_error("LoupFinderSIP::check: BxpNodeData must be set");
	}

	if (res<loup) {
		if (_is_inner || sys.is_inner(pt, *node_data)) {
			loup = res;
			return true;
		}
	}

	return false;
}
} // end namespace ibex
