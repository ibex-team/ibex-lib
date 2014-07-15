//============================================================================
//                                  I B E X                                   
// File        : ibex_CtcExist.h
// Author      : Jordan Ninin, Gilles Chabert
// License     : See the LICENSE file
// Created     : Jan 29, 2014
// Last Update : May 7, 2014
//============================================================================

#ifndef __IBEX_CTC_EXIST_H__
#define __IBEX_CTC_EXIST_H__

#include "ibex_CtcFwdBwd.h"
#include "ibex_Ctc.h"
#include "ibex_LargestFirst.h"

namespace ibex {


/**
 * \ingroup contractor
 *
 * \brief Projection-union operator (for existentially-quantified constraints)
 *
 * This operator allows to contract a box [x] with respect to:
 *
 *    exists y in[y] |  c(x,y).
 *
 * where y is a vector of "parameters".
 */
class CtcExist : public Ctc {
public:

	/**
	 * \brief Create the contractor for "exists y1 in [y_init] such that c(x,y)".
	 *
	 * \see #CtcExist(const NumConstraint&, const Array<const ExprSymbol&>, const IntervalVector&, double).
	 */
	CtcExist(const NumConstraint& c, const ExprSymbol& y1, const IntervalVector& y_init, double prec);

	/**
	 * \brief Create the contractor for "exists (y1,y2) in [y_init] such that c(x,y)"
	 *
	 * \see #CtcExist(const NumConstraint&, const Array<const ExprSymbol&>, const IntervalVector&, double).
	 */
	CtcExist(const NumConstraint& c, const ExprSymbol& y1, const ExprSymbol& y2, const IntervalVector& y_init, double prec);

	/**
	 * \brief Create the contractor for "exists (y1,y2,y3) in [y_init] such that c(x,y)"
	 *
	 * \see #CtcExist(const NumConstraint&, const Array<const ExprSymbol&>, const IntervalVector&, double).
	 */
	CtcExist(const NumConstraint& c, const ExprSymbol& y1, const ExprSymbol& y2, const ExprSymbol& y3, const IntervalVector& y_init, double prec);

	/**
	 * \brief Create the contractor for "exists (y1,...,y4) in [y_init] such that c(x,y)"
	 *
	 * \see #CtcExist(const NumConstraint&, const Array<const ExprSymbol&>, const IntervalVector&, double).
	 */
	CtcExist(const NumConstraint& c, const ExprSymbol& y1, const ExprSymbol& y2, const ExprSymbol& y3, const ExprSymbol& y4, const IntervalVector& y_init, double prec);

	/**
	 * \brief Create the contractor for "exists (y1,...,y5) in [y_init] such that c(x,y)"
	 *
	 * \see #CtcExist(const NumConstraint&, const Array<const ExprSymbol&>, const IntervalVector&, double).
	 */
	CtcExist(const NumConstraint& c, const ExprSymbol& y1, const ExprSymbol& y2, const ExprSymbol& y3, const ExprSymbol& y4, const ExprSymbol& y5, const IntervalVector& y_init, double prec);

	/**
	 * \brief Create the contractor for "exists (y1,...,y6) in [y_init] such that c(x,y)"
	 *
	 * \see #CtcExist(const NumConstraint&, const Array<const ExprSymbol&>, const IntervalVector&, double).
	 */
	CtcExist(const NumConstraint& c, const ExprSymbol& y1, const ExprSymbol& y2, const ExprSymbol& y3, const ExprSymbol& y4, const ExprSymbol& y5, const ExprSymbol& y6, const IntervalVector& y_init, double prec);

	/**
	 * \brief Create the contractor for "exists (y1,...,y7) in [y_init] such that c(x,y)"
	 *
	 * \see #CtcExist(const NumConstraint&, const Array<const ExprSymbol&>, const IntervalVector&, double).
	 */
	CtcExist(const NumConstraint& c, const ExprSymbol& y1, const ExprSymbol& y2, const ExprSymbol& y3, const ExprSymbol& y4, const ExprSymbol& y5, const ExprSymbol& y6, const ExprSymbol& y7, const IntervalVector& y_init, double prec);

	/**
	 * \brief Create the contractor for "exists (y1,...,y8) in [y_init] such that c(x,y)"
	 *
	 * \see #CtcExist(const NumConstraint&, const Array<const ExprSymbol&>, const IntervalVector&, double).
	 */
	CtcExist(const NumConstraint& c, const ExprSymbol& y1, const ExprSymbol& y2, const ExprSymbol& y3, const ExprSymbol& y4, const ExprSymbol& y5, const ExprSymbol& y6, const ExprSymbol& y7, const ExprSymbol& y8, const IntervalVector& y_init, double prec);

	/**
	 * \brief Create the contractor for "exists (y1,...,y9) in [y_init] such that c(x,y)"
	 *
	 * \see #CtcExist(const NumConstraint&, const Array<const ExprSymbol&>, const IntervalVector&, double).
	 */
	CtcExist(const NumConstraint& c, const ExprSymbol& y1, const ExprSymbol& y2, const ExprSymbol& y3, const ExprSymbol& y4, const ExprSymbol& y5, const ExprSymbol& y6, const ExprSymbol& y7, const ExprSymbol& y8, const ExprSymbol& y9, const IntervalVector& y_init, double prec);

