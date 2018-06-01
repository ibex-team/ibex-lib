//============================================================================
//                                  I B E X                                   
// File        : ibex_LoupFinderSIP.h
// Author      : Antoine Marendet, Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : May 4, 2018
// Last Update : May 4, 2018
//============================================================================

#ifndef __SIP_IBEX_LOUPFINDERSIP_H__
#define __SIP_IBEX_LOUPFINDERSIP_H__

#include "ibex_SIPSystem.h"

#include "ibex_Cell.h"
#include "ibex_IntervalVector.h"
#include "ibex_LoupFinder.h"
#include "ibex_Vector.h"

#include <utility>

namespace ibex {
class LoupFinderSIP {
public:
	typedef LoupFinder::NotFound NotFound;

	/**
	 * \brief Find a new loup in a given box.
	 *
	 * This function is abstract and has to be implemented in the subclass.
	 *
	 * The function takes as parameter the last loup-point (x{k}) and
	 * loup value (f(x{k})) found by the optimizer and returns the new
	 * loup-point (x{k+1}) and its value (f(x{k+1}))
	 * Note that xk is not necessarily inside the box.
	 *
	 * \param cell        - the cell where x{k+1} is searched
	 * \param loup_point - an enclosure of x{k}
	 * \param loup       - (an upper bound of) f(x{k})
	 * \return             <x{k+1},f(x{k+1})>
	 * \throws             NotFound in case of failure.
	 */
	virtual std::pair<IntervalVector, double> find(const Cell& cell,
			const IntervalVector& loup_point, double loup)=0;

	virtual ~LoupFinderSIP();

protected:

	/**
	 * \brief Try to reduce the "loup" with a candidate point.
	 *
	 * \param sys      - The NLP problem.
	 * \param pt       - The candidate point.
	 * \param loup     - (input/output). Current loup (updated upon success).
	 * \param is_inner - If true, the point is already known to be inner so there
	 *                   is no need to check constraint satisfaction again. False
	 *                   means "unknown" and a quick check (see
	 *                   #System::is_inner(const IntervalVector&)) is performed.
	 *
	 * \return true in case of success, i.e., if the loup has been decreased.
	 */
	bool check(const SIPSystem& sys, const Vector& pt, double& loup, bool is_inner);

};

} // end namespace ibex

#endif // __SIP_IBEX_LOUPFINDERSIP_H__
