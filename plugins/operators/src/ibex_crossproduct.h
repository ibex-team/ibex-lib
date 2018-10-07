//============================================================================
//                                  I B E X
// File        : ibex_crossproduct.h
// Author      : Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Oct 05, 2018
//============================================================================

#ifndef __IBEX_CROSS_PRODUCT_H__
#define __IBEX_CROSS_PRODUCT_H__

#include "ibex_ExprOperators.h"

namespace ibex {

extern const char CROSS_PRODUCT[];

/**
 * \brief Cross product of two 3D vectors
 */
template<>
class BinaryOperator<CROSS_PRODUCT,IntervalVector,IntervalVector,IntervalVector> {
public:
	/** Dimension */
	static Dim dim(const Dim& x1, const Dim& x2) {
		if (x1.size()!=3 && x2.size()!=3)
			throw DimException("cross product with vectors of sizes<>3");
		return Dim::col_vec(3);
	}

	/** Forward evaluation. */
	static IntervalVector fwd(const IntervalVector& x1, const IntervalVector& x2) {
		assert(x1.size()==3);
		assert(x2.size()==3);

		IntervalVector y(3);
		y[0]=x1[1]*x2[2]-x1[2]*x2[1];
		y[1]=x1[2]*x2[0]-x1[0]*x2[2];
		y[2]=x1[0]*x2[1]-x1[1]*x2[0];
		return y;
	}

	/** Backward evaluation. */
	static void bwd(const IntervalVector& y, IntervalVector& x1, IntervalVector& x2) {
		assert(y.size()==3);
		assert(x1.size()==3);
		assert(x2.size()==3);

		Interval p12=x1[1]*x2[2];
		Interval p21=x1[2]*x2[1];
		Interval p20=x1[2]*x2[0];
		Interval p02=x1[0]*x2[2];
		Interval p01=x1[0]*x2[1];
		Interval p10=x1[1]*x2[0];
		bwd_sub(y[0],p12,p21);
		bwd_sub(y[1],p20,p02);
		bwd_sub(y[2],p01,p10);
		bwd_mul(p12,x1[1],x2[2]);
		bwd_mul(p21,x1[2],x2[1]);
		bwd_mul(p20,x1[2],x2[0]);
		bwd_mul(p02,x1[0],x2[2]);
		bwd_mul(p01,x1[0],x2[1]);
		bwd_mul(p10,x1[1],x2[0]);
	}

	/** Backward numerical derivative. */
	static IntervalVector diff1(const IntervalVector& x1, const IntervalVector& x2, const IntervalVector& g) {
		assert(x1.size()==3);
		assert(x2.size()==3);
		assert(g.size()==3);

		IntervalVector dx1(3);
		dx1[0]=-g[1]*x2[2] + g[2]*x2[1];
		dx1[1]= g[0]*x2[2] - g[2]*x2[0];
		dx1[2]=-g[0]*x2[1] + g[1]*x2[0];
		return dx1;
	}

	/** Backward numerical derivative. */
	static IntervalVector diff2(const IntervalVector& x1, const IntervalVector& x2, const IntervalVector& g) {
		assert(x1.size()==3);
		assert(x2.size()==3);
		assert(g.size()==3);

		IntervalVector dx2(3);
		dx2[0]= g[1]*x1[2] - g[2]*x1[1];
		dx2[1]=-g[0]*x1[2] + g[2]*x1[0];
		dx2[2]= g[0]*x1[1] - g[1]*x1[0];
		return dx2;
	}

	/** Backward symbolic derivative. */
	static const ExprNode& diff1(const ExprNode& x1, const ExprNode& x2, const ExprNode& g) {
		const ExprNode& dx10=-g[1]*x2[2] + g[2]*x2[1];
		const ExprNode& dx11= g[0]*x2[2] - g[2]*x2[0];
		const ExprNode& dx12=-g[0]*x2[1] + g[1]*x2[0];
		return ExprVector::new_col(dx10,dx11,dx12);
	}

	/** Backward symbolic derivative. */
	static const ExprNode& diff2(const ExprNode& x1, const ExprNode& x2, const ExprNode& g) {
		const ExprNode& dx20= g[1]*x1[2] - g[2]*x1[1];
		const ExprNode& dx21=-g[0]*x1[2] + g[2]*x1[0];
		const ExprNode& dx22= g[0]*x1[1] - g[1]*x1[0];
		return ExprVector::new_col(dx20,dx21,dx22);
	}
};

} // end namespace

#endif /* __IBEX_CROSS_PRODUCT_H__ */
