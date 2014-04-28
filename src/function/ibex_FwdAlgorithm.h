/* ============================================================================
 * I B E X - Interface for forward algorithms
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Jan 22, 2012
 * ---------------------------------------------------------------------------- */

#ifndef _IBEX_FWD_ALGORITHM_H_
#define _IBEX_FWD_ALGORITHM_H_

#include "ibex_Expr.h"

namespace ibex {

/**
 * \ingroup symbolic
 * \brief Interface for forward algorithms.
 */
class FwdAlgorithm {

	/** TO BE DEFINED (by the subclass) */
	void index_fwd(const ExprIndex&, const ExprLabel& exprL, ExprLabel& result);

	/** TO BE DEFINED (by the subclass) */
	void symbol_fwd(const ExprSymbol&, ExprLabel& result);

	/** TO BE DEFINED (by the subclass) */
	void cst_fwd(const ExprConstant&, ExprLabel& result);

	/*==================== n-ary operators =========================*/
	/** TO BE DEFINED (by the subclass) */
	void vector_fwd(const ExprVector&, const ExprLabel** compL, ExprLabel& result);

	/** TO BE DEFINED (by the subclass) */
	void apply_fwd(const ExprApply&, ExprLabel** argL, ExprLabel& result);

	/** TO BE DEFINED (by the subclass) */
	void chi_fwd(const ExprChi&,  const ExprLabel& a, const ExprLabel& b, const ExprLabel& c, ExprLabel& result);

	/*==================== binary operators =========================*/
	/** TO BE DEFINED (by the subclass) */
	void add_fwd(const ExprAdd&, const ExprLabel& leftL, const ExprLabel& rightL, ExprLabel& result);

	/** TO BE DEFINED (by the subclass) */
	void add_V_fwd(const ExprAdd&, const ExprLabel& leftL, const ExprLabel& rightL, ExprLabel& result);

	/** TO BE DEFINED (by the subclass) */
	void add_M_fwd(const ExprAdd&, const ExprLabel& leftL, const ExprLabel& rightL, ExprLabel& result);

	/** TO BE DEFINED (by the subclass) */
	void mul_fwd(const ExprMul&, const ExprLabel& leftL, const ExprLabel& rightL, ExprLabel& result);

	/** TO BE DEFINED (by the subclass) */
	void mul_SV_fwd(const ExprMul&, const ExprLabel& leftL, const ExprLabel& rightL, ExprLabel& result);

	/** TO BE DEFINED (by the subclass) */
	void mul_SM_fwd(const ExprMul&, const ExprLabel& leftL, const ExprLabel& rightL, ExprLabel& result);

	/** TO BE DEFINED (by the subclass) */
	void mul_VV_fwd(const ExprMul&, const ExprLabel& leftL, const ExprLabel& rightL, ExprLabel& result);

	/** TO BE DEFINED (by the subclass) */
	void mul_MV_fwd(const ExprMul&, const ExprLabel& leftL, const ExprLabel& rightL, ExprLabel& result);

	/** TO BE DEFINED (by the subclass) */
	void mul_VM_fwd(const ExprMul&, const ExprLabel& leftL, const ExprLabel& rightL, ExprLabel& result);

	/** TO BE DEFINED (by the subclass) */
	void mul_MM_fwd(const ExprMul&, const ExprLabel& leftL, const ExprLabel& rightL, ExprLabel& result);

	/** TO BE DEFINED (by the subclass) */
	void sub_fwd(const ExprSub&, const ExprLabel& leftL, const ExprLabel& rightL, ExprLabel& result);

	/** TO BE DEFINED (by the subclass) */
	void sub_V_fwd(const ExprSub&, const ExprLabel& leftL, const ExprLabel& rightL, ExprLabel& result);

	/** TO BE DEFINED (by the subclass) */
	void sub_M_fwd(const ExprSub&, const ExprLabel& leftL, const ExprLabel& rightL, ExprLabel& result);

	/** TO BE DEFINED (by the subclass) */
	void div_fwd(const ExprDiv&, const ExprLabel& leftL, const ExprLabel& rightL, ExprLabel& result);

