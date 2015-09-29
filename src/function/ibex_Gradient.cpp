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

using namespace std;

namespace ibex {

void Gradient::gradient(const Function& f, const Array<Domain>& d, IntervalVector& g) const {
	assert(f.expr().dim.is_scalar());
	assert(f.expr().deco.d);
	assert(f.expr().deco.g);

	Eval().eval(f,d);

	// outside definition domain -> empty gradient
	if (f.expr().deco.d->is_empty()) { g.set_empty(); return; }

	g.clear();

	f.write_arg_domains(g,true);

	f.forward<Gradient>(*this);

	f.expr().deco.g->i()=1.0;

	f.backward<Gradient>(*this);

	f.read_arg_domains(g,true);
}

void Gradient::gradient(const Function& f, const IntervalVector& box, IntervalVector& g) const {
	assert(f.expr().dim.is_scalar());
	assert(f.expr().deco.d);
	assert(f.expr().deco.g);

	if (f.eval_domain(box).is_empty()) {
		// outside definition domain -> empty gradient
		g.set_empty(); return;
	}

	g.clear();

	f.write_arg_domains(g,true);

	f.forward<Gradient>(*this);

	f.expr().deco.g->i()=1.0;

	f.backward<Gradient>(*this);

	f.read_arg_domains(g,true);
}


void Gradient::jacobian(const Function& f, const Array<Domain>& d, IntervalMatrix& J) const {
	assert(f.expr().dim.is_vector());
	assert(f.expr().deco.d);
	assert(f.expr().deco.g);

	int m=f.expr().dim.vec_size();

	// calculate the gradient of each component of f
	for (int i=0; i<m; i++) {
		const Function* fi=dynamic_cast<const Function*>(&f[i]);
		if (fi!=NULL) {
			// if this is a Function object we can
			// directly calculate the gradient with d
			gradient(*fi,d,J[i]);
		} else {
			// otherwise we must give a box in argument
			// TODO add gradient with Array<Domain> in argument
			// in Function interface?
			// But, for the moment, cannot happen, because
			// this function is called by apply_bwd.
			IntervalVector box(f.nb_var());
			load(box,d);
			f[i].gradient(box,J[i]);
			if (J[i].is_empty()) { J.set_empty(); return; }
		}
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
		if (!a.func.expr().dim.is_vector())
			not_implemented("automatic differentiation of matrix-valued function");
		int m=a.func.expr().dim.vec_size();
		IntervalMatrix J(m,n);
		jacobian(a.func,d,J);
		tmp_g = y.g->v()*J; // pre-multiplication by y.g
		tmp_g += old_g;
		load(g,tmp_g);
	}
}

void Gradient::chi_bwd (const ExprChi&, ExprLabel& a, ExprLabel& b, ExprLabel& c, const ExprLabel& y) {
	Interval ga,gb,gc;

	if (a.d->i().ub()<0) {
		ga=Interval::ZERO;
		gb=Interval::ONE;
		gc=Interval::ZERO;
	}
	else if (a.d->i().lb()>0) {
		ga=Interval::ZERO;
		gb=Interval::ZERO;
		gc=Interval::ONE;
	} else {

		if (b.d->i().is_degenerated() && c.d->i().is_degenerated()) {
			// this applies in particular when b and c are constants.
			// the partial derivative wrt to a can be refined

			double _b =b.d->i().ub();
			double _c =c.d->i().ub();
			if (_b<_c) ga=Interval::POS_REALS;
			else if (_b>_c) ga=Interval::NEG_REALS;
			else ga=Interval::ZERO;
		} else {
			ga=Interval::ALL_REALS;
		}

		gb=Interval(0,1);
		gc=Interval(0,1);
	}

	a.g->i() += y.g->i() * ga;
	b.g->i() += y.g->i() * gb;
	c.g->i() += y.g->i() * gc;
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

	if (x1.d->i().lb() > x2.d->i().ub()) {
		gx1=Interval::ZERO;
		gx2=Interval::ONE;
	}
	else if (x2.d->i().lb() > x1.d->i().ub()) {
		gx1=Interval::ONE;
		gx2=Interval::ZERO;
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
void Gradient::atan2_bwd(const ExprAtan2& , ExprLabel& x1,   ExprLabel& x2,   const ExprLabel& y) {
    x1.g->i() += y.g->i() * x2.d->i() / (sqr(x2.d->i()) + sqr(x1.d->i()));
    x2.g->i() += y.g->i() * - x1.d->i() / (sqr(x2.d->i()) + sqr(x1.d->i()));
}
} // namespace ibex
