/* ============================================================================
 * I B E X - Evaluation of Affine forms
 * ============================================================================
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Jordan Ninin
 * Created     : April 08, 2013
 * ---------------------------------------------------------------------------- */

#ifndef __IBEX_AFFINE_EVAL_H__
#define __IBEX_AFFINE_EVAL_H__

#include "ibex_Function.h"
#include "ibex_Eval.h"
#include "ibex_AffineMatrix.h"
#include "ibex_AffineDomain.h"
#include "ibex_FwdAlgorithm.h"
#include "ibex_Domain.h"


namespace ibex {



template<class T>  class AffineEval;
typedef AffineEval<AF_Default> Affine2Eval;
typedef AffineEval<AF_Other>  Affine3Eval;


/**
 * \ingroup symbolic
 *
 * \brief Evaluate a function with affine form.
 */
template<class T=AF_Default>
class AffineEval : public FwdAlgorithm, public Evaluator {

public:
	/**
	 * \brief Build an Affine evaluator for f.
	 */
	AffineEval(const Function& f);

	/**
	 * \brief Delete this.
	 */
	~AffineEval();

	/**
	 * \brief Run the forward algorithm on the box \a box and return the result as an interval domain.
	 */
	Domain& eval(const IntervalVector& box);

	/**
	 * \brief Run the forward algorithm with input domains.
	 */
	Domain& eval(const Array<const Domain>& d);

	/**
	 * \brief Run the forward algorithm with input domains.
	 */
	Domain& eval(const Array<Domain>& d);

	/**
	 * \brief Evaluate a subset of components.
	 *
	 * (Specific for vector-valued functions).
	 *
	 * \pre components must be non empty and contain indices in
	 *      [0,f.image_dim()-1].
	 */
	IntervalVector eval(const IntervalVector& box, const BitSet& components);

	/**
	 * \brief Run the forward algorithm on the box \a box and return the result as an Affine2 domain.
	 */
	TemplateDomain<AffineMain<T> >& eval(const AffineMainVector<T>& af);

	void idx_cp_fwd (int x, int y);
	void idx_fwd (int  x, int y);
	void vector_fwd(int* x, int y);
	void cst_fwd   (int y);
	void symbol_fwd(int y);
	void apply_fwd (int* x, int y);
	void chi_fwd   (int x1, int x2, int x3, int y);
	void add_fwd   (int x1, int x2, int y);
	void mul_fwd   (int x1, int x2, int y);
	void sub_fwd   (int x1, int x2, int y);
	void div_fwd   (int x1, int x2, int y);
	void max_fwd   (int x1, int x2, int y);
	void min_fwd   (int x1, int x2, int y);
	void atan2_fwd (int x1, int x2, int y);
	void minus_fwd (int x, int y);
	void minus_V_fwd(int x, int y);
	void minus_M_fwd(int x, int y);
	void trans_V_fwd(int x, int y);
	void trans_M_fwd(int x, int y);
	void sign_fwd   (int x, int y);
	void abs_fwd    (int x, int y);
	void power_fwd  (int x, int y, int p);
	void sqr_fwd    (int x, int y);
	void sqrt_fwd   (int x, int y);
	void exp_fwd    (int x, int y);
	void log_fwd    (int x, int y);
	void cos_fwd    (int x, int y);
	void sin_fwd    (int x, int y);
	void tan_fwd    (int x, int y);
	void cosh_fwd   (int x, int y);
	void sinh_fwd   (int x, int y);
	void tanh_fwd   (int x, int y);
	void acos_fwd   (int x, int y);
	void asin_fwd   (int x, int y);
	void atan_fwd   (int x, int y);
	void acosh_fwd  (int x, int y);
	void asinh_fwd  (int x, int y);
	void atanh_fwd  (int x, int y);

	void add_V_fwd  (int x1, int x2, int y);
	void add_M_fwd  (int x1, int x2, int y);
	void mul_SV_fwd (int x1, int x2, int y);
	void mul_SM_fwd (int x1, int x2, int y);
	void mul_VV_fwd (int x1, int x2, int y);
	void mul_MV_fwd (int x1, int x2, int y);
	void mul_VM_fwd (int x1, int x2, int y);
	void mul_MM_fwd (int x1, int x2, int y);
	void sub_V_fwd  (int x1, int x2, int y);
	void sub_M_fwd  (int x1, int x2, int y);

