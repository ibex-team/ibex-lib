/* ============================================================================
 * I B E X - Interface for backward algorithms
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Jan 22, 2012
 * ---------------------------------------------------------------------------- */

#ifndef _IBEX_BWD_ALGORITHM_H_
#define _IBEX_BWD_ALGORITHM_H_

#include "ibex_Expr.h"

namespace ibex {

/**
 * \ingroup symbolic
 * \brief Interface for backward Algorithms.
 */
class BwdAlgorithm {

protected:
	/** TO BE DEFINED (by the subclass) */
	void index_bwd(const ExprIndex&, ExprLabel& exprL, const ExprLabel& result);

	/** TO BE DEFINED (by the subclass) */
	void vector_bwd(const ExprVector&, ExprLabel** compL, const ExprLabel& result);

	/** TO BE DEFINED (by the subclass) */
	void symbol_bwd(const ExprSymbol&, const ExprLabel& result);

	/** TO BE DEFINED (by the subclass) */
	void cst_bwd(const ExprConstant&, const ExprLabel& result);

	/** TO BE DEFINED (by the subclass) */
	void apply_bwd(const ExprApply&, ExprLabel** argL, const ExprLabel& result);

	/** TO BE DEFINED (by the subclass) */
	void chi_bwd(const ExprChi&,  ExprLabel& a, ExprLabel& b, ExprLabel& c, const ExprLabel& result);

	/*==================== binary operators =========================*/
	/** TO BE DEFINED (by the subclass) */
	void add_bwd(const ExprAdd&, ExprLabel& leftL, ExprLabel& rightL, const ExprLabel& result);

	/** TO BE DEFINED (by the subclass) */
	void add_V_bwd(const ExprAdd&, ExprLabel& leftL, ExprLabel& rightL, const ExprLabel& result);

	/** TO BE DEFINED (by the subclass) */
	void add_M_bwd(const ExprAdd&, ExprLabel& leftL, ExprLabel& rightL, const ExprLabel& result);

	/** TO BE DEFINED (by the subclass) */
	void mul_bwd(const ExprMul&, ExprLabel& leftL, ExprLabel& rightL, const ExprLabel& result);

	/** TO BE DEFINED (by the subclass) */
	void mul_SV_bwd(const ExprMul&, ExprLabel& leftL, ExprLabel& rightL, const ExprLabel& result);

	/** TO BE DEFINED (by the subclass) */
	void mul_SM_bwd(const ExprMul&, ExprLabel& leftL, ExprLabel& rightL, const ExprLabel& result);

	/** TO BE DEFINED (by the subclass) */
	void mul_VV_bwd(const ExprMul&, ExprLabel& leftL, ExprLabel& rightL, const ExprLabel& result);

	/** TO BE DEFINED (by the subclass) */
	void mul_MV_bwd(const ExprMul&, ExprLabel& leftL, ExprLabel& rightL, const ExprLabel& result);

	/** TO BE DEFINED (by the subclass) */
	void mul_VM_bwd(const ExprMul&, ExprLabel& leftL, ExprLabel& rightL, const ExprLabel& result);

	/** TO BE DEFINED (by the subclass) */
	void mul_MM_bwd(const ExprMul&, ExprLabel& leftL, ExprLabel& rightL, const ExprLabel& result);

	/** TO BE DEFINED (by the subclass) */
	void sub_bwd(const ExprSub&, ExprLabel& leftL, ExprLabel& rightL, const ExprLabel& result);

	/** TO BE DEFINED (by the subclass) */
	void sub_V_bwd(const ExprSub&, ExprLabel& leftL, ExprLabel& rightL, const ExprLabel& result);

	/** TO BE DEFINED (by the subclass) */
	void sub_M_bwd(const ExprSub&, ExprLabel& leftL, ExprLabel& rightL, const ExprLabel& result);

	/** TO BE DEFINED (by the subclass) */
	void div_bwd(const ExprDiv&, ExprLabel& leftL, ExprLabel& rightL, const ExprLabel& result);

