/* ============================================================================
 * I B E X - Interval definition
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Dec 05, 2011
 * ---------------------------------------------------------------------------- */

#include "ibex_Interval.h"

#ifdef _IBEX_WITH_GAOL_
#include "ibex_gaol_Interval.cpp_"
#else
#ifdef _IBEX_WITH_BIAS_
#include "ibex_bias_Interval.cpp_"
#endif
#endif

namespace ibex {


#define INF_DIV(n,d) ((Interval(n)/Interval(d)).lb())
#define SUP_DIV(n,d) ((Interval(n)/Interval(d)).ub())

#define INF_MUL(x,y) ((Interval(x)*Interval(y)).lb())
#define SUP_MUL(x,y) ((Interval(x)*Interval(y)).ub())

void div2(const Interval& num, const Interval& div, Interval& out1, Interval& out2) {

	if (num.is_empty() || div.is_empty()) {
		out1.set_empty();
		out2.set_empty();
		return;
	}

	const double& a(num.lb());
	const double& b(num.ub());
	const double& c(div.lb());
	const double& d(div.ub());

	// notice : we do not consider 0/0=0 but 0/0=emptyset
	if (c==0 && d==0) {
		out1.set_empty();
		out2.set_empty();
		return;
	}

	if (a==0 && b==0) {
		out1 = num;
		out2.set_empty();
		return;
	}

	if (c>0 || d<0) {
		out1 = num/div;
		out2.set_empty();
		return;
	}

	if (b<=0 && d==0) {
		if (c==NEG_INFINITY)
			out1 = Interval::POS_REALS;
		else
			out1 = Interval(INF_DIV(b,c), POS_INFINITY);
		out2.set_empty();
		return;
	}

	if (b<=0 && c<0 && d>0) {
		if (b==0 || (c==NEG_INFINITY && d==POS_INFINITY)) {
			out1 = Interval::ALL_REALS;
			out2.set_empty();
			return;
		} else {
			out1 = Interval(NEG_INFINITY, d==POS_INFINITY? 0 : SUP_DIV(b,d));
			out2 = Interval(c==NEG_INFINITY? 0 : INF_DIV(b,c), POS_INFINITY);
			return;
		}
	}

	if (b<=0 && c==0) {
		if (d==POS_INFINITY)
			out1 = Interval::NEG_REALS;
		else
			out1 = Interval(NEG_INFINITY, SUP_DIV(b,d));
		out2.set_empty();
		return;
	}

	if (a>=0 && d==0) {
		if (c==NEG_INFINITY)
			out1 = Interval::NEG_REALS;
		else
			out1 = Interval(NEG_INFINITY, SUP_DIV(a,c));
		out2.set_empty();
		return;
	}

	if (a>=0 && c<0 && d>0) {
		if (a==0 || (c==NEG_INFINITY && d==POS_INFINITY)) {
			out1 = Interval::ALL_REALS;
			out2.set_empty();
			return;
		} else {
			out1 = Interval(NEG_INFINITY, c==NEG_INFINITY? 0 : SUP_DIV(a,c));
			out2 = Interval(d==POS_INFINITY? 0 : INF_DIV(a,d), POS_INFINITY);
			return;
		}

	}

	if (a>=0 && c==0) {
		if (d==POS_INFINITY)
			out1 = Interval::POS_REALS;
		else
			out1 = Interval(INF_DIV(a,d), POS_INFINITY);
		out2.set_empty();
		return;
	}

	out1 = Interval::ALL_REALS;
	out2.set_empty();
}

bool Interval::div2_inter(const Interval& num, const Interval& div, Interval& out2) {
	// warning: we may have &num==this
	Interval out1,_out2;
	div2(num,div,out1,_out2);
	out1 &= *this;
	if (out1.is_empty()) {
		*this &= _out2;
		out2.set_empty();
		return !this->is_empty();
	} else {
		out2 = *this & _out2;
		*this = out1;
		return true;
	}
}

double Interval::delta(const Interval& x) const {
	if (is_empty()) return 0;
	if (x.is_empty()) return diam();

	// ** warning **
	// checking if *this or x is infinite by
	// testing if the lower/upper bounds are -oo/+oo
	// is not enough because diam() may return +oo even
	// with finite bounds (very large intervals).

	double d=diam();
	double dx=x.diam();
	//
	if (d==POS_INFINITY) {
		//cout << "d=" << d << " dx=" << dx << endl;
		if (dx==POS_INFINITY) {
			double left=(x.lb()==NEG_INFINITY? 0 : x.lb()-lb());
			double right=(x.ub()==POS_INFINITY? 0 : ub()-x.ub());
			return left+right;

			cout << "left=" << left << " right=" << right << endl;
		} else
			return POS_INFINITY;
	}
	else return d-dx;
}

double Interval::ratiodelta(const Interval& x) const {
	double d=delta(x);
	if (d==POS_INFINITY) return 1;
	double D=diam();
	return (D==0 || D==POS_INFINITY) ? 0.0 : (d/D); // if this.diam()=infinity here, necessarily d=0
}

} // end namespace
