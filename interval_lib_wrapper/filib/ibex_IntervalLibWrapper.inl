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

#ifndef _IBEX_INTERVALLIBWRAPPER_INL_
#define _IBEX_INTERVALLIBWRAPPER_INL_

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

inline Interval::Interval(const FI_INTERVAL& x)  : itv(x), NaN(false)  { 

}

inline Interval& Interval::operator=(const FI_INTERVAL& x) {
	this->itv = x;
	this->NaN = false;
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
	NaN = NaN || x.NaN;
	return *this;
}

inline Interval& Interval::operator-=(const Interval& x) {
	itv-=x.itv;
	NaN = NaN || x.NaN;
	return *this;
}

inline Interval& Interval::operator*=(const Interval& y) {

	FI_INTERVAL r;
	bool saveNaN = this->NaN;
	
	if (is_empty()) {
		this->NaN = saveNaN || y.NaN;
		return *this;
	}
	if (y.is_empty()) { 
		this->set_empty(); 
		this->NaN = saveNaN || y.NaN;
		return *this; 
	}

	const FI_BASE& a(lb());
	const FI_BASE& b(ub());
	const FI_BASE& c(y.lb());
	const FI_BASE& d(y.ub());

	if ((a==0 && b==0) || (c==0 && d==0)) { 
		*this=Interval(0.0,0.0); 
		this->NaN = saveNaN || y.NaN;
		return *this; 
	}

	if (((a<0) && (b>0)) && (c==NEG_INFINITY || d==POS_INFINITY)) { 
		*this=Interval(NEG_INFINITY, POS_INFINITY); 
		this->NaN = saveNaN || y.NaN;
		return *this; 
	}

	if (((c<0) && (d>0)) && (a==NEG_INFINITY || b==POS_INFINITY)) { 
		*this=Interval(NEG_INFINITY, POS_INFINITY); 
		this->NaN = saveNaN || y.NaN;
		return *this; 
	}

	// [-inf, _] x [_ 0] ou [0,_] x [_, +inf]
	if (((a==NEG_INFINITY) && (d==0)) || ((d==POS_INFINITY) && (a==0))) {
		if ((b<=0) || (c>=0)) { 
			*this=Interval(0.0, POS_INFINITY); 
			this->NaN = saveNaN || y.NaN;
			return *this; 
		} else {
			r = FI_INTERVAL(b)*FI_INTERVAL(c);
			*this=Interval(r.inf(), POS_INFINITY);
			this->NaN = saveNaN || y.NaN;
			return *this;
		}
	}

	// [-inf, _] x [0, _] ou [0, _] x [-inf, _]
	if (((a==NEG_INFINITY) && (c==0)) || ((c==NEG_INFINITY) && (a==0))) {
		if ((b<=0) || (d<=0)) { 
			*this=Interval(NEG_INFINITY, 0.0); 
			this->NaN = saveNaN || y.NaN;
			return *this; 
		} else {
			r = FI_INTERVAL(b)*FI_INTERVAL(d);
			*this=Interval(NEG_INFINITY, r.sup());
			this->NaN = saveNaN || y.NaN;
			return *this;
		}
	}

	// [_,0] x [-inf, _] ou [_, +inf] x [0,_]
	if (((c==NEG_INFINITY) && (b==0)) || ((b==POS_INFINITY) && (c==0))) {
		if ((d<=0) || (a>=0)) { 
			*this=Interval(0.0, POS_INFINITY); 
			this->NaN = saveNaN || y.NaN;
			return *this; 
		} else {
			r = FI_INTERVAL(a)*FI_INTERVAL(d);
			*this=Interval(r.inf(), POS_INFINITY);
			this->NaN = saveNaN || y.NaN;
			return *this;
		}
	}

	// [_, +inf] x [_,0] ou [_,0] x [_, +inf]
	if (((b==POS_INFINITY) && (d==0)) || ((d==POS_INFINITY) && (b==0))) {
		if ((a>=0) || (c>=0)) { 
			*this=Interval(NEG_INFINITY, 0.0); 
			this->NaN = saveNaN || y.NaN;
			return *this; 
		} else {
			r = FI_INTERVAL(a)*FI_INTERVAL(c);
			*this=Interval(NEG_INFINITY, r.sup());
			this->NaN = saveNaN || y.NaN;
			return *this;
		}
	}
	*this= itv*y.itv;
	this->NaN = saveNaN || y.NaN;
	return *this;
}


