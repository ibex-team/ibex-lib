//============================================================================
//                                  I B E X                                   
// File        : ibex_InHC4Revise.h
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Jul 12, 2012
// Last Update : Jul 12, 2012
//============================================================================

#ifndef __IBEX_IN_HC4_REVISE_H__
#define __IBEX_IN_HC4_REVISE_H__

#include "ibex_Function.h"
#include "ibex_Exception.h"
#include "ibex_InnerArith.h"

namespace ibex {

class InHC4Revise : public BwdAlgorithm {
public:
	void ibwd(const Function& f, const Domain& y, IntervalVector& x);

	void ibwd(const Function& f, const Domain& y, IntervalVector& x, const IntervalVector& xin);

protected:
	/**
	 * Class used internally to interrupt the
	 * backward procedure when an empty domain occurs.
	 */
	class EmptyBoxException { };

public: // because called from CompiledFunction
	inline void index_bwd (const ExprIndex&,   ExprLabel& , const ExprLabel& )        { /* nothing to do */ }
	void vector_bwd(const ExprVector&,  ExprLabel** , const ExprLabel& )       { not_implemented("Inner projection of \"vector\""); }
//	       void vector_bwd(const ExprVector&,  ExprLabel** compL, const ExprLabel& result)       { not_implemented("Inner projection of \"vector\""); }
	inline void symbol_bwd(const ExprSymbol& , const ExprLabel& )                         { /* nothing to do */ }
	inline void cst_bwd   (const ExprConstant& c, const ExprLabel& y)                              { /* TODO: improve this. */ if (*(y.d)!=c.get()) throw EmptyBoxException(); }
	inline void apply_bwd (const ExprApply& a, ExprLabel** x, const ExprLabel& y)                { if (!ibwd(a.func, *y.d, x)) throw EmptyBoxException(); }
	inline void chi_bwd   (const ExprChi&,ExprLabel& ,ExprLabel& ,ExprLabel& ,const ExprLabel& ) { not_implemented("Inner projection of \"chi\""); }
	inline void add_bwd   (const ExprAdd&,     ExprLabel& x1, ExprLabel& x2, const ExprLabel& y) { if (!ibwd_add(y.d->i(),x1.d->i(),x2.d->i(),x1.p->i(),x2.p->i())) throw EmptyBoxException(); }
	inline void add_V_bwd (const ExprAdd&,     ExprLabel& , ExprLabel& , const ExprLabel& ) { not_implemented("Inner projection of \"add_V\""); }
	inline void add_M_bwd (const ExprAdd&,     ExprLabel& , ExprLabel& , const ExprLabel& ) { not_implemented("Inner projection of \"add_M\""); }
	inline void mul_bwd   (const ExprMul&,     ExprLabel& x1, ExprLabel& x2, const ExprLabel& y) { if (!ibwd_mul(y.d->i(),x1.d->i(),x2.d->i(),x1.p->i(),x2.p->i())) throw EmptyBoxException(); }
	inline void mul_SV_bwd(const ExprMul&,     ExprLabel& , ExprLabel& , const ExprLabel& ) { not_implemented("Inner projection of \"mul_SV\""); }
	inline void mul_SM_bwd(const ExprMul&,     ExprLabel& , ExprLabel& , const ExprLabel& ) { not_implemented("Inner projection of \"mul_SM\""); }
	inline void mul_VV_bwd(const ExprMul&,     ExprLabel& , ExprLabel& , const ExprLabel& ) { not_implemented("Inner projection of \"mul_VV\""); }
	inline void mul_MV_bwd(const ExprMul&,     ExprLabel& , ExprLabel& , const ExprLabel& ) { not_implemented("Inner projection of \"mul_MV\""); }
	inline void mul_VM_bwd(const ExprMul&,     ExprLabel& , ExprLabel& , const ExprLabel& ) { not_implemented("Inner projection of \"mul_VM\""); }
	inline void mul_MM_bwd(const ExprMul&,     ExprLabel& , ExprLabel& , const ExprLabel& ) { not_implemented("Inner projection of \"mul_MM\""); }
	inline void sub_bwd   (const ExprSub&,     ExprLabel& x1, ExprLabel& x2, const ExprLabel& y) { if (!ibwd_sub(y.d->i(),x1.d->i(),x2.d->i(),x1.p->i(),x2.p->i())) throw EmptyBoxException(); }
	inline void sub_V_bwd (const ExprSub&,     ExprLabel& , ExprLabel& , const ExprLabel& ) { not_implemented("Inner projection of \"sub_V\""); }
	inline void sub_M_bwd (const ExprSub&,     ExprLabel& , ExprLabel& , const ExprLabel& ) { not_implemented("Inner projection of \"sub_M\""); }
	inline void div_bwd   (const ExprDiv&,     ExprLabel& x1, ExprLabel& x2, const ExprLabel& y) { if (!ibwd_div(y.d->i(),x1.d->i(),x2.d->i(),x1.p->i(),x2.p->i())) throw EmptyBoxException(); }
	inline void max_bwd   (const ExprMax&,     ExprLabel& x1, ExprLabel& x2, const ExprLabel& y) { if (!ibwd_max(y.d->i(),x1.d->i(),x2.d->i(),x1.p->i(),x2.p->i())) throw EmptyBoxException(); }
	inline void min_bwd   (const ExprMin&,     ExprLabel& x1, ExprLabel& x2, const ExprLabel& y) { if (!ibwd_min(y.d->i(),x1.d->i(),x2.d->i(),x1.p->i(),x2.p->i())) throw EmptyBoxException(); }
	inline void atan2_bwd (const ExprAtan2&,   ExprLabel& , ExprLabel& , const ExprLabel& ) { not_implemented("Inner projection of \"atan2\""); }
	inline void minus_bwd (const ExprMinus& , ExprLabel& x, const ExprLabel& y)                 { if (!ibwd_minus(y.d->i(),x.d->i())) throw EmptyBoxException(); }
    inline void trans_V_bwd(const ExprTrans& ,ExprLabel& , const ExprLabel& )                 { not_implemented("Inner projection of \"transpose\""); }
    inline void trans_M_bwd(const ExprTrans& ,ExprLabel& , const ExprLabel& )                 { not_implemented("Inner projection of \"transpose\""); }
	inline void sign_bwd  (const ExprSign& ,  ExprLabel& , const ExprLabel& )                 { not_implemented("Inner projection of \"sign\""); }
	inline void abs_bwd   (const ExprAbs& ,   ExprLabel& x, const ExprLabel& y)                 { if (!ibwd_abs(y.d->i(),x.d->i())) throw EmptyBoxException(); }
	inline void power_bwd (const ExprPower& e, ExprLabel& x, const ExprLabel& y)                { if (!ibwd_pow(y.d->i(),x.d->i(),e.expon, x.p->i())) throw EmptyBoxException(); }
	inline void sqr_bwd   (const ExprSqr& ,   ExprLabel& x, const ExprLabel& y)                 { if (!ibwd_sqr(y.d->i(),x.d->i(),x.p->i())) throw EmptyBoxException(); }
	inline void sqrt_bwd  (const ExprSqrt& ,  ExprLabel& x, const ExprLabel& y)                 { if (!ibwd_sqrt(y.d->i(),x.d->i())) throw EmptyBoxException(); }
	inline void exp_bwd   (const ExprExp& ,   ExprLabel& x, const ExprLabel& y)                 { if (!ibwd_exp(y.d->i(),x.d->i())) throw EmptyBoxException(); }
	inline void log_bwd   (const ExprLog& ,   ExprLabel& x, const ExprLabel& y)                 { if (!ibwd_log(y.d->i(),x.d->i())) throw EmptyBoxException(); }
	inline void cos_bwd   (const ExprCos& ,   ExprLabel& x, const ExprLabel& y)                 { if (!ibwd_cos(y.d->i(),x.d->i(),x.p->i())) throw EmptyBoxException(); }
	inline void sin_bwd   (const ExprSin& ,   ExprLabel& x, const ExprLabel& y)                 { if (!ibwd_sin(y.d->i(),x.d->i(),x.p->i())) throw EmptyBoxException();}
	inline void tan_bwd   (const ExprTan& ,   ExprLabel& x, const ExprLabel& y)                 { if (!ibwd_tan(y.d->i(),x.d->i(),x.p->i())) throw EmptyBoxException(); }
	inline void cosh_bwd  (const ExprCosh& ,  ExprLabel& , const ExprLabel& )                 { not_implemented("Inner projection of \"cosh\""); }
	inline void sinh_bwd  (const ExprSinh& ,  ExprLabel& , const ExprLabel& )                 { not_implemented("Inner projection of \"sinh\""); }
	inline void tanh_bwd  (const ExprTanh& ,  ExprLabel& , const ExprLabel& )                 { not_implemented("Inner projection of \"tanh\""); }
	inline void acos_bwd  (const ExprAcos& ,  ExprLabel& , const ExprLabel& )                 { not_implemented("Inner projection of \"acos\""); }
	inline void asin_bwd  (const ExprAsin& ,  ExprLabel& , const ExprLabel& )                 { not_implemented("Inner projection of \"asin\""); }
	inline void atan_bwd  (const ExprAtan& ,  ExprLabel& , const ExprLabel& )                 { not_implemented("Inner projection of \"atan\""); }
	inline void acosh_bwd (const ExprAcosh& , ExprLabel& , const ExprLabel& )                 { not_implemented("Inner projection of \"acosh\""); }
	inline void asinh_bwd (const ExprAsinh& , ExprLabel& , const ExprLabel& )                 { not_implemented("Inner projection of \"asinh\""); }
	inline void atanh_bwd (const ExprAtanh& , ExprLabel& , const ExprLabel& )                 { not_implemented("Inner projection of \"atanh\""); }

protected:
	bool ibwd(const Function& f, const Domain& y, ExprLabel** x);
};

} // end namespace ibex
#endif // __IBEX_IN_HC4_REVISE_H__
