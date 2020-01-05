//============================================================================
//                                  I B E X
// File        : ibex_CtcKuhnTucker.h
// Author      : Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Jul 19, 2017
// Created     : Jul 11, 2019
//============================================================================

#ifndef __IBEX_CTC_KUHN_TUCKER_H__
#define __IBEX_CTC_KUHN_TUCKER_H__

#include "ibex_Ctc.h"
#include "ibex_NormalizedSystem.h"
#include "ibex_FncKuhnTucker.h"

namespace ibex {

/**
 * \ingroup contractor
 *
 * \brief Contractor based on first-order (KKT) conditions (for NLP)
 */
class CtcKuhnTucker : public Ctc {
public:

	/**
	 * \brief Build the contractor for a given NLP problem.
	 *
	 * The contractor takes a normalized system in argument but expects
	 * an "extended" box in the "contract" function (in order to be uniform with
	 * all other contractors in optimization).
	 *
	 * \warning: building this object is **costly** in both time and memory!
	 *           (symbolic derivation of all constraints, because Ibex does not
	 *           have automatic hessian computation).
	 *           Don't build this contractor on-the-fly.
	 *
	 * \warning: sys.box should be properly set before calling this constructor.
	 *           In particular, this field **should not change** once this
	 *           contractor is built (the box gives rise to bound constraints
	 *           that are set once for all and not dynamically updated).
	 *
	 * \param reject_unbounded: If true, the contractor does nothing when called
	 *        on an unbounded box. This is because minima points at infinity may
	 *        be lost by contraction (they do not satisfy KKT conditions). Set to
	 *        'true' by default.
	 */
	CtcKuhnTucker(const NormalizedSystem& sys, bool reject_unbounded=true);

	/**
	 * \see Contract function (see #Ctc).
	 *
	 * \param box: an "extended" box (in order to be uniform with
	 *             all other contractors in optimization).
	 *
	 */
	virtual void contract(IntervalVector& box);

	/**
	 * \brief Delete this.
	 */
	virtual ~CtcKuhnTucker();

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

	/**
	 * \brief Whether unbounded boxes are rejected.
	 *
	 * See constructor.
	 */
	bool reject_unbounded;
};

} /* namespace ibex */

#endif /* __IBEX_CTC_KUHN_TUCKER_H__ */
