//============================================================================
//                                  I B E X                                   
// File        : ibex_StcEqual.cpp
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Jul 15, 2014
//============================================================================

#include "ibex_StcEqual.h"
#include <stack>

using namespace std;

namespace ibex {

StcEqual::StcEqual(Ctc& c_in, Ctc& c_out, double eps) : c_in(c_in), c_out(c_out), eps(eps) {

}

void StcEqual::contract(Set& set) {
	set.contract(c_in, c_out, eps);
}

StcEqual::~StcEqual() {
	// TODO Auto-generated destructor stub
}

} // namespace ibex
