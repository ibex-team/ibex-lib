//============================================================================
//                                  I B E X                                   
// File        : HC4Revise Algorithm (The famous forward-backward contraction algorithm).
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Dec 31, 2011
// Last Update : 
//============================================================================

#ifndef __IBEX_HC4_REVISE_H__
#define __IBEX_HC4_REVISE_H__

#include "ibex_Eval.h"

namespace ibex {

/**
 * \ingroup symbolic
 * \brief The famous forward-backward contraction algorithm.
 *
 */
class HC4Revise : public BwdAlgorithm {
public:

	/**
	 * \brief Build the gradient algorithm.
	 *
	 * For memory saving, the gradient is built from an
	 * already existing evaluator "e".
	 */
	HC4Revise(Eval& e);

	/**
	 * \brief Project f(x)=y onto x (forward/backward algorithm)
	 *
	 * \brief true if f(x) is included in y (inactive constraint)
	 *
	 * \note if x is outside the definition domain of f, then
	 *       x is set to the empty set although f([x])\subseteq [y]
	 *       and the return value is "false".
	 */
	bool proj(const Domain& y, IntervalVector& x);

	/**
	 * \brief Ratio for the contraction of a
	 * matrix-vector / matrix-matrix multiplication.
	 *
	 * Set to 0.1.
	 */
	static constexpr double RATIO = 0.1;

protected:
	/**
	 * Class used internally to interrupt the
	 * backward procedure when an empty domain occurs.
	 */
	class EmptyBoxException { };

	/**
	 * Contract x w.r.t. f(x)=y, with forward + backward.
	 *
	 * \warning: may throw EmptyBoxException
	 */
//	bool proj(const Domain& y, const Array<const Domain>& x);

	/**
	 * Contract x w.r.t. f(x)=y, with forward + backward.
	 *
	 * \warning: may throw EmptyBoxException
	 */
	bool proj(const Domain& y, Array<Domain>& x);

	/**
	 * Backward of f(x)=y.
	 *
	 * \warning: may throw EmptyBoxException
	 */
	bool backward(const Domain& y);

