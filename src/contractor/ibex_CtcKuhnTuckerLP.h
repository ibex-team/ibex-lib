//============================================================================
//                                  I B E X
// File        : ibex_CtcKuhnTuckerLP.h
// Author      : Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Jul 31, 2019
//============================================================================

#ifndef __IBEX_CTC_KUHN_TUCKER_LP_H__
#define __IBEX_CTC_KUHN_TUCKER_LP_H__

#include "ibex_Ctc.h"
#include "ibex_NormalizedSystem.h"
#include "ibex_KuhnTuckerSystem.h"
#include "ibex_CtcPolytopeHull.h"

namespace ibex {

/**
 * \ingroup contractor
 *
 * \brief Contractor based on first-order (KKT) conditions (for NLP)
 *
 * This variant resorts to linear programming (CtcPolytopeHull) instead of
 * Newton (as in CtcKuhnTucker). This avoids in particular pessimism due to
 * preconditioning.
 *
 */
class CtcKuhnTuckerLP : public Ctc {
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
	CtcKuhnTuckerLP(const NormalizedSystem& sys, bool reject_unbounded=true);

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
	virtual ~CtcKuhnTuckerLP();

protected:

	/**
	 * \brief The KKT conditions
	 */
	const KuhnTuckerSystem* kkt;

	CtcPolytopeHull* ph;

	/**
	 * \brief Whether unbounded boxes are rejected.
	 *
	 * See constructor.
	 */
	bool reject_unbounded;

	Linearizer* _lr; // kept for cleanup only
};

} /* namespace ibex */

#endif /* __IBEX_CTC_KUHN_TUCKER_LP_H__ */
