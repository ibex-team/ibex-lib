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

#include "ibex_HC4Revise.h"
#include "ibex_AffineEval.h"
#include "ibex_CtcFwdBwd.h"

namespace ibex {

/**
 * \ingroup contractor
 * \brief Forward-backward contractor (HC4Revise).
 *
 */
class CtcAffineFwdBwd: public CtcFwdBwd {


	/**
	 * \brief Delete this.
	 */
	virtual ~CtcAffineFwdBwd();

	/**
	 * \brief Contract the box.
	 */
	virtual void contract(IntervalVector& box);

protected:
	virtual void init();

	Affine2Eval * _affine_evaluator;
	HC4Revise * _hc4revise;
};

} // namespace ibex
#endif // __IBEX_CTC_FWDBWD_H__
