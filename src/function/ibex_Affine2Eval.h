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
#include "ibex_Affine2MatrixArray.h"
#include "ibex_FwdAlgorithm.h"

namespace ibex {

/**
 * \ingroup symbolic
 *
 * \brief Evaluate a function with affine form.
 */
class Affine2Eval : public FwdAlgorithm {

public:
	/**
	 * \brief Run the forward algorithm on the box \a box and return the result as an interval domain.
	 */
	Domain& eval(const Function& f, const IntervalVector& box) const;

	/**
	 * \brief Run the forward algorithm on the box \a box and return the result as an Affine2 domain.
	 */
	Affine2Domain& eval(const Function& f, const Affine2Vector& box) const;

	/**
	 * \brief Run the forward algorithm on the box \a box and return the root node label.
	 */
	ExprLabel& eval_label(const Function& f, const IntervalVector& box) const;

	/**
	 * \brief Run the forward algorithm on the box \a box and return the root node label.
	 */
	ExprLabel& eval_label(const Function& f, const Affine2Vector& box) const;

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

inline Domain& Affine2Eval::eval(const Function& f, ExprLabel** e) const {
	return *eval_label(f,e).d;
}

inline Domain& Affine2Eval::eval(const Function& f,const IntervalVector& box) const {
	return *(eval_label(f,box)).d;
}

inline Affine2Domain& Affine2Eval::eval(const Function& f,const Affine2Vector& box) const {
	return *(eval_label(f,box)).af2;
}

inline void Affine2Eval::index_fwd(const ExprIndex& , const ExprLabel& , ExprLabel& ) { /* nothing to do */ }

inline void Affine2Eval::symbol_fwd(const ExprSymbol& , ExprLabel& ) { /* nothing to do */ }

inline void Affine2Eval::cst_fwd(const ExprConstant& c, ExprLabel& y) {
	switch (c.type()) {
	case Dim::SCALAR:      {
		y.af2->i() = Affine2(c.get_value());
		y.d->i() = c.get_value();
		break;
	}
	case Dim::ROW_VECTOR:
	case Dim::COL_VECTOR: {
		y.af2->v() = Affine2Vector(c.get_vector_value(),false);
		y.d->v() = c.get_vector_value();
		break;
	}
	case Dim::MATRIX: {
		y.af2->m() = Affine2Matrix(c.get_matrix_value());
		y.d->m() = c.get_matrix_value();
		break;
	}
	case Dim::MATRIX_ARRAY:  {
		assert(false); /* impossible */
		break;
	}
	}
}

inline void Affine2Eval::apply_fwd(const ExprApply& a, ExprLabel** x, ExprLabel& y)                          {
	ExprLabel& tmp = eval_label(a.func,x);
	*y.af2 = *tmp.af2;
	*y.d = *tmp.d;
}
inline void Affine2Eval::chi_fwd(const ExprChi&,  const ExprLabel& x1, const ExprLabel& x2, const ExprLabel& x3, ExprLabel& y)                         {
	y.af2->i()=chi(x1.d->i(),x2.af2->i(),x3.af2->i());
	y.d->i()  =chi(x1.d->i(),x2.d->i(),x3.d->i());
}
inline void Affine2Eval::add_fwd(const ExprAdd&, const ExprLabel& x1, const ExprLabel& x2, ExprLabel& y)     {
	y.af2->i()=x1.af2->i()+x2.af2->i();
	y.d->i()=(y.af2->i().itv() & (x1.d->i()+x2.d->i()));
}
inline void Affine2Eval::mul_fwd(const ExprMul&, const ExprLabel& x1, const ExprLabel& x2, ExprLabel& y)     {
	y.af2->i()=x1.af2->i()*x2.af2->i();
	y.d->i()=(y.af2->i().itv() & (x1.d->i()*x2.d->i()));
}
inline void Affine2Eval::sub_fwd(const ExprSub&, const ExprLabel& x1, const ExprLabel& x2, ExprLabel& y)     {
	y.af2->i()=x1.af2->i()-x2.af2->i();
	y.d->i()=(y.af2->i().itv() & (x1.d->i()-x2.d->i()));
}
inline void Affine2Eval::div_fwd(const ExprDiv&, const ExprLabel& x1, const ExprLabel& x2, ExprLabel& y)     {
	y.af2->i()=x1.af2->i()*inv(x2.af2->i(),x2.d->i());
	y.d->i()=(y.af2->i().itv() & (x1.d->i()/x2.d->i()));
}
inline void Affine2Eval::max_fwd(const ExprMax&, const ExprLabel& x1, const ExprLabel& x2, ExprLabel& y)     {
	y.d->i()= max(x1.d->i(),x2.d->i());
	y.af2->i() = Affine2(y.d->i());
}
inline void Affine2Eval::min_fwd(const ExprMin&, const ExprLabel& x1, const ExprLabel& x2, ExprLabel& y)     {
	y.d->i() = min(x1.d->i(),x2.d->i());
	y.af2->i()= Affine2( y.d->i());
}
inline void Affine2Eval::atan2_fwd(const ExprAtan2&, const ExprLabel& x1, const ExprLabel& x2, ExprLabel& y) {
	y.d->i() = atan2(x1.d->i(),x2.d->i());
	y.af2->i()= Affine2(y.d->i());
}
inline void Affine2Eval::minus_fwd(const ExprMinus&, const ExprLabel& x, ExprLabel& y)                       {
	y.af2->i()=-x.af2->i();
	y.d->i()=(y.af2->i().itv() & (-x.d->i()));
}
inline void Affine2Eval::sign_fwd(const ExprSign&, const ExprLabel& x, ExprLabel& y)                         {
	y.af2->i()=sign(x.af2->i(),x.d->i());
	y.d->i()=(y.af2->i().itv() & sign(x.d->i()));
}
inline void Affine2Eval::abs_fwd(const ExprAbs&, const ExprLabel& x, ExprLabel& y)                           {
	y.af2->i()=abs(x.af2->i(),x.d->i());
	y.d->i()=(y.af2->i().itv() & abs(x.d->i()));
}
inline void Affine2Eval::power_fwd(const ExprPower& p, const ExprLabel& x, ExprLabel& y)                     {
	y.af2->i()=pow(x.af2->i(),p.expon,x.d->i());
	y.d->i()=(y.af2->i().itv() & pow(x.d->i(),p.expon));
}
inline void Affine2Eval::sqr_fwd(const ExprSqr&, const ExprLabel& x, ExprLabel& y)                           {
	y.af2->i()=sqr(x.af2->i(),x.d->i());
	y.d->i()=(y.af2->i().itv() & sqr(x.d->i()));
}
inline void Affine2Eval::sqrt_fwd(const ExprSqrt&, const ExprLabel& x, ExprLabel& y)                         {
	y.af2->i()=sqrt(x.af2->i(),x.d->i());
	y.d->i()=(y.af2->i().itv() & sqrt(x.d->i()));
}
inline void Affine2Eval::exp_fwd(const ExprExp&, const ExprLabel& x, ExprLabel& y)                           {
	y.af2->i()=exp(x.af2->i(),x.d->i());
	y.d->i()=(y.af2->i().itv() & exp(x.d->i()));
}
inline void Affine2Eval::log_fwd(const ExprLog&, const ExprLabel& x, ExprLabel& y)                           {
	y.af2->i()=log(x.af2->i(),x.d->i());
	y.d->i()=(y.af2->i().itv() & log(x.d->i()));
}
inline void Affine2Eval::cos_fwd(const ExprCos&, const ExprLabel& x, ExprLabel& y)                           {
	y.af2->i()=cos(x.af2->i(),x.d->i());
	y.d->i()=(y.af2->i().itv() & cos(x.d->i()));
}
inline void Affine2Eval::sin_fwd(const ExprSin&, const ExprLabel& x, ExprLabel& y)                           {
	y.af2->i()=sin(x.af2->i(),x.d->i());
	y.d->i()=(y.af2->i().itv() & sin(x.d->i()));
}
inline void Affine2Eval::tan_fwd(const ExprTan&, const ExprLabel& x, ExprLabel& y)                           {
	y.af2->i()=tan(x.af2->i(),x.d->i());
	y.d->i()=(y.af2->i().itv() & tan(x.d->i()));
}
inline void Affine2Eval::cosh_fwd(const ExprCosh&, const ExprLabel& x, ExprLabel& y)                         {
	y.af2->i()=cosh(x.af2->i(),x.d->i());
	y.d->i()=(y.af2->i().itv() & cosh(x.d->i()));
}
inline void Affine2Eval::sinh_fwd(const ExprSinh&, const ExprLabel& x, ExprLabel& y)                         {
	y.af2->i()=sinh(x.af2->i(),x.d->i());
	y.d->i()=(y.af2->i().itv() & sinh(x.d->i()));
}
inline void Affine2Eval::tanh_fwd(const ExprTanh&, const ExprLabel& x, ExprLabel& y)                         {
	y.af2->i()=tanh(x.af2->i(),x.d->i());
	y.d->i()=(y.af2->i().itv() & tanh(x.d->i()));
}
inline void Affine2Eval::acos_fwd(const ExprAcos&, const ExprLabel& x, ExprLabel& y)                         {
	y.af2->i()=acos(x.af2->i(),x.d->i());
	y.d->i()=(y.af2->i().itv() & acos(x.d->i()));
}
inline void Affine2Eval::asin_fwd(const ExprAsin&, const ExprLabel& x, ExprLabel& y)                         {
	y.af2->i()=asin(x.af2->i(),x.d->i());
	y.d->i()=(y.af2->i().itv() & asin(x.d->i()));
}
inline void Affine2Eval::atan_fwd(const ExprAtan&, const ExprLabel& x, ExprLabel& y)                         {
	y.af2->i()=atan(x.af2->i(),x.d->i());
	y.d->i()=(y.af2->i().itv() & atan(x.d->i()));
}
inline void Affine2Eval::acosh_fwd(const ExprAcosh&, const ExprLabel& x, ExprLabel& y)                       {
	y.d->i()=acosh(x.d->i());
	y.af2->i()= Affine2(y.d->i());
}
inline void Affine2Eval::asinh_fwd(const ExprAsinh&, const ExprLabel& x, ExprLabel& y)                       {
	y.d->i()=asinh(x.d->i());
	y.af2->i()= Affine2(y.d->i());
}
inline void Affine2Eval::atanh_fwd(const ExprAtanh&, const ExprLabel& x, ExprLabel& y)                       {
	y.d->i()=atanh(x.d->i());
	y.af2->i()= Affine2(y.d->i());
}



inline void Affine2Eval::trans_V_fwd(const ExprTrans&, const ExprLabel& x, ExprLabel& y)                     {
	y.af2->v()=x.af2->v();
	y.d->v()=x.d->v();
}
inline void Affine2Eval::trans_M_fwd(const ExprTrans&, const ExprLabel& x, ExprLabel& y)                     {
	y.af2->m()=x.af2->m().transpose();
	y.d->m()=x.d->m().transpose();
}
inline void Affine2Eval::add_V_fwd(const ExprAdd&, const ExprLabel& x1, const ExprLabel& x2, ExprLabel& y)   {
	y.af2->v()=x1.af2->v()+x2.af2->v();
	y.d->v()=(y.af2->v().itv() & (x1.d->v()+x2.d->v()));
}
inline void Affine2Eval::add_M_fwd(const ExprAdd&, const ExprLabel& x1, const ExprLabel& x2, ExprLabel& y)   {
	y.af2->m()=x1.af2->m()+x2.af2->m();
	y.d->m()=(y.af2->m().itv());
	y.d->m() &= (x1.d->m()+x2.d->m());
}
inline void Affine2Eval::mul_SV_fwd(const ExprMul&, const ExprLabel& x1, const ExprLabel& x2, ExprLabel& y)  {
	y.af2->v()=x1.af2->i()*x2.af2->v();
	y.d->v()=(y.af2->v().itv() & (x1.d->i()*x2.d->v()));
}
inline void Affine2Eval::mul_SM_fwd(const ExprMul&, const ExprLabel& x1, const ExprLabel& x2, ExprLabel& y)  {
	y.af2->m()=x1.af2->i()*x2.af2->m();
	y.d->m()=(y.af2->m().itv());
	y.d->m() &= (x1.d->i()*x2.d->m());
}
inline void Affine2Eval::mul_VV_fwd(const ExprMul&, const ExprLabel& x1, const ExprLabel& x2, ExprLabel& y)  {
	y.af2->i()=x1.af2->v()*x2.af2->v();
	y.d->i()=(y.af2->i().itv() & (x1.d->v()*x2.d->v()));
}
inline void Affine2Eval::mul_MV_fwd(const ExprMul&, const ExprLabel& x1, const ExprLabel& x2, ExprLabel& y)  {
	y.af2->v()=x1.af2->m()*x2.af2->v();
	y.d->v()=(y.af2->v().itv() & (x1.d->m()*x2.d->v()));
}
inline void Affine2Eval::mul_VM_fwd(const ExprMul&, const ExprLabel& x1, const ExprLabel& x2, ExprLabel& y)  {
	y.af2->v()=x1.af2->v()*x2.af2->m();
	y.d->v()=(y.af2->v().itv() & (x1.d->v()*x2.d->m()));
}
inline void Affine2Eval::mul_MM_fwd(const ExprMul&, const ExprLabel& x1, const ExprLabel& x2, ExprLabel& y)  {
	y.af2->m()=x1.af2->m()*x2.af2->m();
	y.d->m()=(y.af2->m().itv()) ;
	y.d->m() &=  (x1.d->m()*x2.d->m());
}
inline void Affine2Eval::sub_V_fwd(const ExprSub&, const ExprLabel& x1, const ExprLabel& x2, ExprLabel& y)   {
	y.af2->v()=x1.af2->v()-x2.af2->v();
	y.d->v()=(y.af2->v().itv() & (x1.d->v()-x2.d->v()));
}
inline void Affine2Eval::sub_M_fwd(const ExprSub&, const ExprLabel& x1, const ExprLabel& x2, ExprLabel& y)   {
	y.af2->m()=x1.af2->m()-x2.af2->m();
	y.d->m() = y.af2->m().itv();
	y.d->m() &= (x1.d->m()-x2.d->m());
}

} // namespace ibex


#endif /* __IBEX_AFFINE2_EVAL_H__ */
