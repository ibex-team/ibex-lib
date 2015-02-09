//============================================================================
//                                  I B E X                                   
// File        : ibex_SepNot.h
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Nov 12, 2014
//============================================================================

#ifndef __IBEX_SEP_NOT_H__
#define __IBEX_SEP_NOT_H__

#include "ibex_Sep.h"

namespace ibex {

/**
 * \ingroup set
 *
 * \brief Negation of a Separator
 *
 * The negation of a separator s performs contraction
 * w.r.t. to the complementary of the set associated to s.
 */
class SepNot: public Sep {

public:
	/**
	 * \brief Build the negation of "sep"
	 */
	SepNot(Sep& sep);

    /**
     * \brief Separate a box.
     */
    virtual void separate(IntervalVector& x_in, IntervalVector& x_out);

protected:
	/**
	 * \brief The sub-separator
	 */
	Sep& sep;
};

} // namespace ibex

#endif // __IBEX_SEPNOT_H__
