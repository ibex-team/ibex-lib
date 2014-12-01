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


#include "ibex_Setting.h"

#include <vector>
#include "ibex_NumConstraint.h"


namespace ibex {

namespace parser {
class MainGenerator;
}

class SystemFactory;
class AmplInterface;

/**
 * \defgroup system Systems
 */

/**
 * \ingroup system
 *
 * \brief System.
 *
 * A system includes variables, domains, auxiliary functions,
 * (in)equations and (optionally) an objective function.
 *
 * All theses functions are defined on the same set of variables
 * (the number and the dimension of each component coincide).
 */
class System {
public:

	/**
	 * \brief Build a system with a AMPL model.
	*/
	System(const AmplInterface& ampl);

	/**
	 * \brief Build a system with a factory.
	*/
	System(const SystemFactory& factory);

	/**
	 * \brief Load a system from a file.
	 */
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

	/**
	 * \brief Create a system with a goal function and
	 * a set of constraints (all by copy).
	 *
	 * Basically, checks that the variables coincide between
	 * the objective function and all the constraints.
	 */
	//System(const Function& goal, const Array<NumConstraint>& ctrs);

	/**
	 * \brief Create an uninitialized system (for internal usage).
	 *
	 * \warning: do not confuse nb_var (or input_size) with nb_symbols.
	 *
	 * \remark: used by #ibex::Optimizer to build the
	 * normalized and extended system (including y=f(x))
	 * where f is the objective function.
	 *
	 * \note Size of #func is 0. Size of #vars is 0 (must be resized).
	 * Size of #box is \a nb_var and size of #ctrs is \a nb_ctr. These arrays
	 * have to be initialized. #goal is set to NULL.
	 */

	typedef enum { COPY, INEQ_ONLY, EQ_ONLY } copy_mode;

	/**
	 * \brief Duplicate/Transform the system.
	 *
	 * \param mode   Controls the copy:
	 * <ul>
	 * <li> COPY:      Copy
	 * <li> INEQ_ONLY: Copy the inequalities only. The goal is not copied.
	 * <li> EQ_ONLY:   Copy the equalities only. The goal is not copied.
	 * </ul>
	 *
	 */
	System(const System& sys, copy_mode mode=COPY);

	/**
	 * Merge two systems.
	 */
	System(const System& sys1, const System& sys2);

	/** \brief Delete *this. */
	virtual ~System();

	/** Number of variables.
	 *
	 * \note This number is also sys.f.nb_var() and box.size().
	 * However, in general, it does not match args.size(), which is
	 * the number of arguments (or symbols). For instance, in case of
	 * a single array x of 10 components, there is 10 variables but only one
	 * argument.
	 */
	const int nb_var;

	/** Number of constraints. */
	const int nb_ctr;

	/** Auxiliary functions */
	Array<Function> func;

	/** The goal (if any, otherwise NULL) */
	Function* goal;

	/** The main (vector-valued) function.
	 *
	 * \warning - if this system represents an unconstrained optimization problem,
	 * this field is not initialized and must be ignored. */
	Function f;

	/** All the arguments */
	Array<const ExprSymbol> args;

	/** Indices of arguments to be considered as constants
	 * (not contracted nor bisected) */
	std::vector<int> sybs;

	/** Indices of arguments to be considered as parameters
	 * (contracted but not bisected) */
	std::vector<int> eprs;

	/** Initial Domains
	 * (not only "initial": may be used as input by CHOCO) */
	IntervalVector box;

	/** Constraints.
	 *
	 * If this system represents an unconstrained optimization problem,
	 * this array is zero-sized. */
	Array<NumConstraint> ctrs;

protected:
	/** Uninitialized system */
	System();

	/** Initialize the system with a factory. */
	void init(const SystemFactory&);

private:
	friend class parser::MainGenerator;
	friend class NumConstraint; // NumConstraint requires to build a temporary system for parsing a string

	void load(FILE* file);

	// initialize f from the constraints in ctrs,
	// once *all* the other fields are set (including args and nb_ctr).
	void init_f_from_ctrs();
};

std::ostream& operator<<(std::ostream&, const System&);

} // end namespace ibex
#endif // __IBEX_SYSTEM_H__
