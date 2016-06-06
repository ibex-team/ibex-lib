
/* ============================================================================
 * I B E X - Definition of the Affine2 class based on fAF version 2
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Jordan Ninin
 * Created     : Jul 16, 2013
 * ---------------------------------------------------------------------------- */


#ifndef IBEX_AFFINE2_SAF_H_
#define IBEX_AFFINE2_SAF_H_



#include "ibex_Interval.h"


namespace ibex {

template<class T>  class AffineMain;

class AF_sAF {

	friend class AffineMain<AF_sAF>;

private:
	/**
	 * Code for the particular case:
	 * if the affine form is actif, _n>1  and _n is the size of the affine form
	 * if the set is degenerate, _n = 0 or itv().diam()< AF_EC()
	 * if the set is empty, _n = -1
	 * if the set is ]-oo,+oo[, _n = -2
	 * if the set is [a, +oo[ , _n = -3 and _err= a
	 * if the set is ]-oo, a] , _n = -4 and _err= a
	 *
	 */

	double * _val; 		// vector of elements of the affine form
	double _err; 	// error of the affine form, corresponded to the last term
	//	bool _actif; // boolean to know if the affine form is actif or not. This is to manage the particular case of EMPTY and an unbounded Interval



public:
	/** \brief Create an empty affine form. */
	AF_sAF(double * val, double err);

	/** \brief  Delete the affine form */
	virtual ~AF_sAF();

};


inline AF_sAF::AF_sAF(double * val, double err) :
	_val	(val ),
	_err	(err) {

}



inline AF_sAF::~AF_sAF() {
	if (_val!=NULL) delete[] _val;
}

} // end namespace ibex
#endif /* IBEX_AFFINE2_SAF_H_ */
