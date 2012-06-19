//============================================================================
//                                  I B E X                                   
// File        : ibex_ExprCopy.cpp
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Apr 9, 2012
// Last Update : Apr 9, 2012
//============================================================================

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <cassert>
#include "ibex_ExprCopy.h"
#include "ibex_Expr.h"

namespace ibex {

namespace {

/* Write an index into a string, surrounded with brackets.
 * Up to 6 digits are allowed.
 * Return the length of the string. */
int index_2_string(char* buff, int index) {
	assert(index<1000000);
	int k=0;
	char number[6];
	buff[k++]='[';
	snprintf(number, 6, "%d", index);
	strcpy(&buff[k], number);
	k+=strlen(number);
	buff[k++]=']';
	return k;
}

char buff[1000];

/* Return the string corresponding to the subexpression
 * x[i] where x is a symbol name and i and index. */
const char* index_symbol_2_string(const char* symbol_name, int index1, int index2, int index3) {
	int k=0;
	strcpy(&buff[k], symbol_name);
	k+=strlen(symbol_name);

	if (index1!=-1) k+=index_2_string(&buff[k], index1);
	if (index2!=-1) k+=index_2_string(&buff[k], index2);
	if (index3!=-1) k+=index_2_string(&buff[k], index3);
	buff[k++]='\0';
	//cout << "debug name=" << buff << endl;
	return buff;
}

/*
class Unvectorize {

	vector<const ExprSymbol*> dest_symbols;
	Array<const ExprNode> peers;

	Unvectorize(Array<const ExprSymbol>& x) : peers(x.size()) {
		for (int i=0; i<x.size(); i++)
			visit(x[i]);
	}
protected:
	void visit(const ExprSymbol& x) {

		switch (x.type()) {
		case Dim::SCALAR:
			dest_symbols.push_back(&ExprSymbol::new_(x.name,x.dim));
			peers.set_ref(x.id,*dest_symbols.back());
			break;
		case Dim::ROW_VECTOR:
		case Dim::COL_VECTOR:
			// Example. Let x be a vector of 2 components.
			// The following node "v" is [x[0], x[1]]. It the src expression is:
			//           x->x[0]+x[1].
			// We will get in this case
			//          (x[0],x[1])->x[0]+x[1]
			// So v does not appear in the dest expression. But if the original expression is:
			//           x -> 0.1*x (scalar product)
			// we will get
			//          (x[0],x[1])->0.1*[x[0],x[1]]
			// and the right side of the multiplication points to v.
			const ExprNode* v[x.dim.vec_size()];
			for (int i=0; i<x.dim.vec_size(); i++) {
				dest_symbols.push_back(& ExprSymbol::new_(index_symbol_2_string(x.name,-1,-1,i)));
				v[i]=dest_symbols.back();
			}
			peers.set_ref(x.id,ExprVector::new_(v,x.dim.vec_size(),x.dim.type()==Dim::ROW_VECTOR));
			break;
		case Dim::MATRIX:
			const ExprNode* m[x.dim.dim2];
			for (int i=0; i<x.dim.dim2; i++) {
				const ExprNode* v[x.dim.dim3];
				for (int j=0; j<x.dim.dim3; j++) {
					dest_symbols.push_back(& ExprSymbol::new_(index_symbol_2_string(x.name,-1,i,j)));
					v[j]=dest_symbols.back();
				}
				m[i]= & ExprVector::new_(v,x.dim.dim3,true);
			}
			peers.set_ref(x.id,ExprVector::new_(m,x.dim.dim2,false));
			break;
		case Dim::MATRIX_ARRAY:
			const ExprNode* ma[x.dim.dim1];
			for (int i=0; i<x.dim.dim1; i++) {
				const ExprNode* m[x.dim.dim2];
				for (int j=0; j<x.dim.dim2; j++) {
					const ExprNode* v[x.dim.dim3];
					for (int k=0; k<x.dim.dim3; k++) {
						dest_symbols.push_back(& ExprSymbol::new_(index_symbol_2_string(x.name,i,j,k)));
						v[k]=dest_symbols.back();
					}
					m[j]= & ExprVector::new_(v,x.dim.dim3,true);
				}
				ma[i]= & ExprVector::new_(m,x.dim.dim2,false);
			}
			peers.set_ref(x.id, ExprVector::new_(ma,x.dim.dim1,true)); // notice: true or false does not matter here
			break;
		}
	}
};
*/

} // end namespace

const ExprNode& ExprCopy::copy(const Array<const ExprSymbol>& x, const ExprNode& y) {
	y.reset_visited();
	new_x = &x;
	visit(y);
	//cout << "new x=";
	//for (int j=0; j<new_x.size(); j++) cout << new_x[j].name << " ";
	//cout << endl;
	new_x = NULL;
	return *(const ExprNode*) y.deco.tmp;
	//cout << "new y=" << *new_y << endl;
}

void ExprCopy::visit(const ExprNode& e) {
	if (e.deco.tmp==NULL) {
		e.acceptVisitor(*this);
	}
}

void ExprCopy::visit(const ExprIndex& i) {
	visit(i.expr);
	i.deco.tmp =& (*((const ExprNode*) i.expr.deco.tmp))[i.index];
}

void ExprCopy::visit(const ExprSymbol& x) {
	x.deco.tmp=&(*new_x)[x.key];
}

void ExprCopy::visit(const ExprConstant& c) {
	c.deco.tmp = &c.copy();
}

// (useless so far)
void ExprCopy::visit(const ExprNAryOp& e) {
	e.acceptVisitor(*this);
}

// (useless so far)
void ExprCopy::visit(const ExprBinaryOp& b) {
	b.acceptVisitor(*this);
}

// (useless so far)
void ExprCopy::visit(const ExprUnaryOp& u) {
	u.acceptVisitor(*this);
}

void ExprCopy::visit(const ExprVector& e) {
	for (int i=0; i<e.nb_args; i++)
		visit(e.arg(i));

	const ExprNode* args2[e.nb_args];
	for (int i=0; i<e.nb_args; i++)
		args2[i]=(const ExprNode*) e.arg(i).deco.tmp;

	e.deco.tmp=&ExprVector::new_(args2, e.nb_args, e.row_vector());
}

void ExprCopy::visit(const ExprApply& e) {
	for (int i=0; i<e.nb_args; i++)
		visit(e.arg(i));

	const ExprNode* args2[e.nb_args];
	for (int i=0; i<e.nb_args; i++)
		args2[i]=(const ExprNode*) e.arg(i).deco.tmp;

	e.deco.tmp=&ExprApply::new_(e.func, args2);
}

#define LEFT  (*((const ExprNode*) e.left.deco.tmp))
#define RIGHT (*((const ExprNode*) e.right.deco.tmp))
#define EXPR  (*((const ExprNode*) e.expr.deco.tmp))

void ExprCopy::visit(const ExprAdd& e)   { visit(e.left); visit(e.right); e.deco.tmp = &      (LEFT + RIGHT); }
void ExprCopy::visit(const ExprMul& e)   { visit(e.left); visit(e.right); e.deco.tmp = &      (LEFT * RIGHT); }
void ExprCopy::visit(const ExprSub& e)   { visit(e.left); visit(e.right); e.deco.tmp = &      (LEFT - RIGHT); }
void ExprCopy::visit(const ExprDiv& e)   { visit(e.left); visit(e.right); e.deco.tmp = &      (LEFT / RIGHT); }
void ExprCopy::visit(const ExprMax& e)   { visit(e.left); visit(e.right); e.deco.tmp = & max  (LEFT, RIGHT); }
void ExprCopy::visit(const ExprMin& e)   { visit(e.left); visit(e.right); e.deco.tmp = & min  (LEFT, RIGHT); }
void ExprCopy::visit(const ExprAtan2& e) { visit(e.left); visit(e.right); e.deco.tmp = & atan2(LEFT, RIGHT); }

void ExprCopy::visit(const ExprMinus& e) { visit(e.expr); e.deco.tmp = & -    (EXPR); }
void ExprCopy::visit(const ExprSign& e)  { visit(e.expr); e.deco.tmp = & sign (EXPR); }
void ExprCopy::visit(const ExprAbs& e)   { visit(e.expr); e.deco.tmp = & abs  (EXPR); }
void ExprCopy::visit(const ExprPower& e) { visit(e.expr); e.deco.tmp = & pow  (EXPR,e.expon); }
void ExprCopy::visit(const ExprSqr& e)   { visit(e.expr); e.deco.tmp = & sqr  (EXPR); }
void ExprCopy::visit(const ExprSqrt& e)  { visit(e.expr); e.deco.tmp = & sqrt (EXPR); }
void ExprCopy::visit(const ExprExp& e)   { visit(e.expr); e.deco.tmp = & exp  (EXPR); }
void ExprCopy::visit(const ExprLog& e)   { visit(e.expr); e.deco.tmp = & log  (EXPR); }
void ExprCopy::visit(const ExprCos& e)   { visit(e.expr); e.deco.tmp = & cos  (EXPR); }
void ExprCopy::visit(const ExprSin& e)   { visit(e.expr); e.deco.tmp = & sin  (EXPR); }
void ExprCopy::visit(const ExprTan& e)   { visit(e.expr); e.deco.tmp = & tan  (EXPR); }
void ExprCopy::visit(const ExprCosh& e)  { visit(e.expr); e.deco.tmp = & cosh (EXPR); }
void ExprCopy::visit(const ExprSinh& e)  { visit(e.expr); e.deco.tmp = & sinh (EXPR); }
void ExprCopy::visit(const ExprTanh& e)  { visit(e.expr); e.deco.tmp = & tanh (EXPR); }
void ExprCopy::visit(const ExprAcos& e)  { visit(e.expr); e.deco.tmp = & acos (EXPR); }
void ExprCopy::visit(const ExprAsin& e)  { visit(e.expr); e.deco.tmp = & asin (EXPR); }
void ExprCopy::visit(const ExprAtan& e)  { visit(e.expr); e.deco.tmp = & atan (EXPR); }
void ExprCopy::visit(const ExprAcosh& e) { visit(e.expr); e.deco.tmp = & acosh(EXPR); }
void ExprCopy::visit(const ExprAsinh& e) { visit(e.expr); e.deco.tmp = & asinh(EXPR); }
void ExprCopy::visit(const ExprAtanh& e) { visit(e.expr); e.deco.tmp = & atanh(EXPR); }

} // end ibex namespace





