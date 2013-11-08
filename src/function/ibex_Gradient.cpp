/* ============================================================================
 * I B E X - ibex_Gradient.cpp
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Jan 27, 2012
 * ---------------------------------------------------------------------------- */

#include "ibex_Gradient.h"
#include "ibex_Eval.h"

namespace ibex {

void Gradient::gradient(const Function& f, const Array<Domain>& d, IntervalVector& g) const {
	assert(f.expr().dim.is_scalar());
	assert(f.expr().deco.d);
	assert(f.expr().deco.g);

	Eval().eval(f,d);
	for (int i=0; i<f.nb_arg(); i++)
		((Gradient&) *this).symbol_fwd(f.arg(i), f.arg(i).deco);

	f.forward<Gradient>(*this);

	f.expr().deco.g->i()=1.0;
	f.backward<Gradient>(*this);

	if (f.all_args_scalar()) {
		for (int i=0; i<f.nb_arg(); i++) {
			g[i]=f.arg_deriv[i].i();
		}
	} else {
		load(g,f.arg_deriv);
	}
}

void Gradient::gradient(const Function& f, const IntervalVector& box, IntervalVector& g) const {
	assert(f.expr().dim.is_scalar());
	assert(f.expr().deco.d);
	assert(f.expr().deco.g);

	f.eval_domain(box);
	for (int i=0; i<f.nb_arg(); i++)
		((Gradient&) *this).symbol_fwd(f.arg(i), f.arg(i).deco);

	f.forward<Gradient>(*this);

	f.expr().deco.g->i()=1.0;
	f.backward<Gradient>(*this);

	if (f.all_args_scalar()) {
		for (int i=0; i<f.nb_arg(); i++) {
			g[i]=f.arg_deriv[i].i();
		}
	} else {
		load(g,f.arg_deriv);
	}
}


void Gradient::jacobian(const Function& f, const Array<Domain>& d, IntervalMatrix& J) const {
	assert(f.expr().dim.is_vector());
	assert(f.expr().deco.d);
	assert(f.expr().deco.g);

	int m=f.expr().dim.vec_size();

	// calculate the gradient of each component of f
	for (int i=0; i<m; i++) {
		gradient(f[i],d,J[i]);
	}
}

void Gradient::vector_fwd(const ExprVector& v, const ExprLabel** x, ExprLabel& y) {
	if (v.dim.is_vector())
		y.g->v().clear();
	else
		y.g->m().clear();
}

void Gradient::vector_bwd(const ExprVector& v, ExprLabel** compL, const ExprLabel& y) {
	if (v.dim.is_vector()) {
		for (int i=0; i<v.length(); i++) compL[i]->g->i()+=y.g->v()[i];
	}
	else {
		if (v.row_vector())
			for (int i=0; i<v.length(); i++) compL[i]->g->v()+=y.g->m()[i];
		else
			for (int i=0; i<v.length(); i++) compL[i]->g->v()+=y.g->m().col(i);
	}
}

void Gradient::apply_bwd (const ExprApply& a, ExprLabel** x, const ExprLabel& y) {

	Array<Domain> d(a.nb_args);
	Array<Domain> g(a.nb_args);
	int n=0;

	for (int i=0; i<a.nb_args; i++) {
		d.set_ref(i,*x[i]->d);
		g.set_ref(i,*x[i]->g);
		n+=x[i]->d->dim.size();
	}

	// we unvectorize the components of the gradient.
	IntervalVector old_g(n);
	load(old_g, g);
	IntervalVector tmp_g(n);

	if (a.func.expr().dim.is_scalar()) {
		gradient(a.func,d,tmp_g);
		//cout << "tmp-g=" << tmp_g << endl;
		tmp_g *= y.g->i();   // pre-multiplication by y.g
		tmp_g += old_g;      // addition to the old value of g
		load(g,tmp_g);
	} else {
		assert(a.func.expr().dim.is_vector()); // matrix-valued function not implemented...
		int m=a.func.expr().dim.vec_size();
		IntervalMatrix J(m,n);
		jacobian(a.func,d,J);
		tmp_g = y.g->v()*J; // pre-multiplication by y.g
		tmp_g += old_g;
		load(g,tmp_g);
	}
}

void Gradient::chi_bwd (const ExprChi&, ExprLabel& a, ExprLabel& b, ExprLabel& c, const ExprLabel& y) {
	Interval gx1,gx2;
// TODO Jordan: to check please Gilles C. :D it is inspired from "max"
	if (a.d->i().ub()<=0) {
		gx1=Interval::ONE;
		gx2=Interval::ZERO;
	}
	else if (a.d->i().lb()>0) {
		gx1=Interval::ZERO;
		gx2=Interval::ONE;
	} else {
		gx1=Interval(0,1);
		gx2=Interval(0,1);
	}

	b.g->i() += y.g->i() * gx1;
	c.g->i() += y.g->i() * gx2;
}


void Gradient::max_bwd(const ExprMax&, ExprLabel& x1, ExprLabel& x2, const ExprLabel& y) {
	Interval gx1,gx2;

	if (x1.d->i().lb() > x2.d->i().ub()) {
		gx1=Interval::ONE;
		gx2=Interval::ZERO;
	}
	else if (x2.d->i().lb() > x1.d->i().ub()) {
		gx1=Interval::ZERO;
		gx2=Interval::ONE;
	} else {
		gx1=Interval(0,1);
		gx2=Interval(0,1);
	}

	x1.g->i() += y.g->i() * gx1;
	x2.g->i() += y.g->i() * gx2;
}

void Gradient::min_bwd(const ExprMin&, ExprLabel& x1, ExprLabel& x2, const ExprLabel& y) {
	Interval gx1,gx2;

	if (x1.d->i().lb() < x2.d->i().ub()) {
		gx1=Interval::ONE;
		gx2=Interval::ZERO;
	}
	else if (x2.d->i().lb() < x1.d->i().ub()) {
		gx1=Interval::ZERO;
		gx2=Interval::ONE;
	} else {
		gx1=Interval(0,1);
		gx2=Interval(0,1);
	}

	x1.g->i() += y.g->i() * gx1;
	x2.g->i() += y.g->i() * gx2;
}

void Gradient::sign_bwd  (const ExprSign& e,  ExprLabel& exprL, const ExprLabel& result) {
	if (exprL.d->i().contains(0)) exprL.g->i() += result.g->i()*Interval::POS_REALS;
	else ; // nothing to do: derivative is zero
}

void Gradient::abs_bwd (const ExprAbs& e, ExprLabel& exprL, const ExprLabel& result) {
	if (exprL.d->i().lb()>=0) exprL.g->i() += 1.0*result.g->i();
	else if (exprL.d->i().ub()<=0) exprL.g->i() += -1.0*result.g->i();
	else exprL.g->i() += Interval(-1,1)*result.g->i();
}

} // namespace ibex
