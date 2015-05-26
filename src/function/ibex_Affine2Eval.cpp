/* ============================================================================
 * I B E X - ibex_Affine2Eval.cpp
 * ============================================================================
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Jordan Ninin
 * Created     : April 08, 2013
 * ---------------------------------------------------------------------------- */


#include "ibex_Affine2Eval.h"
#include "ibex_Eval.h"
//#include <stdio.h>

namespace ibex {


//======================== Forward =======================


ExprLabel& Affine2Eval::eval_label(const Function& f, ExprLabel** args) const {
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

	f.write_arg_domains(argD);
	f.write_arg_af2_domains(argDAF2);

	//------------- for debug
//	std::cout << "Function " << f.name << ", domains before eval:" << std::endl;
//		for (int i=0; i<f.nb_arg(); i++) {
//			std::cout << "arg[" << i << "]=" << f.arg_domains[i] << std::endl;
//		}

	return f.forward<Affine2Eval>(*this);
}



ExprLabel& Affine2Eval::eval_label(const Function& f, const IntervalVector& box) const {
	assert(f.expr().deco.af2);
	assert(f.expr().deco.d);

	f.write_arg_domains(box);
	f.write_arg_af2_domains(box);

	return f.forward<Affine2Eval>(*this);

}

ExprLabel& Affine2Eval::eval_label(const Function& f, const Affine2Vector& box) const {
	assert(f.expr().deco.af2);
	assert(f.expr().deco.d);

	f.write_arg_domains(IntervalVector(box));
	f.write_arg_af2_domains(box);

	return f.forward<Affine2Eval>(*this);

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

