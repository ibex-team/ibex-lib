//============================================================================
//                                  I B E X                                   
// File        : ibex_VarSet.h
// Author      : Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Sep 1, 2015
//============================================================================

#ifndef __IBEX_VAR_SET_H__
#define __IBEX_VAR_SET_H__

#include "ibex_BitSet.h"
#include "ibex_Expr.h"

namespace ibex {

class Function;
class ExprNode;

/**
 * \ingroup function
 *
 * \brief Set of Variables
 *
 * Allows to separate "inputs" of a function into two different categories:
 * variable and parameters (that is, symbols considered as interval constants by algorithms).
 */
class VarSet {
public:
	/**
	 * \brief Create the set of variables (or parameters) {x1}
	 * \param var indicates whether x1 is a variable (and other symbols parameters)
	 *            (case var==true) or the contrary.
	 */
	VarSet(const Function& f, const ExprNode& x1, bool var=true);

	/**
	 * \brief Create the set of variables (or parameters) {x1,x2}
	 *
	 */
	VarSet(const Function& f, const ExprNode& x1, const ExprNode& x2, bool var=true);

	/**
	 * \brief Create the set of variables (or parameters) {x1,x2,x3}
	 *
	 */
	VarSet(const Function& f, const ExprNode& x1, const ExprNode& x2, const ExprNode& x3, bool var=true);

	/**
	 * \brief Create the set of variables (or parameters) {x1,...,x4}
	 *
	 */
	VarSet(const Function& f, const ExprNode& x1, const ExprNode& x2, const ExprNode& x3, const ExprNode& x4, bool var=true);

	/**
	 * \brief Create the set of variables (or parameters) {x1,...,x5}
	 *
	 */
	VarSet(const Function& f, const ExprNode& x1, const ExprNode& x2, const ExprNode& x3, const ExprNode& x4, const ExprNode& x5, bool var=true);

	/**
	 * \brief Create the set of variables (or parameters) {x1,...,x6}
	 *
	 */
	VarSet(const Function& f, const ExprNode& x1, const ExprNode& x2, const ExprNode& x3, const ExprNode& x4, const ExprNode& x5, const ExprNode& x6, bool var=true);

	/**
	 * \brief Create the set of variables (or parameters) {x1,...,x7}
	 *
	 */
	VarSet(const Function& f, const ExprNode& x1, const ExprNode& x2, const ExprNode& x3, const ExprNode& x4, const ExprNode& x5, const ExprNode& x6, const ExprNode& x7, bool var=true);

	/**
	 * \brief Create the set of variables (or parameters) {x1,...,x8}
	 *
	 */
	VarSet(const Function& f, const ExprNode& x1, const ExprNode& x2, const ExprNode& x3, const ExprNode& x4, const ExprNode& x5, const ExprNode& x6, const ExprNode& x7, const ExprNode& x8, bool var=true);

	/**
	 * \brief Create the set of variables (or parameters) {x1,...,x9}
	 *
	 */
	VarSet(const Function& f, const ExprNode& x1, const ExprNode& x2, const ExprNode& x3, const ExprNode& x4, const ExprNode& x5, const ExprNode& x6, const ExprNode& x7, const ExprNode& x8, const ExprNode& x9, bool var=true);

	/**
	 * \brief Create the set of variables (or parameters) {x1,...,x10}
	 *
	 */
	VarSet(const Function& f, const ExprNode& x1, const ExprNode& x2, const ExprNode& x3, const ExprNode& x4, const ExprNode& x5, const ExprNode& x6, const ExprNode& x7, const ExprNode& x8, const ExprNode& x9, const ExprNode& x10, bool var=true);

	/**
	 * \brief Create the set of variables (or parameters) {x1,...,x11}
	 *
	 */
	VarSet(const Function& f, const ExprNode& x1, const ExprNode& x2, const ExprNode& x3, const ExprNode& x4, const ExprNode& x5, const ExprNode& x6, const ExprNode& x7, const ExprNode& x8, const ExprNode& x9, const ExprNode& x10, const ExprNode& x11, bool var=true);

