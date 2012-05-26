/* ============================================================================
 * I B E X - Expressions
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Jan 5, 2012
 * ---------------------------------------------------------------------------- */

#include "ibex_Expr.h"
#include "ibex_Function.h"
#include "ibex_NonRecoverableException.h"
#include "ibex_ExprPrinter.cpp_"
#include "ibex_ExprTools.cpp_"
#include <limits.h>
#include <stdio.h>
#include <set>

namespace ibex {

namespace {

int max_height(const ExprNode& n1, const ExprNode& n2) {
	if (n1.height>n2.height) return n1.height;
	else return n2.height;
}

int max_height(const ExprNode** args, int n) {
	int max=0;
	for (int i=0; i<n; i++)
		if (args[i]->height > max) max = args[i]->height;
	return max;
}

// return the dimension of a product (left*right)
Dim mul_dim(const ExprNode& left, const ExprNode& right) {
	const Dim& l=left.dim;
	const Dim& r=right.dim;

	assert(l.dim1==0 && r.dim1==0);

	if (l.type()==Dim::SCALAR) // scalar multiplication.
		return r;
	else {
		if (r.type()==Dim::SCALAR)
			throw NonRecoverableException("Cannot right-multiply by a scalar");
		else if (l.dim3!=r.dim2) {
			if ((l.dim3==0 && r.dim3==0) || (l.dim2==0 && r.dim2==0)) return Dim(0,0,0); // dot product
			else throw NonRecoverableException("Mismatched dimensions in matrix multiplication");
		} else return Dim(0,l.dim2,r.dim3);
		/* should work in all remaining cases:
		 * - if l is a matrix and r is a vector, the result is a col-vector (we have r.dim3 =0)
		 * - if l is a row vector and r a matrix with 1 row, the result is a 1-length row vector (we have l.dim2=0 and r.dim3=1)
		 * - if l is a matrix with 1 row and l a column vector, the result is a 1-length column vector (we have l.dim2=1 and r.dim3=0)
		 * - ...
		 */
	}
}

Dim vec_dim(const ExprNode** comp, int n, bool in_a_row) {
	assert (n>0);
	const Dim& d=comp[0]->dim;

	if (d.is_scalar()) {
		if (in_a_row) {
			for (int i=0; i<n; i++)
				// we could allow concatenation of
				// row vectors of different size
				// in a single row vector;
				// (but not implemented yet)
				if (!comp[i]->type()!=Dim::SCALAR) goto error;
			return Dim(0,0,n);
		}
		else {
			for (int i=0; i<n; i++)
				// we could allow concatenation of
				// column vectors of different size
				// in a single column vector;
				// (but not implemented yet)
				if (comp[i]->type()!=Dim::SCALAR) goto error;
			return Dim(0,n,0);
		}
	} else if (d.is_vector()) {
		if (in_a_row) {
			for (int i=0; i<n; i++)
				// same comment as above: we could also
				// put matrices with different number of columns
				// in a row. Not implemented. Only column vectors are accepted
				if (comp[i]->type()!=Dim::COL_VECTOR || comp[i]->dim.dim2!=d.dim2) goto error;
			return Dim(0,d.dim2,n);
		} else {
			for (int i=0; i<n; i++) {
				// same comment as above: we could also
				// put matrices with different number of rows
				// in column. Not implemented. Only row vectors are accepted
				if (comp[i]->type()!=Dim::ROW_VECTOR || comp[i]->dim.dim3!=d.dim3) goto error;
			}
			return Dim(0,n,d.dim3);
		}
	}

	// notice: array of matrix expressions are only used
	// so far in unvectorizing (for symbols corresponding to matrix arrays)
	else if (d.type()==Dim::MATRIX) {
		for (int i=0; i<n; i++)
			if (comp[i]->type()!=Dim::MATRIX || comp[i]->dim.dim2!=d.dim2 || comp[i]->dim.dim3!=d.dim3) goto error;
		return Dim(n,d.dim2,d.dim3);
	}

	error:
	throw NonRecoverableException("Impossible to form a vector with these components");
}

int bin_size(const ExprNode& left, const ExprNode& right) {
	SizeofDAG s(left,right);
	return s.size+1;
}

int nary_size(const ExprNode** args, int n) {
	SizeofDAG s(args,n);
	return s.size+1;
}

} // end anonymous namespace

void ExprNode::reset_visited() const {
	ResetVisited(*this);
}

const ExprNode** ExprNode::subnodes() const {
	return ExprSubNodes(*this).nodes();
}

ExprNode::ExprNode(int height, int size, const Dim& dim) :
  height(height), size(size), id(-1), dim(dim) {

}

bool ExprIndex::indexed_symbol() const {
	// we prefer to use directly a dynamic cast here
	// instead of creating a visitor class (or adding a new field in ExprNode)
	if (dynamic_cast<const ExprSymbol*>(&expr)) return true;

	const ExprIndex* expr_index=dynamic_cast<const ExprIndex*>(&expr);
	if (!expr_index) return false;

	return expr_index->indexed_symbol();
}

ExprNAryOp::ExprNAryOp(const ExprNode** _args, int n, const Dim& dim) :
		ExprNode(max_height(_args,n)+1, nary_size(_args,n), dim), nb_args(n) {

	args = new const ExprNode*[n];
	for (int i=0; i<n; i++)
		args[i]=_args[i];
}

const ExprVector& ExprVector::new_(const ExprNode** comp, int n, bool in_row) {
	return *new ExprVector(comp,n,in_row);
}

const ExprVector& ExprVector::new_(const ExprNode& e1, const ExprNode& e2, bool in_row) {
	const ExprNode** comp=new const ExprNode*[2];
	comp[0]=&e1;
	comp[1]=&e2;
	return *new ExprVector(comp, 2, in_row);
}

ExprVector::ExprVector(const ExprNode** comp, int n, bool in_row) :
		ExprNAryOp(comp, n, vec_dim(comp,n,in_row)) {
}

#define MAX_NAME_SIZE 20
#define BASE_SYMB_NAME "_symb_"

static char generated_name_buff[MAX_NAME_SIZE];
static int generated_count=0;

const ExprSymbol& ExprSymbol::new_(const Dim& dim) {
	sprintf(generated_name_buff, BASE_SYMB_NAME);
	snprintf(&generated_name_buff[strlen(BASE_SYMB_NAME)], MAX_NAME_SIZE-strlen(BASE_SYMB_NAME), "%d", generated_count++);

	return new_(generated_name_buff, dim);
}

ExprConstant::ExprConstant(const Interval& value) : ExprNode(0,1,Dim(0,0,0)), value(1,1) {
	((Interval&) this->value[0][0])=value;
}

ExprConstant::ExprConstant(const IntervalVector& v, bool in_row)
  : ExprNode(0,1, in_row? Dim(0,0,v.size()) : Dim(0,v.size(),0)),
  value(1,v.size()) {
	/* warning: we represent a vector by a row, even if it is a column vector */
	((IntervalVector&) value[0])=v;
}

ExprConstant::ExprConstant(const IntervalMatrix& m)
  : ExprNode(0,1,Dim(0,m.nb_rows(),m.nb_cols())), value(m) {

}

bool ExprConstant::ExprConstant::is_zero() const {
	for (int i=0; i<value.nb_rows(); i++)
		for (int j=0; j<value.nb_cols(); j++)
			if (value[i][j]!=Interval::ZERO) return false;

	return true;
}

ExprBinaryOp::ExprBinaryOp(const ExprNode& left, const ExprNode& right, const Dim& dim) :
		ExprNode(	max_height(left,right)+1,
					bin_size(left,right),
					dim ),
		left(left), right(right) {
}

ExprAdd::ExprAdd(const ExprNode& left, const ExprNode& right) :
				ExprBinaryOp(left,right,left.dim) {
	assert(left.dim == right.dim);
}

ExprMul::ExprMul(const ExprNode& left, const ExprNode& right) :
				ExprBinaryOp(left,right,mul_dim(left,right)) {
}

ExprSub::ExprSub(const ExprNode& left, const ExprNode& right) :
				ExprBinaryOp(left,right,left.dim) {
	assert(left.dim == right.dim);
}

ExprDiv::ExprDiv(const ExprNode& left, const ExprNode& right) :
						ExprBinaryOp(left,right,Dim(0,0,0)) {
	assert(left.type() == Dim::SCALAR);
	assert(right.type() == Dim::SCALAR);
}

ExprMax::ExprMax(const ExprNode& left, const ExprNode& right) :
		ExprBinaryOp(left,right,Dim(0,0,0)) {
	assert(left.type() == Dim::SCALAR);
	assert(right.type() == Dim::SCALAR);
}

ExprMin::ExprMin(const ExprNode& left, const ExprNode& right) :
		ExprBinaryOp(left,right,Dim(0,0,0)) {
	assert(left.type() == Dim::SCALAR);
	assert(right.type() == Dim::SCALAR);
}

ExprAtan2::ExprAtan2(const ExprNode& left, const ExprNode& right) :
			ExprBinaryOp(left,right,Dim(0,0,0)) {
	assert(left.type() == Dim::SCALAR);
	assert(right.type() == Dim::SCALAR);
}

ExprApply::ExprApply(const Function& f, const ExprNode** args) :
		ExprNAryOp(args,f.nb_symbols(),f.expr().dim),
		func(f) {
	for (int i=0; i<f.nb_symbols(); i++)
		assert(args[i]->dim == f.symbol(i).dim);
}

std::ostream& operator<<(std::ostream& os, const ExprNode& expr) {
  ExprPrinter(os).visit(expr);
  return os;
}

} // end namespace ibex