inline Interval& Interval::operator/=(const Interval& y) {
	bool saveNaN = this->NaN;
	
	if (is_empty()) {
		this->NaN = saveNaN || y.NaN;
		return *this;
	}
	if (y.is_empty()) { 
		this->set_empty(); 
		this->NaN = saveNaN || y.NaN;
		return *this; 
	}

	const FI_BASE& a(lb());
	const FI_BASE& b(ub());
	const FI_BASE& c(y.lb());
	const FI_BASE& d(y.ub());

	FI_INTERVAL r;

	if (c==0 && d==0) {
		this->set_empty(); 
		this->NaN = true; // TODO TO CHECK
		return *this;
	}

	if (a==0 && b==0) {
		// TODO: 0/0 can also be 1...
		this->NaN = saveNaN || y.NaN;
		return *this;
	}

	if (c>0 || d<0) {
		*this= itv / y.itv;
		this->NaN = saveNaN || y.NaN;
		return *this;
	}

	if ((b<=0) && d==0) {
		r = FI_INTERVAL(b) / FI_INTERVAL(c);
		*this=Interval(r.inf(), POS_INFINITY);
		this->NaN = saveNaN || y.NaN;
		return *this;
	}

	if (b<=0 && c<0 && d<0) {
		*this=Interval(NEG_INFINITY, POS_INFINITY);
		this->NaN = saveNaN || y.NaN;
		return *this;
	}

	if (b<=0 && c==0) {
		r = FI_INTERVAL(b) / FI_INTERVAL(d);		
		*this=Interval(NEG_INFINITY, r.sup());
		this->NaN = saveNaN||y.NaN;
		return *this;
	}

	if (a>=0 && d==0) {
		r = FI_INTERVAL(a) / FI_INTERVAL(c);
		*this=Interval(NEG_INFINITY, r.sup());
		this->NaN = saveNaN||y.NaN;
		return *this;
	}

	if (a>=0 && c<0 && d>0) {
		*this=Interval(NEG_INFINITY, POS_INFINITY);
		this->NaN = saveNaN||y.NaN;
		return *this;
	}

	if (a>=0 && c==0) {
		r = FI_INTERVAL(a) / FI_INTERVAL(d);
		*this=Interval(r.inf(), POS_INFINITY);
		this->NaN = saveNaN||y.NaN;
		return *this;
	}

	*this=Interval(NEG_INFINITY, POS_INFINITY); // a<0<b et c<=0<=d
	this->NaN = saveNaN||y.NaN;
	return *this;

}

/*inline Interval Interval::operator+() const {
	return *this;
}*/

inline Interval Interval:: operator-() const {
	Interval res(-itv);
	res.NaN = this->NaN;
	return res;
}

inline Interval& Interval::div2_inter(const Interval& x, const Interval& y) {
	Interval out2;
	div2_inter(x,y,out2);
	return *this |= out2;
}

inline void Interval::set_empty() {
	bool save = this->NaN; // TODO to check
	*this = Interval::empty_set();
	this->NaN = save;
}

inline Interval& Interval::operator&=(const Interval& x) {
	itv = itv.intersect(x.itv);
	NaN = NaN && x.NaN;
	return *this;
}

inline Interval& Interval::operator|=(const Interval& x) {
	itv=itv.hull(x.itv);
	NaN = NaN || x.NaN;
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
	Interval res= x1.itv.intersect(x2.itv);
	res.NaN = x1.NaN && x2.NaN;
	return res;
}


inline Interval operator|(const Interval& x1, const Interval& x2) {
	Interval res;
	if (x1.is_empty()) res = x2;
	else if (x2.is_empty()) res = x1;
	else res = x1.itv.hull(x2.itv);
	res.NaN = x1.NaN || x2.NaN;
	return res;
}

inline double hausdorff(const Interval &x1, const Interval &x2) {
	return x1.itv.dist(x2.itv);
}

inline Interval operator+(const Interval& x, double d) {
	if(d==NEG_INFINITY || d==POS_INFINITY) {
		Interval res = Interval::empty_set();
		res.NaN = x.NaN;
		return res;
	} else {
		Interval res(x);
		return res+=d;
	}
}

inline Interval operator-(const Interval& x, double d) {
	if(d==NEG_INFINITY || d==POS_INFINITY) {
		Interval res = Interval::empty_set();
		res.NaN = x.NaN;
		return res;
	} else {
		Interval res(x);
		return res-=d;
	}
}

inline Interval operator*(const Interval& x, double d) {
	if(d==NEG_INFINITY || d==POS_INFINITY) {
		Interval res = Interval::empty_set();
		res.NaN = x.NaN;
		return res;
	} else {
		Interval res(d);
		return res*=x;
	}
}

inline Interval operator/(const Interval& x, double d) {
	if(d==NEG_INFINITY || d==POS_INFINITY){
		Interval res = Interval::empty_set();
		res.NaN = x.NaN;
		return res;
	} else {
		Interval res(x);
		return res/=Interval(d);
	}
}

