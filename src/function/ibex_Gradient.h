/* ============================================================================
 * I B E X - Gradient of a function
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Jan 27, 2012
 * ---------------------------------------------------------------------------- */

#ifndef __IBEX_GRADIENT_H__
#define __IBEX_GRADIENT_H__

#include "ibex_Function.h"
#include "ibex_BwdAlgorithm.h"

namespace ibex {

/**
 * \ingroup symbolic
 * \brief Calculates the gradient of a function.
 */
class Gradient : public FwdAlgorithm, public BwdAlgorithm {

public:
	/**
	 * \brief Calculate the gradient of f on the domains \a d and store the result in \a g.
	 */
	void gradient(const Function&, const Array<Domain>& d, IntervalVector& g) const;

	/**
	 * \brief Calculate the gradient of f on the box \a box and store the result in \a g.
	 */
	void gradient(const Function& f, const IntervalVector& box, IntervalVector& g) const;

	/**
	 * \brief Calculate the Jacobian on the domains \a d and store the result in \a J.
	 */
	void jacobian(const Function& f, const Array<Domain>& d, IntervalMatrix& J) const;

	inline void index_fwd(const ExprIndex& , const ExprLabel& , ExprLabel& ) { /* nothing to do */ }
	       void vector_fwd(const ExprVector& v, const ExprLabel** s, ExprLabel& y);
	       void cst_fwd(const ExprConstant&, ExprLabel& y)                                  { y.g->clear(); }
	       void symbol_fwd(const ExprSymbol&, ExprLabel& y)                                 { y.g->clear(); }
	       void apply_fwd(const ExprApply&, ExprLabel**, ExprLabel& y)                      { y.g->clear(); }
	inline void chi_fwd(const ExprChi&, const ExprLabel&, const ExprLabel&, const ExprLabel&, ExprLabel& y)  { y.g->i()=0; }
	inline void add_fwd(const ExprAdd&, const ExprLabel&, const ExprLabel&, ExprLabel& y)     { y.g->i()=0; }
	inline void mul_fwd(const ExprMul&, const ExprLabel&, const ExprLabel&, ExprLabel& y)     { y.g->i()=0; }
	inline void sub_fwd(const ExprSub&, const ExprLabel&, const ExprLabel&, ExprLabel& y)     { y.g->i()=0; }
	inline void div_fwd(const ExprDiv&, const ExprLabel&, const ExprLabel&, ExprLabel& y)     { y.g->i()=0; }
	inline void max_fwd(const ExprMax&, const ExprLabel&, const ExprLabel&, ExprLabel& y)     { y.g->i()=0; }
	inline void min_fwd(const ExprMin&, const ExprLabel&, const ExprLabel&, ExprLabel& y)     { y.g->i()=0; }
	inline void atan2_fwd(const ExprAtan2&, const ExprLabel&, const ExprLabel&, ExprLabel& y) { y.g->i()=0; }
	inline void minus_fwd(const ExprMinus&, const ExprLabel&, ExprLabel& y)   { y.g->i()=0; }
	inline void trans_V_fwd(const ExprTrans&, const ExprLabel&, ExprLabel& y) { y.g->v().clear(); }
	inline void trans_M_fwd(const ExprTrans&, const ExprLabel&, ExprLabel& y) { y.g->m().clear(); }
	inline void sign_fwd(const ExprSign&, const ExprLabel&, ExprLabel& y)     { y.g->i()=0; }
	inline void abs_fwd(const ExprAbs&, const ExprLabel&, ExprLabel& y)       { y.g->i()=0; }
	inline void power_fwd(const ExprPower& , const ExprLabel&, ExprLabel& y)  { y.g->i()=0; }
	inline void sqr_fwd(const ExprSqr&, const ExprLabel&, ExprLabel& y)       { y.g->i()=0; }
	inline void sqrt_fwd(const ExprSqrt&, const ExprLabel&, ExprLabel& y)     { y.g->i()=0; }
	inline void exp_fwd(const ExprExp&, const ExprLabel&, ExprLabel& y)       { y.g->i()=0; }
	inline void log_fwd(const ExprLog&, const ExprLabel&, ExprLabel& y)       { y.g->i()=0; }
	inline void cos_fwd(const ExprCos&, const ExprLabel&, ExprLabel& y)       { y.g->i()=0; }
	inline void sin_fwd(const ExprSin&, const ExprLabel&, ExprLabel& y)       { y.g->i()=0; }
	inline void tan_fwd(const ExprTan&, const ExprLabel&, ExprLabel& y)       { y.g->i()=0; }
	inline void cosh_fwd(const ExprCosh&, const ExprLabel&, ExprLabel& y)     { y.g->i()=0; }
	inline void sinh_fwd(const ExprSinh&, const ExprLabel&, ExprLabel& y)     { y.g->i()=0; }
	inline void tanh_fwd(const ExprTanh&, const ExprLabel&, ExprLabel& y)     { y.g->i()=0; }
	inline void acos_fwd(const ExprAcos&, const ExprLabel&, ExprLabel& y)     { y.g->i()=0; }
	inline void asin_fwd(const ExprAsin&, const ExprLabel&, ExprLabel& y)     { y.g->i()=0; }
	inline void atan_fwd(const ExprAtan&, const ExprLabel&, ExprLabel& y)     { y.g->i()=0; }
	inline void acosh_fwd(const ExprAcosh&, const ExprLabel&, ExprLabel& y)   { y.g->i()=0; }
	inline void asinh_fwd(const ExprAsinh&, const ExprLabel&, ExprLabel& y)   { y.g->i()=0; }
	inline void atanh_fwd(const ExprAtanh&, const ExprLabel&, ExprLabel& y)   { y.g->i()=0; }

