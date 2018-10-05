//============================================================================
//                                  I B E X
// File        : ibex_atanhccc.h
// Author      : Gilles Chabert
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
	static Interval eval(const Interval& x) {
		return (atanh(x)-x)/pow(x,3);
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
	static Interval diff(const Interval& x, const Interval& g) {
		return g*(1/(pow(x,3)*(1-sqr(x)))-3*atanh(x)/pow(x,4));
	}

	/** Backward symbolic derivative. */
	static const ExprNode& diff(const ExprNode& x, const ExprNode& g) {
		return g*(1/(pow(x,3)*(1-sqr(x)))-3*atanh(x)/pow(x,4));
	}
};
} // end namespace

#endif /* __IBEX_ATANHCCC_H__ */
