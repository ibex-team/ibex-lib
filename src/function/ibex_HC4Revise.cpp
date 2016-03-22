//============================================================================
//                                  I B E X                                   
// File        : HC4Revise Algorithm
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Dec 31, 2011
// Last Update : 
//============================================================================

#include "ibex_Function.h"
#include "ibex_HC4Revise.h"

namespace ibex {

const double HC4Revise::RATIO = 0.1;

HC4Revise::HC4Revise(Eval& e) : f(e.f), eval(e), d(e.d) {

}

bool HC4Revise::proj(const Domain& y, Array<Domain>& x) {
	eval.eval(x);

	bool is_inner=backward(y);

	d.read_arg_domains(x);

	return is_inner;
//	return proj(y,(const Array<const Domain>&) x);
}

//bool HC4Revise::proj(const Domain& y, const Array<const Domain>& x) {
//}

bool HC4Revise::proj(const Domain& y, IntervalVector& x) {
	eval.eval(x);
	//std::cout << "forward:" << std::endl; f.cf.print(d);

	bool is_inner=false;

	try {
		is_inner = backward(y);

		d.read_arg_domains(x);

		return is_inner;

	} catch(EmptyBoxException&) {
		x.set_empty();
		return false;
	}
}

bool HC4Revise::backward(const Domain& y) {

	Domain& root=*d.top;

	if (root.is_empty())
		throw EmptyBoxException();

	switch(y.dim.type()) {
	case Dim::SCALAR:       if (root.i().is_subset(y.i())) return true; break;
	case Dim::ROW_VECTOR:
	case Dim::COL_VECTOR:   if (root.v().is_subset(y.v())) return true; break;
	case Dim::MATRIX:       if (root.m().is_subset(y.m())) return true; break;
	case Dim::MATRIX_ARRAY: assert(false); /* impossible */ break;
	}

	root &= y;

	if (root.is_empty())
		throw EmptyBoxException();

	// may throw an EmptyBoxException().
	eval.f.backward<HC4Revise>(*this);

	return false;
	//std::cout << "backward:" << std::endl; f.cf.print();
}

void HC4Revise::apply_bwd(int* x, int y) {
	assert(dynamic_cast<const ExprApply*> (&f.node(y)));

	const ExprApply& a = (const ExprApply&) f.node(y);

	assert(&a.func!=&f); // recursive calls not allowed

	Array<Domain> d2(a.func.nb_arg());

	for (int i=0; i<a.func.nb_arg(); i++) {
		d2.set_ref(i,d[x[i]]);
	}

	// if next instruction throws an EmptyBoxException,
	// it will be caught by proj(...,IntervalVector& x).
	// (it is a protected function, not called outside of the class
	// so there is no risk)
	a.func.hc4revise().proj(d[y],d2);
}

void HC4Revise::vector_bwd(int* x, int y) {
	assert(dynamic_cast<const ExprVector*>(&(f.node(y))));

	const ExprVector& v = (const ExprVector&) f.node(y);

	if (v.dim.is_vector()) {
		for (int i=0; i<v.length(); i++)
			if ((d[x[i]].i() &= d[y].v()[i]).is_empty()) throw EmptyBoxException();
	}
	else {
		if (v.row_vector())
			for (int i=0; i<v.length(); i++) {
				if ((d[x[i]].v()&=d[y].m().col(i)).is_empty()) throw EmptyBoxException();
			}
		else
			for (int i=0; i<v.length(); i++) {
				if ((d[x[i]].v()&=d[y].m().row(i)).is_empty()) throw EmptyBoxException();
			}
	}
}

} /* namespace ibex */
