//============================================================================
//                                  I B E X                                   
// File        : ibex_P_SysGenerator.h
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Jun 19, 2012
// Last Update : Jun 19, 2012
//============================================================================

#ifndef __IBEX_PARSER_SYSTEM_GENERATOR_H__
#define __IBEX_PARSER_SYSTEM_GENERATOR_H__

#include "ibex_P_Struct.h"
#include "ibex_System.h"

namespace ibex {

namespace parser {

class P_SysGenerator {
public:
	P_SysGenerator(P_Scope& scopes);

	void generate(P_Source& source, System& result, int simpl_level);

private:
	// to cleanup all expressions (that are copied by the factory)
	// we store them in a garbage. Note: they shared variables,
	// **symbolic constants**, and all expressions represented by
	// temporary symbols, whence this structure.
	void add_garbage(NodeMap<bool>& garbage, const ExprNode& e);

	P_Scope& scopes;
};

} // end namespace parser
} // end namespace ibex

#endif // __IBEX_PARSER_SYSTEM_GENERATOR_H__
