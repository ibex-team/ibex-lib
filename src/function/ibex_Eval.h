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

#ifndef _IBEX_EVAL_H_
#define _IBEX_EVAL_H_

#include "ibex_Function.h"
#include <iostream>

namespace ibex {


/**
 * \ingroup symbolic
 *
 * \brief Function evaluator.
 *
 * \pre Requires the function to be decorated with BasicDecorator.
 */
class Eval : public FwdAlgorithm {

public:
	/**
	 * \brief Run the forward algorithm with input domains.
	 */
	Domain& eval(const Function&, const Array<const Domain>& d) const;

	/**
	 * \brief Run the forward algorithm with input domains.
	 */
	Domain& eval(const Function&, const Array<Domain>& d) const;

	/**
	 * \brief Run the forward algorithm with an input box.
	 */
	Domain& eval(const Function&, const IntervalVector& box) const;

	/**
	 * \brief Run the forward algorithm with input domains.
	 */
	Domain& eval(const Function&, ExprLabel** d) const;

protected:
	/**
	 * Class used internally to interrupt the forward procedure
	 * when an empty domain occurs (<=> the input box is outside
	 * the definition domain of the function).
	 */
	class EmptyBoxException { };

public: // because called from CompiledFunction
	inline void index_fwd(const ExprIndex&, const ExprLabel& x, ExprLabel& y);
	       void vector_fwd(const ExprVector&, const ExprLabel** compL, ExprLabel& y);
	inline void cst_fwd(const ExprConstant&, ExprLabel& y);
	inline void symbol_fwd(const ExprSymbol&, ExprLabel& y);
	inline void apply_fwd(const ExprApply&, ExprLabel** x, ExprLabel& y);
	inline void chi_fwd(const ExprChi&, const ExprLabel& x1, const ExprLabel& x2, const ExprLabel& x3, ExprLabel& y);
	inline void add_fwd(const ExprAdd&, const ExprLabel& x1, const ExprLabel& x2, ExprLabel& y);
	inline void mul_fwd(const ExprMul&, const ExprLabel& x1, const ExprLabel& x2, ExprLabel& y);
	inline void sub_fwd(const ExprSub&, const ExprLabel& x1, const ExprLabel& x2, ExprLabel& y);
	inline void div_fwd(const ExprDiv&, const ExprLabel& x1, const ExprLabel& x2, ExprLabel& y);
	inline void max_fwd(const ExprMax&, const ExprLabel& x1, const ExprLabel& x2, ExprLabel& y);
	inline void min_fwd(const ExprMin&, const ExprLabel& x1, const ExprLabel& x2, ExprLabel& y);
	inline void atan2_fwd(const ExprAtan2&, const ExprLabel& x1, const ExprLabel& x2, ExprLabel& y);
	inline void minus_fwd(const ExprMinus&, const ExprLabel& x, ExprLabel& y);
	inline void trans_V_fwd(const ExprTrans&, const ExprLabel& x, ExprLabel& y);
	inline void trans_M_fwd(const ExprTrans&, const ExprLabel& x, ExprLabel& y);
	inline void sign_fwd(const ExprSign&, const ExprLabel& x, ExprLabel& y);
	inline void abs_fwd(const ExprAbs&, const ExprLabel& x, ExprLabel& y);
	inline void power_fwd(const ExprPower& p, const ExprLabel& x, ExprLabel& y);
	inline void sqr_fwd(const ExprSqr&, const ExprLabel& x, ExprLabel& y);
	inline void sqrt_fwd(const ExprSqrt&, const ExprLabel& x, ExprLabel& y);
	inline void exp_fwd(const ExprExp&, const ExprLabel& x, ExprLabel& y);
	inline void log_fwd(const ExprLog&, const ExprLabel& x, ExprLabel& y);
	inline void cos_fwd(const ExprCos&, const ExprLabel& x, ExprLabel& y);
	inline void sin_fwd(const ExprSin&, const ExprLabel& x, ExprLabel& y);
	inline void tan_fwd(const ExprTan&, const ExprLabel& x, ExprLabel& y);
	inline void cosh_fwd(const ExprCosh&, const ExprLabel& x, ExprLabel& y);
	inline void sinh_fwd(const ExprSinh&, const ExprLabel& x, ExprLabel& y);
	inline void tanh_fwd(const ExprTanh&, const ExprLabel& x, ExprLabel& y);
	inline void acos_fwd(const ExprAcos&, const ExprLabel& x, ExprLabel& y);
	inline void asin_fwd(const ExprAsin&, const ExprLabel& x, ExprLabel& y);
	inline void atan_fwd(const ExprAtan&, const ExprLabel& x, ExprLabel& y);
	inline void acosh_fwd(const ExprAcosh&, const ExprLabel& x, ExprLabel& y);
	inline void asinh_fwd(const ExprAsinh&, const ExprLabel& x, ExprLabel& y);
	inline void atanh_fwd(const ExprAtanh&, const ExprLabel& x, ExprLabel& y);

