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

#define COEFFS pair<Array<Domain>*,bool>

ExprLinearity::ExprLinearity(const Array<const ExprSymbol> x, const ExprNode& y) :
		n(0) {

	for (int i=0; i<x.size(); i++)
		n+=x[i].dim.size();

	for (int i=0; i<x.size(); i++)
		visit(x[i],i); // requires n to be known

	visit(y);
}

ExprLinearity::~ExprLinearity() {
	for (IBEX_NODE_MAP(COEFFS)::const_iterator it=_coeffs.begin(); it!=_coeffs.end(); it++) {
		Array<Domain>& d=*(it->second.first);

		for (int i=0; i<n+1; i++) {
			delete &d[i];
		}
		delete it->second.first;
	}
}

IntervalVector ExprLinearity::coeffs(const ExprNode& e) const {
	IntervalVector c(n+1);
//	cout << "n="<< n << endl;
//	for (int i=0; i<_coeffs[e].first->size(); i++) {
//		cout << (*_coeffs[e].first)[i].dim << endl;
//	}
//	cout << "==========\n";
	if (!_coeffs.found(e))
		c.set_empty();
	else
		load(c,*(_coeffs[e].first));
	return c;
}

Array<Domain>* ExprLinearity::build_zero(const Dim& dim) const {
	Array<Domain>* d=new Array<Domain>(n+1);

	for (int i=0; i<n+1; i++) {
		d->set_ref(i,*new Domain(dim));
		(*d)[i].clear();
	}
	return d;
}

pair<Array<Domain>*, bool> ExprLinearity::build_cst(const Domain& value) const {
	Array<Domain>* d=build_zero(value.dim);
	(*d)[n]=value;
	return make_pair(d,true);
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

	_coeffs.insert(e, make_pair(d,false));
}

void ExprLinearity::visit(const ExprNode& e) {
	if (!_coeffs.found(e))
		e.acceptVisitor(*this);
}

void ExprLinearity::visit(const ExprIndex& e) {
	visit(e.expr);

	if (!_coeffs.found(e)) return;

	Array<Domain>& c=*(_coeffs[e.expr].first);

	Array<Domain>* d=new Array<Domain>(n+1);

	for (int i=0; i<n+1; i++) {
		d->set_ref(i,*new Domain(c[i][e.index]));
	}

	_coeffs.insert(e, make_pair(d,_coeffs[e.expr].second));
}

void ExprLinearity::visit(const ExprConstant& e) {
	_coeffs.insert(e, build_cst(e.get()));
}

void ExprLinearity::visit(const ExprVector& e) {

	bool constant=true;
	bool linear=true;

	for (int j=0; j<e.length(); j++) {
		visit(e.arg(j));

		if (!_coeffs.found(e.arg(j))) {
			// don't exit the loop now, to allow subsequent subexpressions to be detected as linear
			linear = false;
		}
		constant &= _coeffs[e.arg(j)].second;
	}
	if (!linear) return;

	Array<Domain>* d=new Array<Domain>(n+1);

	for (int i=0; i<n+1; i++) {
		Array<const Domain> array(e.length());

		for (int j=0; j<e.length(); j++) {

			Array<Domain>& argj=*(_coeffs[e.arg(j)].first);

			array.set_ref(j,argj[i]);
		}

		d->set_ref(i,*new Domain(array, e.row_vector()));
	}

	_coeffs.insert(e, make_pair(d,constant));
}


void ExprLinearity::visit(const ExprMul& e) {

	bool linear=true;

	visit(e.left);
	linear &= _coeffs.found(e.left);

	visit(e.right);
	linear &= _coeffs.found(e.right);

	if (!linear) return;

	bool left_constant = _coeffs[e.left].second;
	bool right_constant = _coeffs[e.right].second;

	if (!left_constant && !right_constant) {
		return;
	}

	Array<Domain>& l=*(_coeffs[e.left].first);
	Array<Domain>& r=*(_coeffs[e.right].first);

	Array<Domain>* d=new Array<Domain>(n+1);

	for (int i=0; i<n+1; i++) {
		if (left_constant)
			d->set_ref(i,*new Domain(l[n] * r[i]));
		else
			d->set_ref(i,*new Domain(l[i] * r[n]));
	}

	_coeffs.insert(e, make_pair(d,left_constant && right_constant));
}


void ExprLinearity::visit(const ExprDiv& e) {
	bool linear=true;

	visit(e.left);
	linear &= _coeffs.found(e.left);

	visit(e.right);
	linear &= _coeffs.found(e.right);

	if (!linear) return;

	if (!_coeffs[e.right].second) { // the dividend must be constant
		return;
	}

	bool constant=_coeffs[e.left].second;

	Array<Domain>& l=*(_coeffs[e.left].first);
	Array<Domain>& r=*(_coeffs[e.right].first);

	if (constant)
		_coeffs.insert(e,build_cst(l[n] / r[n]));
	else {
		Array<Domain>* d=new Array<Domain>(n+1);

		for (int i=0; i<n+1; i++) {
			d->set_ref(i,*new Domain(l[i] / r[n]));
		}

		_coeffs.insert(e, make_pair(d,false));
	}
}

void ExprLinearity::binary(const ExprBinaryOp& e, Domain (*fcst)(const Domain&,const Domain&), bool linear_op) {

	bool linear=true;

	visit(e.left);
	linear &= _coeffs.found(e.left);

	visit(e.right);
	linear &= _coeffs.found(e.right);

	if (!linear) return;

	bool constant=_coeffs[e.left].second && _coeffs[e.right].second;

	// a non-linear operation requires each argument to be constant
	// otherwise, the expression is not linear.
	if (!linear_op && !constant) return;

	Array<Domain>& l=*(_coeffs[e.left].first);
	Array<Domain>& r=*(_coeffs[e.right].first);

	if (constant)
		_coeffs.insert(e,build_cst(fcst(l[n],r[n])));
	else {
		Array<Domain>* d=new Array<Domain>(n+1);

		for (int i=0; i<n+1; i++) {
			d->set_ref(i,*new Domain(fcst(l[i],r[i])));
		}

		_coeffs.insert(e, make_pair(d,false));
	}
}

void ExprLinearity::unary(const ExprUnaryOp& e, Domain (*fcst)(const Domain&), bool linear_op) {
	visit(e.expr);

	if (!_coeffs.found(e.expr)) return;

	bool constant=_coeffs[e.expr].second;

	if (!linear_op && !constant) return;

	Array<Domain>& c=*(_coeffs[e.expr].first);

	if (constant)
		_coeffs.insert(e,build_cst(fcst(c[n])));
	else {
		Array<Domain>* d=new Array<Domain>(n+1);

		for (int i=0; i<n+1; i++) {
			d->set_ref(i,*new Domain(fcst(c[i])));
		}

		_coeffs.insert(e, make_pair(d,false));
	}
}

void ExprLinearity::visit(const ExprPower& e){
	visit(e.expr);

	if (!_coeffs.found(e.expr)) return;

	bool constant=_coeffs[e.expr].second;

	if (!constant) return;

	Array<Domain>& c=*(_coeffs[e.expr].first);

	_coeffs.insert(e,build_cst(pow(c[n],e.expon)));
}

void ExprLinearity::visit(const ExprSymbol& e) { assert(false); }
void ExprLinearity::visit(const ExprApply& a)  { assert(false); /* deprecated */}
void ExprLinearity::visit(const ExprChi& a)    { /* not linear */ }

void ExprLinearity::visit(const ExprAdd& e)    { binary(e,operator+,true); }
void ExprLinearity::visit(const ExprSub& e)    { binary(e,operator-,true); }

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
