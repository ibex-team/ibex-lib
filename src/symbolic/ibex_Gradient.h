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

#include "ibex_Eval.h"
#include "ibex_BwdAlgorithm.h"
#include "ibex_Decorator.h"
#include "ibex_CompiledFunction.h"
#include "ibex_BasicDecorator.h"

namespace ibex {

/**
 * \ingroup symbolic
 * \brief Label for the gradient computation.
 */
class GradLabel : public virtual Domain {
public:
	GradLabel(const Dim& dim);

	Domain g;
};

/**
 * \ingroup symbolic
 * \brief Label for the gradient computation of Apply nodes.
 */
class ApplyGradLabel : public GradLabel, BasicApplyLabel {
public:
	ApplyGradLabel(const Dim& dim, Function& f);

	Gradient fgrad;
};

/**
 * \ingroup symbolic
 * \brief Decorates a function with the label for gradient computation.
 */

class GradDecorator : public Decorator, public FunctionVisitor {
public:

	void decorate(const Function& f) const;

protected:
	/** Visit an expression. */
	virtual void visit(const ExprNode& n);
	/** Visit an indexed expression. */
	virtual void visit(const ExprIndex& idx);
	/** Visit a symbol. */
	virtual void visit(const ExprSymbol& s);
	/** Visit a constant. */
	virtual void visit(const ExprConstant& c);
	/** Visit a n-ary operator. */
	virtual void visit(const ExprNAryOp& a);
	/** Visit a binary operator. */
	virtual void visit(const ExprBinaryOp& b);
	/** Visit an unary operator. */
	virtual void visit(const ExprUnaryOp& u);
	/** Visit a vector of expressions. */
	virtual void visit(const ExprVector& v);
	/** Visit a function application. */
	virtual void visit(const ExprApply&);
};

/**
 * \ingroup symbolic
 * \brief Calculates the gradient of a function.
 */
class Gradient : public FwdAlgorithm<GradLabel>, BwdAlgorithm<GradLabel> {

public:
	/**
	 * \brief The function f (not decorated).
	 */
	Gradient(const Function& f);

	/**
	 * \brief Calculate the gradient on the box \a box and store the result in \a g.
	 */
	void calculate(const IntervalVector& box, IntervalVector& g) const;

	/**
	 * \brief Calculate the Jacobian on the box \a box and store the result in \a J.
	 */
	void jacobian(const IntervalVector& box, IntervalMatrix& J) const;

private:
	friend class CompiledFunction;

	Eval eval;