	ExprTemplateDomain<AffineMain<T> > af2;

protected:
	/**
	 * Class used internally to interrupt the
	 * backward procedure when an empty domain occurs.
	 */
	class EmptyBoxException { };

	/**
	 * \brief Run the forward algorithm with input domains.
	 */
	void forward(const Array<const Domain>& argD, const Array<const TemplateDomain<AffineMain<T> > >& argDAF2);

	/**
	 * Since there is no affine evaluator in the Function class,
	 * we store here all the evaluators of the called functions
	 * (this avoids to create this object at each evaluation).
	 */
	NodeMap<AffineEval<T>*> apply_eval;
};

typedef AffineEval<AF_Default> Affine2Eval;
typedef AffineEval<AF_Other>  Affine3Eval;

/* ============================================================================
 	 	 	 	 	 	 	 implementation
  ============================================================================*/

template<class T>
inline AffineEval<T>::AffineEval(const Function& f) : Evaluator(f), af2(f) {

}

template<class T>
inline AffineEval<T>::~AffineEval() {

	for (typename IBEX_NODE_MAP(AffineEval<T>*)::iterator it=apply_eval.begin(); it!=apply_eval.end(); it++) {
		delete it->second;
	}

}


template<class T>
Domain& AffineEval<T>::eval(const Array<const Domain>& d2) {

	d.write_arg_domains(d2);
	af2.write_arg_domains(convert_to_affinedomain<T>(d2));

	//------------- for debug
	//	cout << "Function " << f.name << ", domains before eval:" << endl;
	//	for (int i=0; i<f.nb_arg(); i++) {
	//		cout << "arg[" << i << "]=" << f.arg_domains[i] << endl;
	//	}
	try {
		f.forward<AffineEval<T> >(*this);
	} catch(EmptyBoxException&) {
		d.top->set_empty();
		af2.top->set_empty();
	}
	return *d.top;
}

template<class T>
Domain& AffineEval<T>::eval(const Array<Domain>& d2) {

	d.write_arg_domains(d2);
	af2.write_arg_domains(convert_to_affinedomain<T>(d2));

	try {
		f.forward<AffineEval<T> >(*this);
	} catch(EmptyBoxException&) {
		d.top->set_empty();
		af2.top->set_empty();
	}
	return *d.top;
}


template<class T>
IntervalVector AffineEval<T>::eval(const IntervalVector& box, const BitSet& components) {

	d.write_arg_domains(box);
	af2.write_arg_domains(AffineMainVector<T>(box));

	assert(!components.empty());

	int m=components.size();

	IntervalVector res(m);

	//if (fwd_agenda==NULL) {

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
		try {
			f.forward<AffineEval<T> >(*this);
			int c;
			for (int i=0; i<m; i++) {
				c = (i==0 ? components.min() : components.next(c));
				res[i] = (d.top->v())[c];
			}
		} catch(EmptyBoxException&) {
			d.top->set_empty();
			af2.top->set_empty();
			res.set_empty();
		}
		return res;
		//}
// not ready...
//	// merge all the agendas
//	int c;
//	Agenda a(f.nodes.size()); // the global agenda initialized with the maximal possible value
//	for (int i=0; i<m; i++) {
//		c = (i==0 ? components.min() : components.next(c));
//		a.push(*(fwd_agenda[c]));
//	}
//
//	try {
//		f.cf.forward<Eval>(*this,a);
//	} catch(EmptyBoxException&) {
//		d.top->set_empty();
//	}
//	for (int i=0; i<m; i++) {
//		c = (i==0 ? components.min() : components.next(c));
//		res[i] = d[bwd_agenda[c]->first()].i();
//	}
//
//	return res;

}



template<class T>
inline Domain& AffineEval<T>::eval(const IntervalVector& box) {
	d.write_arg_domains(box);
	af2.write_arg_domains(AffineMainVector<T>(box));


	try {
		f.forward<AffineEval<T> >(*this);
	} catch(EmptyBoxException&) {
		d.top->set_empty();
		af2.top->set_empty();
	}
	return *d.top;
}

template<class T>
inline TemplateDomain<AffineMain<T> >& AffineEval<T>::eval(const AffineMainVector<T>& box) {
	d.write_arg_domains(box.itv());
	af2.write_arg_domains(box);

	try {
		f.forward<AffineEval<T> >(*this);
	} catch(EmptyBoxException&) {
		d.top->set_empty();
		af2.top->set_empty();
	}
	return *af2.top;
}

template<class T>
inline void AffineEval<T>::forward(const Array<const Domain>& argD, const Array<const TemplateDomain<AffineMain<T> > >& argDAF2) {

	d.write_arg_domains(argD);
	af2.write_arg_domains(argDAF2);

	//------------- for debug
//	std::cout << "Function " << f.name << ", domains before eval:" << std::endl;
//		for (int i=0; i<f.nb_arg(); i++) {
//			std::cout << "arg[" << i << "]=" << f.arg_domains[i] << std::endl;
//		}
	try {
		f.forward<AffineEval<T> >(*this);
	} catch(EmptyBoxException&) {
		d.top->set_empty();
		af2.top->set_empty();
	}
}

template<class T>
inline void AffineEval<T>::idx_fwd(int, int ) { /* nothing to do */ }


template<class T>
void AffineEval<T>::idx_cp_fwd(int x, int y) {
	assert(dynamic_cast<const ExprIndex*> (&f.node(y)));

	const ExprIndex& e = (const ExprIndex&) f.node(y);

	d[y] = d[x][e.index];
	af2[y] = af2[x][e.index];
}

template<class T>
inline void AffineEval<T>::symbol_fwd(int) { /* nothing to do */ }

template<class T>
inline void AffineEval<T>::cst_fwd(int y) {
	const ExprConstant& c = (const ExprConstant&) f.node(y);
	switch (c.type()) {
	case Dim::SCALAR:      {
		af2[y].i() = c.get_value();
		d[y].i() = c.get_value();
		break;
	}
	case Dim::ROW_VECTOR:
	case Dim::COL_VECTOR: {
		af2[y].v() = c.get_vector_value();
		d[y].v() = c.get_vector_value();
		break;
	}
	case Dim::MATRIX: {
		af2[y].m() = c.get_matrix_value();
		d[y].m() = c.get_matrix_value();
		break;
	}
	}
}

template<class T>
inline void AffineEval<T>::chi_fwd(int x1, int x2, int x3, int y) {
	af2[y].i()=chi(d[x1].i(),af2[x2].i(),af2[x3].i());
	d[y].i()  =chi(d[x1].i(),d[x2].i(),d[x3].i());
}

template<class T>
inline void AffineEval<T>::add_fwd(int x1, int x2, int y) {
	af2[y].i()=af2[x1].i()+af2[x2].i();
	d[y].i()=(af2[y].i().itv() & (d[x1].i()+d[x2].i()));
}

template<class T>
inline void AffineEval<T>::mul_fwd(int x1, int x2, int y) {
	af2[y].i()=af2[x1].i()*af2[x2].i();
	d[y].i()=(af2[y].i().itv() & (d[x1].i()*d[x2].i()));
}

template<class T>
inline void AffineEval<T>::sub_fwd(int x1, int x2, int y) {
	af2[y].i()=af2[x1].i()-af2[x2].i();
	d[y].i()=(af2[y].i().itv() & (d[x1].i()-d[x2].i()));
}

template<class T>
inline void AffineEval<T>::div_fwd(int x1, int x2, int y) {
	af2[y].i()=af2[x2].i();
	af2[y].i().Ainv(d[x2].i());
	af2[y].i() *= af2[x1].i();
	d[y].i()=(af2[y].i().itv() & (d[x1].i()/d[x2].i()));
}

template<class T>
inline void AffineEval<T>::max_fwd(int x1, int x2, int y) {
	d[y].i()= max(d[x1].i(),d[x2].i());
	af2[y].i() = d[y].i(); // not yet implemented in Affine Arithmetic
}

template<class T>
inline void AffineEval<T>::min_fwd(int x1, int x2, int y) {
	d[y].i() = min(d[x1].i(),d[x2].i());
	af2[y].i()=  d[y].i(); // not yet implemented in Affine Arithmetic
}

template<class T>
inline void AffineEval<T>::atan2_fwd(int x1, int x2, int y) {
	d[y].i() = atan2(d[x1].i(),d[x2].i());
	af2[y].i()= d[y].i(); // not yet implemented in Affine Arithmetic
}

template<class T>
inline void AffineEval<T>::minus_fwd(int x, int y) {
	af2[y].i()=-af2[x].i();
	d[y].i()=(af2[y].i().itv() & (-d[x].i()));
}

template<class T>
inline void AffineEval<T>::minus_V_fwd(int x, int y) {
	af2[y].v()=-af2[x].v();
	d[y].v()=(af2[y].v().itv() & (-d[x].v()));
}

template<class T>
inline void AffineEval<T>::minus_M_fwd(int x, int y) {
	af2[y].m()=-af2[x].m();
	d[y].m() = af2[y].m().itv();
	d[y].m()&= (-d[x].m());
}

template<class T>
inline void AffineEval<T>::sign_fwd(int x, int y) {
	d[y].i()=sign(d[x].i());
	af2[y].i()=d[y].i();
}

template<class T>
inline void AffineEval<T>::abs_fwd(int x, int y) {
	af2[y].i()=AffineMain<T>(af2[x].i()).Aabs(d[x].i());
	d[y].i()=(af2[y].i().itv() & abs(d[x].i()));
}

template<class T>
inline void AffineEval<T>::power_fwd(int x, int y, int p) {
	af2[y].i()=AffineMain<T>(af2[x].i()).Apow(p,d[x].i());
	d[y].i()=(af2[y].i().itv() & pow(d[x].i(),p));
}

template<class T>
inline void AffineEval<T>::sqr_fwd(int x, int y) {
	af2[y].i()=AffineMain<T>(af2[x].i()).Asqr(d[x].i());
	d[y].i()=(af2[y].i().itv() & sqr(d[x].i()));
}

template<class T>
inline void AffineEval<T>::sqrt_fwd(int x, int y) {
	af2[y].i()=AffineMain<T>(af2[x].i()).Asqrt(d[x].i());
	d[y].i()=(af2[y].i().itv() & sqrt(d[x].i()));
	if ((d[y].i()).is_empty()) throw EmptyBoxException();
}

template<class T>
inline void AffineEval<T>::exp_fwd(int x, int y) {
	af2[y].i()=AffineMain<T>(af2[x].i()).Aexp(d[x].i());
	d[y].i()=(af2[y].i().itv() & exp(d[x].i()));
}

template<class T>
inline void AffineEval<T>::log_fwd(int x, int y) {
	af2[y].i()=AffineMain<T>(af2[x].i()).Alog(d[x].i());
	d[y].i()=(af2[y].i().itv() & log(d[x].i()));
	if ((d[y].i()).is_empty()) throw EmptyBoxException();
}

template<class T>
inline void AffineEval<T>::cos_fwd(int x, int y) {
	af2[y].i()=AffineMain<T>(af2[x].i()).Acos(d[x].i());
	d[y].i()=(af2[y].i().itv() & cos(d[x].i()));
}

template<class T>
inline void AffineEval<T>::sin_fwd(int x, int y) {
	af2[y].i()=AffineMain<T>(af2[x].i()).Asin(d[x].i());
	d[y].i()=(af2[y].i().itv() & sin(d[x].i()));
}

template<class T>
inline void AffineEval<T>::tan_fwd(int x, int y) {
	af2[y].i()=AffineMain<T>(af2[x].i()).Atan(d[x].i());
	d[y].i()=(af2[y].i().itv() & tan(d[x].i()));
	if ((d[y].i()).is_empty()) throw EmptyBoxException();
}

template<class T>
inline void AffineEval<T>::cosh_fwd(int x, int y) {
	af2[y].i()=AffineMain<T>(af2[x].i()).Acosh(d[x].i());
	d[y].i()=(af2[y].i().itv() & cosh(d[x].i()));
}

template<class T>
inline void AffineEval<T>::sinh_fwd(int x, int y) {
	af2[y].i()=AffineMain<T>(af2[x].i()).Asinh(d[x].i());
	d[y].i()=(af2[y].i().itv() & sinh(d[x].i()));
}

template<class T>
inline void AffineEval<T>::tanh_fwd(int x, int y) {
	af2[y].i()=AffineMain<T>(af2[x].i()).Atanh(d[x].i());
	d[y].i()=(af2[y].i().itv() & tanh(d[x].i()));
}

template<class T>
inline void AffineEval<T>::acos_fwd(int x, int y) {
	af2[y].i()=AffineMain<T>(af2[x].i()).Aacos(d[x].i());
	d[y].i()=(af2[y].i().itv() & acos(d[x].i()));
	if ((d[y].i()).is_empty()) throw EmptyBoxException();
}

template<class T>
inline void AffineEval<T>::asin_fwd(int x, int y) {
	af2[y].i()=AffineMain<T>(af2[x].i()).Aasin(d[x].i());
	d[y].i()=(af2[y].i().itv() & asin(d[x].i()));
	if ((d[y].i()).is_empty()) throw EmptyBoxException();
}

template<class T>
inline void AffineEval<T>::atan_fwd(int x, int y) {
	af2[y].i()=AffineMain<T>(af2[x].i()).Aatan(d[x].i());
	d[y].i()=(af2[y].i().itv() & atan(d[x].i()));
}

template<class T>
inline void AffineEval<T>::acosh_fwd(int x, int y) {
	d[y].i()=acosh(d[x].i());
	af2[y].i()= d[y].i(); // not yet implemented in Affine Arithmetic
	if ((d[y].i()).is_empty()) throw EmptyBoxException();
}

template<class T>
inline void AffineEval<T>::asinh_fwd(int x, int y) {
	d[y].i()=asinh(d[x].i());
	af2[y].i()= d[y].i();  // not yet implemented in Affine Arithmetic
}

template<class T>
inline void AffineEval<T>::atanh_fwd(int x, int y) {
	d[y].i()=atanh(d[x].i());
	af2[y].i()= d[y].i(); // not yet implemented in Affine Arithmetic
	if ((d[y].i()).is_empty()) throw EmptyBoxException();
}

template<class T>
inline void AffineEval<T>::trans_V_fwd(int x, int y) {
	af2[y].v()=af2[x].v();
	d[y].v()=d[x].v();
}

template<class T>
inline void AffineEval<T>::trans_M_fwd(int x, int y) {
	af2[y].m()=af2[x].m().transpose();
	d[y].m()=d[x].m().transpose();
}

template<class T>
inline void AffineEval<T>::add_V_fwd(int x1, int x2, int y) {
	af2[y].v()=af2[x1].v()+af2[x2].v();
	d[y].v()=(af2[y].v().itv() & (d[x1].v()+d[x2].v()));
}

template<class T>
inline void AffineEval<T>::add_M_fwd(int x1, int x2, int y) {
	af2[y].m()=af2[x1].m()+af2[x2].m();
	d[y].m()=(af2[y].m().itv());
	d[y].m() &= (d[x1].m()+d[x2].m());
}

template<class T>
inline void AffineEval<T>::mul_SV_fwd(int x1, int x2, int y) {
	af2[y].v()=af2[x1].i()*af2[x2].v();
	d[y].v()=(af2[y].v().itv() & (d[x1].i()*d[x2].v()));
}

template<class T>
inline void AffineEval<T>::mul_SM_fwd(int x1, int x2, int y) {
	af2[y].m()=af2[x1].i()*af2[x2].m();
	d[y].m()=(af2[y].m().itv());
	d[y].m() &= (d[x1].i()*d[x2].m());
}

template<class T>
inline void AffineEval<T>::mul_VV_fwd(int x1, int x2, int y) {
	af2[y].i()=af2[x1].v()*af2[x2].v();
	d[y].i()=(af2[y].i().itv() & (d[x1].v()*d[x2].v()));
}

template<class T>
inline void AffineEval<T>::mul_MV_fwd(int x1, int x2, int y) {
	af2[y].v()=af2[x1].m()*af2[x2].v();
	d[y].v()=(af2[y].v().itv() & (d[x1].m()*d[x2].v()));
}

template<class T>
inline void AffineEval<T>::mul_VM_fwd(int x1, int x2, int y) {
	af2[y].v()=af2[x1].v()*af2[x2].m();
	d[y].v()=(af2[y].v().itv() & (d[x1].v()*d[x2].m()));
}

template<class T>
inline void AffineEval<T>::mul_MM_fwd(int x1, int x2, int y) {
	af2[y].m()=af2[x1].m()*af2[x2].m();
	d[y].m()=(af2[y].m().itv()) ;
	d[y].m() &=  (d[x1].m()*d[x2].m());
}

template<class T>
inline void AffineEval<T>::sub_V_fwd(int x1, int x2, int y) {
	af2[y].v()=af2[x1].v()-af2[x2].v();
	d[y].v()=(af2[y].v().itv() & (d[x1].v()-d[x2].v()));
}

template<class T>
inline void AffineEval<T>::sub_M_fwd(int x1, int x2, int y) {
	af2[y].m()=af2[x1].m()-af2[x2].m();
	d[y].m() = af2[y].m().itv();
	d[y].m() &= (d[x1].m()-d[x2].m());
}

template<class T>
inline void AffineEval<T>::apply_fwd(int* x, int y) {
	assert(dynamic_cast<const ExprApply*> (&f.node(y)));

	const ExprApply& a = (const ExprApply&) f.node(y);

	assert(&a.func!=&f); // recursive calls not allowed

	Array<const Domain> d2(a.func.nb_arg());
	Array<const TemplateDomain<AffineMain<T> > > af22(a.func.nb_arg());

	for (int i=0; i<a.func.nb_arg(); i++) {
		d2.set_ref(i,d[x[i]]);
		af22.set_ref(i,af2[x[i]]);
	}

	AffineEval<T> *func_eval;
	if (!apply_eval.found(a)) {
		func_eval=new AffineEval<T>(a.func);
		apply_eval.insert(a,func_eval);
	} else {
		func_eval=apply_eval[a];
	}

	func_eval->forward(d2,af22);

	d[y] = *func_eval->d.top;
	af2[y] = *func_eval->af2.top;
}


template<class T>
inline void AffineEval<T>::vector_fwd(int* x, int y) {
	assert(dynamic_cast<const ExprVector*>(&(f.node(y))));

	const ExprVector& v = (const ExprVector&) f.node(y);

	assert(v.type()!=Dim::SCALAR);

	int j=0;

	if (v.dim.is_vector()) {
		for (int i=0; i<v.length(); i++) {
			if (v.arg(i).dim.is_vector()) {
				af2[y].v().put(j,af2[x[i]].v());
				d[y].v().put(j,d[x[i]].v());
				j+=v.arg(i).dim.vec_size();
			} else {
				af2[y].v()[j]=af2[x[i]].i();
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
					af2[y].m().put(0,j,af2[x[i]].m());
					d[y].m().put(0,j,d[x[i]].m());
					j+=v.arg(i).dim.nb_cols();
				} else if (v.arg(i).dim.is_vector()) {
					af2[y].m().set_col(j,af2[x[i]].v());
					d[y].m().set_col(j,d[x[i]].v());
					j++;
				}
			}
		} else {
			for (int i=0; i<v.length(); i++) {
				if (v.arg(i).dim.is_matrix()) {
					af2[y].m().put(j,0,af2[x[i]].m());
					d[y].m().put(j,0,d[x[i]].m());
					j+=v.arg(i).dim.nb_rows();
				} else if (v.arg(i).dim.is_vector()) {
					af2[y].m().set_row(j,af2[x[i]].v());
					d[y].m().set_row(j,d[x[i]].v());
					j++;
				}
			}
		}

		assert((v.row_vector() && j==v.dim.nb_cols()) || (!v.row_vector() && j==v.dim.nb_rows()));
	}

}

} // namespace ibex


#endif /* __IBEX_AFFINE_EVAL_H__ */
