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
#include "ibex_CompiledFunction.h"
#include "ibex_EmptyBoxException.h"

namespace ibex {

/**
 * \ingroup level1
 * \brief The famous forward-backward contraction algorithm.
 *
 */
class HC4Revise : public BwdAlgorithm<Domain> {
public:

	/**
	 * \brief Create the HC4-revise algorithm for a function \a f.
	 */
	HC4Revise(const Function& f);

	/**
	 * \brief Run the backward algorithm.
	 *
	 * Set the domain of the root node to \a root
	 * and run the backward algorithm. The new domains
	 * of the variables can be read from eval.symbolLabels.
	 */
	void backward(const Domain& root);

	/**
	 * \brief Contract the input domains with the backward algorithm.
	 *
	 * The domain of the root is set to \a root.
	 */
	void contract(Domains& d, const Domain& root);

	/**
	 * \brief Contract the input box with the backward algorithm.
	 *
	 * The domain of the root is set to \a root.
	 */
	void contract(IntervalVector& box, const Domain& root);

	/**
	 * \brief The evaluator.
	 *
	 * This field contains in particular a reference to the function "f"
	 * of the constraint.
	 */
	Eval eval;

	/**
	 * \brief Ratio for the contraction of a
	 * matrix-vector / matrix-matrix multiplication.
	 */
	static const double RATIO = 0.1;

protected:
	IntervalVector* current_box; // **HACK**

	friend class CompiledFunction;

