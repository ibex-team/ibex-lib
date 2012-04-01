//============================================================================
//                                  I B E X                                   
// File        : HC4Revise Algorithm (The famous forward-backward contraction algorithm).
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Dec 31, 2011
// Last Update : 
//============================================================================

#ifndef __IBEX_HC4_REVISE_ALGO_H__
#define __IBEX_HC4R_EVISE_ALGO_H__

#include "ibex_Eval.h"
#include "ibex_CompiledFunction.h"
#include "ibex_NumConstraint.h"

namespace ibex {

/**
 * \ingroup level1
 * \brief The famous forward-backward contraction algorithm.
 *
 */
class HC4ReviseAlgo : public BwdAlgorithm<EvalLabel> {
public:

	HC4ReviseAlgo(const NumConstraint& ctr) : eval(ctr.f), equality(ctr.equality) { }

	void contract(Domain& box);

protected:
	friend class CompiledFunction<EvalLabel>;

	Eval eval;

	bool equality;

	mutable const Domain* box; // current box

	inline void index_bwd (const ExprIndex&,   EvalLabel& exprL, const EvalLabel& result)                    { /* nothing to do */ }
	inline void vector_bwd(const ExprVector&,  EvalLabel** compL, const EvalLabel& result);
	inline void symbol_bwd(const ExprSymbol&, const EvalLabel& result)                                       { /* nothing to do */ }
	inline void cst_bwd   (const ExprConstant&, const EvalLabel& result)                                     { /* nothing to do */ }
	inline void apply_bwd (const ExprApply&,   EvalLabel** argL, const EvalLabel& result);
	inline void add_bwd   (const ExprAdd&,     EvalLabel& leftL, EvalLabel& rightL, const EvalLabel& result) { proj_add(result.i(),leftL.i(),rightL.i()); }
	inline void add_V_bwd  (const ExprAdd&,    EvalLabel& leftL, EvalLabel& rightL, const EvalLabel& result) { /* to do */}
	inline void add_M_bwd  (const ExprAdd&,    EvalLabel& leftL, EvalLabel& rightL, const EvalLabel& result) { /* to do */ }
	inline void mul_bwd    (const ExprMul&,    EvalLabel& leftL, EvalLabel& rightL, const EvalLabel& result) { proj_mul(result.i(),leftL.i(),rightL.i()); }
	inline void mul_SV_bwd (const ExprMul&,    EvalLabel& leftL, EvalLabel& rightL, const EvalLabel& result) {  }
	inline void mul_SM_bwd (const ExprMul&,    EvalLabel& leftL, EvalLabel& rightL, const EvalLabel& result) {  }
	inline void mul_VV_bwd (const ExprMul&,    EvalLabel& leftL, EvalLabel& rightL, const EvalLabel& result) {  }
	inline void mul_MV_bwd (const ExprMul&,    EvalLabel& leftL, EvalLabel& rightL, const EvalLabel& result) {  }
	inline void mul_MM_bwd (const ExprMul&,    EvalLabel& leftL, EvalLabel& rightL, const EvalLabel& result) {  }
	inline void sub_bwd   (const ExprSub&,     EvalLabel& leftL, EvalLabel& rightL, const EvalLabel& result) { proj_sub(result.i(),leftL.i(),rightL.i()); }
	inline void sub_V_bwd (const ExprSub&,     EvalLabel& leftL, EvalLabel& rightL, const EvalLabel& result) {  }
	inline void sub_M_bwd (const ExprSub&,     EvalLabel& leftL, EvalLabel& rightL, const EvalLabel& result) {  }
	inline void div_bwd   (const ExprDiv&,     EvalLabel& leftL, EvalLabel& rightL, const EvalLabel& result) { proj_div(result.i(),leftL.i(),rightL.i()); }
	inline void max_bwd   (const ExprMax&,     EvalLabel& leftL, EvalLabel& rightL, const EvalLabel& result) { proj_max(result.i(),leftL.i(),rightL.i()); }
	inline void min_bwd   (const ExprMin&,     EvalLabel& leftL, EvalLabel& rightL, const EvalLabel& result) { proj_min(result.i(),leftL.i(),rightL.i()); }
	inline void atan2_bwd (const ExprAtan2& e, EvalLabel& leftL, EvalLabel& rightL, const EvalLabel& result) { proj_atan2(result.i(),leftL.i(),rightL.i()); }
	inline void minus_bwd (const ExprMinus& e, EvalLabel& exprL, const EvalLabel& result)                    { exprL.i() &=-result.i(); }
	inline void sign_bwd  (const ExprSign& e,  EvalLabel& exprL, const EvalLabel& result)                    { proj_sign(result.i(),exprL.i()); }
	inline void abs_bwd   (const ExprAbs& e,   EvalLabel& exprL, const EvalLabel& result)                    { proj_abs(result.i(),exprL.i()); }
	inline void power_bwd (const ExprPower& e, EvalLabel& exprL, const EvalLabel& result)                    { proj_pow(result.i(),e.expon, exprL.i()); }
	inline void sqr_bwd   (const ExprSqr& e,   EvalLabel& exprL, const EvalLabel& result)                    { proj_sqr(result.i(),exprL.i()); }
	inline void sqrt_bwd  (const ExprSqrt& e,  EvalLabel& exprL, const EvalLabel& result)                    { proj_sqrt(result.i(),exprL.i()); }
	inline void exp_bwd   (const ExprExp& e,   EvalLabel& exprL, const EvalLabel& result)                    { proj_exp(result.i(),exprL.i()); }
	inline void log_bwd   (const ExprLog& e,   EvalLabel& exprL, const EvalLabel& result)                    { proj_log(result.i(),exprL.i()); }
	inline void cos_bwd   (const ExprCos& e,   EvalLabel& exprL, const EvalLabel& result)                    { proj_cos(result.i(),exprL.i()); }
	inline void sin_bwd   (const ExprSin& e,   EvalLabel& exprL, const EvalLabel& result)                    { proj_sin(result.i(),exprL.i()); }
	inline void tan_bwd   (const ExprTan& e,   EvalLabel& exprL, const EvalLabel& result)                    { proj_tan(result.i(),exprL.i()); }
	inline void cosh_bwd  (const ExprCosh& e,  EvalLabel& exprL, const EvalLabel& result)                    { proj_cosh(result.i(),exprL.i()); }
	inline void sinh_bwd  (const ExprSinh& e,  EvalLabel& exprL, const EvalLabel& result)                    { proj_sinh(result.i(),exprL.i()); }
	inline void tanh_bwd  (const ExprTanh& e,  EvalLabel& exprL, const EvalLabel& result)                    { proj_tanh(result.i(),exprL.i()); }
	inline void acos_bwd  (const ExprAcos& e,  EvalLabel& exprL, const EvalLabel& result)                    { proj_acos(result.i(),exprL.i()); }
	inline void asin_bwd  (const ExprAsin& e,  EvalLabel& exprL, const EvalLabel& result)                    { proj_asin(result.i(),exprL.i()); }
	inline void atan_bwd  (const ExprAtan& e,  EvalLabel& exprL, const EvalLabel& result)                    { proj_atan(result.i(),exprL.i()); }
	inline void acosh_bwd (const ExprAcosh& e, EvalLabel& exprL, const EvalLabel& result)                    { proj_acosh(result.i(),exprL.i()); }
	inline void asinh_bwd (const ExprAsinh& e, EvalLabel& exprL, const EvalLabel& result)                    { proj_asinh(result.i(),exprL.i()); }
	inline void atanh_bwd (const ExprAtanh& e, EvalLabel& exprL, const EvalLabel& result)                    { proj_atanh(result.i(),exprL.i()); }

private:
	void write(IntervalVector&);

};


} /* namespace ibex */
#endif /* __IBEX_HC4_REVISE_ALGO_H__ */
