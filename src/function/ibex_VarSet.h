//============================================================================
//                                  I B E X                                   
// File        : ibex_VarSet.h
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Sep 1, 2015
//============================================================================

#ifndef __IBEX_VAR_SET_H__
#define __IBEX_VAR_SET_H__

#include "ibex_Function.h"
#include "ibex_BitSet.h"

namespace ibex {

/**
 * \ingroup function
 *
 * \brief Set of Variables
 *
 * Allows to separate "inputs" of a function into two different categories:
 * variable and parameters.
 */
class VarSet {
public:
	/**
	 * \brief Create the contractor for "exists y1 in [y_init] such that c(x,y)".
	 *
	 * \see #CtcQuantif(const NumConstraint&, const Array<const ExprNode&>, const IntervalVector&, double).
	 */
	VarSet(Function& f, const ExprNode& x1);

	/**
	 * \brief Create the contractor for "exists (y1,y2) in [y_init] such that c(x,y)"
	 *
	 * \see #CtcQuantif(const NumConstraint&, const Array<const ExprNode&>, const IntervalVector&, double).
	 */
	VarSet(Function& f, const ExprNode& x1, const ExprNode& x2);

	/**
	 * \brief Create the contractor for "exists (y1,y2,y3) in [y_init] such that c(x,y)"
	 *
	 * \see #CtcQuantif(const NumConstraint&, const Array<const ExprNode&>, const IntervalVector&, double).
	 */
	VarSet(Function& f, const ExprNode& x1, const ExprNode& x2, const ExprNode& x3);

	/**
	 * \brief Create the contractor for "exists (y1,...,y4) in [y_init] such that c(x,y)"
	 *
	 * \see #CtcQuantif(const NumConstraint&, const Array<const ExprNode&>, const IntervalVector&, double).
	 */
	VarSet(Function& f, const ExprNode& x1, const ExprNode& x2, const ExprNode& x3, const ExprNode& x4);

	/**
	 * \brief Create the contractor for "exists (y1,...,y5) in [y_init] such that c(x,y)"
	 *
	 * \see #CtcQuantif(const NumConstraint&, const Array<const ExprNode&>, const IntervalVector&, double).
	 */
	VarSet(Function& f, const ExprNode& x1, const ExprNode& x2, const ExprNode& x3, const ExprNode& x4, const ExprNode& x5);

	/**
	 * \brief Create the contractor for "exists (y1,...,y6) in [y_init] such that c(x,y)"
	 *
	 * \see #CtcQuantif(const NumConstraint&, const Array<const ExprNode&>, const IntervalVector&, double).
	 */
	VarSet(Function& f, const ExprNode& x1, const ExprNode& x2, const ExprNode& x3, const ExprNode& x4, const ExprNode& x5, const ExprNode& x6);

	/**
	 * \brief Create the contractor for "exists (y1,...,y7) in [y_init] such that c(x,y)"
	 *
	 * \see #CtcQuantif(const NumConstraint&, const Array<const ExprNode&>, const IntervalVector&, double).
	 */
	VarSet(Function& f, const ExprNode& x1, const ExprNode& x2, const ExprNode& x3, const ExprNode& x4, const ExprNode& x5, const ExprNode& x6, const ExprNode& x7);

	/**
	 * \brief Create the contractor for "exists (y1,...,y8) in [y_init] such that c(x,y)"
	 *
	 * \see #CtcQuantif(const NumConstraint&, const Array<const ExprNode&>, const IntervalVector&, double).
	 */
	VarSet(Function& f, const ExprNode& x1, const ExprNode& x2, const ExprNode& x3, const ExprNode& x4, const ExprNode& x5, const ExprNode& x6, const ExprNode& x7, const ExprNode& x8);

	/**
	 * \brief Create the contractor for "exists (y1,...,y9) in [y_init] such that c(x,y)"
	 *
	 * \see #CtcQuantif(const NumConstraint&, const Array<const ExprNode&>, const IntervalVector&, double).
	 */
	VarSet(Function& f, const ExprNode& x1, const ExprNode& x2, const ExprNode& x3, const ExprNode& x4, const ExprNode& x5, const ExprNode& x6, const ExprNode& x7, const ExprNode& x8, const ExprNode& x9);

	/**
	 * \brief Create the contractor for "exists (y1,...,y10) in [y_init] such that c(x,y)"
	 *
	 * \see #CtcQuantif(const NumConstraint&, const Array<const ExprNode&>, const IntervalVector&, double).
	 */
	VarSet(Function& f, const ExprNode& x1, const ExprNode& x2, const ExprNode& x3, const ExprNode& x4, const ExprNode& x5, const ExprNode& x6, const ExprNode& x7, const ExprNode& x8, const ExprNode& x9, const ExprNode& x10);

	/**
	 * \brief Create the contractor for "exists (y1,...,y11) in [y_init] such that c(x,y)"
	 *
	 * \see #CtcQuantif(const NumConstraint&, const Array<const ExprNode&>, const IntervalVector&, double).
	 */
	VarSet(Function& f, const ExprNode& x1, const ExprNode& x2, const ExprNode& x3, const ExprNode& x4, const ExprNode& x5, const ExprNode& x6, const ExprNode& x7, const ExprNode& x8, const ExprNode& x9, const ExprNode& x10, const ExprNode& x11);

	/**
	 * \brief Create the contractor for "exists (y1,...,y12) in [y_init] such that c(x,y)"
	 *
	 * \see #CtcQuantif(const NumConstraint&, const Array<const ExprNode&>, const IntervalVector&, double).
	 */
	VarSet(Function& f, const ExprNode& x1, const ExprNode& x2, const ExprNode& x3, const ExprNode& x4, const ExprNode& x5, const ExprNode& x6, const ExprNode& x7, const ExprNode& x8, const ExprNode& x9, const ExprNode& x10, const ExprNode& x11, const ExprNode& x12);

	/**
	 * \brief Create the contractor for "exists y in [y_init] such that c(x,y)"
	 *
	 * \see #CtcQuantif::CtcQuantif(const NumConstraint& c, const Array<const ExprNode>& y);
	 */
	VarSet(Function& f, const Array<const ExprNode>& x);

	/**
	 * \brief
	 *
	 */
	VarSet(Function& f, const BitSet& vars);

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
	 * \brief Number of variables.
	 *
	 * \warning: a "variable" here is a real variable (not a parameter).
	 * Not to be confused with the number of variables of the function f.
	 */
	const int nb_var;

	/**
	 * \brief Number of parameters
	 */
	const int nb_param;

	/**
	 * vars[i]=true <=> the ith component is a variable ("x_k")
	 * Otherwise, the ith component is a parameter ("y_k")
	 */
	BitSet vars;

protected:

	void init(Function& f, const Array<const ExprNode>& x);
};

} // namespace ibex

#endif // __IBEX_VAR_SET_H__
