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
#include "ibex_Eval.h"

extern void ibexerror (const std::string& msg);

using namespace std;

namespace {
// do not use POS_INFINITY to avoid confusion
enum { NEG_INF, POS_INF, OTHER };

#define NOT_INF if (number_type!=OTHER) throw SyntaxError("Unexpected infinity symbol (\"oo\")")

}

namespace ibex {

namespace parser {

int to_integer(const Domain& d) {
	assert(d.dim.is_scalar());
	assert(d.i().is_degenerated());
	double x=d.i().mid();
	assert(floor(x)==x);
	return (int)x;
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
	map.clean();
	visit(expr);
	Domain r=*map[expr];
	delete map[expr];
	return r;
}

int ConstantGenerator::eval_integer(const ExprNode& expr) {
	return to_integer(eval(expr));
}

double ConstantGenerator::eval_double(const ExprNode& expr) {
	Domain d=eval(expr);
	switch(number_type) {
	case NEG_INF: return NEG_INFINITY;
	case POS_INF: return POS_INFINITY;
	default:      return to_double(d); //eval(expr));
	}
}

void ConstantGenerator::visit(const ExprNode& e) {
	e.acceptVisitor(*this);
}

void ConstantGenerator::visit(const ExprIndex& i) {
	assert(false); // impossible: only P_ExprIndex appears at generation.
}

void ConstantGenerator::visit(const ExprLeaf& x) {
	x.acceptVisitor(*this);
}

void ConstantGenerator::visit(const ExprSymbol& x) {
	stringstream s;
	s << "Cannot use symbol\"" << x.name << "\" inside a constant expression";
	ibexerror(s.str());
}

void ConstantGenerator::visit(const ExprConstantRef& s) {
	map.insert(s, new Domain(s.value,true));
	number_type = OTHER; // neither -oo nor +oo
}

void ConstantGenerator::visit(const ExprConstant& c) {
	map.insert(c, new Domain(c.get(),false));
//	cout << c.id << " " << c << " " << map[c]->dim << endl;
	number_type = OTHER; // neither -oo nor +oo
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
	// TODO: test
	Domain* d= new Domain(e.dim);
//	assert(d->dim.is_vector() || d->dim.type()==Dim::MATRIX);
	// we forbid a "vector operation" between constants
	// to yield an array of matrices

	for (int i=0; i<e.nb_args; i++) {
		visit(e.arg(i));
		NOT_INF;
		const Domain* di=map[e.arg(i)];
		if (d->dim.is_vector()) d->v()[i]=di->i();
		else if (d->dim.type()==Dim::MATRIX) d->m()[i]=di->v();
		else d->ma()[i]=di->m();
		delete di;
	}

	map.insert(e, d);
}

void ConstantGenerator::visit(const ExprApply& e) {
	// TODO: test
	Domain* d= new Domain(e.dim);
	Array<const Domain> args(e.nb_args);

	for (int i=0; i<e.nb_args; i++) {
		visit(e.arg(i));
		NOT_INF;
		const Domain* di=map[e.arg(i)];
		args.set_ref(i,*di);
	}
	*d=Eval().eval(e.func,args);
	for (int i=0; i<e.nb_args; i++) {
		delete &args[i];
	}
	map.insert(e,d);
}

// unary domain function
typedef Domain (*dom_func)(const Domain&);
// binary domain function
typedef Domain (*dom_func2)(const Domain&, const Domain&);

void ConstantGenerator::unary_eval(const ExprUnaryOp& e, dom_func f) {
    visit(e.expr);
    NOT_INF;
	const Domain* dx = map[e.expr];
	Domain* dy = new Domain(e.dim);
	*dy = f(*dx);
	delete dx;
	map.insert(e, dy);
}

void ConstantGenerator::binary_eval(const ExprBinaryOp& e, dom_func2 f) {
	visit(e.left);
	NOT_INF;
	visit(e.right);
	NOT_INF;
	const Domain* dx1 = map[e.left];
	const Domain* dx2 = map[e.right];
	Domain* dy = new Domain(e.dim);
	*dy = f(*dx1,*dx2);
	delete dx1;
	delete dx2;
	map.insert(e, dy);
}

namespace {
// the power under the form of dom_func
Domain _power(const Domain& d, const Domain& expon) {
	return pow(d,to_integer(expon));
}
}

void ConstantGenerator::visit(const P_ExprIndex& e) {
	visit(e.left);
	NOT_INF;
	visit(e.right);
	NOT_INF;
	const Domain* dx1 = map[e.left];
	const Domain* dx2 = map[e.right];
	if (dx1->is_reference) {
		map.insert(e, new Domain((*dx1)[to_integer(*dx2)]));
		delete dx1; // if dx1 is a reference (ConstExprSymbol, the referenced domain will not be deleted.)
	} else {
		// TODO: check this... nothing inserted?? what is dy created for?
		Domain* dy = new Domain(e.dim);
		*dy = (*dx1)[to_integer(*dx2)]; // by copy
		delete dx1;
	}
	delete dx2;
}

void ConstantGenerator::visit(const ExprAdd& e)     { binary_eval(e,operator+); }
void ConstantGenerator::visit(const ExprMul& e)     { binary_eval(e,operator*); }
void ConstantGenerator::visit(const ExprSub& e)     { binary_eval(e,operator-); }
void ConstantGenerator::visit(const ExprDiv& e)     { binary_eval(e,operator/); }
void ConstantGenerator::visit(const ExprMax& e)     { binary_eval(e,max); }
void ConstantGenerator::visit(const ExprMin& e)     { binary_eval(e,min); }
void ConstantGenerator::visit(const ExprAtan2& e)   { binary_eval(e,atan2); }
void ConstantGenerator::visit(const P_ExprPower& e) { binary_eval(e,_power); }

void ConstantGenerator::visit(const ExprMinus& e) {
	visit(e.expr);

	switch(number_type) {
	case NEG_INF:
		number_type=POS_INF;
		map.insert(e, map[e.expr]); // the empty set
		break;
	case POS_INF:
		number_type=NEG_INF;
		map.insert(e, map[e.expr]); // the empty set
		break;
	default:
		const Domain* dx = map[e.expr];
		Domain* dy = new Domain(e.dim);
		*dy = -*dx;
		delete dx;
		map.insert(e, dy);
		break;
	}
}

void ConstantGenerator::visit(const ExprTrans& e) { unary_eval(e,transpose); }
void ConstantGenerator::visit(const ExprSign& e)  { unary_eval(e,sign); }
void ConstantGenerator::visit(const ExprAbs& e)   { unary_eval(e,abs); }
void ConstantGenerator::visit(const ExprPower& e) { assert(false); /* impossible: only P_ExprPower at generation time. */ }
void ConstantGenerator::visit(const ExprSqr& e)   { unary_eval(e,sqr); }
void ConstantGenerator::visit(const ExprSqrt& e)  { unary_eval(e,sqrt); }
void ConstantGenerator::visit(const ExprExp& e)   { unary_eval(e,exp); }
void ConstantGenerator::visit(const ExprLog& e)   { unary_eval(e,log); }
void ConstantGenerator::visit(const ExprCos& e)   { unary_eval(e,cos); }
void ConstantGenerator::visit(const ExprSin& e)   { unary_eval(e,sin); }
void ConstantGenerator::visit(const ExprTan& e)   { unary_eval(e,tan); }
void ConstantGenerator::visit(const ExprCosh& e)  { unary_eval(e,cosh); }
void ConstantGenerator::visit(const ExprSinh& e)  { unary_eval(e,sinh); }
void ConstantGenerator::visit(const ExprTanh& e)  { unary_eval(e,tanh); }
void ConstantGenerator::visit(const ExprAcos& e)  { unary_eval(e,acos); }
void ConstantGenerator::visit(const ExprAsin& e)  { unary_eval(e,asin); }
void ConstantGenerator::visit(const ExprAtan& e)  { unary_eval(e,atan); }
void ConstantGenerator::visit(const ExprAcosh& e) { unary_eval(e,acosh); }
void ConstantGenerator::visit(const ExprAsinh& e) { unary_eval(e,asinh); }
void ConstantGenerator::visit(const ExprAtanh& e) { unary_eval(e,atanh); }

void ConstantGenerator::visit(const ExprIter& x) {
	number_type=OTHER;
	Domain* d= new Domain(x.dim);
	d->i()=Interval(scope.get_iter_value(x.name));
	map.insert(x,d);
}

void ConstantGenerator::visit(const ExprInfinity& x) {
	number_type=POS_INF;
	map.insert(x, new Domain(Dim::scalar()));
	map[x]->i()=Interval::EMPTY_SET;
}

//void ConstantGenerator::visit(const ExprEntity& x) {
//	stringstream s;
//	s << "Cannot use symbol\"" << x.entity.name << "\" inside a constant expression";
//	throw new SyntaxError(s.str(),"",x.line);
//}

} // end namespace parser
} // end namespace ibex
