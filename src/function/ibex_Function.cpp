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

	if (df!=NULL) delete df;

	free((char*) name);
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

const ExprApply& Function::operator()(const ExprNode& arg1) const {
	const ExprNode* args[1] = { &arg1 };
	return (*this)(args);
}
const ExprApply& Function::operator()(const ExprNode& arg1, const ExprNode& arg2) const {
	const ExprNode* args[2] = { &arg1, &arg2 };
	return (*this)(args);
}
const ExprApply& Function::operator()(const ExprNode& arg1, const ExprNode& arg2, const ExprNode& arg3) const {
	const ExprNode* args[3] = { &arg1, &arg2, &arg3 };
	return (*this)(args);
}
const ExprApply& Function::operator()(const ExprNode& arg1, const ExprNode& arg2, const ExprNode& arg3, const ExprNode& arg4) const {
	const ExprNode* args[4] = { &arg1, &arg2, &arg3, &arg4 };
	return (*this)(args);
}
const ExprApply& Function::operator()(const ExprNode& arg1, const ExprNode& arg2, const ExprNode& arg3, const ExprNode& arg4, const ExprNode& arg5) const {
	const ExprNode* args[5] = { &arg1, &arg2, &arg3, &arg4, &arg5 };
	return (*this)(args);
}
const ExprApply& Function::operator()(const ExprNode& arg1, const ExprNode& arg2, const ExprNode& arg3, const ExprNode& arg4, const ExprNode& arg5, const ExprNode& arg6) const {
	const ExprNode* args[6] = { &arg1, &arg2, &arg3, &arg4, &arg5, &arg6 };
	return (*this)(args);
}
const ExprApply& Function::operator()(const ExprNode& arg1, const ExprNode& arg2, const ExprNode& arg3, const ExprNode& arg4, const ExprNode& arg5, const ExprNode& arg6, const ExprNode& arg7) const {
	const ExprNode* args[7] = { &arg1, &arg2, &arg3, &arg4, &arg5, &arg6, &arg7 };
	return (*this)(args);
}
const ExprApply& Function::operator()(const ExprNode& arg1, const ExprNode& arg2, const ExprNode& arg3, const ExprNode& arg4, const ExprNode& arg5, const ExprNode& arg6, const ExprNode& arg7, const ExprNode& arg8) const {
	const ExprNode* args[8] = { &arg1, &arg2, &arg3, &arg4, &arg5, &arg6, &arg7, &arg8 };
	return (*this)(args);
}
const ExprApply& Function::operator()(const ExprNode& arg1, const ExprNode& arg2, const ExprNode& arg3, const ExprNode& arg4, const ExprNode& arg5, const ExprNode& arg6, const ExprNode& arg7, const ExprNode& arg8, const ExprNode& arg9) const {
	const ExprNode* args[9] = { &arg1, &arg2, &arg3, &arg4, &arg5, &arg6, &arg7, &arg8, &arg9 };
	return (*this)(args);
}
const ExprApply& Function::operator()(const ExprNode& arg1, const ExprNode& arg2, const ExprNode& arg3, const ExprNode& arg4, const ExprNode& arg5, const ExprNode& arg6, const ExprNode& arg7, const ExprNode& arg8, const ExprNode& arg9, const ExprNode& arg10) const {
	const ExprNode* args[10] = { &arg1, &arg2, &arg3, &arg4, &arg5, &arg6, &arg7, &arg8, &arg9, &arg10};
	return (*this)(args);
}
const ExprApply& Function::operator()(const ExprNode& arg1, const ExprNode& arg2, const ExprNode& arg3, const ExprNode& arg4, const ExprNode& arg5, const ExprNode& arg6, const ExprNode& arg7, const ExprNode& arg8, const ExprNode& arg9, const ExprNode& arg10, const ExprNode& arg11) const {
	const ExprNode* args[11] = { &arg1, &arg2, &arg3, &arg4, &arg5, &arg6, &arg7, &arg8, &arg9, &arg10, &arg11};
	return (*this)(args);
}
const ExprApply& Function::operator()(const ExprNode& arg1, const ExprNode& arg2, const ExprNode& arg3, const ExprNode& arg4, const ExprNode& arg5, const ExprNode& arg6, const ExprNode& arg7, const ExprNode& arg8, const ExprNode& arg9, const ExprNode& arg10, const ExprNode& arg11, const ExprNode& arg12) const {
	const ExprNode* args[12] = { &arg1, &arg2, &arg3, &arg4, &arg5, &arg6, &arg7, &arg8, &arg9, &arg10, &arg11, &arg12 };
	return (*this)(args);
}
const ExprApply& Function::operator()(const ExprNode& arg1, const ExprNode& arg2, const ExprNode& arg3, const ExprNode& arg4, const ExprNode& arg5, const ExprNode& arg6, const ExprNode& arg7, const ExprNode& arg8, const ExprNode& arg9, const ExprNode& arg10, const ExprNode& arg11, const ExprNode& arg12, const ExprNode& arg13) const {
	const ExprNode* args[13] = { &arg1, &arg2, &arg3, &arg4, &arg5, &arg6, &arg7, &arg8, &arg9, &arg10, &arg11, &arg12, &arg13 };
	return (*this)(args);
}
const ExprApply& Function::operator()(const ExprNode& arg1, const ExprNode& arg2, const ExprNode& arg3, const ExprNode& arg4, const ExprNode& arg5, const ExprNode& arg6, const ExprNode& arg7, const ExprNode& arg8, const ExprNode& arg9, const ExprNode& arg10, const ExprNode& arg11, const ExprNode& arg12, const ExprNode& arg13, const ExprNode& arg14) const {
	const ExprNode* args[14] = { &arg1, &arg2, &arg3, &arg4, &arg5, &arg6, &arg7, &arg8, &arg9, &arg10, &arg11, &arg12, &arg13, &arg14 };
	return (*this)(args);
}
const ExprApply& Function::operator()(const ExprNode& arg1, const ExprNode& arg2, const ExprNode& arg3, const ExprNode& arg4, const ExprNode& arg5, const ExprNode& arg6, const ExprNode& arg7, const ExprNode& arg8, const ExprNode& arg9, const ExprNode& arg10, const ExprNode& arg11, const ExprNode& arg12, const ExprNode& arg13, const ExprNode& arg14, const ExprNode& arg15) const {
	const ExprNode* args[15] = { &arg1, &arg2, &arg3, &arg4, &arg5, &arg6, &arg7, &arg8, &arg9, &arg10, &arg11, &arg12, &arg13, &arg14, &arg15 };
	return (*this)(args);
}
const ExprApply& Function::operator()(const ExprNode& arg1, const ExprNode& arg2, const ExprNode& arg3, const ExprNode& arg4, const ExprNode& arg5, const ExprNode& arg6, const ExprNode& arg7, const ExprNode& arg8, const ExprNode& arg9, const ExprNode& arg10, const ExprNode& arg11, const ExprNode& arg12, const ExprNode& arg13, const ExprNode& arg14, const ExprNode& arg15, const ExprNode& arg16) const {
	const ExprNode* args[16] = { &arg1, &arg2, &arg3, &arg4, &arg5, &arg6, &arg7, &arg8, &arg9, &arg10, &arg11, &arg12, &arg13, &arg14, &arg15, &arg16 };
	return (*this)(args);
}
const ExprApply& Function::operator()(const ExprNode& arg1, const ExprNode& arg2, const ExprNode& arg3, const ExprNode& arg4, const ExprNode& arg5, const ExprNode& arg6, const ExprNode& arg7, const ExprNode& arg8, const ExprNode& arg9, const ExprNode& arg10, const ExprNode& arg11, const ExprNode& arg12, const ExprNode& arg13, const ExprNode& arg14, const ExprNode& arg15, const ExprNode& arg16, const ExprNode& arg17) const {
	const ExprNode* args[17] = { &arg1, &arg2, &arg3, &arg4, &arg5, &arg6, &arg7, &arg8, &arg9, &arg10, &arg11, &arg12, &arg13, &arg14, &arg15, &arg16, &arg17 };
	return (*this)(args);
}
const ExprApply& Function::operator()(const ExprNode& arg1, const ExprNode& arg2, const ExprNode& arg3, const ExprNode& arg4, const ExprNode& arg5, const ExprNode& arg6, const ExprNode& arg7, const ExprNode& arg8, const ExprNode& arg9, const ExprNode& arg10, const ExprNode& arg11, const ExprNode& arg12, const ExprNode& arg13, const ExprNode& arg14, const ExprNode& arg15, const ExprNode& arg16, const ExprNode& arg17, const ExprNode& arg18) const {
	const ExprNode* args[18] = { &arg1, &arg2, &arg3, &arg4, &arg5, &arg6, &arg7, &arg8, &arg9, &arg10, &arg11, &arg12, &arg13, &arg14, &arg15, &arg16, &arg17, &arg18 };
	return (*this)(args);
}
const ExprApply& Function::operator()(const ExprNode& arg1, const ExprNode& arg2, const ExprNode& arg3, const ExprNode& arg4, const ExprNode& arg5, const ExprNode& arg6, const ExprNode& arg7, const ExprNode& arg8, const ExprNode& arg9, const ExprNode& arg10, const ExprNode& arg11, const ExprNode& arg12, const ExprNode& arg13, const ExprNode& arg14, const ExprNode& arg15, const ExprNode& arg16, const ExprNode& arg17, const ExprNode& arg18, const ExprNode& arg19) const {
	const ExprNode* args[19] = { &arg1, &arg2, &arg3, &arg4, &arg5, &arg6, &arg7, &arg8, &arg9, &arg10, &arg11, &arg12, &arg13, &arg14, &arg15, &arg16, &arg17, &arg18, &arg19 };
	return (*this)(args);
}
const ExprApply& Function::operator()(const ExprNode& arg1, const ExprNode& arg2, const ExprNode& arg3, const ExprNode& arg4, const ExprNode& arg5, const ExprNode& arg6, const ExprNode& arg7, const ExprNode& arg8, const ExprNode& arg9, const ExprNode& arg10, const ExprNode& arg11, const ExprNode& arg12, const ExprNode& arg13, const ExprNode& arg14, const ExprNode& arg15, const ExprNode& arg16, const ExprNode& arg17, const ExprNode& arg18, const ExprNode& arg19, const ExprNode& arg20) const {
	const ExprNode* args[20] = { &arg1, &arg2, &arg3, &arg4, &arg5, &arg6, &arg7, &arg8, &arg9, &arg10, &arg11, &arg12, &arg13, &arg14, &arg15, &arg16, &arg17, &arg18, &arg19, &arg20 };
	return (*this)(args);
}


const ExprApply& Function::operator()(const ExprNode** args) const {
	return ExprApply::new_(*this, args);
}

const ExprApply& Function::operator()(const vector<const ExprNode*>& arg) const {
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

//const ExprApply& Function::operator()(const ExprNode& arg0, const Interval& arg1)       { return (*this)(arg0,_I(0,1)); }
} // namespace ibex