	inline void add_V_fwd(const ExprAdd&, const ExprLabel&, const ExprLabel&, ExprLabel& y)  { y.g->v().clear(); }
	inline void add_M_fwd(const ExprAdd&, const ExprLabel&, const ExprLabel&, ExprLabel& y)  { y.g->m().clear(); }
	inline void mul_SV_fwd(const ExprMul&, const ExprLabel&, const ExprLabel&, ExprLabel& y) { y.g->v().clear(); }
	inline void mul_SM_fwd(const ExprMul&, const ExprLabel&, const ExprLabel&, ExprLabel& y) { y.g->m().clear(); }
	inline void mul_VV_fwd(const ExprMul&, const ExprLabel&, const ExprLabel&, ExprLabel& y) { y.g->i()=0; }
	inline void mul_MV_fwd(const ExprMul&, const ExprLabel&, const ExprLabel&, ExprLabel& y) { y.g->v().clear(); }
	inline void mul_VM_fwd(const ExprMul&, const ExprLabel&, const ExprLabel&, ExprLabel& y) { y.g->v().clear(); }
	inline void mul_MM_fwd(const ExprMul&, const ExprLabel&, const ExprLabel&, ExprLabel& y) { y.g->m().clear(); }
	inline void sub_V_fwd(const ExprSub&, const ExprLabel&, const ExprLabel&, ExprLabel& y)  { y.g->v().clear(); }
	inline void sub_M_fwd(const ExprSub&, const ExprLabel&, const ExprLabel&, ExprLabel& y)  { y.g->m().clear(); }


