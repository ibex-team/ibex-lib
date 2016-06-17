/*
 * ibex_ExprSimplify.cpp
 *
 *  Created on: May 19, 2016
 *      Author: gilles
 */

#include "ibex_ExprSimplify.h"
#include "ibex_Expr.h"
#include "ibex_ExprSubNodes.h"
#include "ibex_NodeMap.h"

using namespace std;

#define CLONE_VEC vector<pair<DoubleIndex, const ExprNode*> >

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
		default:              return d.m()==Matrix::eye(d.dim.nb_rows());
		}
	} else {
		return false;
	}
}

} // end anonymous namespace

const ExprNode& ExprSimplify::simplify(const ExprNode& e) {

	idx = DoubleIndex::all(e.dim);
	e.acceptVisitor(*this);
	const ExprNode& result = get(e,idx);

	// If a node does not appear in the final expression
	// AND is not a node of the original expression, it has to be freed.
	//
	// Note: we cannot free copies of constant nodes as we "fold" them
	// because a constant node may be pointed to by another node than
	// the current father (we have DAG, not a tree). Ex: assuming the
	// node "1" is the same in both subexpressions:
	//           (1*2)+(x+1)
	// we must not delete "1" when copying (1*2).

	ExprSubNodes old_nodes(e);
	ExprSubNodes new_nodes(result);

	NodeMap<bool> all_nodes;

	for (IBEX_NODE_MAP(CLONE_VEC*)::const_iterator it=idx_clones.begin();
			it!=idx_clones.end(); it++) {
		for (CLONE_VEC::const_iterator it2=it->second->begin(); it2!=it->second->end(); it2++) {
			if (!all_nodes.found(*it2->second))
				all_nodes.insert(*it2->second,true);
		}
		delete it->second;
	}

	/** auto-cleanup................ */
	for (int i=0; i<old_nodes.size(); i++) {
		if (!all_nodes.found(old_nodes[i]))
			all_nodes.insert(old_nodes[i],true);
	}
	/*...............................*/

	for (IBEX_NODE_MAP(bool)::const_iterator it=all_nodes.begin(); it!=all_nodes.end(); it++) {
		if (/* auto-cleanup, so remove this ----> !old_nodes.found(*it->first) && */
			!dynamic_cast<const ExprSymbol*>(it->first) && !new_nodes.found(*it->first)) {
			delete it->first;
		}
	}

	idx_clones.clean();
	return result;
}

void ExprSimplify::insert(const ExprNode& e, const ExprNode& e2) {
	// first time we access to node e:
	if (!idx_clones.found(e)) {
		idx_clones.insert(e,new CLONE_VEC());
	}
	idx_clones[e]->push_back(pair<DoubleIndex,const ExprNode*>(idx,&e2));
}

const ExprNode& ExprSimplify::get(const ExprNode& e, const DoubleIndex& idx2) {
	// first time we access to node e:
	if (!idx_clones.found(e)) {
		idx_clones.insert(e,new CLONE_VEC());
	}

	CLONE_VEC& v=*(idx_clones[e]);
	unsigned int i=0;
	while (i<v.size() && v[i].first!=idx2)
		i++;
	if (i==v.size()) { // idx2 not found in the clone list
		DoubleIndex old_idx=idx;
		idx=idx2;
		e.acceptVisitor(*this);
		idx=old_idx;
	}
	assert(v.back().first==idx2);

	return *v[i].second;
}

void ExprSimplify::visit(const ExprVector& e) {

	vector<const ExprNode*> res;
	unsigned int i=0;
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
						first_col<0? 0 : first_col, last_col>=n? n-1 : last_col);
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
						first_row<0? 0 : first_row, last_row>=n? n-1 : last_row,
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
	DoubleIndex e_idx=DoubleIndex(e.expr.dim,
			e.index.first_row()+idx.first_row(),
			e.index.first_row()+idx.last_row(),
			e.index.first_col()+idx.first_col(),
			e.index.first_col()+idx.last_col());

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
	DoubleIndex l_idx;
	DoubleIndex r_idx;

	if (e.left.dim.is_scalar()) {
		l_idx=DoubleIndex::all(e.left.dim);
		r_idx=idx;
	} else {
		l_idx=DoubleIndex::rows(e.left.dim,idx.first_row(),idx.last_row());
		r_idx=DoubleIndex::cols(e.right.dim,idx.first_col(),idx.last_col());
	}

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
template<class N>
void ExprSimplify::binary(const N& e, dom_func2 fcst) {

	const ExprNode& l=get(e.left, idx);
	const ExprNode& r=get(e.right, idx);

	if (is_cst(l) && is_cst(r))
		insert(e, ExprConstant::new_(fcst(to_cst(l),to_cst(r))));
	else if ((&l == &e.left) && (&r == &e.right))  // nothing changed
		insert(e, e);
	else
		insert(e, N::new_(l,r));
}

