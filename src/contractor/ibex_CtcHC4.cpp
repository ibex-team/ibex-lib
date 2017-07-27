//============================================================================
//                                  I B E X                                   
// File        : HC4 Contractor
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Apr 22, 2012
// Last Update : Apr 22, 2012
//============================================================================

#include "ibex_CtcHC4.h"
#include "ibex_CtcFwdBwd.h"
#include "ibex_ExtendedSystem.h"

namespace ibex {

namespace {

Array<Ctc> convert(const System& sys) {
	const ExtendedSystem* ext_sys=dynamic_cast<const ExtendedSystem*>(&sys);

	std::vector<Ctc*> vec;
	for (int i=0; i<sys.ctrs.size(); i++) {
		if (ext_sys && i==ext_sys->goal_ctr()) {
			// we replace y>=f(x) by y=f(x).
			vec.push_back(new CtcFwdBwd(sys.ctrs[i].f,EQ));
		} else
			vec.push_back(new CtcFwdBwd(sys.ctrs[i]));
	}
	return vec;
}

Array<Ctc> convert(const Array<NumConstraint>& csp) {
	std::vector<Ctc*> vec;
	for (int i=0; i<csp.size(); i++) {
		vec.push_back(new CtcFwdBwd(csp[i]));
	}
	return vec;
}
}

CtcHC4::CtcHC4(const Array<NumConstraint>& csp, double ratio, bool incremental) :
		CtcPropag(convert(csp), ratio, incremental) {
}

CtcHC4::CtcHC4(const System& sys, double ratio, bool incremental) :
				CtcPropag(convert(sys), ratio, incremental) {

}

CtcHC4::~CtcHC4() {
	for (int i=0; i<list.size(); i++)
		delete &list[i];
}

} // end namespace ibex
