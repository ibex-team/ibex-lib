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

namespace parser {
class MainGenerator;
}

class SystemFactory;

/**
 * \ingroup parser
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

	typedef enum { COPY, NORMALIZE, EXTEND } copy_mode;

	/**
	 * \brief Duplicate/Transform the system.
	 *
	 * The \a mode field controls the copy:
	 * <ul>
	 * <li> COPY:      copy
	 * <li> NORMALIZE: copy and make all inequalities under the form f_i(x)<=0.
	 * <li> EXTEND:    normalize and encode the goal function as a constraint.
	 * The resulting system includes (n+1) variables, the n original variables x_1,...,x_n and
	 * an an additional "goal" variable y. The goal variable is added at the end (after the others).
	 * It also includes (m+1) constraints. The first constraint is y-goal(x)=0 where goal(x) is the
	 * goal function. The others constraints are normalized copy of the original ones.
	 * The name of the goal variable is #goal_name.
	 * </ul>
	 */
	System(const System& sys, copy_mode mode=COPY);

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

	/** Name of the goal variable ("y"). */
	static const char* goal_name;

private:
	friend class parser::MainGenerator;
	//friend class SystemFactory;

	void load(FILE* file);

	// initialize f from the constraints in ctrs,
	// once *all* the other fields are set (including vars and nb_ctr).
	void init_f_from_ctrs();
};

std::ostream& operator<<(std::ostream&, const System&);

} // end namespace ibex
#endif // __IBEX_SYSTEM_H__
