/* ============================================================================
 * I B E X - ibex_LoupFinderSIPDefault.cpp
 * ============================================================================
 * Copyright   : IMT Atlantique (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Antoine Marendet
 * Created     : Nov 12, 2018
 * ---------------------------------------------------------------------------- */
 
#include "ibex_Cell.h"
#include "ibex_LoupFinderSIPDefault.h"
#include "ibex_Vector.h"


using namespace std;

namespace ibex {

LoupFinderSIPDefault::LoupFinderSIPDefault(
		const SIPSystem& system) :
		LoupFinderSIP(system) {
}

LoupFinderSIPDefault::~LoupFinderSIPDefault() {
}

std::pair<IntervalVector, double> LoupFinderSIPDefault::find(const IntervalVector& box, const IntervalVector& loup_point, double loup) {
	ibex_warning("LoupFinderSIPDefault: called with no BoxProperties");
	return make_pair(loup_point, loup);
}

std::pair<IntervalVector, double> LoupFinderSIPDefault::find(
		const IntervalVector& box, const IntervalVector& loup_point, double loup, BoxProperties& prop) {
	if(check(system_, box.mid(), loup, false, prop)) {
		return std::make_pair(box.mid(), loup);
	}
	throw NotFound();
}

} // end namespace ibex

