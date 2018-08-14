//============================================================================
//                                  I B E X                                   
// File        : ibex_LoupFinderLineSearch.h
// Author      : Antoine Marendet, Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : May 4, 2018
// Last Update : May 4, 2018
//============================================================================

#ifndef __SIP_IBEX_LOUPFINDERLINESEARCH_H__
#define __SIP_IBEX_LOUPFINDERLINESEARCH_H__

#include "ibex_RelaxationLinearizerSIP.h"
#include "ibex_SIPSystem.h"

#include "ibex_Cell.h"
#include "ibex_IntervalVector.h"
#include "ibex_LPSolver.h"
#include "ibex_LoupFinderSIP.h"

#include <utility>

namespace ibex {
class LoupFinderLineSearch: public LoupFinderSIP {
public:
	LoupFinderLineSearch(SIPSystem& system);

	virtual std::pair<IntervalVector, double> find(const Cell& cell,
				const IntervalVector& loup_point, double loup);

	virtual ~LoupFinderLineSearch();

	bool is_inner_with_paving_simplification(const IntervalVector& box);
	void blankenship(const IntervalVector& box);
private:
	SIPSystem& system_;
	RelaxationLinearizerSIP linearizer_;
	LPSolver lp_solver_;
};

} // end namespace ibex

#endif // __SIP_IBEX_LOUPFINDERLINESEARCH_H__