	       void index_bwd (const ExprIndex&,   ExprLabel& ,                 const ExprLabel& ) { }
	       void vector_bwd(const ExprVector&,  ExprLabel** x,               const ExprLabel& y);
	inline void symbol_bwd(const ExprSymbol&,                               const ExprLabel& ) { /*cout << "symbol bwd=" << y.g->v() << endl;*/ /* nothing to do */ }
	inline void cst_bwd   (const ExprConstant&,                             const ExprLabel& ) { /* nothing to do */ }
	       void apply_bwd (const ExprApply&,  ExprLabel** x,                const ExprLabel& y);
	       void chi_bwd   (const ExprChi&,    ExprLabel& x1, ExprLabel& x2, ExprLabel& x3, const ExprLabel& y);
	inline void add_bwd   (const ExprAdd&,    ExprLabel& x1, ExprLabel& x2, const ExprLabel& y) { x1.g->i() += y.g->i();  x2.g->i() += y.g->i(); }
	inline void mul_bwd   (const ExprMul&,    ExprLabel& x1, ExprLabel& x2, const ExprLabel& y) { x1.g->i() += y.g->i() * x2.d->i(); x2.g->i() += y.g->i() * x1.d->i(); }
	inline void sub_bwd   (const ExprSub&,    ExprLabel& x1, ExprLabel& x2, const ExprLabel& y) { x1.g->i() += y.g->i();          x2.g->i() += -y.g->i(); }
	inline void div_bwd   (const ExprDiv&,    ExprLabel& x1, ExprLabel& x2, const ExprLabel& y) { x1.g->i() += y.g->i() / x2.d->i(); x2.g->i() += y.g->i()*(-x1.d->i())/sqr(x2.d->i()); }
	       void max_bwd   (const ExprMax&,    ExprLabel& ,   ExprLabel& ,   const ExprLabel& );
	       void min_bwd   (const ExprMin&,    ExprLabel& ,   ExprLabel& ,   const ExprLabel& );
	       void atan2_bwd (const ExprAtan2& , ExprLabel& ,   ExprLabel& ,   const ExprLabel& );
	inline void minus_bwd (const ExprMinus& , ExprLabel& x,                 const ExprLabel& y) { x.g->i() += -1.0*y.g->i(); }
        inline void trans_V_bwd(const ExprTrans&, ExprLabel& /*x*/,             const ExprLabel& /*y*/) { /* nothing to do because x.g->v() is a reference to y.g->v()
                                                                                                     see Decorator */ }
        inline void trans_M_bwd(const ExprTrans&, ExprLabel& x,                 const ExprLabel& y) { x.g->m() += y.g->m().transpose(); }
	       void sign_bwd  (const ExprSign&,   ExprLabel& x,                 const ExprLabel& y);
	       void abs_bwd   (const ExprAbs&,    ExprLabel& x,                 const ExprLabel& y);
	inline void power_bwd(const ExprPower& e, ExprLabel& x,                 const ExprLabel& y) { x.g->i() += y.g->i() * e.expon * pow(x.d->i(), e.expon-1); }
	inline void sqr_bwd   (const ExprSqr& ,   ExprLabel& x,                 const ExprLabel& y) { x.g->i() += y.g->i() * 2.0 * x.d->i(); }
	inline void sqrt_bwd  (const ExprSqrt& ,  ExprLabel& x,                 const ExprLabel& y) { x.g->i() += y.g->i() * 0.5 / sqrt(x.d->i());  }
	inline void exp_bwd   (const ExprExp& ,   ExprLabel& x,                 const ExprLabel& y) { x.g->i() += y.g->i() * exp(x.d->i()); }
	inline void log_bwd   (const ExprLog& ,   ExprLabel& x,                 const ExprLabel& y) { x.g->i() += y.g->i() / x.d->i(); }
	inline void cos_bwd   (const ExprCos& ,   ExprLabel& x,                 const ExprLabel& y) { x.g->i() += y.g->i() * -sin(x.d->i()); }
	inline void sin_bwd   (const ExprSin& ,   ExprLabel& x,                 const ExprLabel& y) { x.g->i() += y.g->i() * cos(x.d->i()); }
	inline void tan_bwd   (const ExprTan& ,   ExprLabel& x,                 const ExprLabel& y) { x.g->i() += y.g->i() * (1.0 + sqr(tan(x.d->i()))); }
	inline void cosh_bwd  (const ExprCosh& ,  ExprLabel& x,                 const ExprLabel& y) { x.g->i() += y.g->i() * sinh(x.d->i()); }
	inline void sinh_bwd  (const ExprSinh& ,  ExprLabel& x,                 const ExprLabel& y) { x.g->i() += y.g->i() * cosh(x.d->i()); }
	inline void tanh_bwd  (const ExprTanh& ,  ExprLabel& x,                 const ExprLabel& y) { x.g->i() += y.g->i() * (1.0 - sqr(tanh(x.d->i()))); }
	inline void acos_bwd  (const ExprAcos& ,  ExprLabel& x,                 const ExprLabel& y) { x.g->i() += y.g->i() * -1.0 / sqrt(1.0-sqr(x.d->i())); }
	inline void asin_bwd  (const ExprAsin& ,  ExprLabel& x,                 const ExprLabel& y) { x.g->i() += y.g->i() * 1.0 / sqrt(1.0-sqr(x.d->i())); }
	inline void atan_bwd  (const ExprAtan& ,  ExprLabel& x,                 const ExprLabel& y) { x.g->i() += y.g->i() * 1.0 / (1.0+sqr(x.d->i())); }
	inline void acosh_bwd (const ExprAcosh& , ExprLabel& x,                 const ExprLabel& y) { x.g->i() += y.g->i() * 1.0 / sqrt(sqr(x.d->i()) -1.0); }
	inline void asinh_bwd (const ExprAsinh& , ExprLabel& x,                 const ExprLabel& y) { x.g->i() += y.g->i() * 1.0 / sqrt(1.0+sqr(x.d->i())); }
	inline void atanh_bwd (const ExprAtanh& , ExprLabel& x,                 const ExprLabel& y) { x.g->i() += y.g->i() * 1.0 / (1.0-sqr(x.d->i())); }

