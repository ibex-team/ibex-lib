//============================================================================
//                                  I B E X                                   
// File        : ibex_PdcProj.cpp
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Nov 27, 2012
// Last Update : Nov 27, 2012
//============================================================================

#include "ibex_PdcProj.h"
#include "ibex_CtcFwdBwd.h"

namespace ibex {

namespace {

}

PdcProj::PdcProj(Function& f, CmpOp op) : PdcCleared(*new CtcFwdBwd(f,!op)){

}

PdcProj::PdcProj(const NumConstraint& ctr) : PdcCleared(*new CtcFwdBwd(ctr.f,!ctr.op)) {

}

PdcProj::~PdcProj() {
	delete &ctc;
}

} // end namespace ibex
