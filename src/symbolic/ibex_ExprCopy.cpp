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
#include "ibex_Domain.h"
#include "ibex_Eval.h"

namespace ibex {

void ExprCopy::mark(const ExprNode& e) {
	if (!used.found(e)) used.insert(e,true);
}

void varcopy(const Array<const ExprSymbol>& src, Array<const ExprSymbol>& dest) {
	assert(src.size()<=dest.size());
	for (int i=0; i<src.size(); i++) {
		dest.set_ref(i,ExprSymbol::new_(src[i].name, src[i].dim));
	}
}

bool varequals(const Array<const ExprSymbol>& arg1, Array<const ExprSymbol>& arg2) {
	if (arg1.size()!=arg2.size()) return false;
	for (int j=0; j<arg1.size(); j++)
		if (arg1[j].dim!=arg2[j].dim) return false;
	return true;
}

const ExprNode& ExprCopy::index_copy(const Array<const ExprSymbol>& old_x, const Array<const ExprNode>& new_x, const ExprNode& y, int i, bool fold_cst) {

	const ExprVector* vec=dynamic_cast<const ExprVector*>(&y);

	if (vec) {
		return copy(old_x, new_x,  vec->arg(i));
	} else {

		const ExprConstant* cst=dynamic_cast<const ExprConstant*>(&y);
		if (cst) {
			if (cst->dim.is_vector())
				return ExprConstant::new_scalar(cst->get_vector_value()[i]);
			else {
				assert(cst->dim.is_matrix());
				return ExprConstant::new_vector(cst->get_matrix_value()[i],true);
			}
		} else {
			const ExprNode* tmp=&y[i];
			const ExprNode& y2=copy(old_x, new_x, *tmp);
			delete (ExprNode*) tmp; // will delete the "[i]" created on-the-fly (but not y, the subexpression)
			return y2;
		}
	}
}

const ExprNode& ExprCopy::index_copy(const Array<const ExprSymbol>& old_x, const Array<const ExprNode>& new_x, const ExprNode& y, int i, int j, bool fold_cst) {

	const ExprVector* vec=dynamic_cast<const ExprVector*>(&y);

	if (vec) {
		const ExprVector* vec2=dynamic_cast<const ExprVector*>(&vec->arg(i));
		if (vec2) {
			return copy(old_x, new_x,  vec2->arg(j));
		} else {
			return index_copy(old_x, new_x, vec->arg(i), j);
		}
	} else {

		const ExprConstant* cst=dynamic_cast<const ExprConstant*>(&y);
		if (cst) {
			assert(cst->dim.is_matrix());
			return ExprConstant::new_scalar(cst->get_matrix_value()[i][j]);
		} else {
			const ExprIndex* tmp=&(y[i][j]);
			const ExprNode& y2=copy(old_x, new_x, *tmp);
			delete &((ExprIndex*) tmp)->expr; // delete y[i][j]
			delete (ExprNode*) tmp;           // delete y[i]
			return y2;
		}
	}
}

const ExprNode& ExprCopy::copy(const Array<const ExprSymbol>& old_x, const Array<const ExprNode>& new_x, const ExprNode& y, bool fold_cst) {

	fold = fold_cst;

	clone.clean();
	used.clean();

	assert(new_x.size()>=old_x.size());

	for (int i=0; i<old_x.size(); i++) {
		clone.insert(old_x[i],&new_x[i]);
		mark(old_x[i]);
	}

	mark(y);
	visit(y);

	//if (fold) {
	for (IBEX_NODE_MAP(const ExprNode*)::iterator it=clone.begin(); it!=clone.end(); it++) {
		if (!used.found(*it->first)) {
			//std::cout << "delete " << *it->second << std::endl;
			delete it->second;
		}
	}
	//}

	return *clone[y];
}

void ExprCopy::visit(const ExprNode& e) {
	if (!clone.found(e)) {
		e.acceptVisitor(*this);
	}
}

void ExprCopy::visit(const ExprIndex& i) {

	visit(i.expr);
	const ExprNode* node=clone[i.expr];
	if (fold) {
		const ExprConstant* c=dynamic_cast<const ExprConstant*>(node);
		if (c) {
			clone.insert(i,&ExprConstant::new_(c->get()[i.index]));
			//delete c; // dont'! see comment in header file
			return;
		}
	}
	mark(i.expr);
	clone.insert(i, &(*node)[i.index]);
}

void ExprCopy::visit(const ExprSymbol& x) {

}

void ExprCopy::visit(const ExprConstant& c) {
	clone.insert(c, &c.copy());
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

#define ARG(i) (*clone[e.arg(i)])
#define LEFT   (*clone[e.left])
#define RIGHT  (*clone[e.right])
#define EXPR   (*clone[e.expr])

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
				clone.insert(e, &ExprConstant::new_vector(v,e.row_vector()));
			} else if (e.dim.type()==Dim::MATRIX) {
				IntervalMatrix m(e.dim.dim2,e.dim.dim3);
				for (i=0; i<e.nb_args; i++) {
					m.set_row(i,((const ExprConstant&) ARG(i)).get_vector_value());
				}
				clone.insert(e, &ExprConstant::new_matrix(m));
			} else {
				assert(e.dim.type()==Dim::MATRIX_ARRAY);
				IntervalMatrixArray ma(e.dim.dim1,e.dim.dim2,e.dim.dim3);
				for (i=0; i<e.nb_args; i++) {
					ma[i]=((const ExprConstant&) ARG(i)).get_matrix_value();
				}
				clone.insert(e, &ExprConstant::new_matrix_array(ma));
			}
			return;
		}
	}

	Array<const ExprNode> args2(e.nb_args);
	for (int i=0; i<e.nb_args; i++) {
		args2.set_ref(i,ARG(i));
		// don't remove this node even if it is a constant because
		// it is an element of this vector.
		mark(e.arg(i));
	}

	clone.insert(e, &ExprVector::new_(args2,e.row_vector()));
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
			clone.insert(e, &ExprConstant::new_(Eval().eval(e.func,d)));
			return;
		}
	}

	Array<const ExprNode> args2(e.nb_args);
	for (int i=0; i<e.nb_args; i++) {
		args2.set_ref(i,ARG(i));
		// don't remove this node even if it is a constant because
		// it is an argument of this function call.
		mark(e.arg(i));
	}

	clone.insert(e, &ExprApply::new_(e.func, args2));
}

