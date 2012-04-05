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
#define __IBEX_HC4_REVISE_ALGO_H__

#include "ibex_Eval.h"
#include "ibex_CompiledFunction.h"
#include "ibex_NumConstraint.h"

namespace ibex {

/**
 * \ingroup level1
 * \brief The famous forward-backward contraction algorithm.
 *
 */
class HC4ReviseAlgo : public BwdAlgorithm<Domain> {
public:

	/**
	 * \brief Create the HC4-revise algorithm for a constraint \a ctr.
	 */
	HC4ReviseAlgo(NumConstraint& ctr);

	/**
	 * \brief Run the backward algorithm.
	 *
	 * Set the domain of the root node to \a root
	 * and run the backward algorithm. The new domains
	 * of the variables can be read from eval.symbolLabels.
	 */
	void backward(const Domain& root);

	/**
	 * \brief Contract the input box with the backward algorithm.
	 */
	void contract(IntervalVector& box);

	/**
	 * \brief The evaluator.
	 *
	 * This field contains in particular a reference to the function "f"
	 * of the constraint.
	 */
	Eval eval;

	/**
	 * \brief True if the constraint is an equality.
	 */
	const bool equality;

protected:
	friend class CompiledFunction;

	inline void index_bwd (const ExprIndex&,   Domain& exprL, const Domain& result)                    { /* nothing to do */ }
	inline void vector_bwd(const ExprVector&,  Domain** compL, const Domain& result);
	inline void symbol_bwd(const ExprSymbol&, const Domain& result)                                    { /* nothing to do */ }
	inline void cst_bwd   (const ExprConstant&, const Domain& result)                                  { /* nothing to do */ }
	inline void apply_bwd (const ExprApply&,   Domain** argL, const Domain& result);
	inline void add_bwd   (const ExprAdd&,     Domain& leftL, Domain& rightL, const Domain& result)    { proj_add(result.i(),leftL.i(),rightL.i()); }
	inline void add_V_bwd  (const ExprAdd&,    Domain& leftL, Domain& rightL, const Domain& result)    { /* to do */}
	inline void add_M_bwd  (const ExprAdd&,    Domain& leftL, Domain& rightL, const Domain& result)    { /* to do */ }
	inline void mul_bwd    (const ExprMul&,    Domain& leftL, Domain& rightL, const Domain& result)    { proj_mul(result.i(),leftL.i(),rightL.i()); }
	inline void mul_SV_bwd (const ExprMul&,    Domain& leftL, Domain& rightL, const Domain& result)    {  }
	inline void mul_SM_bwd (const ExprMul&,    Domain& leftL, Domain& rightL, const Domain& result)    {  }
	inline void mul_VV_bwd (const ExprMul&,    Domain& leftL, Domain& rightL, const Domain& result)    {  }
	inline void mul_MV_bwd (const ExprMul&,    Domain& leftL, Domain& rightL, const Domain& result)    {  }
	inline void mul_MM_bwd (const ExprMul&,    Domain& leftL, Domain& rightL, const Domain& result)    {  }
	inline void sub_bwd   (const ExprSub&,     Domain& leftL, Domain& rightL, const Domain& result)    { proj_sub(result.i(),leftL.i(),rightL.i()); }
	inline void sub_V_bwd (const ExprSub&,     Domain& leftL, Domain& rightL, const Domain& result)    {  }
	inline void sub_M_bwd (const ExprSub&,     Domain& leftL, Domain& rightL, const Domain& result)    {  }
	inline void div_bwd   (const ExprDiv&,     Domain& leftL, Domain& rightL, const Domain& result)    { proj_div(result.i(),leftL.i(),rightL.i()); }
	inline void max_bwd   (const ExprMax&,     Domain& leftL, Domain& rightL, const Domain& result)    { proj_max(result.i(),leftL.i(),rightL.i()); }
	inline void min_bwd   (const ExprMin&,     Domain& leftL, Domain& rightL, const Domain& result)    { proj_min(result.i(),leftL.i(),rightL.i()); }
	inline void atan2_bwd (const ExprAtan2& e, Domain& leftL, Domain& rightL, const Domain& result)    { proj_atan2(result.i(),leftL.i(),rightL.i()); }
	inline void minus_bwd (const ExprMinus& e, Domain& exprL, const Domain& result)                    { exprL.i() &=-result.i(); }
	inline void sign_bwd  (const ExprSign& e,  Domain& exprL, const Domain& result)                    { proj_sign(result.i(),exprL.i()); }
	inline void abs_bwd   (const ExprAbs& e,   Domain& exprL, const Domain& result)                    { proj_abs(result.i(),exprL.i()); }
	inline void power_bwd (const ExprPower& e, Domain& exprL, const Domain& result)                    { proj_pow(result.i(),e.expon, exprL.i()); }
	inline void sqr_bwd   (const ExprSqr& e,   Domain& exprL, const Domain& result)                    { proj_sqr(result.i(),exprL.i()); }
	inline void sqrt_bwd  (const ExprSqrt& e,  Domain& exprL, const Domain& result)                    { proj_sqrt(result.i(),exprL.i()); }
	inline void exp_bwd   (const ExprExp& e,   Domain& exprL, const Domain& result)                    { proj_exp(result.i(),exprL.i()); }
	inline void log_bwd   (const ExprLog& e,   Domain& exprL, const Domain& result)                    { proj_log(result.i(),exprL.i()); }
	inline void cos_bwd   (const ExprCos& e,   Domain& exprL, const Domain& result)                    { proj_cos(result.i(),exprL.i()); }
	inline void sin_bwd   (const ExprSin& e,   Domain& exprL, const Domain& result)                    { proj_sin(result.i(),exprL.i()); }
	inline void tan_bwd   (const ExprTan& e,   Domain& exprL, const Domain& result)                    { proj_tan(result.i(),exprL.i()); }
	inline void cosh_bwd  (const ExprCosh& e,  Domain& exprL, const Domain& result)                    { proj_cosh(result.i(),exprL.i()); }
	inline void sinh_bwd  (const ExprSinh& e,  Domain& exprL, const Domain& result)                    { proj_sinh(result.i(),exprL.i()); }
	inline void tanh_bwd  (const ExprTanh& e,  Domain& exprL, const Domain& result)                    { proj_tanh(result.i(),exprL.i()); }
	inline void acos_bwd  (const ExprAcos& e,  Domain& exprL, const Domain& result)                    { proj_acos(result.i(),exprL.i()); }
	inline void asin_bwd  (const ExprAsin& e,  Domain& exprL, const Domain& result)                    { proj_asin(result.i(),exprL.i()); }
	inline void atan_bwd  (const ExprAtan& e,  Domain& exprL, const Domain& result)                    { proj_atan(result.i(),exprL.i()); }
	inline void acosh_bwd (const ExprAcosh& e, Domain& exprL, const Domain& result)                    { proj_acosh(result.i(),exprL.i()); }
	inline void asinh_bwd (const ExprAsinh& e, Domain& exprL, const Domain& result)                    { proj_asinh(result.i(),exprL.i()); }
	inline void atanh_bwd (const ExprAtanh& e, Domain& exprL, const Domain& result)                    { proj_atanh(result.i(),exprL.i()); }

private:
	void write(IntervalVector&) const;

};


/* ============================================================================
 	 	 	 	 	 	 	 implementation
  ============================================================================*/

inline void HC4ReviseAlgo::backward(const Domain& root) {
	(Domain&) *eval.f.expr().deco = root;
	return eval.f.backward<HC4ReviseAlgo,Domain>(*this);
}

} /* namespace ibex */
#endif /* __IBEX_HC4_REVISE_ALGO_H__ */
