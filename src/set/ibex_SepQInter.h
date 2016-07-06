//============================================================================
//                                  I B E X
// File        : ibex_SepQInter.h
// Author      : Benoit Desrochers, Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Apr 22, 2015
// Last Update : Jul 05, 2016
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
 * See L. Jaulin and B. Desrochers (2014). "Introduction to the
 * Algebra of Separators with Application to Path Planning".
 * Engineering Applications of Artificial Intelligence volume 33, pp. 141-147.
 *
 */

class SepQInter : public Sep {
public:
	/**
	 * \brief q-intersection on a list of separators.
	 *
	 * \param list : list of separators
	 * 				The list itself is not kept by reference.
	 * \param q : the nunmber of constrains that can be relaxed
	 */
    SepQInter(const Array<Sep>& list, int q = 0);


  /**
   * Separates a box
   */
  virtual void separate(IntervalVector& xin, IntervalVector& xout);

	/**
	 * \brief list of separators
	 */
	Array<Sep> list;

	/**
	 * \param set the number of allowed outliers
	 */
	void set_q(int q);

	/**
	 * \return the number of allowed outliers
	 */
	int get_q();

protected:
  /**
   * \brief boxes_in : stores in boxes for each contraction
   */
  IntervalMatrix boxes_in;
   /**
    * \brief boxes_out : stores in boxes for each contraction
    */
  IntervalMatrix boxes_out;

	/**
	 * The number of contractors we have to intersect the
	 * result.
	 */
	int q;

};

/* ============================================================================
 	 	 	 	 	 	 	 inline implementation
  ============================================================================*/

inline void SepQInter::set_q(int q){
	assert (q >= 0 || q < list.size());
	this->q = q;
}

inline int SepQInter::get_q(){ return q; }

} // end namespace ibex
#endif // __IBEX_SEP_QINTER_H__
