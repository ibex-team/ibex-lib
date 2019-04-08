//============================================================================
//                                  I B E X
// File        : ibex_atanhc.h
// Author      : Alexandre Goldsztejn
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Oct 05, 2018
// Last Update : Nov 28, 2018
//============================================================================

#ifndef __IBEX_ATANHC_H__
#define __IBEX_ATANHC_H__

#include "ibex_ExprOperators.h"

namespace ibex {

extern const char ATANHC[];

/**
 * \brief Cardinal arctangent.
 */
template<>
class UnaryOperator<ATANHC,Interval,Interval> {

public:

	/** Dimension */
	static Dim dim(const Dim& x) {
		return Dim::scalar();
	}

	/** Forward evaluation. */
	static Interval fwd(const Interval& xin) {
		Interval x=xin & Interval(-1.,1.);
		Interval y = _atanhc(x.lb());
		y |= _atanhc(x.ub());
		return x.contains(0.) ? y | Interval(1.) : y ;
	}

	/** Backward evaluation. */
	static void bwd(const Interval& y, Interval& x) {
		Interval atanhx=atanh(x);
		bwd_div(y,atanhx,x);
		bwd_atanh(atanhx, x);
	}

	/** Backward numerical derivative. */
	static Interval diff(const Interval& xin, const Interval& g) {
		Interval x=xin & Interval(-1.,1.);
		return g*( _datanhc(x.lb()) | _datanhc(x.ub()) );
	}

	/** Backward symbolic derivative. */
	static const ExprNode& diff(const ExprNode& x, const ExprNode& g) {
		return g*(1/(x*(1-sqr(x)))-atanh(x)/sqr(x));
	}

private :

	static Interval _atanh(Interval x) {
		return 0.5*log((1+x)/(1-x));
	}

	static Interval _atanhc(double x_double) {
		if (x_double<=-1 || x_double>=1) return Interval(std::numeric_limits<double>::max(),std::numeric_limits<double>::infinity()); // Not optimal but easy to implement. Should not have any impact.
		Interval x(x_double);
		// 4th order Taylor Expansion (remainder valid for all x in [-1,1])
		Interval taylor_4 = Interval(1.) + sqr(x)*( Interval(1.)/Interval(3.) + sqr(x)/Interval(5.) ) + Interval(-1.,1.)*(pow(x,6)/(Interval(1.)-sqr(x)));
		return taylor_4 & (_atanh(x)/x);
	}

	static Interval _datanhc(double x_double) {
		if(x_double<=-1) return Interval(-std::numeric_limits<double>::infinity(),-std::numeric_limits<double>::max()); // Not optimal but easy to implement. Should not have any impact.
		if(x_double>=1) return Interval(std::numeric_limits<double>::max(),std::numeric_limits<double>::infinity()); // Not optimal but easy to implement. Should not have any impact.
		Interval x(x_double);
		// 3th order Taylor Expansion (remainder valid for all x in [-1,1])
		Interval taylor_3 = x*(Interval(2.)/Interval(3.) + Interval(4.)/Interval(5.)*sqr(x)) + Interval(-1.,1.)*(pow(x,5)/(Interval(1.)-sqr(x)));
		return taylor_3 & (1./(x*(1 - sqr(x))) - _atanh(x)/sqr(x)) ;
	}
};

} // end namespace

#endif /* __IBEX_ATANHC_H__ */
