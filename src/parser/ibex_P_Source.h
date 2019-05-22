//============================================================================
//                                  I B E X                                   
// File        : ibex_P_Source.h
// Author      : Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Jun 12, 2012
// Last Update : May 22, 2019
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

	P_Source();

	/**
	 * Delete the variables symbols.
	 *
	 * Not the functions (transmitted by reference to P_Result).
	 */
	void cleanup();

	/** The functions */
	std::vector<Function*> func;

	/** The goal (NULL if none (pure satisfaction problem)) */
	const P_ExprNode* goal;

	/** The constraints (NULL if none (pure optimization problem)) */
	P_ConstraintList* ctrs;
};

std::ostream& operator<<(std::ostream& os, const P_Source& source);

} // end namespace parser
} // end namespace ibex

#endif // __IBEX_PARSER_SOURCE_H__
