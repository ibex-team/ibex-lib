/* ============================================================================
 * I B E X - ibex_LoupFinderSIP.h
 * ============================================================================
 * Copyright   : IMT Atlantique (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Antoine Marendet
 * Created     : Nov 12, 2018
 * ---------------------------------------------------------------------------- */
 
#ifndef __SIP_IBEX_LOUPFINDERSIP_H__
#define __SIP_IBEX_LOUPFINDERSIP_H__

#include "ibex_SIPSystem.h"

#include "ibex_Cell.h"
#include "ibex_IntervalVector.h"
#include "ibex_LoupFinder.h"
#include "ibex_Vector.h"

#include <utility>

namespace ibex {
class LoupFinderSIP : public LoupFinder {
public:
	LoupFinderSIP(const SIPSystem& system);
	virtual ~LoupFinderSIP();

	void add_property(const IntervalVector& init_box, BoxProperties& prop);

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
	bool check(const SIPSystem& sys, const Vector& pt, double& loup, bool is_inner, BoxProperties& prop);

	const SIPSystem& system_;
};

} // end namespace ibex

#endif // __SIP_IBEX_LOUPFINDERSIP_H__

