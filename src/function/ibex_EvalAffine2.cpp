/*
 * ibex_EvalAffine2.cpp
 *
 *  Created on: 7 mars 2013
 *      Author: nininjo
 */



#include "ibex_EvalAffine2.h"
#include "ibex_Eval.h"
// #include <stdio.h>

namespace ibex {

DomainAffine2& EvalAffine2::eval_affine2(const Function& f, ExprLabel** args) const {
	assert(f.expr().deco.af2);

	Array<const DomainAffine2> argD(f.nb_arg());

	for (int i=0; i<f.nb_arg(); i++) {
		argD.set_ref(i,*(args[i]->af2));
	}

	load(f.arg_af2,argD,f.nb_used_vars,f.used_var);

	//------------- for debug
//	std::cout << "Function " << f.name << ", domains before eval:" << std::endl;
//		for (int i=0; i<f.nb_arg(); i++) {
//			std::cout << "arg[" << i << "]=" << f.arg_domains[i] << std::endl;
//		}

	return *f.forward<EvalAffine2>(*this).af2;
}


DomainAffine2& EvalAffine2::eval_affine2(const Function& f, const Array<const DomainAffine2>& d) const {
	assert(f.expr().deco.af2);

	load(f.arg_af2,d,f.nb_used_vars,f.used_var);

	return *f.forward<EvalAffine2>(*this).af2;
}

DomainAffine2& EvalAffine2::eval_affine2(const Function& f, const Array<DomainAffine2>& d) const {
	assert(f.expr().deco.af2);

	load(f.arg_af2,d,f.nb_used_vars,f.used_var);

	return *f.forward<EvalAffine2>(*this).af2;
}


DomainAffine2& EvalAffine2::eval_affine2(const Function& f, const IntervalVector& box) const {
	assert(f.expr().deco.af2);

	if (f.all_args_scalar()) {
		int j;
		for (int i=0; i<f.nb_used_vars; i++) {
			j=f.used_var[i];
			f.arg_af2[j].i()= Affine2(f.nb_var(),j+1,box[j]);
		}

	}
	else
		load(f.arg_af2,Affine2Vector(box),f.nb_used_vars,f.used_var); // load the domains of all the symbols


	return *f.forward<EvalAffine2>(*this).af2;

}


void EvalAffine2::vector_fwd(const ExprVector& v, const ExprLabel** compL, ExprLabel& y) {

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


} // namespace ibex