template<class N>
void ExprSimplify::unary(const N& e, Domain (*fcst)(const Domain&)) {

	const ExprNode& expr=get(e.expr, idx);

	if (is_cst(expr))
		/* evaluate the constant expression on-the-fly */
		insert(e, ExprConstant::new_(fcst(to_cst(expr))));
	else if (&e.expr == &expr)  // if nothing changed
		insert(e, e);
	else
		insert(e, N::new_(expr));
}

// Implemented by Soonho
void ExprSimplify::visit(const ExprChi& e) {
	const ExprNode& arg0=get(e.args[0], idx);
	if (is_cst(arg0)) {
		assert(arg0.dim.is_scalar());
		const Interval& c = to_cst(arg0).i();
		if (c.lb() > 0) {
			// if c > 0, chi(c, arg1, arg2) reduces to arg1
			const ExprNode& arg1=get(e.args[1], idx);
			insert(e, arg1);
			return;
		} else if (c.ub() <= 0) {
			// if c <= 0, chi(c, arg1, arg2) reduces to arg2
			const ExprNode& arg2=get(e.args[2], idx);
			insert(e, arg2);
			return;
		}
	} else {
		insert(e, e);
	}
}

void ExprSimplify::visit(const ExprApply& e) {
	not_implemented("Simplify with Apply");
}

void ExprSimplify::visit(const ExprPower& e) {

	const ExprNode& expr=get(e.expr, idx);

	if (is_cst(expr))
		/* evaluate the constant expression on-the-fly */
		insert(e, ExprConstant::new_(pow(to_cst(expr),e.expon)));
	else if (&e.expr == &expr)  // if nothing changed
		insert(e, e);
	else
		insert(e, ExprPower::new_(expr,e.expon));
}

void ExprSimplify::visit(const ExprMax& e)   { binary(e,max); }
void ExprSimplify::visit(const ExprMin& e)   { binary(e,min); }
void ExprSimplify::visit(const ExprAtan2& e) { binary(e,atan2); }
void ExprSimplify::visit(const ExprMinus& e) { unary(e,operator-); }

void ExprSimplify::visit(const ExprTrans& e) {
	const ExprNode& expr=get(e.expr, idx.transpose());

	if (is_cst(expr))
		/* evaluate the constant expression on-the-fly */
		insert(e, ExprConstant::new_(transpose(to_cst(expr))));
	else if (expr.dim.is_scalar())
		insert(e,expr);
	else if (&e.expr == &expr)  // if nothing changed
		insert(e, e);
	else
		insert(e, ExprTrans::new_(expr));
}

void ExprSimplify::visit(const ExprSign& e)  { unary(e,sign); }
void ExprSimplify::visit(const ExprAbs& e)   { unary(e,abs); }
void ExprSimplify::visit(const ExprSqr& e)   { unary(e,sqr); }
void ExprSimplify::visit(const ExprSqrt& e)  { unary(e,sqrt); }
void ExprSimplify::visit(const ExprExp& e)   { unary(e,exp); }
void ExprSimplify::visit(const ExprLog& e)   { unary(e,log); }
void ExprSimplify::visit(const ExprCos& e)   { unary(e,cos); }
void ExprSimplify::visit(const ExprSin& e)   { unary(e,sin); }
void ExprSimplify::visit(const ExprTan& e)   { unary(e,tan); }
void ExprSimplify::visit(const ExprCosh& e)  { unary(e,cosh); }
void ExprSimplify::visit(const ExprSinh& e)  { unary(e,sinh); }
void ExprSimplify::visit(const ExprTanh& e)  { unary(e,tanh); }
void ExprSimplify::visit(const ExprAcos& e)  { unary(e,acos); }
void ExprSimplify::visit(const ExprAsin& e)  { unary(e,asin); }
void ExprSimplify::visit(const ExprAtan& e)  { unary(e,atan); }
void ExprSimplify::visit(const ExprAcosh& e) { unary(e,acosh); }
void ExprSimplify::visit(const ExprAsinh& e) { unary(e,asinh); }
void ExprSimplify::visit(const ExprAtanh& e) { unary(e,atanh); }

} /* namespace ibex */
