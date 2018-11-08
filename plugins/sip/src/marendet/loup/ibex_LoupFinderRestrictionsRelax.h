//============================================================================
//                                  I B E X                                   
// File        : ibex_LoupFinderRestrictionsRelax.h
// Author      : Antoine Marendet, Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : May 4, 2018
// Last Update : May 4, 2018
//============================================================================

#ifndef __SIP_IBEX_LOUPFINDERRESTRICTIONSRELAX_H__
#define __SIP_IBEX_LOUPFINDERRESTRICTIONSRELAX_H__

#include "ibex_SIPSystem.h"

#include "ibex_Cell.h"
#include "ibex_IntervalVector.h"
#include "ibex_Linearizer.h"
#include "ibex_LPSolver.h"
#include "ibex_LoupFinderSIP.h"

#include <utility>

namespace ibex {
class LoupFinderRestrictionsRelax: public LoupFinderSIP {
	const SIPSystem& system_;
	Linearizer& linearizer_;
	LPSolver* lp_solver_;
public:
	LoupFinderRestrictionsRelax(const SIPSystem& system, Linearizer& restrictions);
	virtual ~LoupFinderRestrictionsRelax();
	std::pair<IntervalVector, double> find(const IntervalVector& box, const IntervalVector& loup_point, double loup);

};

} // end namespace ibex

#endif // __SIP_IBEX_LOUPFINDERRESTRICTIONSRELAX_H__