	inline void index_fwd(const ExprIndex& e, const GradLabel& x, GradLabel& y);
	       void vector_fwd(const ExprVector& v, const GradLabel** compL, GradLabel& y);
	inline void cst_fwd(const ExprConstant& c, GradLabel& y);
	inline void symbol_fwd(const ExprSymbol& s, GradLabel& y);
	       void apply_fwd(const ExprApply& a, const GradLabel** argL, GradLabel& y);
	inline void add_fwd(const ExprAdd&, const GradLabel, const GradLabel, GradLabel& y)     { y.g.i()=0; }
	inline void mul_fwd(const ExprMul&, const GradLabel, const GradLabel, GradLabel& y)     { y.g.i()=0; }
	inline void sub_fwd(const ExprSub&, const GradLabel, const GradLabel, GradLabel& y)     { y.g.i()=0; }
	inline void div_fwd(const ExprDiv&, const GradLabel, const GradLabel, GradLabel& y)     { y.g.i()=0; }
	inline void max_fwd(const ExprMax&, const GradLabel, const GradLabel, GradLabel& y)     { y.g.i()=0; }
	inline void min_fwd(const ExprMin&, const GradLabel, const GradLabel, GradLabel& y)     { y.g.i()=0; }
	inline void atan2_fwd(const ExprAtan2&, const GradLabel, const GradLabel, GradLabel& y) { y.g.i()=0; }
	inline void minus_fwd(const ExprMinus&, const GradLabel&, GradLabel& y)   { y.g.i()=0; }
	inline void sign_fwd(const ExprSign&, const GradLabel&, GradLabel& y)     { y.g.i()=0; }
	inline void abs_fwd(const ExprAbs&, const GradLabel&, GradLabel& y)       { y.g.i()=0; }
	inline void power_fwd(const ExprPower& p, const GradLabel&, GradLabel& y) { y.g.i()=0; }
	inline void sqr_fwd(const ExprSqr&, const GradLabel&, GradLabel& y)       { y.g.i()=0; }
	inline void sqrt_fwd(const ExprSqrt&, const GradLabel&, GradLabel& y)     { y.g.i()=0; }
	inline void exp_fwd(const ExprExp&, const GradLabel&, GradLabel& y)       { y.g.i()=0; }
	inline void log_fwd(const ExprLog&, const GradLabel&, GradLabel& y)       { y.g.i()=0; }
	inline void cos_fwd(const ExprCos&, const GradLabel&, GradLabel& y)       { y.g.i()=0; }
	inline void sin_fwd(const ExprSin&, const GradLabel&, GradLabel& y)       { y.g.i()=0; }
	inline void tan_fwd(const ExprTan&, const GradLabel&, GradLabel& y)       { y.g.i()=0; }
	inline void cosh_fwd(const ExprCosh&, const GradLabel&, GradLabel& y)     { y.g.i()=0; }
	inline void sinh_fwd(const ExprSinh&, const GradLabel&, GradLabel& y)     { y.g.i()=0; }
	inline void tanh_fwd(const ExprTanh&, const GradLabel&, GradLabel& y)     { y.g.i()=0; }
	inline void acos_fwd(const ExprAcos&, const GradLabel&, GradLabel& y)     { y.g.i()=0; }
	inline void asin_fwd(const ExprAsin&, const GradLabel&, GradLabel& y)     { y.g.i()=0; }
	inline void atan_fwd(const ExprAtan&, const GradLabel&, GradLabel& y)     { y.g.i()=0; }
	inline void acosh_fwd(const ExprAcosh&, const GradLabel&, GradLabel& y)   { y.g.i()=0; }
	inline void asinh_fwd(const ExprAsinh&, const GradLabel&, GradLabel& y)   { y.g.i()=0; }
	inline void atanh_fwd(const ExprAtanh&, const GradLabel&, GradLabel& y)   { y.g.i()=0; }

	inline void add_V_fwd(const ExprAdd&, const GradLabel&, const GradLabel&, GradLabel& y)  { y.g.v().clear(); }
	inline void add_M_fwd(const ExprAdd&, const GradLabel&, const GradLabel&, GradLabel& y)  { y.g.m().clear(); }
	inline void mul_SV_fwd(const ExprMul&, const GradLabel&, const GradLabel&, GradLabel& y) { y.g.v().clear(); }
	inline void mul_SM_fwd(const ExprMul&, const GradLabel&, const GradLabel&, GradLabel& y) { y.g.m().clear(); }
	inline void mul_VV_fwd(const ExprMul&, const GradLabel&, const GradLabel&, GradLabel& y) { y.g.i()=0; }
	inline void mul_MV_fwd(const ExprMul&, const GradLabel&, const GradLabel&, GradLabel& y) { y.g.v().clear(); }
	inline void mul_VM_fwd(const ExprMul&, const GradLabel&, const GradLabel&, GradLabel& y) { y.g.v().clear(); }
	inline void mul_MM_fwd(const ExprMul&, const GradLabel&, const GradLabel&, GradLabel& y) { y.g.m().clear(); }
	inline void sub_V_fwd(const ExprSub&, const GradLabel&, const GradLabel&, GradLabel& y)  { y.g.v().clear(); }
	inline void sub_M_fwd(const ExprSub&, const GradLabel&, const GradLabel&, GradLabel& y)  { y.g.m().clear(); }


