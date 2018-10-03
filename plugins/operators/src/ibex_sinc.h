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
#include "ibex_Interval.h"

namespace ibex {

extern const char SINC[];

template<>
class UnaryOperator<SINC,Interval,Interval> {
public:
	/** Dimension */
	static Dim dim(const Dim& x) {
		return Dim::scalar();
	}

	/** Forward evaluation. */
	static Interval eval(const Interval& x) {
		return sin(x)/x;
	}

	/** Backward evaluation. */
	static void bwd(Interval& x, const Interval& y) {
		x=sin(x)/y; // pessimistic...
	}

	/** Numerical derivative. */
	static Interval diff(const Interval& x) {
		return cos(x)-sin(x)/sqr(x);
	}

	/** Symbolic derivative. */
	static const ExprNode& diff(const ExprNode& expr) {
		return cos(expr)-sin(expr)/sqr(expr);
	}
};
} // end namespace

#endif /* __IBEX_SINC_H__ */