void ExprCopy::visit(const ExprChi& e) {  // TODO to check  Jordan: Gilles C. help me
	for (int i=0; i<e.nb_args; i++)
		visit(e.arg(i));

	Array<const ExprNode> args2(e.nb_args);
	for (int i=0; i<e.nb_args; i++) {
		args2.set_ref(i,ARG(i));
		// don't remove this node even if it is a constant because
		// it is an argument of this function call.
		mark(e.arg(i));
	}
	clone.insert(e, &ExprChi::new_(args2));
}


typedef Domain (*dom_func2)(const Domain&, const Domain&);

bool ExprCopy::binary_copy(const ExprBinaryOp& e, dom_func2 fcst) {
	visit(e.left);
	visit(e.right);
	//std::cout << "bin op " << e << std::endl;
	//std::cout << " with left=" << e.left << " right=" << e.right << std::endl;

	if (fold) {

		const ExprConstant* cl=dynamic_cast<const ExprConstant*>(&LEFT);
		const ExprConstant* cr=dynamic_cast<const ExprConstant*>(&RIGHT);

		if (cl && cr) {
			/* evaluate the constant expression on-the-fly */
			//std::cout << "  cst left=" << cl->get() << "  cst right=" << cr->get() << std::endl;
			clone.insert(e, &ExprConstant::new_(fcst(cl->get(),cr->get())));
			//std::cout << "    CST=" << *clone[e] << std::endl;
			return false;
		}

		if (dynamic_cast<const ExprMul*>(&e)) {
			if (cl && cl->dim.is_scalar() && cl->get_value()==Interval::ONE) {
				mark(e.right);
				clone.insert(e, &RIGHT);
				return false;
			}
			if (cr && cr->dim.is_scalar() && cr->get_value()==Interval::ONE) {
				mark(e.left);
				clone.insert(e, &LEFT);
				return false;
			}
		}
	}
	// don't remove copies of subnodes, even one is a constant, because it is an argument of this operator
	mark(e.left);
	mark(e.right);

	return true;
}

