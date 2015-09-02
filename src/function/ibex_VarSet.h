//============================================================================
//                                  I B E X                                   
// File        : ibex_VarSet.h
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Sep 1, 2015
//============================================================================

#ifndef __IBEX_VARSET_H__
#define __IBEX_VARSET_H__

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
	VarSet(Function& f, const ExprNode& y1, const IntervalVector& y_init);

	/**
	 * \brief Create the contractor for "exists (y1,y2) in [y_init] such that c(x,y)"
	 *
	 * \see #CtcQuantif(const NumConstraint&, const Array<const ExprNode&>, const IntervalVector&, double).
	 */
	VarSet(Function& f, const ExprNode& y1, const ExprNode& y2, const IntervalVector& y_init);

	/**
	 * \brief Create the contractor for "exists (y1,y2,y3) in [y_init] such that c(x,y)"
	 *
	 * \see #CtcQuantif(const NumConstraint&, const Array<const ExprNode&>, const IntervalVector&, double).
	 */
	VarSet(Function& f, const ExprNode& y1, const ExprNode& y2, const ExprNode& y3, const IntervalVector& y_init);

	/**
	 * \brief Create the contractor for "exists (y1,...,y4) in [y_init] such that c(x,y)"
	 *
	 * \see #CtcQuantif(const NumConstraint&, const Array<const ExprNode&>, const IntervalVector&, double).
	 */
	VarSet(Function& f, const ExprNode& y1, const ExprNode& y2, const ExprNode& y3, const ExprNode& y4, const IntervalVector& y_init);

	/**
	 * \brief Create the contractor for "exists (y1,...,y5) in [y_init] such that c(x,y)"
	 *
	 * \see #CtcQuantif(const NumConstraint&, const Array<const ExprNode&>, const IntervalVector&, double).
	 */
	VarSet(Function& f, const ExprNode& y1, const ExprNode& y2, const ExprNode& y3, const ExprNode& y4, const ExprNode& y5, const IntervalVector& y_init);

	/**
	 * \brief Create the contractor for "exists (y1,...,y6) in [y_init] such that c(x,y)"
	 *
	 * \see #CtcQuantif(const NumConstraint&, const Array<const ExprNode&>, const IntervalVector&, double).
	 */
	VarSet(Function& f, const ExprNode& y1, const ExprNode& y2, const ExprNode& y3, const ExprNode& y4, const ExprNode& y5, const ExprNode& y6, const IntervalVector& y_init);

	/**
	 * \brief Create the contractor for "exists (y1,...,y7) in [y_init] such that c(x,y)"
	 *
	 * \see #CtcQuantif(const NumConstraint&, const Array<const ExprNode&>, const IntervalVector&, double).
	 */
	VarSet(Function& f, const ExprNode& y1, const ExprNode& y2, const ExprNode& y3, const ExprNode& y4, const ExprNode& y5, const ExprNode& y6, const ExprNode& y7, const IntervalVector& y_init);

	/**
	 * \brief Create the contractor for "exists (y1,...,y8) in [y_init] such that c(x,y)"
	 *
	 * \see #CtcQuantif(const NumConstraint&, const Array<const ExprNode&>, const IntervalVector&, double).
	 */
	VarSet(Function& f, const ExprNode& y1, const ExprNode& y2, const ExprNode& y3, const ExprNode& y4, const ExprNode& y5, const ExprNode& y6, const ExprNode& y7, const ExprNode& y8, const IntervalVector& y_init);

	/**
	 * \brief Create the contractor for "exists (y1,...,y9) in [y_init] such that c(x,y)"
	 *
	 * \see #CtcQuantif(const NumConstraint&, const Array<const ExprNode&>, const IntervalVector&, double).
	 */
	VarSet(Function& f, const ExprNode& y1, const ExprNode& y2, const ExprNode& y3, const ExprNode& y4, const ExprNode& y5, const ExprNode& y6, const ExprNode& y7, const ExprNode& y8, const ExprNode& y9, const IntervalVector& y_init);

	/**
	 * \brief Create the contractor for "exists (y1,...,y10) in [y_init] such that c(x,y)"
	 *
	 * \see #CtcQuantif(const NumConstraint&, const Array<const ExprNode&>, const IntervalVector&, double).
	 */
	VarSet(Function& f, const ExprNode& y1, const ExprNode& y2, const ExprNode& y3, const ExprNode& y4, const ExprNode& y5, const ExprNode& y6, const ExprNode& y7, const ExprNode& y8, const ExprNode& y9, const ExprNode& y10, const IntervalVector& y_init);

	/**
	 * \brief Create the contractor for "exists (y1,...,y11) in [y_init] such that c(x,y)"
	 *
	 * \see #CtcQuantif(const NumConstraint&, const Array<const ExprNode&>, const IntervalVector&, double).
	 */
	VarSet(Function& f, const ExprNode& y1, const ExprNode& y2, const ExprNode& y3, const ExprNode& y4, const ExprNode& y5, const ExprNode& y6, const ExprNode& y7, const ExprNode& y8, const ExprNode& y9, const ExprNode& y10, const ExprNode& y11, const IntervalVector& y_init);

	/**
	 * \brief Create the contractor for "exists (y1,...,y12) in [y_init] such that c(x,y)"
	 *
	 * \see #CtcQuantif(const NumConstraint&, const Array<const ExprNode&>, const IntervalVector&, double).
	 */
	VarSet(Function& f, const ExprNode& y1, const ExprNode& y2, const ExprNode& y3, const ExprNode& y4, const ExprNode& y5, const ExprNode& y6, const ExprNode& y7, const ExprNode& y8, const ExprNode& y9, const ExprNode& y10, const ExprNode& y11, const ExprNode& y12, const IntervalVector& y_init);

	/**
	 * \brief Create the contractor for "exists y in [y_init] such that c(x,y)"
	 *
	 * \see #CtcQuantif::CtcQuantif(const NumConstraint& c, const Array<const ExprNode>& y, const IntervalVector& y_init);
	 */
	VarSet(Function& f, const Array<const ExprNode>& y, const IntervalVector& y_init);

	/**
	 * \brief
	 *
	 */
	VarSet(Function& f, const BitSet& vars, const IntervalVector& y_init);

	/**
	 * \brief Extend the box to a "full box" with the parameters
	 */
	IntervalVector extend(const IntervalVector& box) const;

	/**
	 * \brief Restrict the "full box" to the sub-box containing variables only
	 */
	IntervalVector chop(const IntervalVector& box) const;

	/**
	 * \brief Initial box of the parameters (can be set dynamically)
	 */
	IntervalVector y_init;

	/**
	 * \brief Number of variables.
	 *
	 * \warning: a "variable" here is a real variable (not a parameter).
	 * Not to be confused with the number of variables of the function f.
	 */
	int nb_var;

	/**
	 * \brief Number of parameters
	 */
	int nb_param;

	/**
	 * vars[i]=true <=> the ith component is a variable ("x_k")
	 * Otherwise, the ith component is a parameter ("y_k")
	 */
	BitSet vars;

protected:


	void init(Function& f, const Array<const ExprNode>& y, const IntervalVector& y_init);
};

} // namespace ibex

#endif // __IBEX_VARSET_H__
