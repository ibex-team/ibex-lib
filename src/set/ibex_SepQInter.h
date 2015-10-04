//============================================================================
//                                  I B E X                                   
// File        : ibex_SepQInter.h
// Author      : Benoit Desrochers, Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Apr 22, 2015
// Last Update : Apr 22, 2015
//============================================================================

#ifndef __IBEX_SEP_QINTER_H__
#define __IBEX_SEP_QINTER_H__

#include "ibex_Sep.h"
#include "ibex_Array.h"
#include "ibex_IntervalMatrix.h"

namespace ibex {
/**
 * \ingroup set
 * \brief Q-intersection separator.
 * 	Sep_In = N-Q-1-Inter
 *	Sep_Out = Q-Inter
 *
 */

class SepQInterProjF : public Sep {
public:
	/**
	 * \brief q-intersection on a list of separators.
	 *
	 * The list itself is not kept by reference.
	 */
    SepQInterProjF(const Array<Sep>& list, int q = 0);


    /**
     * Separates a box
     */
    virtual void separate(IntervalVector& xin, IntervalVector& xout);

	/**
	 * List of separators
	 */
	Array<Sep> list;

	void setq(int q);
	int getq();
	/**
	 * The number of contractors we have to intersect the
	 * result.
	 */
	int q;

protected:
        /**
         * @brief boxes_in : stores in boxes for each contraction
         */
        IntervalMatrix boxes_in;
         /**
          * @brief boxes_out : stores in boxes for each contraction
          */
        IntervalMatrix boxes_out;

};

} // end namespace ibex
#endif // __IBEX_SEP_QINTER_H__
