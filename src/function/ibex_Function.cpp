/* =========é===================================================================
 * I B E X - Functions
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Jan 5, 2012
 * ---------------------------------------------------------------------------- */

#include "ibex_Function.h"
#include "ibex_Expr.h"
#include "ibex_Eval.h"
#include "ibex_Affine2Eval.h"
#include "ibex_HC4Revise.h"
#include "ibex_InHC4Revise.h"
#include "ibex_Gradient.h"
#include "ibex_FunctionBuild.cpp_"

using namespace std;

namespace ibex {

Function::~Function() {

	if (root!=NULL) {

		/* warning... if there is only one constraint
		 * then comp is the same object as f itself!
		 *
		 * This is not a very consistent choice...
		 */
		if (!expr().dim.is_scalar()) delete[] comp;

		for (unsigned int i=0; i<exprnodes.size(); i++) {
			delete node(i).deco.d;
			delete node(i).deco.g;
			delete node(i).deco.p;
			delete node(i).deco.af2;
		}
		cleanup(expr(),false);

		for (int i=0; i<nb_arg(); i++)
			delete &arg(i);

		delete[] used_var;
	}

	free((char*) name);
}

int Function::nb_var() const {
	return is_used.size();
	/*
	int sum=0;
	for (int i=0; i<nb_symbols(); i++)
		sum+=symbol(i).dim.size();
	return sum;
	*/
}

Domain& Function::eval_domain(const IntervalVector& box) const {
	return Eval().eval(*this,box);
}


Domain& Function::eval_affine2_domain(const IntervalVector& box) const {
	return Affine2Eval().eval(*this,box);
}

Domain& Function::eval_affine2_domain(const IntervalVector& box, Affine2Domain& affine) const {
	ExprLabel res = Affine2Eval().eval_label(*this,box);
	affine = *res.af2;
	return *res.d;
}

Interval Function::eval_affine2(const IntervalVector& box) const {
	return eval_affine2_domain(box).i();
}

Interval Function::eval_affine2(const IntervalVector& box, Affine2& affine) const {
	ExprLabel res = Affine2Eval().eval_label(*this,box);
	affine = res.af2->i();
	return res.d->i();
}

IntervalVector Function::eval_affine2_vector(const IntervalVector& box) const {
	return expr().dim.is_scalar() ? IntervalVector(1,Eval().eval(*this,box).i()) : Eval().eval(*this,box).v();
}

IntervalVector Function::eval_affine2_vector(const IntervalVector& box, Affine2Vector& affine) const {
	ExprLabel res = Affine2Eval().eval_label(*this,box);
	if (expr().dim.is_scalar() ) {
		affine = Affine2Vector(1,res.af2->i());
		return IntervalVector(1,res.d->i());
	} else {
		affine = res.af2->v();
		return res.d->v();
	}
}

IntervalMatrix Function::eval_affine2_matrix(const IntervalVector& box, Affine2Matrix& affine) const {
	ExprLabel res = Affine2Eval().eval_label(*this,box);
	affine = Affine2Matrix(expr().dim.dim2, expr().dim.dim3);

	switch (expr().dim.type()) {
	case Dim::SCALAR     : {
		affine[0][0] = res.af2->i();
		return IntervalMatrix(1,1,res.d->i());
	}
	case Dim::ROW_VECTOR : {
		affine.set_row(0,res.af2->v());
		IntervalMatrix M(image_dim(),1);
		M.set_row(0,res.d->v());
		return M;
	}
	case Dim::COL_VECTOR : {
		affine.set_col(0,res.af2->v());
		IntervalMatrix M(1,image_dim());
		M.set_col(0,res.d->v());
		return M;
	}
	case Dim::MATRIX: {
		affine = res.af2->m();
		return res.d->m();
	}
	default : {
		assert(false);
	}
	}
}

void Function::backward(const Domain& y, IntervalVector& x) const {
	HC4Revise().proj(*this,y,x);
}

void Function::iproj(const Domain& y, IntervalVector& x) const {
	InHC4Revise().iproj(*this,y,x);
}

void Function::iproj(const Domain& y, IntervalVector& x, const IntervalVector& xin) const {
	InHC4Revise().iproj(*this,y,x,xin);
}

void Function::gradient(const IntervalVector& x, IntervalVector& g) const {
	assert(g.size()==nb_var());
	assert(x.size()==nb_var());
	Gradient().gradient(*this,x,g);
//	if (!df) ((Function*) this)->df=new Function(*this,DIFF);
//	g=df->eval_vector(x);
}

void Function::jacobian(const IntervalVector& x, IntervalMatrix& J) const {
	assert(J.nb_cols()==nb_var());
	assert(x.size()==nb_var());
	assert(J.nb_rows()==image_dim());
	assert(expr().deco.d);
	assert(expr().deco.g);

	// calculate the gradient of each component of f
	for (int i=0; i<image_dim(); i++) {
		(*this)[i].gradient(x,J[i]);
	}
}

void Function::hansen_matrix(const IntervalVector& box, IntervalMatrix& H) const {
	int n=nb_var();
	int m=expr().dim.vec_size();

	assert(H.nb_cols()==n);
	assert(box.size()==n);
	assert(expr().dim.is_vector());
	assert(H.nb_rows()==m);

	IntervalVector x=box.mid();
	IntervalMatrix J(m,n);

	// test!
//	int tab[box.size()];
//	box.sort_indices(false,tab);
//	int var;

	for (int var=0; var<n; var++) {
		//var=tab[i];
		x[var]=box[var];
		jacobian(x,J);
		H.set_col(var,J.col(var));
	}

}

std::ostream& operator<<(std::ostream& os, const Function& f) {
	if (f.name!=NULL) os << f.name << ":";
	os << "(";
	for (int i=0; i<f.nb_arg(); i++) {
		os << f.arg_name(i);
		if (i<f.nb_arg()-1) os << ",";
	}
	os << ")->" << f.expr();
	return os;
}

const ExprApply& Function::operator()(const ExprNode& arg1) {
	const ExprNode* args[1] = { &arg1 };
	return (*this)(args);
}

const ExprApply& Function::operator()(const ExprNode& arg1, const ExprNode& arg2) {
	const ExprNode* args[2] = { &arg1, &arg2 };
	return (*this)(args);
}

const ExprApply& Function::operator()(const ExprNode& arg1, const ExprNode& arg2, const ExprNode& arg3) {
	const ExprNode* args[3] = { &arg1, &arg2, &arg3 };
	return (*this)(args);
}

const ExprApply& Function::operator()(const ExprNode& arg1, const ExprNode& arg2, const ExprNode& arg3, const ExprNode& arg4) {
	const ExprNode* args[4] = { &arg1, &arg2, &arg3, &arg4 };
	return (*this)(args);
}

const ExprApply& Function::operator()(const ExprNode** args) {
	return ExprApply::new_(*this, args);
}

const ExprApply& Function::operator()(const vector<const ExprNode*>& arg) {
	int n=arg.size();
	assert(nb_arg()==n);
	const ExprNode** tmp=new const ExprNode*[n];
	for (int i=0; i<n; i++) tmp[i]=arg[i];
	const ExprApply* a=&ExprApply::new_(*this, tmp);
	delete[] tmp;
	return *a;
}

#define CONCAT(a,b)         CONCAT_HIDDEN(a,b)
#define CONCAT_HIDDEN(a,b)  a ## b
#define _I(ref,i) ExprConstant::new_scalar(CONCAT(arg,i))
#define _V(ref,i) ExprConstant::new_vector(CONCAT(arg,i), arg(i).type()==Dim::ROW_VECTOR)
#define _M(ref,i) ExprConstant::new_matrix(CONCAT(arg,i))

const ExprApply& Function::operator()(const ExprNode& arg0, const Interval& arg1)       { return (*this)(arg0,_I(0,1)); }
const ExprApply& Function::operator()(const ExprNode& arg0, const IntervalVector& arg1) { return (*this)(arg0,_V(0,1)); }
const ExprApply& Function::operator()(const ExprNode& arg0, const IntervalMatrix& arg1) { return (*this)(arg0,_M(0,1)); }
const ExprApply& Function::operator()(const Interval& arg0, const ExprNode& arg1)       { return (*this)(_I(1,0),arg1); }
const ExprApply& Function::operator()(const IntervalVector& arg0, const ExprNode& arg1) { return (*this)(_V(1,0),arg1); }
const ExprApply& Function::operator()(const IntervalMatrix& arg0, const ExprNode& arg1) { return (*this)(_M(1,0),arg1); }

const ExprApply& Function::operator()(const ExprNode& arg0, const ExprNode& arg1, const Interval& arg2)       { return (*this)(arg0, arg1, _I(0,2)); }
const ExprApply& Function::operator()(const ExprNode& arg0, const ExprNode& arg1, const IntervalVector& arg2) { return (*this)(arg0, arg1, _V(0,2)); }
const ExprApply& Function::operator()(const ExprNode& arg0, const ExprNode& arg1, const IntervalMatrix& arg2) { return (*this)(arg0, arg1, _M(0,2)); }

const ExprApply& Function::operator()(const ExprNode& arg0, const Interval& arg1, const ExprNode& arg2)             { return (*this)(arg0, _I(0,1), arg2); }
const ExprApply& Function::operator()(const ExprNode& arg0, const Interval& arg1, const Interval& arg2)             { return (*this)(arg0, _I(0,1), _I(0,2)); }
const ExprApply& Function::operator()(const ExprNode& arg0, const Interval& arg1, const IntervalVector& arg2)       { return (*this)(arg0, _I(0,1), _V(0,2)); }
const ExprApply& Function::operator()(const ExprNode& arg0, const Interval& arg1, const IntervalMatrix& arg2)       { return (*this)(arg0, _I(0,1), _M(0,2)); }
const ExprApply& Function::operator()(const ExprNode& arg0, const IntervalVector& arg1, const ExprNode& arg2)       { return (*this)(arg0, _V(0,1), arg2); }
const ExprApply& Function::operator()(const ExprNode& arg0, const IntervalVector& arg1, const Interval& arg2)       { return (*this)(arg0, _V(0,1), _I(0,2)); }
const ExprApply& Function::operator()(const ExprNode& arg0, const IntervalVector& arg1, const IntervalVector& arg2) { return (*this)(arg0, _V(0,1), _V(0,2)); }
const ExprApply& Function::operator()(const ExprNode& arg0, const IntervalVector& arg1, const IntervalMatrix& arg2) { return (*this)(arg0, _V(0,1), _M(0,2)); }
const ExprApply& Function::operator()(const ExprNode& arg0, const IntervalMatrix& arg1, const ExprNode& arg2)       { return (*this)(arg0, _M(0,1), arg2); }
const ExprApply& Function::operator()(const ExprNode& arg0, const IntervalMatrix& arg1, const Interval& arg2)       { return (*this)(arg0, _M(0,1), _I(0,2)); }
const ExprApply& Function::operator()(const ExprNode& arg0, const IntervalMatrix& arg1, const IntervalVector& arg2) { return (*this)(arg0, _M(0,1), _V(0,2)); }
const ExprApply& Function::operator()(const ExprNode& arg0, const IntervalMatrix& arg1, const IntervalMatrix& arg2) { return (*this)(arg0, _M(0,1), _M(0,2)); }
const ExprApply& Function::operator()(const Interval& arg0, const ExprNode& arg1, const ExprNode& arg2)             { return (*this)(_I(1,0), arg1, arg2); }
const ExprApply& Function::operator()(const Interval& arg0, const ExprNode& arg1, const Interval& arg2)             { return (*this)(_I(1,0), arg1, _I(1,2)); }
const ExprApply& Function::operator()(const Interval& arg0, const ExprNode& arg1, const IntervalVector& arg2)       { return (*this)(_I(1,0), arg1, _V(1,2)); }
const ExprApply& Function::operator()(const Interval& arg0, const ExprNode& arg1, const IntervalMatrix& arg2)       { return (*this)(_I(1,0), arg1, _M(1,2)); }
const ExprApply& Function::operator()(const IntervalVector& arg0, const ExprNode& arg1, const ExprNode& arg2)       { return (*this)(_V(1,0), arg1, arg2); }
const ExprApply& Function::operator()(const IntervalVector& arg0, const ExprNode& arg1, const Interval& arg2)       { return (*this)(_V(1,0), arg1, _I(1,2)); }
const ExprApply& Function::operator()(const IntervalVector& arg0, const ExprNode& arg1, const IntervalVector& arg2) { return (*this)(_V(1,0), arg1, _V(1,2)); }
const ExprApply& Function::operator()(const IntervalVector& arg0, const ExprNode& arg1, const IntervalMatrix& arg2) { return (*this)(_V(1,0), arg1, _M(1,2)); }
const ExprApply& Function::operator()(const IntervalMatrix& arg0, const ExprNode& arg1, const ExprNode& arg2)       { return (*this)(_M(1,0), arg1, arg2); }
const ExprApply& Function::operator()(const IntervalMatrix& arg0, const ExprNode& arg1, const Interval& arg2)       { return (*this)(_M(1,0), arg1, _I(1,2)); }
const ExprApply& Function::operator()(const IntervalMatrix& arg0, const ExprNode& arg1, const IntervalVector& arg2) { return (*this)(_M(1,0), arg1, _V(1,2)); }
const ExprApply& Function::operator()(const IntervalMatrix& arg0, const ExprNode& arg1, const IntervalMatrix& arg2) { return (*this)(_M(1,0), arg1, _M(1,2)); }
const ExprApply& Function::operator()(const Interval& arg0, const Interval& arg1, const ExprNode& arg2)             { return (*this)(_I(2,0), _I(2,1), arg2); }
const ExprApply& Function::operator()(const Interval& arg0, const IntervalVector& arg1, const ExprNode& arg2)       { return (*this)(_I(2,0), _V(2,1), arg2); }
const ExprApply& Function::operator()(const Interval& arg0, const IntervalMatrix& arg1, const ExprNode& arg2)       { return (*this)(_I(2,0), _M(2,1), arg2); }
const ExprApply& Function::operator()(const IntervalVector& arg0, const Interval& arg1, const ExprNode& arg2)       { return (*this)(_V(2,0), _I(2,1), arg2); }
const ExprApply& Function::operator()(const IntervalVector& arg0, const IntervalVector& arg1, const ExprNode& arg2) { return (*this)(_V(2,0), _V(2,1), arg2); }
const ExprApply& Function::operator()(const IntervalVector& arg0, const IntervalMatrix& arg1, const ExprNode& arg2) { return (*this)(_V(2,0), _M(2,1), arg2); }
const ExprApply& Function::operator()(const IntervalMatrix& arg0, const Interval& arg1, const ExprNode& arg2)       { return (*this)(_M(2,0), _I(2,1), arg2); }
const ExprApply& Function::operator()(const IntervalMatrix& arg0, const IntervalVector& arg1, const ExprNode& arg2) { return (*this)(_M(2,0), _V(2,1), arg2); }
const ExprApply& Function::operator()(const IntervalMatrix& arg0, const IntervalMatrix& arg1, const ExprNode& arg2) { return (*this)(_M(2,0), _M(2,1), arg2); }
} // namespace ibex
