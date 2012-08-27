//============================================================================
//                                  I B E X                                   
// File        : ibex_SystemFactory.h
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Aug 27, 2012
// Last Update : Aug 27, 2012
//============================================================================

#ifndef __IBEX_SYSTEM_FACTORY_H__
#define __IBEX_SYSTEM_FACTORY_H__

#include "ibex_System.h"

namespace ibex {

/**
 * \ingroup parser
 * \brief System factory.
 *
 * All the build a system step-by-step.
 */
class SystemFactory {
public:

	/**
	 * \brief Build a system factory.
	 */
	SystemFactory();

	/**
	 * \brief Add a new variable.
	 */
	void add_var(Variable& v);

	/**
	 * \brief Add a goal function.
	 *
	 * \pre All the variables must have been added.
	 * \note A reference to the argument is kept until
	 * the system is built.
	 */
	void add_goal(const ExprNode& goal);

	/**
	 * \brief Add a new constraint.
	 *
	 * \pre All the variables must have been added.
	 * \note A reference to the argument is kept until
	 * the system is built.
	 */
	void add_ctr(const ExprCtr& ctr);

protected:
	friend class System;
	vector<const ExprSymbol*> vars;
	const ExprNode* goal;
	vector<const ExprNode*> exprs;
	vector<CmpOp> ops;
};

} // end namespace ibex
#endif // __IBEX_SYSTEM_FACTORY_H__
