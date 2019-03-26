//============================================================================
//                                  I B E X
// File        : ibex_LoupFinderDefault.cpp
// Author      : Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Jul 09, 2017
// Last update : Aug 01, 2018
//============================================================================

#include "ibex_LoupFinderDefault.h"
#include "ibex_LoupFinderInHC4.h"
#include "ibex_LoupFinderFwdBwd.h"
#include "ibex_BxpLinearRelaxArgMin.h"
#include "ibex_LoupFinderProbing.h"

using namespace std;

namespace ibex {

LoupFinderDefault::LoupFinderDefault(const System& sys, bool inHC4) :
	finder_probing(inHC4? (LoupFinder&) *new LoupFinderInHC4(sys) : (LoupFinder&) *new LoupFinderFwdBwd(sys)),
	finder_x_taylor(sys) {

}

void LoupFinderDefault::add_property(const IntervalVector& init_box, BoxProperties& prop) {
	finder_probing.add_property(init_box,prop);
	finder_x_taylor.add_property(init_box,prop);

	//--------------------------------------------------------------------------
	/* Using line search from LP relaxation minimizer seems not interesting. */
//	if (!prop[BxpLinearRelaxArgMin::get_id(finder_x_taylor.sys)]) {
//		prop.add(new BxpLinearRelaxArgMin(finder_x_taylor.sys));
//	}
	//--------------------------------------------------------------------------

}

std::pair<IntervalVector, double> LoupFinderDefault::find(const IntervalVector& box, const IntervalVector& old_loup_point, double old_loup, BoxProperties& prop) {

	pair<IntervalVector,double> p=make_pair(old_loup_point, old_loup);

	bool found=false;

	try {
		p=finder_probing.find(box,p.first,p.second,prop);
		found=true;
	} catch(NotFound&) { }

	try {
		// TODO
		// in_x_taylor.set_inactive_ctr(entailed->norm_entailed);
		p=finder_x_taylor.find(box,p.first,p.second,prop);
		found=true;
	} catch(NotFound&) { }

	if (found) {
		//--------------------------------------------------------------------------
		/* Using line search from LP relaxation minimizer seems not interesting. */
		//	BxpLinearRelaxArgMin* argmin=(BxpLinearRelaxArgMin*) prop[BxpLinearRelaxArgMin::get_id(finder_x_taylor.sys)];
		BxpLinearRelaxArgMin* argmin=NULL;
		//--------------------------------------------------------------------------

		if (argmin && argmin->argmin()) {
			Vector loup_point = p.first.lb();
			double loup = p.second;
			bool new_loup_found=LoupFinderProbing(finder_x_taylor.sys).dichotomic_line_search(loup_point, loup, *argmin->argmin(), false);
			//cout << "better loup found! " << loup << endl;
			p=make_pair(loup_point,loup);
		}
		return p;
	} else
		throw NotFound();
}

LoupFinderDefault::~LoupFinderDefault() {
	delete &finder_probing;
}

} /* namespace ibex */
