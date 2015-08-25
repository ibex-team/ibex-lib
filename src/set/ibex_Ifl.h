	//============================================================================
//                                  I B E X                                   
// File        : ibex_Ifl.h
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Jul 19, 2013
// Last Update : Jul 19, 2013
//============================================================================

#ifndef __IBEX_IFL_H__
#define __IBEX_IFL_H__

#include "ibex_IntervalVector.h"

namespace ibex {

/**
 * \brief Inflator interface
 */

class Ifl {
public:
	/**
	 * Possible return status of inflation.
	 * Note that the value are ordered (this order is used, e.g., in Sweep).
	 */
	typedef enum {BAD_POINT, UNKNOWN_POINT, NOT_SIGNIFICANT, INFLATE, FULL_INFLATE} ifl_status;

	/**
	 * \param pt: the point to inflate
	 * \param box: as input, represents the working area.
	 * As output: the inflated box.
	 */
	virtual ifl_status inflate(IntervalVector& box, const IntervalVector& pt=IntervalVector::empty(1))=0;

	virtual ~Ifl();
};

std::ostream& operator<<(std::ostream& os, const Ifl::ifl_status& status);

} // end namespace ibex
#endif // IBEX_IFL_H_
