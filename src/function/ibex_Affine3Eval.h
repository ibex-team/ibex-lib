/* ============================================================================
 * I B E X - Evaluation of Affine3 forms
 * ============================================================================
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Jordan Ninin
 * Created     : April 08, 2013
 * ---------------------------------------------------------------------------- */

#ifndef __IBEX_Affine3_EVAL_H__
#define __IBEX_Affine3_EVAL_H__

#include "ibex_Function.h"
#include "ibex_AffineMatrixArray.h"
#include "ibex_AffineDomain.h"
#include "ibex_Domain.h"
#include "ibex_FwdAlgorithm.h"

namespace ibex {

/**
 * \ingroup symbolic
 *
 * \brief Evaluate a function with affine form.
 */
class Affine3Eval : public FwdAlgorithm {

public:
	/**
	 * \brief Run the forward algorithm on the box \a box and return the result as an interval domain.
	 */
	Domain& eval(const Function& f, const IntervalVector& box) const;

	/**
	 * \brief Run the forward algorithm on the box \a box and return the result as an Affine3 domain.
	 */
	Affine3Domain& eval(const Function& f, const Affine3Vector& box) const;

	/**
	 * \brief Run the forward algorithm on the box \a box and return the root node label.
	 */
	ExprLabel& eval_label(const Function& f, const IntervalVector& box) const;

	/**
	 * \brief Run the forward algorithm on the box \a box and return the root node label.
	 */
	ExprLabel& eval_label(const Function& f, const Affine3Vector& box) const;

	/**
	 * \brief Run the forward algorithm with input node labels and return the result as an interval domain.
	 */
	ExprLabel& eval_label(const Function& f, ExprLabel** d) const;

	/**
	 * \brief Run the forward algorithm with input node labels and return the root node label.
	 */
	Domain& eval(const Function& f , ExprLabel** d) const;

	void index_fwd(const ExprIndex&, const ExprLabel& x, ExprLabel& y);
	void vector_fwd(const ExprVector&, const ExprLabel** compL, ExprLabel& y);
	void cst_fwd(const ExprConstant&, ExprLabel& y);
	void symbol_fwd(const ExprSymbol&, ExprLabel& y);
	void apply_fwd(const ExprApply&, ExprLabel** x, ExprLabel& y);
	void chi_fwd(const ExprChi&, const ExprLabel& x1, const ExprLabel& x2, const ExprLabel& x3, ExprLabel& y);
	void add_fwd(const ExprAdd&, const ExprLabel& x1, const ExprLabel& x2, ExprLabel& y);
	void mul_fwd(const ExprMul&, const ExprLabel& x1, const ExprLabel& x2, ExprLabel& y);
	void sub_fwd(const ExprSub&, const ExprLabel& x1, const ExprLabel& x2, ExprLabel& y);
	void div_fwd(const ExprDiv&, const ExprLabel& x1, const ExprLabel& x2, ExprLabel& y);
	void max_fwd(const ExprMax&, const ExprLabel& x1, const ExprLabel& x2, ExprLabel& y);
	void min_fwd(const ExprMin&, const ExprLabel& x1, const ExprLabel& x2, ExprLabel& y);
	void atan2_fwd(const ExprAtan2&, const ExprLabel& x1, const ExprLabel& x2, ExprLabel& y);
	void minus_fwd(const ExprMinus&, const ExprLabel& x, ExprLabel& y);
	void trans_V_fwd(const ExprTrans&, const ExprLabel& x, ExprLabel& y);
	void trans_M_fwd(const ExprTrans&, const ExprLabel& x, ExprLabel& y);
	void sign_fwd(const ExprSign&, const ExprLabel& x, ExprLabel& y);
	void abs_fwd(const ExprAbs&, const ExprLabel& x, ExprLabel& y);
	void power_fwd(const ExprPower& p, const ExprLabel& x, ExprLabel& y);
	void sqr_fwd(const ExprSqr&, const ExprLabel& x, ExprLabel& y);
	void sqrt_fwd(const ExprSqrt&, const ExprLabel& x, ExprLabel& y);
	void exp_fwd(const ExprExp&, const ExprLabel& x, ExprLabel& y);
	void log_fwd(const ExprLog&, const ExprLabel& x, ExprLabel& y);
	void cos_fwd(const ExprCos&, const ExprLabel& x, ExprLabel& y);
	void sin_fwd(const ExprSin&, const ExprLabel& x, ExprLabel& y);
	void tan_fwd(const ExprTan&, const ExprLabel& x, ExprLabel& y);
	void cosh_fwd(const ExprCosh&, const ExprLabel& x, ExprLabel& y);
	void sinh_fwd(const ExprSinh&, const ExprLabel& x, ExprLabel& y);
	void tanh_fwd(const ExprTanh&, const ExprLabel& x, ExprLabel& y);
	void acos_fwd(const ExprAcos&, const ExprLabel& x, ExprLabel& y);
	void asin_fwd(const ExprAsin&, const ExprLabel& x, ExprLabel& y);
	void atan_fwd(const ExprAtan&, const ExprLabel& x, ExprLabel& y);
	void acosh_fwd(const ExprAcosh&, const ExprLabel& x, ExprLabel& y);
	void asinh_fwd(const ExprAsinh&, const ExprLabel& x, ExprLabel& y);
	void atanh_fwd(const ExprAtanh&, const ExprLabel& x, ExprLabel& y);

