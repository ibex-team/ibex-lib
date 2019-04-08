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
	CtcFwdBwd(const Function& f, CmpOp op=EQ);

	/**
	 * \brief Build the contractor for "f(x) in [y]".
	 */
	CtcFwdBwd(const Function& f, const Domain& y);

	/**
	 * \brief Build the contractor for "f(x) in [y]".
	 */
	CtcFwdBwd(const Function& f, const Interval& y);

	/**
	 * \brief Build the contractor for "f(x) in [y]".
	 */
	CtcFwdBwd(const Function& f, const IntervalVector& y);

	/**
	 * \brief Build the contractor for "f(x) in [y]".
	 */
	CtcFwdBwd(const Function& f, const IntervalMatrix& y);

	/**
	 * \remark ctr is kept by reference.
	 */
	CtcFwdBwd(const NumConstraint& ctr);

	/**
	 * \brief Build the contrator for the ith constraint
	 *
	 * Allow to benefit from the system cache associated
	 * to the system.
	 */
	CtcFwdBwd(const System& sys, int i);

	/**
	 * \brief Delete this.
	 */
	~CtcFwdBwd();

	/**
	 * \brief Contract a box.
	 */
	void contract(IntervalVector& box);

	/**
	 * \brief Contract the box.
	 */
	virtual void contract(IntervalVector& box, ContractContext& context);

	/**
	 * \brief Add BxpActiveCtr.
	 */
	virtual void add_property(const IntervalVector& init_box, BoxProperties& map);

	/*
	 * \brief Whether this contractor is idempotent (optional)
	 */
	// TODO
	// bool idempotent();
	//

	/** The constraint. */
	const NumConstraint& ctr;

protected:
	void init();

	/* Right-hand side of the constraint */
	Domain d;

	/* Constraint number (-1 if standalone). */
	int ctr_num;

	/* Identifier of the active property.
	 * Used if the constraint is stand-alone, -1 otherwise.
	 */
	const long active_prop_id;

	/* Identifier of the system cache.
	 * Used if the constraint is from a system, -1 otherwise.
	 */
	const long system_cache_id;

	/* Just to avoid a copy when ctr is given to the constructor. */
	bool own_ctr;
};

} // namespace ibex
#endif // __IBEX_CTC_FWDBWD_H__