	inline void index_bwd (const ExprIndex&,   GradLabel& x,                 const GradLabel& y);
	       void vector_bwd(const ExprVector&,  GradLabel** x,                const GradLabel& y);
	inline void symbol_bwd(const ExprSymbol&,                                const GradLabel& y) { /* nothing to do */ }
	inline void cst_bwd   (const ExprConstant&,                              const GradLabel& y) { /* nothing to do */ }
	inline void apply_bwd (const ExprApply&,   GradLabel** x,                const GradLabel& y);
	inline void add_bwd   (const ExprAdd&,     GradLabel& x1, GradLabel& x2, const GradLabel& y) { x1.g.i() += y.g.i();          x2.g.i() += y.g.i(); }
	inline void mul_bwd   (const ExprMul&,     GradLabel& x1, GradLabel& x2, const GradLabel& y) { x1.g.i() += y.g.i() * x2.i(); x2.g.i() += y.g.i() * x1.i(); }
	inline void sub_bwd   (const ExprSub&,     GradLabel& x1, GradLabel& x2, const GradLabel& y) { x1.g.i() += y.g.i();          x2.g.i() += -y.g.i();}
	inline void div_bwd   (const ExprDiv&,     GradLabel& x1, GradLabel& x2, const GradLabel& y) { x1.g.i() += y.g.i() / x2.i(); x2.g.i() += y.g.i()*(-x1.i())/sqr(x2.i()); }
	inline void max_bwd   (const ExprMax&,     GradLabel& x1, GradLabel& x2, const GradLabel& y) { /* not implemented yet */ assert(false); }
	inline void min_bwd   (const ExprMin&,     GradLabel& x1, GradLabel& x2, const GradLabel& y) { /* not implemented yet */ assert(false); }
	inline void atan2_bwd (const ExprAtan2& e, GradLabel& x1, GradLabel& x2, const GradLabel& y) { /* not implemented yet */ assert(false); }
	inline void minus_bwd (const ExprMinus& e, GradLabel& x,                 const GradLabel& y) { x.g.i() += -1.0*y.g.i(); }
           void sign_bwd  (const ExprSign& e,  GradLabel& x,                 const GradLabel& y);
	       void abs_bwd   (const ExprAbs& e,   GradLabel& x,                 const GradLabel& y);
	inline void power_bwd (const ExprPower& e, GradLabel& x,                 const GradLabel& y) { x.g.i() += y.g.i() * e.expon * pow(x.i(), e.expon-1); }
	inline void sqr_bwd   (const ExprSqr& e,   GradLabel& x,                 const GradLabel& y) { x.g.i() += y.g.i() * 2.0 * x.i(); }
	inline void sqrt_bwd  (const ExprSqrt& e,  GradLabel& x,                 const GradLabel& y) { x.g.i() += y.g.i() * 0.5 / sqrt(x.i());  }
	inline void exp_bwd   (const ExprExp& e,   GradLabel& x,                 const GradLabel& y) { x.g.i() += y.g.i() * exp(x.i()); }
	inline void log_bwd   (const ExprLog& e,   GradLabel& x,                 const GradLabel& y) { x.g.i() += y.g.i() / x.i(); }
	inline void cos_bwd   (const ExprCos& e,   GradLabel& x,                 const GradLabel& y) { x.g.i() += y.g.i() * -sin(x.i()); }
	inline void sin_bwd   (const ExprSin& e,   GradLabel& x,                 const GradLabel& y) { x.g.i() += y.g.i() * cos(x.i()); }
	inline void tan_bwd   (const ExprTan& e,   GradLabel& x,                 const GradLabel& y) { x.g.i() += y.g.i() * (1.0 + sqr(tan(x.i()))); }
	inline void cosh_bwd  (const ExprCosh& e,  GradLabel& x,                 const GradLabel& y) { x.g.i() += y.g.i() * sinh(x.i()); }
	inline void sinh_bwd  (const ExprSinh& e,  GradLabel& x,                 const GradLabel& y) { x.g.i() += y.g.i() * cosh(x.i()); }
	inline void tanh_bwd  (const ExprTanh& e,  GradLabel& x,                 const GradLabel& y) { x.g.i() += y.g.i() * (1.0 - sqr(tanh(x.i()))); }
	inline void acos_bwd  (const ExprAcos& e,  GradLabel& x,                 const GradLabel& y) { x.g.i() += y.g.i() * -1.0 / sqrt(1.0-sqr(x.i())); }
	inline void asin_bwd  (const ExprAsin& e,  GradLabel& x,                 const GradLabel& y) { x.g.i() += y.g.i() * 1.0 / sqrt(1.0-sqr(x.i())); }
	inline void atan_bwd  (const ExprAtan& e,  GradLabel& x,                 const GradLabel& y) { x.g.i() += y.g.i() * 1.0 / (1.0+sqr(x.i())); }
	inline void acosh_bwd (const ExprAcosh& e, GradLabel& x,                 const GradLabel& y) { x.g.i() += y.g.i() * 1.0 / sqrt(sqr(x.i()) -1.0); }
	inline void asinh_bwd (const ExprAsinh& e, GradLabel& x,                 const GradLabel& y) { x.g.i() += y.g.i() * 1.0 / sqrt(1.0+sqr(x.i())); }
	inline void atanh_bwd (const ExprAtanh& e, GradLabel& x,                 const GradLabel& y) { x.g.i() += y.g.i() * 1.0 / (1.0-sqr(x.i())); }