	/**
	 * \brief Create the set of variables (or parameters) {x1,...,x12}
	 *
	 */
	VarSet(const Function& f, const ExprNode& x1, const ExprNode& x2, const ExprNode& x3, const ExprNode& x4, const ExprNode& x5, const ExprNode& x6, const ExprNode& x7, const ExprNode& x8, const ExprNode& x9, const ExprNode& x10, const ExprNode& x11, const ExprNode& x12, bool var=true);

	/**
	 * \brief Create the set of variables (or parameters) x[0],...
	 */
	VarSet(const Function& f, const Array<const ExprNode>& x, bool var=true);

	/**
	 * \brief Create the set of variables (or parameters) x[0],...
	 */
	VarSet(const Function& f, const Array<const ExprSymbol>& x, bool var=true);

	/**
	 * \brief Create a set of variables (or parameters) from a bitset
	 *
	 * \param total - The number of variables and parameters (sum of the number
	 *                of variable and parameter components (not the number of symbols)).
	 *
	 * x[i]==true <=> the ith component is a variable (if var is "true") or a parameter (if var is "false")
	 */
	VarSet(int total, const BitSet& x, bool var=true);

	/**
	 * \brief Copy constructor.
	 */
	VarSet(const VarSet& v);

	/**
	 * \brief Delete this.
	 */
	~VarSet();

	/**
	 * \brief True if the bitsets are the same.
	 *
	 * The original function is not take into account
	 */
	bool operator==(const VarSet& v) const;

	/**
	 * \brief True if the bitsets differ.
	 *
	 * The original function is not take into account
	 */
	bool operator!=(const VarSet& v) const;

	/**
	 * \brief Assignment (erases everything)
	 */
	VarSet& operator=(const VarSet& v);

	/**
	 * \brief Extend the box to a "full box" with the parameters
	 */
	IntervalVector full_box(const IntervalVector& var_box, const IntervalVector& param_box) const;

	/**
	 * \brief Extract the domain of variables from the "full box"
	 */
	IntervalVector var_box(const IntervalVector& full_box) const;

	/**
	 * \brief Set the domain of variables in a "full box"
	 */
	void set_var_box(IntervalVector& full_box, const IntervalVector& var_box) const;

	/**
	 * \brief Set the domain of parameters in a "full box"
	 */
	void set_param_box(IntervalVector& full_box, const IntervalVector& param_box) const;

	/**
	 * \brief Extract the domain of parameters from the "full box"
	 */
	IntervalVector param_box(const IntervalVector& full_box) const;

	/**
	 * \brief Retun the ith variable
	 *
	 * Return the index of the ith variable in all the dimensions
	 * of the "full box".
	 *
	 * \pre 0<=i<nb_var
	 */
	int var(int i) const;

	/**
	 * \brief Retun the ith parameter
	 *
	 * Return the index of the ith parameter in all the dimensions
	 * of the "full box".
	 *
	 * \pre 0<=i<nb_param
	 */
	int param(int i) const;

	/**
	 * \brief Number of variables.
	 *
	 * \warning: a "variable" here is a real variable (not a parameter).
	 * Not to be confused with the original number of variables (of the function f).
	 */
	const int nb_var;

	/**
	 * \brief Number of parameters
	 */
	const int nb_param;

	/**
	 * is_var[i]==true <=> the ith component is a variable
	 * Otherwise, the ith component is a parameter
	 */
	const BitSet is_var;

protected:

	// variables
	int* vars;

	// parameters
	int* params;

	// Init is_var
	void init_bitset(const Function& f, const Array<const ExprNode>& x, bool var);

	// Init vars & params
	// (to be called after init_bitset)
	void init_arrays();
};

std::ostream& operator<<(std::ostream& os, const VarSet& v);


/*================================== inline implementations ========================================*/

inline int VarSet::var(int i) const {
	assert(i>=0 && i<nb_var);
	return vars[i];
}

inline int VarSet::param(int i) const {
	assert(i>=0 && i<nb_param);
	return params[i];
}

inline bool VarSet::operator==(const VarSet& v) const {
	return ((BitSet&) is_var) == v.is_var;
}

inline bool VarSet::operator!=(const VarSet& v) const {
	return !(*this==v);
}


} // namespace ibex

#endif // __IBEX_VAR_SET_H__
