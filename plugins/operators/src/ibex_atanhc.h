//============================================================================
//                                  I B E X
// File        : ibex_atanhc.h
// Author      : Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Oct 05, 2018
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
	static Interval fwd(const Interval& x) {
		return atanh(x)/x;
	}

	/** Backward evaluation. */
	static void bwd(const Interval& y, Interval& x) {
		Interval atanhx=atanh(x);
		bwd_div(y,atanhx,x);
		bwd_atan(atanhx, x);
	}

	/** Backward numerical derivative. */
	static Interval diff(const Interval& x, const Interval& g) {
		return g*(1/(x*(1-sqr(x)))-atanh(x)/sqr(x));
	}

	/** Backward symbolic derivative. */
	static const ExprNode& diff(const ExprNode& x, const ExprNode& g) {
		return g*(1/(x*(1-sqr(x)))-atanh(x)/sqr(x));
	}
};
} // end namespace

#endif /* __IBEX_ATANHC_H__ */
