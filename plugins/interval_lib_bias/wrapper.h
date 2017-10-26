/* ============================================================================
 * I B E X - Implementation of the Interval class based on Profil/Bias
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Jan 10, 2013
 * ---------------------------------------------------------------------------- */

#ifndef _IBEX_BIAS_INTERVAL_H_
#define _IBEX_BIAS_INTERVAL_H_

#include "BIAS/BiasF.h"
#include "Functions.h"
#include <float.h>
#include <cassert>
#include <iostream>
#include <climits>

namespace ibex {

inline void fpu_round_down() {
	BiasRoundDown();
}

inline void fpu_round_up() {
	BiasRoundUp();
}

inline void fpu_round_near() {
	BiasRoundNear();
}

inline double previous_float(double x) {
    if (x==POS_INFINITY) return DBL_MAX;
	else return Pred(x);
}

inline double next_float(double x) {
    if (x==NEG_INFINITY) return -DBL_MAX;
	else return Succ(x);
}

inline Interval::Interval(const INTERVAL& x) : itv(x) {

}

inline Interval& Interval::operator=(const INTERVAL& x) {
	this->itv = x;
	return *this;
}

inline Interval& Interval::operator+=(double d) {
	if (!is_empty()) {
		if (d==NEG_INFINITY || d==POS_INFINITY) *this=EMPTY_SET;
		else itv+=d;
	}
	return *this;
}

inline Interval& Interval::operator-=(double d) {
	if (!is_empty()) {
		if (d==NEG_INFINITY || d==POS_INFINITY) *this=EMPTY_SET;
		else itv-=d;
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
	else if (x.is_empty()) { *this=Interval::EMPTY_SET; return *this; }
	else { itv+=x.itv; return *this; }
}

inline Interval& Interval::operator-=(const Interval& x) {
	if (is_empty()) return *this;
	else if (x.is_empty()) { *this=Interval::EMPTY_SET; return *this; }
	else { itv-=x.itv; return *this; }
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

	INTERVAL r;

	if (is_empty()) return *this;
	if (y.is_empty()) { *this=Interval::EMPTY_SET; return *this; }

	const REAL& a(lb());
	const REAL& b(ub());
	const REAL& c(y.lb());
	const REAL& d(y.ub());

	if ((a==0 && b==0) || (c==0 && d==0)) { *this=Interval(0.0,0.0); return *this; }

	if (((a<0) && (b>0)) && (c==NEG_INFINITY || d==POS_INFINITY)) { *this=Interval(NEG_INFINITY, POS_INFINITY); return *this; }

	if (((c<0) && (d>0)) && (a==NEG_INFINITY || b==POS_INFINITY)) { *this=Interval(NEG_INFINITY, POS_INFINITY); return *this; }

	// [-inf, _] x [_ 0] ou [0,_] x [_, +inf]
	if (((a==NEG_INFINITY) && (d==0)) || ((d==POS_INFINITY) && (a==0))) {
		if ((b<=0) || (c>=0)) { *this=Interval(0.0, POS_INFINITY); return *this; }
		else {
			BiasMulII (Bias(r), Bias(INTERVAL(b)), Bias(INTERVAL(c)));
			*this=Interval(Inf(r), POS_INFINITY);
			return *this;
		}
	}

	// [-inf, _] x [0, _] ou [0, _] x [-inf, _]
	if (((a==NEG_INFINITY) && (c==0)) || ((c==NEG_INFINITY) && (a==0))) {
		if ((b<=0) || (d<=0)) { *this=Interval(NEG_INFINITY, 0.0); return *this; }
		else {
			BiasMulII (Bias(r), Bias(INTERVAL(b)), Bias(INTERVAL(d)));
			*this=Interval(NEG_INFINITY, Sup(r));
			return *this;
		}
	}

	// [_,0] x [-inf, _] ou [_, +inf] x [0,_]
	if (((c==NEG_INFINITY) && (b==0)) || ((b==POS_INFINITY) && (c==0))) {
		if ((d<=0) || (a>=0)) { *this=Interval(0.0, POS_INFINITY); return *this; }
		else {
			BiasMulII (Bias(r), Bias(INTERVAL(a)), Bias(INTERVAL(d)));
			*this=Interval(Inf(r), POS_INFINITY);
			return *this;
		}
	}

	// [_, +inf] x [_,0] ou [_,0] x [_, +inf]
	if (((b==POS_INFINITY) && (d==0)) || ((d==POS_INFINITY) && (b==0))) {
		if ((a>=0) || (c>=0)) { *this=Interval(NEG_INFINITY, 0.0); return *this; }
		else {
			BiasMulII (Bias(r), Bias(INTERVAL(a)), Bias(INTERVAL(c)));
			*this=Interval(NEG_INFINITY, Sup(r));
			return *this;
		}
	}

	BiasMulII (Bias(r), Bias(itv), Bias(y.itv));
	*this=r;
	return *this;
}

inline Interval& Interval::operator/=(const Interval& y) {

	if (is_empty()) return *this;
	if (y.is_empty()) { *this=Interval::EMPTY_SET; return *this; }

	const REAL& a(lb());
	const REAL& b(ub());
	const REAL& c(y.lb());
	const REAL& d(y.ub());

	INTERVAL r;

	if (c==0 && d==0) {
		*this=Interval::EMPTY_SET;
		return *this;
	}

	if (a==0 && b==0) {
		// TODO: 0/0 can also be 1...
		return *this;
	}

	if (c>0 || d<0) {
		BiasDivII (Bias(r), Bias(itv), Bias(y.itv));
		*this=r;
		return *this;
	}

	if ((b<=0) && d==0) {
		BiasDivII (Bias(r), Bias(INTERVAL(b)), Bias(INTERVAL(c)));
		*this=Interval(Inf(r), POS_INFINITY);
		return *this;
	}

	if (b<=0 && c<0 && d<0) {
		*this=Interval(NEG_INFINITY, POS_INFINITY);
		return *this;
	}

	if (b<=0 && c==0) {
		BiasDivII (Bias(r), Bias(INTERVAL(b)), Bias(INTERVAL(d)));
		*this=Interval(NEG_INFINITY, Sup(r));
		return *this;
	}

	if (a>=0 && d==0) {
		BiasDivII (Bias(r), Bias(INTERVAL(a)), Bias(INTERVAL(c)));
		*this=Interval(NEG_INFINITY, Sup(r));
		return *this;
	}

	if (a>=0 && c<0 && d>0) {
		*this=Interval(NEG_INFINITY, POS_INFINITY);
		return *this;
	}

	if (a>=0 && c==0) {
		BiasDivII (Bias(r), Bias(INTERVAL(a)), Bias(INTERVAL(d)));
		*this=Interval(Inf(r), POS_INFINITY);
		return *this;
	}

	*this=Interval(NEG_INFINITY, POS_INFINITY); // a<0<b et c<=0<=d
	return *this;
}

inline Interval Interval:: operator-() const {
	if (is_empty()) return *this;
	return -itv;
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

	INTERVAL tmp;
	if (Intersection(tmp, itv, x.itv)) itv = tmp;
	else set_empty();
	return *this;
}

inline Interval& Interval::operator|=(const Interval& x) {
	if (!x.is_empty()) {
		if (is_empty()) itv = x.itv;
		else itv = Hull(itv, x.itv);
	}
	return *this;
}

inline double Interval::lb() const {
	return Inf(itv);
}

inline double Interval::ub() const {
	return Sup(itv);
}

inline double Interval::mid() const {
	if (Inf(itv)==NEG_INFINITY)
		if (Sup(itv)==POS_INFINITY) return 0;
		else return -DBL_MAX;
	else if (Sup(itv)==POS_INFINITY) return DBL_MAX;
	else {
		double m=Mid(itv);
		if (m<Inf(itv)) m=Inf(itv); // watch dog
		else if (m>Sup(itv)) m=Sup(itv);
		return m;
	}
}

inline bool Interval::is_empty() const {
	return (ub()==NEG_INFINITY || lb()==POS_INFINITY); //*this==EMPTY_SET;
}

inline bool Interval::is_degenerated() const {
	return is_empty() || lb()==ub();
}

inline bool Interval::is_unbounded() const {
	if (is_empty()) return false;
	return lb()==NEG_INFINITY || ub()==POS_INFINITY;
}

inline double Interval::diam() const {
	return is_empty()? 0: (is_unbounded()? POS_INFINITY : Diam(itv));
}

inline double Interval::mig() const {
	return Mig(itv);
}

inline double Interval::mag() const {
	return Abs(itv);
}

inline Interval operator&(const Interval& x1, const Interval& x2) {
	INTERVAL res;
	if (x1.is_empty() || x2.is_empty() || !Intersection(res, x1.itv, x2.itv))
		return Interval::EMPTY_SET;
	else
		return res;
}

inline Interval operator|(const Interval& x1, const Interval& x2) {
	if (x1.is_empty()) return x2;
	if (x2.is_empty()) return x1;
	return Hull(x1.itv,x2.itv);
}

inline Interval operator+(const Interval& x, double d) {
	if (x.is_empty()) return x;
		else if (d==NEG_INFINITY || d==POS_INFINITY) return Interval::EMPTY_SET;
	else return x.itv+d;
}

inline Interval operator-(const Interval& x, double d) {
	if (x.is_empty()) return x;
	else if (d==NEG_INFINITY || d==POS_INFINITY) return Interval::EMPTY_SET;
	else return x.itv-d;
}

inline Interval operator*(const Interval& x, double d) {
	if (x.is_empty()) return x;
	else if (d==NEG_INFINITY || d==POS_INFINITY) return Interval::EMPTY_SET;
	else return x.itv*d;
}

inline Interval operator/(const Interval& x, double d) {
	if (x.is_empty()) return x;
	else if (d==0 || d==NEG_INFINITY || d==POS_INFINITY) return Interval::EMPTY_SET;
	else return x.itv/INTERVAL(d);
}

inline Interval operator+(double d,const Interval& x) {
	return x+d;
}

inline Interval operator-(double d, const Interval& x) {
	if (x.is_empty()) return x;
	else if (d==NEG_INFINITY || d==POS_INFINITY) return Interval::EMPTY_SET;
	else return d-x.itv;
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
	else
		return x1.itv+x2.itv;
}

inline Interval operator-(const Interval& x1, const Interval& x2) {
	if (x1.is_empty() || x2.is_empty())
		return Interval::EMPTY_SET;
	else
		return x1.itv-x2.itv;
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
	else return Sqr(x.itv);
}

inline Interval sqrt(const Interval& x) {
	if (x.is_empty()) return Interval::EMPTY_SET;

	INTERVAL r;

	if (x.ub()<0) return Interval::EMPTY_SET;
	else {
		if (x.lb()<0) {
			BiasSqrt (Bias(r), Bias(INTERVAL(0.0, x.ub())));
			return r;
		}
		else {
			BiasSqrt (Bias(r), Bias(x.itv));
			return r;
		}
	}
}

inline Interval pow(const Interval& x, int n) {
	if (x.is_empty()) return Interval::EMPTY_SET;
	else if (n==0)	return Interval::ONE;
	else if (n<0)	return 1.0/pow(x,-n);
	else if (n==1)	return x;
	else if (n%2!=0) {
	// there are bugs with negative x
	// such as Power([-2,3],3)->[-18,27])
		if (x.ub()<0) return -Power(-x.itv,n);
		else if (x.lb()<0) return Interval(Inf(-Power(INTERVAL(-x.lb(),-x.lb()),n)),
										   Sup( Power(INTERVAL(x.ub(), x.ub()),n)));
		else return Power(x.itv,n);
	}
	else return Power(x.itv,n);
}

inline Interval pow(const Interval& x, double d) {
	if (x.is_empty()) return Interval::EMPTY_SET;
	else return Power(x.itv,d);
}

inline Interval pow(const Interval &x, const Interval &y) {
	if (x.is_empty()) return Interval::EMPTY_SET;
	else return Power(x.itv,y.itv);
}

inline Interval root(const Interval& x, int n) {
	if (x.is_empty()) return Interval::EMPTY_SET;

	INTERVAL r;

	if (x.lb()==0 && x.ub()==0) return Interval::ZERO;
	if (n==0) return Interval::ONE;
	if (n<0) return 1.0/root(x,-n);
	if (n==1) return x;
	// odd root
	if (n%2==0) {
		if (x.ub()<0)
			return Interval::EMPTY_SET;
		else
			if (x.lb()<0) {
				BiasRoot(Bias(r), Bias(INTERVAL(0.0, x.ub())), n);
				return r;
			} else {
				BiasRoot(Bias(r), Bias(x.itv), n);
				return r;
			}
	} else {
		// even root
		if (x.lb()>=0) {
			BiasRoot(Bias(r), Bias(x.itv), n);
			return r;
		}
		if (x.ub()<=0) {
			BiasRoot(Bias(r), Bias(-x.itv), n);
			return -r;
		}
		INTERVAL r2;
		BiasRoot(Bias(r), Bias(INTERVAL(0,-x.lb())), n);
		BiasRoot(Bias(r2), Bias(INTERVAL(0,+x.ub())), n);

		return Interval(-Sup(r), Sup(r2));
	}
}

inline Interval exp(const Interval& x) {
	if (x.is_empty()) return Interval::EMPTY_SET;
	else if (::exp(x.lb())==POS_INFINITY) return Interval(DBL_MAX,POS_INFINITY);
	else if (::exp(x.ub())==POS_INFINITY) {
		INTERVAL r;
		BiasExp(Bias(r), Bias(INTERVAL(x.lb())));
		return Interval(Inf(r),POS_INFINITY);
	}
	else return Exp(x.itv);
}

inline Interval log(const Interval& x) {
	if (x.is_empty() || x.ub()<=0)
		return Interval::EMPTY_SET;
	else if (x.lb()<=0) {
		INTERVAL r;
		BiasLog (Bias(r), Bias(INTERVAL(x.ub())));
		return Interval(NEG_INFINITY, Sup(r));
	} else {
		INTERVAL r;
		BiasLog (Bias(r), Bias(x.itv));
		return r;
	}
}


inline Interval cos(const Interval& x) {
	if (x.is_empty()) return Interval::EMPTY_SET;
	else return Cos(x.itv);
}

inline Interval sin(const Interval& x) {
	if (x.is_empty()) return Interval::EMPTY_SET;
	else return Sin(x.itv);
}

inline Interval tan(const Interval& x) {
	if (x.is_empty()) return Interval::EMPTY_SET;

	INTERVAL r;

	Interval nb_period = (x+INTERVAL((x.lb()<0?-1:1)*Interval::PI.lb()/2.0,(x.ub()<0?-1:1)*Interval::PI.ub()/2.0))/Interval::PI;
	if (nb_period.is_unbounded() ||
		(((int) nb_period.lb())< ((int) nb_period.ub()))) return Interval(NEG_INFINITY, POS_INFINITY);

	BiasTan (Bias(r), Bias(x.itv));
	return r;
}

inline Interval acos(const Interval& x) {
	if (x.is_empty()) return Interval::EMPTY_SET;

	INTERVAL r;

	if (x.ub()<-1.0 || x.lb()>1.0)
		return Interval::EMPTY_SET;
	else {
		bool minus1=x.lb()<=-1;
		bool plus1=x.ub()>=1;
		BiasArcCos(Bias(r), Bias(INTERVAL(minus1?-1:x.lb(), plus1?1:x.ub())));
		return Interval(plus1?0.0:Inf(r), minus1?Interval::PI.ub():Sup(r));
	}

}

inline Interval asin(const Interval& x) {
	if (x.is_empty()) return Interval::EMPTY_SET;

	INTERVAL r;

	if (x.ub()<-1.0 || x.lb()>1.0)
		return Interval::EMPTY_SET;
	else {
		bool minus1=x.lb()<-1;
		bool plus1=x.ub()>1;
		BiasArcSin(Bias(r), Bias(INTERVAL(minus1?-1:x.lb(), plus1?1:x.ub())));
		return Interval(minus1?(-Interval::HALF_PI).lb():Inf(r), plus1?Interval::HALF_PI.ub():Sup(r));
	}
}

inline Interval atan(const Interval& x) {
	if (x.is_empty()) return Interval::EMPTY_SET;
	else return ArcTan(x.itv);
}

inline Interval cosh(const Interval& x) {
	if (x.is_empty()) return Interval::EMPTY_SET;
	else if (x.ub()==POS_INFINITY) {
		if (x.lb()<=0) return Interval(1,POS_INFINITY);
		else return Interval(Inf(Cosh(x.lb())),POS_INFINITY);
	}
	else if (x.lb()==NEG_INFINITY) {
		if (x.ub()>=0) return Interval(1,POS_INFINITY);
		else return Interval(Inf(Cosh(x.ub())),POS_INFINITY);
	}
	else if (x.lb()>=0)
		return Interval(Inf(Cosh(x.lb())),Sup(Cosh(x.ub())));
	else if (x.ub()<=0)
		return Interval(Inf(Cosh(x.ub())),Sup(Cosh(x.lb())));
	else
		return ((fabs(x.lb())> fabs(x.ub())) ? Interval(1,Sup(Cosh(x.lb()))) :Interval(1,Sup(Cosh(x.ub()))));
}

inline Interval sinh(const Interval& x) {
	if (x.is_empty()) return Interval::EMPTY_SET;
	else if (x.lb()==NEG_INFINITY) {
		if (x.ub()==POS_INFINITY) return Interval::ALL_REALS;
		else return Interval(NEG_INFINITY,Sup(Sinh(x.ub())));
	}
	else if (x.ub()==POS_INFINITY)
		return Interval(Inf(Sinh(x.lb())),POS_INFINITY);
	else
		return Sinh(x.itv);
}

inline Interval tanh(const Interval& x) {
	if (x.is_empty()) return Interval::EMPTY_SET;

	INTERVAL r;

	if (x.lb()==NEG_INFINITY)
		if (x.ub()==POS_INFINITY) return Interval(-1, 1);
		else {
			BiasTanh (Bias(r), Bias(INTERVAL(x.ub())));
			return Interval(-1,Sup(r));
		}
	else {
		if (x.ub()==POS_INFINITY) {
			BiasTanh (Bias(r), Bias(INTERVAL(x.lb())));
			return Interval(Inf(r),1);
		}
		else {
			BiasTanh (Bias(r), Bias(x.itv));
			return r;
		}
	}
}

inline Interval acosh(const Interval& x) {
	if (x.is_empty()) return Interval::EMPTY_SET;
	if (x.ub()<1.0) return Interval::EMPTY_SET;


	INTERVAL r;

	BiasArCosh (Bias(r), Bias(INTERVAL(x.lb()<1?1:x.lb(),x.ub())));
	return r;

}

inline Interval asinh(const Interval& x) {
	if (x.is_empty()) return Interval::EMPTY_SET;
	INTERVAL r;
	if (x.lb()==NEG_INFINITY)
		if (x.ub()==POS_INFINITY) return Interval::ALL_REALS;
		else {
			BiasArSinh (Bias(r), Bias(INTERVAL(x.ub())));
			return Interval(NEG_INFINITY,Sup(r));
		}
	else {
		if (x.ub()==POS_INFINITY) {
			BiasArSinh (Bias(r), Bias(INTERVAL(x.lb())));
			return Interval(Inf(r),POS_INFINITY);
		}
		else {
			BiasArSinh (Bias(r), Bias(x.itv));
			return r;
		}
	}
}

inline Interval atanh(const Interval& x) {
	if (x.is_empty()) return Interval::EMPTY_SET;

	INTERVAL r;

	if (x.ub()<-1.0 || x.lb()>1.0)
		return Interval::EMPTY_SET;
	else
		if (x.lb()<=-1)
			if (x.ub()>=1)
				return Interval(NEG_INFINITY, POS_INFINITY);
			else {
				BiasArTanh(Bias(r), Bias(INTERVAL(x.ub())));
				return Interval(NEG_INFINITY,Sup(r));
			}
		else
			if (x.ub()>=1) {
				BiasArTanh(Bias(r), Bias(INTERVAL(x.lb())));
				return Interval(Inf(r), POS_INFINITY);
			}

	BiasArTanh (Bias(r), Bias(x.itv));
	return r;
}

inline Interval abs(const Interval &x) {
	if (x.is_empty()) return Interval::EMPTY_SET;
	else return IAbs(x.itv);
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
	double l=x.lb()==NEG_INFINITY? NEG_INFINITY : ceil(x.lb());
	double r=x.ub()==POS_INFINITY? POS_INFINITY : floor(x.ub());
	if (l>r) return Interval::EMPTY_SET;
	else return Interval(l,r);
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

		return !x.is_empty();

	} else {

		x &= root(y, expon);
		return !x.is_empty();

	}
}

inline bool bwd_pow(const Interval& , Interval& , Interval& ) {
	not_implemented("warning: bwd_power(y,x1,x2) (with x1 and x2 intervals) not implemented yet with BIAS");
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

#endif // _IBEX_BIAS_INTERVAL_H_
