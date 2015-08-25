//============================================================================
//                                  I B E X                                   
// File        : ibex_IflSet.h
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Jun 23, 2015
//============================================================================

#ifndef __IBEX_IFLSET_H__
#define __IBEX_IFLSET_H__

#include "ibex_Ifl.h"
#include "ibex_Set.h"

namespace ibex {

/**
 * \brief Inflator wrt a set (basic variant)
 */
class IflSet : public Ifl {
public:
	IflSet(Set& set);

	virtual Ifl::ifl_status inflate(IntervalVector& box, const IntervalVector& pt=IntervalVector::empty(1));

	Set& set;
};

} // namespace ibex

#endif // __IBEX_IFLSET_H__
