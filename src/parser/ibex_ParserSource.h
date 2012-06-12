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
	vector<const ExprSymbol*> vars;

	/** The sybs */
	vector<const ExprSymbol*> sybs;

	/** The exist-parameters */
	vector<const ExprSymbol*> eprs;

	/** The functions */
	vector<Function*> func;

	/** The constraints */
	const std::vector<P_NumConstraint*> ctrs;
};

} // end namespace parser
} // end namespace ibex

#endif // __IBEX_PARSER_SOURCE_H__
