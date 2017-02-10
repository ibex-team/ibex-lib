/* ============================================================================
 * I B E X - Evaluation of Affine forms
 * ============================================================================
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Jordan Ninin, Gilles Chabert
 * Created     : April 08, 2013
 * ---------------------------------------------------------------------------- */

#ifndef __IBEX_AFFINE_EVAL_H__
#define __IBEX_AFFINE_EVAL_H__

#include "ibex_Function.h"
#include "ibex_AffineMatrix.h"
#include "ibex_AffineDomain.h"
#include "ibex_Domain.h"
#include "ibex_TemplateDomain.h"
#include "ibex_Eval.h"


namespace ibex {

/**
 * \ingroup symbolic
 *
 * \brief Evaluate a function with affine form.
 */
template<class T>
class AffineEval : public Eval {

public:
	/**
	 * \brief Build an Affine evaluator for f.
	 */
	explicit AffineEval(const Function& f);

	/**
	 * \brief Delete this.
	 */
	~AffineEval();

	/**
	 * \brief Run the forward algorithm with input domains.
	 */
	Domain& eval(const Array<const Domain>& d);

	/**
	 * \brief Run the forward algorithm with input domains.
	 */
	Domain& eval(const Array<Domain>& d);

	/**
	 * \brief Run the forward algorithm with an input box.
	 */
	Domain& eval(const IntervalVector& box);

	/**
	 * \brief Run the forward algorithm on the box \a box and return the result as an Affine2 domain.
	 */
	TemplateDomain<AffineMain<T> >& eval(const AffineMainVector<T>& af);

	/**
	 * \brief Run the forward algorithm on the box \a box and return the result as an Affine2 domain.
	 */
	TemplateDomain<AffineMain<T> >& eval(const Array< TemplateDomain<AffineMain<T> > >& af);

	/**
	 * \brief Run the forward algorithm on the box \a box and return the result as an Affine2 domain.
	 */
	TemplateDomain<AffineMain<T> >& eval(const Array<const TemplateDomain<AffineMain<T> > >& af);

	/**
	 * \brief Run the forward algorithm with input domains \a d and the same input in \a af as AffineDomain.
	 */
	Domain& eval(const Array<const Domain>& d, const Array<const TemplateDomain<AffineMain<T> > >& af);

	/**
	 * \brief Run the forward algorithm with input domains \a d and the same input in \a af as AffineDomain.
	 */
	Domain& eval(const Array<Domain>& d, const Array< TemplateDomain<AffineMain<T> > >& af);

	/**
	 * \brief Run the forward algorithm with an input box \a d and the same input in \a af as AffineVector.
	 */
	Domain& eval(const IntervalVector& box, const AffineMainVector<T>& af);


//public:	// because called from CompiledFunction

	       void vector_fwd (int* x, int y);
	       void apply_fwd  (int* x, int y);
	       void idx_cp_fwd (int x, int y);
	inline void idx_fwd    (int x, int y);
	inline void symbol_fwd (int y);
	inline void cst_fwd    (int y);
	inline void chi_fwd    (int x1, int x2, int x3, int y);
	inline void add_fwd    (int x1, int x2, int y);
	inline void mul_fwd    (int x1, int x2, int y);
	inline void sub_fwd    (int x1, int x2, int y);
	inline void div_fwd    (int x1, int x2, int y);
	inline void max_fwd    (int x1, int x2, int y);
	inline void min_fwd    (int x1, int x2, int y);
	inline void atan2_fwd  (int x1, int x2, int y);
	inline void minus_fwd  (int x, int y);
	inline void minus_V_fwd(int x, int y);
	inline void minus_M_fwd(int x, int y);
	inline void trans_V_fwd(int x, int y);
	inline void trans_M_fwd(int x, int y);
	inline void sign_fwd   (int x, int y);
	inline void abs_fwd    (int x, int y);
	inline void power_fwd  (int x, int y, int p);
	inline void sqr_fwd    (int x, int y);
	inline void sqrt_fwd   (int x, int y);
	inline void exp_fwd    (int x, int y);
	inline void log_fwd    (int x, int y);
	inline void cos_fwd    (int x, int y);
	inline void sin_fwd    (int x, int y);
	inline void tan_fwd    (int x, int y);
	inline void cosh_fwd   (int x, int y);
	inline void sinh_fwd   (int x, int y);
	inline void tanh_fwd   (int x, int y);
	inline void acos_fwd   (int x, int y);
	inline void asin_fwd   (int x, int y);
	inline void atan_fwd   (int x, int y);
	inline void acosh_fwd  (int x, int y);
	inline void asinh_fwd  (int x, int y);
	inline void atanh_fwd  (int x, int y);
	inline void add_V_fwd  (int x1, int x2, int y);
	inline void add_M_fwd  (int x1, int x2, int y);
	inline void mul_SV_fwd (int x1, int x2, int y);
	inline void mul_SM_fwd (int x1, int x2, int y);
	inline void mul_VV_fwd (int x1, int x2, int y);
	inline void mul_MV_fwd (int x1, int x2, int y);
	inline void mul_VM_fwd (int x1, int x2, int y);
	inline void mul_MM_fwd (int x1, int x2, int y);
	inline void sub_V_fwd  (int x1, int x2, int y);
	inline void sub_M_fwd  (int x1, int x2, int y);


