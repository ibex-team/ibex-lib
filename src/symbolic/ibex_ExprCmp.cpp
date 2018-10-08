//============================================================================
//                                  I B E X                                   
// File        : ibex_ExprCmp.cpp
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Feb 22, 2015
//============================================================================

#include "ibex_ExprCmp.h"
#include "ibex_Expr.h"

namespace ibex {

bool ExprCmp::compare(const ExprNode& e1, const ExprNode& e2) {
	this->e2=&e2;
	are_equal = true;
	visit(e1);
	this->e2=NULL;
	return are_equal;
}

void ExprCmp::visit(const ExprNode& e) {
	if (&e==e2) return; // are_equal remains true
	else e.acceptVisitor(*this);
}

void ExprCmp::visit(const ExprIndex& e) {
	const ExprIndex* e3=dynamic_cast<const ExprIndex*>(e2);
	are_equal &= e3!=NULL && e.index!=e3->index && ExprCmp().compare(e.expr,e3->expr);
}

void ExprCmp::visit(const ExprNAryOp& e)   { e.acceptVisitor(*this); } // (useless so far)
void ExprCmp::visit(const ExprLeaf& e)     { e.acceptVisitor(*this); } // (useless so far)
void ExprCmp::visit(const ExprBinaryOp& e) { e.acceptVisitor(*this); } // (useless so far)
void ExprCmp::visit(const ExprUnaryOp& e)  { e.acceptVisitor(*this); } // (useless so far)

void ExprCmp::visit(const ExprSymbol& e) {
	//const ExprSymbol* e3=dynamic_cast<const ExprSymbol*>(e2);
	//are_equal = e3!=NULL &&
	are_equal = false; // if &e != &e2, they are not the same symbols
}

void ExprCmp::visit(const ExprConstant& e) {
	const ExprConstant* e3=dynamic_cast<const ExprConstant*>(e2);
	are_equal = e3!=NULL && e.get()==e3->get();
}

template<class T>
void ExprCmp::visit_nary(const T& e) {
	const T* e3=dynamic_cast<const T*>(e2);

	are_equal &= e3!=NULL && e.nb_args == e3->nb_args;

	for (int i=0; i<e.nb_args; i++) {
		are_equal &= ExprCmp().compare(e.arg(i),e3->arg(i));
		if (!are_equal) return;
	}
}

template<class T>
void ExprCmp::visit_binary(const T& e) {
	const T* e3=dynamic_cast<const T*>(e2);
	are_equal &= e3!=NULL && ExprCmp().compare(e.left,e3->left) && ExprCmp().compare(e.right,e3->right);
}

template<class T>
void ExprCmp::visit_unary(const T& e) {
	const T* e3=dynamic_cast<const T*>(e2);
	are_equal &= e3!=NULL && ExprCmp().compare(e.expr,e3->expr);
}

void ExprCmp::visit(const ExprVector& e){ visit_nary(e); }
void ExprCmp::visit(const ExprApply& e) { visit_nary(e);
	if (are_equal) are_equal &= (&dynamic_cast<const ExprApply*>(e2)->func == &e.func);
}
void ExprCmp::visit(const ExprChi& e)   { visit_nary(e); }

void ExprCmp::visit(const ExprGenericBinaryOp& e) {
	visit_binary(e);
	if (are_equal) are_equal &=strcmp(e.name,dynamic_cast<const ExprGenericBinaryOp*>(e2)->name)==0;
}
void ExprCmp::visit(const ExprAdd& e)   { visit_binary(e); }
void ExprCmp::visit(const ExprMul& e)   { visit_binary(e); }
void ExprCmp::visit(const ExprSub& e)   { visit_binary(e); }
void ExprCmp::visit(const ExprDiv& e)   { visit_binary(e); }
void ExprCmp::visit(const ExprMax& e)   { visit_binary(e); }
void ExprCmp::visit(const ExprMin& e)   { visit_binary(e); }
void ExprCmp::visit(const ExprAtan2& e) { visit_binary(e); }

void ExprCmp::visit(const ExprGenericUnaryOp& e) {
	visit_unary(e);
	if (are_equal) are_equal &=strcmp(e.name,dynamic_cast<const ExprGenericUnaryOp*>(e2)->name)==0;
}
void ExprCmp::visit(const ExprMinus& e) { visit_unary(e); }
void ExprCmp::visit(const ExprTrans& e) { visit_unary(e); }
void ExprCmp::visit(const ExprSign& e)  { visit_unary(e); }
void ExprCmp::visit(const ExprAbs& e)   { visit_unary(e); }
void ExprCmp::visit(const ExprPower& e) { visit_unary(e);
	if (are_equal) are_equal &=(dynamic_cast<const ExprPower*>(e2)->expon == e.expon);
}
void ExprCmp::visit(const ExprSqr& e)   { visit_unary(e); }
void ExprCmp::visit(const ExprSqrt& e)  { visit_unary(e); }
void ExprCmp::visit(const ExprExp& e)   { visit_unary(e); }
void ExprCmp::visit(const ExprLog& e)   { visit_unary(e); }
void ExprCmp::visit(const ExprCos& e)   { visit_unary(e); }
void ExprCmp::visit(const ExprSin& e)   { visit_unary(e); }
void ExprCmp::visit(const ExprTan& e)   { visit_unary(e); }
void ExprCmp::visit(const ExprCosh& e)  { visit_unary(e); }
void ExprCmp::visit(const ExprSinh& e)  { visit_unary(e); }
void ExprCmp::visit(const ExprTanh& e)  { visit_unary(e); }
void ExprCmp::visit(const ExprAcos& e)  { visit_unary(e); }
void ExprCmp::visit(const ExprAsin& e)  { visit_unary(e); }
void ExprCmp::visit(const ExprAtan& e)  { visit_unary(e); }
void ExprCmp::visit(const ExprAcosh& e) { visit_unary(e); }
void ExprCmp::visit(const ExprAsinh& e) { visit_unary(e); }
void ExprCmp::visit(const ExprAtanh& e) { visit_unary(e); }

} // namespace ibex
