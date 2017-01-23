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
#include "ibex_AffineMatrix.h"
#include "ibex_AffineDomain.h"
#include "ibex_FwdAlgorithm.h"
#include "ibex_Domain.h"
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
	AffineEval(Function& f);

	virtual ~AffineEval();

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


public:	// because called from CompiledFunction

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
	NodeMap<AffineEval<T>*> apply_eval;

};

typedef AffineEval<AF_Default> Affine2Eval;
typedef AffineEval<AF_Other>  Affine3Eval;

/* ============================================================================
 	 	 	 	 	 	 	 implementation
  ============================================================================*/


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
	af[y].i()=-af[x].i();
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
