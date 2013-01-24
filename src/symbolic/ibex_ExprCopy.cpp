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
#include "ibex_Eval.h"

namespace ibex {

void varcopy(const Array<const ExprSymbol>& src, Array<const ExprSymbol>& dest) {
	assert(src.size()<=dest.size());
	for (int i=0; i<src.size(); i++) {
		dest.set_ref(i,ExprSymbol::new_(src[i].name, src[i].dim));
	}
}

const ExprNode& ExprCopy::index_copy(const Array<const ExprSymbol>& old_x, const Array<const ExprSymbol>& new_x, const ExprNode& y, int i, bool fold_cst) {

	const ExprVector* vec=dynamic_cast<const ExprVector*>(&y);

	if (vec) {
		return copy(old_x, new_x,  vec->arg(i));
	} else {
		const ExprNode* tmp=&y[i];
		const ExprNode& y2=copy(old_x, new_x, *tmp);
		delete (ExprNode*) tmp; // will delete the "[i]" created on-the-fly (but not y, the subexpression)
		return y2;
	}
}

const ExprNode& ExprCopy::index_copy(const Array<const ExprSymbol>& old_x, const Array<const ExprSymbol>& new_x, const ExprNode& y, int i, int j, bool fold_cst) {

	const ExprVector* vec=dynamic_cast<const ExprVector*>(&y);

	if (vec) {
		const ExprVector* vec2=dynamic_cast<const ExprVector*>(&vec->arg(i));
		if (vec2) {
			return copy(old_x, new_x,  vec2->arg(j));
		} else {
			const ExprNode* tmp=&(vec->arg(i)[j]);
			const ExprNode& y2=copy(old_x, new_x, *tmp);
			delete (ExprNode*) tmp; // will delete the "[j]" created on-the-fly (but not y[i], the subexpression)
			return y2;
		}
	} else {
		const ExprIndex* tmp=&(y[i][j]);
		const ExprNode& y2=copy(old_x, new_x, *tmp);
		delete &((ExprIndex*) tmp)->expr; // delete y[i][j]
		delete (ExprNode*) tmp;           // delete y[i]
		return y2;
	}
}

const ExprNode& ExprCopy::copy(const Array<const ExprSymbol>& old_x, const Array<const ExprSymbol>& new_x, const ExprNode& y, bool fold_cst) {

	fold = fold_cst;

	y.reset_visited();

	assert(new_x.size()>=old_x.size());

	for (int i=0; i<old_x.size(); i++)
		old_x[i].deco.tmp=&new_x[i]; // must be done *after* y.reset_visited()

	visit(y);

	return *(const ExprNode*) y.deco.tmp;
}

void ExprCopy::visit(const ExprNode& e) {
	if (e.deco.tmp==NULL) {
		e.acceptVisitor(*this);
	}
}

void ExprCopy::visit(const ExprIndex& i) {

	visit(i.expr);
	const ExprNode* node=(const ExprNode*) i.expr.deco.tmp;
	if (fold) {
		const ExprConstant* c=dynamic_cast<const ExprConstant*>(node);
		if (c) {
			i.deco.tmp = &ExprConstant::new_(c->get()[i.index]);
			delete c;
			return;
		}
	}

	i.deco.tmp =& (*node)[i.index];
}

void ExprCopy::visit(const ExprSymbol& x) {

}

void ExprCopy::visit(const ExprConstant& c) {
	c.deco.tmp = &c.copy();
}

// (useless so far)
void ExprCopy::visit(const ExprNAryOp& e) {
	e.acceptVisitor(*this);
}

void ExprCopy::visit(const ExprLeaf& e) {
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


#define ARG(i) (*((const ExprNode*) e.arg(i).deco.tmp))
#define LEFT   (*((const ExprNode*) e.left.deco.tmp))
#define RIGHT  (*((const ExprNode*) e.right.deco.tmp))
#define EXPR   (*((const ExprNode*) e.expr.deco.tmp))

void ExprCopy::visit(const ExprVector& e) {
	for (int i=0; i<e.nb_args; i++)
		visit(e.arg(i));

	if (fold) {
		int i=0;
		for (; i<e.nb_args; i++) {
			if (!dynamic_cast<const ExprConstant*>(&ARG(i))) break;
		}
		if (i==e.nb_args) {
			if (e.dim.is_vector()) {
				IntervalVector v(e.dim.vec_size());
				for (i=0; i<e.nb_args; i++) {
					v[i]=((const ExprConstant&) ARG(i)).get_value();
				}
				e.deco.tmp=&ExprConstant::new_vector(v,e.row_vector());
			} else if (e.dim.type()==Dim::MATRIX) {
				IntervalMatrix m(e.dim.dim2,e.dim.dim3);
				for (i=0; i<e.nb_args; i++) {
					m.set_row(i,((const ExprConstant&) ARG(i)).get_vector_value());
				}
				e.deco.tmp=&ExprConstant::new_matrix(m);
			} else {
				assert(e.dim.type()==Dim::MATRIX_ARRAY);
				IntervalMatrixArray ma(e.dim.dim1,e.dim.dim2,e.dim.dim3);
				for (i=0; i<e.nb_args; i++) {
					ma[i]=((const ExprConstant&) ARG(i)).get_matrix_value();
				}
				e.deco.tmp=&ExprConstant::new_matrix_array(ma);
			}
			return;
		}
	}

	const ExprNode** args2 = new const ExprNode* [e.nb_args];
	for (int i=0; i<e.nb_args; i++)
		args2[i]=&ARG(i);

	e.deco.tmp=&ExprVector::new_(args2, e.nb_args, e.row_vector());
	delete [] args2;
}

void ExprCopy::visit(const ExprApply& e) {
	for (int i=0; i<e.nb_args; i++)
		visit(e.arg(i));

	if (fold) {
		int i=0;
		for (; i<e.nb_args; i++) {
			if (!dynamic_cast<const ExprConstant*>(&ARG(i))) break;
		}
		if (i==e.nb_args) {
			Array<const Domain> d(e.nb_args);
			for (i=0; i<e.nb_args; i++) {
				d.set_ref(i,((const ExprConstant&) ARG(i)).get());
			}
			e.deco.tmp=&ExprConstant::new_(Eval().eval(e.func,d));
			return;
		}
	}

	const ExprNode** args2 = new const ExprNode* [e.nb_args];
	for (int i=0; i<e.nb_args; i++)
		args2[i]=&ARG(i);

	e.deco.tmp=&ExprApply::new_(e.func, args2);
	delete [] args2;
}

#define visit_binary(f) \
		visit(e.left); \
		visit(e.right); \
		if (fold) { \
			const ExprConstant* cl=dynamic_cast<const ExprConstant*>(&LEFT); \
			if (cl) { \
				const ExprConstant* cr=dynamic_cast<const ExprConstant*>(&RIGHT); \
				if (cr) { \
					/* evaluate the constant expression on-the-fly */ \
					e.deco.tmp = &ExprConstant::new_(f(cl->get(),cr->get())); \
					delete cl; \
					delete cr; \
					return; \
				} \
			} \
		} \
		e.deco.tmp = &f(LEFT,RIGHT); \

void ExprCopy::visit(const ExprAdd& e)   { visit_binary(operator+); }
void ExprCopy::visit(const ExprMul& e)   { visit_binary(operator*); }
void ExprCopy::visit(const ExprSub& e)   { visit_binary(operator-); }
void ExprCopy::visit(const ExprDiv& e)   { visit_binary(operator/); }
void ExprCopy::visit(const ExprMax& e)   { visit_binary( max  ); }
void ExprCopy::visit(const ExprMin& e)   { visit_binary( min  ); }
void ExprCopy::visit(const ExprAtan2& e) { visit_binary( atan2); }

void ExprCopy::visit(const ExprPower& e) {
	visit(e.expr);
	if (fold) {
		const ExprConstant* c=dynamic_cast<const ExprConstant*>(&EXPR);
		if (c) {
			/* evaluate the constant expression on-the-fly */
			e.deco.tmp = &ExprConstant::new_(pow(c->get(),e.expon));
			delete c;
			return;
		}
	}
	e.deco.tmp = &pow(EXPR,e.expon);
}

#define visit_unary(f) \
		visit(e.expr); \
		if (fold) { \
			const ExprConstant* c=dynamic_cast<const ExprConstant*>(&EXPR); \
			if (c) { \
				/* evaluate the constant expression on-the-fly */ \
				e.deco.tmp = &ExprConstant::new_(f(c->get())); \
				delete c; \
				return; \
			} \
		} \
		e.deco.tmp = &f(EXPR); \

void ExprCopy::visit(const ExprMinus& e) { visit_unary( - ); }
void ExprCopy::visit(const ExprTrans& e) { visit_unary( transpose ); }
void ExprCopy::visit(const ExprSign& e)  { visit_unary( sign ); }
void ExprCopy::visit(const ExprAbs& e)   { visit_unary( abs ); }
void ExprCopy::visit(const ExprSqr& e)   { visit_unary( sqr ); }
void ExprCopy::visit(const ExprSqrt& e)  { visit_unary( sqrt ); }
void ExprCopy::visit(const ExprExp& e)   { visit_unary( exp  ); }
void ExprCopy::visit(const ExprLog& e)   { visit_unary( log  ); }
void ExprCopy::visit(const ExprCos& e)   { visit_unary( cos  ); }
void ExprCopy::visit(const ExprSin& e)   { visit_unary( sin  ); }
void ExprCopy::visit(const ExprTan& e)   { visit_unary( tan  ); }
void ExprCopy::visit(const ExprCosh& e)  { visit_unary( cosh ); }
void ExprCopy::visit(const ExprSinh& e)  { visit_unary( sinh ); }
void ExprCopy::visit(const ExprTanh& e)  { visit_unary( tanh ); }
void ExprCopy::visit(const ExprAcos& e)  { visit_unary( acos ); }
void ExprCopy::visit(const ExprAsin& e)  { visit_unary( asin ); }
void ExprCopy::visit(const ExprAtan& e)  { visit_unary( atan ); }
void ExprCopy::visit(const ExprAcosh& e) { visit_unary( acosh); }
void ExprCopy::visit(const ExprAsinh& e) { visit_unary( asinh); }
void ExprCopy::visit(const ExprAtanh& e) { visit_unary( atanh); }

} // end ibex namespace





