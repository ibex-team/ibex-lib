/* ============================================================================
 * I B E X - Evaluation of Affine2 forms
 * ============================================================================
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Jordan Ninin
 * Created     : April 08, 2013
 * ---------------------------------------------------------------------------- */

#ifndef __IBEX_AFFINE2_EVAL_H__
#define __IBEX_AFFINE2_EVAL_H__

#include "ibex_Function.h"
#include "ibex_Affine2Domain.h"
#include "ibex_Affine2MatrixArray.h"
#include "ibex_FwdAlgorithm.h"
#include "ibex_ExprDomain.h"

namespace ibex {

typedef ExprTemplateDomain<Affine2> ExprAffine2Domain;

/**
 * \ingroup symbolic
 *
 * \brief Evaluate a function with affine form.
 */
class Affine2Eval : public FwdAlgorithm {

public:
	Affine2Eval(const Function& f);

	/**
	 * \brief Run the forward algorithm on the box \a box and return the result as an interval domain.
	 */
	Domain& eval(const IntervalVector& box);

	/**
	 * \brief Run the forward algorithm on the box \a box and return the result as an Affine2 domain.
	 */
	Affine2Domain& eval(const Affine2Vector& box);

	void index_fwd (int  x, int y);
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

protected:

	/**
	 * \brief Run the forward algorithm on the box \a box and return the root node label.
	 */
	void forward(const IntervalVector& box);

	/**
	 * \brief Run the forward algorithm on the box \a box and return the root node label.
	 */
	void forward(const Affine2Vector& box);

	/**
	 * \brief Run the forward algorithm with input node labels and return the result as an interval domain.
	 */
	void forward(const Array<const Domain>& argD, const Array<const Affine2Domain>& argDAF2);

