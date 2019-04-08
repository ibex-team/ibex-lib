//============================================================================
//                                  I B E X                                   
// File        : ibex_Expr2DAG.cpp
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Feb 22, 2015
//============================================================================

#include "ibex_Expr2DAG.h"
#include "ibex_ExprSubNodes.h"

using namespace std;

namespace ibex {

const ExprNode& Expr2DAG::transform(const Array<const ExprSymbol>& old_x, const Array<const ExprNode>& new_x, const ExprNode& y) {
	ExprSubNodes nodes(old_x,y);

	assert(new_x.size()>=old_x.size());

	// we first deal with the symbols
	for (int i=0; i<old_x.size(); i++) {
		peer.insert(old_x[i], &new_x[i]);
	}

	int i=nodes.size()-1;
	int h=0; // height in the DAG

	// then we copy the constants
	while (i>=0 && nodes[i].height==h) {
		const ExprConstant* c=dynamic_cast<const ExprConstant*>(&nodes[i]);
		if (c) peer.insert(nodes[i], (const ExprNode*) &c->copy());
		i--;
	}

	if (i==-1)
		return *peer[nodes[0]];

	// we proceed by "level", where a level corresponds
	// to a height "h" in the expression.
	// - i is the index of to the first node in the level
	// - i2 is the index of the current node in the level

	while (i>0) {
		int h=nodes[i].height;

		// we first temporarily build all the peer nodes
		// from the new nodes of the sub-level
		int i2=i;
		//cout << "---- level h=" << h << "----" << endl;
		//cout << "---- copy ----" << endl;
		while (nodes[i2].height==h) {
			//cout << "i2=" << i2 << " : " << nodes[i2] << endl;
			assert(!peer.found(nodes[i2]));
			visit(nodes[i2]);
			i2--;
		}
		//cout << "---- merge ----" << endl;
		// then we look for equivalent nodes (at the same level)
		i2=i;
		while (nodes[i2].height==h) {
			//cout << "i2=" << i2 << " : " << nodes[i2];
			// find the same node among the nodes already visited
			for (int i3=i; i3>i2; i3--) {
				// then replace the peer node.
				if (*peer[nodes[i3]]==*peer[nodes[i2]]) {
					//cout << "... merge!";
					peer[nodes[i2]]=peer[nodes[i3]];
					break;
				}
			}
			//cout << endl;
			i2--;
		}
		//cout << "-------------" << endl;

		i=i2; // jump to the next level
	}

	assert(i==0);
	visit(nodes[0]);
	return *peer[nodes[0]];
}

Array<const ExprNode> Expr2DAG::comps(const ExprNAryOp& e) {

	Array<const ExprNode> tab(e.nb_args);
	for (int i=0; i<e.nb_args; i++) {
		tab.set_ref(i,*peer[e.arg(i)]);
	}
	return tab;
}

template<class T>
void Expr2DAG::visit_binary(const T& e) {
	peer[e]=&T::new_(*peer[e.left],*peer[e.right]);
}

template<class T>
void Expr2DAG::visit_unary(const T& e) {
	peer[e]=&T::new_(*peer[e.expr]);
}

void Expr2DAG::visit(const ExprNode& e) { e.acceptVisitor(*this); }
void Expr2DAG::visit(const ExprIndex& i) { /* TODO: nothing here?? */ }

void Expr2DAG::visit(const ExprNAryOp& e)   { e.acceptVisitor(*this); } // (useless so far)
void Expr2DAG::visit(const ExprLeaf& e)     { e.acceptVisitor(*this); } // (useless so far)
void Expr2DAG::visit(const ExprBinaryOp& e) { e.acceptVisitor(*this); } // (useless so far)
void Expr2DAG::visit(const ExprUnaryOp& e)  { e.acceptVisitor(*this); } // (useless so far)
void Expr2DAG::visit(const ExprSymbol& x)   { assert(false); }
void Expr2DAG::visit(const ExprConstant& c) { assert(false); }

void Expr2DAG::visit(const ExprVector& e) { peer.insert(e,&ExprVector::new_(comps(e),e.orient)); }
void Expr2DAG::visit(const ExprApply& e)  { peer.insert(e,&ExprApply::new_(e.func,comps(e))); }
void Expr2DAG::visit(const ExprChi& e)    { peer.insert(e,&ExprChi::new_(comps(e))); }
void Expr2DAG::visit(const ExprGenericBinaryOp& e)  { peer[e]=&ExprGenericBinaryOp::new_(e.name,*peer[e.left],*peer[e.right]); }
void Expr2DAG::visit(const ExprAdd& e)    { visit_binary(e); }
void Expr2DAG::visit(const ExprMul& e)    { visit_binary(e); }
void Expr2DAG::visit(const ExprSub& e)    { visit_binary(e); }
void Expr2DAG::visit(const ExprDiv& e)    { visit_binary(e); }
void Expr2DAG::visit(const ExprMax& e)    { visit_binary(e); }
void Expr2DAG::visit(const ExprMin& e)    { visit_binary(e); }
void Expr2DAG::visit(const ExprAtan2& e)  { visit_binary(e); }
void Expr2DAG::visit(const ExprGenericUnaryOp& e)  { peer[e]=&ExprGenericUnaryOp::new_(e.name,*peer[e.expr]); }
void Expr2DAG::visit(const ExprMinus& e)  { visit_unary(e); }
void Expr2DAG::visit(const ExprTrans& e)  { visit_unary(e); }
void Expr2DAG::visit(const ExprSign& e)   { visit_unary(e); }
void Expr2DAG::visit(const ExprAbs& e)    { visit_unary(e); }
void Expr2DAG::visit(const ExprPower& e)  { peer[e]=&ExprPower::new_(*peer[e.expr],e.expon); }
void Expr2DAG::visit(const ExprSqr& e)    { visit_unary(e); }
void Expr2DAG::visit(const ExprSqrt& e)   { visit_unary(e); }
void Expr2DAG::visit(const ExprExp& e)    { visit_unary(e); }
void Expr2DAG::visit(const ExprLog& e)    { visit_unary(e); }
void Expr2DAG::visit(const ExprCos& e)    { visit_unary(e); }
void Expr2DAG::visit(const ExprSin& e)    { visit_unary(e); }
void Expr2DAG::visit(const ExprTan& e)    { visit_unary(e); }
void Expr2DAG::visit(const ExprCosh& e)   { visit_unary(e); }
void Expr2DAG::visit(const ExprSinh& e)   { visit_unary(e); }
void Expr2DAG::visit(const ExprTanh& e)   { visit_unary(e); }
void Expr2DAG::visit(const ExprAcos& e)   { visit_unary(e); }
void Expr2DAG::visit(const ExprAsin& e)   { visit_unary(e); }
void Expr2DAG::visit(const ExprAtan& e)   { visit_unary(e); }
void Expr2DAG::visit(const ExprAcosh& e)  { visit_unary(e); }
void Expr2DAG::visit(const ExprAsinh& e)  { visit_unary(e); }
void Expr2DAG::visit(const ExprAtanh& e)  { visit_unary(e); }

} // namespace ibex
