//============================================================================
//                                  I B E X
// File        : ibex_atanhccc.h
// Author      : Alexandre Goldsztejn
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Oct 05, 2018
//============================================================================

#ifndef __IBEX_ATANHCCC_H__
#define __IBEX_ATANHCCC_H__

#include "ibex_ExprOperators.h"

namespace ibex {

extern const char ATANHCCC[];

/**
 * \brief 3rd-order cardinal arctangent.
 */
template<>
class UnaryOperator<ATANHCCC,Interval,Interval> {

public:

	/** Dimension */
	static Dim dim(const Dim& x) {
		return Dim::scalar();
	}

	/** Forward evaluation. */
	static Interval fwd(const Interval& xin) {
        Interval x=xin & Interval(-1.,1.);
        Interval y = _atanhccc(x.lb());
        y |= _atanhccc(x.ub());
        return x.contains(0.) ? y | Interval(1.)/Interval(3.) : y;
	}

	/** Backward evaluation. */
	static void bwd(const Interval& y, Interval& x) {
		Interval atanhx=atanh(x);
		Interval xcube=pow(x,3);
		Interval ycopy=y;
		bwd_sub(y*xcube, atanhx, x);
		bwd_mul(atanhx-x, ycopy, xcube);
		bwd_pow(xcube, 3, x);
		bwd_atanh(atanhx,x);
	}

	/** Backward numerical derivative. */
	static Interval diff(const Interval& xin, const Interval& g) {
        Interval x=xin & Interval(-1.,1.);
        return g*( _datanhccc(x.lb()) | _datanhccc(x.ub()) );
	}

	/** Backward symbolic derivative. */
	static const ExprNode& diff(const ExprNode& x, const ExprNode& g) {
        return g*((-1 + 1/(1 - sqr(x)))/pow(x,3) - (3*(-x + atanh(x)))/pow(x,4));
	}

private:

    static Interval _atanh(Interval x) {
        return 0.5*log((1+x)/(1-x));
    }

    static Interval _atanhccc(double x_double) {
        if (x_double<=-1 || x_double>=1) return Interval(std::numeric_limits<double>::max(),std::numeric_limits<double>::infinity()); // Not optimal but easy to implement. Should not have any impact.
        Interval x(x_double);
        // 4th order Taylor Expansion (remainder valid for all x in [-1,1])
        Interval taylor_4 = Interval(1.)/Interval(3.) + sqr(x)*( Interval(1.)/Interval(5.) + sqr(x)/Interval(7.) ) + Interval(-1.,1.)*(pow(x,6)/(Interval(1.)-sqr(x)));
        return taylor_4 & (_atanh(x)-x)/pow(x,3);
    }

    static Interval _datanhccc(double x_double) {
        if(x_double<=-1) return Interval(-std::numeric_limits<double>::infinity(),-std::numeric_limits<double>::max()); // Not optimal but easy to implement. Should not have any impact.
        if(x_double>=1) return Interval(std::numeric_limits<double>::max(),std::numeric_limits<double>::infinity()); // Not optimal but easy to implement. Should not have any impact.
        Interval x(x_double);
        // 3th order Taylor Expansion (remainder valid for all x in [-1,1])
        Interval taylor_3 = x*(Interval(2.)/Interval(5.) + Interval(4.)/Interval(7.)*sqr(x)) + Interval(-1.,1.)*(pow(x,5)/(Interval(1.)-sqr(x)));
        return taylor_3 & ((-1 + 1/(1 - sqr(x)))/pow(x,3) - (3*(-x + _atanh(x)))/pow(x,4));
    }
};

} // end namespace

#endif /* __IBEX_ATANHCCC_H__ */
