//============================================================================
//                                  I B E X
// File        : ibex_LoupFinderDefault.cpp
// Author      : Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Jul 09, 2017
//============================================================================

#include "ibex_LoupFinderDefault.h"
#include "ibex_LoupFinderInHC4.h"
#include "ibex_LoupFinderFwdBwd.h"

using namespace std;

namespace ibex {

LoupFinderDefault::LoupFinderDefault(const System& sys, bool inHC4) :
	finder_probing(inHC4? (LoupFinder&) *new LoupFinderInHC4(sys) : (LoupFinder&) *new LoupFinderFwdBwd(sys)),
	finder_x_taylor(sys) {

}

std::pair<IntervalVector, double> LoupFinderDefault::find(const IntervalVector& box, const IntervalVector& old_loup_point, double old_loup) {

	pair<IntervalVector,double> p=make_pair(old_loup_point, old_loup);

	bool found=false;

	try {
		p=finder_probing.find(box,p.first,p.second);
		found=true;
	} catch(NotFound&) { }

	try {
		// TODO
		// in_x_taylor.set_inactive_ctr(entailed->norm_entailed);
		p=finder_x_taylor.find(box,p.first,p.second);
		found=true;
	} catch(NotFound&) { }

	if (found)
		return p;
	else
		throw NotFound();
}

LoupFinderDefault::~LoupFinderDefault() {
	delete &finder_probing;
}

} /* namespace ibex */
