//============================================================================
//                                  I B E X                                   
// File        : ibex_InHC4Revise.cpp
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Jul 12, 2012
// Last Update : Jul 12, 2012
//============================================================================

#include "ibex_Function.h"
#include "ibex_InHC4Revise.h"

namespace ibex {

namespace {

class CheckImplemented : public FwdAlgorithm {
public:
	class NotImplemented : public Exception { };
	void vector_fwd (int* x, int y) { throw NotImplemented(); }
	void apply_fwd  (int* x, int y) { }
	void idx_fwd    (int x, int y) { }
	void idx_cp_fwd (int x, int y) { }
	void symbol_fwd (int y) { }
	void cst_fwd    (int y) { }
	void chi_fwd    (int x1, int x2, int x3, int y) { throw NotImplemented(); }
	void gen2_fwd   (int x, int x2, int y) { throw NotImplemented(); }
	void add_fwd    (int x1, int x2, int y) { }
	void mul_fwd    (int x1, int x2, int y) { }
	void sub_fwd    (int x1, int x2, int y) { }
	void div_fwd    (int x1, int x2, int y) { }
	void max_fwd    (int x1, int x2, int y) { }
	void min_fwd    (int x1, int x2, int y) { }
	void atan2_fwd  (int x1, int x2, int y) { throw NotImplemented(); }
	void gen1_fwd   (int x, int y) { throw NotImplemented(); }
	void minus_fwd  (int x, int y) { }
	void minus_V_fwd(int x, int y) { throw NotImplemented(); }
	void minus_M_fwd(int x, int y) { throw NotImplemented(); }
	void trans_V_fwd(int x, int y) { throw NotImplemented(); }
	void trans_M_fwd(int x, int y) { throw NotImplemented(); }
	void sign_fwd   (int x, int y) { throw NotImplemented(); }
	void abs_fwd    (int x, int y) { }
	void power_fwd  (int x, int y, int p) { }
	void sqr_fwd    (int x, int y) { }
	void sqrt_fwd   (int x, int y) { }
	void exp_fwd    (int x, int y) { }
	void log_fwd    (int x, int y) { }
	void cos_fwd    (int x, int y) { }
	void sin_fwd    (int x, int y) { }
	void tan_fwd    (int x, int y) { }
	void cosh_fwd   (int x, int y) { throw NotImplemented(); }
	void sinh_fwd   (int x, int y) { throw NotImplemented(); }
	void tanh_fwd   (int x, int y) { throw NotImplemented(); }
	void acos_fwd   (int x, int y) { throw NotImplemented(); }
	void asin_fwd   (int x, int y) { throw NotImplemented(); }
	void atan_fwd   (int x, int y) { throw NotImplemented(); }
	void acosh_fwd  (int x, int y) { throw NotImplemented(); }
	void asinh_fwd  (int x, int y) { throw NotImplemented(); }
	void atanh_fwd  (int x, int y) { throw NotImplemented(); }
	void add_V_fwd  (int x1, int x2, int y) { throw NotImplemented(); }
	void add_M_fwd  (int x1, int x2, int y) { throw NotImplemented(); }
	void mul_SV_fwd (int x1, int x2, int y) { throw NotImplemented(); }
	void mul_SM_fwd (int x1, int x2, int y) { throw NotImplemented(); }
	void mul_VV_fwd (int x1, int x2, int y) { throw NotImplemented(); }
	void mul_MV_fwd (int x1, int x2, int y) { throw NotImplemented(); }
	void mul_VM_fwd (int x1, int x2, int y) { throw NotImplemented(); }
	void mul_MM_fwd (int x1, int x2, int y) { throw NotImplemented(); }
	void sub_V_fwd  (int x1, int x2, int y) { throw NotImplemented(); }
	void sub_M_fwd  (int x1, int x2, int y) { throw NotImplemented(); }
	void floor_fwd  (int x, int y) { throw NotImplemented(); }
	void ceil_fwd   (int x, int y) { throw NotImplemented(); }
	void saw_fwd    (int x, int y) { throw NotImplemented(); }
};

}

InHC4Revise::InHC4Revise(Eval& e) : f(e.f), eval(e), d(e.d), p_eval(f), p(p_eval.d) {

}

bool InHC4Revise::implemented() const {
	CheckImplemented c;
	try {
		f.forward<CheckImplemented>(c);
		return true;
	} catch(CheckImplemented::NotImplemented&) {
		return false;
	}
}

void InHC4Revise::iproj(const Domain& y, IntervalVector& x) {
	iproj(y,x,IntervalVector::empty(x.size()));
}

void InHC4Revise::iproj(const Domain& y, IntervalVector& x, const IntervalVector& xin) {

	if (!xin.is_empty()) {
		p_eval.eval(xin);

		assert(!p.top->is_empty());
	}
	else {
		for (int i=0; i<f.expr().size; i++)
			p[i].set_empty();
	}

	eval.eval(x);

	if (d.top->is_empty()) {
		assert(xin.is_empty());
		x.set_empty();
		return;
	}

	*d.top = y;

	try {
		f.backward<InHC4Revise>(*this);

		d.read_arg_domains(x);

	} catch(EmptyBoxException&) {
		assert(xin.is_empty());
		x.set_empty();
	}
}

void InHC4Revise::iproj(const Domain& y, Array<Domain>& x, const Array<Domain>& argP) {
	if (!argP[0].is_empty()) { // if the first domain is empty, so they all are
		p_eval.eval(argP);
	} else {
		for (int i=0; i<f.expr().size; i++)
			p[i].set_empty();
	}

	eval.eval(x);

	assert(argP[0].is_empty() || !d.top->is_empty());

	*d.top = y;

	// may throw EmptyBoxException&) {
	f.backward<InHC4Revise>(*this);

	d.read_arg_domains(x);
}

void InHC4Revise::idx_cp_bwd(int x, int y) {
	assert(dynamic_cast<const ExprIndex*> (&f.node(y)));

	const ExprIndex& e = (const ExprIndex&) f.node(y);

	d[x].put(e.index.first_row(), e.index.first_col(), d[y]);
}

void InHC4Revise::apply_bwd(int* x, int y) {

	assert(dynamic_cast<const ExprApply*> (&f.node(y)));

	const ExprApply& a = (const ExprApply&) f.node(y);

	assert(&a.func!=&f); // recursive calls not allowed

	Array<Domain> d2(a.func.nb_arg());
	Array<Domain> p2(a.func.nb_arg());

	for (int i=0; i<a.func.nb_arg(); i++) {
		d2.set_ref(i,d[x[i]]);
		p2.set_ref(i,p[x[i]]);
	}

	// if next instruction throws an EmptyBoxException,
	// it will be caught by iproj(...,IntervalVector& x).
	// (it is a protected function, not called outside of the class
	// so there is no risk)
	a.func.inhc4revise().iproj(d[y],d2,p2);
}

} // end namespace ibex
