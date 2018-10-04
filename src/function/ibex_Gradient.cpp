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

#include "ibex_Function.h"
#include "ibex_Gradient.h"
#include "ibex_ExprLinearity.h"

using namespace std;

namespace ibex {

Gradient::Gradient(Eval& e): f(e.f), _eval(e), d(e.d), g(f),
		coeff_matrix(f.image_dim(),f.nb_var()+1), is_linear(new bool[f.image_dim()]) {

	if (f.expr().dim.is_matrix())
		return; // class not called in this case

	ExprLinearity el(f.args(),f.expr());

	if (f.expr().dim.is_scalar())
		coeff_matrix[0]=el.coeff_vector(f.expr());
	else
		coeff_matrix=el.coeff_matrix(f.expr());

	for (int i=0; i<f.image_dim(); i++) {
		is_linear[i]=!coeff_matrix[i].is_unbounded();
	}
}

Gradient::~Gradient() {
	delete[] is_linear;
}

void Gradient::gradient(const Array<Domain>& d2, IntervalVector& gbox) {
	assert(f.expr().dim.is_scalar());

	_eval.eval(d2);

	// outside definition domain -> empty gradient
	if (d.top->is_empty()) { gbox.set_empty(); return; }

	gbox.clear();

	g.write_arg_domains(gbox);

	f.forward<Gradient>(*this);

	g.top->i()=1.0;

	f.backward<Gradient>(*this);

	g.read_arg_domains(gbox);
}

void Gradient::gradient(const IntervalVector& box, IntervalVector& gbox) {

	if (!f.expr().dim.is_scalar()) {
		ibex_error("Cannot called \"gradient\" on a vector-valued function");
	}

	if (_eval.eval(box).is_empty()) {
		// outside definition domain -> empty gradient
		gbox.set_empty(); return;
	}

	gbox.clear();

	g.write_arg_domains(gbox);

	f.forward<Gradient>(*this);

	g.top->i()=1.0;

	f.backward<Gradient>(*this);

	g.read_arg_domains(gbox);
}


void Gradient::jacobian(const IntervalVector& box, IntervalMatrix& J, const BitSet& components, int v) {

	int n=f.nb_var();
	int m=components.size();

	if (f.expr().dim.is_matrix()) {
		ibex_error("Cannot called \"jacobian\" on a matrix-valued function");
	}

	assert(m<=f.image_dim());

	assert(J.nb_rows()==components.size());
	assert(J.nb_cols()==n);
	assert(box.size()==n);
	assert(!components.empty());


	int c; // constraint number

	// ============================================================================
	// Detect the "nonlinear" components (those that requires gradient calculation)
	BitSet nonlinear_components=BitSet::empty(f.image_dim());

	for (int i=0; i<m; i++) {

		c=(i==0? components.min() : components.next(c));

		const IntervalVector& row=coeff_matrix[c];

		if (is_linear[c])
			if (v!=-1)
				J[i][v]=row[v];
			else
				J[i]=row.subvector(0,n-1); // the row also contains the additive constant
		else if (v!=-1 && !row[v].is_unbounded()) // linearity w.r.t. to v is enough
			J[i][v]=row[v];
		else
			nonlinear_components.add(c);
	}

	if (nonlinear_components.empty()) return;
	// ============================================================================

	if (f.image_dim()==1) {

		gradient(box,J[0]);

	} else if(_eval.fwd_agenda!=NULL) {

		// If f is just a vector of expressions, we avoid to generate
		// all the components. This has two advantages:
		// - we spare time and space (generation of all components can be very heavy)
		// - we take advantage of the DAG structure, since common subexpressions are
		//   duplicated in each components. Note that this is however only true for the
		//   forward phase (in the backward phase, each components are handled
		//   separately so that shared subexpressions are treated as if they were separate).

		if (_eval.eval(box,nonlinear_components).is_empty()) {
			// outside definition domain -> empty jacobian
			J.set_empty();
			return;
		}

		for (int i=0; i<m; i++) {

			c=(i==0? components.min() : components.next(c));

			if (!nonlinear_components[c]) continue;

			J.row(i).clear();

			g.write_arg_domains(J.row(i));

			f.cf.forward<Gradient>(*this, *(_eval.fwd_agenda)[c]);

			g[_eval.bwd_agenda[c]->first()].i() = 1.0;

			f.cf.backward<Gradient>(*this, *(_eval.bwd_agenda)[c]);

			g.read_arg_domains(J.row(i));

			// // uncomment this to inspect the previous computation:
//
//			const Agenda& a=*(_eval.bwd_agenda[c]);
//			for (int z=a.first(); z!=a.end(); z=a.next(z)) {
//				cout << "  " << d[z] << '\t' << g[z] << '\t' << f.node(z) << endl;
//			}

			if (J.row(i).is_empty()) {
				J.set_empty();
				return;
			}
		}
	} else {

		// Option 1: we calculate the gradient of each component.
		// Option 2: we perform an automatic differentation of p_i(f)
		//           where p_i is the ith projection.
		// Advantage of option 1:
		// - we benefit from the symbolic simplification of components
		//   (that can be sometimes drastic)
		// Advantage of option 2:
		// - we don't need to generate the components of f
		//   (that can also sometimes save a lot of time and memory)
		// We chose option 1 since option 2 is already what is done
		// when f is a vector of expressions (the most frequent case).
		// ======================== option 1 =============================
		for (int i=0; i<m; i++) {
			c=i==0? components.min() : components.next(c);

			f[c].gradient(box,J[i]);

			if (J[i].is_empty()) {
				J.set_empty();
				return;
			}
		}

		// ======================== option 2 =============================
//		if (_eval.eval(box).is_empty()) {
//			// outside definition domain -> empty jacobian
//			J.set_empty(); return;
//		}
//
//		for (int i=0; i<m; i++) {
//			J.row(i).clear();
//
//			g.write_arg_domains(J.row(i));
//
//			f.forward<Gradient>(*this);
//
//			g.top->v()=Vector::zeros(m);
//			g.top->v()[i]=1.0;
//
//			f.backward<Gradient>(*this);
//
//			if (J[i].is_empty()) {
//				J.set_empty();
//				return;
//			} else
//				g.read_arg_domains(J.row(i));
//		}
		// ===============================================================
	}
}

void Gradient::jacobian(const IntervalVector& box, IntervalMatrix& J, int v) {
	jacobian(box,J, BitSet::all(f.image_dim()), v);
}

void Gradient::jacobian(const Array<Domain>& d, IntervalMatrix& J) {

	if (!f.expr().dim.is_vector()) {
		ibex_error("Cannot called \"jacobian\" on a real-valued function");
	}

	int m=f.expr().dim.vec_size();

	// ================== Option n°1 ===========================
	// calculate the gradient of each component of f
	for (int i=0; i<m; i++) {
		const Function* fi=dynamic_cast<const Function*>(&f[i]);
		if (fi!=NULL) {
			// if this is a Function object we can
			// directly calculate the gradient with d
			fi->deriv_calculator().gradient(d,J[i]);
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

	// ================== Option n°2 ===========================
	// direct calculation
	// TODO
}

void Gradient::vector_fwd(int* x, int y) {
	const ExprVector& v = (const ExprVector&) f.node(y);

	if (v.dim.is_vector())
		g[y].v().clear();
	else
		g[y].m().clear();
}

void Gradient::idx_cp_bwd(int x, int y) {
	assert(dynamic_cast<const ExprIndex*> (&f.node(y)));

	const ExprIndex& e = (const ExprIndex&) f.node(y);
	Domain gx=g[x][e.index];
	gx = gx + g[y];
	g[x].put(e.index.first_row(), e.index.first_col(), gx);
}

void Gradient::vector_bwd(int* x, int y) {
	assert(dynamic_cast<const ExprVector*>(&(f.node(y))));

	const ExprVector& v = (const ExprVector&) f.node(y);

	assert(v.type()!=Dim::SCALAR);

	int j=0;

	if (v.dim.is_vector()) {
		for (int i=0; i<v.length(); i++) {
			if (v.arg(i).dim.is_vector()) {
				g[x[i]].v()+=g[y].v().subvector(j,j+v.arg(i).dim.vec_size()-1);
				j+=v.arg(i).dim.vec_size();
			} else {
				g[x[i]].i()+=g[y].v()[j];
				j++;
			}
		}

		assert(j==v.dim.vec_size());
	}
	else {
		if (v.row_vector()) {
			for (int i=0; i<v.length(); i++) {
				if (v.arg(i).dim.is_matrix()) {
					g[x[i]].m()+=g[y].m().submatrix(0,v.dim.nb_rows()-1,j,j+v.arg(i).dim.nb_cols()-1);
					j+=v.arg(i).dim.nb_cols();
				} else if (v.arg(i).dim.is_vector()) {
					g[x[i]].v()+=g[y].m().col(j);
					j++;
				}
			}
		} else {
			for (int i=0; i<v.length(); i++) {
				if (v.arg(i).dim.is_matrix()) {
					g[x[i]].m()+=g[y].m().submatrix(j,j+v.arg(i).dim.nb_rows()-1,0,v.dim.nb_cols()-1);
					j+=v.arg(i).dim.nb_rows();
				} else if (v.arg(i).dim.is_vector()) {
					g[x[i]].v()+=g[y].m().row(j);
					j++;
				}
			}
		}
	}
}

void Gradient::apply_bwd(int* x, int y) {

	const ExprApply& a = (const ExprApply&) f.node(y);

	Array<Domain> d2(a.func.nb_arg());
	Array<Domain> g2(a.nb_args);

	int n=0;

	for (int i=0; i<a.func.nb_arg(); i++) {
		d2.set_ref(i,d[x[i]]);
		g2.set_ref(i,g[x[i]]);
		n+=d[x[i]].dim.size();
	}

	// we unvectorize the components of the gradient.
	IntervalVector old_g(n);
	load(old_g, g2);
	IntervalVector tmp_g(n);

	if (a.func.expr().dim.is_scalar()) {
		a.func.deriv_calculator().gradient(d2,tmp_g);
		//cout << "tmp-g=" << tmp_g << endl;
		tmp_g *= g[y].i();   // pre-multiplication by y.g
		tmp_g += old_g;      // addition to the old value of g
		load(g2,tmp_g);
	} else {
		if (!a.func.expr().dim.is_vector())
			not_implemented("automatic differentiation of matrix-valued function");
		int m=a.func.expr().dim.vec_size();
		IntervalMatrix J(m,n);
		a.func.deriv_calculator().jacobian(d2,J);
		tmp_g = g[y].v()*J; // pre-multiplication by y.g
		tmp_g += old_g;
		load(g2,tmp_g);
	}
}

void Gradient::chi_bwd(int a, int b, int c, int y) {
	Interval ga,gb,gc;

	if (d[a].i().ub()<0) {
		ga=Interval::ZERO;
		gb=Interval::ONE;
		gc=Interval::ZERO;
	}
	else if (d[a].i().lb()>0) {
		ga=Interval::ZERO;
		gb=Interval::ZERO;
		gc=Interval::ONE;
	} else {

		if (d[b].i().is_degenerated() && d[c].i().is_degenerated()) {
			// this applies in particular when b and c are constants.
			// the partial derivative wrt to a can be refined

			double _b =d[b].i().ub();
			double _c =d[c].i().ub();
			if (_b<_c) ga=Interval::POS_REALS;
			else if (_b>_c) ga=Interval::NEG_REALS;
			else ga=Interval::ZERO;
		} else {
			ga=Interval::ALL_REALS;
		}

		gb=Interval(0,1);
		gc=Interval(0,1);
	}

	g[a].i() += g[y].i() * ga;
	g[b].i() += g[y].i() * gb;
	g[c].i() += g[y].i() * gc;
}


void Gradient::max_bwd(int x1, int x2, int y) {
	Interval gx1,gx2;

	if (d[x1].i().lb() > d[x2].i().ub()) {
		gx1=Interval::ONE;
		gx2=Interval::ZERO;
	}
	else if (d[x2].i().lb() > d[x1].i().ub()) {
		gx1=Interval::ZERO;
		gx2=Interval::ONE;
	} else {
		gx1=Interval(0,1);
		gx2=Interval(0,1);
	}

	g[x1].i() += g[y].i() * gx1;
	g[x2].i() += g[y].i() * gx2;
}

void Gradient::min_bwd(int x1, int x2, int y) {
	Interval gx1,gx2;

	if (d[x1].i().lb() > d[x2].i().ub()) {
		gx1=Interval::ZERO;
		gx2=Interval::ONE;
	}
	else if (d[x2].i().lb() > d[x1].i().ub()) {
		gx1=Interval::ONE;
		gx2=Interval::ZERO;
	} else {
		gx1=Interval(0,1);
		gx2=Interval(0,1);
	}

	g[x1].i() += g[y].i() * gx1;
	g[x2].i() += g[y].i() * gx2;
}

void Gradient::sign_bwd(int x, int y) {
	if (d[x].i().contains(0)) g[x].i() += g[y].i()*Interval::POS_REALS;
	else ; // nothing to do: derivative is zero
}

void Gradient::abs_bwd (int x, int y) {
	if (d[x].i().lb()>0) g[x].i() += 1.0*g[y].i();
	else if (d[x].i().ub()<0) g[x].i() += -1.0*g[y].i();
	else g[x].i() += Interval(-1,1)*g[y].i();
}

void Gradient::atan2_bwd(int x1, int x2, int y) {
    g[x1].i() += g[y].i() * d[x2].i() / (sqr(d[x2].i()) + sqr(d[x1].i()));
    g[x2].i() += g[y].i() * - d[x1].i() / (sqr(d[x2].i()) + sqr(d[x1].i()));
}

void Gradient::gen2_bwd(int x1, int x2, int y) {
	/* TODO */
}

void Gradient::gen1_bwd(int x, int y) {
	assert(dynamic_cast<const ExprGenericUnaryOp*>(&(f.node(y))));

	const ExprGenericUnaryOp& e = (const ExprGenericUnaryOp&) f.node(y);

	g[x] = g[x] + e.num_diff(d[x],g[y]); // TODO: implement += for Domain?
}

} // namespace ibex
