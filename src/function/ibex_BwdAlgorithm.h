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

#ifndef __IBEX_BWD_ALGORITHM_H__
#define __IBEX_BWD_ALGORITHM_H__

#include "ibex_Expr.h"

namespace ibex {

/**
 * \ingroup symbolic
 * \brief Interface for backward Algorithms.
 */
class BwdAlgorithm {

protected:
	/** TO BE DEFINED (by the subclass) */
	void idx_bwd(const ExprIndex&, int x, int y);

	/** TO BE DEFINED (by the subclass) */
	void idx_cp_bwd(const ExprIndex&, int x, int y);

	/** TO BE DEFINED (by the subclass) */
	void vector_bwd(const ExprVector&, int* x, int y);

	/** TO BE DEFINED (by the subclass) */
	void symbol_bwd(const ExprSymbol&, int y);

	/** TO BE DEFINED (by the subclass) */
	void cst_bwd(const ExprConstant&, int y);

	/** TO BE DEFINED (by the subclass) */
	void apply_bwd(const ExprApply&, int* x, int y);

	/** TO BE DEFINED (by the subclass) */
	void chi_bwd(const ExprChi&,  int a, int b, int c, int y);

	/*==================== binary operators =========================*/
	/** TO BE DEFINED (by the subclass) */
	void add_bwd(const ExprAdd&, int x1, int x2, int y);

	/** TO BE DEFINED (by the subclass) */
	void add_V_bwd(const ExprAdd&, int x1, int x2, int y);

	/** TO BE DEFINED (by the subclass) */
	void add_M_bwd(const ExprAdd&, int x1, int x2, int y);

	/** TO BE DEFINED (by the subclass) */
	void mul_bwd(const ExprMul&, int x1, int x2, int y);

	/** TO BE DEFINED (by the subclass) */
	void mul_SV_bwd(const ExprMul&, int x1, int x2, int y);

	/** TO BE DEFINED (by the subclass) */
	void mul_SM_bwd(const ExprMul&, int x1, int x2, int y);

	/** TO BE DEFINED (by the subclass) */
	void mul_VV_bwd(const ExprMul&, int x1, int x2, int y);

	/** TO BE DEFINED (by the subclass) */
	void mul_MV_bwd(const ExprMul&, int x1, int x2, int y);

	/** TO BE DEFINED (by the subclass) */
	void mul_VM_bwd(const ExprMul&, int x1, int x2, int y);

	/** TO BE DEFINED (by the subclass) */
	void mul_MM_bwd(const ExprMul&, int x1, int x2, int y);

	/** TO BE DEFINED (by the subclass) */
	void sub_bwd(const ExprSub&, int x1, int x2, int y);

	/** TO BE DEFINED (by the subclass) */
	void sub_V_bwd(const ExprSub&, int x1, int x2, int y);

	/** TO BE DEFINED (by the subclass) */
	void sub_M_bwd(const ExprSub&, int x1, int x2, int y);

	/** TO BE DEFINED (by the subclass) */
	void div_bwd(const ExprDiv&, int x1, int x2, int y);

	/** TO BE DEFINED (by the subclass) */
	void max_bwd(const ExprMax&, int x1, int x2, int y);

	/** TO BE DEFINED (by the subclass) */
	void min_bwd(const ExprMin&, int x1, int x2, int y);

	/** TO BE DEFINED (by the subclass) */
	void atan2_bwd(const ExprAtan2&, int x1, int x2, int y);

	/*==================== unary operators =========================*/

	/** TO BE DEFINED (by the subclass) */
	void minus_bwd(const ExprMinus&, int x, int y);

	/** TO BE DEFINED (by the subclass) */
	void trans_V_bwd(const ExprTrans&, int x, int y);

	/** TO BE DEFINED (by the subclass) */
	void trans_M_bwd(const ExprTrans&, int x, int y);

	/** TO BE DEFINED (by the subclass) */
	void sign_bwd(const ExprSign&, int x, int y);

	/** TO BE DEFINED (by the subclass) */
	void abs_bwd(const ExprAbs&, int x, int y);

	/** TO BE DEFINED (by the subclass) */
	void power_bwd(const ExprPower&, int x, int y, int p);

	/** TO BE DEFINED (by the subclass) */
	void sqr_bwd(const ExprSqr&, int x, int y);

	/** TO BE DEFINED (by the subclass) */
	void sqrt_bwd(const ExprSqrt&, int x, int y);

	/** TO BE DEFINED (by the subclass) */
	void exp_bwd(const ExprExp&, int x, int y);

	/** TO BE DEFINED (by the subclass) */
	void log_bwd(const ExprLog&, int x, int y);

	/** TO BE DEFINED (by the subclass) */
	void cos_bwd(const ExprCos&, int x, int y);

	/** TO BE DEFINED (by the subclass) */
	void sin_bwd(const ExprSin&, int x, int y);

	/** TO BE DEFINED (by the subclass) */
	void tan_bwd(const ExprTan&, int x, int y);

	/** TO BE DEFINED (by the subclass) */
	void cosh_bwd(const ExprCosh&, int x, int y);

	/** TO BE DEFINED (by the subclass) */
	void sinh_bwd(const ExprSinh&, int x, int y);

	/** TO BE DEFINED (by the subclass) */
	void tanh_bwd(const ExprTanh&, int x, int y);

	/** TO BE DEFINED (by the subclass) */
	void acos_bwd(const ExprAcos&, int x, int y);

	/** TO BE DEFINED (by the subclass) */
	void asin_bwd(const ExprAsin&, int x, int y);

	/** TO BE DEFINED (by the subclass) */
	void atan_bwd(const ExprAtan&, int x, int y);

	/** TO BE DEFINED (by the subclass) */
	void acosh_bwd(const ExprAcosh&, int x, int y);

	/** TO BE DEFINED (by the subclass) */
	void asinh_bwd(const ExprAsinh&, int x, int y);

	/** TO BE DEFINED (by the subclass) */
	void atanh_bwd(const ExprAtanh&, int x, int y);
};

} // namespace ibex

#endif // __IBEX_BWD_ALGORITHM_H__
