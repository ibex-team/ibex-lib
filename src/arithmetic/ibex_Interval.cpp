/* ============================================================================
 * I B E X - Interval definition
 * ============================================================================
 * Copyright   : IMT Atlantique (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Dec 05, 2011
 * Last update : Oct 30, 2019
 * ---------------------------------------------------------------------------- */

#include "ibex_Interval.h"
#include <float.h>
#include <cassert>

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
			out1 = Interval::pos_reals();
		else
			out1 = Interval(INF_DIV(b,c), POS_INFINITY);
		out2.set_empty();
		return;
	}

	if (b<=0 && c<0 && d>0) {
		if (b==0 || (c==NEG_INFINITY && d==POS_INFINITY)) {
			out1 = Interval::all_reals();
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
			out1 = Interval::neg_reals();
		else
			out1 = Interval(NEG_INFINITY, SUP_DIV(b,d));
		out2.set_empty();
		return;
	}

	if (a>=0 && d==0) {
		if (c==NEG_INFINITY)
			out1 = Interval::neg_reals();
		else
			out1 = Interval(NEG_INFINITY, SUP_DIV(a,c));
		out2.set_empty();
		return;
	}

	if (a>=0 && c<0 && d>0) {
		if (a==0 || (c==NEG_INFINITY && d==POS_INFINITY)) {
			out1 = Interval::all_reals();
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
			out1 = Interval::pos_reals();
		else
			out1 = Interval(INF_DIV(a,d), POS_INFINITY);
		out2.set_empty();
		return;
	}

	out1 = Interval::all_reals();
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

Interval saw(const Interval& x) {
	double u = round(x.ub());
	double l = round(x.lb());

	assert(l<=u);

	if (l<u) return Interval(-1,1);
	else {
		fpu_round_up();
		double du  = x.ub()-u;
		double dl  = l-x.lb();

		return Interval(-dl,du);
	}
}

bool bwd_saw(const Interval& y_, Interval& x) {

	if (y_.is_empty()) {
		x.set_empty();
		return false;
	}

	if (y_.lb()==0 && y_.ub()==0) {
		// a very very frequent case,
		// handled separately just for efficiency
		// note: the interval [ceil(x.lb()),floor(x.ub())] can be empty (reversed bounds)
		return !(x &= Interval(std::ceil(x.lb()),std::floor(x.ub()))).is_empty();
	}

	assert(sizeof(double)==8); // |mantissa|=53
	// 2^51 is a rough bound. Troubles start when |x|~2^53
	// that is, when we start jumping integers in the
	// double representation. In this case, no contraction
	// can be expected unless x is degenerated
	// or y is empty.
	// And we actually requires 2^51 and not 2^52 because we
	// need below to have midpoints between integers representable.
	if (x.mag()>=std::pow(2,52)) {
		if (x.is_degenerated() && !y_.contains(0)) {
			x.set_empty();
			return false;
		} else
			return true;
	}

	double l = round(x.lb());
	double u = round(x.ub());

	double half=1/2.0; // nice: 1/2 is exactly representable.

	Interval y = y_ & Interval(-half,half);

	fpu_round_up();

	double xl;

	if (x.lb() > l+y.ub()) { // l+y.ub() rounded upward
		xl = -(-(l+1)        // l+1=the next integer, no rounding should occur (see above)
		       - y.lb());    // note: (l+1)+y.lb() cannot be less than (l+1)-0.5, which is representable
		assert(xl>=x.lb());  // hence xl>=x.lb()
	} else {
		xl = -(-l - y.lb()); // l+y.lb() rounded downward
	}

	double xu;

	if (x.ub() < -(-u-y.lb())) { // u+y.lb() rounded downward
		xu = (u-1)          // the previous integer, no rounding should occur (see above)
		     + y.ub();      // note: (u-1)+y.ub() cannot exceed (u-1)+0.5, which is representable
		assert(xu<=x.ub()); // hence xu<=x.ub()
	} else {
		xu = u + y.ub();
	}

	// note: [xl,xu] can be empty if xl>xu.
	return !(x &= Interval(xl,xu)).is_empty();
}

double Interval::delta(const Interval& x) const {
	if (is_empty()) return 0;
	if (x.is_empty()) return diam();

	// ** warning **
	// checking if *this or x is infinite by
	// testing if the lower/upper bounds are -oo/+oo
	// is not enough because diam() may return +oo even
	// with finite bounds (e.g, very large intervals like [-DBL_MAX,DBL_MAX]).
    // ("almost-unboundedness")

	volatile double d=diam();
	volatile double dx=x.diam();
	// furthermore, if these variables are not declared volatile
	// conditions like d==POS_INFINITY are evaluated
	// to FALSE for intervals like [-DBL_MAX,DBL_MAX] (with -O3 option)
	// while the returned expression (d-dx) evaluates to +oo (instead of 0).

	if (d==POS_INFINITY) {
		//cout << "d=" << d << " dx=" << dx << endl;
		if (dx==POS_INFINITY) {
			double left=(x.lb()==NEG_INFINITY? 0 : x.lb()-lb());
			double right=(x.ub()==POS_INFINITY? 0 : ub()-x.ub());
			//cout << "left=" << left << " right=" << right << endl;
			return left+right;
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

std::pair<Interval,Interval> Interval::bisect(double ratio) const {

	assert(is_bisectable());
	assert(ratio>0 && ratio<1);

	Interval left,right;

	if (lb()==NEG_INFINITY) {
		if (ub()==POS_INFINITY) {
			left = Interval(NEG_INFINITY,0);
			right = Interval(0,POS_INFINITY);
		}
		else {
			left = Interval(NEG_INFINITY,-DBL_MAX);
			right = Interval(-DBL_MAX,ub());
		}
	}

	else if (ub()==POS_INFINITY) {
		left = Interval(lb(),DBL_MAX);
		right = Interval(DBL_MAX,POS_INFINITY);
	}

	else {
		double point;
		if (ratio==0.5)
			point = mid();
		else {
			point = lb()+ratio*diam();

			// watch dog. note that since *this is
			// bisectable, we have next_float(left.lb()) < ub()
			if (point >= ub()) point=next_float(lb());
			assert(point<ub());
		}
		left = Interval(lb(), point);
		right = Interval(point, ub());
	}

	return std::pair<Interval,Interval>(left,right);
}

int Interval::complementary(Interval& c1, Interval& c2, bool compactness) const {
	if (is_empty() || (compactness && is_degenerated())) { // x.is_empty() should not happen if called from compl()
		c1=Interval::all_reals();
		c2=Interval::empty_set();
		return 1;
	}
	else {
		if (lb()>NEG_INFINITY) {
			c1=Interval(NEG_INFINITY,lb());
			if (ub()<POS_INFINITY) {
				c2=Interval(ub(),POS_INFINITY);
				return 2;
			} else {
				c2=Interval::empty_set();
				return 1;
			}
		} else if (ub()<POS_INFINITY) {
			c1=Interval(ub(),POS_INFINITY);
			c2=Interval::empty_set();
			return 1;
		} else {
			c1=c2=Interval::empty_set();
			return 0;
		}
	}
}

int Interval::diff(const Interval& y, Interval& c1, Interval& c2, bool compactness) const {
	const Interval& x=*this;

	if (compactness && x.is_degenerated()) { // specific treatment to avoid overestimation of complementary
		// the following test allows to return EMPTY when x is a bound of y (e.g., x=0 and y=0 or y=[0,1])
		if (x.is_empty() || y.contains(x.lb())) {
			c1.set_empty();
			c2.set_empty();
			return 0;
		} else {
			c1=x;
			c2.set_empty();
			return 1;
		}
	}

	y.complementary(c1, c2, compactness);
	c1 &= x;
	int res=2;
	if (c1.is_degenerated()) { c1=Interval::empty_set(); res--; }
	c2 &= x;
	if (c2.is_degenerated()) { c2=Interval::empty_set(); res--; }

	if (c1.is_empty()) {
		c1=c2;
		c2=Interval::empty_set();
	}
	return res;
}


} // end namespace
