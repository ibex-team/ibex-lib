//============================================================================
//                                  I B E X                                   
// File        : ibex_ConstantEvaluator.cpp
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Jun 19, 2012
// Last Update : Jun 19, 2012
//============================================================================

#include "ibex_ConstantGenerator.h"

namespace ibex {

namespace parser {

int to_integer(const Domain& d) {
	assert(d.dim.is_scalar());
	assert(d.i().is_degenerated());
	double x=d.i().mid();
	assert(floor(x)==x);
	return x;
}

int to_double(const Domain& d) {
	assert(d.dim.is_scalar());

	// WARNING: this is quite unsafe. But
	// requiring d.i().is_degenerated() is wrong,
	// the result of a calculus with degenerated intervals
	// may not be degenerated (and large)... Still, we could
	// give some warning if, say, the diameter here was > 1e-10.
	return d.i().mid();
}

ConstantEvaluator::ConstantEvaluator(const Scope& scope) : scope(scope ){

}

ConstantEvaluator::~ConstantEvaluator() {

}

Domain ConstantEvaluator::eval(const ExprNode& expr) {
	visit(expr);
	Domain r=*((const Domain*) expr.deco.tmp);
	delete (Domain*) expr.deco.tmp;
	return r;
}

int ConstantEvaluator::eval_integer(const ExprNode& expr) {
	return to_integer(eval(expr));
}

double ConstantEvaluator::eval_double(const ExprNode& expr) {
	return to_double(eval(expr));
}

void ConstantEvaluator::visit(const ExprNode& e) {
	e.acceptVisitor(*this);
}

void ConstantEvaluator::visit(const ExprIndex& i) {
	assert(false); // only P_ExprIndex appears at parse time.
}

void ConstantEvaluator::visit(const ExprSymbol& x) {
	assert(false); // only P_ExprSymbol appears at parse time.
}

void ConstantEvaluator::visit(const ExprConstant& c) {
	Domain* d=new Domain(c.dim);
	switch(c.dim.type()) {
	case Dim::SCALAR:     d->i()=c.get_value(); break;
	case Dim::ROW_VECTOR:
	case Dim::COL_VECTOR: d->v()=c.get_vector_value(); break;
	case Dim::MATRIX:     d->m()=c.get_matrix_value(); break;
	default: assert(false); break;
	}
	c.deco.tmp = d;
}

void ConstantEvaluator::visit(const ExprNAryOp& e) {
	e.acceptVisitor(*this);
}

void ConstantEvaluator::visit(const ExprBinaryOp& b) {
	b.acceptVisitor(*this);
}

void ConstantEvaluator::visit(const ExprUnaryOp& u) {
	u.acceptVisitor(*this);
}

void ConstantEvaluator::visit(const ExprVector& e) {
	Domain* d= new Domain(e.dim);
	assert(d->dim.is_vector() || d->dim.type()==Dim::MATRIX);
	// we forbid a "vector operation" between constants
	// to yield an array of matrices

	for (int i=0; i<e.nb_args; i++) {
		const Domain& di=(*((const Domain*) e.arg(i).deco.tmp));
		if (d->dim.is_vector()) d->v()[i]=di.i();
		else d->m()[i]=di.v();
	}
	e.deco.tmp = d;
}

void ConstantEvaluator::visit(const ExprApply& e) {
	assert(false); // TO DO
}

namespace {

// unary domain function
typedef Domain (*dom_func)(const Domain&);
// binary domain function
typedef Domain (*dom_func2)(const Domain&, const Domain&);

void unary_eval(const ExprUnaryOp& e, dom_func f) {
	Domain  dx = *((const Domain*) e.expr.deco.tmp);
	delete (Domain*) e.expr.deco.tmp;
	Domain* dy = new Domain(e.dim);
	*dy = f(dx);
	e.deco.tmp = dy;
}

void binary_eval(const ExprBinaryOp& e, dom_func2 f) {
	Domain  dx1 = *((const Domain*) e.left.deco.tmp);
	Domain  dx2 = *((const Domain*) e.right.deco.tmp);
	delete (Domain*) e.left.deco.tmp;
	delete (Domain*) e.right.deco.tmp;
	Domain* dy = new Domain(e.dim);
	*dy = f(dx1,dx2);
	e.deco.tmp = dy;
}

// the index under the form of dom_func2
Domain _index(const Domain& d, const Domain& idx) {
	return d[to_integer(idx)];
}

// the power under the form of dom_func
Domain _power(const Domain& d, const Domain& expon) {
	return pow(d,to_integer(expon));
}
// =====================================

}

void ConstantEvaluator::visit(const ExprAdd& e)     { visit(e.left); visit(e.right); binary_eval(e,operator+); }
void ConstantEvaluator::visit(const ExprMul& e)     { visit(e.left); visit(e.right); binary_eval(e,operator*); }
void ConstantEvaluator::visit(const ExprSub& e)     { visit(e.left); visit(e.right); binary_eval(e,operator-); }
void ConstantEvaluator::visit(const ExprDiv& e)     { visit(e.left); visit(e.right); binary_eval(e,operator/); }
void ConstantEvaluator::visit(const ExprMax& e)     { visit(e.left); visit(e.right); binary_eval(e,max); }
void ConstantEvaluator::visit(const ExprMin& e)     { visit(e.left); visit(e.right); binary_eval(e,min); }
void ConstantEvaluator::visit(const ExprAtan2& e)   { visit(e.left); visit(e.right); binary_eval(e,atan2); }
void ConstantEvaluator::visit(const P_ExprIndex& e) { visit(e.left); visit(e.right); binary_eval(e,_index); }
void ConstantEvaluator::visit(const P_ExprPower& e) { visit(e.left); visit(e.right); binary_eval(e,_power); }

void ConstantEvaluator::visit(const ExprMinus& e) { visit(e.expr); unary_eval(e,operator-); }
void ConstantEvaluator::visit(const ExprSign& e)  { visit(e.expr); unary_eval(e,sign); }
void ConstantEvaluator::visit(const ExprAbs& e)   { visit(e.expr); unary_eval(e,abs); }
void ConstantEvaluator::visit(const ExprPower& e) { assert(false); /* only P_ExprPower possible */ }
void ConstantEvaluator::visit(const ExprSqr& e)   { visit(e.expr); unary_eval(e,sqr); }
void ConstantEvaluator::visit(const ExprSqrt& e)  { visit(e.expr); unary_eval(e,sqrt); }
void ConstantEvaluator::visit(const ExprExp& e)   { visit(e.expr); unary_eval(e,exp); }
void ConstantEvaluator::visit(const ExprLog& e)   { visit(e.expr); unary_eval(e,log); }
void ConstantEvaluator::visit(const ExprCos& e)   { visit(e.expr); unary_eval(e,cos); }
void ConstantEvaluator::visit(const ExprSin& e)   { visit(e.expr); unary_eval(e,sin); }
void ConstantEvaluator::visit(const ExprTan& e)   { visit(e.expr); unary_eval(e,tan); }
void ConstantEvaluator::visit(const ExprCosh& e)  { visit(e.expr); unary_eval(e,cosh); }
void ConstantEvaluator::visit(const ExprSinh& e)  { visit(e.expr); unary_eval(e,sinh); }
void ConstantEvaluator::visit(const ExprTanh& e)  { visit(e.expr); unary_eval(e,tanh); }
void ConstantEvaluator::visit(const ExprAcos& e)  { visit(e.expr); unary_eval(e,acos); }
void ConstantEvaluator::visit(const ExprAsin& e)  { visit(e.expr); unary_eval(e,asin); }
void ConstantEvaluator::visit(const ExprAtan& e)  { visit(e.expr); unary_eval(e,atan); }
void ConstantEvaluator::visit(const ExprAcosh& e) { visit(e.expr); unary_eval(e,acosh); }
void ConstantEvaluator::visit(const ExprAsinh& e) { visit(e.expr); unary_eval(e,asinh); }
void ConstantEvaluator::visit(const ExprAtanh& e) { visit(e.expr); unary_eval(e,atanh); }



void ConstantEvaluator::visit(const P_ExprSymbol& x) {

	if (scope.is_iter_symbol(x.name)) {
		Domain* d= new Domain(x.dim);
		d->i()=Interval(scope.get_iter_value(x.name));
		x.deco.tmp=d;
	} else if (scope.is_cst_symbol(x.name)) {
		x.deco.tmp = new Domain(scope.get_cst(x.name),true); // reference
	} else {
		assert(false); // cannot use a variable in a constant expression.
	}

}

} // end namespace parser
} // end namespace ibex
