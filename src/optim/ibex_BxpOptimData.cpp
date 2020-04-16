//============================================================================
//                                  I B E X                                   
// File        : ibex_BxpOptimData.cpp
// Author      : Jordan Ninin, Gilles Chabert
// License     : See the LICENSE file
// Created     : Oct 18, 2014
// Last Update : Jul 05, 2018
//============================================================================

#include "ibex_BxpOptimData.h"
#include "ibex_Id.h"

namespace ibex {

Map<long,false>& BxpOptimData::ids() {
	static Map<long,false> _ids;
	return _ids;
}

BxpOptimData::BxpOptimData(const ExtendedSystem& sys) : Bxp(get_id(sys)), sys(sys), pf(), pu(0) {

}

BxpOptimData::BxpOptimData(const BxpOptimData& e) : Bxp(e.id), sys(e.sys), pf(e.pf), pu(e.pu) {

}

BxpOptimData::~BxpOptimData() {

}

long BxpOptimData::get_id(const ExtendedSystem& sys) {
	try {
		return ids()[sys.id];
	} catch(Map<long,false>::NotFound&) {
		long new_id=next_id();
		ids().insert_new(sys.id, new_id);
		return new_id;
	}
}

void BxpOptimData::compute_pf(const Function& goal, const IntervalVector& box) {
	pf=goal.eval(box);
}


void BxpOptimData::compute_pu(const ExtendedSystem& sys, const IntervalVector& box) {
	double pu=1;

	if (sys.active_ctrs(box).empty()) return;

	IntervalVector e=sys.active_ctrs_eval(box);

	for (int j=1; j<e.size();j++) {
		double pui=1;
		if (e[j].diam()>0)
			pui= -e[j].lb()/e[j].diam();
		pu=pu*pui;
	}
}

} // end namespace ibex
