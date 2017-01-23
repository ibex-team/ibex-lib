/* ============================================================================
 * I B E X - HC4 Revise with Affine arithmetic (forward-backward algorithm)
 * ============================================================================
 * Copyright   : ENSTA Bretagne (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Jordan Ninin
 * Created     : Jan 20, 2017
 * ---------------------------------------------------------------------------- */

#ifndef __IBEX_CTC_AFFINEFWDBWD_H__
#define __IBEX_CTC_AFFINEFWDBWD_H__

#include "ibex_Ctc.h"
#include "ibex_HC4Revise.h"
#include "ibex_AffineEval.h"
#include "ibex_CtcFwdBwd.h"

namespace ibex {

/**
 * \ingroup contractor
 * \brief Forward-backward contractor (HC4Revise).
 *
 */
class CtcAffineFwdBwd: protected CtcFwdBwd {

	/**
	 * \brief Build the contractor for "f(x)=0" or "f(x)<=0".
	 *
	 * \param op: by default: EQ.
	 *
	 */
	CtcAffineFwdBwd(Function& f, CmpOp op=EQ);

	/**
	 * \brief Build the contractor for "f(x) in [y]".
	 */
	CtcAffineFwdBwd(Function& f, const Domain& y);

	/**
	 * \brief Build the contractor for "f(x) in [y]".
	 */
	CtcAffineFwdBwd(Function& f, const Interval& y);

	/**
	 * \brief Build the contractor for "f(x) in [y]".
	 */
	CtcAffineFwdBwd(Function& f, const IntervalVector& y);

	/**
	 * \brief Build the contractor for "f(x) in [y]".
	 */
	CtcAffineFwdBwd(Function& f, const IntervalMatrix& y);

	/**
	 * \remark ctr is not kept by reference.
	 */
	CtcAffineFwdBwd(const NumConstraint& ctr);

	/**
	 * \brief Contract the box.
	 */
	void contract(IntervalVector& box);

protected:

	Affine2Eval _affine_evaluator;
	HC4Revise _hc4revise;
};

} // namespace ibex
#endif // __IBEX_CTC_FWDBWD_H__
