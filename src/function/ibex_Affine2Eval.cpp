/*
 * ibex_Affine2Eval.cpp
 *
 *  Created on: 7 mars 2013
 *      Author: nininjo
 */



#include "ibex_Affine2Eval.h"
#include "ibex_Eval.h"
//#include <stdio.h>

namespace ibex {


//======================== Forward =======================


Affine2Domain& Affine2Eval::eval_affine2(const Function& f, ExprLabel** args) const {
	assert(f.expr().deco.af2);

	Array<const Affine2Domain> argD(f.nb_arg());

	for (int i=0; i<f.nb_arg(); i++) {
		argD.set_ref(i,*(args[i]->af2));
	}

	load(f.arg_af2,argD,f.nb_used_vars,f.used_var);

	//------------- for debug
//	std::cout << "Function " << f.name << ", domains before eval:" << std::endl;
//		for (int i=0; i<f.nb_arg(); i++) {
//			std::cout << "arg[" << i << "]=" << f.arg_domains[i] << std::endl;
//		}

	return *f.forward<Affine2Eval>(*this).af2;
}


Affine2Domain& Affine2Eval::eval_affine2(const Function& f, const Array<const Affine2Domain>& d) const {
	assert(f.expr().deco.af2);

	load(f.arg_af2,d,f.nb_used_vars,f.used_var);

	return *f.forward<Affine2Eval>(*this).af2;
}

Affine2Domain& Affine2Eval::eval_affine2(const Function& f, const Array<Affine2Domain>& d) const {
	assert(f.expr().deco.af2);

	load(f.arg_af2,d,f.nb_used_vars,f.used_var);

	return *f.forward<Affine2Eval>(*this).af2;
}


Affine2Domain& Affine2Eval::eval_affine2(const Function& f, const IntervalVector& box) const {
	assert(f.expr().deco.af2);

	if (f.all_args_scalar()) {
		int j;
		for (int i=0; i<f.nb_used_vars; i++) {
			j=f.used_var[i];
			f.arg_af2[j].i()= Affine2(f.nb_var(),j+1,box[j]);
		}

	}
	else {
		load(f.arg_af2,Affine2Vector(box),f.nb_used_vars,f.used_var); // load the domains of all the symbols
	}

	return *f.forward<Affine2Eval>(*this).af2;

}


void Affine2Eval::vector_fwd(const ExprVector& v, const ExprLabel** compL, ExprLabel& y) {

	assert(v.type()!=Dim::SCALAR);
	assert(v.type()!=Dim::MATRIX_ARRAY);

	if (v.dim.is_vector()) {
		for (int i=0; i<v.length(); i++) y.af2->v()[i]=compL[i]->af2->i();
	}
	else {
		if (v.row_vector())
			for (int i=0; i<v.length(); i++) y.af2->m().set_col(i,compL[i]->af2->v());
		else
			for (int i=0; i<v.length(); i++) y.af2->m().set_row(i,compL[i]->af2->v());
	}
}


// ====================  backward =============

const double Affine2Eval::RATIO = 0.1;

void Affine2Eval::proj_affine2(const Function& f, const Affine2Domain& y, Array<Affine2Domain>& x) {
	Affine2Eval().eval_affine2(f,x);
	*f.expr().deco.af2 = *f.expr().deco.af2 & y ;// &= between 2 Affine2Domain does not exist
	f.backward<Affine2Eval>(*this);
	// note: not very clean.
	// the box x is not emptied if an EmptyBoxException is thrown
	// before (this is done by the contractor).
	load(x,f.arg_af2,f.nb_used_vars,f.used_var);
}

void Affine2Eval::proj_affine2(const Function& f, const Affine2Domain& y, IntervalVector& x) {
	Affine2Eval().eval_affine2(f,x);
	*f.expr().deco.af2 =  *f.expr().deco.af2 & y ;// &= between 2 Affine2Domain does not exist
	f.backward<Affine2Eval>(*this);

	if (f.all_args_scalar()) {
		int j;
		for (int i=0; i<f.nb_used_vars; i++) {
			j=f.used_var[i];
			x[j]=f.arg_af2[j].i();
		}
	}
	else
		load(x,f.arg_af2,f.nb_used_vars,f.used_var);
}

void Affine2Eval::proj_affine2(const Function& f, const Affine2Domain& y, ExprLabel** x) {
	Affine2Eval().eval_affine2(f,x);
	*f.expr().deco.af2 =  *f.expr().deco.af2 & y ;// &= between 2 Affine2Domain does not exist
	f.backward<Affine2Eval>(*this);

	Array<Affine2Domain> argD(f.nb_arg());

	for (int i=0; i<f.nb_arg(); i++) {
		argD.set_ref(i,*(x[i]->af2));
	}

	load(argD,f.arg_af2,f.nb_used_vars,f.used_var);
}

void Affine2Eval::vector_bwd(const ExprVector& v, ExprLabel** compL, const ExprLabel& y) {
	if (v.dim.is_vector()) {
		for (int i=0; i<v.length(); i++) {
			compL[i]->af2->i() = compL[i]->af2->i() & y.af2->v()[i];   // &= between 2 Affine2Domain does not exist
			if ((compL[i]->af2->i()).is_empty()) throw EmptyBoxException();
		}
	}
	else {
		if (v.row_vector())
			for (int i=0; i<v.length(); i++) {
				compL[i]->af2->v() = compL[i]->af2->v() & y.af2->m().col(i);// &= between 2 Affine2Domain does not exist
				if ((compL[i]->af2->v()).is_empty()) throw EmptyBoxException();
			}
		else
			for (int i=0; i<v.length(); i++) {
				compL[i]->af2->v()= compL[i]->af2->v()  & y.af2->m().row(i);// &= between 2 Affine2Domain does not exist
				if ((compL[i]->af2->v()).is_empty()) throw EmptyBoxException();
			}
	}
}




// TODO attention  au &=




} // namespace ibex

