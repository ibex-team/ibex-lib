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
	LoupFinderLineSearch(const SIPSystem& system);
	virtual ~LoupFinderLineSearch();

	std::pair<IntervalVector, double> find(const IntervalVector& box, const IntervalVector& loup_point, double loup);
	std::pair<IntervalVector, double> find(const IntervalVector& box, const IntervalVector& loup_point, double loup, BoxProperties& prop);
	bool is_inner_with_paving_simplification(const IntervalVector& box);
	void blankenship(const IntervalVector& box);
private:
	RelaxationLinearizerSIP linearizer_;
	LPSolver lp_solver_;
	BxpNodeData* node_data_ = nullptr;
};

} // end namespace ibex

#endif // __SIP_IBEX_LOUPFINDERLINESEARCH_H__
