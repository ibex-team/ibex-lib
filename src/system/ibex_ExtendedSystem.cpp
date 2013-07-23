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

const char* ExtendedSystem::goal_name = "__goal__";

ExtendedSystem::ExtendedSystem(const System& sys) /*: original_goal(*sys.goal)*/ {
	init(SystemExtend(sys));
}

} // end namespace ibex
