//============================================================================
//                                  I B E X                                   
// File        : ibex_CtcFilterSICParameters.h
// Author      : Antoine Marendet, Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : May 4, 2018
// Last Update : May 4, 2018
//============================================================================

#ifndef __SIP_IBEX_CTCFILTERSICPARAMETERS_H__
#define __SIP_IBEX_CTCFILTERSICPARAMETERS_H__

#include "ibex_SIPSystem.h"

#include "ibex_CellCtc.h"
#include "ibex_IntervalVector.h"

namespace ibex {
/**
 * \brief CtcFilterSICParameters
 *
 * This cell contractor remove from the SIConstraintCache parameters for which g(x,y) <= 0, because we do not need to keep
 * parameters box for which the constraint is satisfied. We also instantiate parameter boxes for which the evaluation of
 * the gradient of the constraint is > 0 or < 0. See the article for more details.
 */
class CtcFilterSICParameters: public CellCtc {
private:
	const SIPSystem& system_;
	void contractOneConstraint(size_t i, NodeData& node_data, const IntervalVector& box);
public:
	CtcFilterSICParameters(const SIPSystem& system_);
	virtual ~CtcFilterSICParameters();
	void contractCell(Cell& cell);
};

} // end namespace ibex

#endif // __SIP_IBEX_CTCFILTERSICPARAMETERS_H__
