/* ============================================================================
 * I B E X - Propagation of contractors
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Feb 27, 2012
 * ---------------------------------------------------------------------------- */

#ifndef __IBEX_CTC_PROPAG_H__
#define __IBEX_CTC_PROPAG_H__

#include "ibex_Agenda.h"
#include "ibex_Ctc.h"
#include "ibex_DirectedHyperGraph.h"
#include "ibex_Array.h"

namespace ibex {

/**
 * \ingroup contractor
 *
 * \brief Propagation contractor.
 *
 * This class is an implementation of the classical interval variant of the AC3 constraint propagation
 * algorithm.
 *
 */
class CtcPropag : public Ctc {
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
	CtcPropag(const Array<Ctc>& cl, double ratio=default_ratio, bool incr=false);

	/**
	 * \brief Contract a box.
	 */
	void contract(IntervalVector& box);

	/**
	 * \brief Enforces propagation (e.g.: HC4 or BOX) fitering.
	 *
	 * Call #contract(IntervalVector&, const BitSet&) with the mask
	 * set to all the variables.
	 * If #incremental is true, the propagation will start from the
	 * impacted variables only (instead of from all the variables).
	 *
	 * \see #contract(IntervalVector&, const BitSet&).
	 * \throw #ibex::EmptyBoxException - if inconsistency is detected.
	 */
	virtual void contract(IntervalVector& box, ContractContext& context);

	/**
	 * \brief Add sub-contractors properties to the map
	 */
	virtual void add_property(const IntervalVector& init_box, BoxProperties& map);

	/** The list of contractors to propagate */
	Array<Ctc> list;

	/** Ratio (see \link CtcPropag(const Array<Ctc>&, double, bool) constructor \endlink for details). */
	const double ratio;

	/** Agenda initialization mode (see \link CtcPropag(const Array<Ctc>&, double, bool) constructor \endlink for details).*/
	const bool incremental;

	/** Accumulate residual contractions? */
	bool accumulate;

	/** Default ratio used by propagation, set to 0.1. */
	static constexpr double default_ratio = 0.01;

protected:

	DirectedHyperGraph g; // constraint network (hypergraph)

	Agenda agenda;        // propagation agenda

	BitSet active;      // mark active sub-contractors


};

} // namespace ibex
#endif // __IBEX_CTC_PROPAG_H__
