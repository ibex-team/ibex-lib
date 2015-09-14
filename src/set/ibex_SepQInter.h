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

 // NOTES : - Comme la dimension du separateur n est pas connu, les boites qui contiennent 
 //			  les resultats des appels aux separateur sont realloue a chaque appel a separate.
 //			- Pour l'interface python, la valeur de q par defaut est zero et doit etre definie par un
 // 		  appel setq.

class SepQInterProjF : public Sep {
public:
	/**
	 * \brief q-intersection on a list of separators.
	 *
	 * The list itself is not kept by reference.
	 */
    SepQInterProjF(const Array<Sep>& list);


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

// protected:
	// IntervalMatrix boxes; // store boxes for each contraction

};

} // end namespace ibex
#endif // __IBEX_SEP_QINTER_H__
