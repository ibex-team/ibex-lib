/* ============================================================================
 * I B E X - Implementation of the Interval class based on filib
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Jordan Ninin
 * Created     : Jan 10, 2013
 * ---------------------------------------------------------------------------- */

#ifndef _IBEX_FILIB_INTERVAL_H_
#define _IBEX_FILIB_INTERVAL_H_

#include "ibex_Exception.h"
#include <cassert>
#include <float.h>
#include <iostream>
#include <climits>

namespace ibex {

inline void fpu_round_down() {
	filib::fp_traits<FI_BASE,FI_ROUNDING>::downward();
}

inline void fpu_round_up() {
	filib::fp_traits<FI_BASE,FI_ROUNDING>::upward();
}

inline void fpu_round_near() {
	filib::fp_traits<FI_BASE,FI_ROUNDING>::tonearest();
}

inline double previous_float(double x) {
	if ( x==NEG_INFINITY)	return x;
	else return filib::primitive::pred(x);
}

inline double next_float(double x) {
	if (x==POS_INFINITY)	return x;
	else return filib::primitive::succ(x);
}

inline void fpu_round_zero() {
	filib::fp_traits<FI_BASE,FI_ROUNDING>::tozero();
}

inline Interval::Interval(const FI_INTERVAL& x)  : itv(x)  { 

}

inline Interval& Interval::operator=(const FI_INTERVAL& x) {
	this->itv = x;
	return *this;
}

inline Interval& Interval::operator+=(double d) {
	if (d==POS_INFINITY || d==NEG_INFINITY)
		set_empty();
	else
		itv+=d;
	return *this;
}

inline Interval& Interval::operator-=(double d) {
	if (d==POS_INFINITY || d==NEG_INFINITY)
		set_empty();
	else
		itv-=d;
	return *this;

}

inline Interval& Interval::operator*=(double d) {
	if (d==POS_INFINITY || d==NEG_INFINITY)
		set_empty();
	else
		*this*=Interval(d);
	return *this;
}

inline Interval& Interval::operator/=(double d) {
	if (d==POS_INFINITY || d==NEG_INFINITY)
		set_empty();
	else{
		*this/=Interval(d);
	}
	return *this;
}

inline Interval& Interval::operator+=(const Interval& x) {
	itv+=x.itv;
	return *this;
}

inline Interval& Interval::operator-=(const Interval& x) {
	itv-=x.itv;
	return *this;
}

inline Interval& Interval::operator*=(const Interval& y) {

	FI_INTERVAL r;

	if (is_empty()) return *this;
	if (y.is_empty()) { *this=Interval::EMPTY_SET; return *this; }

	const FI_BASE& a(lb());
	const FI_BASE& b(ub());
	const FI_BASE& c(y.lb());
	const FI_BASE& d(y.ub());

	if ((a==0 && b==0) || (c==0 && d==0)) { *this=Interval(0.0,0.0); return *this; }

	if (((a<0) && (b>0)) && (c==NEG_INFINITY || d==POS_INFINITY)) { *this=Interval(NEG_INFINITY, POS_INFINITY); return *this; }

	if (((c<0) && (d>0)) && (a==NEG_INFINITY || b==POS_INFINITY)) { *this=Interval(NEG_INFINITY, POS_INFINITY); return *this; }

	// [-inf, _] x [_ 0] ou [0,_] x [_, +inf]
	if (((a==NEG_INFINITY) && (d==0)) || ((d==POS_INFINITY) && (a==0))) {
		if ((b<=0) || (c>=0)) { *this=Interval(0.0, POS_INFINITY); return *this; }
		else {
			r = FI_INTERVAL(b)*FI_INTERVAL(c);
			*this=Interval(r.inf(), POS_INFINITY);
			return *this;
		}
	}

	// [-inf, _] x [0, _] ou [0, _] x [-inf, _]
	if (((a==NEG_INFINITY) && (c==0)) || ((c==NEG_INFINITY) && (a==0))) {
		if ((b<=0) || (d<=0)) { *this=Interval(NEG_INFINITY, 0.0); return *this; }
		else {
			r = FI_INTERVAL(b)*FI_INTERVAL(d);
			*this=Interval(NEG_INFINITY, r.sup());
			return *this;
		}
	}

	// [_,0] x [-inf, _] ou [_, +inf] x [0,_]
	if (((c==NEG_INFINITY) && (b==0)) || ((b==POS_INFINITY) && (c==0))) {
		if ((d<=0) || (a>=0)) { *this=Interval(0.0, POS_INFINITY); return *this; }
		else {
			r = FI_INTERVAL(a)*FI_INTERVAL(d);
			*this=Interval(r.inf(), POS_INFINITY);
			return *this;
		}
	}

	// [_, +inf] x [_,0] ou [_,0] x [_, +inf]
	if (((b==POS_INFINITY) && (d==0)) || ((d==POS_INFINITY) && (b==0))) {
		if ((a>=0) || (c>=0)) { *this=Interval(NEG_INFINITY, 0.0); return *this; }
		else {
			r = FI_INTERVAL(a)*FI_INTERVAL(c);
			*this=Interval(NEG_INFINITY, r.sup());
			return *this;
		}
	}
	*this= itv*y.itv;
	return *this;
}


inline Interval& Interval::operator/=(const Interval& y) {

	if (is_empty()) return *this;
	if (y.is_empty()) { *this=Interval::EMPTY_SET; return *this; }

	const FI_BASE& a(lb());
	const FI_BASE& b(ub());
	const FI_BASE& c(y.lb());
	const FI_BASE& d(y.ub());

	FI_INTERVAL r;

	if (c==0 && d==0) {
		*this=Interval::EMPTY_SET;
		return *this;
	}

	if (a==0 && b==0) {
		// TODO: 0/0 can also be 1...
		return *this;
	}

	if (c>0 || d<0) {
		*this= itv / y.itv;
		return *this;
	}

	if ((b<=0) && d==0) {
		r = FI_INTERVAL(b) / FI_INTERVAL(c);
		*this=Interval(r.inf(), POS_INFINITY);
		return *this;
	}

	if (b<=0 && c<0 && d<0) {
		*this=Interval(NEG_INFINITY, POS_INFINITY);
		return *this;
	}

	if (b<=0 && c==0) {
		r = FI_INTERVAL(b) / FI_INTERVAL(d);		
		*this=Interval(NEG_INFINITY, r.sup());
		return *this;
	}

	if (a>=0 && d==0) {
		r = FI_INTERVAL(a) / FI_INTERVAL(c);
		*this=Interval(NEG_INFINITY, r.sup());
		return *this;
	}

	if (a>=0 && c<0 && d>0) {
		*this=Interval(NEG_INFINITY, POS_INFINITY);
		return *this;
	}

	if (a>=0 && c==0) {
		r = FI_INTERVAL(a) / FI_INTERVAL(d);
		*this=Interval(r.inf(), POS_INFINITY);
		return *this;
	}

	*this=Interval(NEG_INFINITY, POS_INFINITY); // a<0<b et c<=0<=d
	return *this;

}

/*inline Interval Interval::operator+() const {
	return *this;
}*/

inline Interval Interval:: operator-() const {
	return -itv;
}

inline Interval& Interval::div2_inter(const Interval& x, const Interval& y) {
	Interval out2;
	div2_inter(x,y,out2);
	return *this |= out2;
}

inline void Interval::set_empty() {
	*this = EMPTY_SET;
}

inline Interval& Interval::operator&=(const Interval& x) {
	itv = itv.intersect(x.itv);
	return *this;
}

inline Interval& Interval::operator|=(const Interval& x) {
	itv=itv.hull(x.itv);
	return *this;
}

inline double Interval::lb() const {
	return itv.inf();
}

inline double Interval::ub() const {
	return itv.sup();
}

inline double Interval::mid() const {
	if (itv.inf()==NEG_INFINITY)
		if (itv.sup()==POS_INFINITY) return 0;
		else return -DBL_MAX;
	else if (itv.sup()==POS_INFINITY) return DBL_MAX;
	else {
		double m=itv.mid();
		if (m<itv.inf()) m=itv.inf(); // watch dog
		else if (m>itv.sup()) m=itv.sup();
		return m;
	}
}

inline bool Interval::is_empty() const {
	return itv.isEmpty();
}

inline bool Interval::is_degenerated() const {
	return is_empty() || itv.isPoint();
}

inline bool Interval::is_unbounded() const {
	if (is_empty()) return false;
	return lb()==NEG_INFINITY || ub()==POS_INFINITY;
}


inline double Interval::diam() const {
	return itv.width();
}

inline double Interval::mig() const {
	return itv.mig();
}

inline double Interval::mag() const {
	return itv.mag();
}

inline Interval operator&(const Interval& x1, const Interval& x2) {
	return x1.itv.intersect(x2.itv);
}


inline Interval operator|(const Interval& x1, const Interval& x2) {
	if (x1.is_empty()) return x2;
	if (x2.is_empty()) return x1;
	return x1.itv.hull(x2.itv);
}

inline double hausdorff(const Interval &x1, const Interval &x2) {
	return x1.itv.dist(x2.itv);
}

inline Interval operator+(const Interval& x, double d) {
	if(d==NEG_INFINITY || d==POS_INFINITY)
		return Interval::EMPTY_SET;
	else
		return x.itv+d;
}

inline Interval operator-(const Interval& x, double d) {
	if(d==NEG_INFINITY || d==POS_INFINITY)
		return Interval::EMPTY_SET;
	else
		return x.itv-d;
}

inline Interval operator*(const Interval& x, double d) {
	if(d==NEG_INFINITY || d==POS_INFINITY)
		return Interval::EMPTY_SET;
	else {
		Interval res(d);
		return res*=x;
	}
}

inline Interval operator/(const Interval& x, double d) {
	if(d==NEG_INFINITY || d==POS_INFINITY)
		return Interval::EMPTY_SET;
	else{
		Interval res(x);
		return res/=Interval(d);
	}
}

inline Interval operator+(double d,const Interval& x) {
	if(d==NEG_INFINITY || d==POS_INFINITY)
		return Interval::EMPTY_SET;
	else
		return d+x.itv;
}

inline Interval operator-(double d, const Interval& x) {
	if(d==NEG_INFINITY || d==POS_INFINITY)
		return Interval::EMPTY_SET;
	else
		return d-x.itv;
}

inline Interval operator*(double d, const Interval& x) {
	if(d==NEG_INFINITY || d==POS_INFINITY)
		return Interval::EMPTY_SET;
	else {
		Interval res(d);
		return res*=x;
	}
}

inline Interval operator/(double d, const Interval& x) {
	if(d==NEG_INFINITY || d==POS_INFINITY)
		return Interval::EMPTY_SET;
	else{
		Interval res(d);
		return res/=x;
	}
}

inline Interval operator+(const Interval& x1, const Interval& x2) {
	return x1.itv+x2.itv;
}

inline Interval operator-(const Interval& x1, const Interval& x2) {
	return x1.itv-x2.itv;
}

inline Interval operator*(const Interval& x1, const Interval& x2) {
	Interval res(x1);
	return res*=x2;
}

inline Interval operator/(const Interval& x1, const Interval& x2) {
	Interval res(x1);
	return res/=Interval(x2);
}

inline Interval sqr(const Interval& x) {
	return filib::sqr(x.itv);
}

inline Interval sqrt(const Interval& x) {
	return filib::sqrt(x.itv) & Interval::POS_REALS;
}

inline Interval pow(const Interval& x, int n) {
	if (n==0)
		return Interval::ONE;
	else if (n<0)
		return 1.0/Interval(filib::power(x.itv,-n));
	else 
		return filib::power(x.itv,n);
}

inline Interval pow(const Interval &x, double d) {
	if(d==NEG_INFINITY || d==POS_INFINITY)
		return Interval::EMPTY_SET;
	else if (d==0)
		return Interval::ONE;
	else if (d<0)
		return 1.0/pow(x,-d);
	else
		return pow(x,Interval(d));
}

inline Interval pow(const Interval &x, const Interval &y) {
	return filib::pow(x.itv, y.itv);
}



inline Interval root(const Interval& x, int n) {

	if (x.is_empty()) return Interval::EMPTY_SET;
	if (x.lb()==0 && x.ub()==0) return Interval::ZERO;
	if (n==0) return Interval::ONE;
	if (n<0) return 1.0/root(x,-n);
	if (n==1) return x;

	if (n%2==0) {
		return pow(x,Interval::ONE/n);   // the negative part of x should be removed
	} else {
		return pow(x,Interval::ONE/n) |  // the negative part of x should be removed
	    (-pow(-x,Interval::ONE/n)); // the positive part of x should be removed
	}

}



inline Interval exp(const Interval& x) {
	return filib::exp(x.itv);
}

inline Interval log(const Interval& x) {
	if (x.ub()<=0) // filib returns (-oo,-DBL_MAX) if x.ub()==0, instead of EMPTY_SET
		return Interval::EMPTY_SET;
	else if (x.ub()<=next_float(0))
		return Interval(NEG_INFINITY,filib::filib_consts<FI_BASE>::q_minr);
	else
		return filib::log(x.itv);
}

inline Interval cos(const Interval& x) {
	return filib::cos(x.itv);
}

inline Interval sin(const Interval& x) {
	return filib::sin(x.itv);
}

inline Interval tan(const Interval& x) {
	Interval res =  filib::tan(x.itv);
	if (res.is_empty()&& (!x.is_empty())) {
		return Interval::ALL_REALS;
	} else
		return res;
}

inline Interval acos(const Interval& x) {
	return filib::acos(x.itv);
}

inline Interval asin(const Interval& x) {
	return filib::asin(x.itv);
}

inline Interval atan(const Interval& x) {
	return filib::atan(x.itv);
}

inline Interval cosh(const Interval& x) {
	return filib::cosh(x.itv);
}

inline Interval sinh(const Interval& x) {
	return filib::sinh(x.itv);
}

inline Interval tanh(const Interval& x) {
	return filib::tanh(x.itv);
}

inline Interval acosh(const Interval& x) {
	return filib::acosh(x.itv);
}

inline Interval asinh(const Interval& x) {
	return filib::asinh(x.itv);
}

inline Interval atanh(const Interval& x) {
	return filib::atanh(x.itv);
}

inline Interval abs(const Interval &x) {
	return filib::abs(x.itv);
}

inline Interval max(const Interval& x, const Interval& y) {
	if (x.is_empty()||y.is_empty()) return Interval::EMPTY_SET;
	else return x.itv.imax(y.itv);
}

inline Interval min(const Interval& x, const Interval& y) {
	if (x.is_empty()||y.is_empty()) return Interval::EMPTY_SET;
	else return x.itv.imin(y.itv);
}

inline Interval integer(const Interval& x) {
	return Interval(std::ceil(x.lb()),std::floor(x.ub()));
}

inline bool bwd_mul(const Interval& y, Interval& x1, Interval& x2) {
	if (y.contains(0)) {
		if (!x2.contains(0))                           // if y and x2 contains 0, x1 can be any real number.
			if (x1.div2_inter(y,x2).is_empty()) { x2.set_empty(); return false; }  // otherwise y=x1*x2 => x1=y/x2
		if (x1.contains(0)) return true;
		if (x2.div2_inter(y,x1).is_empty()) { x1.set_empty(); return false; }
		else return true;
	} else {
		if (x1.div2_inter(y,x2).is_empty()) { x2.set_empty(); return false; }
		if (x2.div2_inter(y,x1).is_empty()) { x1.set_empty(); return false; }
		else return true;
	}

}

inline bool bwd_sqr(const Interval& y, Interval& x) {

	Interval proj=sqrt(y);
	Interval pos_proj= proj & x;
	Interval neg_proj = (-proj) & x;

	x = pos_proj | neg_proj;
	return !x.is_empty();

}

inline bool bwd_pow(const Interval& y, int expon, Interval& x) {
	if (expon % 2 ==0) {
		Interval proj=root(y,expon);
		Interval pos_proj= proj & x;
		Interval neg_proj = (-proj) & x;
		x = pos_proj | neg_proj;
	} 
	else {
		x &= root(y, expon);
	}
	return !x.is_empty();
}

inline bool bwd_pow(const Interval& , Interval& , Interval& ) {
	ibex_error("bwd_power(y,x1,x2) (with x1 and x2 intervals) not implemented yet with filib");
	return false;
}


/**
 * ftype:
 *   COS = 0
 *   SIN = 1
 *   TAN = 2
 */
inline bool bwd_trigo(const Interval& y, Interval& x, int ftype) {

	const int COS=0;
	const int SIN=1;
	const int TAN=2;

	Interval period_0, nb_period;

	switch (ftype) {
	case COS :
		period_0 = acos(y); break;
	case SIN :
		period_0 = asin(y); break;
	case TAN :
		period_0 = atan(y); break;
	default : 
		assert(false); break;
	} 

	if (period_0.is_empty()) { x.set_empty(); return false; }

	if (x.lb()==NEG_INFINITY || x.ub()==POS_INFINITY) return true; // infinity of periods

	switch (ftype) {
	case COS :
		nb_period = x / Interval::PI; break;
	case SIN :
		nb_period = (x+Interval::HALF_PI) / Interval::PI; break;
	case TAN :
		nb_period = (x+Interval::HALF_PI) / Interval::PI; break;
	default :
		assert(false); break;
	}

	if (nb_period.mag() > INT_MAX) return true;
	
	int p1 = ((int) nb_period.lb())-1;
	int p2 = ((int) nb_period.ub());
	Interval tmp1, tmp2;

	bool found = false;
	int i = p1-1;

	switch(ftype) {
	case COS :
		// should find in at most 2 turns.. but consider rounding !
		while (++i<=p2 && !found) found = !(tmp1 = (x & (i%2==0? period_0 + i*Interval::PI : (i+1)*Interval::PI - period_0))).is_empty();
		break;
	case SIN :
		while (++i<=p2 && !found) found = !(tmp1 = (x & (i%2==0? period_0 + i*Interval::PI : i*Interval::PI - period_0))).is_empty();
		break;
	case TAN :
		while (++i<=p2 && !found) found = !(tmp1 = (x & (period_0 + i*Interval::PI))).is_empty();
		break;
	}

	if (!found) { x.set_empty(); return false; }
	found = false;
	i=p2+1;

	switch(ftype) {
	case COS :
		while (--i>=p1 && !found) found = !(tmp2 = (x & (i%2==0? period_0 + i*Interval::PI : (i+1)*Interval::PI - period_0))).is_empty();
		break;
	case SIN :
		while (--i>=p1 && !found) found = !(tmp2 = (x & (i%2==0? period_0 + i*Interval::PI : i*Interval::PI - period_0))).is_empty();
		break;
	case TAN :
		while (--i>=p1 && !found) found = !(tmp2 = (x & (period_0 + i*Interval::PI))).is_empty();
		break;
	}

	if (!found) {  x.set_empty(); return false; }

	x = tmp1 | tmp2;

	return true;
}


inline bool bwd_cos(const Interval& y,  Interval& x) {
	return bwd_trigo(y,x,0);
}

inline bool bwd_sin(const Interval& y,  Interval& x) {
	return bwd_trigo(y,x,1);
}

inline bool bwd_tan(const Interval& y,  Interval& x) {
	return bwd_trigo(y,x,2);
}

inline bool bwd_cosh(const Interval& y,  Interval& x) {

	Interval proj=acosh(y);
	if (proj.is_empty()) return false;
	Interval pos_proj= proj & x;
	Interval neg_proj = (-proj) & x;

	x = pos_proj | neg_proj;

	return !x.is_empty();
}

inline bool bwd_sinh(const Interval& y,  Interval& x) {
	x &= asinh(y);
	return !x.is_empty();
}

inline bool bwd_tanh(const Interval& y,  Interval& x) {
	x &= atanh(y);
	return !x.is_empty();
}



inline bool bwd_abs(const Interval& y,  Interval& x) {
	Interval x1 = x & y;
	Interval x2 = x & (-y);
	x &= x1 | x2;
	return !x.is_empty();
}


} // end namespace ibex

#endif // _IBEX_filib_INTERVAL_H_
