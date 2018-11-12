/* ============================================================================
 * I B E X - ibex_LoupFinderRestrictionsRelax.h
 * ============================================================================
 * Copyright   : IMT Atlantique (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Antoine Marendet
 * Created     : Nov 12, 2018
 * ---------------------------------------------------------------------------- */
 
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
	Linearizer& linearizer_;
	LPSolver* lp_solver_;
public:
	LoupFinderRestrictionsRelax(const SIPSystem& system, Linearizer& restrictions);
	virtual ~LoupFinderRestrictionsRelax();
	std::pair<IntervalVector, double> find(const IntervalVector& box, const IntervalVector& loup_point, double loup);
	std::pair<IntervalVector, double> find(const IntervalVector& box, const IntervalVector& loup_point, double loup, BoxProperties& prop);

};

} // end namespace ibex

#endif // __SIP_IBEX_LOUPFINDERRESTRICTIONSRELAX_H__

