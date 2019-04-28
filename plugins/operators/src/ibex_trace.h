//============================================================================
//                                  I B E X
// File        : ibex_trace.h
// Author      : Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Oct 03, 2018
//============================================================================

#ifndef __IBEX_TRACE_H__
#define __IBEX_TRACE_H__

#include "ibex_ExprOperators.h"

namespace ibex {

extern const char TRACE[];

template<>
class UnaryOperator<TRACE,IntervalMatrix,IntervalVector> {
public:
	/** Dimension */
	static Dim dim(const Dim& x) {
		if (x.nb_rows()!=x.nb_cols())
			throw DimException("trace of non-square matrix");
		return Dim::col_vec(x.nb_rows());
	}

	/** Forward evaluation. */
	static IntervalVector fwd(const IntervalMatrix& x) {
		int n=x.nb_rows();
		IntervalVector t(n);
		for (int i=0; i<n; i++)
			t[i]=x[i][i];
		return t;
	}

	/** Backward evaluation. */
	static void bwd(const IntervalVector& y, IntervalMatrix& x) {
		int n=x.nb_rows();
		for (int i=0; i<n; i++)
			x[i][i] &= y[i];
	}

	/** Backward numerical derivative. */
	static IntervalMatrix diff(const IntervalMatrix& x, const IntervalVector& g) {
		int n=x.nb_rows();
		IntervalMatrix M=Matrix::zeros(n);
		for (int i=0; i<n; i++)
			M[i][i] &= g[i];
		return M;
	}

	/** Backward symbolic derivative. */
	static const ExprNode& diff(const ExprNode& x, const ExprNode& g) {
		int n=x.dim.nb_rows();
		Array<const ExprNode> matrix(n);
		for (int i=0; i<n; i++) {
			Array<const ExprNode> row(n);
			row.set_ref(i,g[i]);
			for (int j=0; j<n; j++) {
				if (j!=i)
					row.set_ref(j,ExprConstant::new_scalar(Interval::zero()));
			}
			matrix.set_ref(i,ExprVector::new_row(row));
		}
		return ExprVector::new_col(matrix);
	}
};
} // end namespace

#endif /* __IBEX_TRACE_H__ */
