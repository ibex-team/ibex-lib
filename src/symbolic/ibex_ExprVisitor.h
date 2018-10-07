/* ============================================================================
 * I B E X - Interface for expression visitors
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Jan 06, 2012
 * ---------------------------------------------------------------------------- */

#ifndef __IBEX_EXPR_VISITOR_H__
#define __IBEX_EXPR_VISITOR_H__

#include "ibex_Interval.h"
#include "ibex_IntervalMatrix.h"

namespace ibex {

class ExprNode;

class ExprIndex;
class ExprLeaf;
class ExprNAryOp;
class ExprBinaryOp;
class ExprUnaryOp;

class ExprSymbol;
class ExprConstant;

class ExprVector;
class ExprApply;
class ExprChi;

class ExprGenericBinaryOp;

class ExprAdd;
class ExprMul;
class ExprSub;
class ExprDiv;
class ExprMax;
class ExprMin;
class ExprAtan2;

class ExprGenericUnaryOp;

class ExprMinus;
class ExprTrans;
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

/*
 * \ingroup level1
 * \brief Function visitor.
 *
 * An interface for visitors of #Function instances.
 *
 * \author Gilles Chabert
 * \date March 2007
 */
class ExprVisitor {

 public:
  virtual ~ExprVisitor() { }

  /*===============================================================*/
  /*                mandatory functions                            */
  /*============================================================== */

  /** Visit an expression. */
  virtual void visit(const ExprNode&) {
	  ibex_error("ExprVisitor: Missing implementation for some node type");
  }

  /** Visit an indexed expression. */
  virtual void visit(const ExprIndex& e) {
	  visit((const ExprNode&) e);
  }

  /** Visit a leaf. */
  virtual void visit(const ExprLeaf& e) {
	  visit((const ExprNode&) e);
  }

  /** Visit a n-ary operator. */
  virtual void visit(const ExprNAryOp& e) {
	  visit((const ExprNode&) e);
  }

  /** Visit a binary operator. */
  virtual void visit(const ExprBinaryOp& e) {
	  visit((const ExprNode&) e);
  }

  /** Visit an unary operator. */
  virtual void visit(const ExprUnaryOp& e) {
	  visit((const ExprNode&) e);
  }

  /*===============================================================*/
  /*                optional functions                             */
  /*===============================================================*/

  /*==================== 0-ary operators =========================*/
  /** Visit a symbol. */
  virtual void visit(const ExprSymbol& e) {
	   visit((const ExprLeaf&) e);
  }

  /** Visit a constant. */
  virtual void visit(const ExprConstant& e) {
	   visit((const ExprLeaf&) e);
  }

  /*==================== n-ary operators =========================*/
  /** Visit a vector of expressions.
   * By default: call visit(const ExprNAryOp& e). */
   virtual void visit(const ExprVector& e) {
	   visit((const ExprNAryOp&) e);
   }

   /** Visit a function application.
   * By default: call visit(const ExprNAryOp& e). */
   virtual void visit(const ExprApply& e) {
	   visit((const ExprNAryOp&) e);
   }

   /** Visit a function Chi.
   * By default: call visit(const ExprNAryOp& e). */
   virtual void visit(const ExprChi& e) {
	   visit((const ExprNAryOp&) e);
   }

  /*==================== binary operators =========================*/
   /** Visit a generic binary operator (Implementation is not mandatory).
    * By default: call visit(const ExprBinaryOp& e). */
   virtual void visit(const ExprGenericBinaryOp& e) {
 	  visit((const ExprBinaryOp&) e);
   }

   /** Visit an addition (Implementation is not mandatory).
   * By default: call visit(const ExprBinaryOp& e). */
  virtual void visit(const ExprAdd& e) {
	  visit((const ExprBinaryOp&) e);
  }

  /** Visit an multiplication (Implementation is not mandatory).
   * By default: call visit(const ExprBinaryOp& e). */
  virtual void visit(const ExprMul& e) {
	  visit((const ExprBinaryOp&) e);
  }

  /** Visit a subtraction (Implementation is not mandatory).
   * By default: call visit(const ExprBinaryOp& e). */
  virtual void visit(const ExprSub& e) {
	  visit((const ExprBinaryOp&) e);
  }

  /** Visit a division (Implementation is not mandatory).
   * By default: call visit(const ExprBinaryOp& e). */
  virtual void visit(const ExprDiv& e) {
	  visit((const ExprBinaryOp&) e);
  }

  /** Visit a max  (Implementation is not mandatory).
   * By default: call visit(const ExprBinaryOp& e). */
  virtual void visit(const ExprMax& e) {
	  visit((const ExprBinaryOp&) e);
  }

  /** Visit a min (Implementation is not mandatory).
   * By default: call visit(const ExprBinaryOp& e). */
  virtual void visit(const ExprMin& e) {
	  visit((const ExprBinaryOp&) e);
  }

  /** Visit a arctan2 (Implementation is not mandatory).
   * By default: call visit(const ExprBinaryOp& e). */
  virtual void visit(const ExprAtan2& e) {
	  visit((const ExprBinaryOp&) e);
  }


