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

namespace ibex {
namespace parser {

void P_Source::cleanup() {

	for (vector<P_NumConstraint*>::const_iterator it=ctrs.begin(); it!=ctrs.end(); it++) {
		delete *it; // will not delete symbols (shared by all the constraints)
	}

	for (vector<const P_ExprSymbol*>::const_iterator it=vars.begin(); it!=vars.end(); it++) delete *it;
	for (vector<const P_ExprSymbol*>::const_iterator it=eprs.begin(); it!=eprs.end(); it++) delete *it;
	for (vector<const P_ExprSymbol*>::const_iterator it=sybs.begin(); it!=sybs.end(); it++) delete *it;
}

} // end namespace parser
} // end namespace ibex
