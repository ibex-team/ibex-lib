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

//namespace {

int max_height(const ExprNode& n1, const ExprNode& n2) {
	if (n1.height>n2.height) return n1.height;
	else return n2.height;
}

int max_height(int n, const ExprNode** args) {
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
		if (r.dim3==0)
			throw NonRecoverableException("Cannot right-multiply by a scalar");
		if (r.dim2==0)  // matrix-vector multiplication
			if (l.dim2==0) {
				if (r.dim3!=l.dim3)
					throw NonRecoverableException("Mismatched dimensions in dot product");
				else
					return Dim(0,0,0); // dot product
			} else if (l.dim3!=r.dim3)
				throw NonRecoverableException("Vector dimension does not match the number of matrix columns in matrix-vector multiplication");
			else return Dim(0,0,l.dim2);
		else
			if (l.dim3!=r.dim2) throw NonRecoverableException("Mismatched dimensions in matrix multiplication");
			else return Dim(0,l.dim2,r.dim3);
	}
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
	virtual void visit(const ExprVector& e)   { for (int i=0; i<e.size(); i++) visit(e.get(i)); }
	virtual void visit(const ExprSymbol& e)   { }
	virtual void visit(const ExprConstant& e) { }
	virtual void visit(const ExprUnaryOp& e)  { visit(e.expr); }
	virtual void visit(const ExprBinaryOp& e) { visit(e.left); visit(e.right); }
	virtual void visit(const ExprApply& e)    { for (int i=0; i<e.nb_args(); i++) visit(e.arg(i)); }

	std::set<int> visited;
};

int bin_size(const ExprNode& left, const ExprNode& right) {
	SizeofDAG s(left,right);
	return s.size;
}

int apply_size(const ExprNode** args, int n) {
	SizeofDAG s(args,n);
	return s.size;
//}

} // end anonymous namespace

ExprNode::ExprNode(Function& env, int height, int size, const Dim& dim) :
  context(env), height(height), size(size), id(context.nb_nodes()), dim(dim), deco(NULL) {
  env.add_node(*this);
}

ExprBinaryOp::ExprBinaryOp(const ExprNode& left, const ExprNode& right, const Dim& dim) :
		ExprNode(	left.context,
					max_height(left,right)+1,
					bin_size(left,right)+1,
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
		ExprNode(	args[0]->context,
					max_height(f.nb_symbols(),args),
					apply_size(args,f.nb_symbols()),
					f.expr().dim ),
		func(f), args(args ) {
}

int ExprApply::nb_args() const {
	return func.nb_symbols();
}

std::ostream& operator<<(std::ostream& os, const ExprNode& expr) {
  ExprPrinter(os).visit(expr);
  return os;
}

} // end namespace ibex
