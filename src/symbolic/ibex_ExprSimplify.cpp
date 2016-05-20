/*
 * ibex_ExprSimplify.cpp
 *
 *  Created on: May 19, 2016
 *      Author: gilles
 */

#include "ibex_ExprSimplify.h"
#include "ibex_Expr.h"

using namespace std;

namespace ibex {

namespace {

bool is_cst(const ExprNode& e) {
	return dynamic_cast<const ExprConstant*>(&e)!=NULL;
}

const Domain& to_cst(const ExprNode& e) {
	const ExprConstant* c=dynamic_cast<const ExprConstant*>(&e);
	assert(c);
	return c->get();
}

bool is_identity(const ExprNode& e) {
	if (is_cst(e)) {
		// we could be more incremental than creating the whole domain first
		// but it requires to determine if an element is diagonal/off-diagonal.
		const Domain& d = to_cst(e);
		switch(d.dim.type()) {
		case Dim::SCALAR:     return d.i()==Interval::ONE;
		case Dim::ROW_VECTOR:
		case Dim::COL_VECTOR: return false;
		case Dim::MATRIX:     return d.m()==Matrix::eye(d.dim.nb_rows());
		}
	} else {
		return false;
	}
}

} // end anonymous namespace

void ExprSimplify::insert(const ExprNode& e, const ExprNode& e2) {
	idx_clones[e]->push_back(pair<DoubleIndex,const ExprNode*>(idx,e2));
}

const ExprNode& ExprSimplify::get(const ExprNode& e, const DoubleIndex& idx2) {
	// first time we access to node e:
	if (!idx_clones.found(e)) {
		idx_clones.insert(e,new vector<pair<DoubleIndex,const ExprNode*> >());
	}

	vector<pair<DoubleIndex,const ExprNode*> >& v=*(idx_clones[e]);
	int i=0;
	while (i<v.size() && v[i].first!=idx2)
		i++;
	if (i==v.size()) { // idx2 not found in the clone list
		DoubleIndex old_idx=idx;
		idx=idx2;
		e.acceptVisitor(*this);
		idx=old_idx;
	}
	assert(v.back().first==idx2);

	return v[i].second;
}

void ExprSimplify::visit(const ExprVector& e) {

	vector<const ExprNode*> res;
	int i=0;
	bool all_cst=true;
	bool all_same=true;

	if (e.row_vector()) {
		int c=0;
		while (c<=idx.last_col()) {
			int n=e.arg(i).dim.nb_cols();
			int first_col=idx.first_col()-c;
			if (first_col<n) { // can be negative
				int last_col=idx.last_col()-c;
				DoubleIndex e_idx=DoubleIndex(e.arg(i).dim,
						idx.first_row(), idx.last_row(),
						first_col<0? 0 : first_col, last_col>n? n : last_col);
				res.push_back(&get(e.arg(i),e_idx));
				all_cst &= is_cst(*res.back());
				all_same &= (res.back()==&e.arg(i));
			}
			c+=n;
			assert(i<e.nb_args);
			i++;
		}
	} else {
		int r=0;
		while (r<=idx.last_row()) {
			int n=e.arg(i).dim.nb_rows();
			int first_row=idx.first_row()-r;
			if (first_row<n) { // can be negative
				int last_row=idx.last_row()-r;
				DoubleIndex e_idx=DoubleIndex(e.arg(i).dim,
						first_row<0? 0 : first_row, last_row>n? n : last_row,
						idx.first_col(), idx.last_col());
				res.push_back(&get(e.arg(i),e_idx));
				all_cst &= is_cst(*res.back());
				all_same &= (res.back()==&e.arg(i));
			}
			r+=n;
			assert(i<e.nb_args);
			i++;
		}
	}

	if (res.size()==1)
		insert(e, *res.back());
	else if (all_cst) {
		Array<const Domain> arg_cst(res.size());
		for (i=0; i<res.size(); i++)
			arg_cst.set_ref(i, to_cst(*res[i]));
		insert(e, ExprConstant::new_(Domain(arg_cst, e.row_vector())));
	} else if (res.size()==e.nb_args && all_same)
		insert(e, e);
	else
		insert(e, ExprVector::new_(res,e.row_vector()));
}

void ExprSimplify::visit(const ExprIndex& e) {
	DoubleIndex e_idx;
	if (!idx.all()) {
		assert(e.index.all_cols());
		assert(idx.all_rows());
		e_idx=DoubleIndex(e.expr.dim,e.index.first_row(),e.index.last_row(),
				idx.first_col(),idx.last_col());
	} else {
		e_idx=e.index;
	}

	insert(e, get(e.expr,e_idx));
}

void ExprSimplify::visit(const ExprSymbol& x) {
	if (idx.all())
		insert(x,x);
	else
		insert(x,x[idx]);
}


void ExprSimplify::visit(const ExprConstant& c) {
	if (idx.all())
		insert(c,c);
	else
		insert(c,ExprConstant::new_(c.get()[idx]));
}

void ExprSimplify::visit(const ExprAdd& e) {

	const ExprNode& l=get(e.left, idx);
	const ExprNode& r=get(e.right, idx);

	if (is_cst(l) && to_cst(l).is_zero())
		insert(e, r);
	else if (is_cst(r) && to_cst(r).is_zero())
		insert(e, l);
	else if (is_cst(l) && is_cst(r))
		insert(e, ExprConstant::new_(to_cst(l)+to_cst(r)));
	else if ((&l == &e.left) && (&r == &e.right))  // nothing changed
		insert(e, e);
	else
		insert(e, l+r);
}

void ExprSimplify::visit(const ExprSub& e) {
	const ExprNode& l=get(e.left, idx);
	const ExprNode& r=get(e.right, idx);

	if (is_cst(l) && to_cst(l).is_zero())
		insert(e, -r);
	else if (is_cst(r) && to_cst(r).is_zero())
		insert(e, l);
	else if (is_cst(l) && is_cst(r))
		insert(e, ExprConstant::new_(to_cst(l)-to_cst(r)));
	else if ((&l == &e.left) && (&r == &e.right))  // nothing changed
		insert(e, e);
	else
		insert(e, l-r);
}

void ExprSimplify::visit(const ExprMul& e) {
	DoubleIndex l_idx=DoubleIndex::rows(e.left.dim,idx.first_row(),idx.last_row());
	DoubleIndex r_idx=DoubleIndex::cols(e.right.dim,idx.first_col(),idx.last_col());

	const ExprNode& l=get(e.left, l_idx);
	const ExprNode& r=get(e.right, r_idx);

	if (is_identity(l) || (is_cst(r) && to_cst(r).is_zero()))
		insert(e, r);
	else if (is_identity(r) || (is_cst(l) && to_cst(l).is_zero()))
		insert(e, l);
	else if (is_cst(l) && is_cst(r))
		insert(e, ExprConstant::new_(to_cst(l)*to_cst(r)));
	else if ((&l == &e.left) && (&r == &e.right))  // nothing changed
		insert(e, e);
	else
		insert(e, l*r);
}

void ExprSimplify::visit(const ExprDiv& e) {

	const ExprNode& l=get(e.left, idx);
	const ExprNode& r=get(e.right, idx);

	if (is_identity(r))
		insert(e, l);
	else if (is_cst(l) && is_cst(r))
		insert(e, ExprConstant::new_(to_cst(l)/to_cst(r)));
	else if ((&l == &e.left) && (&r == &e.right))  // nothing changed
		insert(e, e);
	else
		insert(e, l/r);
}

//typedef const ExprNode& (*func)(const ExprNode&);
//void ExprSimplify::nary_copy(const ExprNaryOp& e, func f) {
//	if (&e.expr == &EXPR)  // nothing changed
//		clone.insert(e,&e);
//	else
//		clone.insert(e, &(f(EXPR)));
//}


typedef Domain (*dom_func2)(const Domain&, const Domain&);
typedef const ExprNode& (*func2)(const ExprNode&, const ExprNode&);
void ExprSimplify::binary(const ExprBinaryOp& e, dom_func2 fcst, func2 f) {

	const ExprNode& l=get(e.left, idx);
	const ExprNode& r=get(e.right, idx);

	if (is_cst(l) && is_cst(r))
		insert(e, ExprConstant::new_(fcst(to_cst(l),to_cst(r))));
	else if ((&l == &e.left) && (&r == &e.right))  // nothing changed
		insert(e, e);
	else
		insert(e, f(l,r));
}

void ExprSimplify::unary(const ExprUnaryOp& e, Domain (*fcst)(const Domain&), const ExprNode& (*f)(const ExprNode&)) {

	const ExprNode& expr=get(e.expr, idx);

	if (is_cst(expr))
		/* evaluate the constant expression on-the-fly */
		insert(e, ExprConstant::new_(fcst(to_cst(expr))));
	else if (&e.expr == &expr)  // if nothing changed
		insert(e, e);
	else
		insert(e, f(expr));
}

void ExprSimplify::visit(const ExprChi& e) {
	not_implemented("Simplify with Chi function");
}

void ExprSimplify::visit(const ExprApply& e) {
	not_implemented("Simplify with Apply");
}

void ExprSimplify::visit(const ExprMax& e)   { binary(max,max); }
void ExprSimplify::visit(const ExprMin& e)   { binary(min,min); }
void ExprSimplify::visit(const ExprAtan2& e) { binary(atan2,atan2); }
void ExprSimplify::visit(const ExprMinus& e) { unary(operator-, operator-); }
void ExprSimplify::visit(const ExprTrans& e) { unary(transpose, transpose); }
void ExprSimplify::visit(const ExprSign& e)  { unary(sign, sign); }
void ExprSimplify::visit(const ExprAbs& e)   { unary(abs, abs); }
void ExprSimplify::visit(const ExprSqr& e)   { unary(sqr, sqr); }
void ExprSimplify::visit(const ExprSqrt& e)  { unary(sqrt, sqrt); }
void ExprSimplify::visit(const ExprExp& e)   { unary(exp, exp); }
void ExprSimplify::visit(const ExprLog& e)   { unary(log, log); }
void ExprSimplify::visit(const ExprCos& e)   { unary(cos, cos); }
void ExprSimplify::visit(const ExprSin& e)   { unary(sin, sin); }
void ExprSimplify::visit(const ExprTan& e)   { unary(tan, tan); }
void ExprSimplify::visit(const ExprCosh& e)  { unary(cosh, cosh); }
void ExprSimplify::visit(const ExprSinh& e)  { unary(sinh, sinh); }
void ExprSimplify::visit(const ExprTanh& e)  { unary(tanh, tanh); }
void ExprSimplify::visit(const ExprAcos& e)  { unary(acos, acos); }
void ExprSimplify::visit(const ExprAsin& e)  { unary(asin, asin); }
void ExprSimplify::visit(const ExprAtan& e)  { unary(atan, atan); }
void ExprSimplify::visit(const ExprAcosh& e) { unary(acosh, acosh); }
void ExprSimplify::visit(const ExprAsinh& e) { unary(asinh, asinh); }
void ExprSimplify::visit(const ExprAtanh& e) { unary(atanh, atanh); }

} /* namespace ibex */