	void add_V_fwd(const ExprAdd&, const ExprLabel& x1, const ExprLabel& x2, ExprLabel& y);
	void add_M_fwd(const ExprAdd&, const ExprLabel& x1, const ExprLabel& x2, ExprLabel& y);
	void mul_SV_fwd(const ExprMul&, const ExprLabel& x1, const ExprLabel& x2, ExprLabel& y);
	void mul_SM_fwd(const ExprMul&, const ExprLabel& x1, const ExprLabel& x2, ExprLabel& y);
	void mul_VV_fwd(const ExprMul&, const ExprLabel& x1, const ExprLabel& x2, ExprLabel& y);
	void mul_MV_fwd(const ExprMul&, const ExprLabel& x1, const ExprLabel& x2, ExprLabel& y);
	void mul_VM_fwd(const ExprMul&, const ExprLabel& x1, const ExprLabel& x2, ExprLabel& y);
	void mul_MM_fwd(const ExprMul&, const ExprLabel& x1, const ExprLabel& x2, ExprLabel& y);
	void sub_V_fwd(const ExprSub&, const ExprLabel& x1, const ExprLabel& x2, ExprLabel& y);
	void sub_M_fwd(const ExprSub&, const ExprLabel& x1, const ExprLabel& x2, ExprLabel& y);

};

/* ============================================================================
 	 	 	 	 	 	 	 implementation
  ============================================================================*/

inline Domain& Affine3Eval::eval(const Function& f, ExprLabel** e) const {
	return *eval_label(f,e).d;
}

inline Domain& Affine3Eval::eval(const Function& f,const IntervalVector& box) const {
	return *(eval_label(f,box)).d;
}

inline Affine3Domain& Affine3Eval::eval(const Function& f,const Affine3Vector& box) const {
	return *(eval_label(f,box)).af3;
}

inline void Affine3Eval::index_fwd(const ExprIndex& , const ExprLabel& , ExprLabel& ) { /* nothing to do */ }

inline void Affine3Eval::symbol_fwd(const ExprSymbol& , ExprLabel& ) { /* nothing to do */ }

inline void Affine3Eval::cst_fwd(const ExprConstant& c, ExprLabel& y) {
	switch (c.type()) {
	case Dim::SCALAR:      {
		y.af3->i() = Affine3(c.get_value());
		y.d->i() = c.get_value();
		break;
	}
	case Dim::ROW_VECTOR:
	case Dim::COL_VECTOR: {
		y.af3->v() = Affine3Vector(c.get_vector_value(),false);
		y.d->v() = c.get_vector_value();
		break;
	}
	case Dim::MATRIX: {
		y.af3->m() = Affine3Matrix(c.get_matrix_value());
		y.d->m() = c.get_matrix_value();
		break;
	}
	case Dim::MATRIX_ARRAY:  {
		assert(false); /* impossible */
		break;
	}
	}
}

inline void Affine3Eval::apply_fwd(const ExprApply& a, ExprLabel** x, ExprLabel& y)                          {
	ExprLabel& tmp = eval_label(a.func,x);
	*y.af3 = (*tmp.af3);
	*y.d = (*tmp.d);
}
inline void Affine3Eval::chi_fwd(const ExprChi&,  const ExprLabel& x1, const ExprLabel& x2, const ExprLabel& x3, ExprLabel& y)                         {
	y.af3->i()=chi(x1.d->i(),x2.af3->i(),x3.af3->i());
	y.d->i()  =chi(x1.d->i(),x2.d->i(),x3.d->i());
}
inline void Affine3Eval::add_fwd(const ExprAdd&, const ExprLabel& x1, const ExprLabel& x2, ExprLabel& y)     {
	y.af3->i()=x1.af3->i()+x2.af3->i();
	y.d->i()=(y.af3->i().itv() & (x1.d->i()+x2.d->i()));
}
inline void Affine3Eval::mul_fwd(const ExprMul&, const ExprLabel& x1, const ExprLabel& x2, ExprLabel& y)     {
	y.af3->i()=x1.af3->i()*x2.af3->i();
	y.d->i()=(y.af3->i().itv() & (x1.d->i()*x2.d->i()));
}
inline void Affine3Eval::sub_fwd(const ExprSub&, const ExprLabel& x1, const ExprLabel& x2, ExprLabel& y)     {
	y.af3->i()=x1.af3->i()-x2.af3->i();
	y.d->i()=(y.af3->i().itv() & (x1.d->i()-x2.d->i()));
}
inline void Affine3Eval::div_fwd(const ExprDiv&, const ExprLabel& x1, const ExprLabel& x2, ExprLabel& y)     {
	y.af3->i()=x2.af3->i();
	y.af3->i().invA(x2.d->i());
	y.af3->i()*=x1.af3->i();
	y.d->i()=(y.af3->i().itv() & (x1.d->i()/x2.d->i()));
}
inline void Affine3Eval::max_fwd(const ExprMax&, const ExprLabel& x1, const ExprLabel& x2, ExprLabel& y)     {
	y.d->i()= max(x1.d->i(),x2.d->i());
	y.af3->i() = Affine3(y.d->i());
}
inline void Affine3Eval::min_fwd(const ExprMin&, const ExprLabel& x1, const ExprLabel& x2, ExprLabel& y)     {
	y.d->i() = min(x1.d->i(),x2.d->i());
	y.af3->i()= Affine3( y.d->i());
}
inline void Affine3Eval::atan2_fwd(const ExprAtan2&, const ExprLabel& x1, const ExprLabel& x2, ExprLabel& y) {
	y.d->i() = atan2(x1.d->i(),x2.d->i());
	y.af3->i()= Affine3(y.d->i());
}
inline void Affine3Eval::minus_fwd(const ExprMinus&, const ExprLabel& x, ExprLabel& y)                       {
	y.af3->i()=-x.af3->i();
	y.d->i()=(y.af3->i().itv() & (-x.d->i()));
}
inline void Affine3Eval::sign_fwd(const ExprSign&, const ExprLabel& x, ExprLabel& y)                         {
	y.af3->i()=sign(x.af3->i());
	y.af3->i().signA(x.d->i());
	y.d->i()=(y.af3->i().itv() & sign(x.d->i()));
}
inline void Affine3Eval::abs_fwd(const ExprAbs&, const ExprLabel& x, ExprLabel& y)                           {
	y.af3->i()=x.af3->i();
	y.af3->i().absA(x.d->i());
	y.d->i()=(y.af3->i().itv() & abs(x.d->i()));
}
inline void Affine3Eval::power_fwd(const ExprPower& p, const ExprLabel& x, ExprLabel& y)                     {
	y.af3->i()=x.af3->i();
	y.af3->i().powerA(p.expon,x.d->i());
	y.d->i()=(y.af3->i().itv() & pow(x.d->i(),p.expon));
}
inline void Affine3Eval::sqr_fwd(const ExprSqr&, const ExprLabel& x, ExprLabel& y)                           {
	y.af3->i()=x.af3->i();
	y.af3->i().sqrA(x.d->i());
	y.d->i()=(y.af3->i().itv() & sqr(x.d->i()));
}
inline void Affine3Eval::sqrt_fwd(const ExprSqrt&, const ExprLabel& x, ExprLabel& y)                         {
	y.af3->i()=x.af3->i();
	y.af3->i().sqrtA(x.d->i());
	y.d->i()=(y.af3->i().itv() & sqrt(x.d->i()));
}
inline void Affine3Eval::exp_fwd(const ExprExp&, const ExprLabel& x, ExprLabel& y)                           {
	y.af3->i()=x.af3->i();
	y.af3->i().expA(x.d->i());
	y.d->i()=(y.af3->i().itv() & exp(x.d->i()));
}
inline void Affine3Eval::log_fwd(const ExprLog&, const ExprLabel& x, ExprLabel& y)                           {
	y.af3->i()=x.af3->i();
	y.af3->i().logA(x.d->i());
	y.d->i()=(y.af3->i().itv() & log(x.d->i()));
}
inline void Affine3Eval::cos_fwd(const ExprCos&, const ExprLabel& x, ExprLabel& y)                           {
	y.af3->i()=x.af3->i();
	y.af3->i().cosA(x.d->i());
	y.d->i()=(y.af3->i().itv() & cos(x.d->i()));
}
inline void Affine3Eval::sin_fwd(const ExprSin&, const ExprLabel& x, ExprLabel& y)                           {
	y.af3->i()=x.af3->i();
	y.af3->i().sinA(x.d->i());
	y.d->i()=(y.af3->i().itv() & sin(x.d->i()));
}
inline void Affine3Eval::tan_fwd(const ExprTan&, const ExprLabel& x, ExprLabel& y)                           {
	y.af3->i()=x.af3->i();
	y.af3->i().tanA(x.d->i());
	y.d->i()=(y.af3->i().itv() & tan(x.d->i()));
}
inline void Affine3Eval::cosh_fwd(const ExprCosh&, const ExprLabel& x, ExprLabel& y)                         {
	y.af3->i()=x.af3->i();
	y.af3->i().coshA(x.d->i());
	y.d->i()=(y.af3->i().itv() & cosh(x.d->i()));
}
inline void Affine3Eval::sinh_fwd(const ExprSinh&, const ExprLabel& x, ExprLabel& y)                         {
	y.af3->i()=x.af3->i();
	y.af3->i().sinhA(x.d->i());
	y.d->i()=(y.af3->i().itv() & sinh(x.d->i()));
}
inline void Affine3Eval::tanh_fwd(const ExprTanh&, const ExprLabel& x, ExprLabel& y)                         {
	y.af3->i()=x.af3->i();
	y.af3->i().tanhA(x.d->i());
	y.d->i()=(y.af3->i().itv() & tanh(x.d->i()));
}
inline void Affine3Eval::acos_fwd(const ExprAcos&, const ExprLabel& x, ExprLabel& y)                         {
	y.af3->i()=x.af3->i();
	y.af3->i().acosA(x.d->i());
	y.d->i()=(y.af3->i().itv() & acos(x.d->i()));
}
inline void Affine3Eval::asin_fwd(const ExprAsin&, const ExprLabel& x, ExprLabel& y)                         {
	y.af3->i()=x.af3->i();
	y.af3->i().asinA(x.d->i());
	y.d->i()=(y.af3->i().itv() & asin(x.d->i()));
}
inline void Affine3Eval::atan_fwd(const ExprAtan&, const ExprLabel& x, ExprLabel& y)                         {
	y.af3->i()=x.af3->i();
	y.af3->i().atanA(x.d->i());
	y.d->i()=(y.af3->i().itv() & atan(x.d->i()));
}
inline void Affine3Eval::acosh_fwd(const ExprAcosh&, const ExprLabel& x, ExprLabel& y)                       {
	y.d->i()=acosh(x.d->i());
	y.af3->i()= Affine3(y.d->i());
}
inline void Affine3Eval::asinh_fwd(const ExprAsinh&, const ExprLabel& x, ExprLabel& y)                       {
	y.d->i()=asinh(x.d->i());
	y.af3->i()= Affine3(y.d->i());
}
inline void Affine3Eval::atanh_fwd(const ExprAtanh&, const ExprLabel& x, ExprLabel& y)                       {
	y.d->i()=atanh(x.d->i());
	y.af3->i()= Affine3(y.d->i());
}



inline void Affine3Eval::trans_V_fwd(const ExprTrans&, const ExprLabel& x, ExprLabel& y)                     {
	y.af3->v()=x.af3->v();
	y.d->v()=x.d->v();
}
inline void Affine3Eval::trans_M_fwd(const ExprTrans&, const ExprLabel& x, ExprLabel& y)                     {
	y.af3->m()=x.af3->m().transpose();
	y.d->m()=x.d->m().transpose();
}
inline void Affine3Eval::add_V_fwd(const ExprAdd&, const ExprLabel& x1, const ExprLabel& x2, ExprLabel& y)   {
	y.af3->v()=x1.af3->v()+x2.af3->v();
	y.d->v()=(y.af3->v().itv() & (x1.d->v()+x2.d->v()));
}
inline void Affine3Eval::add_M_fwd(const ExprAdd&, const ExprLabel& x1, const ExprLabel& x2, ExprLabel& y)   {
	y.af3->m()=x1.af3->m()+x2.af3->m();
	y.d->m()=(y.af3->m().itv());
	y.d->m() &= (x1.d->m()+x2.d->m());
}
inline void Affine3Eval::mul_SV_fwd(const ExprMul&, const ExprLabel& x1, const ExprLabel& x2, ExprLabel& y)  {
	y.af3->v()=x1.af3->i()*x2.af3->v();
	y.d->v()=(y.af3->v().itv() & (x1.d->i()*x2.d->v()));
}
inline void Affine3Eval::mul_SM_fwd(const ExprMul&, const ExprLabel& x1, const ExprLabel& x2, ExprLabel& y)  {
	y.af3->m()=x1.af3->i()*x2.af3->m();
	y.d->m()=(y.af3->m().itv());
	y.d->m() &= (x1.d->i()*x2.d->m());
}
inline void Affine3Eval::mul_VV_fwd(const ExprMul&, const ExprLabel& x1, const ExprLabel& x2, ExprLabel& y)  {
	y.af3->i()=x1.af3->v()*x2.af3->v();
	y.d->i()=(y.af3->i().itv() & (x1.d->v()*x2.d->v()));
}
inline void Affine3Eval::mul_MV_fwd(const ExprMul&, const ExprLabel& x1, const ExprLabel& x2, ExprLabel& y)  {
	y.af3->v()=x1.af3->m()*x2.af3->v();
	y.d->v()=(y.af3->v().itv() & (x1.d->m()*x2.d->v()));
}
inline void Affine3Eval::mul_VM_fwd(const ExprMul&, const ExprLabel& x1, const ExprLabel& x2, ExprLabel& y)  {
	y.af3->v()=x1.af3->v()*x2.af3->m();
	y.d->v()=(y.af3->v().itv() & (x1.d->v()*x2.d->m()));
}
inline void Affine3Eval::mul_MM_fwd(const ExprMul&, const ExprLabel& x1, const ExprLabel& x2, ExprLabel& y)  {
	y.af3->m()=x1.af3->m()*x2.af3->m();
	y.d->m()=(y.af3->m().itv()) ;
	y.d->m() &=  (x1.d->m()*x2.d->m());
}
inline void Affine3Eval::sub_V_fwd(const ExprSub&, const ExprLabel& x1, const ExprLabel& x2, ExprLabel& y)   {
	y.af3->v()=x1.af3->v()-x2.af3->v();
	y.d->v()=(y.af3->v().itv() & (x1.d->v()-x2.d->v()));
}
inline void Affine3Eval::sub_M_fwd(const ExprSub&, const ExprLabel& x1, const ExprLabel& x2, ExprLabel& y)   {
	y.af3->m()=x1.af3->m()-x2.af3->m();
	y.d->m() = y.af3->m().itv();
	y.d->m() &= (x1.d->m()-x2.d->m());
}

} // namespace ibex


#endif /* __IBEX_Affine3_EVAL_H__ */