	const Function& f;
	ExprDomain d;
	ExprAffine2Domain af2;
};

/* ============================================================================
 	 	 	 	 	 	 	 implementation
  ============================================================================*/


inline Domain& Affine2Eval::eval(const IntervalVector& box) {
	forward(box);
	return *d.top;
}

inline Affine2Domain& Affine2Eval::eval(const Affine2Vector& box) {
	forward(box);
	return *af2.top;
}

inline void Affine2Eval::index_fwd(int, int ) { /* nothing to do */ }

inline void Affine2Eval::symbol_fwd(int) { /* nothing to do */ }

inline void Affine2Eval::cst_fwd(int y) {
	const ExprConstant& c = (const ExprConstant&) f.node(y);
	switch (c.type()) {
	case Dim::SCALAR:      {
		af2[y].i() = Affine2(c.get_value());
		d[y].i() = c.get_value();
		break;
	}
	case Dim::ROW_VECTOR:
	case Dim::COL_VECTOR: {
		af2[y].v() = Affine2Vector(c.get_vector_value(),false);
		d[y].v() = c.get_vector_value();
		break;
	}
	case Dim::MATRIX: {
		af2[y].m() = Affine2Matrix(c.get_matrix_value());
		d[y].m() = c.get_matrix_value();
		break;
	}
	case Dim::MATRIX_ARRAY:  {
		assert(false); /* impossible */
		break;
	}
	}
}

inline void Affine2Eval::chi_fwd(int x1, int x2, int x3, int y) {
	af2[y].i()=chi(d[x1].i(),af2[x2].i(),af2[x3].i());
	d[y].i()  =chi(d[x1].i(),d[x2].i(),d[x3].i());
}

inline void Affine2Eval::add_fwd(int x1, int x2, int y) {
	af2[y].i()=af2[x1].i()+af2[x2].i();
	d[y].i()=(af2[y].i().itv() & (d[x1].i()+d[x2].i()));
}

inline void Affine2Eval::mul_fwd(int x1, int x2, int y) {
	af2[y].i()=af2[x1].i()*af2[x2].i();
	d[y].i()=(af2[y].i().itv() & (d[x1].i()*d[x2].i()));
}

inline void Affine2Eval::sub_fwd(int x1, int x2, int y) {
	af2[y].i()=af2[x1].i()-af2[x2].i();
	d[y].i()=(af2[y].i().itv() & (d[x1].i()-d[x2].i()));
}

inline void Affine2Eval::div_fwd(int x1, int x2, int y) {
	af2[y].i()=af2[x1].i()*inv(af2[x2].i(),d[x2].i());
	d[y].i()=(af2[y].i().itv() & (d[x1].i()/d[x2].i()));
}

inline void Affine2Eval::max_fwd(int x1, int x2, int y) {
	d[y].i()= max(d[x1].i(),d[x2].i());
	af2[y].i() = Affine2(d[y].i());
}

inline void Affine2Eval::min_fwd(int x1, int x2, int y) {
	d[y].i() = min(d[x1].i(),d[x2].i());
	af2[y].i()= Affine2( d[y].i());
}

inline void Affine2Eval::atan2_fwd(int x1, int x2, int y) {
	d[y].i() = atan2(d[x1].i(),d[x2].i());
	af2[y].i()= Affine2(d[y].i());
}

inline void Affine2Eval::minus_fwd(int x, int y) {
	af2[y].i()=-af2[x].i();
	d[y].i()=(af2[y].i().itv() & (-d[x].i()));
}

inline void Affine2Eval::sign_fwd(int x, int y) {
	af2[y].i()=sign(af2[x].i(),d[x].i());
	d[y].i()=(af2[y].i().itv() & sign(d[x].i()));
}

inline void Affine2Eval::abs_fwd(int x, int y) {
	af2[y].i()=abs(af2[x].i(),d[x].i());
	d[y].i()=(af2[y].i().itv() & abs(d[x].i()));
}

inline void Affine2Eval::power_fwd(int x, int y, int p) {
	af2[y].i()=pow(af2[x].i(),p,d[x].i());
	d[y].i()=(af2[y].i().itv() & pow(d[x].i(),p));
}

inline void Affine2Eval::sqr_fwd(int x, int y) {
	af2[y].i()=sqr(af2[x].i(),d[x].i());
	d[y].i()=(af2[y].i().itv() & sqr(d[x].i()));
}

inline void Affine2Eval::sqrt_fwd(int x, int y) {
	af2[y].i()=sqrt(af2[x].i(),d[x].i());
	d[y].i()=(af2[y].i().itv() & sqrt(d[x].i()));
}

inline void Affine2Eval::exp_fwd(int x, int y) {
	af2[y].i()=exp(af2[x].i(),d[x].i());
	d[y].i()=(af2[y].i().itv() & exp(d[x].i()));
}

inline void Affine2Eval::log_fwd(int x, int y) {
	af2[y].i()=log(af2[x].i(),d[x].i());
	d[y].i()=(af2[y].i().itv() & log(d[x].i()));
}

inline void Affine2Eval::cos_fwd(int x, int y) {
	af2[y].i()=cos(af2[x].i(),d[x].i());
	d[y].i()=(af2[y].i().itv() & cos(d[x].i()));
}

inline void Affine2Eval::sin_fwd(int x, int y) {
	af2[y].i()=sin(af2[x].i(),d[x].i());
	d[y].i()=(af2[y].i().itv() & sin(d[x].i()));
}

inline void Affine2Eval::tan_fwd(int x, int y) {
	af2[y].i()=tan(af2[x].i(),d[x].i());
	d[y].i()=(af2[y].i().itv() & tan(d[x].i()));
}

inline void Affine2Eval::cosh_fwd(int x, int y) {
	af2[y].i()=cosh(af2[x].i(),d[x].i());
	d[y].i()=(af2[y].i().itv() & cosh(d[x].i()));
}

inline void Affine2Eval::sinh_fwd(int x, int y) {
	af2[y].i()=sinh(af2[x].i(),d[x].i());
	d[y].i()=(af2[y].i().itv() & sinh(d[x].i()));
}

inline void Affine2Eval::tanh_fwd(int x, int y) {
	af2[y].i()=tanh(af2[x].i(),d[x].i());
	d[y].i()=(af2[y].i().itv() & tanh(d[x].i()));
}

inline void Affine2Eval::acos_fwd(int x, int y) {
	af2[y].i()=acos(af2[x].i(),d[x].i());
	d[y].i()=(af2[y].i().itv() & acos(d[x].i()));
}

inline void Affine2Eval::asin_fwd(int x, int y) {
	af2[y].i()=asin(af2[x].i(),d[x].i());
	d[y].i()=(af2[y].i().itv() & asin(d[x].i()));
}

inline void Affine2Eval::atan_fwd(int x, int y) {
	af2[y].i()=atan(af2[x].i(),d[x].i());
	d[y].i()=(af2[y].i().itv() & atan(d[x].i()));
}

inline void Affine2Eval::acosh_fwd(int x, int y) {
	d[y].i()=acosh(d[x].i());
	af2[y].i()= Affine2(d[y].i());
}

inline void Affine2Eval::asinh_fwd(int x, int y) {
	d[y].i()=asinh(d[x].i());
	af2[y].i()= Affine2(d[y].i());
}

inline void Affine2Eval::atanh_fwd(int x, int y) {
	d[y].i()=atanh(d[x].i());
	af2[y].i()= Affine2(d[y].i());
}

inline void Affine2Eval::trans_V_fwd(int x, int y) {
	af2[y].v()=af2[x].v();
	d[y].v()=d[x].v();
}

inline void Affine2Eval::trans_M_fwd(int x, int y) {
	af2[y].m()=af2[x].m().transpose();
	d[y].m()=d[x].m().transpose();
}

inline void Affine2Eval::add_V_fwd(int x1, int x2, int y) {
	af2[y].v()=af2[x1].v()+af2[x2].v();
	d[y].v()=(af2[y].v().itv() & (d[x1].v()+d[x2].v()));
}

inline void Affine2Eval::add_M_fwd(int x1, int x2, int y) {
	af2[y].m()=af2[x1].m()+af2[x2].m();
	d[y].m()=(af2[y].m().itv());
	d[y].m() &= (d[x1].m()+d[x2].m());
}

inline void Affine2Eval::mul_SV_fwd(int x1, int x2, int y) {
	af2[y].v()=af2[x1].i()*af2[x2].v();
	d[y].v()=(af2[y].v().itv() & (d[x1].i()*d[x2].v()));
}

inline void Affine2Eval::mul_SM_fwd(int x1, int x2, int y) {
	af2[y].m()=af2[x1].i()*af2[x2].m();
	d[y].m()=(af2[y].m().itv());
	d[y].m() &= (d[x1].i()*d[x2].m());
}

inline void Affine2Eval::mul_VV_fwd(int x1, int x2, int y) {
	af2[y].i()=af2[x1].v()*af2[x2].v();
	d[y].i()=(af2[y].i().itv() & (d[x1].v()*d[x2].v()));
}

inline void Affine2Eval::mul_MV_fwd(int x1, int x2, int y) {
	af2[y].v()=af2[x1].m()*af2[x2].v();
	d[y].v()=(af2[y].v().itv() & (d[x1].m()*d[x2].v()));
}

inline void Affine2Eval::mul_VM_fwd(int x1, int x2, int y) {
	af2[y].v()=af2[x1].v()*af2[x2].m();
	d[y].v()=(af2[y].v().itv() & (d[x1].v()*d[x2].m()));
}

inline void Affine2Eval::mul_MM_fwd(int x1, int x2, int y) {
	af2[y].m()=af2[x1].m()*af2[x2].m();
	d[y].m()=(af2[y].m().itv()) ;
	d[y].m() &=  (d[x1].m()*d[x2].m());
}

inline void Affine2Eval::sub_V_fwd(int x1, int x2, int y) {
	af2[y].v()=af2[x1].v()-af2[x2].v();
	d[y].v()=(af2[y].v().itv() & (d[x1].v()-d[x2].v()));
}

inline void Affine2Eval::sub_M_fwd(int x1, int x2, int y) {
	af2[y].m()=af2[x1].m()-af2[x2].m();
	d[y].m() = af2[y].m().itv();
	d[y].m() &= (d[x1].m()-d[x2].m());
}

} // namespace ibex


#endif /* __IBEX_AFFINE2_EVAL_H__ */
