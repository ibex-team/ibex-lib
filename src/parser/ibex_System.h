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
	 * \brief Load a stand-alone system from a string.
	 *
	 * \remark Used by Choco.
	 * \param n    Number of variables.
	 */
	System(int n, const char* syntax);

	/** \brief Delete *this. */
	~System();

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
