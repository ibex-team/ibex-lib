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
	void idx_bwd(int x, int y);

	/** TO BE DEFINED (by the subclass) */
	void idx_cp_bwd(int x, int y);

	/** TO BE DEFINED (by the subclass) */
	void vector_bwd(int* x, int y);

	/** TO BE DEFINED (by the subclass) */
	void symbol_bwd(int y);

	/** TO BE DEFINED (by the subclass) */
	void cst_bwd(int y);

	/** TO BE DEFINED (by the subclass) */
	void apply_bwd(int* x, int y);

	/** TO BE DEFINED (by the subclass) */
	void chi_bwd(  int a, int b, int c, int y);

	/*==================== binary operators =========================*/
	/** TO BE DEFINED (by the subclass) */
	void gen2_bwd(int x, int y);

	/** TO BE DEFINED (by the subclass) */
	void add_bwd(int x1, int x2, int y);

	/** TO BE DEFINED (by the subclass) */
	void add_V_bwd(int x1, int x2, int y);

	/** TO BE DEFINED (by the subclass) */
	void add_M_bwd(int x1, int x2, int y);

	/** TO BE DEFINED (by the subclass) */
	void mul_bwd(int x1, int x2, int y);

	/** TO BE DEFINED (by the subclass) */
	void mul_SV_bwd(int x1, int x2, int y);

	/** TO BE DEFINED (by the subclass) */
	void mul_SM_bwd(int x1, int x2, int y);

	/** TO BE DEFINED (by the subclass) */
	void mul_VV_bwd(int x1, int x2, int y);

	/** TO BE DEFINED (by the subclass) */
	void mul_MV_bwd(int x1, int x2, int y);

	/** TO BE DEFINED (by the subclass) */
	void mul_VM_bwd(int x1, int x2, int y);

	/** TO BE DEFINED (by the subclass) */
	void mul_MM_bwd(int x1, int x2, int y);

	/** TO BE DEFINED (by the subclass) */
	void sub_bwd(int x1, int x2, int y);

	/** TO BE DEFINED (by the subclass) */
	void sub_V_bwd(int x1, int x2, int y);

	/** TO BE DEFINED (by the subclass) */
	void sub_M_bwd(int x1, int x2, int y);

	/** TO BE DEFINED (by the subclass) */
	void div_bwd(int x1, int x2, int y);

	/** TO BE DEFINED (by the subclass) */
	void max_bwd(int x1, int x2, int y);

	/** TO BE DEFINED (by the subclass) */
	void min_bwd(int x1, int x2, int y);

	/** TO BE DEFINED (by the subclass) */
	void atan2_bwd(int x1, int x2, int y);

	/*==================== unary operators =========================*/

	/** TO BE DEFINED (by the subclass) */
	void gen1_bwd(int x, int y);

	/** TO BE DEFINED (by the subclass) */
	void minus_bwd(int x, int y);

	/** TO BE DEFINED (by the subclass) */
	void minus_V_bwd(int x, int y);

	/** TO BE DEFINED (by the subclass) */
	void minus_M_bwd(int x, int y);

	/** TO BE DEFINED (by the subclass) */
	void trans_V_bwd(int x, int y);

	/** TO BE DEFINED (by the subclass) */
	void trans_M_bwd(int x, int y);

	/** TO BE DEFINED (by the subclass) */
	void sign_bwd(int x, int y);

	/** TO BE DEFINED (by the subclass) */
	void abs_bwd(int x, int y);

	/** TO BE DEFINED (by the subclass) */
	void power_bwd(int x, int y, int p);

	/** TO BE DEFINED (by the subclass) */
	void sqr_bwd(int x, int y);

	/** TO BE DEFINED (by the subclass) */
	void sqrt_bwd(int x, int y);

	/** TO BE DEFINED (by the subclass) */
	void exp_bwd(int x, int y);

	/** TO BE DEFINED (by the subclass) */
	void log_bwd(int x, int y);

	/** TO BE DEFINED (by the subclass) */
	void cos_bwd(int x, int y);

	/** TO BE DEFINED (by the subclass) */
	void sin_bwd(int x, int y);

	/** TO BE DEFINED (by the subclass) */
	void tan_bwd(int x, int y);

	/** TO BE DEFINED (by the subclass) */
	void cosh_bwd(int x, int y);

	/** TO BE DEFINED (by the subclass) */
	void sinh_bwd(int x, int y);

	/** TO BE DEFINED (by the subclass) */
	void tanh_bwd(int x, int y);

	/** TO BE DEFINED (by the subclass) */
	void acos_bwd(int x, int y);

	/** TO BE DEFINED (by the subclass) */
	void asin_bwd(int x, int y);

	/** TO BE DEFINED (by the subclass) */
	void atan_bwd(int x, int y);

	/** TO BE DEFINED (by the subclass) */
	void acosh_bwd(int x, int y);

	/** TO BE DEFINED (by the subclass) */
	void asinh_bwd(int x, int y);

	/** TO BE DEFINED (by the subclass) */
	void atanh_bwd(int x, int y);
};

} // namespace ibex

#endif // __IBEX_BWD_ALGORITHM_H__
