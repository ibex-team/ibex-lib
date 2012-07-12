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
	void iproj(const Function& f, const Domain& y, IntervalVector& x);

	void iproj(const Function& f, const Domain& y, IntervalVector& x, const IntervalVector& xin);

	inline void index_bwd (const ExprIndex&,   ExprLabel& exprL, const ExprLabel& result)        { /* nothing to do */ }
	       void vector_bwd(const ExprVector&,  ExprLabel** compL, const ExprLabel& result)       { not_implemented("Inner projection of \"vector\""); }
	inline void symbol_bwd(const ExprSymbol& s, const ExprLabel& result)                         { /* nothing to do */ }
	inline void cst_bwd   (const ExprConstant&, const ExprLabel& y)                              { /* nothing to do */ }
	inline void apply_bwd (const ExprApply& a, ExprLabel** x, const ExprLabel& y)                { not_implemented("Inner projection of \"apply\""); }
	inline void add_bwd   (const ExprAdd&,     ExprLabel& x1, ExprLabel& x2, const ExprLabel& y) { iproj_add(y.d->i(),x1.d->i(),x2.d->i(),x1.p->i(),x2.p->i()); }
	inline void add_V_bwd (const ExprAdd&,     ExprLabel& x1, ExprLabel& x2, const ExprLabel& y) { not_implemented("Inner projection of \"add_V\""); }
	inline void add_M_bwd (const ExprAdd&,     ExprLabel& x1, ExprLabel& x2, const ExprLabel& y) { not_implemented("Inner projection of \"add_M\""); }
	inline void mul_bwd   (const ExprMul&,     ExprLabel& x1, ExprLabel& x2, const ExprLabel& y) { iproj_mul(y.d->i(),x1.d->i(),x2.d->i(),x1.p->i(),x2.p->i()); }
	inline void mul_SV_bwd(const ExprMul&,     ExprLabel& x1, ExprLabel& x2, const ExprLabel& y) { not_implemented("Inner projection of \"mul_SV\""); }
	inline void mul_SM_bwd(const ExprMul&,     ExprLabel& x1, ExprLabel& x2, const ExprLabel& y) { not_implemented("Inner projection of \"mul_SM\""); }
	inline void mul_VV_bwd(const ExprMul&,     ExprLabel& x1, ExprLabel& x2, const ExprLabel& y) { not_implemented("Inner projection of \"mul_VV\""); }
	inline void mul_MV_bwd(const ExprMul&,     ExprLabel& x1, ExprLabel& x2, const ExprLabel& y) { not_implemented("Inner projection of \"mul_MV\""); }
	inline void mul_VM_bwd(const ExprMul&,     ExprLabel& x1, ExprLabel& x2, const ExprLabel& y) { not_implemented("Inner projection of \"mul_VM\""); }
	inline void mul_MM_bwd(const ExprMul&,     ExprLabel& x1, ExprLabel& x2, const ExprLabel& y) { not_implemented("Inner projection of \"mul_MM\""); }
	inline void sub_bwd   (const ExprSub&,     ExprLabel& x1, ExprLabel& x2, const ExprLabel& y) { iproj_sub(y.d->i(),x1.d->i(),x2.d->i(),x1.p->i(),x2.p->i()); }
	inline void sub_V_bwd (const ExprSub&,     ExprLabel& x1, ExprLabel& x2, const ExprLabel& y) { not_implemented("Inner projection of \"sub_V\""); }
	inline void sub_M_bwd (const ExprSub&,     ExprLabel& x1, ExprLabel& x2, const ExprLabel& y) { not_implemented("Inner projection of \"sub_M\""); }
	inline void div_bwd   (const ExprDiv&,     ExprLabel& x1, ExprLabel& x2, const ExprLabel& y) { iproj_div(y.d->i(),x1.d->i(),x2.d->i(),x1.p->i(),x2.p->i()); }
	inline void max_bwd   (const ExprMax&,     ExprLabel& x1, ExprLabel& x2, const ExprLabel& y) { not_implemented("Inner projection of \"max\""); }
	inline void min_bwd   (const ExprMin&,     ExprLabel& x1, ExprLabel& x2, const ExprLabel& y) { not_implemented("Inner projection of \"min\""); }
	inline void atan2_bwd (const ExprAtan2& e, ExprLabel& x1, ExprLabel& x2, const ExprLabel& y) { not_implemented("Inner projection of \"atan2\""); }
	inline void minus_bwd (const ExprMinus& e, ExprLabel& x, const ExprLabel& y)                 { iproj_minus(y.d->i(),x.d->i()); }
	inline void sign_bwd  (const ExprSign& e,  ExprLabel& x, const ExprLabel& y)                 { not_implemented("Inner projection of \"sign\""); }
	inline void abs_bwd   (const ExprAbs& e,   ExprLabel& x, const ExprLabel& y)                 { not_implemented("Inner projection of \"abs\""); }
	inline void power_bwd (const ExprPower& e, ExprLabel& x, const ExprLabel& y)                 { iproj_pow(y.d->i(),x.d->i(),e.expon, x.p->i()); }
	inline void sqr_bwd   (const ExprSqr& e,   ExprLabel& x, const ExprLabel& y)                 { iproj_sqr(y.d->i(),x.d->i(),x.p->i()); }
	inline void sqrt_bwd  (const ExprSqrt& e,  ExprLabel& x, const ExprLabel& y)                 { not_implemented("Inner projection of \"sqrt\""); }
	inline void exp_bwd   (const ExprExp& e,   ExprLabel& x, const ExprLabel& y)                 { iproj_exp(y.d->i(),x.d->i()); }
	inline void log_bwd   (const ExprLog& e,   ExprLabel& x, const ExprLabel& y)                 { iproj_log(y.d->i(),x.d->i()); }
	inline void cos_bwd   (const ExprCos& e,   ExprLabel& x, const ExprLabel& y)                 { not_implemented("Inner projection of \"cos\""); }
	inline void sin_bwd   (const ExprSin& e,   ExprLabel& x, const ExprLabel& y)                 { not_implemented("Inner projection of \"sin\""); }
	inline void tan_bwd   (const ExprTan& e,   ExprLabel& x, const ExprLabel& y)                 { not_implemented("Inner projection of \"tan\""); }
	inline void cosh_bwd  (const ExprCosh& e,  ExprLabel& x, const ExprLabel& y)                 { not_implemented("Inner projection of \"cosh\""); }
	inline void sinh_bwd  (const ExprSinh& e,  ExprLabel& x, const ExprLabel& y)                 { not_implemented("Inner projection of \"sinh\""); }
	inline void tanh_bwd  (const ExprTanh& e,  ExprLabel& x, const ExprLabel& y)                 { not_implemented("Inner projection of \"tanh\""); }
	inline void acos_bwd  (const ExprAcos& e,  ExprLabel& x, const ExprLabel& y)                 { not_implemented("Inner projection of \"acos\""); }
	inline void asin_bwd  (const ExprAsin& e,  ExprLabel& x, const ExprLabel& y)                 { not_implemented("Inner projection of \"asin\""); }
	inline void atan_bwd  (const ExprAtan& e,  ExprLabel& x, const ExprLabel& y)                 { not_implemented("Inner projection of \"atan\""); }
	inline void acosh_bwd (const ExprAcosh& e, ExprLabel& x, const ExprLabel& y)                 { not_implemented("Inner projection of \"acosh\""); }
	inline void asinh_bwd (const ExprAsinh& e, ExprLabel& x, const ExprLabel& y)                 { not_implemented("Inner projection of \"asinh\""); }
	inline void atanh_bwd (const ExprAtanh& e, ExprLabel& x, const ExprLabel& y)                 { not_implemented("Inner projection of \"atanh\""); }

};

} // end namespace ibex
#endif // __IBEX_IN_HC4_REVISE_H__
