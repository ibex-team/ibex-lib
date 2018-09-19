//============================================================================
//                                  I B E X                                   
// File        : ibex_SystemFactory.h
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Aug 27, 2012
// Last Update : May 26, 2013
//============================================================================

#ifndef __IBEX_SYSTEM_FACTORY_H__
#define __IBEX_SYSTEM_FACTORY_H__

#include "ibex_System.h"
#include "ibex_ExprCopy.h"

namespace ibex {

/**
 * \ingroup system
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

	virtual ~SystemFactory();

	/**
	 * \brief Add a new variable.
	 *
	 * 	The reference to this variable is kept
	 * 	until the system is built.
	 */
	void add_var(const ExprSymbol& v);
	void add_var(const ExprSymbol& v, const Interval& init_box);
	void add_var(const ExprSymbol& v, const IntervalVector& init_box);

	/**
	 * \brief Add all variables.
	 *
	 *  References to the variables are kept
	 * 	until the system is built.
	 */
	void add_var(const Array<const ExprSymbol>& a);
	void add_var(const Array<const ExprSymbol>& a, const IntervalVector& box);
	//void add_var(const Array<const ExprSymbol>& a, const Array<const IntervalVector>& init_boxes);

	/**
	 * \brief Add a goal function (by copy)
	 *
	 * \pre All the variables must have been added.
	 * \warning for convenience, goal is cleaned up (the expression
	 * does not exist anymore after this function call).
	 */
	void add_goal(const ExprNode& goal);

	/**
	 * \brief Add a goal function (by copy).
	 */
	void add_goal(const Function& goal);

	/**
	 * \brief Add a new constraint (by copy).
	 *
	 * \pre All the variables must have been added.
	 *
	 * \warning the ctr expression must no be deleted until the final system has been built
	 * (the factory keeps track of nodes for building DAGS).
	 */
	void add_ctr(const ExprCtr& ctr);

	/**
	 * \brief Add a new constraint (by copy).
	 *
	 * \pre All the variables must have been added.
	 *
	 * \warning the ctr expression must no be deleted until the final system has been built
	 * (the factory keeps track of nodes for building DAGS).
	 */
	void add_ctr(const NumConstraint& ctr);

	/**
	 * \brief Add a new equality constraint (by copy).
	 *
	 * \pre All the variables must have been added.
	 */
	void add_ctr_eq (const ExprNode& exp);

protected:
	friend class System;

	// total number of arguments
	int nb_arg;
	// total number of variables
	int nb_var;

	// Domains of arguments
	std::vector<IntervalVector> boxes;
	// temporary arguments (until last call to add_var)
	std::vector<const ExprSymbol*> tmp_input_args;
	// arguments of the input expressions
	Array<const ExprSymbol> input_args;

	// arguments of the output system
	Array<const ExprSymbol> sys_args;
	// gaol function of the output system
	Function* goal;
	// constraints of the output system
	std::vector<NumConstraint*> ctrs;
	// expression of the global function sys.f_ctrs
	ExprCopy f_ctrs_copy;
	std::vector<const ExprNode*> f_ctrs;

	mutable bool system_built; // for cleanup

private:

	void init_args();
};


inline void SystemFactory::add_ctr_eq (const ExprNode& exp) {
	return add_ctr(ExprCtr(exp,EQ));
}


} // end namespace ibex
#endif // __IBEX_SYSTEM_FACTORY_H__
