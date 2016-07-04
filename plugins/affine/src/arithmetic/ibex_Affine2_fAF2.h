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

#ifndef IBEX_AFFINE2_FAF2_H_
#define IBEX_AFFINE2_FAF2_H_

#include <math.h>
#ifndef _MSC_VER
	#ifdef __FP_FAST_FMA
		#define IBEX_FMA
	#endif
	#ifdef FP_FAST_FMA
		#define IBEX_FMA
	#endif

#else
#if (_MSC_VER >= 1800)
	#define IBEX_FMA
#endif
#endif




#include "ibex_Interval.h"

namespace ibex {

template<class T>  class AffineMain;

class AF_fAF2 {

	friend class AffineMain<AF_fAF2>;

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

	/**
	 * \brief return the exact rounding error of the addition of 2 floating-point numbers
	 */
	double twoSum(double a, double b, double *res);

	/**
	 * \brief return the exact rounding error of the multiplication of 2 floating-point numbers
	 */
	double twoProd(double a, double b, double *res);
	void Split(double x, int sp, double *x_high, double *x_low);



public:
	/** \brief Create an empty affine form. */
	AF_fAF2(double * val, double err);

	/** \brief  Delete the affine form */
	virtual ~AF_fAF2();

};


inline AF_fAF2::AF_fAF2(double * val, double err) :
	_val	(val ),
	_err	(err) {

}



inline AF_fAF2::~AF_fAF2() {
	if (_val!=NULL) delete[] _val;
}



/////////////////////
// CODE extract from "Handbook of Floating-Point Arithmetic" p.132-139
inline void AF_fAF2::Split(double x, int sp, double *x_high, double *x_low)
{
	unsigned long C = (1UL << sp) + 1;
	double gamma = (C * x);
	double delta = (x - gamma);
	*x_high= (gamma + delta);
	*x_low= (x - *x_high);
}

inline double AF_fAF2::twoProd(double x, double y, double *r_1)
{
#ifdef IBEX_FMA
	*r_1 = (x * y);
	return std::fma(x,y,-(*r_1));
#else

	int SHIFT_POW = 27; //  53 / 2 for double precision.
	double x_high, x_low;
	double y_high, y_low;
	double t_1;
	double t_2;
	double t_3;
	Split(x, SHIFT_POW, &x_high, &x_low);
	Split(y, SHIFT_POW, &y_high, &y_low);
	*r_1 = (x * y);
	t_1 = (-*r_1 + x_high * y_high);
	t_2 =   (t_1 + x_high * y_low );
	t_3 =	(t_2 + x_low  * y_high);
	return  (t_3 + x_low  * y_low );

#endif

}



// CODE extract from "Handbook of Floating-Point Arithmetic" p.130
inline double AF_fAF2::twoSum(double a, double b, double *res) {
	*res = (a+b);
	double a2 = (*res - b);
	double b2 = (*res - a2);
	double delta_a = (a - a2);
	double delta_b = (b - b2);
	return (delta_a + delta_b);
}

//////////////////////

}

#endif /* IBEX_AFFINE2_FAF2_H_ */
