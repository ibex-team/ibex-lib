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
#include <float.h>
#include <cassert>
#include <sstream>
#include <iomanip>

#ifdef _IBEX_WITH_GAOL_
#include "ibex_Interval_gaol.cpp_"
#else
#ifdef _IBEX_WITH_BIAS_
#include "ibex_Interval_bias.cpp_"
#else
#ifdef _IBEX_WITH_FILIB_
#include "ibex_Interval_filib.cpp_"
#else
#ifdef _IBEX_WITH_DIRECT_
#include "ibex_Interval_direct.cpp_"
#endif
#endif
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

int Interval::complementary(Interval& c1, Interval& c2) const {
	if (is_empty() || is_degenerated()) { // x.is_empty() should not happen if called from compl()
		c1=Interval::ALL_REALS;
		c2=Interval::EMPTY_SET;
		return 1;
	}
	else {
		if (lb()>NEG_INFINITY) {
			c1=Interval(NEG_INFINITY,lb());
			if (ub()<POS_INFINITY) {
				c2=Interval(ub(),POS_INFINITY);
				return 2;
			} else {
				c2=Interval::EMPTY_SET;
				return 1;
			}
		} else if (ub()<POS_INFINITY) {
			c1=Interval(ub(),POS_INFINITY);
			c2=Interval::EMPTY_SET;
			return 1;
		} else {
			c1=c2=Interval::EMPTY_SET;
			return 0;
		}
	}
}

int Interval::diff(const Interval& y, Interval& c1, Interval& c2) const {
	const Interval& x=*this;

	if (x.is_degenerated()) { // specific treatment to avoid overestimation of complementary
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

	y.complementary(c1,c2);
	c1 &= x;
	int res=2;
	if (c1.is_degenerated()) { c1=Interval::EMPTY_SET; res--; }
	c2 &= x;
	if (c2.is_degenerated()) { c2=Interval::EMPTY_SET; res--; }

	if (c1.is_empty()) {
		c1=c2;
		c2=Interval::EMPTY_SET;
	}
	return res;
}

Interval::Interval(const std::string& s)
{
	if(s.find(EMPTY_SET.str()) != std::string::npos)
		*this = EMPTY_SET;

	else if(s.find(POS_REALS.str()) != std::string::npos)
		*this = POS_REALS;

	else if(s.find(NEG_REALS.str()) != std::string::npos)
		*this = NEG_REALS;

	else if(s.find(ALL_REALS.str()) != std::string::npos)
		*this = ALL_REALS;

	else { // string of the form "[lb, ub]"
		// Removing unwanted spaces:
		std::string clean_s = s;
		clean_s.erase(std::remove(clean_s.begin(), clean_s.end(), ' '), clean_s.end());

		std::string delimiter = ",";
		size_t pos_delimiter = clean_s.find(',');

		assert(pos_delimiter != std::string::npos);

		std::string lb = clean_s.substr(1, clean_s.find(delimiter) - delimiter.length());
		std::string ub = clean_s.substr(lb.length() + 2, clean_s.length() - lb.length() - delimiter.length()  - 2);

		*this = Interval(atof(lb.c_str()), atof(ub.c_str()));
	}
}

std::ostream& operator<<(std::ostream& os, const Interval& x) {
  // A specific string has to be set for the following cases:
  // (in order to not depend on the interval library)
  if(x == Interval::EMPTY_SET)
    os << "[ empty ]";
  
  else if(x == Interval::POS_REALS)
    os << "[ pos_reals ]";
  
  else if(x == Interval::NEG_REALS)
    os << "[ neg_reals ]";
  
  else if(x == Interval::ALL_REALS)
    os << "[ all_reals ]";

  else
  {
  	#ifdef _IBEX_WITH_FILIB_
  		filib::interval<FI_BASE,FI_ROUNDING,FI_MODE>::precision(os.precision());
  	#endif
    os << "[" << (double)x.lb() << ", " << (double)x.ub() << "]";
  }
  
  return os;
}

std::string Interval::str(int precision) const
{
	std::stringstream sstream;
	sstream << std::setprecision(precision) << *this;
	return sstream.str();
}

} // end namespace
