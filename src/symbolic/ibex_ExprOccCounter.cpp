//============================================================================
//                                  I B E X
// File        : ibex_ExprOccCounter.cpp
// Author      : Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Apr 08, 2020
// Last update : May 12, 2020
//============================================================================

#include "ibex_ExprOccCounter.h"

namespace ibex {

const Matrix& ExprOccCounter::count(const ExprNode& e) const {
	return *((ExprOccCounter*) this)->visit(e);
}

unsigned int ExprOccCounter::total(const Matrix& m) const {
	unsigned int acc = 0;
	for (int i=0; i<m.nb_rows(); i++) {
		for (int j=0; j<m.nb_cols(); j++)
			acc += m[i][j];
	}
	return acc;
}

unsigned int ExprOccCounter::total(const ExprNode& e) const {
	return total(*((ExprOccCounter*) this)->visit(e));
}

Matrix* ExprOccCounter::visit(const ExprNode& e) {
	return ExprVisitor<Matrix*>::visit(e);
}

Matrix* ExprOccCounter::visit(const ExprIndex& e) {
	Matrix* ce = visit(e.expr);
	Matrix* m = new Matrix(e.dim.nb_rows(), e.dim.nb_cols());

	int ie = e.index.first_row();
	for (int i=0; i<e.dim.nb_rows(); i++,ie++) {
		int je = e.index.first_col();
		for (int j=0; j<e.dim.nb_cols(); j++,je++) {
			(*m)[i][j] = (*ce)[ie][je];
		}
	}
	return m;
}

Matrix* ExprOccCounter::visit(const ExprSymbol& e) {
	return new Matrix(e.dim.nb_rows(), e.dim.nb_cols(), 1.);
}

Matrix* ExprOccCounter::visit(const ExprConstant& e) {
	return new Matrix(e.dim.nb_rows(), e.dim.nb_cols(), 0.);
}

Matrix* ExprOccCounter::visit(const ExprNAryOp& e) {
	// This is the default behavior, overridden
	// in the case of ExprVector
	unsigned int arg_total=0;
	for (int k=0; k<e.nb_args; k++) {
		arg_total += total(e.arg(k));
	}
	return new Matrix(e.dim.nb_rows(), e.dim.nb_cols(), arg_total);
}

Matrix* ExprOccCounter::visit(const ExprVector& e) {
	Matrix* c = new Matrix(e.dim.nb_rows(), e.dim.nb_cols());

	int I=0; // starting row of the kth argument
	int J=0; // starting column of the kth argument
	for (int k=0; k<e.nb_args; k++) {
		const ExprNode& ek=e.arg(k);
		Matrix* ck = visit(ek);
		int i=I;
		for (int ik=0; ik<ek.dim.nb_rows(); ik++, i++) {
			int j=J;
			for (int jk=0; jk<ek.dim.nb_cols(); jk++, j++) {
				(*c)[i][j]=(*ck)[ik][jk];
			}
		}
		if (e.row_vector()) J += ek.dim.nb_cols();
		else I += ek.dim.nb_rows();
	}
	return c;
}

Matrix* ExprOccCounter::visit(const ExprBinaryOp& e) {
	// This is the default behavior, overridden
	// in the case of +,-,* and generic binary op.

	return new Matrix(1, 1, (*visit(e.left))[0][0] + (*visit(e.right))[0][0]);
}

Matrix* ExprOccCounter::visit(const ExprGenericBinaryOp& e) {
	if (e.dim.is_scalar())
		return visit((const ExprBinaryOp&) e); // will be faster
	else {
		// since we have no a priori information,
		// all components of e depends on all components
		// of left and right.
		unsigned int t = total(*visit(e.left)) + total(*visit(e.right));
		return new Matrix(e.dim.nb_rows(), e.dim.nb_cols(), t);
	}
}

Matrix* ExprOccCounter::visit(const ExprAdd& e) {
	if (e.dim.is_scalar())
		return visit((const ExprBinaryOp&) e); // will be faster

	Matrix* cl = visit(e.left);
	Matrix* cr = visit(e.right);

	Matrix* c = new Matrix(e.dim.nb_rows(), e.dim.nb_cols());
	for (int i=0; i<e.dim.nb_rows(); i++) {
		for (int j=0; j<e.dim.nb_cols(); j++) {
			(*c)[i][j] = (*cl)[i][j] + (*cr)[i][j];
		}
	}
	return c;
}

Matrix ExprOccCounter::occ_product(const Matrix& m1, const Matrix& m2) const {

	if (m1.nb_rows()==1 && m1.nb_cols()==1) {
		if (m2.nb_rows()==1 && m2.nb_cols()==1)
			return Matrix(1, 1, m1[0][0]+m2[0][0]); // faster
		else { // scalar multiplication
			return m2+m1[0][0]*Matrix::ones(m2.nb_rows(), m2.nb_cols());
		}
	}

	unsigned int* left_row_sum = new unsigned int[m1.nb_rows()];
	unsigned int* right_col_sum = new unsigned int[m2.nb_cols()];
	for (int i=0; i<m1.nb_rows(); i++) {
		left_row_sum[i]=0;
		for (int j=0; j<m1.nb_cols(); j++)
			left_row_sum[i] += m1[i][j];
	}
	for (int j=0; j<m2.nb_cols(); j++) {
		right_col_sum[j]=0;
		for (int i=0; i<m2.nb_rows(); i++) {
			right_col_sum[j] += m2[i][j];
		}
	}

	Matrix c(m1.nb_rows(), m2.nb_cols());
	for (int i=0; i<m1.nb_rows(); i++) {
		for (int j=0; j<m2.nb_cols(); j++) {
			c[i][j] = left_row_sum[i]+right_col_sum[j];
		}
	}

	delete[] left_row_sum;
	delete[] right_col_sum;
	return c;
}

Matrix* ExprOccCounter::visit(const ExprMul& e) {
	if (e.dim.is_scalar())
		return visit((const ExprBinaryOp&) e); // will be faster
	else
	return new Matrix(occ_product(*visit(e.left), *visit(e.right)));
}

Matrix* ExprOccCounter::visit(const ExprSub& e) {
	// copy-paste with ExprAdd
	if (e.dim.is_scalar())
		return visit((const ExprBinaryOp&) e); // will be faster

	Matrix* cl = visit(e.left);
	Matrix* cr = visit(e.right);

	Matrix* c = new Matrix(e.dim.nb_rows(), e.dim.nb_cols());
	for (int i=0; i<e.dim.nb_rows(); i++) {
		for (int j=0; j<e.dim.nb_cols(); j++) {
			(*c)[i][j] = (*cl)[i][j] + (*cr)[i][j];
		}
	}
	return c;
}

Matrix* ExprOccCounter::visit(const ExprUnaryOp& e) {
	// This is the default behavior, overridden
	// in the case of -,transpose and generic unary op.
	return new Matrix(1, 1, (*visit(e.expr))[0][0]);
}

Matrix* ExprOccCounter::visit(const ExprGenericUnaryOp& e) {
	if (e.dim.is_scalar())
		return visit((const ExprUnaryOp&) e); // will be faster
	else
		return new Matrix(e.dim.nb_rows(), e.dim.nb_cols(), total(e.expr));
}

Matrix* ExprOccCounter::visit(const ExprMinus& e) {
	if (e.dim.is_scalar()) return visit((const ExprUnaryOp&) e); // will be faster

	Matrix* ce = visit(e.expr);
	Matrix* c = new Matrix(e.dim.nb_rows(), e.dim.nb_cols());
	for (int i=0; i<e.dim.nb_rows(); i++) {
		for (int j=0; j<e.dim.nb_cols(); j++) {
			(*c)[i][j] = (*ce)[i][j];
		}
	}
	return c;
}

Matrix* ExprOccCounter::visit(const ExprTrans& e) {
	if (e.dim.is_scalar()) return visit((const ExprUnaryOp&) e); // will be faster

	Matrix* ce = visit(e.expr);
	Matrix* c = new Matrix(e.dim.nb_rows(), e.dim.nb_cols());
	for (int i=0; i<e.dim.nb_rows(); i++) {
		for (int j=0; j<e.dim.nb_cols(); j++) {
			(*c)[i][j] = (*ce)[j][i];
		}
	}
	return c;
}

} /* namespace ibex */
