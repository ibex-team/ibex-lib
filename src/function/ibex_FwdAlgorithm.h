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

#ifndef __IBEX_FWD_ALGORITHM_H__
#define __IBEX_FWD_ALGORITHM_H__

#include "ibex_Expr.h"

namespace ibex {

/**
 * \ingroup symbolic
 * \brief Interface for forward algorithms.
 */
class FwdAlgorithm {
protected:

	/** TO BE DEFINED (by the subclass) */
	void idx_fwd(int x, int y);

	/** TO BE DEFINED (by the subclass) */
	void idx_cp_fwd(int x, int y);

	/** TO BE DEFINED (by the subclass) */
	void symbol_fwd(int y);

	/** TO BE DEFINED (by the subclass) */
	void cst_fwd(int y);

	/*==================== n-ary operators =========================*/
	/** TO BE DEFINED (by the subclass) */
	void vector_fwd(int* x, int y);

	/** TO BE DEFINED (by the subclass) */
	void apply_fwd(int* x, int y);

	/** TO BE DEFINED (by the subclass) */
	void chi_fwd(int a, int b, int c, int y);

	/*==================== binary operators =========================*/
	/** TO BE DEFINED (by the subclass) */
	void gen2_fwd(int x, int y);

	/** TO BE DEFINED (by the subclass) */
	void add_fwd(int x1, int x2, int y);

	/** TO BE DEFINED (by the subclass) */
	void add_V_fwd(int x1, int x2, int y);

	/** TO BE DEFINED (by the subclass) */
	void add_M_fwd(int x1, int x2, int y);

	/** TO BE DEFINED (by the subclass) */
	void mul_fwd(int x1, int x2, int y);

	/** TO BE DEFINED (by the subclass) */
	void mul_SV_fwd(int x1, int x2, int y);

	/** TO BE DEFINED (by the subclass) */
	void mul_SM_fwd(int x1, int x2, int y);

	/** TO BE DEFINED (by the subclass) */
	void mul_VV_fwd(int x1, int x2, int y);

	/** TO BE DEFINED (by the subclass) */
	void mul_MV_fwd(int x1, int x2, int y);

	/** TO BE DEFINED (by the subclass) */
	void mul_VM_fwd(int x1, int x2, int y);

	/** TO BE DEFINED (by the subclass) */
	void mul_MM_fwd(int x1, int x2, int y);

	/** TO BE DEFINED (by the subclass) */
	void sub_fwd(int x1, int x2, int y);

	/** TO BE DEFINED (by the subclass) */
	void sub_V_fwd(int x1, int x2, int y);

	/** TO BE DEFINED (by the subclass) */
	void sub_M_fwd(int x1, int x2, int y);

	/** TO BE DEFINED (by the subclass) */
	void div_fwd(int x1, int x2, int y);

	/** TO BE DEFINED (by the subclass) */
	void max_fwd(int x1, int x2, int y);

	/** TO BE DEFINED (by the subclass) */
	void min_fwd(int x1, int x2, int y);

	/** TO BE DEFINED (by the subclass) */
	void atan2_fwd(int x1, int x2, int y);

	/*==================== unary operators =========================*/
	/** TO BE DEFINED (by the subclass) */
	void gen1_fwd(int x, int y);

	/** TO BE DEFINED (by the subclass) */
	void minus_fwd(int x, int y);

	/** TO BE DEFINED (by the subclass) */
	void minus_V_fwd(int x, int y);

	/** TO BE DEFINED (by the subclass) */
	void minus_M_fwd(int x, int y);

	/** TO BE DEFINED (by the subclass) */
	void trans_V_fwd(int x, int y);

	/** TO BE DEFINED (by the subclass) */
	void trans_M_fwd(int x, int y);

	/** TO BE DEFINED (by the subclass) */
	void sign_fwd(int x, int y);

	/** TO BE DEFINED (by the subclass) */
	void abs_fwd(int x, int y);

	/** TO BE DEFINED (by the subclass) */
	void power_fwd(int x, int y, int p);

	/** TO BE DEFINED (by the subclass) */
	void sqr_fwd(int x, int y);

	/** TO BE DEFINED (by the subclass) */
	void sqrt_fwd(int x, int y);

	/** TO BE DEFINED (by the subclass) */
	void exp_fwd(int x, int y);

	/** TO BE DEFINED (by the subclass) */
	void log_fwd(int x, int y);

	/** TO BE DEFINED (by the subclass) */
	void cos_fwd(int x, int y);

	/** TO BE DEFINED (by the subclass) */
	void sin_fwd(int x, int y);

	/** TO BE DEFINED (by the subclass) */
	void tan_fwd(int x, int y);

	/** TO BE DEFINED (by the subclass) */
	void cosh_fwd(int x, int y);

	/** TO BE DEFINED (by the subclass) */
	void sinh_fwd(int x, int y);

	/** TO BE DEFINED (by the subclass) */
	void tanh_fwd(int x, int y);

	/** TO BE DEFINED (by the subclass) */
	void acos_fwd(int x, int y);

	/** TO BE DEFINED (by the subclass) */
	void asin_fwd(int x, int y);

	/** TO BE DEFINED (by the subclass) */
	void atan_fwd(int x, int y);

	/** TO BE DEFINED (by the subclass) */
	void acosh_fwd(int x, int y);

	/** TO BE DEFINED (by the subclass) */
	void asinh_fwd(int x, int y);

	/** TO BE DEFINED (by the subclass) */
	void atanh_fwd(int x, int y);
};

} // namespace ibex

#endif // __IBEX_FWD_ALGORITHM_H__
