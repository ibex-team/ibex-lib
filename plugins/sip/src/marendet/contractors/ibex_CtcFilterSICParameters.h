/* ============================================================================
 * I B E X - ibex_CtcFilterSICParameters.h
 * ============================================================================
 * Copyright   : IMT Atlantique (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Antoine Marendet, Gilles Chabert
 * Created     : May 4, 2018
 * ---------------------------------------------------------------------------- */
 
#ifndef __SIP_IBEX_CTCFILTERSICPARAMETERS_H__
#define __SIP_IBEX_CTCFILTERSICPARAMETERS_H__

#include "ibex_SIPSystem.h"

#include "ibex_Ctc.h"
#include "ibex_IntervalVector.h"

namespace ibex {
/**
 * \brief CtcFilterSICParameters
 *
 * This cell contractor remove from the SIConstraintCache parameters for which g(x,y) <= 0, because we do not need to keep
 * parameters box for which the constraint is satisfied. We also instantiate parameter boxes for which the evaluation of
 * the gradient of the constraint is > 0 or < 0. See the article for more details.
 */
class CtcFilterSICParameters: public Ctc {
private:
	const SIPSystem& system_;
	//void contractOneConstraint(size_t i, BxpNodeData& node_data, const IntervalVector& box);
public:
	CtcFilterSICParameters(const SIPSystem& system_);
	virtual ~CtcFilterSICParameters();
	void add_property(const IntervalVector& init_box, BoxProperties& map);
    void contract(IntervalVector& box);
    void contract(IntervalVector& box, ContractContext& context);
};

} // end namespace ibex

#endif // __SIP_IBEX_CTCFILTERSICPARAMETERS_H__
