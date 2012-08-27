//============================================================================
//                                  I B E X                                   
// File        : ibex_DefaultOptimizer.cpp
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Aug 27, 2012
// Last Update : Aug 27, 2012
//============================================================================

#include "ibex_DefaultOptimizer.h"
#include "ibex_RoundRobin.h"
#include "ibex_CtcHC4.h"

namespace ibex {

static System* tmp;

DefaultOptimizer::DefaultOptimizer(System& sys) : Optimizer(sys,*new RoundRobin(),*new CtcHC4((tmp=new System(sys,System::EXTEND))->ctrs)),
		__ctc(&ctc), __bsc(&bsc), __sys(tmp) {

}

DefaultOptimizer::~DefaultOptimizer() {
	delete __ctc;
	delete __bsc;
	delete __sys;
}

} // end namespace ibex
