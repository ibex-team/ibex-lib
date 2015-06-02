/* ============================================================================
 * I B E X - Evaluation of AffineLin forms
 * ============================================================================
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Jordan Ninin
 * Created     : April 08, 2013
 * ---------------------------------------------------------------------------- */

#ifndef __IBEX_AffineLin_EVAL_H__
#define __IBEX_AffineLin_EVAL_H__

#include "ibex_Function.h"
#include "ibex_Affine2MatrixArray.h"
#include "ibex_FwdAlgorithm.h"

namespace ibex {

/**
 * \ingroup symbolic
 *
 * \brief Evaluate a function with affine form.
 */
class AffineLinEval : public FwdAlgorithm {

public:
	/**
	 * \brief Run the forward algorithm on the box \a box and return the result as an interval domain.
	 */
	Domain& eval(const Function& f, const IntervalVector& box) const;

	/**
	 * \brief Run the forward algorithm on the box \a box and return the result as an AffineLin domain.
	 */
	AffineLinDomain& eval(const Function& f, const AffineLinVector& box) const;

	/**
	 * \brief Run the forward algorithm on the box \a box and return the root node label.
	 */
	ExprLabel& eval_label(const Function& f, const IntervalVector& box) const;

	/**
	 * \brief Run the forward algorithm on the box \a box and return the root node label.
	 */
	ExprLabel& eval_label(const Function& f, const AffineLinVector& box) const;

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

inline Domain& AffineLinEval::eval(const Function& f, ExprLabel** e) const {
	return *eval_label(f,e).d;
}

inline Domain& AffineLinEval::eval(const Function& f,const IntervalVector& box) const {
	return *(eval_label(f,box)).d;
}

inline AffineLinDomain& AffineLinEval::eval(const Function& f,const AffineLinVector& box) const {
	return *(eval_label(f,box)).af_lin;
}

inline void AffineLinEval::index_fwd(const ExprIndex& , const ExprLabel& , ExprLabel& ) { /* nothing to do */ }

inline void AffineLinEval::symbol_fwd(const ExprSymbol& , ExprLabel& ) { /* nothing to do */ }

inline void AffineLinEval::cst_fwd(const ExprConstant& c, ExprLabel& y) {
	switch (c.type()) {
	case Dim::SCALAR:      {
		y.af_lin->i() = AffineLin(c.get_value());
		y.d->i() = c.get_value();
		break;
	}
	case Dim::ROW_VECTOR:
	case Dim::COL_VECTOR: {
		y.af_lin->v() = AffineLinVector(c.get_vector_value(),false);
		y.d->v() = c.get_vector_value();
		break;
	}
	case Dim::MATRIX: {
		y.af_lin->m() = AffineLinMatrix(c.get_matrix_value());
		y.d->m() = c.get_matrix_value();
		break;
	}
	case Dim::MATRIX_ARRAY:  {
		assert(false); /* impossible */
		break;
	}
	}
}

inline void AffineLinEval::apply_fwd(const ExprApply& a, ExprLabel** x, ExprLabel& y)                          {
	ExprLabel& tmp = eval_label(a.func,x);
	*y.af_lin = *tmp.af_lin;
	*y.d = *tmp.d;
}
inline void AffineLinEval::chi_fwd(const ExprChi&,  const ExprLabel& x1, const ExprLabel& x2, const ExprLabel& x3, ExprLabel& y)                         {
	y.af_lin->i()=chi(x1.d->i(),x2.af_lin->i(),x3.af_lin->i());
	y.d->i()  =chi(x1.d->i(),x2.d->i(),x3.d->i());
}
inline void AffineLinEval::add_fwd(const ExprAdd&, const ExprLabel& x1, const ExprLabel& x2, ExprLabel& y)     {
	y.af_lin->i()=x1.af_lin->i()+x2.af_lin->i();
	y.d->i()=(y.af_lin->i().itv() & (x1.d->i()+x2.d->i()));
}
inline void AffineLinEval::mul_fwd(const ExprMul&, const ExprLabel& x1, const ExprLabel& x2, ExprLabel& y)     {
	y.af_lin->i()=x1.af_lin->i()*x2.af_lin->i();
	y.d->i()=(y.af_lin->i().itv() & (x1.d->i()*x2.d->i()));
}
inline void AffineLinEval::sub_fwd(const ExprSub&, const ExprLabel& x1, const ExprLabel& x2, ExprLabel& y)     {
	y.af_lin->i()=x1.af_lin->i()-x2.af_lin->i();
	y.d->i()=(y.af_lin->i().itv() & (x1.d->i()-x2.d->i()));
}
inline void AffineLinEval::div_fwd(const ExprDiv&, const ExprLabel& x1, const ExprLabel& x2, ExprLabel& y)     {
	y.af_lin->i()=x1.af_lin->i()*inv(x2.af_lin->i(),x2.d->i());
	y.d->i()=(y.af_lin->i().itv() & (x1.d->i()/x2.d->i()));
}
inline void AffineLinEval::max_fwd(const ExprMax&, const ExprLabel& x1, const ExprLabel& x2, ExprLabel& y)     {
	y.d->i()= max(x1.d->i(),x2.d->i());
	y.af_lin->i() = AffineLin(y.d->i());
}
inline void AffineLinEval::min_fwd(const ExprMin&, const ExprLabel& x1, const ExprLabel& x2, ExprLabel& y)     {
	y.d->i() = min(x1.d->i(),x2.d->i());
	y.af_lin->i()= AffineLin( y.d->i());
}
inline void AffineLinEval::atan2_fwd(const ExprAtan2&, const ExprLabel& x1, const ExprLabel& x2, ExprLabel& y) {
	y.d->i() = atan2(x1.d->i(),x2.d->i());
	y.af_lin->i()= AffineLin(y.d->i());
}
inline void AffineLinEval::minus_fwd(const ExprMinus&, const ExprLabel& x, ExprLabel& y)                       {
	y.af_lin->i()=-x.af_lin->i();
	y.d->i()=(y.af_lin->i().itv() & (-x.d->i()));
}
inline void AffineLinEval::sign_fwd(const ExprSign&, const ExprLabel& x, ExprLabel& y)                         {
	y.af_lin->i()=sign(x.af_lin->i(),x.d->i());
	y.d->i()=(y.af_lin->i().itv() & sign(x.d->i()));
}
inline void AffineLinEval::abs_fwd(const ExprAbs&, const ExprLabel& x, ExprLabel& y)                           {
	y.af_lin->i()=abs(x.af_lin->i(),x.d->i());
	y.d->i()=(y.af_lin->i().itv() & abs(x.d->i()));
}
inline void AffineLinEval::power_fwd(const ExprPower& p, const ExprLabel& x, ExprLabel& y)                     {
	y.af_lin->i()=pow(x.af_lin->i(),p.expon,x.d->i());
	y.d->i()=(y.af_lin->i().itv() & pow(x.d->i(),p.expon));
}
inline void AffineLinEval::sqr_fwd(const ExprSqr&, const ExprLabel& x, ExprLabel& y)                           {
	y.af_lin->i()=sqr(x.af_lin->i(),x.d->i());
	y.d->i()=(y.af_lin->i().itv() & sqr(x.d->i()));
}
inline void AffineLinEval::sqrt_fwd(const ExprSqrt&, const ExprLabel& x, ExprLabel& y)                         {
	y.af_lin->i()=sqrt(x.af_lin->i(),x.d->i());
	y.d->i()=(y.af_lin->i().itv() & sqrt(x.d->i()));
}
inline void AffineLinEval::exp_fwd(const ExprExp&, const ExprLabel& x, ExprLabel& y)                           {
	y.af_lin->i()=exp(x.af_lin->i(),x.d->i());
	y.d->i()=(y.af_lin->i().itv() & exp(x.d->i()));
}
inline void AffineLinEval::log_fwd(const ExprLog&, const ExprLabel& x, ExprLabel& y)                           {
	y.af_lin->i()=log(x.af_lin->i(),x.d->i());
	y.d->i()=(y.af_lin->i().itv() & log(x.d->i()));
}
inline void AffineLinEval::cos_fwd(const ExprCos&, const ExprLabel& x, ExprLabel& y)                           {
	y.af_lin->i()=cos(x.af_lin->i(),x.d->i());
	y.d->i()=(y.af_lin->i().itv() & cos(x.d->i()));
}
inline void AffineLinEval::sin_fwd(const ExprSin&, const ExprLabel& x, ExprLabel& y)                           {
	y.af_lin->i()=sin(x.af_lin->i(),x.d->i());
	y.d->i()=(y.af_lin->i().itv() & sin(x.d->i()));
}
inline void AffineLinEval::tan_fwd(const ExprTan&, const ExprLabel& x, ExprLabel& y)                           {
	y.af_lin->i()=tan(x.af_lin->i(),x.d->i());
	y.d->i()=(y.af_lin->i().itv() & tan(x.d->i()));
}
inline void AffineLinEval::cosh_fwd(const ExprCosh&, const ExprLabel& x, ExprLabel& y)                         {
	y.af_lin->i()=cosh(x.af_lin->i(),x.d->i());
	y.d->i()=(y.af_lin->i().itv() & cosh(x.d->i()));
}
inline void AffineLinEval::sinh_fwd(const ExprSinh&, const ExprLabel& x, ExprLabel& y)                         {
	y.af_lin->i()=sinh(x.af_lin->i(),x.d->i());
	y.d->i()=(y.af_lin->i().itv() & sinh(x.d->i()));
}
inline void AffineLinEval::tanh_fwd(const ExprTanh&, const ExprLabel& x, ExprLabel& y)                         {
	y.af_lin->i()=tanh(x.af_lin->i(),x.d->i());
	y.d->i()=(y.af_lin->i().itv() & tanh(x.d->i()));
}
inline void AffineLinEval::acos_fwd(const ExprAcos&, const ExprLabel& x, ExprLabel& y)                         {
	y.af_lin->i()=acos(x.af_lin->i(),x.d->i());
	y.d->i()=(y.af_lin->i().itv() & acos(x.d->i()));
}
inline void AffineLinEval::asin_fwd(const ExprAsin&, const ExprLabel& x, ExprLabel& y)                         {
	y.af_lin->i()=asin(x.af_lin->i(),x.d->i());
	y.d->i()=(y.af_lin->i().itv() & asin(x.d->i()));
}
inline void AffineLinEval::atan_fwd(const ExprAtan&, const ExprLabel& x, ExprLabel& y)                         {
	y.af_lin->i()=atan(x.af_lin->i(),x.d->i());
	y.d->i()=(y.af_lin->i().itv() & atan(x.d->i()));
}
inline void AffineLinEval::acosh_fwd(const ExprAcosh&, const ExprLabel& x, ExprLabel& y)                       {
	y.d->i()=acosh(x.d->i());
	y.af_lin->i()= AffineLin(y.d->i());
}
inline void AffineLinEval::asinh_fwd(const ExprAsinh&, const ExprLabel& x, ExprLabel& y)                       {
	y.d->i()=asinh(x.d->i());
	y.af_lin->i()= AffineLin(y.d->i());
}
inline void AffineLinEval::atanh_fwd(const ExprAtanh&, const ExprLabel& x, ExprLabel& y)                       {
	y.d->i()=atanh(x.d->i());
	y.af_lin->i()= AffineLin(y.d->i());
}



inline void AffineLinEval::trans_V_fwd(const ExprTrans&, const ExprLabel& x, ExprLabel& y)                     {
	y.af_lin->v()=x.af_lin->v();
	y.d->v()=x.d->v();
}
inline void AffineLinEval::trans_M_fwd(const ExprTrans&, const ExprLabel& x, ExprLabel& y)                     {
	y.af_lin->m()=x.af_lin->m().transpose();
	y.d->m()=x.d->m().transpose();
}
inline void AffineLinEval::add_V_fwd(const ExprAdd&, const ExprLabel& x1, const ExprLabel& x2, ExprLabel& y)   {
	y.af_lin->v()=x1.af_lin->v()+x2.af_lin->v();
	y.d->v()=(y.af_lin->v().itv() & (x1.d->v()+x2.d->v()));
}
inline void AffineLinEval::add_M_fwd(const ExprAdd&, const ExprLabel& x1, const ExprLabel& x2, ExprLabel& y)   {
	y.af_lin->m()=x1.af_lin->m()+x2.af_lin->m();
	y.d->m()=(y.af_lin->m().itv());
	y.d->m() &= (x1.d->m()+x2.d->m());
}
inline void AffineLinEval::mul_SV_fwd(const ExprMul&, const ExprLabel& x1, const ExprLabel& x2, ExprLabel& y)  {
	y.af_lin->v()=x1.af_lin->i()*x2.af_lin->v();
	y.d->v()=(y.af_lin->v().itv() & (x1.d->i()*x2.d->v()));
}
inline void AffineLinEval::mul_SM_fwd(const ExprMul&, const ExprLabel& x1, const ExprLabel& x2, ExprLabel& y)  {
	y.af_lin->m()=x1.af_lin->i()*x2.af_lin->m();
	y.d->m()=(y.af_lin->m().itv());
	y.d->m() &= (x1.d->i()*x2.d->m());
}
inline void AffineLinEval::mul_VV_fwd(const ExprMul&, const ExprLabel& x1, const ExprLabel& x2, ExprLabel& y)  {
	y.af_lin->i()=x1.af_lin->v()*x2.af_lin->v();
	y.d->i()=(y.af_lin->i().itv() & (x1.d->v()*x2.d->v()));
}
inline void AffineLinEval::mul_MV_fwd(const ExprMul&, const ExprLabel& x1, const ExprLabel& x2, ExprLabel& y)  {
	y.af_lin->v()=x1.af_lin->m()*x2.af_lin->v();
	y.d->v()=(y.af_lin->v().itv() & (x1.d->m()*x2.d->v()));
}
inline void AffineLinEval::mul_VM_fwd(const ExprMul&, const ExprLabel& x1, const ExprLabel& x2, ExprLabel& y)  {
	y.af_lin->v()=x1.af_lin->v()*x2.af_lin->m();
	y.d->v()=(y.af_lin->v().itv() & (x1.d->v()*x2.d->m()));
}
inline void AffineLinEval::mul_MM_fwd(const ExprMul&, const ExprLabel& x1, const ExprLabel& x2, ExprLabel& y)  {
	y.af_lin->m()=x1.af_lin->m()*x2.af_lin->m();
	y.d->m()=(y.af_lin->m().itv()) ;
	y.d->m() &=  (x1.d->m()*x2.d->m());
}
inline void AffineLinEval::sub_V_fwd(const ExprSub&, const ExprLabel& x1, const ExprLabel& x2, ExprLabel& y)   {
	y.af_lin->v()=x1.af_lin->v()-x2.af_lin->v();
	y.d->v()=(y.af_lin->v().itv() & (x1.d->v()-x2.d->v()));
}
inline void AffineLinEval::sub_M_fwd(const ExprSub&, const ExprLabel& x1, const ExprLabel& x2, ExprLabel& y)   {
	y.af_lin->m()=x1.af_lin->m()-x2.af_lin->m();
	y.d->m() = y.af_lin->m().itv();
	y.d->m() &= (x1.d->m()-x2.d->m());
}

} // namespace ibex


#endif /* __IBEX_AffineLin_EVAL_H__ */
