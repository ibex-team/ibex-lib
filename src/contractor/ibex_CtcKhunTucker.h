//============================================================================
//                                  I B E X
// File        : ibex_CtcKhunTucker.h
// Author      : Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Jul 19, 2017
//============================================================================

#ifndef __IBEX_CTC_KHUN_TUCKER_H__
#define __IBEX_CTC_KHUN_TUCKER_H__

#include "ibex_Ctc.h"
#include "ibex_NormalizedSystem.h"

namespace ibex {

/**
 * \ingroup contractor
 *
 * \brief Contractor based on first-order (KKT) conditions (for NLP)
 */
class CtcKhunTucker : public Ctc {
public:

	/**
	 * \brief Build the contractor for a given NLP problem.
	 *
	 * \warning: building this object is **costly** in both time and memory!
	 *           Don't build this contractor on-the-fly.
	 *
	 * \warning: sys.box should be properly set before calling this constructor.
	 *           In particular, this field **should not change** once this
	 *           contractor is built (the box gives rise to bound constraints
	 *           that are set once for all and not dynamically updated).
	 */
	CtcKhunTucker(const NormalizedSystem& sys);

	/**
	 * \see #Ctc
	 */
	virtual void contract(IntervalVector& box);

	/**
	 * \brief Delete this.
	 */
	virtual ~CtcKhunTucker();

protected:

	/**
	 * \brief The (normalized) NLP problem.
	 */
	const NormalizedSystem& sys;

	/**
	 * \brief Symbolic gradient of the objective.
	 */
	Function* df;

	/**
	 * \brief Symbolic gradient of constraints.
	 */
	Function** dg;
};

} /* namespace ibex */

#endif /* __IBEX_CTC_KHUN_TUCKER_H__ */
