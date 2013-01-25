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
#include "ibex_EmptyBoxException.h"
#include "ibex_Function.h"

namespace ibex {

/**
 * \ingroup symbolic
 * \brief The famous forward-backward contraction algorithm.
 *
 */
class HC4Revise : public BwdAlgorithm {
public:
	/**
	 * \brief Project f(x)=y onto x (backward algorithm)
	 */
	void proj(const Function& f, const Domain& y, Array<Domain>& x);

	/**
	 * \brief Project f(x)=y onto x (backward algorithm)
	 */
	void proj(const Function& f, const Domain& y, IntervalVector& x);

	/**
	 * \brief Ratio for the contraction of a
	 * matrix-vector / matrix-matrix multiplication.
	 * Set to 0.1.
	 */
	static const double RATIO;

	inline void index_bwd (const ExprIndex&,   ExprLabel& exprL, const ExprLabel& result)            { /* nothing to do */ }
	       void vector_bwd(const ExprVector&,  ExprLabel** compL, const ExprLabel& result);
	inline void symbol_bwd(const ExprSymbol& , const ExprLabel& result)                             { /* nothing to do */ }
	inline void cst_bwd   (const ExprConstant&, const ExprLabel& y)                                  { /* nothing to do */ }
	inline void apply_bwd (const ExprApply& a, ExprLabel** x, const ExprLabel& y)                   { proj(a.func,*y.d,x); }
	inline void add_bwd   (const ExprAdd&,     ExprLabel& x1, ExprLabel& x2, const ExprLabel& y)    { if (!(proj_add(y.d->i(),x1.d->i(),x2.d->i()))) throw EmptyBoxException();  }
	inline void add_V_bwd  (const ExprAdd&,    ExprLabel& x1, ExprLabel& x2, const ExprLabel& y)    { if (!(proj_add(y.d->v(),x1.d->v(),x2.d->v()))) throw EmptyBoxException();  }
	inline void add_M_bwd  (const ExprAdd&,    ExprLabel& x1, ExprLabel& x2, const ExprLabel& y)    { if (!(proj_add(y.d->m(),x1.d->m(),x2.d->m()))) throw EmptyBoxException();  }
	inline void mul_bwd    (const ExprMul&,    ExprLabel& x1, ExprLabel& x2, const ExprLabel& y)    { if (!(proj_mul(y.d->i(),x1.d->i(),x2.d->i()))) throw EmptyBoxException();  }
	inline void mul_SV_bwd (const ExprMul&,    ExprLabel& x1, ExprLabel& x2, const ExprLabel& y)    { if (!(proj_mul(y.d->v(),x1.d->i(),x2.d->v()))) throw EmptyBoxException();  }
	inline void mul_SM_bwd (const ExprMul&,    ExprLabel& x1, ExprLabel& x2, const ExprLabel& y)    { if (!(proj_mul(y.d->m(),x1.d->i(),x2.d->m()))) throw EmptyBoxException();  }
	inline void mul_VV_bwd (const ExprMul&,    ExprLabel& x1, ExprLabel& x2, const ExprLabel& y)    { if (!(proj_mul(y.d->i(),x1.d->v(),x2.d->v()))) throw EmptyBoxException();  }
	inline void mul_MV_bwd (const ExprMul&,    ExprLabel& x1, ExprLabel& x2, const ExprLabel& y)    { if (!(proj_mul(y.d->v(),x1.d->m(),x2.d->v(), RATIO))) throw EmptyBoxException();  }
	inline void mul_VM_bwd (const ExprMul&,    ExprLabel& x1, ExprLabel& x2, const ExprLabel& y)    { if (!(proj_mul(y.d->v(),x1.d->v(),x2.d->m(), RATIO))) throw EmptyBoxException();  }
	inline void mul_MM_bwd (const ExprMul&,    ExprLabel& x1, ExprLabel& x2, const ExprLabel& y)    { if (!(proj_mul(y.d->m(),x1.d->m(),x2.d->m(), RATIO))) throw EmptyBoxException();  }
	inline void sub_bwd   (const ExprSub&,     ExprLabel& x1, ExprLabel& x2, const ExprLabel& y)    { if (!(proj_sub(y.d->i(),x1.d->i(),x2.d->i()))) throw EmptyBoxException();  }
	inline void sub_V_bwd (const ExprSub&,     ExprLabel& x1, ExprLabel& x2, const ExprLabel& y)    { if (!(proj_sub(y.d->v(),x1.d->v(),x2.d->v()))) throw EmptyBoxException();  }
	inline void sub_M_bwd (const ExprSub&,     ExprLabel& x1, ExprLabel& x2, const ExprLabel& y)    { if (!(proj_sub(y.d->m(),x1.d->m(),x2.d->m()))) throw EmptyBoxException();  }
	inline void div_bwd   (const ExprDiv&,     ExprLabel& x1, ExprLabel& x2, const ExprLabel& y)    { if (!(proj_div(y.d->i(),x1.d->i(),x2.d->i()))) throw EmptyBoxException();  }
	inline void max_bwd   (const ExprMax&,     ExprLabel& x1, ExprLabel& x2, const ExprLabel& y)    { if (!(proj_max(y.d->i(),x1.d->i(),x2.d->i()))) throw EmptyBoxException();  }
	inline void min_bwd   (const ExprMin&,     ExprLabel& x1, ExprLabel& x2, const ExprLabel& y)    { if (!(proj_min(y.d->i(),x1.d->i(),x2.d->i()))) throw EmptyBoxException();  }
	inline void atan2_bwd (const ExprAtan2& e, ExprLabel& x1, ExprLabel& x2, const ExprLabel& y)    { if (!(proj_atan2(y.d->i(),x1.d->i(),x2.d->i()))) throw EmptyBoxException();  }
	inline void minus_bwd (const ExprMinus& e, ExprLabel& x, const ExprLabel& y)                    { if ((x.d->i() &=-y.d->i()).is_empty()) throw EmptyBoxException();  }
    inline void trans_V_bwd(const ExprTrans& e,ExprLabel& x, const ExprLabel& y)                    { if ((x.d->v() &= y.d->v()).is_empty()) throw EmptyBoxException();  }
    inline void trans_M_bwd(const ExprTrans& e,ExprLabel& x, const ExprLabel& y)                    { if ((x.d->m() &= y.d->m().transpose()).is_empty()) throw EmptyBoxException();  }
	inline void sign_bwd  (const ExprSign& e,  ExprLabel& x, const ExprLabel& y)                    { if (!(proj_sign(y.d->i(),x.d->i()))) throw EmptyBoxException();  }
	inline void abs_bwd   (const ExprAbs& e,   ExprLabel& x, const ExprLabel& y)                    { if (!(proj_abs(y.d->i(),x.d->i()))) throw EmptyBoxException();  }
	inline void power_bwd (const ExprPower& e, ExprLabel& x, const ExprLabel& y)                    { if (!(proj_pow(y.d->i(),e.expon, x.d->i()))) throw EmptyBoxException();  }
	inline void sqr_bwd   (const ExprSqr& e,   ExprLabel& x, const ExprLabel& y)                    { if (!(proj_sqr(y.d->i(),x.d->i()))) throw EmptyBoxException();  }
	inline void sqrt_bwd  (const ExprSqrt& e,  ExprLabel& x, const ExprLabel& y)                    { if (!(proj_sqrt(y.d->i(),x.d->i()))) throw EmptyBoxException();  }
	inline void exp_bwd   (const ExprExp& e,   ExprLabel& x, const ExprLabel& y)                    { if (!(proj_exp(y.d->i(),x.d->i()))) throw EmptyBoxException();  }
	inline void log_bwd   (const ExprLog& e,   ExprLabel& x, const ExprLabel& y)                    { if (!(proj_log(y.d->i(),x.d->i()))) throw EmptyBoxException();  }
	inline void cos_bwd   (const ExprCos& e,   ExprLabel& x, const ExprLabel& y)                    { if (!(proj_cos(y.d->i(),x.d->i()))) throw EmptyBoxException();  }
	inline void sin_bwd   (const ExprSin& e,   ExprLabel& x, const ExprLabel& y)                    { if (!(proj_sin(y.d->i(),x.d->i()))) throw EmptyBoxException();  }
	inline void tan_bwd   (const ExprTan& e,   ExprLabel& x, const ExprLabel& y)                    { if (!(proj_tan(y.d->i(),x.d->i()))) throw EmptyBoxException();  }
	inline void cosh_bwd  (const ExprCosh& e,  ExprLabel& x, const ExprLabel& y)                    { if (!(proj_cosh(y.d->i(),x.d->i()))) throw EmptyBoxException();  }
	inline void sinh_bwd  (const ExprSinh& e,  ExprLabel& x, const ExprLabel& y)                    { if (!(proj_sinh(y.d->i(),x.d->i()))) throw EmptyBoxException();  }
	inline void tanh_bwd  (const ExprTanh& e,  ExprLabel& x, const ExprLabel& y)                    { if (!(proj_tanh(y.d->i(),x.d->i()))) throw EmptyBoxException();  }
	inline void acos_bwd  (const ExprAcos& e,  ExprLabel& x, const ExprLabel& y)                    { if (!(proj_acos(y.d->i(),x.d->i()))) throw EmptyBoxException();  }
	inline void asin_bwd  (const ExprAsin& e,  ExprLabel& x, const ExprLabel& y)                    { if (!(proj_asin(y.d->i(),x.d->i()))) throw EmptyBoxException();  }
	inline void atan_bwd  (const ExprAtan& e,  ExprLabel& x, const ExprLabel& y)                    { if (!(proj_atan(y.d->i(),x.d->i()))) throw EmptyBoxException();  }
	inline void acosh_bwd (const ExprAcosh& e, ExprLabel& x, const ExprLabel& y)                    { if (!(proj_acosh(y.d->i(),x.d->i()))) throw EmptyBoxException();  }
	inline void asinh_bwd (const ExprAsinh& e, ExprLabel& x, const ExprLabel& y)                    { if (!(proj_asinh(y.d->i(),x.d->i()))) throw EmptyBoxException();  }
	inline void atanh_bwd (const ExprAtanh& e, ExprLabel& x, const ExprLabel& y)                    { if (!(proj_atanh(y.d->i(),x.d->i()))) throw EmptyBoxException();  }

protected:
	void proj(const Function& f, const Domain& y, ExprLabel** x);
};

} /* namespace ibex */
#endif /* __IBEX_HC4_REVISE_H__ */