	inline void add_V_bwd (const ExprAdd&, GradLabel& x1, GradLabel& x2, const GradLabel& y) { x1.g.v() += y.g.v();        x2.g.v() += y.g.v(); }
	inline void add_M_bwd (const ExprAdd&, GradLabel& x1, GradLabel& x2, const GradLabel& y) { x1.g.m() += y.g.m();        x2.g.m() += y.g.m(); }
	inline void mul_SV_bwd(const ExprMul&, GradLabel& x1, GradLabel& x2, const GradLabel& y) { x1.g.i() += y.g.v()*x2.v(); x2.g.v() += x1.i()*y.g.v(); }
	inline void mul_SM_bwd(const ExprMul&, GradLabel& x1, GradLabel& x2, const GradLabel& y) { for (int i=0; i<y.m().nb_rows(); i++) x1.g.i()+=y.g.m()[i]*x2.m()[i]; x2.g.m() += x1.i()*y.g.m();  }
	inline void mul_VV_bwd(const ExprMul&, GradLabel& x1, GradLabel& x2, const GradLabel& y) { x1.g.v() += y.g.i()*x2.v(); x2.g.v() += y.g.i()*x1.v(); }
	inline void mul_MV_bwd(const ExprMul&, GradLabel& x1, GradLabel& x2, const GradLabel& y) { x1.g.m() += outer_product(y.g.v(),x2.v()); x2.g.v() += x1.m().transpose()*y.g.v(); }
	inline void mul_MM_bwd(const ExprMul&, GradLabel& x1, GradLabel& x2, const GradLabel& y) { x1.g.m() += y.g.m()*x2.m().transpose(); x2.g.m() += x1.m().transpose()*y.g.m(); }
	inline void sub_V_bwd (const ExprSub&, GradLabel& x1, GradLabel& x2, const GradLabel& y) { x1.g.v() += y.g.v(); x2.g.v() -= y.g.v(); }
	inline void sub_M_bwd (const ExprSub&, GradLabel& x1, GradLabel& x2, const GradLabel& y) { x1.g.m() += y.g.m(); x2.g.m() -= y.g.m(); }
};

} // namespace ibex
#endif // __IBEX_GRADIENT_H__