	/** TO BE DEFINED (by the subclass) */
	void max_bwd(const ExprMax&, ExprLabel& leftL, ExprLabel& rightL, const ExprLabel& result);

	/** TO BE DEFINED (by the subclass) */
	void min_bwd(const ExprMin&, ExprLabel& leftL, ExprLabel& rightL, const ExprLabel& result);

	/** TO BE DEFINED (by the subclass) */
	void atan2_bwd(const ExprAtan2&, ExprLabel& leftL, ExprLabel& rightL, const ExprLabel& result);

	/*==================== unary operators =========================*/

	/** TO BE DEFINED (by the subclass) */
	void minus_bwd(const ExprMinus&, ExprLabel& exprL, const ExprLabel& result);

	/** TO BE DEFINED (by the subclass) */
	void trans_V_bwd(const ExprTrans&, ExprLabel& exprL, const ExprLabel& result);

	/** TO BE DEFINED (by the subclass) */
	void trans_M_bwd(const ExprTrans&, ExprLabel& exprL, const ExprLabel& result);

	/** TO BE DEFINED (by the subclass) */
	void sign_bwd(const ExprSign&, ExprLabel& exprL, const ExprLabel& result);

	/** TO BE DEFINED (by the subclass) */
	void abs_bwd(const ExprAbs&, ExprLabel& exprL, const ExprLabel& result);

	/** TO BE DEFINED (by the subclass) */
	void power_bwd(const ExprPower&, ExprLabel& exprL, const ExprLabel& result);

	/** TO BE DEFINED (by the subclass) */
	void sqr_bwd(const ExprSqr&, ExprLabel& exprL, const ExprLabel& result);

	/** TO BE DEFINED (by the subclass) */
	void sqrt_bwd(const ExprSqrt&, ExprLabel& exprL, const ExprLabel& result);

	/** TO BE DEFINED (by the subclass) */
	void exp_bwd(const ExprExp&, ExprLabel& exprL, const ExprLabel& result);

	/** TO BE DEFINED (by the subclass) */
	void log_bwd(const ExprLog&, ExprLabel& exprL, const ExprLabel& result);

	/** TO BE DEFINED (by the subclass) */
	void cos_bwd(const ExprCos&, ExprLabel& exprL, const ExprLabel& result);

	/** TO BE DEFINED (by the subclass) */
	void sin_bwd(const ExprSin&, ExprLabel& exprL, const ExprLabel& result);

	/** TO BE DEFINED (by the subclass) */
	void tan_bwd(const ExprTan&, ExprLabel& exprL, const ExprLabel& result);

	/** TO BE DEFINED (by the subclass) */
	void cosh_bwd(const ExprCosh&, ExprLabel& exprL, const ExprLabel& result);

	/** TO BE DEFINED (by the subclass) */
	void sinh_bwd(const ExprSinh&, ExprLabel& exprL, const ExprLabel& result);

	/** TO BE DEFINED (by the subclass) */
	void tanh_bwd(const ExprTanh&, ExprLabel& exprL, const ExprLabel& result);

	/** TO BE DEFINED (by the subclass) */
	void acos_bwd(const ExprAcos&, ExprLabel& exprL, const ExprLabel& result);

	/** TO BE DEFINED (by the subclass) */
	void asin_bwd(const ExprAsin&, ExprLabel& exprL, const ExprLabel& result);

	/** TO BE DEFINED (by the subclass) */
	void atan_bwd(const ExprAtan&, ExprLabel& exprL, const ExprLabel& result);

	/** TO BE DEFINED (by the subclass) */
	void acosh_bwd(const ExprAcosh&, ExprLabel& exprL, const ExprLabel& result);

	/** TO BE DEFINED (by the subclass) */
	void asinh_bwd(const ExprAsinh&, ExprLabel& exprL, const ExprLabel& result);

	/** TO BE DEFINED (by the subclass) */
	void atanh_bwd(const ExprAtanh&, ExprLabel& exprL, const ExprLabel& result);
};


} // namespace ibex
#endif // _IBEX_BWD_ALGORITHM_H_
