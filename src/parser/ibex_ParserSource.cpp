//============================================================================
//                                  I B E X                                   
// File        : ibex_ParserSource.cpp
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Jun 12, 2012
// Last Update : Jun 12, 2012
//============================================================================

#include "ibex_ParserSource.h"
#include "ibex_ExprTools.h"

namespace ibex {
namespace parser {

void P_Source::cleanup() {
	delete ctrs;

	for (vector<Entity*>::iterator it=vars.begin(); it!=vars.end(); it++) {
		delete (ExprSymbol*) &((*it)->symbol);
		delete *it;
	}
}

} // end namespace parser
} // end namespace ibex
