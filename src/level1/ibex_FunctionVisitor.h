/* ============================================================================
 * I B E X - Interface for function visitors
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Jan 06, 2012
 * ---------------------------------------------------------------------------- */

#ifndef _IBEX_FUNC_VISITOR_H_
#define _IBEX_FUNC_VISITOR_H_

#include "Interval.h"
#include "IntervalMatrix.h"

namespace ibex {

class ExprNode;

class ExprIndex;
class ExprVector;
class ExprSymbol;
class ExprConstant;

class ExprBinaryOp;
class ExprAdd;
class ExprMul;
class ExprSub;
class ExprDiv;
class ExprMax;
class ExprMin;
class ExprAtan2;

class ExprUnaryOp;
class ExprMinus;
class ExprSign;
class ExprAbs;
class ExprPower;
class ExprSqr;
class ExprSqrt;
class ExprExp;
class ExprLog;
class ExprCos;
class ExprSin;
class ExprTan;
class ExprCosh;
class ExprSinh;
class ExprTanh;
class ExprAcos;
class ExprAsin;
class ExprAtan;
class ExprAcosh;
class ExprAsinh;
class ExprAtanh;

class ExprApply;

/*
 * \ingroup level1
 * \brief Function visitor.
 *
 * An interface for visitors of #Function instances.
 *
 * \author Gilles Chabert
 * \date March 2007
 */
class FunctionVisitor {

 public:
  virtual ~FunctionVisitor() { }

  /*===============================================================*/
  /*                mandatory functions                            */
  /*============================================================== */

  /** Visit an expression. */
  virtual void visit(const ExprNode&)=0;

  /** Visit an indexed expression. */
  virtual void visit(const ExprIndex&)=0;

  /** Visit a vector of expressions. */
  virtual void visit(const ExprVector&)=0;

  /** Visit a symbol. */
  virtual void visit(const ExprSymbol&)=0;

  /** Visit a constant. */
  virtual void visit(const ExprConstant&)=0;

  /** Visit an unary operator. */
  virtual void visit(const ExprUnaryOp&)=0;

  /** Visit a binary operator. */
  virtual void visit(const ExprBinaryOp&)=0;

  /** Visit a function application. */
  virtual void visit(const ExprApply&)=0;


  /*===============================================================*/
  /*                optional functions                             */
  /*===============================================================*/

  /*==================== binary operators =========================*/
  /** Visit an addition (Implementation is not mandatory). */
  virtual void visit(const ExprAdd&) { }

  /** Visit an multiplication (Implementation is not mandatory). */
  virtual void visit(const ExprMul&) { }

  /** Visit a subtraction (Implementation is not mandatory). */
  virtual void visit(const ExprSub&) { }

  /** Visit a division (Implementation is not mandatory). */
  virtual void visit(const ExprDiv&) { }

  /** Visit a max  (Implementation is not mandatory). */
  virtual void visit(const ExprMax&) { }

  /** Visit a min (Implementation is not mandatory). */
  virtual void visit(const ExprMin&) { }

  /** Visit a arctan2 (Implementation is not mandatory). */
  virtual void visit(const ExprAtan2&) { }


  /*==================== unary operators =========================*/
  /** Visit a sqr (Implementation is not mandatory). */
  virtual void visit(const ExprMinus&) { }

  /** Visit a sign (Implementation is not mandatory). */
  virtual void visit(const ExprSign&) { }

    /** Visit an abs (Implementation is not mandatory). */
  virtual void visit(const ExprAbs&) { }

  /** Visit a power expression. */
  virtual void visit(const ExprPower&) { }

  /** Visit a sqr (Implementation is not mandatory). */
  virtual void visit(const ExprSqr&) { }

  /** Visit a sqrt (Implementation is not mandatory). */
  virtual void visit(const ExprSqrt&) { }

  /** Visit an exp (Implementation is not mandatory). */
  virtual void visit(const ExprExp&) { }

  /** Visit a log (Implementation is not mandatory). */
  virtual void visit(const ExprLog&) { }

  /** Visit a cos (Implementation is not mandatory). */
  virtual void visit(const ExprCos&) { }

  /** Visit a sin (Implementation is not mandatory). */
  virtual void visit(const ExprSin&) { }

  /** Visit a tan (Implementation is not mandatory). */
  virtual void visit(const ExprTan&) { }

  /** Visit a cosh (Implementation is not mandatory). */
  virtual void visit(const ExprCosh&) { }

  /** Visit a sinh (Implementation is not mandatory). */
  virtual void visit(const ExprSinh&) { }

  /** Visit a tanh (Implementation is not mandatory). */
  virtual void visit(const ExprTanh&) { }

  /** Visit a acos (Implementation is not mandatory). */
  virtual void visit(const ExprAcos&) { }

  /** Visit a asin (Implementation is not mandatory). */
  virtual void visit(const ExprAsin&) { }

  /** Visit a atan (Implementation is not mandatory). */
  virtual void visit(const ExprAtan&) { }

  /** Visit a acosh (Implementation is not mandatory). */
  virtual void visit(const ExprAcosh&) { }

  /** Visit a asinh (Implementation is not mandatory). */
  virtual void visit(const ExprAsinh&) { }

  /** Visit a atanh (Implementation is not mandatory). */
  virtual void visit(const ExprAtanh&) { }

 protected:
  /* The following protected methods are introduced because
     friendship grant is not inherited.
     They serve for visitors to access private
     information of #ExprNode subclasses.
   */
};

} // end namespace ibex

#endif // _IBEX_FUNC_VISITOR_H_
