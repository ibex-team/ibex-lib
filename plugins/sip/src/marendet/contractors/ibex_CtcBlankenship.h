//============================================================================
//                                  I B E X                                   
// File        : ibex_CtcBlankenship.h
// Author      : Antoine Marendet, Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : May 4, 2018
// Last Update : May 4, 2018
//============================================================================

#ifndef __SIP_IBEX_CTCBLANKENSHIP_H__
#define __SIP_IBEX_CTCBLANKENSHIP_H__

#include "ibex_RelaxationLinearizerSIP.h"
#include "ibex_SIPSystem.h"

#include "ibex_Ctc.h"
#include "ibex_IntervalVector.h"
#include "ibex_LPSolver.h"
#include "ibex_Vector.h"

namespace ibex {
/**
 * \brief CtcBlankenship.
 *
 * Maximize each SIC for the linear relaxation point with respect to the parameters,
 * and add the parameter point to a "Blankenship points" pool. The pool has a max size of 2n,
 * where n is the number of variables of the constraint.
 */
class CtcBlankenship: public Ctc {
public:
	CtcBlankenship(SIPSystem& system, double eps_, int max_iter);
	virtual ~CtcBlankenship();
	void add_property(const IntervalVector& init_box, BoxProperties& map);
    void contract(IntervalVector& box);
    void contract(IntervalVector& box, ContractContext& context);
	bool maximizeSIC(int sic_index, const Vector& uplo_point);

private:
	IntervalVector* box_;
	BxpNodeData* node_data_ = nullptr;
	const SIPSystem& system_;
	double eps_;
	int max_iter_;
	RelaxationLinearizerSIP relax_;
	LPSolver lp_solver_;

	ParameterEvaluationsCache _createNewCache(const SIConstraint& constraint, const IntervalVector& box,
			const IntervalVector& parameter_box);
};

} // end namespace ibex

#endif // __SIP_IBEX_CTCBLANKENSHIP_H__
