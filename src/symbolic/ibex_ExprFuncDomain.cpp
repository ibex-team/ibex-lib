//============================================================================
//                                  I B E X                                   
// File        : ibex_ExprFuncDomain.cpp
// Author      : Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : May 06, 2020
//============================================================================

#include "ibex_ExprFuncDomain.h"
#include "ibex_System.h"

using namespace std;

namespace ibex {


ExprFuncDomain::ExprFuncDomain(const Function& f) {
	for (int i=0; i<f.nb_arg(); i++)
		fac.add_var(f.arg(i));

	visit(f.expr());

	sys = new System(fac);

	for (vector<const ExprNode*>::iterator it=garbage.begin(); it!=garbage.end(); ++it)
		delete *it;
}

System* ExprFuncDomain::get() {
	return sys;
}

void ExprFuncDomain::visit(const ExprNode& e) {
	ExprVisitor<void>::visit(e);
}

void ExprFuncDomain::visit(const ExprIndex& e) { visit(e.expr);}

void ExprFuncDomain::visit(const ExprSymbol& e) { }

void ExprFuncDomain::visit(const ExprConstant& e) { }

void ExprFuncDomain::visit(const ExprVector& e) { for (int i=0; i<e.nb_args; i++) visit(e.arg(i)); }

void ExprFuncDomain::visit(const ExprApply& e)  { for (int i=0; i<e.nb_args; i++) visit(e.arg(i)); }

void ExprFuncDomain::visit(const ExprChi& e)    { for (int i=0; i<e.nb_args; i++) visit(e.arg(i));  }

void ExprFuncDomain::visit(const ExprGenericBinaryOp& e) { visit(e.left); visit(e.right); }

void ExprFuncDomain::visit(const ExprAdd& e) { visit(e.left); visit(e.right); }

void ExprFuncDomain::visit(const ExprMul& e) { visit(e.left); visit(e.right); }

void ExprFuncDomain::visit(const ExprSub& e) { visit(e.left); visit(e.right); }

void ExprFuncDomain::visit(const ExprDiv& e) { visit(e.left); visit(e.right); }

void ExprFuncDomain::visit(const ExprMax& e) { visit(e.left); visit(e.right); }

void ExprFuncDomain::visit(const ExprMin& e) { visit(e.left); visit(e.right); }

void ExprFuncDomain::visit(const ExprAtan2& e) { visit(e.left); visit(e.right); }

void ExprFuncDomain::visit(const ExprGenericUnaryOp& e) { visit(e.expr); }

void ExprFuncDomain::visit(const ExprMinus& e) { visit(e.expr); }

void ExprFuncDomain::visit(const ExprTrans& e) { visit(e.expr); }

void ExprFuncDomain::visit(const ExprSign& e)  { visit(e.expr); }

void ExprFuncDomain::visit(const ExprAbs& e)   { visit(e.expr); }

void ExprFuncDomain::visit(const ExprPower& e) { visit(e.expr); }

void ExprFuncDomain::visit(const ExprSqr& e)   { visit(e.expr); }

void ExprFuncDomain::visit(const ExprSqrt& e) {
	visit(e.expr);
	fac.add_ctr(ExprCtr(e.expr,GEQ)); // no new node created -> no garbage
}

void ExprFuncDomain::visit(const ExprExp& e) { visit(e.expr); }

void ExprFuncDomain::visit(const ExprLog& e) {
	visit(e.expr);
	fac.add_ctr(ExprCtr(e.expr,GEQ)); // no new node created -> no garbage
}

void ExprFuncDomain::visit(const ExprCos& e)  { visit(e.expr);}

void ExprFuncDomain::visit(const ExprSin& e)  { visit(e.expr);}

void ExprFuncDomain::visit(const ExprTan& e)  { visit(e.expr);}

void ExprFuncDomain::visit(const ExprCosh& e) { visit(e.expr);}

void ExprFuncDomain::visit(const ExprSinh& e) { visit(e.expr);}

void ExprFuncDomain::visit(const ExprTanh& e) { visit(e.expr);}

void ExprFuncDomain::visit(const ExprAcos& e) {
	visit(e.expr);

	const ExprCtr& c1=e.expr>=-1;
	fac.add_ctr(c1);
	garbage.push_back(&c1.e);
	delete &c1;

	const ExprCtr& c2=e.expr<=1;
	fac.add_ctr(c2);
	garbage.push_back(&c2.e);
	delete &c2;
}

void ExprFuncDomain::visit(const ExprAsin& e) {
	visit(e.expr);

	const ExprCtr& c1=e.expr>=-1;
	fac.add_ctr(c1);
	garbage.push_back(&c1.e);
	delete &c1;

	const ExprCtr& c2=e.expr<=1;
	fac.add_ctr(c2);
	garbage.push_back(&c2.e);
	delete &c2;
}

void ExprFuncDomain::visit(const ExprAtan& e) { visit(e.expr);}

void ExprFuncDomain::visit(const ExprAcosh& e) {
	visit(e.expr);

	const ExprCtr& c=e.expr>=1;
	fac.add_ctr(c);
	garbage.push_back(&c.e);
	delete &c;
}

void ExprFuncDomain::visit(const ExprAsinh& e) { visit(e.expr); }

void ExprFuncDomain::visit(const ExprAtanh& e) {
	visit(e.expr);

	const ExprCtr& c1=e.expr>=-1;
	fac.add_ctr(c1);
	garbage.push_back(&c1.e);
	delete &c1;

	const ExprCtr& c2=e.expr<=1;
	fac.add_ctr(c2);
	garbage.push_back(&c2.e);
	delete &c2;
}

void ExprFuncDomain::visit(const ExprFloor& e) { visit(e.expr); }

void ExprFuncDomain::visit(const ExprCeil& e)  { visit(e.expr); }

void ExprFuncDomain::visit(const ExprSaw& e)   { visit(e.expr); }

} // end namespace ibex
