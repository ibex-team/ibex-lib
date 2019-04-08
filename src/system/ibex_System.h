//============================================================================
//                                  I B E X                                   
// File        : ibex_System.h
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Jun 12, 2012
// Last Update : Apr 08, 2019
//============================================================================

#ifndef __IBEX_SYSTEM_H__
#define __IBEX_SYSTEM_H__

#include "ibex_Setting.h"
#include "ibex_NumConstraint.h"

#include <vector>

namespace ibex {

namespace parser {
class MainGenerator;
}

class SystemFactory;

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

	/**
	 * \brief The upper bound of the goal.
	 *
	 * \note Return +oo if the box is outside definition domain
	 */
	double goal_ub(const Vector& x) const;

	/**
	 * \brief Interval evaluation of the goal.
	 */
	Interval goal_eval(const IntervalVector& box) const;

	/**
	 * \brief Interval gradient of the goal.
	 */
	IntervalVector goal_gradient(const IntervalVector& box) const;

	/**
	 * \brief Interval gradient of the goal.
	 *
	 * See comments above.
	 */
	void goal_gradient(const IntervalVector& box, IntervalVector& g) const;

	/**
	 * \brief Interval evaluation of the constraint functions.
	 *
	 * E.g. if there are two constraints x<=0 and y+x>=0, the
	 * function returns ([x],[y]+[x]).
	 * Usually, this function is called either on a system containing
	 * only equalities or a normalized system (see NormalizedSystem).
	 */

	IntervalVector ctrs_eval(const IntervalVector& box) const;

	/**
	 * \brief Interval evaluation of the constraint functions.
	 *
	 * See comments above.
	 */
	void ctrs_eval(const IntervalVector& box, IntervalVector& ev) const;

	/**
	 * \brief Get the jacobian matrix of the constraints.
	 */
	IntervalMatrix ctrs_jacobian(const IntervalVector& box) const;

	/**
	 * \brief Get the jacobian matrix of the constraints.
	 *
	 * See comments above.
	 */
	void ctrs_jacobian(const IntervalVector& box, IntervalMatrix& J) const;

	/**
	 * \brief Get the (potentially) active constraints.
	 *
	 * A constraint which is not in the bitset is proven to be inactive.
	 *
	 * \warning A "constraint" here corresponds to a component of "f_ctrs"
	 *          **not** a constraint in the "ctrs" array (this is different
	 *          in the case of vector/matrix constraints).
	 */
	BitSet active_ctrs(const IntervalVector& box) const;

	/**
	 * \brief Quick check that the box is inside g(x)<=0.
	 *
	 * \return True only if all the constraints are inactive
	 */
	bool is_inner(const IntervalVector& box) const;

	/**
	 * \brief Interval evaluation of the active constraints.
	 *
	 * \pre The number of (potentially) active constraints must be >0
	 */
	IntervalVector active_ctrs_eval(const IntervalVector& box) const;

	/**
	 * \brief Interval jacobian matrix of the active constraints.
	 *
	 * * \pre The number of (potentially) active constraints must be >0
	 */
	IntervalMatrix active_ctrs_jacobian(const IntervalVector& box) const;

	/**
	 * \brief All the variable names in a string vector.
	 */
	std::vector<std::string> var_names() const;

	/**
	 * \brief Serialize the system (get the Minibex code)
	 *
	 * \param human: if true, numeric constant are converted to character
	 *               in decimal format to be human-readable, but this is an
	 *               unsafe conversion. Value by default is "true".
	 *
	 *               If false, all constants are converted to their exact
	 *               hexadecimal representation, whence a safe serialization.
	 */
	std::string minibex(bool human=true) const;

	/**
	 * \brief Identifying number.
	 */
	const long id;

	/** Number of variables.
	 *
	 * \note This number is also sys.f_ctrs.nb_var() and box.size().
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
	Function f_ctrs;

	/**
	 * The array of comparison operators:
	 *
	 *         f_ctrs[i] (x) ops[i] 0.
	 *
	 * Set to NULL if there is no constraint.
	 */
	CmpOp *ops;

	/** All the arguments */
	Array<const ExprSymbol> args;

	/** Box Constraint on the variable
	 * (not only "initial": may be used as input by CHOCO) */
	IntervalVector box;

	/**
	 * Constraints.
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

	// initialize f and ops from the constraints in ctrs,
	// once *all* the other fields are set (including args and nb_ctr).
	void init_f_ctrs(const std::vector<const ExprNode*>& fac_f_ctrs);
};

std::ostream& operator<<(std::ostream&, const System&);

inline double System::goal_ub(const Vector& x) const {
	Interval fx=goal->eval(x);
	if (fx.is_empty())  // means: outside of the definition domain of the function
		return POS_INFINITY;
	else
		return fx.ub();

}

} // end namespace ibex

#endif // __IBEX_SYSTEM_H__
