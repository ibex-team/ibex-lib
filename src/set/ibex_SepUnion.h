//============================================================================
//                                  I B E X
// File        : Union of separators
// Author      : Benoit Desrochers, Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : May 14, 2012
// Last Update : May 14, 2012
//============================================================================

#ifndef __IBEX_SEP_UNION_H__
#define __IBEX_SEP_UNION_H__

#include "ibex_Sep.h"
#include "ibex_Array.h"

namespace ibex {

/**
 * \ingroup iset
 *
 * \brief Union of separators
 *
 * SepInter(s1,...sn) generates a separator for the set
 *
 *             S1 \cup ... \cup Sn
 *
 * where Si is the set associated to si.
 *
 * For a box [x], SepInter performs:
 *
 *       x_out := s_1^{out}([x]) \cup ... \cup s_n^{out}([x]) and
 *       x_in  := s_1^{in}([x])  \cap ... \cap s_n^{in}([x])
 *
 * where s_i^{in} (resp. s_i^{out}) denotes the inner (resp. outer) contraction
 * of s_i.
 */
class SepUnion : public Sep {
public:

	/**
     * \brief Create the union of all the separators in the list.
	 */
    SepUnion(const Array<Sep>& list);

	/**
     * \brief Create the union of two separators.
	 */
    SepUnion(Sep& s1, Sep& s2);

	/**
     * \brief Create the union of three separators.
	 */
    SepUnion(Sep& s1, Sep& s2, Sep& s3);

	/**
     * \brief Create the union of 4 separators.
	 */
    SepUnion(Sep& c1, Sep& c2, Sep& c3, Sep& c4);

	/**
     * \brief Create the union of 5 separators.
	 */
    SepUnion(Sep& c1, Sep& c2, Sep& c3, Sep& c4, Sep& c5);

	/**
     * \brief Create the union of 6 separators.
	 */
    SepUnion(Sep& c1, Sep& c2, Sep& c3, Sep& c4, Sep& c5, Sep& c6);

    /**
     * \brief Separate a box.
     */
   virtual void separate(IntervalVector &x_in, IntervalVector &x_out);

    /**
     * \brief The list of sub-separators.
	 */
    Array<Sep> list;
};

} // end namespace ibex

#endif // __IBEX_SEP_UNION_H__
