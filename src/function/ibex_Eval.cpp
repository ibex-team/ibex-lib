/* ============================================================================
 * I B E X - Function basic evaluation
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Jan 14, 2012
 * ---------------------------------------------------------------------------- */

#include "ibex_Function.h"
#include "ibex_Eval.h"

#include <typeinfo>

namespace ibex {

Eval::Eval(Function& f) : f(f), d(f) {

}

Domain& Eval::eval(const Array<const Domain>& d2) {

	d.write_arg_domains(d2);

	//------------- for debug
	//	cout << "Function " << f.name << ", domains before eval:" << endl;
	//	for (int i=0; i<f.nb_arg(); i++) {
	//		cout << "arg[" << i << "]=" << f.arg_domains[i] << endl;
	//	}

	try {
		f.forward<Eval>(*this);
	} catch(EmptyBoxException&) {
		d.top->set_empty();
	}
	return *d.top;
}

Domain& Eval::eval(const Array<Domain>& d2) {

	d.write_arg_domains(d2);

	try {
		f.forward<Eval>(*this);
	} catch(EmptyBoxException&) {
		d.top->set_empty();
	}
	return *d.top;
}

Domain& Eval::eval(const IntervalVector& box) {

	d.write_arg_domains(box);

	try {
		f.forward<Eval>(*this);
	} catch(EmptyBoxException&) {
		d.top->set_empty();
	}
	return *d.top;
}


void Eval::idx_cp_fwd(int x, int y) {
	assert(dynamic_cast<const ExprIndex*> (&f.node(y)));

	const ExprIndex& e = (const ExprIndex&) f.node(y);

	d[y] = d[x][e.index];
}

void Eval::apply_fwd(int* x, int y) {
	assert(dynamic_cast<const ExprApply*> (&f.node(y)));

	const ExprApply& a = (const ExprApply&) f.node(y);

	assert(&a.func!=&f); // recursive calls not allowed

	Array<const Domain> d2(a.func.nb_arg());

	for (int i=0; i<a.func.nb_arg(); i++) {
		d2.set_ref(i,d[x[i]]);
	}

	d[y] = a.func.basic_evaluator().eval(d2);
}

void Eval::vector_fwd(int* x, int y) {
	assert(dynamic_cast<const ExprVector*>(&(f.node(y))));

	const ExprVector& v = (const ExprVector&) f.node(y);

	assert(v.type()!=Dim::SCALAR);

	int j=0;

	if (v.dim.is_vector()) {
		for (int i=0; i<v.length(); i++) {
			if (v.arg(i).dim.is_vector()) {
				d[y].v().put(j,d[x[i]].v());
				j+=v.arg(i).dim.vec_size();
			} else {
				d[y].v()[j]=d[x[i]].i();
				j++;
			}
		}

		assert(j==v.dim.vec_size());
	}
	else {
		if (v.row_vector()) {
			for (int i=0; i<v.length(); i++) {
				if (v.arg(i).dim.is_matrix()) {
					d[y].m().put(0,j,d[x[i]].m());
					j+=v.arg(i).dim.nb_cols();
				} else if (v.arg(i).dim.is_vector()) {
					d[y].m().set_col(j,d[x[i]].v());
					j++;
				}
			}
		} else {
			for (int i=0; i<v.length(); i++) {
				if (v.arg(i).dim.is_matrix()) {
					d[y].m().put(j,0,d[x[i]].m());
					j+=v.arg(i).dim.nb_rows();
				} else if (v.arg(i).dim.is_vector()) {
					d[y].m().set_row(j,d[x[i]].v());
					j++;
				}
			}
		}

		assert((v.row_vector() && j==v.dim.nb_cols()) || (!v.row_vector() && j==v.dim.nb_rows()));
	}

}

} // namespace ibex
