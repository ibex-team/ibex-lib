/* ============================================================================
 * I B E X - ibex_LoupFinderSIP.cpp
 * ============================================================================
 * Copyright   : IMT Atlantique (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Antoine Marendet
 * Created     : Nov 12, 2018
 * ---------------------------------------------------------------------------- */
 
#include "ibex_LoupFinderSIP.h"
#include "ibex_utils.h"

using namespace std;

namespace ibex {

LoupFinderSIP::LoupFinderSIP(const SIPSystem& system) : system_(system) {

}

LoupFinderSIP::~LoupFinderSIP() {

}

void LoupFinderSIP::add_property(const IntervalVector& init_box, BoxProperties& prop) {
	if(prop[BxpNodeData::id] == nullptr) {
        prop.add(new BxpNodeData(system_.getInitialNodeCaches()));
        ((BxpNodeData*)prop[BxpNodeData::id])->init_box = init_box;
    }
}

bool LoupFinderSIP::check(const SIPSystem& sys, const Vector& pt, double& loup, bool _is_inner, BoxProperties& prop) {

	// "res" will contain an upper bound of the criterion
	double res = sys.goal_ub(pt);

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
	Vector ext_pt = sip_to_ext_box(pt, res);
	if (res<loup) {
		if (_is_inner || sys.is_inner(ext_pt, *node_data)) {
			loup = res;
			return true;
		}
	}

	return false;
}
} // end namespace ibex

