//============================================================================
//                                  I B E X                                   
// File        : ibex_PdcDiameterLT.cpp
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Apr 24, 2014
// Last Update : Apr 24, 2014
//============================================================================

#include "ibex_PdcDiameterLT.h"
#include <cassert>

namespace ibex {

PdcDiameterLT::PdcDiameterLT(double ceil) : Pdc(1), ceil(1,ceil) { }

PdcDiameterLT::PdcDiameterLT(const Vector& ceil) : Pdc(ceil.size()), ceil(ceil) { }

PdcDiameterLT::~PdcDiameterLT() { }

BoolInterval PdcDiameterLT::test(const IntervalVector& box) {
	if (ceil.size()==1)  { // same precision for all variables
		for (int i=0; i<box.size(); i++) {
			if (box[i].diam()>ceil[0]) return NO;
		}
	} else {
		assert(ceil.size()==box.size());
		for (int i=0; i<box.size(); i++) {
			if (box[i].diam()>ceil[i]) return NO;
		}
	}
	return YES;
}


} // end namespace ibex

