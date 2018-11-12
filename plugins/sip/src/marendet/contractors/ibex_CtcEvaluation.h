/* ============================================================================
 * I B E X - ibex_CtcEvaluation.h
 * ============================================================================
 * Copyright   : IMT Atlantique (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Antoine Marendet, Gilles Chabert
 * Created     : May 4, 2018
 * ---------------------------------------------------------------------------- */
 
#ifndef __SIP_IBEX_CTCEVALUATION_H__
#define __SIP_IBEX_CTCEVALUATION_H__

#include "ibex_SIPSystem.h"

#include "ibex_Ctc.h"

namespace ibex {
/**
 * \brief EvaluationCtc.
 *
 * This CellCtc set box to empty iff any constraint evalution lb() for the box is >0.
 * This contractor is necessary because with SIC, the evaluation of the constraint does not only depends on the cell box,
 * it depends also on the paving stored in SIConstraintCache.
 */
class CtcEvaluation: public Ctc {
private:
	const SIPSystem& system_;
public:
	CtcEvaluation(const SIPSystem& system);
	virtual ~CtcEvaluation();
	void add_property(const IntervalVector& init_box, BoxProperties& map);
    void contract(IntervalVector& box);
    void contract(IntervalVector& box, ContractContext& context);
};

} // end namespace ibex

#endif // __SIP_IBEX_CTCEVALUATION_H__
