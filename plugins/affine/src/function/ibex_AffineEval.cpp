/* ============================================================================
 * I B E X - ibex_AffineEval.cpp
 * ============================================================================
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Jordan Ninin
 * Created     : April 08, 2013
 * ---------------------------------------------------------------------------- */


#include "ibex_AffineEval.h"
#include "ibex_Function.h"
//#include <stdio.h>

namespace ibex {


template<class T>
AffineEval<T>::AffineEval(Function& f) : Eval(f), af(f) {

}

template<class T>
Domain& AffineEval<T>::eval(const Array<const Domain>& d2) {

	d.write_arg_domains(d2);
	af.write_arg_domains(AffineMainVector<T>(d2));

	//------------- for debug
	//	cout << "Function " << f.name << ", domains before eval:" << endl;
	//	for (int i=0; i<f.nb_arg(); i++) {
	//		cout << "arg[" << i << "]=" << f.arg_domains[i] << endl;
	//	}

	try {
		f.forward<AffineEval<T> >(*this);
	} catch(EmptyBoxException&) {
		d.top->set_empty();
		af.top->set_empty();
	}
	return *d.top;
}

template<class T>
Domain& AffineEval<T>::eval(const Array<Domain>& d2) {

	d.write_arg_domains(d2);
	af.write_arg_domains(AffineMainVector<T>(d2));

	try {
		f.forward<AffineEval<T> >(*this);
	} catch(EmptyBoxException&) {
		d.top->set_empty();
		af.top->set_empty();
	}
	return *d.top;
}

template<class T>
Domain& AffineEval<T>::eval(const IntervalVector& box) {

	d.write_arg_domains(box);
	af.write_arg_domains(AffineMainVector<T>(box));

	try {
		f.forward<AffineEval<T> >(*this);
	} catch(EmptyBoxException&) {
		d.top->set_empty();
		af.top->set_empty();
	}
	return *d.top;
}


template<class T>
TemplateDomain<AffineMain<T> >& AffineEval<T>::eval(const Array< const TemplateDomain<AffineMain<T> > >& af2) {

	d.write_arg_domains(af2.itv());
	af.write_arg_domains(af2);

	try {
		f.forward<AffineEval<T> >(*this);
	} catch(EmptyBoxException&) {
		d.top->set_empty();
		af.top->set_empty();
	}
	return *af.top;
}

template<class T>
TemplateDomain<AffineMain<T> >& AffineEval<T>::eval(const Array< TemplateDomain<AffineMain<T> > >& af2) {

	d.write_arg_domains(af2.itv());
	af.write_arg_domains(af2);

	try {
		f.forward<AffineEval<T> >(*this);
	} catch(EmptyBoxException&) {
		d.top->set_empty();
		af.top->set_empty();
	}
	return *af.top;
}

template<class T>
TemplateDomain<AffineMain<T> >& AffineEval<T>::eval(const AffineMainVector<T>& af2) {

	d.write_arg_domains(af2.itv());
	af.write_arg_domains(af2);

	try {
		f.forward<AffineEval<T> >(*this);
	} catch(EmptyBoxException&) {
		d.top->set_empty();
		af.top->set_empty();
	}
	return *af.top;
}



template<class T>
Domain& AffineEval<T>::eval(const Array<const Domain>& d2, const Array< const TemplateDomain<AffineMain<T> > >& af2) {

	d.write_arg_domains(d2);
	af.write_arg_domains(af2);

	//------------- for debug
	//	cout << "Function " << f.name << ", domains before eval:" << endl;
	//	for (int i=0; i<f.nb_arg(); i++) {
	//		cout << "arg[" << i << "]=" << f.arg_domains[i] << endl;
	//	}

	try {
		f.forward<AffineEval<T> >(*this);
	} catch(EmptyBoxException&) {
		d.top->set_empty();
		af.top->set_empty();
	}
	return *d.top;
}

template<class T>
Domain& AffineEval<T>::eval(const Array<Domain>& d2, const Array< TemplateDomain<AffineMain<T> > >& af2) {

	d.write_arg_domains(d2);
	af.write_arg_domains(af2);

	try {
		f.forward<AffineEval<T> >(*this);
	} catch(EmptyBoxException&) {
		d.top->set_empty();
		af.top->set_empty();
	}
	return *d.top;
}

template<class T>
Domain& AffineEval<T>::eval(const IntervalVector& box, const AffineMainVector<T>& af2) {

	d.write_arg_domains(box);
	af.write_arg_domains(af2);

	try {
		f.forward<AffineEval<T> >(*this);
	} catch(EmptyBoxException&) {
		d.top->set_empty();
		af.top->set_empty();
	}
	return *d.top;
}






template<class T>
void AffineEval<T>::idx_cp_fwd(int x, int y) {
	assert(dynamic_cast<const ExprIndex*> (&f.node(y)));

	const ExprIndex& e = (const ExprIndex&) f.node(y);

	d[y] = d[x][e.index];
	af[y] = af[x][e.index];
}

template<class T>
inline void AffineEval<T>::apply_fwd(int* x, int y) {
	assert(dynamic_cast<const ExprApply*> (&f.node(y)));

	const ExprApply& a = (const ExprApply&) f.node(y);

	assert(&a.func!=&f); // recursive calls not allowed

	Array<const Domain> d2(a.func.nb_arg());
	Array<const TemplateDomain<AffineMain<T> > > af2(a.func.nb_arg());

	for (int i=0; i<a.func.nb_arg(); i++) {
		d2.set_ref(i,d[x[i]]);
		af2.set_ref(i,af[x[i]]);
	}

	AffineEval<T> *func_eval;
	if (!apply_eval.found(a)) {
		func_eval=new AffineEval<T>(a.func);
		apply_eval.insert(a,func_eval);
	} else {
		func_eval=apply_eval[a];
	}

	d[y] = func_eval->eval(d2,af2);

	af[y]  = *(func_eval->af).top;
}

template<class T>
inline void AffineEval<T>::vector_fwd(int *x, int y) {
	assert(dynamic_cast<const ExprVector*>(&(f.node(y))));

	const ExprVector& v = (const ExprVector&) f.node(y);

	assert(v.type()!=Dim::SCALAR);


	int j=0;

	if (v.dim.is_vector()) {
		for (int i=0; i<v.length(); i++) {
			if (v.arg(i).dim.is_vector()) {
				d[y].v().put(j,d[x[i]].v());
				af[y].v().put(j,af[x[i]].v());
				j+=v.arg(i).dim.vec_size();
			} else {
				d[y].v()[j]=d[x[i]].i();
				af[y].v()[j]=af[x[i]].i();
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
					af[y].m().put(0,j,af[x[i]].m());
					j+=v.arg(i).dim.nb_cols();
				} else if (v.arg(i).dim.is_vector()) {
					d[y].m().set_col(j,d[x[i]].v());
					af[y].m().set_col(j,af[x[i]].v());
					j++;
				}
			}
		} else {
			for (int i=0; i<v.length(); i++) {
				if (v.arg(i).dim.is_matrix()) {
					d[y].m().put(j,0,d[x[i]].m());
					af[y].m().put(j,0,af[x[i]].m());
					j+=v.arg(i).dim.nb_rows();
				} else if (v.arg(i).dim.is_vector()) {
					d[y].m().set_row(j,d[x[i]].v());
					af[y].m().set_row(j,af[x[i]].v());
					j++;
				}
			}
		}

		assert((v.row_vector() && j==v.dim.nb_cols()) || (!v.row_vector() && j==v.dim.nb_rows()));
	}

}

} // namespace ibex

