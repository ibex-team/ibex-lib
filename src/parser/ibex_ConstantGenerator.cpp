//============================================================================
//                                  I B E X                                   
// File        : ibex_ConstantEvaluator.cpp
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Jun 19, 2012
// Last Update : Jun 19, 2012
//============================================================================

#include <sstream>

#include "ibex_ConstantGenerator.h"
#include "ibex_SyntaxError.h"

extern void ibexerror (const std::string& msg);

using namespace std;

namespace ibex {

namespace parser {

int to_integer(const Domain& d) {
	assert(d.dim.is_scalar());
	assert(d.i().is_degenerated());
	double x=d.i().mid();
	assert(floor(x)==x);
	return x;
}

double to_double(const Domain& d) {
	assert(d.dim.is_scalar());
	// WARNING: this is quite unsafe. But
	// requiring d.i().is_degenerated() is wrong,
	// the result of a calculus with degenerated intervals
	// may not be degenerated (and large)... Still, we could
	// give some warning if, say, the diameter here was > 1e-10.
	return d.i().mid();
}

ConstantGenerator::ConstantGenerator(const Scope& scope) : scope(scope ){

}

ConstantGenerator::~ConstantGenerator() {

}

Domain ConstantGenerator::eval(const ExprNode& expr) {
	visit(expr);
	Domain r=*((const Domain*) expr.deco.tmp);
	delete (Domain*) expr.deco.tmp;
	return r;
}

int ConstantGenerator::eval_integer(const ExprNode& expr) {
	return to_integer(eval(expr));
}

double ConstantGenerator::eval_double(const ExprNode& expr) {
	return to_double(eval(expr));
}

void ConstantGenerator::visit(const ExprNode& e) {
	e.acceptVisitor(*this);
}

void ConstantGenerator::visit(const ExprIndex& i) {
	assert(false); // only P_ExprIndex appears at parse time.
}

void ConstantGenerator::visit(const ExprLeaf& x) {
	x.acceptVisitor(*this);
}

void ConstantGenerator::visit(const ExprSymbol& x) {
	stringstream s;
	s << "Cannot use symbol\"" << x.name << "\" inside a constant expression";
	ibexerror(s.str());
}

void ConstantGenerator::visit(const ExprConstant& c) {
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

void ConstantGenerator::visit(const ExprNAryOp& e) {
	e.acceptVisitor(*this);
}

void ConstantGenerator::visit(const ExprBinaryOp& b) {
	b.acceptVisitor(*this);
}

void ConstantGenerator::visit(const ExprUnaryOp& u) {
	u.acceptVisitor(*this);
}

void ConstantGenerator::visit(const ExprVector& e) {
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

void ConstantGenerator::visit(const ExprApply& e) {
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

void ConstantGenerator::visit(const ExprAdd& e)     { visit(e.left); visit(e.right); binary_eval(e,operator+); }
void ConstantGenerator::visit(const ExprMul& e)     { visit(e.left); visit(e.right); binary_eval(e,operator*); }
void ConstantGenerator::visit(const ExprSub& e)     { visit(e.left); visit(e.right); binary_eval(e,operator-); }
void ConstantGenerator::visit(const ExprDiv& e)     { visit(e.left); visit(e.right); binary_eval(e,operator/); }
void ConstantGenerator::visit(const ExprMax& e)     { visit(e.left); visit(e.right); binary_eval(e,max); }
void ConstantGenerator::visit(const ExprMin& e)     { visit(e.left); visit(e.right); binary_eval(e,min); }
void ConstantGenerator::visit(const ExprAtan2& e)   { visit(e.left); visit(e.right); binary_eval(e,atan2); }
void ConstantGenerator::visit(const P_ExprIndex& e) { visit(e.left); visit(e.right); binary_eval(e,_index); }
void ConstantGenerator::visit(const P_ExprPower& e) { visit(e.left); visit(e.right); binary_eval(e,_power); }

void ConstantGenerator::visit(const ExprMinus& e) { visit(e.expr); unary_eval(e,operator-); }
void ConstantGenerator::visit(const ExprSign& e)  { visit(e.expr); unary_eval(e,sign); }
void ConstantGenerator::visit(const ExprAbs& e)   { visit(e.expr); unary_eval(e,abs); }
void ConstantGenerator::visit(const ExprPower& e) { assert(false); /* only P_ExprPower possible */ }
void ConstantGenerator::visit(const ExprSqr& e)   { visit(e.expr); unary_eval(e,sqr); }
void ConstantGenerator::visit(const ExprSqrt& e)  { visit(e.expr); unary_eval(e,sqrt); }
void ConstantGenerator::visit(const ExprExp& e)   { visit(e.expr); unary_eval(e,exp); }
void ConstantGenerator::visit(const ExprLog& e)   { visit(e.expr); unary_eval(e,log); }
void ConstantGenerator::visit(const ExprCos& e)   { visit(e.expr); unary_eval(e,cos); }
void ConstantGenerator::visit(const ExprSin& e)   { visit(e.expr); unary_eval(e,sin); }
void ConstantGenerator::visit(const ExprTan& e)   { visit(e.expr); unary_eval(e,tan); }
void ConstantGenerator::visit(const ExprCosh& e)  { visit(e.expr); unary_eval(e,cosh); }
void ConstantGenerator::visit(const ExprSinh& e)  { visit(e.expr); unary_eval(e,sinh); }
void ConstantGenerator::visit(const ExprTanh& e)  { visit(e.expr); unary_eval(e,tanh); }
void ConstantGenerator::visit(const ExprAcos& e)  { visit(e.expr); unary_eval(e,acos); }
void ConstantGenerator::visit(const ExprAsin& e)  { visit(e.expr); unary_eval(e,asin); }
void ConstantGenerator::visit(const ExprAtan& e)  { visit(e.expr); unary_eval(e,atan); }
void ConstantGenerator::visit(const ExprAcosh& e) { visit(e.expr); unary_eval(e,acosh); }
void ConstantGenerator::visit(const ExprAsinh& e) { visit(e.expr); unary_eval(e,asinh); }
void ConstantGenerator::visit(const ExprAtanh& e) { visit(e.expr); unary_eval(e,atanh); }


void ConstantGenerator::visit(const ExprIter& x) {
	Domain* d= new Domain(x.dim);
	d->i()=Interval(scope.get_iter_value(x.name));
	x.deco.tmp=d;
}

//void ConstantGenerator::visit(const ExprEntity& x) {
//	stringstream s;
//	s << "Cannot use symbol\"" << x.entity.name << "\" inside a constant expression";
//	throw new SyntaxError(s.str(),"",x.line);
//}

} // end namespace parser
} // end namespace ibex
