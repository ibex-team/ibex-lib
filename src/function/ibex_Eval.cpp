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

using namespace std;

namespace ibex {

Eval::Eval(Function& f) : f(f), d(f), fwd_agenda(NULL), bwd_agenda(NULL) {
	int m=f.image_dim();
	if (m>1) {
		const ExprVector* vec=dynamic_cast<const ExprVector*>(&f.expr());
		if (vec && m==vec->nb_args) {
			fwd_agenda = new Agenda*[m];
			bwd_agenda = new Agenda*[m];
			for (int i=0; i<m; i++) {
				bwd_agenda[i] = f.cf.agenda(f.nodes.rank(vec->arg(i)));
				fwd_agenda[i] = new Agenda(*bwd_agenda[i],true); // true<=>swap
			}
		}
	}
}

Eval::~Eval() {
	if (fwd_agenda!=NULL) {
		for (int i=0; i<f.image_dim(); i++) {
			delete fwd_agenda[i];
			delete bwd_agenda[i];
		}
		delete[] fwd_agenda;
		delete[] bwd_agenda;
	}
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

IntervalVector Eval::eval(const IntervalVector& box, const BitSet& components) {

	d.write_arg_domains(box);

	assert(!components.empty());

	int m=components.size();

	IntervalVector res(m);

	if (fwd_agenda==NULL) {

		assert(!f.expr().dim.is_matrix());

		// The vector of expression is heterogeneous (or the expression is scalar).
		//
		// We might be able in this case to use the DAG but
		// - the algorithm is more complex
		// - we might not benefit from possible symbolic simplification due
		//   to the fact that only specific components are required (there is
		//   no simple "on the fly" simplification as in the case of a vector
		//   of homogeneous expressions)
		// so we resort to the components functions f[i] --> symbolic copy+no DAG :(
		int c;
		for (int i=0; i<m; i++) {
			c = (i==0 ? components.min() : components.next(c));
			res[i] = f[c].eval(box);
		}

		return res;
	}

	// merge all the agendas
	int c;
	Agenda a(f.nodes.size()); // the global agenda initialized with the maximal possible value
	for (int i=0; i<m; i++) {
		c = (i==0 ? components.min() : components.next(c));
		a.push(*(fwd_agenda[c]));
	}

	try {
		f.cf.forward<Eval>(*this,a);

		for (int i=0; i<m; i++) {
			c = (i==0 ? components.min() : components.next(c));
			res[i] = d[bwd_agenda[c]->first()].i();
		}
	} catch(EmptyBoxException&) {
		d.top->set_empty();
		res.set_empty();
	}

	return res;
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

void Eval::gen1_fwd(int x, int y) {
	assert(dynamic_cast<const ExprGenericUnaryOp*>(&(f.node(y))));

	const ExprGenericUnaryOp& e = (const ExprGenericUnaryOp&) f.node(y);
	d[y]=e.eval(d[x]);
}

void Eval::gen2_fwd(int x1, int x2, int y) {
	assert(dynamic_cast<const ExprGenericBinaryOp*>(&(f.node(y))));

	const ExprGenericBinaryOp& e = (const ExprGenericBinaryOp&) f.node(y);
	d[y]=e.eval(d[x1],d[x2]);
}


} // namespace ibex