	inline void index_bwd (const ExprIndex&,   Domain& exprL, const Domain& result)                    { /* nothing to do */ }
	       void vector_bwd(const ExprVector&,  Domain** compL, const Domain& result);
	inline void symbol_bwd(const ExprSymbol& s, const Domain& result)                                    {
		// **HACK **
		if (current_box) (*current_box)[s.key]=result.i();
		/* else: nothing to do */ }
	inline void cst_bwd   (const ExprConstant&, const Domain& result)                                  { /* nothing to do */ }
	       void apply_bwd (const ExprApply&,   Domain** argL, const Domain& result);
	inline void add_bwd   (const ExprAdd&,     Domain& leftL, Domain& rightL, const Domain& result)    { if (!(proj_add(result.i(),leftL.i(),rightL.i()))) throw EmptyBoxException();  }
	inline void add_V_bwd  (const ExprAdd&,    Domain& leftL, Domain& rightL, const Domain& result)    { if (!(proj_add(result.v(),leftL.v(),rightL.v()))) throw EmptyBoxException();  }
	inline void add_M_bwd  (const ExprAdd&,    Domain& leftL, Domain& rightL, const Domain& result)    { if (!(proj_add(result.m(),leftL.m(),rightL.m()))) throw EmptyBoxException();  }
	inline void mul_bwd    (const ExprMul&,    Domain& leftL, Domain& rightL, const Domain& result)    { if (!(proj_mul(result.i(),leftL.i(),rightL.i()))) throw EmptyBoxException();  }
	inline void mul_SV_bwd (const ExprMul&,    Domain& leftL, Domain& rightL, const Domain& result)    { if (!(proj_mul(result.v(),leftL.i(),rightL.v()))) throw EmptyBoxException();  }
	inline void mul_SM_bwd (const ExprMul&,    Domain& leftL, Domain& rightL, const Domain& result)    { if (!(proj_mul(result.m(),leftL.i(),rightL.m()))) throw EmptyBoxException();  }
	inline void mul_VV_bwd (const ExprMul&,    Domain& leftL, Domain& rightL, const Domain& result)    { if (!(proj_mul(result.i(),leftL.v(),rightL.v()))) throw EmptyBoxException();  }
	inline void mul_MV_bwd (const ExprMul&,    Domain& leftL, Domain& rightL, const Domain& result)    { if (!(proj_mul(result.v(),leftL.m(),rightL.v(), RATIO))) throw EmptyBoxException();  }
	inline void mul_MM_bwd (const ExprMul&,    Domain& leftL, Domain& rightL, const Domain& result)    { if (!(proj_mul(result.m(),leftL.m(),rightL.m(), RATIO))) throw EmptyBoxException();  }
	inline void sub_bwd   (const ExprSub&,     Domain& leftL, Domain& rightL, const Domain& result)    { if (!(proj_sub(result.i(),leftL.i(),rightL.i()))) throw EmptyBoxException();  }
	inline void sub_V_bwd (const ExprSub&,     Domain& leftL, Domain& rightL, const Domain& result)    { if (!(proj_sub(result.v(),leftL.v(),rightL.v()))) throw EmptyBoxException();  }
	inline void sub_M_bwd (const ExprSub&,     Domain& leftL, Domain& rightL, const Domain& result)    { if (!(proj_sub(result.m(),leftL.m(),rightL.m()))) throw EmptyBoxException();  }
	inline void div_bwd   (const ExprDiv&,     Domain& leftL, Domain& rightL, const Domain& result)    { if (!(proj_div(result.i(),leftL.i(),rightL.i()))) throw EmptyBoxException();  }
	inline void max_bwd   (const ExprMax&,     Domain& leftL, Domain& rightL, const Domain& result)    { if (!(proj_max(result.i(),leftL.i(),rightL.i()))) throw EmptyBoxException();  }
	inline void min_bwd   (const ExprMin&,     Domain& leftL, Domain& rightL, const Domain& result)    { if (!(proj_min(result.i(),leftL.i(),rightL.i()))) throw EmptyBoxException();  }
	inline void atan2_bwd (const ExprAtan2& e, Domain& leftL, Domain& rightL, const Domain& result)    { if (!(proj_atan2(result.i(),leftL.i(),rightL.i()))) throw EmptyBoxException();  }
	inline void minus_bwd (const ExprMinus& e, Domain& exprL, const Domain& result)                    { if ((exprL.i() &=-result.i()).is_empty()) throw EmptyBoxException();  }
	inline void sign_bwd  (const ExprSign& e,  Domain& exprL, const Domain& result)                    { if (!(proj_sign(result.i(),exprL.i()))) throw EmptyBoxException();  }
	inline void abs_bwd   (const ExprAbs& e,   Domain& exprL, const Domain& result)                    { if (!(proj_abs(result.i(),exprL.i()))) throw EmptyBoxException();  }
	inline void power_bwd (const ExprPower& e, Domain& exprL, const Domain& result)                    { if (!(proj_pow(result.i(),e.expon, exprL.i()))) throw EmptyBoxException();  }
	inline void sqr_bwd   (const ExprSqr& e,   Domain& exprL, const Domain& result)                    { if (!(proj_sqr(result.i(),exprL.i()))) throw EmptyBoxException();  }
	inline void sqrt_bwd  (const ExprSqrt& e,  Domain& exprL, const Domain& result)                    { if (!(proj_sqrt(result.i(),exprL.i()))) throw EmptyBoxException();  }
	inline void exp_bwd   (const ExprExp& e,   Domain& exprL, const Domain& result)                    { if (!(proj_exp(result.i(),exprL.i()))) throw EmptyBoxException();  }
	inline void log_bwd   (const ExprLog& e,   Domain& exprL, const Domain& result)                    { if (!(proj_log(result.i(),exprL.i()))) throw EmptyBoxException();  }
	inline void cos_bwd   (const ExprCos& e,   Domain& exprL, const Domain& result)                    { if (!(proj_cos(result.i(),exprL.i()))) throw EmptyBoxException();  }
	inline void sin_bwd   (const ExprSin& e,   Domain& exprL, const Domain& result)                    { if (!(proj_sin(result.i(),exprL.i()))) throw EmptyBoxException();  }
	inline void tan_bwd   (const ExprTan& e,   Domain& exprL, const Domain& result)                    { if (!(proj_tan(result.i(),exprL.i()))) throw EmptyBoxException();  }
	inline void cosh_bwd  (const ExprCosh& e,  Domain& exprL, const Domain& result)                    { if (!(proj_cosh(result.i(),exprL.i()))) throw EmptyBoxException();  }
	inline void sinh_bwd  (const ExprSinh& e,  Domain& exprL, const Domain& result)                    { if (!(proj_sinh(result.i(),exprL.i()))) throw EmptyBoxException();  }
	inline void tanh_bwd  (const ExprTanh& e,  Domain& exprL, const Domain& result)                    { if (!(proj_tanh(result.i(),exprL.i()))) throw EmptyBoxException();  }
	inline void acos_bwd  (const ExprAcos& e,  Domain& exprL, const Domain& result)                    { if (!(proj_acos(result.i(),exprL.i()))) throw EmptyBoxException();  }
	inline void asin_bwd  (const ExprAsin& e,  Domain& exprL, const Domain& result)                    { if (!(proj_asin(result.i(),exprL.i()))) throw EmptyBoxException();  }
	inline void atan_bwd  (const ExprAtan& e,  Domain& exprL, const Domain& result)                    { if (!(proj_atan(result.i(),exprL.i()))) throw EmptyBoxException();  }
	inline void acosh_bwd (const ExprAcosh& e, Domain& exprL, const Domain& result)                    { if (!(proj_acosh(result.i(),exprL.i()))) throw EmptyBoxException();  }
	inline void asinh_bwd (const ExprAsinh& e, Domain& exprL, const Domain& result)                    { if (!(proj_asinh(result.i(),exprL.i()))) throw EmptyBoxException();  }
	inline void atanh_bwd (const ExprAtanh& e, Domain& exprL, const Domain& result)                    { if (!(proj_atanh(result.i(),exprL.i()))) throw EmptyBoxException();  }

private:
	void write(IntervalVector&) const;

};


/* ============================================================================
 	 	 	 	 	 	 	 implementation
  ============================================================================*/

inline void HC4Revise::backward(const Domain& root) {
	(Domain&) *eval.f.expr().deco = root;
	return eval.f.backward<HC4Revise,Domain>(*this);
}

inline void HC4Revise::contract(Domains& d, const Domain& root) {
	eval.eval(d);
	backward(root);
	d = eval.symbolLabels;
}

inline void HC4Revise::contract(IntervalVector& box, const Domain& root) {
	eval.eval(box);

	if (eval.f.all_symbols_scalar()) // **HACK**
		current_box = &box;

	try {
		backward(root);
	} catch (EmptyBoxException&) {
		box.set_empty();
	}
	if (current_box) // **HACK**
		current_box = NULL;
	else
		box = eval.symbolLabels;
}

} /* namespace ibex */
#endif /* __IBEX_HC4_REVISE_H__ */
