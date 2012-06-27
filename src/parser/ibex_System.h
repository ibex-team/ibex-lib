//============================================================================
//                                  I B E X                                   
// File        : ibex_System.h
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Jun 12, 2012
// Last Update : Jun 12, 2012
//============================================================================

#ifndef __IBEX_SYSTEM_H__
#define __IBEX_SYSTEM_H__

#include <vector>
#include "ibex_NumConstraint.h"

namespace ibex {

class System {
public:
	System(const char* filename);

	/** Auxiliary functions */
	Array<Function> func;

	/** Main function */
	Function f;

	/** All the variables */
	Array<const ExprSymbol> vars;

	/** Indices of sybs */
	std::vector<int> sybs;

	/** Indices of eprs */
	std::vector<int> eprs;

	/** Domains */
	IntervalVector box;

	/** Constraints */
	Array<NumConstraint> ctrs;
};

} // end namespace ibex
#endif // __IBEX_SYSTEM_H__