inline Interval operator+(double d,const Interval& x) {
	if(d==NEG_INFINITY || d==POS_INFINITY){
		Interval res = Interval::empty_set();
		res.NaN = x.NaN;
		return res;
	} else {
		Interval res(x);
		return res+=d;
	}
}

inline Interval operator-(double d, const Interval& x) {
	if(d==NEG_INFINITY || d==POS_INFINITY){
		Interval res = Interval::empty_set();
		res.NaN = x.NaN;
		return res;
	} else {
		Interval res(d);
		return res-=x;
	}
}

inline Interval operator*(double d, const Interval& x) {
	if(d==NEG_INFINITY || d==POS_INFINITY){
		Interval res = Interval::empty_set();
		res.NaN = x.NaN;
		return res;
	} else {
		Interval res(d);
		return res*=x;
	}
}

inline Interval operator/(double d, const Interval& x) {
	if(d==NEG_INFINITY || d==POS_INFINITY){
		Interval res = Interval::empty_set();
		res.NaN = x.NaN;
		return res;
	} else {
		Interval res(d);
		return res/=x;
	}
}

inline Interval operator+(const Interval& x1, const Interval& x2) {
	Interval res=x1;
	return res+=x2;
}

inline Interval operator-(const Interval& x1, const Interval& x2) {
	Interval res=x1;
	return res-=x2;
}

inline Interval operator*(const Interval& x1, const Interval& x2) {
	Interval res=x1;
	return res*=x2;
}

inline Interval operator/(const Interval& x1, const Interval& x2) {
	Interval res=x1;
	return res/=x2;
}

inline Interval sqr(const Interval& x) {
	Interval res(filib::sqr(x.itv));
	res.NaN = x.NaN;
	return res;
}

inline Interval sqrt(const Interval& x) {
	Interval res(filib::sqrt(x.itv) & Interval::pos_reals());
	res.NaN = x.NaN || (x.lb()<0);
	return res;
}

inline Interval pow(const Interval& x, int n) {
	Interval res;
	if (n==0)
		res = Interval::one();
	else if (n<0)
		res = 1.0/Interval(filib::power(x.itv,-n));
	else 
		res = filib::power(x.itv,n);
	res.NaN = res.NaN || x.NaN;
	return res;
}

inline Interval pow(const Interval &x, double d) {
	Interval res;
	if(d==NEG_INFINITY || d==POS_INFINITY) {
		res = Interval::empty_set();
	} else if (d==0) {
		res = Interval::one();
	} else if (d<0)
		res = 1.0/pow(x,-d);
	else
		res = pow(x,Interval(d));
	res.NaN = res.NaN || x.NaN;
	return res;
	
}

inline Interval pow(const Interval &x, const Interval &y) {
	Interval res = filib::pow(x.itv, y.itv);
	res.NaN = x.NaN || y.NaN || (x.lb()<0);
	return res;
}



inline Interval root(const Interval& x, int n) {
	Interval res;
	if (x.is_empty()) res= Interval::empty_set();
	else if (x.lb()==0 && x.ub()==0) res = Interval::zero();
	else if (n==0) 		res = Interval::one();
	else if (n<0) 		res = 1.0/root(x,-n);
	else if (n==1) 		res = x;
	else if (n%2==0) {
		res = pow(x,Interval::one()/n);   // the negative part of x should be removed
	} else {
		res = pow(x,Interval::one()/n) |  // the negative part of x should be removed
	    (-pow(-x,Interval::one()/n)); // the positive part of x should be removed
		res.NaN = x.NaN;
	}
	
	res.NaN = res.NaN || x.NaN;
	return res;
}



inline Interval exp(const Interval& x) {
	Interval res = filib::exp(x.itv);
	res.NaN = x.NaN;
	return res;
}

inline Interval log(const Interval& x) {
	if (x.ub()<=0) { // filib returns (-oo,-DBL_MAX) if x.ub()==0, instead of EMPTY_SET
		Interval res = Interval::empty_set(); // TODO to check
		res.NaN = true;
		return res;
	} else if (x.ub()<=next_float(0)) {
		Interval res(NEG_INFINITY,filib::filib_consts<FI_BASE>::q_minr);
		res.NaN = (x.lb()<0);
		return res;
	} else {
		Interval res = filib::log(x.itv);
		res.NaN = (x.lb()<0);
		return res;
	}
}

inline Interval cos(const Interval& x) {
	Interval res = filib::cos(x.itv);
	res.NaN = x.NaN;
	return res;
}

inline Interval sin(const Interval& x) {
	Interval res =filib::sin(x.itv);
	res.NaN = x.NaN;
	return res;
}

inline Interval tan(const Interval& x) {
	Interval res =  filib::tan(x.itv);
	if (res.is_empty()&& (!x.is_empty())) {
		res = Interval::all_reals();
	}
	res.NaN = x.NaN;
	return res;
}