	Function& f;
	Eval& eval;
	ExprDomain& d;

public: // because called from CompiledFunction
	inline void idx_bwd    (int, int)          { /* nothing to do */ }
	       void idx_cp_bwd (int, int);
	       void vector_bwd (int* x, int y);
	inline void symbol_bwd (int)                 { /* nothing to do */ }
	inline void cst_bwd    (int)                 { /* nothing to do */ }
	       void apply_bwd  (int* x, int y);
	inline void chi_bwd(int a, int b, int c, int y){ if (!(bwd_chi(d[y].i(),d[a].i(),d[b].i(),d[c].i()))) throw EmptyBoxException();  }
	       void gen2_bwd   (int x1, int x2, int y);
	inline void add_bwd    (int x1, int x2, int y) { if (!(bwd_add(d[y].i(),d[x1].i(),d[x2].i()))) throw EmptyBoxException();  }
	inline void add_V_bwd  (int x1, int x2, int y) { if (!(bwd_add(d[y].v(),d[x1].v(),d[x2].v()))) throw EmptyBoxException();  }
	inline void add_M_bwd  (int x1, int x2, int y) { if (!(bwd_add(d[y].m(),d[x1].m(),d[x2].m()))) throw EmptyBoxException();  }
	inline void mul_bwd    (int x1, int x2, int y) { if (!(bwd_mul(d[y].i(),d[x1].i(),d[x2].i()))) throw EmptyBoxException();  }
	inline void mul_SV_bwd (int x1, int x2, int y) { if (!(bwd_mul(d[y].v(),d[x1].i(),d[x2].v()))) throw EmptyBoxException();  }
	inline void mul_SM_bwd (int x1, int x2, int y) { if (!(bwd_mul(d[y].m(),d[x1].i(),d[x2].m()))) throw EmptyBoxException();  }
	inline void mul_VV_bwd (int x1, int x2, int y) { if (!(bwd_mul(d[y].i(),d[x1].v(),d[x2].v()))) throw EmptyBoxException();  }
	inline void mul_MV_bwd (int x1, int x2, int y) { if (!(bwd_mul(d[y].v(),d[x1].m(),d[x2].v(), RATIO))) throw EmptyBoxException();  }
	inline void mul_VM_bwd (int x1, int x2, int y) { if (!(bwd_mul(d[y].v(),d[x1].v(),d[x2].m(), RATIO))) throw EmptyBoxException();  }
	inline void mul_MM_bwd (int x1, int x2, int y) { if (!(bwd_mul(d[y].m(),d[x1].m(),d[x2].m(), RATIO))) throw EmptyBoxException();  }
	inline void sub_bwd    (int x1, int x2, int y) { if (!(bwd_sub(d[y].i(),d[x1].i(),d[x2].i()))) throw EmptyBoxException();  }
	inline void sub_V_bwd  (int x1, int x2, int y) { if (!(bwd_sub(d[y].v(),d[x1].v(),d[x2].v()))) throw EmptyBoxException();  }
	inline void sub_M_bwd  (int x1, int x2, int y) { if (!(bwd_sub(d[y].m(),d[x1].m(),d[x2].m()))) throw EmptyBoxException();  }
	inline void div_bwd    (int x1, int x2, int y) { if (!(bwd_div(d[y].i(),d[x1].i(),d[x2].i()))) throw EmptyBoxException();  }
	inline void max_bwd    (int x1, int x2, int y) { if (!(bwd_max(d[y].i(),d[x1].i(),d[x2].i()))) throw EmptyBoxException();  }
	inline void min_bwd    (int x1, int x2, int y) { if (!(bwd_min(d[y].i(),d[x1].i(),d[x2].i()))) throw EmptyBoxException();  }
	inline void atan2_bwd  (int x1, int x2, int y) { if (!(bwd_atan2(d[y].i(),d[x1].i(),d[x2].i()))) throw EmptyBoxException();  }
	       void gen1_bwd   (int x, int y);
	inline void minus_bwd  (int x, int y)          { if ((d[x].i() &=-d[y].i()).is_empty()) throw EmptyBoxException();  }
	inline void minus_V_bwd(int x, int y)          { if ((d[x].v() &=-d[y].v()).is_empty()) throw EmptyBoxException();  }
	inline void minus_M_bwd(int x, int y)          { if ((d[x].m() &=-d[y].m()).is_empty()) throw EmptyBoxException();  }
    inline void trans_V_bwd(int x, int y)          { if ((d[x].v() &= d[y].v()).is_empty()) throw EmptyBoxException();  }
    inline void trans_M_bwd(int x, int y)          { if ((d[x].m() &= d[y].m().transpose()).is_empty()) throw EmptyBoxException();  }
	inline void sign_bwd   (int x, int y)          { if (!(bwd_sign(d[y].i(),d[x].i()))) throw EmptyBoxException();  }
	inline void abs_bwd    (int x, int y)          { if (!(bwd_abs(d[y].i(),d[x].i()))) throw EmptyBoxException();  }
	inline void power_bwd  (int x, int y, int p)   { if (!(bwd_pow(d[y].i(),p, d[x].i()))) throw EmptyBoxException();  }
	inline void sqr_bwd    (int x, int y)          { if (!(bwd_sqr(d[y].i(),d[x].i()))) throw EmptyBoxException();  }
	inline void sqrt_bwd   (int x, int y)          { if (!(bwd_sqrt(d[y].i(),d[x].i()))) throw EmptyBoxException();  }
	inline void exp_bwd    (int x, int y)          { if (!(bwd_exp(d[y].i(),d[x].i()))) throw EmptyBoxException();  }
	inline void log_bwd    (int x, int y)          { if (!(bwd_log(d[y].i(),d[x].i()))) throw EmptyBoxException();  }
	inline void cos_bwd    (int x, int y)          { if (!(bwd_cos(d[y].i(),d[x].i()))) throw EmptyBoxException();  }
	inline void sin_bwd    (int x, int y)          { if (!(bwd_sin(d[y].i(),d[x].i()))) throw EmptyBoxException();  }
	inline void tan_bwd    (int x, int y)          { if (!(bwd_tan(d[y].i(),d[x].i()))) throw EmptyBoxException();  }
	inline void cosh_bwd   (int x, int y)          { if (!(bwd_cosh(d[y].i(),d[x].i()))) throw EmptyBoxException();  }
	inline void sinh_bwd   (int x, int y)          { if (!(bwd_sinh(d[y].i(),d[x].i()))) throw EmptyBoxException();  }
	inline void tanh_bwd   (int x, int y)          { if (!(bwd_tanh(d[y].i(),d[x].i()))) throw EmptyBoxException();  }
	inline void acos_bwd   (int x, int y)          { if (!(bwd_acos(d[y].i(),d[x].i()))) throw EmptyBoxException();  }
	inline void asin_bwd   (int x, int y)          { if (!(bwd_asin(d[y].i(),d[x].i()))) throw EmptyBoxException();  }
	inline void atan_bwd   (int x, int y)          { if (!(bwd_atan(d[y].i(),d[x].i()))) throw EmptyBoxException();  }
	inline void acosh_bwd  (int x, int y)          { if (!(bwd_acosh(d[y].i(),d[x].i()))) throw EmptyBoxException();  }
	inline void asinh_bwd  (int x, int y)          { if (!(bwd_asinh(d[y].i(),d[x].i()))) throw EmptyBoxException();  }
	inline void atanh_bwd  (int x, int y)          { if (!(bwd_atanh(d[y].i(),d[x].i()))) throw EmptyBoxException();  }
};

} // namespace ibex

#endif // __IBEX_HC4_REVISE_H__
