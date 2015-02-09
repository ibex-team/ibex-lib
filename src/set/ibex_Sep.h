/* ============================================================================
 * I B E X - Separator interface
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (France)
 *
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Benoit Desrochers, ENSTA Bretagne (France)
 *               Gilles Chabert, Mines de Nantes (France)
 *
 * Created     : Mar 24, 2014
 * ---------------------------------------------------------------------------- */

#ifndef __IBEX_SEPARATOR_H__
#define __IBEX_SEPARATOR_H__

#include "ibex_IntervalVector.h"

namespace ibex {

/**
 * \ingroup iset
 *
 * \brief Separator interface.
 *
 * A separator is an operator that performs two independent and
 * complementary contractions. The separator is associated with
 * a set (noted S) and the first contraction (called "inner")
 * removes points inside S. The second contraction (called
 * "outer") removes points outside S.
 *
 * In summary:
 * Given a box [x], the separator produces two sub-boxes
 * [x_in] and [x_out] that verify:
 *
 *     ([x] \ [x_in]) \subset S
 *     ([x] \ [x_out]) \cap S = \emptyset
 *
 * For efficiency reasons, the separate(...) function takes only
 * two input-output arguments, x_in and x_out, each containing
 * initially a copy of the box [x].
 *
 * A separator can also be viewed a as pair of contractors.
 * See ibex::SepCtcPair.
 *
 * See L. Jaulin and B. Desrochers (2014). "Introduction to the
 * Algebra of Separators with Application to Path Planning".
 * Engineering Applications of Artificial Intelligence volume 33, pp. 141-147.
 *
 *
 */
class Sep {

public:
	/**
     * \brief Separate a box in two sub-boxes.
     *
     * \param x_in  - As input: the initial box. As output:
     *                result of the first ("inner") contraction
     * \param x_out - As input: the initial box. As output:
     *                the result of the second ("outer") contraction
     *
     * Precondition: x_in and x_out must be the same boxes.
	 */
    virtual void separate(IntervalVector& x_in, IntervalVector& x_out) = 0;


	/**
	 * \brief Delete *this.
	 */
    virtual ~Sep();
};

/* ============================================================================
 	 	 	 	 	 	 	 inline implementation
  ============================================================================*/

inline Sep::~Sep() { }


} // namespace ibex

#endif // __IBEX_SEPARATOR_H__