	ExprTemplateDomain< AffineMain<T> > af;

protected:
	/**
	 * Since there is no affine evaluator in the Function class,
	 * we store here all the evaluators of the called functions
	 * (this avoids to create this object at each evaluation).
	 */
	NodeMap< AffineEval<T>* > apply_eval;

private:
	/**
	 * Cast all the Domains into AffineDomains
	 */
	AffineMainVector<T> convert(const Array<const Domain>& d);
	AffineMainVector<T> convert(const Array<Domain>& d);

};

typedef AffineEval<AF_Default> Affine2Eval;
typedef AffineEval<AF_Other>  Affine3Eval;

/* ============================================================================
 	 	 	 	 	 	 	 implementation
  ============================================================================*/


template<class T>
AffineEval<T>::AffineEval(const Function& f) : Eval(f), af(f) {

}


template<class T>
AffineEval<T>::~AffineEval() {
	for (typename IBEX_NODE_MAP(AffineEval<T>*)::iterator it=apply_eval.begin(); it!=apply_eval.end(); it++) {
		delete it->second;
	}
}


template<class T>
AffineMainVector<T> AffineEval<T>::convert(const Array<Domain>& y) {
	return convert((const Array<const Domain >&) y);
}
template<class T>
AffineMainVector<T> AffineEval<T>::convert(const Array<const Domain>& y) {

	int l=0; // the maximum size of the Affine form
	for (int s=0; s<y.size(); s++) {
		l += y[s].dim.size();
	}

	IntervalVector box(l);
	load(box, y);
	return AffineMainVector<T>(box);
}


template<class T>
Domain& AffineEval<T>::eval(const Array<Domain>& d2) {
	return this->eval((const Array<const Domain>&) d2 );
}


template<class T>
Domain& AffineEval<T>::eval(const Array<const Domain>& d2) {

	d.write_arg_domains(d2);
	af.write_arg_domains(convert(d2));

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



template<class T>
inline void AffineEval<T>::idx_fwd(int, int ) { /* nothing to do */ }

template<class T>
inline void AffineEval<T>::symbol_fwd(int) { /* nothing to do */ }

template<class T>
inline void AffineEval<T>::cst_fwd(int y) {
	const ExprConstant& c = (const ExprConstant&) f.node(y);
	switch (c.type()) {
	case Dim::SCALAR:      {
		af[y].i() = c.get_value();
		d[y].i() = c.get_value();
		break;
	}
	case Dim::ROW_VECTOR:
	case Dim::COL_VECTOR: {
		af[y].v() = c.get_vector_value();
		d[y].v() = c.get_vector_value();
		break;
	}
	case Dim::MATRIX: {
		af[y].m() = c.get_matrix_value();
		d[y].m() = c.get_matrix_value();
		break;
	}
	}
}

template<class T>
inline void AffineEval<T>::chi_fwd(int x1, int x2, int x3, int y) {
	af[y].i()=chi(d[x1].i(),af[x2].i(),af[x3].i());
	d[y].i()  =chi(d[x1].i(),d[x2].i(),d[x3].i());
}

template<class T>
inline void AffineEval<T>::add_fwd(int x1, int x2, int y) {
	af[y].i()=af[x1].i()+af[x2].i();
	d[y].i()=(af[y].i().itv() & (d[x1].i()+d[x2].i()));
}

template<class T>
inline void AffineEval<T>::mul_fwd(int x1, int x2, int y) {
	af[y].i()=af[x1].i()*af[x2].i();
	d[y].i()=(af[y].i().itv() & (d[x1].i()*d[x2].i()));
}

template<class T>
inline void AffineEval<T>::sub_fwd(int x1, int x2, int y) {
	af[y].i()=af[x1].i()-af[x2].i();
	d[y].i()=(af[y].i().itv() & (d[x1].i()-d[x2].i()));
}

template<class T>
inline void AffineEval<T>::div_fwd(int x1, int x2, int y) {
	af[y].i()=af[x2].i();
	af[y].i().Ainv(d[x2].i());
	af[y].i() *= af[x1].i();
	d[y].i()=(af[y].i().itv() & (d[x1].i()/d[x2].i()));
}

template<class T>
inline void AffineEval<T>::max_fwd(int x1, int x2, int y) {
	d[y].i()= max(d[x1].i(),d[x2].i());
	af[y].i() = AffineMain<T>(d[y].i());
}

template<class T>
inline void AffineEval<T>::min_fwd(int x1, int x2, int y) {
	d[y].i() = min(d[x1].i(),d[x2].i());
	af[y].i()= AffineMain<T>( d[y].i());
}

template<class T>
inline void AffineEval<T>::atan2_fwd(int x1, int x2, int y) {
	d[y].i() = atan2(d[x1].i(),d[x2].i());
	af[y].i()= AffineMain<T>(d[y].i());
}

template<class T>
inline void AffineEval<T>::minus_fwd(int x, int y) {
	af[y].i()= -af[x].i();
	d[y].i()=(af[y].i().itv() & (-d[x].i()));
}

template<class T>
inline void AffineEval<T>::minus_V_fwd(int x, int y) {
	af[y].v()=-af[x].v();
	d[y].v()=(af[y].v().itv() & (-d[x].v()));
}

template<class T>
inline void AffineEval<T>::minus_M_fwd(int x, int y) {
	af[y].m()=-af[x].m();
	d[y].m() = af[y].m().itv();
	d[y].m()&= (-d[x].m());
}

template<class T>
inline void AffineEval<T>::sign_fwd(int x, int y) {
	d[y].i()=sign(d[x].i());
	af[y].i()=d[y].i();
}

template<class T>
inline void AffineEval<T>::abs_fwd(int x, int y) {
	af[y].i()=AffineMain<T>(af[x].i()).Aabs(d[x].i());
	d[y].i()=(af[y].i().itv() & abs(d[x].i()));
}

template<class T>
inline void AffineEval<T>::power_fwd(int x, int y, int p) {
	af[y].i()=AffineMain<T>(af[x].i()).Apow(p,d[x].i());
	d[y].i()=(af[y].i().itv() & pow(d[x].i(),p));
}

template<class T>
inline void AffineEval<T>::sqr_fwd(int x, int y) {
	af[y].i()=AffineMain<T>(af[x].i()).Asqr(d[x].i());
	d[y].i()=(af[y].i().itv() & sqr(d[x].i()));
	if ((d[y].i()).is_empty()) throw EmptyBoxException();
}

template<class T>
inline void AffineEval<T>::sqrt_fwd(int x, int y) {
	af[y].i()=AffineMain<T>(af[x].i()).Asqrt(d[x].i());
	d[y].i()=(af[y].i().itv() & sqrt(d[x].i()));
}

template<class T>
inline void AffineEval<T>::exp_fwd(int x, int y) {
	af[y].i()=AffineMain<T>(af[x].i()).Aexp(d[x].i());
	d[y].i()=(af[y].i().itv() & exp(d[x].i()));
}

template<class T>
inline void AffineEval<T>::log_fwd(int x, int y) {
	af[y].i()=AffineMain<T>(af[x].i()).Alog(d[x].i());
	d[y].i()=(af[y].i().itv() & log(d[x].i()));
	if ((d[y].i()).is_empty()) throw EmptyBoxException();
}

template<class T>
inline void AffineEval<T>::cos_fwd(int x, int y) {
	af[y].i()=AffineMain<T>(af[x].i()).Acos(d[x].i());
	d[y].i()=(af[y].i().itv() & cos(d[x].i()));
}

template<class T>
inline void AffineEval<T>::sin_fwd(int x, int y) {
	af[y].i()=AffineMain<T>(af[x].i()).Asin(d[x].i());
	d[y].i()=(af[y].i().itv() & sin(d[x].i()));
}

template<class T>
inline void AffineEval<T>::tan_fwd(int x, int y) {
	af[y].i()=AffineMain<T>(af[x].i()).Atan(d[x].i());
	d[y].i()=(af[y].i().itv() & tan(d[x].i()));
	if ((d[y].i()).is_empty()) throw EmptyBoxException();
}

template<class T>
inline void AffineEval<T>::cosh_fwd(int x, int y) {
	af[y].i()=AffineMain<T>(af[x].i()).Acosh(d[x].i());
	d[y].i()=(af[y].i().itv() & cosh(d[x].i()));
}

template<class T>
inline void AffineEval<T>::sinh_fwd(int x, int y) {
	af[y].i()=AffineMain<T>(af[x].i()).Asinh(d[x].i());
	d[y].i()=(af[y].i().itv() & sinh(d[x].i()));
}

template<class T>
inline void AffineEval<T>::tanh_fwd(int x, int y) {
	af[y].i()=AffineMain<T>(af[x].i()).Atanh(d[x].i());
	d[y].i()=(af[y].i().itv() & tanh(d[x].i()));
}

template<class T>
inline void AffineEval<T>::acos_fwd(int x, int y) {
	af[y].i()=AffineMain<T>(af[x].i()).Aacos(d[x].i());
	d[y].i()=(af[y].i().itv() & acos(d[x].i()));
	if ((d[y].i()).is_empty()) throw EmptyBoxException();
}

template<class T>
inline void AffineEval<T>::asin_fwd(int x, int y) {
	af[y].i()=AffineMain<T>(af[x].i()).Aasin(d[x].i());
	d[y].i()=(af[y].i().itv() & asin(d[x].i()));
	if ((d[y].i()).is_empty()) throw EmptyBoxException();
}

template<class T>
inline void AffineEval<T>::atan_fwd(int x, int y) {
	af[y].i()=AffineMain<T>(af[x].i()).Aatan(d[x].i());
	d[y].i()=(af[y].i().itv() & atan(d[x].i()));
}

template<class T>
inline void AffineEval<T>::acosh_fwd(int x, int y) {
	d[y].i()=acosh(d[x].i());
	af[y].i()= AffineMain<T>(d[y].i());
	if ((d[y].i()).is_empty()) throw EmptyBoxException();
}

template<class T>
inline void AffineEval<T>::asinh_fwd(int x, int y) {
	d[y].i()=asinh(d[x].i());
	af[y].i()= AffineMain<T>(d[y].i());
}

template<class T>
inline void AffineEval<T>::atanh_fwd(int x, int y) {
	d[y].i()=atanh(d[x].i());
	af[y].i()= AffineMain<T>(d[y].i());
	if ((d[y].i()).is_empty()) throw EmptyBoxException();
}

template<class T>
inline void AffineEval<T>::trans_V_fwd(int x, int y) {
	af[y].v()=af[x].v();
	d[y].v()=d[x].v();
}

template<class T>
inline void AffineEval<T>::trans_M_fwd(int x, int y) {
	af[y].m()=af[x].m().transpose();
	d[y].m()=d[x].m().transpose();
}

template<class T>
inline void AffineEval<T>::add_V_fwd(int x1, int x2, int y) {
	af[y].v()=af[x1].v()+af[x2].v();
	d[y].v()=(af[y].v().itv() & (d[x1].v()+d[x2].v()));
}

template<class T>
inline void AffineEval<T>::add_M_fwd(int x1, int x2, int y) {
	af[y].m()=af[x1].m()+af[x2].m();
	d[y].m()=(af[y].m().itv());
	d[y].m() &= (d[x1].m()+d[x2].m());
}

template<class T>
inline void AffineEval<T>::mul_SV_fwd(int x1, int x2, int y) {
	af[y].v()=af[x1].i()*af[x2].v();
	d[y].v()=(af[y].v().itv() & (d[x1].i()*d[x2].v()));
}

template<class T>
inline void AffineEval<T>::mul_SM_fwd(int x1, int x2, int y) {
	af[y].m()=af[x1].i()*af[x2].m();
	d[y].m()=(af[y].m().itv());
	d[y].m() &= (d[x1].i()*d[x2].m());
}

template<class T>
inline void AffineEval<T>::mul_VV_fwd(int x1, int x2, int y) {
	af[y].i()=af[x1].v()*af[x2].v();
	d[y].i()=(af[y].i().itv() & (d[x1].v()*d[x2].v()));
}

template<class T>
inline void AffineEval<T>::mul_MV_fwd(int x1, int x2, int y) {
	af[y].v()=af[x1].m()*af[x2].v();
	d[y].v()=(af[y].v().itv() & (d[x1].m()*d[x2].v()));
}

template<class T>
inline void AffineEval<T>::mul_VM_fwd(int x1, int x2, int y) {
	af[y].v()=af[x1].v()*af[x2].m();
	d[y].v()=(af[y].v().itv() & (d[x1].v()*d[x2].m()));
}

template<class T>
inline void AffineEval<T>::mul_MM_fwd(int x1, int x2, int y) {
	af[y].m()=af[x1].m()*af[x2].m();
	d[y].m()=(af[y].m().itv()) ;
	d[y].m() &=  (d[x1].m()*d[x2].m());
}

template<class T>
inline void AffineEval<T>::sub_V_fwd(int x1, int x2, int y) {
	af[y].v()=af[x1].v()-af[x2].v();
	d[y].v()=(af[y].v().itv() & (d[x1].v()-d[x2].v()));
}

template<class T>
inline void AffineEval<T>::sub_M_fwd(int x1, int x2, int y) {
	af[y].m()=af[x1].m()-af[x2].m();
	d[y].m() = af[y].m().itv();
	d[y].m() &= (d[x1].m()-d[x2].m());
}

} // namespace ibex


#endif /* __IBEX_AFFINE_EVAL_H__ */
