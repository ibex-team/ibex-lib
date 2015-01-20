//============================================================================
//                                  I B E X                                   
// File        : Intersection of separators
// Author      : Benoit Desrochers, Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Mar 24, 2014
// Last Update : Mar 24, 2014
//============================================================================

#ifndef __IBEX_SEP_INTER_H__
#define __IBEX_SEP_INTER_H__

#include "ibex_Sep.h"
#include "ibex_Array.h"

namespace ibex {

/**
 * \ingroup iset
 *
 * \brief Intersection of separators
 *
 * SepInter(s1,...sn) generates a separator for the set
 *
 *             S1 \cap ... \cap Sn
 *
 * where Si is the set associated to si.
 *
 * For a box [x], SepInter performs:
 *
 *       x_out := s_1^{out}([x]) \cap ... \cap s_n^{out}([x]) and
 *       x_in  := s_1^{in}([x])  \cup ... \cup s_n^{in}([x])
 *
 * where s_i^{in} (resp. s_i^{out}) denotes the inner (resp. outer) contraction
 * of s_i.
 */
class SepInter : public Sep {
public:

    /**
     * \brief Create the intersection of all the separators in the list.
     */
    SepInter(const Array<Sep>& list);

    /**
     * \brief Create the intersection of two separators.
     */
    SepInter(Sep& s1, Sep& s2);

    /**
     * \brief Create the intersection of three separators
     */
    SepInter(Sep& s1, Sep& s2, Sep& s3);

    /**
     * \brief Create the intersection of 4 separators
     */
    SepInter(Sep& s1, Sep& s2, Sep& s3, Sep& s4);

    /**
     * \brief Create the intersection of 5 separators
     */
    SepInter(Sep& c1, Sep& c2, Sep& c3, Sep& c4, Sep& c5);

    /**
     * \brief Create the intersection of 6 separators
     */
    SepInter(Sep& c1, Sep& c2, Sep& c3, Sep& c4, Sep& c5, Sep& c6);

    /**
     * \brief Separate a box.
     */
    virtual void separate(IntervalVector& x_in, IntervalVector& x_out);

    /**
     * \brief The list of sub-separators.
     */
    Array<Sep> list;

};

} // end namespace ibex

#endif // __IBEX_SEP_INTER_H__