	/** TO BE DEFINED (by the subclass) */
	void max_fwd(const ExprMax&, const ExprLabel& leftL, const ExprLabel& rightL, ExprLabel& result);

	/** TO BE DEFINED (by the subclass) */
	void min_fwd(const ExprMin&, const ExprLabel& leftL, const ExprLabel& rightL, ExprLabel& result);

	/** TO BE DEFINED (by the subclass) */
	void atan2_fwd(const ExprAtan2&, const ExprLabel& leftL, const ExprLabel& rightL, ExprLabel& result);

	/*==================== unary operators =========================*/
	/** TO BE DEFINED (by the subclass) */
	void minus_fwd(const ExprMinus&, const ExprLabel& exprL, ExprLabel& result);

	/** TO BE DEFINED (by the subclass) */
	void trans_V_fwd(const ExprTrans&, const ExprLabel& exprL, ExprLabel& result);

	/** TO BE DEFINED (by the subclass) */
	void trans_M_fwd(const ExprTrans&, const ExprLabel& exprL, ExprLabel& result);

	/** TO BE DEFINED (by the subclass) */
	void sign_fwd(const ExprSign&, const ExprLabel& exprL, ExprLabel& result);

	/** TO BE DEFINED (by the subclass) */
	void abs_fwd(const ExprAbs&, const ExprLabel& exprL, ExprLabel& result);

	/** TO BE DEFINED (by the subclass) */
	void power_fwd(const ExprPower&, const ExprLabel& exprL, ExprLabel& result);

	/** TO BE DEFINED (by the subclass) */
	void sqr_fwd(const ExprSqr&, const ExprLabel& exprL, ExprLabel& result);

	/** TO BE DEFINED (by the subclass) */
	void sqrt_fwd(const ExprSqrt&, const ExprLabel& exprL, ExprLabel& result);

	/** TO BE DEFINED (by the subclass) */
	void exp_fwd(const ExprExp&, const ExprLabel& exprL, ExprLabel& result);

	/** TO BE DEFINED (by the subclass) */
	void log_fwd(const ExprLog&, const ExprLabel& exprL, ExprLabel& result);

	/** TO BE DEFINED (by the subclass) */
	void cos_fwd(const ExprCos&, const ExprLabel& exprL, ExprLabel& result);

	/** TO BE DEFINED (by the subclass) */
	void sin_fwd(const ExprSin&, const ExprLabel& exprL, ExprLabel& result);

	/** TO BE DEFINED (by the subclass) */
	void tan_fwd(const ExprTan&, const ExprLabel& exprL, ExprLabel& result);

	/** TO BE DEFINED (by the subclass) */
	void cosh_fwd(const ExprCosh&, const ExprLabel& exprL, ExprLabel& result);

	/** TO BE DEFINED (by the subclass) */
	void sinh_fwd(const ExprSinh&, const ExprLabel& exprL, ExprLabel& result);

	/** TO BE DEFINED (by the subclass) */
	void tanh_fwd(const ExprTanh&, const ExprLabel& exprL, ExprLabel& result);

	/** TO BE DEFINED (by the subclass) */
	void acos_fwd(const ExprAcos&, const ExprLabel& exprL, ExprLabel& result);

	/** TO BE DEFINED (by the subclass) */
	void asin_fwd(const ExprAsin&, const ExprLabel& exprL, ExprLabel& result);

	/** TO BE DEFINED (by the subclass) */
	void atan_fwd(const ExprAtan&, const ExprLabel& exprL, ExprLabel& result);

	/** TO BE DEFINED (by the subclass) */
	void acosh_fwd(const ExprAcosh&, const ExprLabel& exprL, ExprLabel& result);

	/** TO BE DEFINED (by the subclass) */
	void asinh_fwd(const ExprAsinh&, const ExprLabel& exprL, ExprLabel& result);

	/** TO BE DEFINED (by the subclass) */
	void atanh_fwd(const ExprAtanh&, const ExprLabel& exprL, ExprLabel& result);
};


} // namespace ibex
#endif // _IBEX_FWD_ALGORITHM_H_
