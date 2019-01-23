//============================================================================
//                                  I B E X
// File        : ibex_BxpActiveCtrs.cpp
// Author      : Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Jul 25, 2018
//============================================================================

#include "ibex_BxpActiveCtrs.h"
#include "ibex_Id.h"

using namespace std;

namespace ibex {

Map<long,false>& BxpActiveCtrs::ids() {
	static Map<long,false> _ids;
	return _ids;
}

BxpActiveCtrs::BxpActiveCtrs(const System& sys) :
		Bxp(get_id(sys)), sys(sys), active(sys.nb_ctr), ineq(sys.nb_ctr) {

	for (int i=0; i<sys.nb_ctr; i++) {
		active.add(i); // all set by default
		if (sys.ctrs[i].op!=EQ) {
			ineq.add(i);
			long id=BxpActiveCtr::get_id(sys.ctrs[i]);
			dependencies.push_back(id);
		}
	}
}

BxpActiveCtrs* BxpActiveCtrs::copy(const IntervalVector& box, const BoxProperties& prop) const {
	return new BxpActiveCtrs(sys);
}

void BxpActiveCtrs::check(BoxProperties& prop) {
	for (vector<long>::iterator it=dependencies.begin(); it!=dependencies.end(); ++it) {
		((BxpActiveCtr*) prop[*it])->check();
	}
	_update(prop);
}

void BxpActiveCtrs::update(const BoxEvent& e, const BoxProperties& prop) {
	_update(prop);
}

void BxpActiveCtrs::_update(const BoxProperties& prop) {
	if (ineq.empty()) return;

	BitSet::const_iterator c=ineq.begin(); // constraint number

	for (vector<long>::const_iterator it=dependencies.begin(); it!=dependencies.end(); ++it) {
		const BxpActiveCtr* p=(const BxpActiveCtr*) prop[*it];
		if (!p) {
			stringstream ss;
			ss << "[BxpActiveCtrs] missing property n°" << *it;
			ibex_error(ss.str().c_str());
		}
		if (p->active())
			active.add(c);
		else
			active.remove(c);
		++c;
	}
}

long BxpActiveCtrs::get_id(const System& sys) {
	try {
		return ids()[sys.id];
	} catch(Map<long,false>::NotFound&) {
		long new_id=next_id();
		ids().insert_new(sys.id, new_id);
		return new_id;
	}
}

std::string BxpActiveCtrs::to_string() const {
	stringstream ss;
	ss << '[' << id << "] BxpActiveCtrs System n°" << sys.id;
	ss << " --> ";
	for (vector<long>::const_iterator it=dependencies.begin(); it!=dependencies.end(); ++it) {
		ss << *it << ' ';
	}
	return ss.str();
}
} // end namespace ibex
