//============================================================================
//                                  I B E X
// File        : ibex_BxpLinearRelaxArgMin.cpp
// Authors     : Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Jul 30, 2018
// Last Update : Aug 01, 2018
//============================================================================

#include "ibex_BxpLinearRelaxArgMin.h"
#include "ibex_NormalizedSystem.h"
#include "ibex_ExtendedSystem.h"

using namespace std;

namespace ibex {

namespace {

/*
 * TODO: the use of System/NormalizedSystem should
 * be simplified to avoid this kind of stuff:
 */
int get_original_nb_var(const System& sys) {

	const ExtendedSystem* ext_sys = dynamic_cast<const ExtendedSystem*>(&sys);
	if (ext_sys) return ext_sys->nb_var-1;
	else return sys.nb_var;
}

}

Map<long,false>& BxpLinearRelaxArgMin::ids() {
	static Map<long,false> _ids;
	return _ids;
}

BxpLinearRelaxArgMin::BxpLinearRelaxArgMin(const System& sys) : Bxp(get_id(sys)), sys(sys), _argmin(get_original_nb_var(sys)), inside(false) {

}

BxpLinearRelaxArgMin* BxpLinearRelaxArgMin::copy(const IntervalVector& box, const BoxProperties& prop) const {
	return new BxpLinearRelaxArgMin(sys);
}

void BxpLinearRelaxArgMin::update(const BoxEvent& event, const BoxProperties& prop) {
	if (inside) {
		int n=_argmin.size();
		// box gotten from extended system
		if (event.box.size() > n) {
			assert(event.box.size()==n+1);
			IntervalVector tmpbox(n);
			((ExtendedSystem&) sys).read_ext_box(event.box,tmpbox);
			if (!tmpbox.contains(_argmin))
				inside=false;
		} else {
			assert(event.box.size()==n);
			if (!event.box.contains(_argmin))
				inside=false;
		}
	}
}

string BxpLinearRelaxArgMin::to_string() const {
	stringstream ss;
	ss << '[' << id << "] BxpLinearRelaxArgmin Sys n°";
	const NormalizedSystem* norm_sys = dynamic_cast<const NormalizedSystem*>(&sys);
	ss << (norm_sys? norm_sys->original_sys_id : sys.id);
	return ss.str();
}

long BxpLinearRelaxArgMin::get_id(const System& sys) {
	const NormalizedSystem* norm_sys = dynamic_cast<const NormalizedSystem*>(&sys);
	long sys_id = norm_sys? norm_sys->original_sys_id : sys.id;
	try {
		return ids()[sys_id];
	} catch(Map<long,false>::NotFound&) {
		long new_id=next_id();
		ids().insert_new(sys_id, new_id);
		return new_id;
	}
}

} /* namespace ibex */
