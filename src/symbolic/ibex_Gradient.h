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
 * \ingroup level1
 * \brief Label for the gradient computation.
 */
class GradLabel : public Domain {
public:
	GradLabel(const Dim& dim) : Domain(dim) { }

	Interval g; // we don't manage yet vector/matrix operations
};

class ApplyGradLabel : public BasicApplyLabel {
/* to do */
};

/**
 * \ingroup level1
 * \brief Decorates a function with the label for gradient computation.
 */

class GradDecorator : public Decorator, public FunctionVisitor {
public:

	void decorate(const Function& f) const {
		if (f.expr().deco!=NULL) {
			return; //throw NonRecoverableException("Cannot re-decorate a function");
		}
		((GradDecorator&) *this).visit(f.expr()); // // cast -> we know *this will not be modified
	}

protected:
	 /** Visit an expression. */
	  virtual void visit(const ExprNode& n) {
		  n.acceptVisitor(*this);
	  }

	  /** Visit an indexed expression. */
	  virtual void visit(const ExprIndex& idx) {
		  idx.deco = new GradLabel(idx.dim);
	  }

	  /** Visit a vector of expressions. */
	  virtual void visit(const ExprVector& v) {
		  v.deco = new GradLabel(v.dim);
	  }

	  /** Visit a symbol. */
	  virtual void visit(const ExprSymbol& s) {
		  s.deco = new GradLabel(s.dim);
	  }

	  /** Visit a constant. */
	  virtual void visit(const ExprConstant& c) {
		  c.deco = new GradLabel(c.dim);
	  }

	  /** Visit a n-ary operator. */
	  virtual void visit(const ExprNAryOp& a) {
		  a.deco = new GradLabel(a.dim);
	  }

	  /** Visit a binary operator. */
	  virtual void visit(const ExprBinaryOp& b) {
		  b.deco = new GradLabel(b.dim);
	  }

	  /** Visit an unary operator. */
	  virtual void visit(const ExprUnaryOp& u) {
		  u.deco = new GradLabel(u.dim);
	  }

};

/**
 * \ingroup level1
 * \brief Calculates the gradient of a function.
 */
class Gradient : public BwdAlgorithm<GradLabel> {

public:
	/**
	 * \brief The function f (not decorated).
	 */
	Gradient(const Function& f);

	/**
	 * \brief Calculate the gradient on the box \a box and store the result in \a g.
	 */
	void calculate(const IntervalVector& box, IntervalVector& g) const;

private:
	friend class CompiledFunction;

	Eval eval;

	inline void index_bwd (const ExprIndex&,   GradLabel& exprL,                    const GradLabel& result) { /* nothing to do */ }
	inline void vector_bwd(const ExprVector&,  GradLabel** compL,                   const GradLabel& result) {  }
	inline void symbol_bwd(const ExprSymbol&,                                       const GradLabel& result) { /* nothing to do */ }
	inline void cst_bwd   (const ExprConstant&,                                     const GradLabel& result) { /* nothing to do */ }
	inline void apply_bwd (const ExprApply&,   GradLabel** argL,                    const GradLabel& result) {  }
	inline void add_bwd   (const ExprAdd&,     GradLabel& leftL, GradLabel& rightL, const GradLabel& result) {  }
	inline void mul_bwd   (const ExprMul&,     GradLabel& leftL, GradLabel& rightL, const GradLabel& result) {  }
	inline void sub_bwd   (const ExprSub&,     GradLabel& leftL, GradLabel& rightL, const GradLabel& result) {  }
	inline void div_bwd   (const ExprDiv&,     GradLabel& leftL, GradLabel& rightL, const GradLabel& result) {  }
	inline void max_bwd   (const ExprMax&,     GradLabel& leftL, GradLabel& rightL, const GradLabel& result) {  }
	inline void min_bwd   (const ExprMin&,     GradLabel& leftL, GradLabel& rightL, const GradLabel& result) {  }
	inline void atan2_bwd (const ExprAtan2& e, GradLabel& leftL, GradLabel& rightL, const GradLabel& result) {  }
	inline void minus_bwd (const ExprMinus& e, GradLabel& exprL,                    const GradLabel& result) {  }
	inline void sign_bwd  (const ExprSign& e,  GradLabel& exprL,                    const GradLabel& result) {  }
	inline void abs_bwd   (const ExprAbs& e,   GradLabel& exprL,                    const GradLabel& result) {  }
	inline void power_bwd (const ExprPower& e, GradLabel& exprL,                    const GradLabel& result) {  }
	inline void sqr_bwd   (const ExprSqr& e,   GradLabel& exprL,                    const GradLabel& result) {  }
	inline void sqrt_bwd  (const ExprSqrt& e,  GradLabel& exprL,                    const GradLabel& result) {  }
	inline void exp_bwd   (const ExprExp& e,   GradLabel& exprL,                    const GradLabel& result) {  }
	inline void log_bwd   (const ExprLog& e,   GradLabel& exprL,                    const GradLabel& result) {  }
	inline void cos_bwd   (const ExprCos& e,   GradLabel& exprL,                    const GradLabel& result) {  }
	inline void sin_bwd   (const ExprSin& e,   GradLabel& exprL,                    const GradLabel& result) {  }
	inline void tan_bwd   (const ExprTan& e,   GradLabel& exprL,                    const GradLabel& result) {  }
	inline void cosh_bwd  (const ExprCosh& e,  GradLabel& exprL,                    const GradLabel& result) {  }
	inline void sinh_bwd  (const ExprSinh& e,  GradLabel& exprL,                    const GradLabel& result) {  }
	inline void tanh_bwd  (const ExprTanh& e,  GradLabel& exprL,                    const GradLabel& result) {  }
	inline void acos_bwd  (const ExprAcos& e,  GradLabel& exprL,                    const GradLabel& result) {  }
	inline void asin_bwd  (const ExprAsin& e,  GradLabel& exprL,                    const GradLabel& result) {  }
	inline void atan_bwd  (const ExprAtan& e,  GradLabel& exprL,                    const GradLabel& result) {  }
	inline void acosh_bwd (const ExprAcosh& e, GradLabel& exprL,                    const GradLabel& result) {  }
	inline void asinh_bwd (const ExprAsinh& e, GradLabel& exprL,                    const GradLabel& result) {  }
	inline void atanh_bwd (const ExprAtanh& e, GradLabel& exprL,                    const GradLabel& result) {  }

};

} // namespace ibex
#endif // __IBEX_GRADIENT_H__