  /*==================== unary operators =========================*/

  /** Visit a generic unary operator (Implementation is not mandatory).
     * By default call visit(const ExprUnaryOp&). */
    virtual void visit(const ExprGenericUnaryOp& e) {
  	  visit((const ExprUnaryOp&) e);
    }

  /** Visit a minus sign (Implementation is not mandatory).
   * By default call visit(const ExprUnaryOp&). */
  virtual void visit(const ExprMinus& e) {
	  visit((const ExprUnaryOp&) e);
  }

  /** Visit a transpose (Implementation is not mandatory).
   * By default call visit(const ExprUnaryOp&). */
  virtual void visit(const ExprTrans& e) {
	  visit((const ExprUnaryOp&) e);
  }

  /** Visit a sign (Implementation is not mandatory).
   * By default call visit(const ExprUnaryOp&). */
  virtual void visit(const ExprSign& e) {
	  visit((const ExprUnaryOp&) e);
  }

  /** Visit an abs (Implementation is not mandatory).
   * By default call visit(const ExprUnaryOp&). */
  virtual void visit(const ExprAbs& e) {
	  visit((const ExprUnaryOp&) e);
  }

  /** Visit a power expression.
   * By default call visit(const ExprUnaryOp&). */
  virtual void visit(const ExprPower& e) {
	  visit((const ExprUnaryOp&) e);
  }

  /** Visit a sqr (Implementation is not mandatory).
   * By default call visit(const ExprUnaryOp&). */
  virtual void visit(const ExprSqr& e) {
	  visit((const ExprUnaryOp&) e);
  }

  /** Visit a sqrt (Implementation is not mandatory).
   * By default call visit(const ExprUnaryOp&). */
  virtual void visit(const ExprSqrt& e) {
	  visit((const ExprUnaryOp&) e);
  }

  /** Visit an exp (Implementation is not mandatory).
   * By default call visit(const ExprUnaryOp&). */
  virtual void visit(const ExprExp& e) {
	  visit((const ExprUnaryOp&) e);
  }

  /** Visit a log (Implementation is not mandatory).
   * By default call visit(const ExprUnaryOp&). */
  virtual void visit(const ExprLog& e) {
	  visit((const ExprUnaryOp&) e);
  }

  /** Visit a cos (Implementation is not mandatory).
   * By default call visit(const ExprUnaryOp&). */
  virtual void visit(const ExprCos& e) {
	  visit((const ExprUnaryOp&) e);
  }

  /** Visit a sin (Implementation is not mandatory).
   * By default call visit(const ExprUnaryOp&). */
  virtual void visit(const ExprSin& e) {
	  visit((const ExprUnaryOp&) e);
  }

  /** Visit a tan (Implementation is not mandatory).
   * By default call visit(const ExprUnaryOp&). */
  virtual void visit(const ExprTan& e) {
	  visit((const ExprUnaryOp&) e);
  }

  /** Visit a cosh (Implementation is not mandatory).
   * By default call visit(const ExprUnaryOp&). */
  virtual void visit(const ExprCosh& e) {
	  visit((const ExprUnaryOp&) e);
  }

  /** Visit a sinh (Implementation is not mandatory).
   * By default call visit(const ExprUnaryOp&). */
  virtual void visit(const ExprSinh& e) {
	  visit((const ExprUnaryOp&) e);
  }

  /** Visit a tanh (Implementation is not mandatory).
   * By default call visit(const ExprUnaryOp&). */
  virtual void visit(const ExprTanh& e) {
	  visit((const ExprUnaryOp&) e);
  }

  /** Visit a acos (Implementation is not mandatory).
   * By default call visit(const ExprUnaryOp&). */
  virtual void visit(const ExprAcos& e) {
	  visit((const ExprUnaryOp&) e);
  }

  /** Visit a asin (Implementation is not mandatory).
   * By default call visit(const ExprUnaryOp&). */
  virtual void visit(const ExprAsin& e) {
	  visit((const ExprUnaryOp&) e);
  }

  /** Visit a atan (Implementation is not mandatory).
   * By default call visit(const ExprUnaryOp&). */
  virtual void visit(const ExprAtan& e) {
	  visit((const ExprUnaryOp&) e);
  }

  /** Visit a acosh (Implementation is not mandatory).
   * By default call visit(const ExprUnaryOp&). */
  virtual void visit(const ExprAcosh& e) {
	  visit((const ExprUnaryOp&) e);
  }

  /** Visit a asinh (Implementation is not mandatory).
   * By default call visit(const ExprUnaryOp&). */
  virtual void visit(const ExprAsinh& e) {
	  visit((const ExprUnaryOp&) e);
  }

  /** Visit a atanh (Implementation is not mandatory).
   * By default call visit(const ExprUnaryOp&). */
  virtual void visit(const ExprAtanh& e) {
	  visit((const ExprUnaryOp&) e);
  }

 protected:
  /* The following protected methods are introduced because
     friendship grant is not inherited.
     They serve for visitors to access private
     information of #ExprNode subclasses.
   */
};

} // end namespace ibex

#endif // __IBEX_EXPR_VISITOR_H__
