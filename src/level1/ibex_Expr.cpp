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
#include <limits.h>
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
		else if (l.dim3!=r.dim2)
			throw NonRecoverableException("Mismatched dimensions in matrix multiplication");
		else return Dim(0,l.dim2,r.dim3);
		/* will work in all cases:
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

	if (d.type()==Dim::SCALAR) {
		if (in_a_row) {
			for (int i=0; i<n; i++)
				// we could allow concatenation of
				// row vectors of different size
				// in a single row vector;
				// (but not implemented yet)
				if (!comp[i]->type()==Dim::SCALAR) goto error;
			return Dim(0,0,n);
		}
		else {
			for (int i=0; i<n; i++)
				// we could allow concatenation of
				// column vectors of different size
				// in a single column vector;
				// (but not implemented yet)
				if (comp[i]->type()==Dim::SCALAR) goto error;
			return Dim(0,n,0);
		}
	} else {
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
	error:
	throw NonRecoverableException("Impossible to form a vector with these components");
}

class SizeofDAG : public FunctionVisitor {
public:
	/* for binary expressions (BinOpExpr). */
	SizeofDAG(const ExprNode& l, const ExprNode& r) : size(0) {
		visit(l);
		visit(r);
	}

	/* for n-ary expressions (Apply). */
	SizeofDAG(const ExprNode** args, int n) : size(0) {
		for (int i=0; i<n; i++)
			visit(*args[i]);
	}

	int size;

private:
	virtual void visit(const ExprNode& e) {
		if (visited.find(e.id)==visited.end()) {
			visited.insert(e.id);
			size++;
			e.acceptVisitor(*this);
		}
	}

	virtual void visit(const ExprIndex& e)    { visit(e.expr); }
	virtual void visit(const ExprSymbol& e)   { }
	virtual void visit(const ExprConstant& e) { }
	virtual void visit(const ExprNAryOp& e)   { for (int i=0; i<e.nb_args; i++) visit(e.arg(i)); }
	virtual void visit(const ExprBinaryOp& e) { visit(e.left); visit(e.right); }
	virtual void visit(const ExprUnaryOp& e)  { visit(e.expr); }

	std::set<int> visited;
};

int bin_size(const ExprNode& left, const ExprNode& right) {
	SizeofDAG s(left,right);
	return s.size+1;
}

int nary_size(const ExprNode** args, int n) {
	SizeofDAG s(args,n);
	return s.size+1;
}

} // end anonymous namespace

ExprNode::ExprNode(Function& env, int height, int size, const Dim& dim) :
  context(env), height(height), size(size), id(context.nb_nodes()), dim(dim), deco(NULL) {
  env.add_node(*this);
}

ExprNAryOp::ExprNAryOp(const ExprNode** _args, int n, const Dim& dim) :
		ExprNode(_args[0]->context, max_height(_args,n)+1, nary_size(_args,n), dim), nb_args(n) {

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

ExprConstant::ExprConstant(Function& expr, const Interval& value) : ExprNode(expr,0,1,Dim(0,0,0)), value(1,1) {
	((Interval&) this->value[0][0])=value;
}

ExprConstant::ExprConstant(Function& expr, const IntervalVector& v, bool in_row)
  : ExprNode(expr,0,1, in_row? Dim(0,0,v.size()) : Dim(0,v.size(),0)),
  value(1,v.size()) {
	/* warning: we represent a vector by a row, even if it is a column vector */
	((IntervalVector&) value[0])=v;
}

ExprConstant::ExprConstant(Function& expr, const IntervalMatrix& m)
  : ExprNode(expr,0,1,Dim(0,m.nb_rows(),m.nb_cols())), value(m) {

}

bool ExprConstant::ExprConstant::is_zero() const {
	for (int i=0; i<value.nb_rows(); i++)
		for (int j=0; j<value.nb_cols(); j++)
			if (value[i][j]!=Interval::ZERO) return false;

	return true;
}

ExprBinaryOp::ExprBinaryOp(const ExprNode& left, const ExprNode& right, const Dim& dim) :
		ExprNode(	left.context,
					max_height(left,right)+1,
					bin_size(left,right),
					dim ),
		left(left), right(right) {

	assert(&left.context == &right.context);
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
