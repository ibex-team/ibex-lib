//============================================================================
//                                  I B E X                                   
// File        : ibex_ExtendedSystem.cpp
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Jun 26, 2013
// Last Update : Jun 26, 2013
//============================================================================

#include "ibex_ExtendedSystem.h"
#include "ibex_SystemExtend.cpp_"

namespace ibex {

const char* ExtendedSystem::goal_name() {
	static const char* _goal_name = "__goal__"; // construct-on-first-use idiom (safe: no de-initialization dependency between static objects)
	return _goal_name;
}

ExtendedSystem::ExtendedSystem(const System& sys, double eps) /*: original_goal(*sys.goal)*/ {
	init(SystemExtend(sys,eps));
}

} // end namespace ibex