	inline void add_V_fwd(const ExprAdd&, const ExprLabel& x1, const ExprLabel& x2, ExprLabel& y);
	inline void add_M_fwd(const ExprAdd&, const ExprLabel& x1, const ExprLabel& x2, ExprLabel& y);
	inline void mul_SV_fwd(const ExprMul&, const ExprLabel& x1, const ExprLabel& x2, ExprLabel& y);
	inline void mul_SM_fwd(const ExprMul&, const ExprLabel& x1, const ExprLabel& x2, ExprLabel& y);
	inline void mul_VV_fwd(const ExprMul&, const ExprLabel& x1, const ExprLabel& x2, ExprLabel& y);
	inline void mul_MV_fwd(const ExprMul&, const ExprLabel& x1, const ExprLabel& x2, ExprLabel& y);
	inline void mul_VM_fwd(const ExprMul&, const ExprLabel& x1, const ExprLabel& x2, ExprLabel& y);
	inline void mul_MM_fwd(const ExprMul&, const ExprLabel& x1, const ExprLabel& x2, ExprLabel& y);
	inline void sub_V_fwd(const ExprSub&, const ExprLabel& x1, const ExprLabel& x2, ExprLabel& y);
	inline void sub_M_fwd(const ExprSub&, const ExprLabel& x1, const ExprLabel& x2, ExprLabel& y);

};

/* ============================================================================
 	 	 	 	 	 	 	 implementation
  ============================================================================*/

inline void Eval::index_fwd(const ExprIndex& , const ExprLabel& , ExprLabel& ) { /* nothing to do */ }

inline void Eval::symbol_fwd(const ExprSymbol& , ExprLabel& ) { /* nothing to do */ }

inline void Eval::cst_fwd(const ExprConstant& c, ExprLabel& y) {
	switch (c.type()) {
	case Dim::SCALAR:       y.d->i() = c.get_value();         break;
	case Dim::ROW_VECTOR:
	case Dim::COL_VECTOR:   y.d->v() = c.get_vector_value();  break;
	case Dim::MATRIX:       y.d->m() = c.get_matrix_value();  break;
	case Dim::MATRIX_ARRAY: assert(false); /* impossible */ break;
	}
}
inline void Eval::apply_fwd(const ExprApply& a, ExprLabel** x, ExprLabel& y)                          { *y.d = eval(a.func,x); }
inline void Eval::chi_fwd(const ExprChi&, const ExprLabel& x1, const ExprLabel& x2, const ExprLabel& x3, ExprLabel& y) { y.d->i() = chi(x1.d->i(),x2.d->i(),x3.d->i()); }
inline void Eval::add_fwd(const ExprAdd&, const ExprLabel& x1, const ExprLabel& x2, ExprLabel& y)     { y.d->i()=x1.d->i()+x2.d->i(); }
inline void Eval::mul_fwd(const ExprMul&, const ExprLabel& x1, const ExprLabel& x2, ExprLabel& y)     { y.d->i()=x1.d->i()*x2.d->i(); }
inline void Eval::sub_fwd(const ExprSub&, const ExprLabel& x1, const ExprLabel& x2, ExprLabel& y)     { y.d->i()=x1.d->i()-x2.d->i(); }
inline void Eval::div_fwd(const ExprDiv&, const ExprLabel& x1, const ExprLabel& x2, ExprLabel& y)     { y.d->i()=x1.d->i()/x2.d->i(); }
inline void Eval::max_fwd(const ExprMax&, const ExprLabel& x1, const ExprLabel& x2, ExprLabel& y)     { y.d->i()=max(x1.d->i(),x2.d->i()); }
inline void Eval::min_fwd(const ExprMin&, const ExprLabel& x1, const ExprLabel& x2, ExprLabel& y)     { y.d->i()=min(x1.d->i(),x2.d->i()); }
inline void Eval::atan2_fwd(const ExprAtan2&, const ExprLabel& x1, const ExprLabel& x2, ExprLabel& y) { y.d->i()=atan2(x1.d->i(),x2.d->i()); }

inline void Eval::minus_fwd(const ExprMinus&, const ExprLabel& x, ExprLabel& y)                       { y.d->i()=-x.d->i(); }
inline void Eval::sign_fwd(const ExprSign&, const ExprLabel& x, ExprLabel& y)                         { y.d->i()=sign(x.d->i()); }
inline void Eval::abs_fwd(const ExprAbs&, const ExprLabel& x, ExprLabel& y)                           { y.d->i()=abs(x.d->i()); }
inline void Eval::power_fwd(const ExprPower& p, const ExprLabel& x, ExprLabel& y)                     { y.d->i()=pow(x.d->i(),p.expon); }
inline void Eval::sqr_fwd(const ExprSqr&, const ExprLabel& x, ExprLabel& y)                           { y.d->i()=sqr(x.d->i()); }
inline void Eval::sqrt_fwd(const ExprSqrt&, const ExprLabel& x, ExprLabel& y)                         { if ((y.d->i()=sqrt(x.d->i())).is_empty()) throw EmptyBoxException(); }
inline void Eval::exp_fwd(const ExprExp&, const ExprLabel& x, ExprLabel& y)                           { y.d->i()=exp(x.d->i()); }
inline void Eval::log_fwd(const ExprLog&, const ExprLabel& x, ExprLabel& y)                           { if ((y.d->i()=log(x.d->i())).is_empty()) throw EmptyBoxException(); }
inline void Eval::cos_fwd(const ExprCos&, const ExprLabel& x, ExprLabel& y)                           { y.d->i()=cos(x.d->i()); }
inline void Eval::sin_fwd(const ExprSin&, const ExprLabel& x, ExprLabel& y)                           { y.d->i()=sin(x.d->i()); }
inline void Eval::tan_fwd(const ExprTan&, const ExprLabel& x, ExprLabel& y)                           { if ((y.d->i()=tan(x.d->i())).is_empty()) throw EmptyBoxException(); }
inline void Eval::cosh_fwd(const ExprCosh&, const ExprLabel& x, ExprLabel& y)                         { y.d->i()=cosh(x.d->i()); }
inline void Eval::sinh_fwd(const ExprSinh&, const ExprLabel& x, ExprLabel& y)                         { y.d->i()=sinh(x.d->i()); }
inline void Eval::tanh_fwd(const ExprTanh&, const ExprLabel& x, ExprLabel& y)                         { y.d->i()=tanh(x.d->i()); }
inline void Eval::acos_fwd(const ExprAcos&, const ExprLabel& x, ExprLabel& y)                         { if ((y.d->i()=acos(x.d->i())).is_empty()) throw EmptyBoxException(); }
inline void Eval::asin_fwd(const ExprAsin&, const ExprLabel& x, ExprLabel& y)                         { if ((y.d->i()=asin(x.d->i())).is_empty()) throw EmptyBoxException(); }
inline void Eval::atan_fwd(const ExprAtan&, const ExprLabel& x, ExprLabel& y)                         { y.d->i()=atan(x.d->i()); }
inline void Eval::acosh_fwd(const ExprAcosh&, const ExprLabel& x, ExprLabel& y)                       { if ((y.d->i()=acosh(x.d->i())).is_empty()) throw EmptyBoxException(); }
inline void Eval::asinh_fwd(const ExprAsinh&, const ExprLabel& x, ExprLabel& y)                       { y.d->i()=asinh(x.d->i()); }
inline void Eval::atanh_fwd(const ExprAtanh&, const ExprLabel& x, ExprLabel& y)                       { if ((y.d->i()=atanh(x.d->i())).is_empty()) throw EmptyBoxException(); }

inline void Eval::trans_V_fwd(const ExprTrans&, const ExprLabel& x, ExprLabel& y)                     { y.d->v()=x.d->v(); }
inline void Eval::trans_M_fwd(const ExprTrans&, const ExprLabel& x, ExprLabel& y)                     { y.d->m()=x.d->m().transpose(); }
inline void Eval::add_V_fwd(const ExprAdd&, const ExprLabel& x1, const ExprLabel& x2, ExprLabel& y)   { y.d->v()=x1.d->v()+x2.d->v(); }
inline void Eval::add_M_fwd(const ExprAdd&, const ExprLabel& x1, const ExprLabel& x2, ExprLabel& y)   { y.d->m()=x1.d->m()+x2.d->m(); }
inline void Eval::mul_SV_fwd(const ExprMul&, const ExprLabel& x1, const ExprLabel& x2, ExprLabel& y)  { y.d->v()=x1.d->i()*x2.d->v(); }
inline void Eval::mul_SM_fwd(const ExprMul&, const ExprLabel& x1, const ExprLabel& x2, ExprLabel& y)  { y.d->m()=x1.d->i()*x2.d->m(); }
inline void Eval::mul_VV_fwd(const ExprMul&, const ExprLabel& x1, const ExprLabel& x2, ExprLabel& y)  { y.d->i()=x1.d->v()*x2.d->v(); }
inline void Eval::mul_MV_fwd(const ExprMul&, const ExprLabel& x1, const ExprLabel& x2, ExprLabel& y)  { y.d->v()=x1.d->m()*x2.d->v(); }
inline void Eval::mul_VM_fwd(const ExprMul&, const ExprLabel& x1, const ExprLabel& x2, ExprLabel& y)  { y.d->v()=x1.d->v()*x2.d->m(); }
inline void Eval::mul_MM_fwd(const ExprMul&, const ExprLabel& x1, const ExprLabel& x2, ExprLabel& y)  { y.d->m()=x1.d->m()*x2.d->m(); }
inline void Eval::sub_V_fwd(const ExprSub&, const ExprLabel& x1, const ExprLabel& x2, ExprLabel& y)   { y.d->v()=x1.d->v()-x2.d->v(); }
inline void Eval::sub_M_fwd(const ExprSub&, const ExprLabel& x1, const ExprLabel& x2, ExprLabel& y)   { y.d->m()=x1.d->m()-x2.d->m(); }

} // namespace ibex
#endif // IBEX_EVAL_H_
