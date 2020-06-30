//============================================================================
//                                  I B E X                                   
// File        : ibex_ExprSimplify2.cpp
// Author      : Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Mar 27, 2020
// Last update : May 15, 2020
//============================================================================

#include "ibex_ExprSimplify2.h"
#include "ibex_ExprSubNodes.h"
#include "ibex_Expr.h"
#include "ibex_Map.h"

#include <iostream>
#include <typeinfo>
#include <algorithm>


using namespace std;

namespace ibex {

namespace {

bool is_cst(const ExprNode& e) {
	return dynamic_cast<const ExprConstant*>(&e)!=NULL;
}

bool is_mutable(const ExprNode& e) {
	return ((const ExprConstant&) e).is_mutable();
}

bool is_index(const ExprNode& e) {
	return dynamic_cast<const ExprIndex*>(&e)!=NULL;
}

bool is_minus(const ExprNode& e) {
	return dynamic_cast<const ExprMinus*>(&e)!=NULL;
}

bool is_trans(const ExprNode& e) {
	return dynamic_cast<const ExprTrans*>(&e)!=NULL;
}

bool is_add(const ExprNode& e) {
	return dynamic_cast<const ExprAdd*>(&e)!=NULL;
}

bool is_sub(const ExprNode& e) {
	return dynamic_cast<const ExprSub*>(&e)!=NULL;
}

bool is_mul(const ExprNode& e) {
	return dynamic_cast<const ExprMul*>(&e)!=NULL;
}

//bool is_div(const ExprNode& e) {
//	return dynamic_cast<const ExprDiv*>(&e)!=NULL;
//}

bool is_vec(const ExprNode& e) {
	return dynamic_cast<const ExprVector*>(&e)!=NULL;
}

const Domain& to_cst(const ExprNode& e) {
	const ExprConstant* c=dynamic_cast<const ExprConstant*>(&e);
	assert(c);
	return c->get();
}

const ExprNode& left(const ExprNode& e) {
	const ExprBinaryOp* b=dynamic_cast<const ExprBinaryOp*>(&e);
	assert(b);
	return b->left;
}

const ExprNode& right(const ExprNode& e) {
	const ExprBinaryOp* b=dynamic_cast<const ExprBinaryOp*>(&e);
	assert(b);
	return b->right;
}

const ExprNode& expr(const ExprNode& e) {
	const ExprUnaryOp* u=dynamic_cast<const ExprUnaryOp*>(&e);
	assert(u);
	return u->expr;
}

} // end anonymous namespace

ExprSimplify2::ExprSimplify2(bool develop) : _2polynom(*this, develop) {

}

const ExprNode& ExprSimplify2::rec(const ExprNode& e) {
	record.push_back(&e);
	return e;
}

const ExprNode& ExprSimplify2::simplify(const ExprNode& e) {
	const ExprNode& _result = *visit(e);
	//cout  << "result=" << _result << endl;
	const ExprNode& result = _2polynom.get(_result)->to_expr(&record);

	// If a node does not appear in the final expression
	// AND is not a node of the original expression, it has to be freed.

	ExprSubNodes old_nodes(e);
	ExprSubNodes new_nodes(result);

	/* All the new nodes created in the simplification process
	 * and that does not appear in the final expression have just
	 * to be deleted
	 */
	for (vector<const ExprNode*>::iterator it=record.begin(); it!=record.end(); ++it) {
		if (!new_nodes.found(**it)) {
			delete *it;
		}
	}
	record.clear(); // important for next call to simplify()

	vector<const ExprNode*> expr_symbols;

	/** All the old nodes that are neither symbols, locked
	 * or in the final expression also have to be deleted
	 */
	for (int i=0; i<old_nodes.size(); i++) {
		if (!dynamic_cast<const ExprSymbol*>(&old_nodes[i]) &&
				!new_nodes.found(old_nodes[i]) &&
				!lock.found(old_nodes[i])) {
			delete &old_nodes[i];
		}
		// the list of fathers must also be recalculated
		// for all these nodes !!
		// and this is not as simple as the next commented loop
		// as  the old nodes may have fathers "alive"
		// that do not belong to the new nodes.
		//			 ((ExprNode&) old_nodes[i]).fathers.clear();
	}


	//	 for (int i=0; i<new_nodes.size(); i++) {
	//		 if (dynamic_cast<const ExprNAryOp*>(&new_nodes[i])) {
	//			 const ExprNAryOp& nary=(const ExprNAryOp&) new_nodes[i];
	//			 for (int j=0; j<nary.nb_args; j++)
	//				 ((ExprNode&) nary.args[j]).fathers.add(nary);
	//		 } else if (dynamic_cast<const ExprBinaryOp*>(&new_nodes[i])) {
	//			 const ExprBinaryOp& b=(const ExprBinaryOp&) new_nodes[i];
	//			 ((ExprNode&) b.left).fathers.add(b);
	//			 ((ExprNode&) b.right).fathers.add(b);
	//		 } else if (dynamic_cast<const ExprUnaryOp*>(&new_nodes[i])) {
	//			 const ExprUnaryOp& u=(const ExprUnaryOp&) new_nodes[i];
	//			 ((ExprNode&) u.expr).fathers.add(u);
	//		 } else if (dynamic_cast<const ExprIndex*>(&new_nodes[i])) {
	//			 const ExprIndex& index=(const ExprIndex&) new_nodes[i];
	//			 ((ExprNode&) index.expr).fathers.add(index);
	//		 }
	//	 }

	//important: clear the cache, some nodes have been deleted!
	cache.clean();
	_2polynom.cleanup();

	return result;
}

const ExprNode* ExprSimplify2::visit(const ExprNode& e) {
	return ExprVisitor<const ExprNode*>::visit(e);
}

const ExprNode* ExprSimplify2::visit(const ExprIndex& e) {

	const ExprNode* expr2 = visit(e.expr);
	assert(expr2->dim==e.expr.dim);

	if (is_cst(*expr2)) {
		if (is_mutable(*expr2))
			return &rec(ExprConstant::new_mutable(to_cst(*expr2)[e.index]));
		else
			return &rec(ExprConstant::new_(to_cst(*expr2)[e.index]));
	} else if (e.index.all()) {
		return &*expr2;
	} else if (is_index(*expr2)) {
		const ExprIndex* id2= (const ExprIndex*) expr2;
		return &rec(id2->expr[id2->index[e.index]]);
	} else if (is_minus(*expr2)) {
		return visit(rec(-rec(expr(*expr2)[e.index])));
	} else if (is_trans(*expr2)) {
		return visit(rec(expr(*expr2)[e.index.transpose()]));
	} else if (is_add(*expr2)) {
		return visit(rec(rec(left(*expr2)[e.index]) + rec(right(*expr2)[e.index])));
	} else if (is_sub(*expr2)) {
		return visit(rec(rec(left(*expr2)[e.index]) - rec(right(*expr2)[e.index])));
	} else if (is_mul(*expr2)) {
		if (left(*expr2).dim.is_scalar())
			if (is_cst(*expr2) && !is_mutable(*expr2) && to_cst(*expr2).is_zero())
				return &rec(ExprConstant::new_matrix(Matrix::zeros(e.dim.nb_rows(),e.dim.nb_cols())));
			else
				return visit(rec(left(*expr2) * rec(right(*expr2)[e.index])));
		else
			return visit(rec(
				rec(left(*expr2)[e.index.rows(left(*expr2).dim,e.index.first_row(),e.index.last_row())]) *
				rec(right(*expr2)[e.index.cols(right(*expr2).dim,e.index.first_col(),e.index.last_col())])));
	} else if (is_vec(*expr2)) {
		vector<const ExprNode*> res;
		const DoubleIndex& idx=e.index;
		const ExprVector& v=(const ExprVector&) *expr2;
		unsigned int i=0;
		if (v.row_vector()) {
			int c=0;
			while (c<=idx.last_col()) {
				int n=v.arg(i).dim.nb_cols();
				int first_col=idx.first_col()-c;
				if (first_col<n) { // can be negative
					int last_col=idx.last_col()-c;
					DoubleIndex e_idx=DoubleIndex(v.arg(i).dim,
							idx.first_row(), idx.last_row(),
							first_col<0? 0 : first_col, last_col>=n? n-1 : last_col);
					res.push_back(& rec(v.arg(i)[e_idx]));
				}
				c+=n;
				assert(((int) i)<v.nb_args);
				i++;
			}
		} else {
			int r=0;
			while (r<=idx.last_row()) {
				int n=v.arg(i).dim.nb_rows();
				int first_row=idx.first_row()-r;
				if (first_row<n) { // can be negative
					int last_row=idx.last_row()-r;
					DoubleIndex e_idx=DoubleIndex(v.arg(i).dim,
							first_row<0? 0 : first_row, last_row>=n? n-1 : last_row,
									idx.first_col(), idx.last_col());
					res.push_back(& rec(v.arg(i)[e_idx]));
				}
				r+=n;
				assert(((int) i)<v.nb_args);
				i++;
			}
		}
		if (res.size()==1)
			return visit(*res.back());
		else
			return visit(rec(ExprVector::new_(res,v.orient)));
	} else if (expr2==&e.expr) {
		return &e;
	} else
		return &rec((*expr2)[e.index]);
}

const ExprNode* ExprSimplify2::visit(const ExprSymbol& e) {
	return &e;
}

const ExprNode* ExprSimplify2::visit(const ExprConstant& e) {
	return &e;
}


// Note: I don't understand why I have to do these casts.
// The compiler should directly convert arguments to
//      std::function<Domain(const Domain&)>
// or
//      std::function<Domain(const Domain&, const Domain&)>
typedef Domain (*_domain_una_op)(const Domain&);
typedef Domain (*_domain_bin_op)(const Domain&, const Domain&);


const ExprNode* ExprSimplify2::unary(const ExprUnaryOp& e,
		std::function<Domain(const Domain&)> fcst,
		std::function<const ExprNode&(const ExprNode&)> fctr) {

	const ExprNode* expr2=visit(e.expr);
	assert(expr2->dim==e.expr.dim);

	if (is_cst(*expr2)) {
		if (is_mutable(*expr2))
			return &rec(fctr(*expr2));
		else
			return &rec(ExprConstant::new_(fcst(to_cst(*expr2))));
	} else if (!e.dim.is_scalar()) { // for MINUS --> distribute over components
		assert(dynamic_cast<const ExprMinus*>(&e));
		bool row = e.dim.is_matrix() || e.dim.type()==Dim::ROW_VECTOR;
		// note: if e.expr is a vector, we don't necessarily iterate over
		// arguments of v, because dimensions may be non homogeneous (we let
		// the simplification of ExprIndex do the job..)
		Array<const ExprNode> args(e.dim.vec_size());
		for (int i=0; i<e.dim.vec_size(); i++)
			args.set_ref(i, rec(fctr(rec((*expr2)[i]))));
		return visit(rec(ExprVector::new_(args, row ? ExprVector::ROW : ExprVector::COL)));
	} else if (expr2==&e.expr) {
		return &e;
	} else
		return &rec(fctr(*expr2));
}

const ExprNode* ExprSimplify2::binary(const ExprBinaryOp& e,
		std::function<Domain(const Domain&, const Domain&)> fcst,
		std::function<const ExprBinaryOp&(const ExprNode&, const ExprNode&)> fctr) {

	const ExprNode* l2=visit(e.left);
	assert(l2->dim==e.left.dim);
	const ExprNode* r2=visit(e.right);
	assert(r2->dim==e.right.dim);

	bool all_same = (l2==&e.left && r2==&e.right);

	if (is_cst(*l2) && !is_mutable(*l2) && is_cst(*r2) && !is_mutable(*r2)) {
		return &rec(ExprConstant::new_(fcst(to_cst(*l2), to_cst(*r2))));
	} else if ((!l2->dim.is_scalar() && (is_vec(*l2) || is_cst(*l2)))
			|| (!r2->dim.is_scalar() && (is_vec(*r2) || is_cst(*r2)))) { // for ADD and SUB --> distribute addition / vector

		Array<const ExprNode> rows(e.dim.nb_rows());
		Array<const ExprNode> col (e.dim.nb_cols());

		for (int i=0; i<e.dim.nb_rows(); i++) {
			col.clear();
			for (int j=0; j<e.dim.nb_cols(); j++) {
				col.set_ref(j,rec(fctr(
						rec((*l2)[DoubleIndex::one_elt(e.dim,i,j)]),
						rec((*r2)[DoubleIndex::one_elt(e.dim,i,j)]))));
			}
			if (e.dim.nb_cols()>1)
				rows.set_ref(i, rec(ExprVector::new_(col,ExprVector::ROW)));
			else
				rows.set_ref(i, col[0]);
		}

		if (e.dim.nb_rows()>1)
			return visit(rec(ExprVector::new_(rows,ExprVector::COL)));
		else
			return visit(rows[0]);
	} else if (all_same) {
		return &e;
	} else
		return &rec(fctr(*l2,*r2));
}

const ExprNode* ExprSimplify2::nary(const ExprNAryOp& e,
		std::function<Domain(Array<const Domain>&)> fcst,
		std::function<const ExprNAryOp&(Array<const ExprNode>&)> f) {

	bool all_cst = true;
	bool all_same = true;
	Array<const ExprNode> args2(e.nb_args);
	for (int i=0; i<e.nb_args; i++) {
		args2.set_ref(i,*visit(e.arg(i)));
		all_cst &= ( is_cst(args2[i]) && !is_mutable(args2[i]) );
		all_same &= (&args2[i]==&e.arg(i));
	}

	if (all_cst) {
		Array<const Domain> arg_cst(e.nb_args);
		for (int i=0; i<e.nb_args; i++) {
			arg_cst.set_ref(i, to_cst(args2[i]));
		}
		return &rec(ExprConstant::new_(fcst(arg_cst)));
	} else if (all_same) {
		return &e;
	} else {
		return &rec(f(args2));
	}
}

const ExprNode* ExprSimplify2::visit(const ExprVector& e) {
	// this block copy-pasted from nary(...)
	// <<--------------
	bool all_cst = true;
	bool all_same = true;
	Array<const ExprNode> args2(e.nb_args);
	for (int i=0; i<e.nb_args; i++) {
		args2.set_ref(i,*visit(e.arg(i)));
		all_cst &= ( is_cst(args2[i]) && !is_mutable(args2[i]) );
		all_same &= (&args2[i]==&e.arg(i));
	}

	if (all_cst) {
		Array<const Domain> arg_cst(e.nb_args);
		for (int i=0; i<e.nb_args; i++) {
			arg_cst.set_ref(i, to_cst(args2[i]));
		}
		return &rec(ExprConstant::new_(Domain(arg_cst, e.row_vector())));
	}
	// -------------->>
	else if (args2.size()==1)
		return &args2[0]; // no rec here!
	else if (all_same) {
		return &e;
	} else {
		return &rec(ExprVector::new_(args2, e.orient));
	}
}

const ExprNode* ExprSimplify2::visit(const ExprChi& e) {
	return nary(e,
			[](Array<const Domain>& args) { return chi(args[0],args[1],args[2]); },
			[](const Array<const ExprNode>& args)->const ExprChi& { return ExprChi::new_(args); });
}

const ExprNode* ExprSimplify2::visit(const ExprApply& e) { return &e; /* not implemented */ }

const ExprNode* ExprSimplify2::visit(const ExprGenericBinaryOp& e) {
	return binary( e,
			e.eval,
			[&e](const ExprNode& x,const ExprNode& y)->const ExprBinaryOp& { return ExprGenericBinaryOp::new_(e.name,x,y);});
}

const ExprNode* ExprSimplify2::visit(const ExprAdd& e)   { return binary(e, (_domain_bin_op) operator+, ExprAdd::new_); }
const ExprNode* ExprSimplify2::visit(const ExprSub& e)   { return binary(e, (_domain_bin_op) operator-, ExprSub::new_); }

const ExprNode* ExprSimplify2::visit(const ExprMul& e)   {
	const ExprNode* l2=visit(e.left);
	assert(l2->dim==e.left.dim);
	const ExprNode* r2=visit(e.right);
	assert(r2->dim==e.right.dim);

	bool all_same = (l2==&e.left && r2==&e.right);

	if (is_cst(*l2) && !is_mutable(*l2) && is_cst(*r2) && !is_mutable(*r2)) {
		return &rec(ExprConstant::new_(to_cst(*l2) * to_cst(*r2)));
	} else if ((!l2->dim.is_scalar() && (is_vec(*l2) || is_cst(*l2)))
			|| (!r2->dim.is_scalar() && (is_vec(*r2) || is_cst(*r2)))) { // distribute multiplication / vector

		Array<const ExprNode> cols(r2->dim.nb_cols());
		if (l2->dim.is_scalar()) { // scalar*vector/matrix case
			// note: a row ExprVector of column is more efficient than a column ExprVector of rows.
			if (is_cst(*l2) && !is_mutable(*l2) && to_cst(*l2).is_zero())
				return &rec(ExprConstant::new_matrix(Matrix::zeros(r2->dim.nb_rows(),r2->dim.nb_cols())));

			Array<const ExprNode> col (r2->dim.nb_rows());
			for (int j=0; j<r2->dim.nb_cols(); j++) {
				col.clear();
				for (int i=0; i<r2->dim.nb_rows(); i++) {
					col.set_ref(i, rec((*l2)*rec((*r2)[DoubleIndex::one_elt(r2->dim,i,j)])));
				}
				if (r2->dim.nb_rows()>1)
					cols.set_ref(j, rec(ExprVector::new_(col,ExprVector::COL)));
				else
					cols.set_ref(j, col[0]);
			}
		} else {
			Array<const ExprNode> col (l2->dim.nb_rows());
			for (int j=0; j<r2->dim.nb_cols(); j++) {
				col.clear();
				for (int i=0; i<l2->dim.nb_rows(); i++) {
					const ExprNode* e=NULL;

					for (int k=0; k<l2->dim.nb_cols(); k++) {
						if (e)
							e = & rec(*e +
									rec(rec((*l2)[DoubleIndex::one_elt(l2->dim,i,k)])*
											rec((*r2)[DoubleIndex::one_elt(r2->dim,k,j)]))
							);
						else
							e = & rec(
									rec((*l2)[DoubleIndex::one_elt(l2->dim,i,k)])*
									rec((*r2)[DoubleIndex::one_elt(r2->dim,k,j)])
							);
					}
					col.set_ref(i,*e);
				}
				if (l2->dim.nb_rows()>1)
					cols.set_ref(j, rec(ExprVector::new_(col,ExprVector::COL)));
				else
					cols.set_ref(j, col[0]);
			}
		}
		if (r2->dim.nb_cols()>1) {
			return visit(rec(ExprVector::new_(cols,ExprVector::ROW)));
		}
		else
			return visit(cols[0]);
	} else if (all_same) {
		return &e;
	} else
		return &rec((*l2)*(*r2));
}

const ExprNode* ExprSimplify2::visit(const ExprDiv& e)   { return binary(e, (_domain_bin_op) operator/, ExprDiv::new_); }
const ExprNode* ExprSimplify2::visit(const ExprMax& e)   { return binary(e, (_domain_bin_op) max,       ExprMax::new_); }
const ExprNode* ExprSimplify2::visit(const ExprMin& e)   { return binary(e, (_domain_bin_op) min,       ExprMin::new_); }
const ExprNode* ExprSimplify2::visit(const ExprAtan2& e) { return binary(e, (_domain_bin_op) atan2,     ExprAtan2::new_); }



const ExprNode* ExprSimplify2::visit(const ExprPower& e) {
	return unary(e,
			[&e](const Domain& x)->Domain { return pow(x,e.expon); },
			[&e](const ExprNode& expr)->const ExprNode& { return ibex::pow(expr,e.expon); });
}

const ExprNode* ExprSimplify2::visit(const ExprTrans& e) {

	const ExprNode* expr2 = visit(e.expr);

	if (is_cst(*expr2) && !is_mutable(*expr2)) {
		return &rec(ExprConstant::new_(transpose(to_cst(*expr2))));
	} else if (is_trans(*expr2)) {
		return &expr(*expr2); // no need to visit again
	} else if (is_index(*expr2)) {
		const ExprIndex& i=(const ExprIndex&) *expr2;
		return visit(rec(i.expr[i.index.transpose()]));
	} else if (is_minus(*expr2)) {
		return visit(rec(-rec(transpose(expr(*expr2)))));
	} else if (is_add(*expr2)) {
		return visit(rec(rec(transpose(left(*expr2))) + rec(transpose(right(*expr2)))));
	} else if (is_sub(*expr2)) {
		return visit(rec(rec(transpose(left(*expr2))) - rec(transpose(right(*expr2)))));
	} else if (is_mul(*expr2)) {
		return visit(rec(rec(transpose(right(*expr2))) * rec(transpose(left(*expr2)))));
	} else if (is_vec(*expr2)) {
		const ExprVector& ev=(const ExprVector&) *expr2;
		// no need to visit again
		return &rec(ExprVector::new_(ev.args, ev.orient==ExprVector::ROW? ExprVector::COL : ExprVector::ROW));
	} else if (e.dim.is_scalar())
		return expr2;
	else if (expr2==&e.expr) {
		return &e;
	} else {
		return &rec(transpose(*expr2));
	}
}

const ExprNode* ExprSimplify2::visit(const ExprGenericUnaryOp& e) {
	return unary(e, e.eval,
			[&e](const ExprNode& x)->const ExprUnaryOp& { return ExprGenericUnaryOp::new_(e.name,x);});
}

const ExprNode* ExprSimplify2::visit(const ExprMinus& e) { return unary(e, (_domain_una_op) operator-, ExprMinus::new_); }
const ExprNode* ExprSimplify2::visit(const ExprSqr& e)   { return unary(e, (_domain_una_op) sqr,  ExprSqr::new_);  }
const ExprNode* ExprSimplify2::visit(const ExprSign& e)  { return unary(e, (_domain_una_op) sign, ExprSign::new_);  }
const ExprNode* ExprSimplify2::visit(const ExprAbs& e)   { return unary(e, (_domain_una_op) abs,  ExprAbs::new_);  }
const ExprNode* ExprSimplify2::visit(const ExprSqrt& e)  { return unary(e, (_domain_una_op) sqrt, ExprSqrt::new_); }
const ExprNode* ExprSimplify2::visit(const ExprExp& e)   { return unary(e, (_domain_una_op) exp,  ExprExp::new_); }
const ExprNode* ExprSimplify2::visit(const ExprLog& e)   { return unary(e, (_domain_una_op) log,  ExprLog::new_); }
const ExprNode* ExprSimplify2::visit(const ExprCos& e)   { return unary(e, (_domain_una_op) cos,  ExprCos::new_); }
const ExprNode* ExprSimplify2::visit(const ExprSin& e)   { return unary(e, (_domain_una_op) sin,  ExprSin::new_); }
const ExprNode* ExprSimplify2::visit(const ExprTan& e)   { return unary(e, (_domain_una_op) tan,  ExprTan::new_); }
const ExprNode* ExprSimplify2::visit(const ExprCosh& e)  { return unary(e, (_domain_una_op) cosh, ExprCosh::new_); }
const ExprNode* ExprSimplify2::visit(const ExprSinh& e)  { return unary(e, (_domain_una_op) sinh, ExprSinh::new_); }
const ExprNode* ExprSimplify2::visit(const ExprTanh& e)  { return unary(e, (_domain_una_op) tanh, ExprTanh::new_); }
const ExprNode* ExprSimplify2::visit(const ExprAcos& e)  { return unary(e, (_domain_una_op) acos, ExprAcos::new_); }
const ExprNode* ExprSimplify2::visit(const ExprAsin& e)  { return unary(e, (_domain_una_op) asin, ExprAsin::new_); }
const ExprNode* ExprSimplify2::visit(const ExprAtan& e)  { return unary(e, (_domain_una_op) atan, ExprAtan::new_); }
const ExprNode* ExprSimplify2::visit(const ExprAcosh& e) { return unary(e, (_domain_una_op) acosh,ExprAcosh::new_); }
const ExprNode* ExprSimplify2::visit(const ExprAsinh& e) { return unary(e, (_domain_una_op) asinh,ExprAsinh::new_); }
const ExprNode* ExprSimplify2::visit(const ExprAtanh& e) { return unary(e, (_domain_una_op) atanh,ExprAtanh::new_); }
const ExprNode* ExprSimplify2::visit(const ExprFloor& e) { return unary(e, (_domain_una_op) floor,ExprFloor::new_); }
const ExprNode* ExprSimplify2::visit(const ExprCeil& e)  { return unary(e, (_domain_una_op) ceil, ExprCeil::new_); }
const ExprNode* ExprSimplify2::visit(const ExprSaw& e)   { return unary(e, (_domain_una_op) saw,  ExprSaw::new_);  }

} // namespace ibex
