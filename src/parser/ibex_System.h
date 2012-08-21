//============================================================================
//                                  I B E X                                   
// File        : ibex_System.h
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Jun 12, 2012
// Last Update : Jul 16, 2012
//============================================================================

#ifndef __IBEX_SYSTEM_H__
#define __IBEX_SYSTEM_H__

#include <vector>
#include "ibex_NumConstraint.h"

namespace ibex {

/**
 * \ingroup parser
 *
 * \brief System.
 *
 * A system includes variables, domains, auxiliary functions,
 * (in)equations and (optionally) an objective function.
 */
class System {
public:
	/** \brief Load a system from a file. */
	System(const char* filename);

	/**
	 * \brief Load a stand-alone conjunction of constraints
	 * from a string.
	 *
	 * Example: System(2,"{0}+{1}=0;{0}-{1}=0") will create
	 * the system x+y=0 ^ x-y=0.
	 *
	 * \remark Used by Choco.
	 * \param n    Number of variables.
	 */
	System(int n, const char* syntax);

	/** \brief Delete *this. */
	~System();

	/*
	 * \brief Create an uninitialized system (for internal usage).
	 *
	 * \warning: do not confuser nb_var with input_size.
	 *
	 * \remark: used by #ibex::Optimizer to build the
	 * extended system including y=f(x) where f is the objective
	 * function.
	 *
	 * \note Size of #func is 0. Size of #vars and #box is \a nb_var and
	 * size of #ctrs is \a nb_ctr. These arrays have to be initialized.
	 * #goal is set to NULL.
	 */
	System(int nb_ctr, int nb_var);

	/** Number of variables.
	 *
	 * \note This number is also sys.f.input_size() and box.size().
	 * However, in general, it does not match vars.size(), which is
	 * the number of symbols. For instance, in case of a single array
	 * x of 10 components, there is 10 variables but only one symbol.
	 */
	const int nb_var;

	/** Number of constraints. */
	const int nb_ctr;

	/** Auxiliary functions */
	Array<Function> func;

	/** The goal (if any, otherwise NULL) */
	Function* goal;

	/** The main (vector-valued) function */
	Function f;

	/** All the variables */
	Array<const ExprSymbol> vars;

	/** Indices of variables to be considered as constants
	 * (not contracted nor bisected) */
	std::vector<int> sybs;

	/** Indices of variables to be considered as parameters
	 * (contracted but not bisected) */
	std::vector<int> eprs;

	/** Initial Domains
	 * (not only "initial": may be used as input by CHOCO) */
	IntervalVector box;

	/** Constraints */
	Array<NumConstraint> ctrs;

private:
	explicit System(const System& sys); // forbidden

	void load(FILE* file);
};

} // end namespace ibex
#endif // __IBEX_SYSTEM_H__
