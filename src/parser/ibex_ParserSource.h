//============================================================================
//                                  I B E X                                   
// File        : ibex_ParserSource.h
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Jun 12, 2012
// Last Update : Jun 12, 2012
//============================================================================

#ifndef __IBEX_PARSER_SOURCE_H__
#define __IBEX_PARSER_SOURCE_H__

#include <vector>
#include "ibex_ParserExpr.h"
#include "ibex_ParserNumConstraint.h"
#include "ibex_Function.h"

namespace ibex {

namespace parser {

/**
 * \brief The source.
 */
class P_Source {
public:
	P_Source();

	~P_Source();

	/** The variables */
	std::vector<const P_ExprSymbol*> vars;

	/** The sybs */
	std::vector<const P_ExprSymbol*> sybs;

	/** The exist-parameters */
	std::vector<const P_ExprSymbol*> eprs;

	/** The functions */
	std::vector<Function*> func;

	/** The constraints */
	const std::vector<P_NumConstraint*> ctrs;
};

} // end namespace parser
} // end namespace ibex

#endif // __IBEX_PARSER_SOURCE_H__
