/* ============================================================================
 * I B E X - HC4 Revise (forward-backward algorithm)
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert, Jordan Ninin
 * Created     : Feb 27, 2012
 * ---------------------------------------------------------------------------- */

#ifndef __IBEX_CTC_FWDBWD_H__
#define __IBEX_CTC_FWDBWD_H__

#include "ibex_Ctc.h"
#include "ibex_NumConstraint.h"

namespace ibex {

/**
 * \ingroup contractor
 * \brief Forward-backward contractor (HC4Revise).
 *
 */
class CtcFwdBwd: public Ctc {

public:
	/**
	 * \brief Build the contractor for "f(x)=0" or "f(x)<=0".
	 *
	 * \param op: by default: EQ.
	 *
	 */
	CtcFwdBwd(Function& f, CmpOp op=EQ);

	/**
	 * \brief Build the contractor for "f(x) in [y]".
	 */
	CtcFwdBwd(Function& f, const Domain& y);

	/**
	 * \brief Build the contractor for "f(x) in [y]".
	 */
	CtcFwdBwd(Function& f, const Interval& y);

	/**
	 * \brief Build the contractor for "f(x) in [y]".
	 */
	CtcFwdBwd(Function& f, const IntervalVector& y);

	/**
	 * \brief Build the contractor for "f(x) in [y]".
	 */
	CtcFwdBwd(Function& f, const IntervalMatrix& y);

	/**
	 * \remark ctr is not kept by reference.
	 */
	CtcFwdBwd(const NumConstraint& ctr);

	/**
	 * \brief Delete this.
	 */
	~CtcFwdBwd();

	/**
	 * \brief Contract the box.
	 */
	virtual void contract(IntervalVector& box);

	/*
	 * \brief Whether this contractor is idempotent (optional)
	 */
	// TODO
	// bool idempotent();
	//

	/** The function "f". */
	const Function& f;

	/** The domain "y". */
	Domain d;

protected:
	void init();
};

} // namespace ibex
#endif // __IBEX_CTC_FWDBWD_H__
