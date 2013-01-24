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
#include "ibex_P_Expr.h"
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
	 * Delete the vars, sybs, eprs, ctrs.
	 *
	 * Not the functions (transmitted by reference to P_Result).
	 */
	void cleanup();

	P_Source();

	/** The "entities" */
	std::vector<Entity*> vars;

	/** The functions */
	std::vector<Function*> func;

	/** The goal (NULL if none (pure satisfaction problem)) */
	const ExprNode* goal;

	/** The constraints */
	P_ConstraintList* ctrs;
};

} // end namespace parser
} // end namespace ibex

#endif // __IBEX_PARSER_SOURCE_H__
