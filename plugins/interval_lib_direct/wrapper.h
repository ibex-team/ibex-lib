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

#ifndef _IBEX_DIRECT_INTERVAL_H_
#define _IBEX_DIRECT_INTERVAL_H_

#include "ibex_Exception.h"
#include <cassert>
#include <float.h>
#include <iostream>
#include <cmath>
#include <climits>

namespace ibex {

inline void fpu_round_down() {
}

inline void fpu_round_up() {
}

inline void fpu_round_near() {
}

inline double previous_float_mod(double x) {
	if (x<0) return (1.0+DBL_EPSILON)*x;
	else return (1.0-DBL_EPSILON)*x;
}

inline double next_float_mod(double x) {
	if (x<0) return (1.0-DBL_EPSILON)*x;
	else return (1.0+DBL_EPSILON)*x;
}

inline double previous_float(double x) {
	if (x==POS_INFINITY)	return DBL_MAX;
	if (x==NEG_INFINITY)	return x;
	else if (x==0) return -DBL_MIN;
	else return previous_float_mod( x);
}

inline double next_float(double x) {
	if (x==POS_INFINITY)	return x;
	if (x==NEG_INFINITY)	return -DBL_MAX;
	else if (x==0) return DBL_MIN;
	else return next_float_mod( x);
}

inline double previous_exact(double x) {
	if (((int)x)==x ) 	return x;
	else 				return previous_float(x);
}


inline double next_exact(double x) {
	if (((int)x)==x ) 	return x;
	else 				return next_float(x);
}



inline Interval::Interval(const DIRECT_INTERVAL& x) : itv(x) {

}

inline Interval& Interval::operator=(const DIRECT_INTERVAL& x) {
	this->itv = x;
	return *this;
}

inline Interval& Interval::operator+=(double d) {
	if (!is_empty()) {
		if (d==NEG_INFINITY || d==POS_INFINITY) set_empty();
		else if (d!=0)  *this=Interval(DIRECT_INTERVAL(previous_exact(lb()+d),next_exact(ub()+d)));
	}
	return *this;
}

inline Interval& Interval::operator-=(double d) {
	if (!is_empty()) {
		if (d==NEG_INFINITY || d==POS_INFINITY) set_empty();
		else if (d!=0) *this=Interval(DIRECT_INTERVAL(previous_exact(lb()-d),next_exact(ub()-d)));
	}
	return *this;
}

inline Interval& Interval::operator*=(double d) {
	return ((*this)*=Interval(d));
}

inline Interval& Interval::operator/=(double d) {
	return ((*this)/=Interval(d));
}

inline Interval& Interval::operator+=(const Interval& x) {
	if (is_empty()) return *this;
	else if (x.is_empty()) {
		set_empty();
		return *this;
	} else {
		*this=Interval(DIRECT_INTERVAL(previous_exact(lb()+x.lb()),next_exact(ub()+x.ub())));
		return *this;
	}
}

inline Interval& Interval::operator-=(const Interval& x) {
	if (is_empty()) return *this;
	else if (x.is_empty()) {
		set_empty();
		return *this;
	} else {
		*this=Interval(DIRECT_INTERVAL(previous_exact(lb()-x.ub()),next_exact(ub()-x.lb())));
		return *this;
	}
}

/*
 *  Multiplication
 *
 * \note Some situations where Bias/Profil may fail (underscore stands for any value): <ul>
 *  <li> <tt>[0,0] * [-inf,_]</tt>
 *  <li> <tt> [0,0] * [_,inf] </tt>
 *  <li> <tt> [-inf,_] * [_,0] </tt>
 *  <li> <tt> [_,inf] * [_,0] </tt>
 *  <li> <tt> [-0,_] * [_,inf] </tt>
 *  <li> <tt> [inf([0,0])*-1,1] * [1,inf] </tt>

 *  <li> <tt> [inf([0,0])*-1,1] * [-inf,1] </tt>
 *  </ul> */
inline Interval& Interval::operator*=(const Interval& y) {


	if (is_empty()) return *this;
	if (y.is_empty()) { *this=Interval::EMPTY_SET; return *this; }

	const double& a(lb());
	const double& b(ub());
	const double& c(y.lb());
	const double& d(y.ub());

	if ((a==0 && b==0) || (c==0 && d==0)) { *this=Interval(0.0); return *this; }

	if (((a<0) && (b>0)) && (c==NEG_INFINITY || d==POS_INFINITY)) { *this=Interval(NEG_INFINITY, POS_INFINITY); return *this; }

	if (((c<0) && (d>0)) && (a==NEG_INFINITY || b==POS_INFINITY)) { *this=Interval(NEG_INFINITY, POS_INFINITY); return *this; }

	// [-inf, _] x [_ 0] ou [0,_] x [_, +inf]
	if (((a==NEG_INFINITY) && (d==0)) || ((d==POS_INFINITY) && (a==0))) {
		if ((b<=0) || (c>=0)) { *this=Interval(0.0, POS_INFINITY); return *this; }
		else {
			*this=Interval(previous_exact(b*c), POS_INFINITY);
			return *this;
		}
	}

	// [-inf, _] x [0, _] ou [0, _] x [-inf, _]
	if (((a==NEG_INFINITY) && (c==0)) || ((c==NEG_INFINITY) && (a==0))) {
		if ((b<=0) || (d<=0)) { *this=Interval(NEG_INFINITY, 0.0); return *this; }
		else {
			*this=Interval(NEG_INFINITY,next_exact(b*d));
			return *this;
		}
	}

	// [_,0] x [-inf, _] ou [_, +inf] x [0,_]
	if (((c==NEG_INFINITY) && (b==0)) || ((b==POS_INFINITY) && (c==0))) {
		if ((d<=0) || (a>=0)) { *this=Interval(0.0, POS_INFINITY); return *this; }
		else {
			*this=Interval(previous_exact(a*d), POS_INFINITY);
			return *this;
		}
	}

	// [_, +inf] x [_,0] ou [_,0] x [_, +inf]
	if (((b==POS_INFINITY) && (d==0)) || ((d==POS_INFINITY) && (b==0))) {
		if ((a>=0) || (c>=0)) { *this=Interval(NEG_INFINITY, 0.0); return *this; }
		else {
			*this=Interval(NEG_INFINITY, next_exact(a*c));
			return *this;
		}
	}

	if (a>=0)	{
		if  (c>=0) {
			*this= Interval(DIRECT_INTERVAL(previous_exact(a*c),next_exact(b*d)));
		}
		else {
			if  (d<=0) *this=Interval(DIRECT_INTERVAL(previous_exact(b*c),next_exact(a*d)));
			else *this=  Interval(DIRECT_INTERVAL(previous_exact(b*c),next_exact(b*d)));
		}
	} else {
		if (b<=0)	{
			if  (c>=0) *this=Interval(DIRECT_INTERVAL(previous_exact(a*d),next_exact(b*c)));
			else if (d<=0) *this=Interval(DIRECT_INTERVAL(previous_exact(b*d),next_exact(a*c)));
			else *this=Interval(DIRECT_INTERVAL(previous_exact(a*d),next_exact(a*c)));
		} else {
			if (c>=0) *this=Interval(DIRECT_INTERVAL(previous_exact(a*d),next_exact(b*d)));
			else if (d<=0) *this=Interval(DIRECT_INTERVAL(previous_exact(b*c),next_exact(a*c)));
			else {
				*this=Interval( DIRECT_INTERVAL(
						previous_exact((a*d<b*c) ? a*d : b*c ),
						next_exact((a*c<b*d) ? b*d : a*c)));
			}
		}

	}
	return *this;
}

inline Interval& Interval::operator/=(const Interval& y) {

	if (is_empty()) return *this;
	if (y.is_empty()) { *this=Interval::EMPTY_SET; return *this; }

	const double& a(lb());
	const double& b(ub());
	const double& c(y.lb());
	const double& d(y.ub());


	if (c==0 && d==0) {
		*this=Interval::EMPTY_SET;
		return *this;
	}

	if (a==0 && b==0) {
		// TODO: 0/0 can also be 1...
		return *this;
	}

	if (c> 0)	{
		if    (a>=0)  {
			*this= Interval(DIRECT_INTERVAL(previous_exact(a/d), next_exact(b/c)));
		}
		else if (b<0)  {
			*this= Interval(DIRECT_INTERVAL(previous_exact(a/c), next_exact(b/d)));
		}
		else  {
			*this= Interval(DIRECT_INTERVAL(previous_exact(a/c), next_exact(b/c)));
		}
		return *this;
	}

	if (d<0)	{
		if (a>=0) {
			*this= Interval(DIRECT_INTERVAL(previous_exact(b/d), next_exact(a/c)));
		}
		else if (b<0)  {
			*this= Interval(DIRECT_INTERVAL(previous_exact(b/c), next_exact(a/d)));
		}
		else {
			*this= Interval(DIRECT_INTERVAL(previous_exact(b/d), next_exact(a/d)));
		}
		return *this;
	}

	if ((b<=0) && d==0) {
		*this=Interval(previous_exact(b/c), POS_INFINITY);
		return *this;
	}

	if (b<=0 && c<0 && d<0) {
		*this=Interval(NEG_INFINITY, POS_INFINITY);
		return *this;
	}

	if (b<=0 && c==0) {
		*this=Interval(NEG_INFINITY, next_exact(b/d));
		return *this;
	}

	if (a>=0 && d==0) {
		*this=Interval(NEG_INFINITY, next_exact(a/c));
		return *this;
	}

	if (a>=0 && c<0 && d>0) {
		*this=Interval(NEG_INFINITY, POS_INFINITY);
		return *this;
	}

	if (a>=0 && c==0) {
		*this=Interval(previous_exact(a/d), POS_INFINITY);
		return *this;
	}

	*this=Interval(NEG_INFINITY, POS_INFINITY); // a<0<b et c<=0<=d
	return *this;
}

inline Interval Interval:: operator-() const {
	if (is_empty()) return *this;
	return Interval(DIRECT_INTERVAL(-ub(),-lb()));
}

inline Interval& Interval::div2_inter(const Interval& x, const Interval& y) {
	Interval out2;
	div2_inter(x,y,out2);
	*this |= out2;
	return *this;
}

inline void Interval::set_empty() {
	*this=EMPTY_SET;
}

inline Interval& Interval::operator&=(const Interval& x) {

	if (is_empty()) return *this;
	if (x.is_empty()) { set_empty(); return *this; }

	if ((this->lb()>x.ub())||(x.lb()>this->ub())) {
		set_empty();
		return *this;
	}
	DIRECT_INTERVAL r(0);
	if (this->lb()<=x.lb()) r.inf=x.lb(); else r.inf=this->lb();
	if (this->ub()>=x.ub()) r.sup=x.ub(); else r.sup=this->ub();
	this->itv=r;

	return *this;

}

inline Interval& Interval::operator|=(const Interval& x) {

	if (is_empty()) { *this=x;	return *this; }
	if (x.is_empty()) return *this;

	DIRECT_INTERVAL r(0);
	if (this->lb()<=x.lb()) r.inf=this->lb(); else r.inf=x.lb();
	if (this->ub()>=x.ub()) r.sup=this->ub(); else r.sup=x.ub();
	this->itv=r;

	return *this;
}

inline double Interval::lb() const {
	return itv.inf;
}

inline double Interval::ub() const {
	return itv.sup;
}

inline double Interval::mid() const {
	if (lb()==NEG_INFINITY)
		if (ub()==POS_INFINITY) return 0;
		else return -DBL_MAX;
	else if (ub()==POS_INFINITY) return DBL_MAX;
	else if (lb()==0) {
		if (ub()==DBL_MIN) return 0;
		else return ub()/2;
	}
	else if (ub()==0) {
		if (lb()==-DBL_MIN) return 0;
		else return lb()/2;
	}
	else {
		double m=(lb()-(lb()-ub())/2); // better way to compute the middle, reduce the number of overfloat
		if (m<lb()) m=lb(); // watch dog
		else if (m>ub()) m=ub();
		return m;
	}
}

inline bool Interval::is_empty() const {
	return itv.isEmpty; //*this==EMPTY_SET;
}

inline bool Interval::is_degenerated() const {
	return is_empty() || lb()==ub();
}

inline bool Interval::is_unbounded() const {
	if (is_empty()) return false;
	return lb()==NEG_INFINITY || ub()==POS_INFINITY;
}

inline double Interval::diam() const {
	return is_empty()? 0: (is_unbounded()? POS_INFINITY : ((itv.sup-itv.inf)));
}

inline double Interval::mig() const {
	if (lb()>0)      return lb();
	else if (ub()<0) return -ub();
	else             return 0;
}

inline double Interval::mag() const {
    return  (fabs(lb())> fabs(ub())) ? fabs(lb()) : fabs(ub());
}

inline Interval operator&(const Interval& x1, const Interval& x2) {
	Interval res(x1);
	res &= x2;
	return res;
}

inline Interval operator|(const Interval& x1, const Interval& x2) {
	Interval res(x1);
	res |= x2;
	return res;
}

inline Interval operator+(const Interval& x, double d) {
	if (x.is_empty()) return x;
	else if (d==NEG_INFINITY || d==POS_INFINITY) return Interval::EMPTY_SET;
	else {
		Interval r(x);
		r += d;
		return r;
	}
}


inline Interval operator-(const Interval& x, double d) {
	if (x.is_empty()) return x;
		else if (d==NEG_INFINITY || d==POS_INFINITY) return Interval::EMPTY_SET;
	else {
		Interval r(x);
		r -= d;
		return r;
	}
}

inline Interval operator*(const Interval& x, double d) {
	if (x.is_empty()) return x;
		else if (d==NEG_INFINITY || d==POS_INFINITY) return Interval::EMPTY_SET;
	else {
		Interval r(x);
		r *= d;
		return r;
	}
}

inline Interval operator/(const Interval& x, double d) {
	if (x.is_empty()) return x;
		else if (d==NEG_INFINITY || d==POS_INFINITY) return Interval::EMPTY_SET;
	else {
		Interval r(x);
		r /= d;
		return r;
	}
}

inline Interval operator+(double d,const Interval& x) {
	return x+d;
}

inline Interval operator-(double d, const Interval& x) {
	if (x.is_empty()) return x;
		else if (d==NEG_INFINITY || d==POS_INFINITY) return Interval::EMPTY_SET;
	else {
		Interval r(d);
		r -= x;
		return r;
	}
}

inline Interval operator*(double d, const Interval& x) {
	return x*d;
}

inline Interval operator/(double d, const Interval& x) {
	/*if (x.is_empty()) return x;
	else if (d==0 || d==NEG_INFINITY || d==POS_INFINITY) return Interval::EMPTY_SET;
	else*/
	return Interval(d)/x.itv;
}

inline Interval operator+(const Interval& x1, const Interval& x2) {
	if (x1.is_empty() || x2.is_empty())
		return Interval::EMPTY_SET;
	else {
		Interval r(x1);
		r += x2;
		return r;
	}
}

inline Interval operator-(const Interval& x1, const Interval& x2) {
	if (x1.is_empty() || x2.is_empty())
		return Interval::EMPTY_SET;
	else{
		Interval r(x1);
		r -= x2;
		return r;
	}
}

/*
 *  Multiplication
 *
 * \note Some situations where Bias/Profil may fail (underscore stands for any value): <ul>
 *  <li> <tt>[0,0] * [-inf,_]</tt>
 *  <li> <tt> [0,0] * [_,inf] </tt>
 *  <li> <tt> [-inf,_] * [_,0] </tt>
 *  <li> <tt> [_,inf] * [_,0] </tt>
 *  <li> <tt> [-0,_] * [_,inf] </tt>
 *  <li> <tt> [inf([0,0])*-1,1] * [1,inf] </tt>

 *  <li> <tt> [inf([0,0])*-1,1] * [-inf,1] </tt>
 *  </ul> */
inline Interval operator*(const Interval& x, const Interval& y) {
	return (Interval(x)*=y);
}

inline Interval operator/(const Interval& x, const Interval& y) {
	return (Interval(x)/=y);
}

inline Interval sqr(const Interval& x) {
	if (x.is_empty()) return Interval::EMPTY_SET;
	else {
		double a1=x.lb(), a2=x.ub();
		if ((a1>=0)||(a2<=0))   return Interval(DIRECT_INTERVAL(previous_exact(a1*a1),next_exact(a2*a2)));
		if (fabs(a1)>fabs(a2))  return Interval(0,next_exact(a1*a1));
		else                    return Interval(0,next_exact(a2*a2));
	}
}

inline Interval sqrt(const Interval& x) {
	if (x.is_empty()) return Interval::EMPTY_SET;
	if (x.ub()<0) return Interval::EMPTY_SET;
	if (x.lb()>=0)  return (Interval(previous_exact(::sqrt(x.lb())),next_exact(::sqrt(x.ub()))));
	else return (Interval(0,next_exact(::sqrt(x.ub()))));
}

inline Interval pow(const Interval& x, int n) {
	if (x.is_empty()) return Interval::EMPTY_SET;
	else if (n==0)	  return Interval::ONE;
	else if (n<0)	  return 1.0/pow(x,-n);
	else if (n==1)	  return x;
	else if (n%2!=0) {
	// there are bugs with negative x
	// such as Power([-2,3],3)->[-18,27])
		if (x.ub()<0)
			return -pow(-x,n);
		else if (x.lb()<0)
			return Interval(DIRECT_INTERVAL(previous_exact(-(::pow(-x.lb(),n))),next_exact(::pow(x.ub(),n))));
		else
			return Interval(DIRECT_INTERVAL(previous_exact((::pow(x.lb(),n))),next_exact((::pow(x.ub(),n)))));
	}
	else {
		double a1, a2;
		if (fabs(x.lb())> fabs(x.ub())) {
			a1 = x.ub() ; a2 = x.lb();
		} else {
			a1 = x.lb() ; a2 = x.ub();
		}
		if (a1*a2<0) return Interval(0,::pow(a2,n));
		else return Interval(DIRECT_INTERVAL(previous_exact(::pow(a1,n)),next_exact(::pow(a2,n))));
	}
}

inline Interval pow(const Interval& x, double d) {
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
	if (x.is_empty()) return Interval::EMPTY_SET;
	else return exp(y * log(x));
}

inline Interval root(const Interval& x, int den) {
	if (x.is_empty()) return Interval::EMPTY_SET;
	if (den>0) {
		double m=den;
		if (den % 2 == 0) {
			if (x.lb()*x.ub()<=0){
				return Interval(DIRECT_INTERVAL(0,next_exact(::pow(x.ub(),1/m))));
			} else {
				return Interval(DIRECT_INTERVAL(previous_exact(::pow(x.lb(),1/m)),next_exact(::pow(x.ub(),1/m))));
			}
		} else {
			double a1, a2, tmp;
			a1 = fabs(x.lb()) ; a2 = fabs(x.ub());
			if (x.lb()*x.ub()<=0){
				return Interval(DIRECT_INTERVAL(previous_exact(-::pow(a1,1/m)),next_exact(::pow(a2,1/m))));
			} else if (x.lb() >=0 ) {
				return Interval(DIRECT_INTERVAL(previous_exact(::pow(a1,1/m)),next_exact(::pow(a2,1/m))));
			} else {
				return Interval(DIRECT_INTERVAL(previous_exact(-::pow(a1,1/m)),next_exact(-::pow(a2,1/m))));
			}
		}
	}
	else return Interval(1.0)/root(x,-den);
}

inline Interval exp(const Interval& x) {
	if (x.is_empty()) return Interval::EMPTY_SET;

	// [gch] This block is unecessary I think
	else if (::exp(x.lb())>= DBL_MAX)
		return Interval(DBL_MAX,POS_INFINITY);
	else if (::exp(x.ub())==POS_INFINITY)
		return Interval(previous_exact(::exp(x.lb())),POS_INFINITY);
	// =======================================

	else return Interval(DIRECT_INTERVAL(previous_exact(::exp(x.lb())),next_exact(::exp(x.ub()))));
}

inline Interval log(const Interval& x) {
	if (x.is_empty() || x.ub()<=0)
		return Interval::EMPTY_SET;
	else {
		Interval b(abs(x));
		if (x.lb()<0) return Interval(NEG_INFINITY,next_exact(::log(x.ub())));
		return Interval(DIRECT_INTERVAL(previous_exact(::log(b.lb())),next_exact(::log(b.ub()))));
	}
}

inline Interval cos(const Interval& x) {
	if (x.is_empty()) return Interval::EMPTY_SET;
	else return sin(x+Interval::HALF_PI);
}

inline Interval sin(const Interval& x) {
	if (x.is_empty()) return Interval::EMPTY_SET;
    if (x.ub()==x.lb()) return Interval(::sin(x.lb()));
    if (x.diam()>Interval::TWO_PI.lb()) return (Interval(-1,1));

    Interval b;
    //  b = Modulo(x,Interval::TWO_PI.lb());
    if ((x.lb()>=0)&&(x.lb()<Interval::TWO_PI.ub())) {
    	b= x;
    } else {
    	int k = (long)floorl((x.lb()/Interval::TWO_PI.ub()));
    	double offset = Interval::TWO_PI.ub() * k;
    	b =Interval(x.lb()-offset,x.ub()-offset);
    }

    double sin1,sin2,r1,r2;
    sin1=::sin(b.lb());   sin2=::sin(b.ub());
    if ((b.lb() < 3*Interval::HALF_PI.ub())&&(b.ub() > 3*Interval::HALF_PI.ub())) r1=-1.0;
    else if ((b.lb() < 7*Interval::HALF_PI.ub())&&(b.ub() > 7*Interval::HALF_PI.ub())) r1=-1.0;
    else r1=((sin1 < sin2)? sin1 : sin2);
    if ((b.lb() < Interval::HALF_PI.ub())&&(b.ub() > Interval::HALF_PI.ub())) r2=1.0;
    else if ((b.lb() < 5*Interval::HALF_PI.ub())&&(b.ub() > 5*Interval::HALF_PI.ub())) r2=1.0;
    else r2=((sin1 > sin2)? sin1 : sin2);
    return (Interval(-1,1) & Interval(DIRECT_INTERVAL(previous_exact(r1),next_exact(r2))));
}

inline Interval tan(const Interval& x) {
	if (x.is_empty()) return Interval::EMPTY_SET;

    if (x.ub()==x.lb()) {
    	if (x.lb()==Interval::HALF_PI.ub()||x.lb()==-Interval::HALF_PI.ub()) {
    		return Interval::ALL_REALS;
    	}
    	return Interval(::tan(x.lb()));
    }
    if (x.diam()>Interval::PI.lb()) return Interval::ALL_REALS;

    Interval b;
    //  b = Modulo(x,Interval::PI.lb());
    if ((x.lb()>=0)&&(x.lb()<Interval::PI.ub())) {
    	b= x;
    } else {
    	int k = (long)floorl((x.lb()/Interval::PI.ub()));
    	double offset = Interval::PI.ub() * k;
    	b =Interval(x.lb()-offset,x.ub()-offset);
    }

    // degenerated case
    if (b.ub()==b.lb()) {
    	if (b.lb()==Interval::HALF_PI.ub()||b.lb()==-Interval::HALF_PI.ub()) {
    		return Interval::ALL_REALS;
    	}
    	else return Interval(::tan(b.lb()));
    }
    // particular case
    if (b.lb()<=Interval::HALF_PI.ub() && Interval::HALF_PI.ub()<=b.ub() )
    	return Interval::ALL_REALS;
    if (b.lb()<=-Interval::HALF_PI.lb() && -Interval::HALF_PI.lb()<=b.ub() )
    	return Interval::ALL_REALS;

    // general case
    return Interval(DIRECT_INTERVAL(previous_exact(::tan(b.lb())), next_exact(::tan(b.ub()))));

}

inline Interval cosh(const Interval& x) {
	if (x.is_empty()) return Interval::EMPTY_SET;
	else if (x.ub()==POS_INFINITY) {
		if (x.lb()<=0) return Interval(1,POS_INFINITY);
		else return Interval((::cosh(x.lb())),POS_INFINITY);
	}
	else if (x.lb()==NEG_INFINITY) {
		if (x.ub()>=0) return Interval(1,POS_INFINITY);
		else return Interval((::cosh(x.ub())),POS_INFINITY);
	}
	else if (x.lb()>=0)
		return Interval(DIRECT_INTERVAL((::cosh(x.lb())),next_exact(::cosh(x.ub()))));
	else if (x.ub()<=0)
		return Interval(DIRECT_INTERVAL((::cosh(x.ub())),next_exact(::cosh(x.lb()))));
	else
		return ((fabs(x.lb())> fabs(x.ub())) ? Interval(1,next_exact(::cosh(x.lb()))) :Interval(1,next_exact(::cosh(x.ub()))));
}

inline Interval acos(const Interval& x) {
	if (x.is_empty()||x.ub()<-1.0 || x.lb()>1.0) return Interval::EMPTY_SET;
	else {
		return Interval(DIRECT_INTERVAL((x.ub()>=1)? 0.0 : previous_exact(::acos(x.ub())), (x.lb()<=-1) ? Interval::PI.ub() : next_exact(::acos(x.lb()))));
	}

}

inline Interval asin(const Interval& x) {
	if (x.is_empty()||x.ub()<-1.0 || x.lb()>1.0) return Interval::EMPTY_SET;
	else {
		return Interval(DIRECT_INTERVAL((x.lb()<-1)? (-Interval::HALF_PI).lb() : previous_exact(::asin(x.lb())), (x.ub()>1) ? Interval::HALF_PI.ub() : next_exact(::asin(x.ub()))));
	}
}

inline Interval atan(const Interval& x) {
	if (x.is_empty()) return Interval::EMPTY_SET;
	else return Interval(DIRECT_INTERVAL(previous_exact(::atan(x.lb())), next_exact(::atan(x.ub()))));
}

inline Interval sinh(const Interval& x) {
	if (x.is_empty()) return Interval::EMPTY_SET;
	else if (x.lb()==NEG_INFINITY) {
		if (x.ub()==POS_INFINITY) return Interval::ALL_REALS;
		else return Interval(NEG_INFINITY, next_exact(::sinh(x.ub())));
	}
	else if (x.ub()==POS_INFINITY)
		return Interval(::sinh(x.lb()),POS_INFINITY);
	else
		return Interval(DIRECT_INTERVAL(previous_exact(::sinh(x.lb())), next_exact(::sinh(x.ub()))));
}

inline Interval tanh(const Interval& x) {
	if (x.is_empty()) return Interval::EMPTY_SET;

	if (x.lb()==NEG_INFINITY) {
		if (x.ub()==POS_INFINITY) return Interval(-1, 1);
		else {
			return Interval(-1,next_exact(::tanh(x.ub())));
		}
	} else {
		if (x.ub()==POS_INFINITY) {
			return Interval(::tanh(x.lb()),1);
		}
		else {
			return  Interval(-1, 1) & Interval(DIRECT_INTERVAL(previous_exact(::tanh(x.lb())), next_exact(::tanh(x.ub()))));
		}
	}
}

inline Interval acosh(const Interval& x) {
	if (x.is_empty()) return Interval::EMPTY_SET;
	if (x.ub()<1.0) return Interval::EMPTY_SET;

	return Interval(DIRECT_INTERVAL(previous_exact(::acosh((x.lb()<1) ? 1 : x.lb())),next_exact(::acosh(x.ub()))));

}

inline Interval asinh(const Interval& x) {
	if (x.is_empty()) return Interval::EMPTY_SET;

	if (x.lb()==NEG_INFINITY) {
		if (x.ub()==POS_INFINITY) return Interval::ALL_REALS;
		else {
			return Interval(NEG_INFINITY, ::asinh(x.ub()));
		}
	} else {
		if (x.ub()==POS_INFINITY) {
			return Interval(::asinh(x.lb()),POS_INFINITY);
		}
		else {
			return Interval(DIRECT_INTERVAL(previous_exact(::asinh(x.lb())),next_exact(::asinh(x.ub()))));
		}
	}
}

inline Interval atanh(const Interval& x) {
	if (x.is_empty()) return Interval::EMPTY_SET;


	if (x.ub()<-1.0 || x.lb()>1.0)
		return Interval::EMPTY_SET;
	else {
		if (x.lb()<=-1) {
			if (x.ub()>=1)
				return Interval::ALL_REALS;
			else {
				return Interval(NEG_INFINITY,next_exact(::atanh(x.ub())));
			}
		} else {
			if (x.ub()>=1) {
				return Interval(::atanh(x.lb()), POS_INFINITY);
			} else {
				return Interval(DIRECT_INTERVAL(previous_exact(::atanh(x.lb())),next_exact(::atanh(x.ub()))));
			}
		}
	}

}

inline Interval abs(const Interval &x) {
	if (x.is_empty()) return Interval::EMPTY_SET;
	else {
		double a1=x.lb(), a2=x.ub();
		if ((a1>=0)||(a2<=0))   return Interval(DIRECT_INTERVAL(fabs(a1),fabs(a2)));
		if (fabs(a1)>fabs(a2))  return Interval(0,fabs(a1));
		else                    return Interval(0,fabs(a2));
	}
}

inline Interval max(const Interval& x, const Interval& y) {
	if (x.is_empty() || y.is_empty()) return Interval::EMPTY_SET;
	else return Interval(x.lb()>y.lb()? x.lb() : y.lb(), x.ub()>y.ub()? x.ub() : y.ub());
}

inline Interval min(const Interval& x, const Interval& y) {
	if (x.is_empty() || y.is_empty()) return Interval::EMPTY_SET;
	else return Interval(x.lb()<y.lb()? x.lb() : y.lb(), x.ub()<y.ub()? x.ub() : y.ub());
}

inline Interval integer(const Interval& x) {
	if (x.is_empty()) return Interval::EMPTY_SET;
	double l= (x.lb()==NEG_INFINITY? NEG_INFINITY : ceil(x.lb()));
	double r= (x.ub()==POS_INFINITY? POS_INFINITY : floor(x.ub()));
	if (l>r) return Interval::EMPTY_SET;
	else return Interval(l,r);
}

inline bool bwd_mul(const Interval& y, Interval& x1, Interval& x2) {
	if (y.contains(0)) {
		if (!x2.contains(0))                           // if y and x2 contains 0, x1 can be any double number.
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

		return !x.is_empty();

	} else {

		x &= root(y, expon);
		return !x.is_empty();

	}
}

inline bool bwd_pow(const Interval& , Interval& , Interval& ) {
	not_implemented("warning: bwd_power(y,x1,x2) (with x1 and x2 intervals) not implemented yet with DIRECT");
	return true;
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

#endif // _IBEX_DIRECT_INTERVAL_H_
