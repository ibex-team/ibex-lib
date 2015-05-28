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

#include "ibex_Function.h"

namespace ibex {

typedef enum { INTERVAL_MODE, AFFINE2_MODE, AFFINE_MODE } FwdMode;

/**
 * \ingroup symbolic
 * \brief The famous forward-backward contraction algorithm.
 *
 */
class HC4Revise : public BwdAlgorithm {
public:

	/*
	 * \brief HC4Revise
	 *
	 * \param mode  the arithmetic for forward evaluation. By default: interval arithmetic.
	 * Accepted values are: INTERVAL_MODE or AFFINE2_MODE.
	 *
	 * \warning Throws an #EmptyBoxException if the contraction gives an empty set.
	 */
	HC4Revise(FwdMode mode=INTERVAL_MODE);

	//void bwd(const Function& f, const Domain& y, Array<Domain>& x);

	/**
	 * \brief Project f(x)=y onto x (backward algorithm)
	 *
	 * \brief true if f(x) is included in y (inactive constraint)
	 *
	 * \note if x is outside the definition domain of f, then
	 *       x is set to the empty set although f([x])\subseteq [y]
	 *       and the return value is "false".
	 */
	bool proj(const Function& f, const Domain& y, IntervalVector& x);

	/**
	 * \brief Ratio for the contraction of a
	 * matrix-vector / matrix-matrix multiplication.
	 * Set to 0.1.
	 */
	static const double RATIO;

protected:
	/**
	 * Class used internally to interrupt the
	 * backward procedure when an empty domain occurs.
	 */
	class EmptyBoxException { };

public: // because called from CompiledFunction
	inline void index_bwd (const ExprIndex&,   ExprLabel& , const ExprLabel& )            { /* nothing to do */ }
	       void vector_bwd(const ExprVector&,  ExprLabel** compL, const ExprLabel& result);
	inline void symbol_bwd(const ExprSymbol& , const ExprLabel& )                             { /* nothing to do */ }
	inline void cst_bwd   (const ExprConstant&, const ExprLabel& )                                  { /* nothing to do */ }
	inline void apply_bwd (const ExprApply& a, ExprLabel** x, const ExprLabel& y)                   { proj(a.func,*y.d,x); }
	inline void chi_bwd   (const ExprChi&,ExprLabel& a,ExprLabel& b,ExprLabel& c,const ExprLabel& f){ if (!(bwd_chi(f.d->i(),a.d->i(),b.d->i(),c.d->i()))) throw EmptyBoxException();  }
	inline void add_bwd   (const ExprAdd&,     ExprLabel& x1, ExprLabel& x2, const ExprLabel& y)    { if (!(bwd_add(y.d->i(),x1.d->i(),x2.d->i()))) throw EmptyBoxException();  }
	inline void add_V_bwd  (const ExprAdd&,    ExprLabel& x1, ExprLabel& x2, const ExprLabel& y)    { if (!(bwd_add(y.d->v(),x1.d->v(),x2.d->v()))) throw EmptyBoxException();  }
	inline void add_M_bwd  (const ExprAdd&,    ExprLabel& x1, ExprLabel& x2, const ExprLabel& y)    { if (!(bwd_add(y.d->m(),x1.d->m(),x2.d->m()))) throw EmptyBoxException();  }
	inline void mul_bwd    (const ExprMul&,    ExprLabel& x1, ExprLabel& x2, const ExprLabel& y)    { if (!(bwd_mul(y.d->i(),x1.d->i(),x2.d->i()))) throw EmptyBoxException();  }
	inline void mul_SV_bwd (const ExprMul&,    ExprLabel& x1, ExprLabel& x2, const ExprLabel& y)    { if (!(bwd_mul(y.d->v(),x1.d->i(),x2.d->v()))) throw EmptyBoxException();  }
	inline void mul_SM_bwd (const ExprMul&,    ExprLabel& x1, ExprLabel& x2, const ExprLabel& y)    { if (!(bwd_mul(y.d->m(),x1.d->i(),x2.d->m()))) throw EmptyBoxException();  }
	inline void mul_VV_bwd (const ExprMul&,    ExprLabel& x1, ExprLabel& x2, const ExprLabel& y)    { if (!(bwd_mul(y.d->i(),x1.d->v(),x2.d->v()))) throw EmptyBoxException();  }
	inline void mul_MV_bwd (const ExprMul&,    ExprLabel& x1, ExprLabel& x2, const ExprLabel& y)    { if (!(bwd_mul(y.d->v(),x1.d->m(),x2.d->v(), RATIO))) throw EmptyBoxException();  }
	inline void mul_VM_bwd (const ExprMul&,    ExprLabel& x1, ExprLabel& x2, const ExprLabel& y)    { if (!(bwd_mul(y.d->v(),x1.d->v(),x2.d->m(), RATIO))) throw EmptyBoxException();  }
	inline void mul_MM_bwd (const ExprMul&,    ExprLabel& x1, ExprLabel& x2, const ExprLabel& y)    { if (!(bwd_mul(y.d->m(),x1.d->m(),x2.d->m(), RATIO))) throw EmptyBoxException();  }
	inline void sub_bwd   (const ExprSub&,     ExprLabel& x1, ExprLabel& x2, const ExprLabel& y)    { if (!(bwd_sub(y.d->i(),x1.d->i(),x2.d->i()))) throw EmptyBoxException();  }
	inline void sub_V_bwd (const ExprSub&,     ExprLabel& x1, ExprLabel& x2, const ExprLabel& y)    { if (!(bwd_sub(y.d->v(),x1.d->v(),x2.d->v()))) throw EmptyBoxException();  }
	inline void sub_M_bwd (const ExprSub&,     ExprLabel& x1, ExprLabel& x2, const ExprLabel& y)    { if (!(bwd_sub(y.d->m(),x1.d->m(),x2.d->m()))) throw EmptyBoxException();  }
	inline void div_bwd   (const ExprDiv&,     ExprLabel& x1, ExprLabel& x2, const ExprLabel& y)    { if (!(bwd_div(y.d->i(),x1.d->i(),x2.d->i()))) throw EmptyBoxException();  }
	inline void max_bwd   (const ExprMax&,     ExprLabel& x1, ExprLabel& x2, const ExprLabel& y)    { if (!(bwd_max(y.d->i(),x1.d->i(),x2.d->i()))) throw EmptyBoxException();  }
	inline void min_bwd   (const ExprMin&,     ExprLabel& x1, ExprLabel& x2, const ExprLabel& y)    { if (!(bwd_min(y.d->i(),x1.d->i(),x2.d->i()))) throw EmptyBoxException();  }
	inline void atan2_bwd (const ExprAtan2& , ExprLabel& x1, ExprLabel& x2, const ExprLabel& y)    { if (!(bwd_atan2(y.d->i(),x1.d->i(),x2.d->i()))) throw EmptyBoxException();  }
	inline void minus_bwd (const ExprMinus& , ExprLabel& x, const ExprLabel& y)                    { if ((x.d->i() &=-y.d->i()).is_empty()) throw EmptyBoxException();  }
    inline void trans_V_bwd(const ExprTrans& ,ExprLabel& x, const ExprLabel& y)                    { if ((x.d->v() &= y.d->v()).is_empty()) throw EmptyBoxException();  }
    inline void trans_M_bwd(const ExprTrans& ,ExprLabel& x, const ExprLabel& y)                    { if ((x.d->m() &= y.d->m().transpose()).is_empty()) throw EmptyBoxException();  }
	inline void sign_bwd  (const ExprSign& ,  ExprLabel& x, const ExprLabel& y)                    { if (!(bwd_sign(y.d->i(),x.d->i()))) throw EmptyBoxException();  }
	inline void abs_bwd   (const ExprAbs& ,   ExprLabel& x, const ExprLabel& y)                    { if (!(bwd_abs(y.d->i(),x.d->i()))) throw EmptyBoxException();  }
	inline void power_bwd (const ExprPower& e, ExprLabel& x, const ExprLabel& y)                    { if (!(bwd_pow(y.d->i(),e.expon, x.d->i()))) throw EmptyBoxException();  }
	inline void sqr_bwd   (const ExprSqr& ,   ExprLabel& x, const ExprLabel& y)                    { if (!(bwd_sqr(y.d->i(),x.d->i()))) throw EmptyBoxException();  }
	inline void sqrt_bwd  (const ExprSqrt& ,  ExprLabel& x, const ExprLabel& y)                    { if (!(bwd_sqrt(y.d->i(),x.d->i()))) throw EmptyBoxException();  }
	inline void exp_bwd   (const ExprExp& ,   ExprLabel& x, const ExprLabel& y)                    { if (!(bwd_exp(y.d->i(),x.d->i()))) throw EmptyBoxException();  }
	inline void log_bwd   (const ExprLog& ,   ExprLabel& x, const ExprLabel& y)                    { if (!(bwd_log(y.d->i(),x.d->i()))) throw EmptyBoxException();  }
	inline void cos_bwd   (const ExprCos& ,   ExprLabel& x, const ExprLabel& y)                    { if (!(bwd_cos(y.d->i(),x.d->i()))) throw EmptyBoxException();  }
	inline void sin_bwd   (const ExprSin& ,   ExprLabel& x, const ExprLabel& y)                    { if (!(bwd_sin(y.d->i(),x.d->i()))) throw EmptyBoxException();  }
	inline void tan_bwd   (const ExprTan& ,   ExprLabel& x, const ExprLabel& y)                    { if (!(bwd_tan(y.d->i(),x.d->i()))) throw EmptyBoxException();  }
	inline void cosh_bwd  (const ExprCosh& ,  ExprLabel& x, const ExprLabel& y)                    { if (!(bwd_cosh(y.d->i(),x.d->i()))) throw EmptyBoxException();  }
	inline void sinh_bwd  (const ExprSinh& ,  ExprLabel& x, const ExprLabel& y)                    { if (!(bwd_sinh(y.d->i(),x.d->i()))) throw EmptyBoxException();  }
	inline void tanh_bwd  (const ExprTanh& ,  ExprLabel& x, const ExprLabel& y)                    { if (!(bwd_tanh(y.d->i(),x.d->i()))) throw EmptyBoxException();  }
	inline void acos_bwd  (const ExprAcos& ,  ExprLabel& x, const ExprLabel& y)                    { if (!(bwd_acos(y.d->i(),x.d->i()))) throw EmptyBoxException();  }
	inline void asin_bwd  (const ExprAsin& ,  ExprLabel& x, const ExprLabel& y)                    { if (!(bwd_asin(y.d->i(),x.d->i()))) throw EmptyBoxException();  }
	inline void atan_bwd  (const ExprAtan& ,  ExprLabel& x, const ExprLabel& y)                    { if (!(bwd_atan(y.d->i(),x.d->i()))) throw EmptyBoxException();  }
	inline void acosh_bwd (const ExprAcosh& , ExprLabel& x, const ExprLabel& y)                    { if (!(bwd_acosh(y.d->i(),x.d->i()))) throw EmptyBoxException();  }
	inline void asinh_bwd (const ExprAsinh& , ExprLabel& x, const ExprLabel& y)                    { if (!(bwd_asinh(y.d->i(),x.d->i()))) throw EmptyBoxException();  }
	inline void atanh_bwd (const ExprAtanh& , ExprLabel& x, const ExprLabel& y)                    { if (!(bwd_atanh(y.d->i(),x.d->i()))) throw EmptyBoxException();  }

protected:
	void proj(const Function& f, const Domain& y, ExprLabel** x);
	FwdMode fwd_mode;
};

} /* namespace ibex */
#endif /* __IBEX_HC4_REVISE_H__ */
