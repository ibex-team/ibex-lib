/* ============================================================================
 * I B E X - Newton-based Parametric proofs
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes and CNRS
 *
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert, Alexandre Goldsztejn
 * Created     : Sep 06, 2016
 * ---------------------------------------------------------------------------- */

#ifndef __IBEX_PARAMETRIC_PROOF_H__
#define __IBEX_PARAMETRIC_PROOF_H__

#include "ibex_Function.h"
#include "ibex_Ctc.h"
#include "ibex_VarSet.h"

namespace ibex {

/* Precision  of the parametric Newton contraction
 * (when apply on a box [p]x[x]) */
#define NEWTON_CTC_PREC 1e-10

/* Precision  of the parametric Newton certificate
 * (when apply on a box {p}x[x], p being instantiated) */
#define NEWTON_CERTIF_PREC 1e-12

#define FIND_SOLUTION_MAX_ITER 1000

/**
 * \brief A contractor for solving an under-constrained system f(x)=0
 *
 * Contraction is based on parametric Newton.
 *
 * The parameters are fixed statically using the "vars" argument.
 */
class CtcParamNewton : public Ctc {
public:
	CtcParamNewton(Function& f, const VarSet& vars);

	/**
	 * \brief Contract the box.
	 *
	 * The box is the "full" box, containing both domains of parameters and variables.
	 */
	void contract(IntervalVector& box);

	Function& f;
	const VarSet& vars;
};

/**
 * \brief Thrown by find_solution
 *
 * Thrown when find_solution could neither find a certified
 * solution (after NEWTON_MAX_FAIL tries) nor contract the
 * box to the empty set.
 */

class FindSolutionFail : public Exception { };

/*
 * Find a certified solution of an under-constrained system f(x)=0
 * in a box called "facet" using a parametric Newton.
 *
 * The parameters are fixed in the "vars" parameters.
 *
 * \return an enclosure of the solution if found or an empty box otherwise.
 */
IntervalVector find_solution(Function& f, IntervalVector& facet, const VarSet& vars);

/**
 * True if the constraint g(x)<=0 inside a box is homeomorph to a half ball.
 *
 * Proves that g(x) takes all possible combination of negative and positive signs
 * inside param_box.
 * The function can be implicit and satisfies g(x)\in ginf + [Dg]
 * where ginf=g(param_box.lb())
 */
bool is_homeomorph_half_ball(const IntervalVector& ginf, const IntervalMatrix& Dg, const IntervalVector& param_box);

} // end namespace ibex

#endif /* __IBEX_PARAMETRIC_PROOF_H__ */