	inline void add_V_bwd (const ExprAdd&, ExprLabel& x1, ExprLabel& x2,    const ExprLabel& y) { x1.g->v() += y.g->v();        x2.g->v() += y.g->v(); }
	inline void add_M_bwd (const ExprAdd&, ExprLabel& x1, ExprLabel& x2,    const ExprLabel& y) { x1.g->m() += y.g->m();        x2.g->m() += y.g->m(); }
	inline void mul_SV_bwd(const ExprMul&, ExprLabel& x1, ExprLabel& x2,    const ExprLabel& y) { x1.g->i() += y.g->v()*x2.d->v(); x2.g->v() += x1.d->i()*y.g->v(); }
	inline void mul_SM_bwd(const ExprMul&, ExprLabel& x1, ExprLabel& x2,    const ExprLabel& y) { for (int i=0; i<y.d->m().nb_rows(); i++) x1.g->i()+=y.g->m()[i]*x2.d->m()[i]; x2.g->m() += x1.d->i()*y.g->m();  }
	inline void mul_VV_bwd(const ExprMul&, ExprLabel& x1, ExprLabel& x2,    const ExprLabel& y) { x1.g->v() += y.g->i()*x2.d->v(); x2.g->v() += y.g->i()*x1.d->v(); }
	inline void mul_MV_bwd(const ExprMul&, ExprLabel& x1, ExprLabel& x2,    const ExprLabel& y) { x1.g->m() += outer_product(y.g->v(),x2.d->v()); x2.g->v() += x1.d->m().transpose()*y.g->v(); }
	inline void mul_MM_bwd(const ExprMul&, ExprLabel& x1, ExprLabel& x2,    const ExprLabel& y) { x1.g->m() += y.g->m()*x2.d->m().transpose(); x2.g->m() += x1.d->m().transpose()*y.g->m(); }
	inline void mul_VM_bwd(const ExprMul&, ExprLabel& x1, ExprLabel& x2,    const ExprLabel& y) { x1.g->v() += x2.d->m()*y.g->v(); x2.g->m() += outer_product(x1.d->v(),y.g->v()); }
	inline void sub_V_bwd (const ExprSub&, ExprLabel& x1, ExprLabel& x2,    const ExprLabel& y) { x1.g->v() += y.g->v(); x2.g->v() -= y.g->v(); }
	inline void sub_M_bwd (const ExprSub&, ExprLabel& x1, ExprLabel& x2,    const ExprLabel& y) { x1.g->m() += y.g->m(); x2.g->m() -= y.g->m(); }
};

} // namespace ibex
#endif // __IBEX_GRADIENT_H__