inline Interval acos(const Interval& x) {
	Interval res = filib::acos(x.itv);
	res.NaN = x.NaN;
	return res;
}

inline Interval asin(const Interval& x) {
	Interval res = filib::asin(x.itv);
	res.NaN = x.NaN;
	return res;
}

inline Interval atan(const Interval& x) {
	Interval res = filib::atan(x.itv);
	res.NaN = x.NaN;
	return res;
}

inline Interval cosh(const Interval& x) {
	Interval res = filib::cosh(x.itv);
	res.NaN = x.NaN;
	return res;
}

inline Interval sinh(const Interval& x) {
	Interval res = filib::sinh(x.itv);
	res.NaN = x.NaN;
	return res;
}

inline Interval tanh(const Interval& x) {
	Interval res = filib::tanh(x.itv);
	res.NaN = x.NaN;
	return res;
}

inline Interval acosh(const Interval& x) {
	Interval res = filib::acosh(x.itv);
	res.NaN = x.NaN;
	return res;
}

inline Interval asinh(const Interval& x) {
	Interval res = filib::asinh(x.itv);
	res.NaN = x.NaN;
	return res;
}

inline Interval atanh(const Interval& x) {
	Interval res = filib::atanh(x.itv);
	res.NaN = x.NaN;
	return res;
}

inline Interval abs(const Interval &x) {
	Interval res = filib::abs(x.itv);
	res.NaN = x.NaN;
	return res;
}

inline Interval max(const Interval& x, const Interval& y) {
	Interval res;
	if (x.is_empty()||y.is_empty()) {
		res = Interval::empty_set();
	} else {
		res = x.itv.imax(y.itv);
	}
	res.NaN = x.NaN || y.NaN;
	return res;
}

inline Interval min(const Interval& x, const Interval& y) {
	Interval res;
	if (x.is_empty()||y.is_empty()) {
		res = Interval::empty_set();
	} else {
		res = x.itv.imin(y.itv);
	}
	res.NaN = x.NaN || y.NaN;
	return res;
}

inline Interval integer(const Interval& x) {
	Interval res;
	if (x.is_empty()) {
		res = Interval::empty_set();
	} else {
		double l= x.lb()==NEG_INFINITY? NEG_INFINITY : std::ceil(x.lb());
		double r= x.ub()==POS_INFINITY? POS_INFINITY : std::floor(x.ub());
		if (l>r) {
			res = Interval::empty_set();
		} else {
			res = Interval(l,r);
		}
	}
	res.NaN = x.NaN;
	return res;
}

inline Interval floor(const Interval& x) {
	Interval res;
	if (x.is_empty()) {
		res = Interval::empty_set();
	} else {
		 res = Interval(std::floor(x.lb()),std::floor(x.ub()));
	}
	res.NaN = x.NaN;
	return res;
}

inline Interval ceil(const Interval& x) {
	Interval res;
	if (x.is_empty()) {
		res = Interval::empty_set();
	}
	else {
		res = Interval(std::ceil(x.lb()),std::ceil(x.ub()));
	}
	res.NaN = x.NaN;
	return res;
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
		nb_period = x / Interval::pi(); break;
	case SIN :
		nb_period = (x+Interval::half_pi()) / Interval::pi(); break;
	case TAN :
		nb_period = (x+Interval::half_pi()) / Interval::pi(); break;
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
		while (++i<=p2 && !found) found = !(tmp1 = (x & (i%2==0? period_0 + i*Interval::pi() : (i+1)*Interval::pi() - period_0))).is_empty();
		break;
	case SIN :
		while (++i<=p2 && !found) found = !(tmp1 = (x & (i%2==0? period_0 + i*Interval::pi() : i*Interval::pi() - period_0))).is_empty();
		break;
	case TAN :
		while (++i<=p2 && !found) found = !(tmp1 = (x & (period_0 + i*Interval::pi()))).is_empty();
		break;
	}

	if (!found) { x.set_empty(); return false; }
	found = false;
	i=p2+1;

	switch(ftype) {
	case COS :
		while (--i>=p1 && !found) found = !(tmp2 = (x & (i%2==0? period_0 + i*Interval::pi() : (i+1)*Interval::pi() - period_0))).is_empty();
		break;
	case SIN :
		while (--i>=p1 && !found) found = !(tmp2 = (x & (i%2==0? period_0 + i*Interval::pi() : i*Interval::pi() - period_0))).is_empty();
		break;
	case TAN :
		while (--i>=p1 && !found) found = !(tmp2 = (x & (period_0 + i*Interval::pi()))).is_empty();
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

#endif /* _IBEX_INTERVALLIBWRAPPER_INL_ */
