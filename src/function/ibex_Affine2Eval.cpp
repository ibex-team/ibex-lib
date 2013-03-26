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


ExprLabel& Affine2Eval::eval(const Function& f, ExprLabel** args) const {
	assert(f.expr().deco.af2);
	assert(f.expr().deco.d);

	Array<const Affine2Domain> argDAF2(f.nb_arg());
	Array<const Domain> argD(f.nb_arg());

	for (int i=0; i<f.nb_arg(); i++) {
		argDAF2.set_ref(i,*(args[i]->af2));
	}
	for (int i=0; i<f.nb_arg(); i++) {
		argD.set_ref(i,*(args[i]->d));
	}

	load(f.arg_domains,argD,f.nb_used_vars,f.used_var);
	load(f.arg_af2,argDAF2,f.nb_used_vars,f.used_var);

	//------------- for debug
//	std::cout << "Function " << f.name << ", domains before eval:" << std::endl;
//		for (int i=0; i<f.nb_arg(); i++) {
//			std::cout << "arg[" << i << "]=" << f.arg_domains[i] << std::endl;
//		}

	return *f.forward<Affine2Eval>(*this);
}



ExprLabel& Affine2Eval::eval(const Function& f, const IntervalVector& box) const {
	assert(f.expr().deco.af2);
	assert(f.expr().deco.d);

	if (f.all_args_scalar()) {
		int j;
		for (int i=0; i<f.nb_used_vars; i++) {
			j=f.used_var[i];
			f.arg_af2[j].i()= Affine2(f.nb_var(),j+1,box[j]);
			f.arg_domains[j].i()=box[j];
		}

	}
	else {
		load(f.arg_af2,Affine2Vector(box),f.nb_used_vars,f.used_var);
		load(f.arg_domains,box,f.nb_used_vars,f.used_var); // load the domains of all the symbols
	}

	return *f.forward<Affine2Eval>(*this);

}


void Affine2Eval::vector_fwd(const ExprVector& v, const ExprLabel** compL, ExprLabel& y) {

	assert(v.type()!=Dim::SCALAR);
	assert(v.type()!=Dim::MATRIX_ARRAY);

	if (v.dim.is_vector()) {
		for (int i=0; i<v.length(); i++)  {
			y.af2->v()[i]=compL[i]->af2->i();
			y.d->v()[i]=compL[i]->d->i();
		}
	}
	else {
		if (v.row_vector())
			for (int i=0; i<v.length(); i++) {
				y.af2->m().set_col(i,compL[i]->af2->v());
				y.d->m().set_col(i,compL[i]->d->v());
			}
		else
			for (int i=0; i<v.length(); i++) {
				y.af2->m().set_row(i,compL[i]->af2->v());
				y.d->m().set_row(i,compL[i]->d->v());
			}
	}
}

} // namespace ibex

