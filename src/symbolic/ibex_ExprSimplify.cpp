/*
 * ibex_ExprSimplify.cpp
 *
 *  Created on: May 19, 2016
 *      Author: gilles
 */

#include "ibex_ExprSimplify.h"

namespace ibex {

class NotConstExpr : public Exception { };

// throw NotConst if fails
Domain to_cst(const ExprNode& e) {
	const ExprConstant* c=dynamic_cast<const ExprConstant*>(&e);
	if (c) return c->get();
	else {
		const ExprVector* v=dynamic_cast<const ExprVector*>(&e);
		std::vector<Domain> arg_cst(v->nb_args);
		if (v) {
			for (int i=0; i<v->nb_args; i++)
				arg_cst.push_back(to_cst(v->arg(i)));
			return Domain(arg_cst,v->row_vector());
		} else
			throw NotConstExpr();
	}
}

bool is_zero(const ExprNode& e) {
	const ExprConstant* c=dynamic_cast<const ExprConstant*>(&e);
	if (c)
		return c->get().is_zero();
	else {
		const ExprVector* v=dynamic_cast<const ExprVector*>(&e);
		if (v) {
			for (int i=0; i<v->nb_args; i++)
				if (!is_zero(v->arg(i))) return false;
			return true;
		} else
			return false;
	}
}

bool is_identity(const ExprNode& e) {
	try {
		// we could be more incremental than creating the whole domain first
		// but it requires to determine if an element is diagonal/off-diagonal.
		Domain d = to_cst(e);
		switch(d.dim.type()) {
		case Dim::SCALAR: return d.i()==Interval::ONE;
		case Dim::ROW_VECTOR:
		case Dim::COL_VECTOR: return false;
		case Dim::MATRIX: return
				(d.dim.nb_rows()==d.dim.nb_cols()) &&
				d.m()==Matrix::eye(d.dim.nb_rows());
		}
	} catch(NotConstExpr&) {
		return false;
	}
}

#define ARG(i) (*clone[e.arg(i)])
#define LEFT   (*clone[e.left])
#define RIGHT  (*clone[e.right])
#define EXPR   (*clone[e.expr])

void ExprSimplify::visit(const ExprAdd& e) {
	if (binary_copy(e, operator+)) return;

	if (is_zero(e.left)) clone.insert(e, &RIGHT);
	else if (is_zero(e.right)) clone.insert(e, &LEFT);
}

void ExprSimplify::visit(const ExprSub& e) {
	if (binary_copy(e, operator-)) return;

	if (is_zero(e.left)) clone.insert(e, &(-RIGHT));
	else if (is_zero(e.right)) clone.insert(e, &LEFT);
}

void ExprSimplify::visit(const ExprMul& e) {

	if (binary_copy(e, operator*)) return;

	if (is_identity(e.left)) clone.insert(e, &RIGHT);
	else if (is_identity(e.right)) clone.insert(e, &LEFT);
}

void ExprSimplify::visit(const ExprVector& e) {
	try {
		Domain d=to_cst(e);
		clone.insert(e,&ExprConstant::new_(d));
	} catch(NotConstExpr&) {
		clone.insert(e,&e);
	}
}

typedef const ExprNode& (*func2)(const ExprNode&, const ExprNode&);
void ExprSimplify::binary_copy(const ExprBinaryOp& e, func2 fcst) {
	if ((&LEFT == e.left) && (&RIGHT == e.right))
		clone.insert(e,&e);
	else
		clone.insert(&func2(LEFT,RIGHT));
}

typedef Domain (*dom_func2)(const Domain&, const Domain&);
bool ExprSimplify::binary_copy(const ExprBinaryOp& e, dom_func2 fcst) {
	visit(e.left);
	visit(e.right);
	//std::cout << "bin op " << e << std::endl;
	//std::cout << " with left=" << e.left << " right=" << e.right << std::endl;


	const ExprConstant* cl=dynamic_cast<const ExprConstant*>(&LEFT);
	const ExprConstant* cr=dynamic_cast<const ExprConstant*>(&RIGHT);

	if (cl && cr) {
		/* evaluate the constant expression on-the-fly */
		//std::cout << "  cst left=" << cl->get() << "  cst right=" << cr->get() << std::endl;
		clone.insert(e, &ExprConstant::new_(fcst(cl->get(),cr->get())));
		//std::cout << "    CST=" << *clone[e] << std::endl;
		return false;
	}
	return true;
}

bool ExprSimplify::unary_copy(const ExprUnaryOp& e, Domain (*fcst)(const Domain&)) {
	visit(e.expr);

	const ExprConstant* c=dynamic_cast<const ExprConstant*>(&EXPR);
	if (c) {
		/* evaluate the constant expression on-the-fly */
		clone.insert(e, &ExprConstant::new_(fcst(c->get())));
		return false;
	} else if (&e.expr == &EXPR) { // nothing changed
		clone.insert(e,&e);
		return false;
	} else {
		return true;
	}
}

void ExprSimplify::visit(const ExprDiv& e)   { if (binary_copy(e, operator/)) clone.insert(e, &(LEFT/RIGHT)); }
void ExprSimplify::visit(const ExprMax& e)   { if (binary_copy(e, max  ))     clone.insert(e, &max(LEFT,RIGHT)); }
void ExprSimplify::visit(const ExprMin& e)   { if (binary_copy(e, min  ))     clone.insert(e, &min(LEFT,RIGHT)); }
void ExprSimplify::visit(const ExprAtan2& e) { if (binary_copy(e, atan2))     clone.insert(e, &atan2(LEFT,RIGHT)); }
void ExprSimplify::visit(const ExprMinus& e) { if (unary_copy(e,operator- )) clone.insert(e,&(-EXPR)); }
void ExprSimplify::visit(const ExprTrans& e) { if (unary_copy(e,transpose )) clone.insert(e,&transpose (EXPR)); }
void ExprSimplify::visit(const ExprSign& e)  { if (unary_copy(e,sign )) clone.insert(e,&sign (EXPR)); }
void ExprSimplify::visit(const ExprAbs& e)   { if (unary_copy(e,abs ))  clone.insert(e,&abs (EXPR)); }
void ExprSimplify::visit(const ExprSqr& e)   { if (unary_copy(e,sqr ))  clone.insert(e,&sqr (EXPR)); }
void ExprSimplify::visit(const ExprSqrt& e)  { if (unary_copy(e,sqrt )) clone.insert(e,&sqrt (EXPR)); }
void ExprSimplify::visit(const ExprExp& e)   { if (unary_copy(e,exp  )) clone.insert(e,&exp  (EXPR)); }
void ExprSimplify::visit(const ExprLog& e)   { if (unary_copy(e,log  )) clone.insert(e,&log  (EXPR)); }
void ExprSimplify::visit(const ExprCos& e)   { if (unary_copy(e,cos  )) clone.insert(e,&cos  (EXPR)); }
void ExprSimplify::visit(const ExprSin& e)   { if (unary_copy(e,sin  )) clone.insert(e,&sin  (EXPR)); }
void ExprSimplify::visit(const ExprTan& e)   { if (unary_copy(e,tan  )) clone.insert(e,&tan  (EXPR)); }
void ExprSimplify::visit(const ExprCosh& e)  { if (unary_copy(e,cosh )) clone.insert(e,&cosh (EXPR)); }
void ExprSimplify::visit(const ExprSinh& e)  { if (unary_copy(e,sinh )) clone.insert(e,&sinh (EXPR)); }
void ExprSimplify::visit(const ExprTanh& e)  { if (unary_copy(e,tanh )) clone.insert(e,&tanh (EXPR)); }
void ExprSimplify::visit(const ExprAcos& e)  { if (unary_copy(e,acos )) clone.insert(e,&acos (EXPR)); }
void ExprSimplify::visit(const ExprAsin& e)  { if (unary_copy(e,asin )) clone.insert(e,&asin (EXPR)); }
void ExprSimplify::visit(const ExprAtan& e)  { if (unary_copy(e,atan )) clone.insert(e,&atan (EXPR)); }
void ExprSimplify::visit(const ExprAcosh& e) { if (unary_copy(e,acosh)) clone.insert(e,&acosh(EXPR)); }
void ExprSimplify::visit(const ExprAsinh& e) { if (unary_copy(e,asinh)) clone.insert(e,&asinh(EXPR)); }
void ExprSimplify::visit(const ExprAtanh& e) { if (unary_copy(e,atanh)) clone.insert(e,&atanh(EXPR)); }

} /* namespace ibex */
