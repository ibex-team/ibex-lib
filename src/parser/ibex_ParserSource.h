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
#include "ibex_Expr.h"
#include "ibex_P_NumConstraint.h"
#include "ibex_Function.h"

namespace ibex {

namespace parser {

/**
 * \brief The source.
 */
class P_Source {
public:
	/**
	 * Delete the variables symbols.
	 *
	 * Not the functions (transmitted by reference to P_Result).
	 */
	void cleanup();

	P_Source();

	/** Variable symbols */
//	std::vector<const ExprSymbol*> vars;

//	std::vector<const char*> vars;
//
//	/** Constant symbols */
//	std::vector<const char*> cst;

	/** The functions */
	std::vector<Function*> func;

	/** The goal (NULL if none (pure satisfaction problem)) */
	const P_ExprNode* goal;

	/** The constraints */
	P_ConstraintList* ctrs;
};

} // end namespace parser
} // end namespace ibex

#endif // __IBEX_PARSER_SOURCE_H__