	/**
	 * \brief Create the contractor for "exists (y1,...,y10) in [y_init] such that c(x,y)"
	 *
	 * \see #CtcExist(const NumConstraint&, const Array<const ExprSymbol&>, const IntervalVector&, double).
	 */
	CtcExist(const NumConstraint& c, const ExprSymbol& y1, const ExprSymbol& y2, const ExprSymbol& y3, const ExprSymbol& y4, const ExprSymbol& y5, const ExprSymbol& y6, const ExprSymbol& y7, const ExprSymbol& y8, const ExprSymbol& y9, const ExprSymbol& y10, const IntervalVector& y_init, double prec);

	/**
	 * \brief Create the contractor for "exists (y1,...,y11) in [y_init] such that c(x,y)"
	 *
	 * \see #CtcExist(const NumConstraint&, const Array<const ExprSymbol&>, const IntervalVector&, double).
	 */
	CtcExist(const NumConstraint& c, const ExprSymbol& y1, const ExprSymbol& y2, const ExprSymbol& y3, const ExprSymbol& y4, const ExprSymbol& y5, const ExprSymbol& y6, const ExprSymbol& y7, const ExprSymbol& y8, const ExprSymbol& y9, const ExprSymbol& y10, const ExprSymbol& y11, const IntervalVector& y_init, double prec);

	/**
	 * \brief Create the contractor for "exists (y1,...,y12) in [y_init] such that c(x,y)"
	 *
	 * \see #CtcExist(const NumConstraint&, const Array<const ExprSymbol&>, const IntervalVector&, double).
	 */
	CtcExist(const NumConstraint& c, const ExprSymbol& y1, const ExprSymbol& y2, const ExprSymbol& y3, const ExprSymbol& y4, const ExprSymbol& y5, const ExprSymbol& y6, const ExprSymbol& y7, const ExprSymbol& y8, const ExprSymbol& y9, const ExprSymbol& y10, const ExprSymbol& y11, const ExprSymbol& y12, const IntervalVector& y_init, double prec);

	/**
	 * \brief Create the contractor for "exists y in [y_init] such that c(x,y)"
	 *
	 * The constraint c must take n+m arguments where n is the size of x and m the size of y.
	 * Contrary to what the notation c(x,y) suggests, symbols y1...ym does not necessarily
	 * need to appear after the symbols x1...xn in c.
	 *
	 * \param c       The constraint
	 * \param y       The parameters
	 * \param y_init  Initial box for the parameters
	 * \param prec    Bisection precision on the parameters (the contraction involves a
	 *                bisection process on y)
	 */
	CtcExist(const NumConstraint& c, const Array<const ExprSymbol>& y, const IntervalVector& y_init, double prec);

	/**
	 * \brief Proj-union operator applied on the contractor c.
	 *
	 * This is the generalization of the quantified constraint proposed in "Contractor programming".
	 * The constraint is implicitly represented by a contractor.
	 *
	 * \param c       The contractor
	 * \param vars    Indicates whether the ith component handled by the contractor c
	 *                is a variable (vars[i]==true)or a parameter (vars[i]==false).
	 * \param y_init  Initial box for the parameters
	 * \param prec    Bisection precision on the parameters
	 * \param own_ctc Boolean which indicate if the ctc mast be destroy with this
	 */
	CtcExist(Ctc& c, const BoolMask& vars, const IntervalVector& y_init, double prec, bool own_ctc=false);

	/**
	 * \brief Delete this.
	 */
	~CtcExist();

	/**
	 * \brief Contract a box.
	 */
	void contract(IntervalVector& x);

	/**
	 * \brief Initial box of the parameters (can be set dynamically)
	 */
	IntervalVector y_init;

private:

	/**
	 * \brief Initialization for contractors with symbols y
	 */
	void init(const NumConstraint& ctr, const Array<const ExprSymbol>& y, const IntervalVector& y_init, double prec);

	/**
	 * \brief Contract the "full" box (x,y)
	 *
	 * \param x the vector of variables
	 * \param y the vector parameters.
	 */
	void contract(IntervalVector& x, IntervalVector& y);

	/**
	 * \brief Number of parameters
	 */
	int nb_param;

	/**
	 * \brief The Contractor.
	 */
	Ctc* ctc;

	/**
	 * \brief a bisector
	 */
	LargestFirst* bsc;

	/**
	 * vars[i]=true <=> the ith component is a variable ("x_k")
	 * Otherwise, the ith component is a parameter ("y_k")
	 */
	BoolMask vars;

	/**
	 * \brief precision
	 */
	double prec;

	/* Information for cleanup only */
	bool _own_ctc;
};

typedef  CtcExist CtcProjUnion;

} // end namespace ibex
#endif // __IBEX_CTC_EXIST_H__
