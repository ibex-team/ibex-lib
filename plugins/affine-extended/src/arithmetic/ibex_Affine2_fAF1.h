/* ============================================================================
 * I B E X - Definition of the Affine2 class based on fAF version 1
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Jordan Ninin
 * Created     : Jul 16, 2013
 * ---------------------------------------------------------------------------- */

#ifndef IBEX_AFFINE2_FAF1_H_
#define IBEX_AFFINE2_FAF1_H_

#include "ibex_Interval.h"

namespace ibex {

template<class T>  class AffineMain;

class AF_fAF1 {

	friend class AffineMain<AF_fAF1>;

private:
	/**
	 * Code for the particular case:
	 * if the affine form is actif, _n>1  and _n is the size of the affine form
	 * if the set is degenerate, _n = 0 or itv().diam()< AF_EC()
	 * if the set is empty, _n = -1
	 * if the set is ]-oo,+oo[, _n = -2 and _ err=]-oo,+oo[
	 * if the set is [a, +oo[ , _n = -3 and _err = [a, +oo[
	 * if the set is ]-oo, a] , _n = -4 and _err = ]-oo, a]
	 *
	 */


	double * _val; 		// vector of elements of the affine form
	Interval _err; 	// error of the affine form, corresponded to the last term
	//	bool _actif; // boolean to know if the affine form is actif or not. This is to manage the particular case of EMPTY and an unbounded Interval

public:
	/** \brief Create an empty affine form. */
	AF_fAF1(double * val, Interval err);

	/** \brief  Delete the affine form */
	~AF_fAF1();

};


inline AF_fAF1::AF_fAF1(double * val, Interval err) :
	_val	(val ),
	_err	(err) {

}



inline AF_fAF1::~AF_fAF1() {
	if (_val!=NULL) delete[] _val;
}



}

#endif /* IBEX_Affine2_H_ */