void ExprCopy::visit(const ExprAdd& e)   { if (binary_copy(e, operator+)) clone.insert(e, &(LEFT+RIGHT));; }
void ExprCopy::visit(const ExprMul& e)   { if (binary_copy(e, operator*)) clone.insert(e, &(LEFT*RIGHT)); }
void ExprCopy::visit(const ExprSub& e)   { if (binary_copy(e, operator-)) clone.insert(e, &(LEFT-RIGHT)); }
void ExprCopy::visit(const ExprDiv& e)   { if (binary_copy(e, operator/)) clone.insert(e, &(LEFT/RIGHT)); }
void ExprCopy::visit(const ExprMax& e)   { if (binary_copy(e, max  ))     clone.insert(e, &max(LEFT,RIGHT)); }
void ExprCopy::visit(const ExprMin& e)   { if (binary_copy(e, min  ))     clone.insert(e, &min(LEFT,RIGHT)); }
void ExprCopy::visit(const ExprAtan2& e) { if (binary_copy(e, atan2))     clone.insert(e, &atan2(LEFT,RIGHT)); }

void ExprCopy::visit(const ExprPower& e) {
	visit(e.expr);
	if (fold) {
		const ExprConstant* c=dynamic_cast<const ExprConstant*>(&EXPR);
		if (c) {
			/* evaluate the constant expression on-the-fly */
			clone.insert(e, &ExprConstant::new_(pow(c->get(),e.expon)));
			return;
		}
	}
	mark(e.expr);
	clone.insert(e, &pow(EXPR,e.expon));
}

bool ExprCopy::unary_copy(const ExprUnaryOp& e, Domain (*fcst)(const Domain&)) {
	visit(e.expr);
	if (fold) {
		const ExprConstant* c=dynamic_cast<const ExprConstant*>(&EXPR);
		if (c) {
			/* evaluate the constant expression on-the-fly */
			clone.insert(e, &ExprConstant::new_(fcst(c->get())));
			return false;
		}
	}
	mark(e.expr);
	return true;
}

void ExprCopy::visit(const ExprMinus& e) { if (unary_copy(e,operator- )) clone.insert(e,&(-EXPR)); }
void ExprCopy::visit(const ExprTrans& e) { if (unary_copy(e,transpose )) clone.insert(e,&transpose (EXPR)); }
void ExprCopy::visit(const ExprSign& e)  { if (unary_copy(e,sign )) clone.insert(e,&sign (EXPR)); }
void ExprCopy::visit(const ExprAbs& e)   { if (unary_copy(e,abs ))  clone.insert(e,&abs (EXPR)); }
void ExprCopy::visit(const ExprSqr& e)   { if (unary_copy(e,sqr ))  clone.insert(e,&sqr (EXPR)); }
void ExprCopy::visit(const ExprSqrt& e)  { if (unary_copy(e,sqrt )) clone.insert(e,&sqrt (EXPR)); }
void ExprCopy::visit(const ExprExp& e)   { if (unary_copy(e,exp  )) clone.insert(e,&exp  (EXPR)); }
void ExprCopy::visit(const ExprLog& e)   { if (unary_copy(e,log  )) clone.insert(e,&log  (EXPR)); }
void ExprCopy::visit(const ExprCos& e)   { if (unary_copy(e,cos  )) clone.insert(e,&cos  (EXPR)); }
void ExprCopy::visit(const ExprSin& e)   { if (unary_copy(e,sin  )) clone.insert(e,&sin  (EXPR)); }
void ExprCopy::visit(const ExprTan& e)   { if (unary_copy(e,tan  )) clone.insert(e,&tan  (EXPR)); }
void ExprCopy::visit(const ExprCosh& e)  { if (unary_copy(e,cosh )) clone.insert(e,&cosh (EXPR)); }
void ExprCopy::visit(const ExprSinh& e)  { if (unary_copy(e,sinh )) clone.insert(e,&sinh (EXPR)); }
void ExprCopy::visit(const ExprTanh& e)  { if (unary_copy(e,tanh )) clone.insert(e,&tanh (EXPR)); }
void ExprCopy::visit(const ExprAcos& e)  { if (unary_copy(e,acos )) clone.insert(e,&acos (EXPR)); }
void ExprCopy::visit(const ExprAsin& e)  { if (unary_copy(e,asin )) clone.insert(e,&asin (EXPR)); }
void ExprCopy::visit(const ExprAtan& e)  { if (unary_copy(e,atan )) clone.insert(e,&atan (EXPR)); }
void ExprCopy::visit(const ExprAcosh& e) { if (unary_copy(e,acosh)) clone.insert(e,&acosh(EXPR)); }
void ExprCopy::visit(const ExprAsinh& e) { if (unary_copy(e,asinh)) clone.insert(e,&asinh(EXPR)); }
void ExprCopy::visit(const ExprAtanh& e) { if (unary_copy(e,atanh)) clone.insert(e,&atanh(EXPR)); }

} // end ibex namespace





