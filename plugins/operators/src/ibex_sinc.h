//============================================================================
//                                  I B E X
// File        : ibex_sinc.h
// Author      : Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Oct 02, 2018
//============================================================================

#ifndef __IBEX_SINC_H__
#define __IBEX_SINC_H__

#include "ibex_ExprOperators.h"

namespace ibex {

extern const char SINC[];

/**
 * \brief Cardinal sine.
 */
template<>
class UnaryOperator<SINC,Interval,Interval> {
public:
	/** Dimension */
	static Dim dim(const Dim& x) {
		return Dim::scalar();
	}

	/** Forward evaluation. */
	static Interval fwd(const Interval& x) {
		return sin(x)/x;
	}

	/** Backward evaluation. */
	static void bwd(const Interval& y, Interval& x) {
		Interval sinx=sin(x);
		bwd_div(y, sinx, x);
		bwd_sin(sinx,x);
	}

	/** Backward numerical derivative. */
	static Interval diff(const Interval& x, const Interval& g) {
		return g*((x*cos(x)-sin(x))/sqr(x));
	}

	/** Backward symbolic derivative. */
	static const ExprNode& diff(const ExprNode& x, const ExprNode& g) {
		return g*((x*cos(x)-sin(x))/sqr(x));
	}
};
} // end namespace

#endif /* __IBEX_SINC_H__ */
