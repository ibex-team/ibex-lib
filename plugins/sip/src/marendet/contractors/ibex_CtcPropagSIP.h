//============================================================================
//                                  I B E X                                   
// File        : ibex_CtcPropagSIP.h
// Author      : Antoine Marendet, Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : May 4, 2018
// Last Update : May 4, 2018
//============================================================================

#ifndef __SIP_IBEX_CTCPROPAGSIP_H__
#define __SIP_IBEX_CTCPROPAGSIP_H__

#include "ibex_Agenda.h"
#include "ibex_Array.h"
#include "ibex_BitSet.h"
#include "ibex_Ctc.h"
#include "ibex_DirectedHyperGraph.h"
#include "ibex_IntervalVector.h"

namespace ibex {
/**
 * \ingroup contractor
 *
 * \brief Propagation contractor.
 *
 * This class is an implementation of the classical interval variant of the AC3 constraint propagation
 * algorithm, adapted to SIConstraint.
 *
 */
class CtcPropagSIP : public Ctc {
public:

	/**
	 * \brief Create a AC3-like propagation with a list of contractors.
	 *
	 * \param nb_var           - Number of variables contractors work with
	 * \param cl               - The list of contractors.
	 * \param ratio (optional) - Criterion for stopping propagation. If a projection does not remove
	 *                           more that \a ratio times the diameter of a variable domain, then this
	 *                           reduction is not propagated. The default value is #default_ratio.
	 * \param incr (optional)  - Whether the propagation works incrementally. This parameter is
	 *                           only used when contraction is called with an "impact" bool mask.
	 *
	 * \see #contract(IntervalVector&, const BitSet&).
	 */
	CtcPropagSIP(const Array<Ctc>& cl, double ratio=default_ratio, bool incr=false);

	/**
	 * \brief Enforces propagation (e.g.: HC4 or BOX) fitering.
	 *
	 * Call #contract(IntervalVector&, const BitSet&) with the mask
	 * set to all the variables.
	 * If #incremental is true, the propagation will start from the
	 * impacted variables only (instead of from all the variables).
	 *
	 * \see #contract(IntervalVector&, const BitSet&).
	 * \throw #EmptyBoxException - if inconsistency is detected.
	 */
	virtual void contract(IntervalVector& box);

	/** The list of contractors to propagate */
	Array<Ctc> list;

	/** Ratio (see \link CtcPropag(const Array<Ctc>&, double, bool) constructor \endlink for details). */
	const double ratio;

	/** Agenda initialization mode (see \link CtcPropag(const Array<Ctc>&, double, bool) constructor \endlink for details).*/
	const bool incremental;

	/** Accumulate residual contractions? */
	bool accumulate;

	/** Default ratio used by propagation, set to 0.1. */
	static const double default_ratio;

protected:


	DirectedHyperGraph g; // constraint network (hypergraph)

	Agenda agenda;        // propagation agenda

	BitSet _impact;     // impact given to sub-contractors

	BitSet flags;       // status of a contraction

	BitSet active;      // mark active sub-contractors


};

} // end namespace ibex

#endif // __SIP_IBEX_CTCPROPAGSIP_H__
