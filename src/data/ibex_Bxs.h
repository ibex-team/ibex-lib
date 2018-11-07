//============================================================================
//                                  I B E X
// File        : ibex_Bxs.h
// Author      : Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Nov 07, 2018
// Last update : Not 07, 2018
//============================================================================

#ifndef __IBEX_BXS_H__
#define __IBEX_BXS_H__

#include "ibex_IntervalVector.h"
#include "ibex_BoolInterval.h"
#include "ibex_Solver.h"

#include <vector>

namespace ibex {

class Bxs {
public:
	virtual int subformat_number() const=0;

	virtual Bxs& operator&=(const Bxs& set)=0;

	Bxs(size_t n);

	virtual ~Bxs();

	/**
	 * \brief Number of variables
	 */
	const size_t n;

protected:
	static const char* SIGNATURE;
};

} /* namespace ibex */

#endif /* __IBEX_BXS_H__ */
