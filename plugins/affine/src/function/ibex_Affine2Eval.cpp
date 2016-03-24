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
#include "ibex_Function.h"
//#include <stdio.h>

namespace ibex {


Affine2Eval::Affine2Eval(const Function& f) : f(f), d(f), af2(f) {

}

void Affine2Eval::forward(const Array<const Domain>& argD, const Array<const Affine2Domain>& argDAF2) {

	d.write_arg_domains(argD);
	af2.write_arg_domains(argDAF2);

	//------------- for debug
//	std::cout << "Function " << f.name << ", domains before eval:" << std::endl;
//		for (int i=0; i<f.nb_arg(); i++) {
//			std::cout << "arg[" << i << "]=" << f.arg_domains[i] << std::endl;
//		}

	f.forward<Affine2Eval>(*this);
}


void Affine2Eval::forward(const IntervalVector& box) {
	d.write_arg_domains(box);
	af2.write_arg_domains(Affine2Vector(box,true));

	// TODO: should manage empty result! (see Eval.cpp)
	f.forward<Affine2Eval>(*this);
}

void Affine2Eval::forward(const Affine2Vector& box) {

	d.write_arg_domains(box.itv());
	af2.write_arg_domains(box);

	f.forward<Affine2Eval>(*this);
}

void Affine2Eval::apply_fwd(int* x, int y) {
	assert(dynamic_cast<const ExprApply*> (&f.node(y)));

	const ExprApply& a = (const ExprApply&) f.node(y);

	assert(&a.func!=&f); // recursive calls not allowed

	Array<const Domain> d2(a.func.nb_arg());
	Array<const Affine2Domain> af22(a.func.nb_arg());

	for (int i=0; i<a.func.nb_arg(); i++) {
		d2.set_ref(i,d[x[i]]);
		af22.set_ref(i,af2[x[i]]);
	}

	/** TODO: Not efficient !! */
	Affine2Eval func_eval(a.func);
	/*============================*/

	func_eval.forward(d2,af22);

	d[y] = *func_eval.d.top;
	af2[y] = *func_eval.af2.top;
}

void Affine2Eval::vector_fwd(int *x, int y) {
	assert(dynamic_cast<const ExprVector*>(&(f.node(y))));

	const ExprVector& v = (const ExprVector&) f.node(y);

	assert(v.type()!=Dim::SCALAR);
	assert(v.type()!=Dim::MATRIX_ARRAY);

	if (v.dim.is_vector()) {
		for (int i=0; i<v.length(); i++)  {
			af2[y].v()[i]=af2[x[i]].i();
			d[y].v()[i]=d[x[i]].i();
		}
	}
	else {
		if (v.row_vector())
			for (int i=0; i<v.length(); i++) {
				af2[y].m().set_col(i,af2[x[i]].v());
				d[y].m().set_col(i,d[x[i]].v());
			}
		else
			for (int i=0; i<v.length(); i++) {
				af2[y].m().set_row(i,af2[x[i]].v());
				d[y].m().set_row(i,d[x[i]].v());
			}
	}
}

} // namespace ibex

