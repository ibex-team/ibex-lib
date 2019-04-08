/* ============================================================================
 * I B E X - Expression linearity check
 * ============================================================================
 * Copyright   : IMT Atlantique (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : July 3rd, 2017
 * ---------------------------------------------------------------------------- */

#include "ibex_ExprLinearity.h"

using namespace std;

namespace ibex {

ExprLinearity::ExprLinearity(const Array<const ExprSymbol> x, const ExprNode& y) :
		n(0) {

	for (int i=0; i<x.size(); i++)
		n+=x[i].dim.size();

	int k=0;
	for (int i=0; i<x.size(); i++) {
		visit(x[i],k); // requires n to be known
		k+=x[i].dim.size();
	}

	visit(y);
}

ExprLinearity::~ExprLinearity() {
	for (IBEX_NODE_MAP(LinData)::const_iterator it=_coeffs.begin(); it!=_coeffs.end(); it++) {
		Array<Domain>& d=*(it->second.first);

		for (int i=0; i<n+1; i++) {
			delete &d[i];
		}
		delete it->second.first;
	}
}

IntervalVector ExprLinearity::coeff_vector(const ExprNode& e) const {

	assert(e.dim.type()==Dim::SCALAR);

	IntervalVector c(n+1);

	if (!_coeffs.found(e))
		c.set_empty();
	else
		load(c,*(_coeffs[e].first));
	return c;
}

IntervalMatrix ExprLinearity::coeff_matrix(const ExprNode& e) const {

	assert(!e.dim.is_matrix());

	if (!_coeffs.found(e)) {
		return IntervalMatrix::empty(e.dim.vec_size(), n+1);
	} else {
		IntervalMatrix c(n+1, e.dim.vec_size()); // the transpose of the result

		for (int i=0; i<n+1; i++)
			c.row(i)=(*(_coeffs[e].first))[i].v();

		return c.transpose();
	}
}

Array<Domain>* ExprLinearity::build_zero(const Dim& dim) const {
	Array<Domain>* d=new Array<Domain>(n+1);

	for (int i=0; i<n+1; i++) {
		d->set_ref(i,*new Domain(dim));
		(*d)[i].clear();
	}
	return d;
}

ExprLinearity::LinData ExprLinearity::build_cst(const Domain& value) const {
	Array<Domain>* d=build_zero(value.dim);
	(*d)[n]=value;
	return make_pair(d,CONSTANT);
}

void ExprLinearity::visit(const ExprSymbol& e, int k) {
	Array<Domain>* d=build_zero(e.dim);

	switch(e.dim.type()) {
	case Dim::SCALAR:
		(*d)[k].i()=Interval::ONE;
		break;
	case Dim::COL_VECTOR:
	case Dim::ROW_VECTOR:
		for (int j=0; j<e.dim.vec_size(); j++)
			(*d)[k+j].v()[j]=Interval::ONE;
		break;
	case Dim::MATRIX:
		// we know that matrix variables are
		// "flattened" row by row.
		// See, e.g., ibex_TemplateDomain.h
		for (int i=0; i<e.dim.nb_rows(); i++)
			for (int j=0; j<e.dim.nb_cols(); j++)
				(*d)[k+(i*e.dim.nb_cols())+j].m()[i][j]=Interval::ONE;
		break;
	}

	_coeffs.insert(e, make_pair(d,LINEAR));
}

void ExprLinearity::visit(const ExprNode& e) {
	if (!_coeffs.found(e))
		e.acceptVisitor(*this);
}

void ExprLinearity::visit(const ExprIndex& e) {
	visit(e.expr);

	Array<Domain>& expr_d=*(_coeffs[e.expr].first);
	nodetype expr_type=_coeffs[e.expr].second;

	if (expr_type==CONSTANT)
		_coeffs.insert(e,build_cst(expr_d[n][e.index]));
	else {
		Array<Domain>* d=new Array<Domain>(n+1);
		nodetype type=CONSTANT; // by default

		for (int i=0; i<n+1; i++) {
			d->set_ref(i,*new Domain(expr_d[i][e.index]));
			if (i<n) {
				if (expr_type!=CONSTANT // just to avoid following test if useless
						&& type==CONSTANT && !(*d)[i].is_zero())
					type=expr_type;

				if (expr_type!=LINEAR // just to avoid following test if useless
						&& type==LINEAR && (*d)[i].is_unbounded())
					type=expr_type;
			}
		}
		_coeffs.insert(e, make_pair(d,type));
	}
}

void ExprLinearity::visit(const ExprConstant& e) {
	_coeffs.insert(e, build_cst(e.get()));
}

void ExprLinearity::visit(const ExprVector& e) {
	Array<Domain>* d=new Array<Domain>(n+1);
	nodetype type=CONSTANT; // by default

	for (int j=0; j<e.length(); j++) {
		visit(e.arg(j));
		nodetype arg_type=_coeffs[e.arg(j)].second;
		if (type==CONSTANT && arg_type!=CONSTANT) type=arg_type;
		if (type==LINEAR && arg_type==NONLINEAR) type=NONLINEAR;
	}

	for (int i=0; i<n+1; i++) {
		Array<const Domain> array(e.length());

		for (int j=0; j<e.length(); j++) {
			Array<Domain>& arg_d=*(_coeffs[e.arg(j)].first);
			array.set_ref(j,arg_d[i]);
		}

		// some unbounded domains (<->nonlinearity) may appear inside
		// the array
		d->set_ref(i,*new Domain(array, e.row_vector()));
	}

	_coeffs.insert(e, make_pair(d,type));
}

void ExprLinearity::visit(const ExprMul& e) {
	Array<Domain>* d=new Array<Domain>(n+1);
	nodetype type=CONSTANT; // by default

	visit(e.left);
	visit(e.right);

	Array<Domain>& l=*(_coeffs[e.left].first);
	Array<Domain>& r=*(_coeffs[e.right].first);

	nodetype left_type = _coeffs[e.left].second;
	nodetype right_type = _coeffs[e.right].second;

	for (int i=0; i<n+1; i++) {
		d->set_ref(i,*new Domain(e.dim));
		(*d)[i].clear();
	}

	Domain non_linear(Dim::scalar()); // =ALL_REALS

	// can be in O(n^4) :-(
	for (int i=0; i<n+1; i++) {
		for (int j=0; j<n+1; j++) {

			if (i<n && j<n) {
				// we introduce the "non_linear" coefficient so that
				// a non-null term x_i*x_j will have its corresponding
				// entry set to [-oo,oo]
				Domain prod=non_linear*l[i]*r[j];
				(*d)[i] = (*d)[i]+prod; //TODO: implement += in TemplateDomain
				(*d)[j] = (*d)[j]+prod;
			} else {
				if (i<n) (*d)[i] = (*d)[i] + l[i]*r[n];
				else if (j<n) (*d)[j] = (*d)[j] + (l[n]*r[j]);
				else (*d)[n] = l[n]*r[n];
			}
		}
	}

	for (int i=0; i<n; i++) {
		if (type==CONSTANT && !(*d)[i].is_zero()) type=LINEAR;
		if (type==LINEAR && (*d)[i].is_unbounded()) {
			type=NONLINEAR;
			break;
		}
	}

	_coeffs.insert(e, make_pair(d,type));
}

void ExprLinearity::visit(const ExprDiv& e) {
	visit(e.left);
	visit(e.right);

	Array<Domain>& l=*(_coeffs[e.left].first);
	Array<Domain>& r=*(_coeffs[e.right].first);

	nodetype left_type = _coeffs[e.left].second;
	nodetype right_type = _coeffs[e.right].second;

	if (left_type==CONSTANT && right_type==CONSTANT)
		_coeffs.insert(e,build_cst(l[n] / r[n]));
	else {
		Array<Domain>* d=new Array<Domain>(n+1);
		nodetype type=LINEAR; // by default

		for (int i=0; i<n+1; i++) {
			d->set_ref(i,*new Domain(e.dim)); // (-oo,oo) means non-linear

			if (i<n) {
				if (l[i].is_zero())
					if (r[i].is_zero())       // ex: 1/y  (assuming x is the ith var)
						(*d)[i].clear();
					else                      // ex: 1/x
						type=NONLINEAR;
				else if (!l[i].is_unbounded())
					if (right_type==CONSTANT) // ex: x/2
						(*d)[i]=l[i] / r[n];
					else                      // ex: x/y
						type=NONLINEAR;
				else                          // ex: sin(x)/2
						type=NONLINEAR;
			}
		}
		_coeffs.insert(e, make_pair(d,type));
	}
}

void ExprLinearity::binary(const ExprBinaryOp& e, Domain (*fcst)(const Domain&,const Domain&), bool linear_op) {
	visit(e.left);
	visit(e.right);

	Array<Domain>& l=*(_coeffs[e.left].first);
	Array<Domain>& r=*(_coeffs[e.right].first);

	nodetype left_type = _coeffs[e.left].second;
	nodetype right_type = _coeffs[e.right].second;

	if (left_type==CONSTANT && right_type==CONSTANT)
		_coeffs.insert(e,build_cst(fcst(l[n],r[n])));
	else {
		Array<Domain>* d=new Array<Domain>(n+1);

		// a non-linear operation requires each argument to be constant
		// otherwise, the expression is not linear.
		nodetype type=linear_op? CONSTANT : NONLINEAR;

		for (int i=0; i<n+1; i++) {
			if (linear_op) {
				// --- '+' or '-' ------
				// some unbounded components may be "inherited" from l or r.
				// no risk of becoming bounded like with sin((-oo,+oo))=[-1,1]
				d->set_ref(i,*new Domain(fcst(l[i],r[i])));
				if (i<n) {
					if (type==CONSTANT && !(*d)[i].is_zero()) type=LINEAR;
					if (type==LINEAR && (*d)[i].is_unbounded()) type=NONLINEAR;
				}
			} else {
				// --- 'min', 'max' or 'atan2' ----
				// by default: (-oo,oo) means non-linear
				d->set_ref(i,*new Domain(e.dim));

				if (i<n && l[i].is_zero() && r[i].is_zero()) (*d)[i].clear();
			}
		}

		_coeffs.insert(e, make_pair(d,type));
	}
}

void ExprLinearity::unary(const ExprUnaryOp& e, Domain (*fcst)(const Domain&), bool linear_op) {
	visit(e.expr);

	Array<Domain>& expr_d=*(_coeffs[e.expr].first);
	nodetype expr_type=_coeffs[e.expr].second;

	if (expr_type==CONSTANT)
		_coeffs.insert(e,build_cst(fcst(expr_d[n])));
	else {
		Array<Domain>* d=new Array<Domain>(n+1);

		// a non-linear operation requires the argument to be constant
		// otherwise, the expression is not linear.
		nodetype type=linear_op? CONSTANT : NONLINEAR;

		for (int i=0; i<n+1; i++) {
			if (linear_op) {
				// ----- '-', 'transpose' -------
				// possible unbounded domain here.
				d->set_ref(i,*new Domain(fcst(expr_d[i])));
				if (i<n) {
					if (type==CONSTANT && !(*d)[i].is_zero()) type=LINEAR;
					if (type==LINEAR && (*d)[i].is_unbounded()) type=NONLINEAR;
				}
			} else {
				// by default: (-oo,oo) means non-linear
				d->set_ref(i,*new Domain(e.dim));
				if (i<n && expr_d[i].is_zero()) (*d)[i].clear();
			}
		}

		_coeffs.insert(e, make_pair(d,type));
	}
}

void ExprLinearity::visit(const ExprPower& e) {
	visit(e.expr);

	Array<Domain>& expr_d=*(_coeffs[e.expr].first);
	nodetype expr_type=_coeffs[e.expr].second;

	if (expr_type==CONSTANT)
		_coeffs.insert(e,build_cst(pow(expr_d[n],e.expon)));
	else {
		Array<Domain>* d=new Array<Domain>(n+1);

		for (int i=0; i<n+1; i++) {
			// by default: (-oo,oo) means non-linear
			d->set_ref(i,*new Domain(e.dim));

			if (i<n && expr_d[i].is_zero()) (*d)[i].clear();
		}

		_coeffs.insert(e, make_pair(d,NONLINEAR));
	}
}

void ExprLinearity::visit(const ExprChi& e) {
	Array<Domain>* d=new Array<Domain>(n+1);

	visit(e.arg(0));
	visit(e.arg(1));
	visit(e.arg(2));

	Array<Domain>& d_a=*(_coeffs[e.arg(0)].first);
	Array<Domain>& d_b=*(_coeffs[e.arg(1)].first);
	Array<Domain>& d_c=*(_coeffs[e.arg(2)].first);

	for (int i=0; i<n+1; i++) {
		d->set_ref(i,*new Domain(Dim::scalar()));

		if (i<n && d_a[i].is_zero() && d_b[i].is_zero() && d_c[i].is_zero())
			(*d)[i].clear();
	}

	_coeffs.insert(e, make_pair(d,NONLINEAR));
}

void ExprLinearity::visit(const ExprSymbol& e) { assert(false); }
void ExprLinearity::visit(const ExprApply& a)  { assert(false); /* deprecated */}

void ExprLinearity::visit(const ExprGenericBinaryOp& e) { binary(e,e.eval,false); }

void ExprLinearity::visit(const ExprAdd& e)    { binary(e,operator+,true); }
void ExprLinearity::visit(const ExprSub& e)    { binary(e,operator-,true); }

void ExprLinearity::visit(const ExprGenericUnaryOp& e) { unary(e,e.eval,false); }

void ExprLinearity::visit(const ExprMinus& e)  { unary(e,operator-,true); }
void ExprLinearity::visit(const ExprTrans& e)  { unary(e,transpose,true); }

void ExprLinearity::visit(const ExprMax& e)    { binary(e,max,false); }
void ExprLinearity::visit(const ExprMin& e)    { binary(e,min,false); }
void ExprLinearity::visit(const ExprAtan2& e)  { binary(e,atan2,false); }

void ExprLinearity::visit(const ExprAbs& e)    { unary(e,abs,false); }
void ExprLinearity::visit(const ExprSign& e)   { unary(e,sign,false); }
void ExprLinearity::visit(const ExprSqr& e)    { unary(e,sqr,false);}
void ExprLinearity::visit(const ExprSqrt& e)   { unary(e,sqrt,false);}
void ExprLinearity::visit(const ExprExp& e)    { unary(e,exp,false);}
void ExprLinearity::visit(const ExprLog& e)    { unary(e,log,false);}
void ExprLinearity::visit(const ExprCos& e)    { unary(e,cos,false);}
void ExprLinearity::visit(const ExprSin& e)    { unary(e,sin,false);}
void ExprLinearity::visit(const ExprTan& e)    { unary(e,tan,false);}
void ExprLinearity::visit(const ExprCosh& e)   { unary(e,cosh,false);}
void ExprLinearity::visit(const ExprSinh& e)   { unary(e,sinh,false);}
void ExprLinearity::visit(const ExprTanh& e)   { unary(e,tanh,false);}
void ExprLinearity::visit(const ExprAcos& e)   { unary(e,acos,false);}
void ExprLinearity::visit(const ExprAsin& e)   { unary(e,asin,false);}
void ExprLinearity::visit(const ExprAtan& e)   { unary(e,atan,false);}
void ExprLinearity::visit(const ExprAcosh& e)  { unary(e,acosh,false);}
void ExprLinearity::visit(const ExprAsinh& e)  { unary(e,asinh,false);}
void ExprLinearity::visit(const ExprAtanh& e)  { unary(e,atanh,false);}

} /* namespace ibex */
