//============================================================================
//                                  I B E X                                   
// File        : ibex_PdcFwdBwd.cpp
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Nov 27, 2012
// Last Update : Nov 27, 2012
//============================================================================

#include "ibex_PdcFwdBwd.h"
#include "ibex_CtcFwdBwd.h"

namespace ibex {

PdcFwdBwd::PdcFwdBwd(const Function& f, CmpOp op) : PdcCleared(*new CtcFwdBwd(f,!op)){

}

PdcFwdBwd::PdcFwdBwd(const NumConstraint& ctr) : PdcCleared(*new CtcFwdBwd(ctr.f,!ctr.op)) {

}

PdcFwdBwd::~PdcFwdBwd() {
	delete &ctc;
}

} // end namespace ibex
