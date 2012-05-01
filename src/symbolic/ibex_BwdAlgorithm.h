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
 * \ingroup level1
 * \brief Interface for backward Algorithms.
 */
template<typename T>
class BwdAlgorithm {

protected:
	/** TO BE DEFINED (by the subclass) */
	void index_bwd(const ExprIndex&, T& exprL, const T& result);

	/** TO BE DEFINED (by the subclass) */
	void vector_bwd(const ExprVector&, T** compL, const T& result);

	/** TO BE DEFINED (by the subclass) */
	void symbol_bwd(const ExprSymbol&, const T& result);

	/** TO BE DEFINED (by the subclass) */
	void cst_bwd(const ExprConstant&, const T& result);

	/** TO BE DEFINED (by the subclass) */
	void apply_bwd(const ExprApply&, T** argL, const T& result);

	/*==================== binary operators =========================*/
	/** TO BE DEFINED (by the subclass) */
	void add_bwd(const ExprAdd&, T& leftL, T& rightL, const T& result);

	/** TO BE DEFINED (by the subclass) */
	void add_V_bwd(const ExprAdd&, T& leftL, T& rightL, const T& result);

	/** TO BE DEFINED (by the subclass) */
	void add_M_bwd(const ExprAdd&, T& leftL, T& rightL, const T& result);

	/** TO BE DEFINED (by the subclass) */
	void mul_bwd(const ExprMul&, T& leftL, T& rightL, const T& result);

	/** TO BE DEFINED (by the subclass) */
	void mul_SV_bwd(const ExprMul&, T& leftL, T& rightL, const T& result);

	/** TO BE DEFINED (by the subclass) */
	void mul_SM_bwd(const ExprMul&, T& leftL, T& rightL, const T& result);

	/** TO BE DEFINED (by the subclass) */
	void mul_VV_bwd(const ExprMul&, T& leftL, T& rightL, const T& result);

	/** TO BE DEFINED (by the subclass) */
	void mul_MV_bwd(const ExprMul&, T& leftL, T& rightL, const T& result);

	/** TO BE DEFINED (by the subclass) */
	void mul_MM_bwd(const ExprMul&, T& leftL, T& rightL, const T& result);

	/** TO BE DEFINED (by the subclass) */
	void sub_bwd(const ExprSub&, T& leftL, T& rightL, const T& result);

	/** TO BE DEFINED (by the subclass) */
	void sub_V_bwd(const ExprSub&, T& leftL, T& rightL, const T& result);

	/** TO BE DEFINED (by the subclass) */
	void sub_M_bwd(const ExprSub&, T& leftL, T& rightL, const T& result);

	/** TO BE DEFINED (by the subclass) */
	void div_bwd(const ExprDiv&, T& leftL, T& rightL, const T& result);

	/** TO BE DEFINED (by the subclass) */
	void max_bwd(const ExprMax&, T& leftL, T& rightL, const T& result);

	/** TO BE DEFINED (by the subclass) */
	void min_bwd(const ExprMin&, T& leftL, T& rightL, const T& result);

	/** TO BE DEFINED (by the subclass) */
	void atan2_bwd(const ExprAtan2&, T& leftL, T& rightL, const T& result);

	/** TO BE DEFINED (by the subclass) */
	void minus_bwd(const ExprMinus&, T& exprL, const T& result);

	/** TO BE DEFINED (by the subclass) */
	void sign_bwd(const ExprSign&, T& exprL, const T& result);

	/** TO BE DEFINED (by the subclass) */
	void abs_bwd(const ExprAbs&, T& exprL, const T& result);

	/** TO BE DEFINED (by the subclass) */
	void power_bwd(const ExprPower&, T& exprL, const T& result);

	/** TO BE DEFINED (by the subclass) */
	void sqr_bwd(const ExprSqr&, T& exprL, const T& result);

	/** TO BE DEFINED (by the subclass) */
	void sqrt_bwd(const ExprSqrt&, T& exprL, const T& result);

	/** TO BE DEFINED (by the subclass) */
	void exp_bwd(const ExprExp&, T& exprL, const T& result);

	/** TO BE DEFINED (by the subclass) */
	void log_bwd(const ExprLog&, T& exprL, const T& result);

	/** TO BE DEFINED (by the subclass) */
	void cos_bwd(const ExprCos&, T& exprL, const T& result);

	/** TO BE DEFINED (by the subclass) */
	void sin_bwd(const ExprSin&, T& exprL, const T& result);

	/** TO BE DEFINED (by the subclass) */
	void tan_bwd(const ExprTan&, T& exprL, const T& result);

	/** TO BE DEFINED (by the subclass) */
	void cosh_bwd(const ExprCosh&, T& exprL, const T& result);

	/** TO BE DEFINED (by the subclass) */
	void sinh_bwd(const ExprSinh&, T& exprL, const T& result);

	/** TO BE DEFINED (by the subclass) */
	void tanh_bwd(const ExprTanh&, T& exprL, const T& result);

	/** TO BE DEFINED (by the subclass) */
	void acos_bwd(const ExprAcos&, T& exprL, const T& result);

	/** TO BE DEFINED (by the subclass) */
	void asin_bwd(const ExprAsin&, T& exprL, const T& result);

	/** TO BE DEFINED (by the subclass) */
	void atan_bwd(const ExprAtan&, T& exprL, const T& result);

	/** TO BE DEFINED (by the subclass) */
	void acosh_bwd(const ExprAcosh&, T& exprL, const T& result);

	/** TO BE DEFINED (by the subclass) */
	void asinh_bwd(const ExprAsinh&, T& exprL, const T& result);

	/** TO BE DEFINED (by the subclass) */
	void atanh_bwd(const ExprAtanh&, T& exprL, const T& result);
};


} // namespace ibex
#endif // _IBEX_BWD_ALGORITHM_H_
