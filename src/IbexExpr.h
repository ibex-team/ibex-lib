/*---------------------------------------------------------------------------------
 * 
 * Abstract syntax DAG of expressions
 * -----------------------------------
 *
 * Copyright (C) 2007-2009 Gilles Chabert
 * 
 * This file is part of IBEX.
 *
 * IBEX is free software; you can redistribute it and/or modify it under the terms of 
 * the GNU General Public License as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later version.
 *
 * IBEX is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; 
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
 * PURPOSE. See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with IBEX; 
 * if not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, 
 * Boston, MA 02110-1301, USA 
 *
 --------------------------------------------------------------------------------*/

#ifndef _IBEX_EXPR_H
#define _IBEX_EXPR_H

#include <vector>

#include "IbexToken.h"
#include "IbexVisitor.h"
#include "IbexException.h"
#include "IbexDomain.h"
#include "IbexEnv.h"
#include "IbexEvaluator.h"


namespace ibex {

/**\defgroup exprgroup Expressions */
/*@{*/

class Constraint;
class Equality;
class Inequality;

/** 
 * \brief Arithmetical expression
 *
 * Representation of the abstract syntax tree of any <i>arithmetical expression</i>.
 * \author Gilles Chabert
 * \date March 2007
 *
 * Example of <i>arithmetical expression</i>: sin(x+y)^2*3.14.
 */
class Expr { 

 public:  
  /** Builds an expression of a given depth, size and dimension. 
   * \see #depth, #size, #dim. */
  Expr(Env& env, int depth, int size, const Dim& dim);

  /** Accept an #ibex::Expr visitor. */
  virtual void acceptVisitor(Visitor& v) const = 0;

  friend class Visitor;

  /** Deletes this instance. */
  virtual ~Expr() { }

  /** Streams out this expression. */
  friend std::ostream& operator<<(std::ostream&, const Expr&);
   
  /** The environment where this expresion is defined */
  Env& env;

  /** Depth (following topological order) of this node in the DAG. */
  const int depth;        

  /** Number of subnodes (including itself) in the DAG (not in the TREE:
   * two subnodes referencing the same object count for 1).*/
  const int size;         

  /** Unique number identifying this node. */
  const int id;  

  /** Dimensions */
  const Dim dim; 

  /** Return true if this expression is the constant 0. */
  virtual bool is_zero() const { return false; }

  /** Return true if this is a scalar expression. */
  bool scalar() const { return dim.scalar(); }

  /** Create an equality constraint expr=expr. */
  const Equality& operator=(const Expr& right) const;
  
  /** Create an equality constraint expr=value. */
  const Equality& operator=(const INTERVAL& value) const;
  
  /** Create an inequality constraint expr<=expr. */
  const Inequality& operator<=(const Expr& right) const;
  
  /** Create an inequality constraint expr<=value. */
  const Inequality& operator<=(const INTERVAL& value) const;
  
  /** Create an inequality constraint expr>=expr. */
  const Inequality& operator>=(const Expr& right) const;
  
  /** Create an inequality constraint expr>=value. */
  const Inequality& operator>=(const INTERVAL& value) const;
  
  /** Create an inequality constraint expr<value. */
  const Inequality& operator<(const Expr& right) const;
  
  /** Create an inequality constraint expr<value. */
  const Inequality& operator<(const INTERVAL& value) const;
  
  /** Create an inequality constraint expr>expr. */
  const Inequality& operator>(const Expr& right) const;
  
  /** Create an inequality constraint expr>value. */
  const Inequality& operator>(const INTERVAL& value) const;
};


class SymbolWithIndex;

/** 
 * \brief Extended symbol 
 *
 * An <i>extended symbol</i> is a symbol with possibly one a several indexes such as \a x[1][2].
 *
 * \author Gilles Chabert
 * \date 2007
 */
class ExtendedSymbol : public Expr {

 public:
  /** Create an equality constraint symbol=expr. */
  const Equality& operator=(const Expr& expr) const { return ((Expr&) *this)=expr; }

  /** Create an equality constraint symbol=value. */
  const Equality& operator=(const INTERVAL& value) const  { return ((Expr&) *this)=value; }

  /** Create an indexed expression. */  
  ExtendedSymbol(Env& env, int depth, int size, const Dim& dim, int key) : Expr(env, depth,size,dim), key(key) { }

  /** Accept an #ibex::Expr visitor. */
  virtual void acceptVisitor(Visitor& v) const=0;

  /** Create an indexed symbol. */
  const SymbolWithIndex& operator[](int i) const;

  /** The environment key. */
  int key; 
};

/** 
 * \brief Indexed symbol 
 *
 * \author Gilles Chabert
 * \date 2007
 */
class SymbolWithIndex : public ExtendedSymbol {

 public:  

  /** Create an equality constraint symbol=expr. */
  const Equality& operator=(const Expr& expr) const { return ((Expr&) *this)=expr; }

  /** Create an equality constraint symbol=value. */
  const Equality& operator=(const INTERVAL& value) const  { return ((Expr&) *this)=value; }

  /** Accept an #ibex::Expr visitor. */
  virtual void acceptVisitor(Visitor& v) const { v.visit(*this); };

  /** The index. */
  const int index;

  /** The indexed symbol. */
  const ExtendedSymbol& expr;

 private:
  friend const SymbolWithIndex& ExtendedSymbol::operator[](int i) const;

  /** Create an indexed expression. */  
  SymbolWithIndex(const ExtendedSymbol& expr, int index) : 
    ExtendedSymbol(expr.env, expr.depth+1, expr.size+1, index_dim(expr.dim), index_num(expr.dim, expr.key, index)), 
    index(index), expr(expr) { }
};

/** 
 * \brief Symbol
 *
 * An instance of this class represents a leaf in the syntax tree. This leaf
 * merely contains the symbol of an entity.
 * \author Gilles Chabert
 * \date 2007
 */
class Symbol : public ExtendedSymbol {

 public:
  /** Create an equality constraint symbol=expr. */
  const Equality& operator=(const Expr& expr) const { return ((Expr&) *this)=expr; }

  /** Create an equality constraint symbol=value. */
  const Equality& operator=(const INTERVAL& value) const  { return ((Expr&) *this)=value; }

  /** Create a symbol. */
  static const Symbol& new_(Env& env, const char* name, const Dim& dim, int key) { return *new Symbol(env,name,dim,key); }

  /** Accept an #ibex::Expr visitor. */
  virtual void acceptVisitor(Visitor& v) const { v.visit(*this); };

  /** Deletes this instance. */    
  ~Symbol() { free((char*) name); }

  /** The symbol identifier. */
  const char* name;

 private:
  /** Create a symbol. */
  Symbol(Env& env, const char* name, const Dim& dim, int key) : ExtendedSymbol(env, 0,1,dim,key), name(strdup(name)) { }

  /** Duplicate this symbol: forbidden. */
  Symbol(const Symbol&); 
};

/**
 * \brief Constant expression
 * \author Gilles Chabert
 * \date 2007
 */
class Constant : public Expr {

 public:
  /** Create a scalar constant. */
  static const Constant& new_scalar(Env& env, const INTERVAL& value) { return *new Constant(env,value); }

  /** Create a vector constant. */
  static const Constant& new_vector(Env& env, const INTERVAL_VECTOR& value) { return *new Constant(env,value); }

  /** Create a matrix constant. */
  static const Constant& new_matrix(Env& env, const INTERVAL_MATRIX& value) { return *new Constant(env,value); }

  /** Return a copy of a constant. */
  const Constant& copy() const { return *new Constant(*this); }

  /** Return true if this constant is either 0, the null vector or the null matrix. */
  virtual bool is_zero() const;

  /** Accept an #ibex::Expr visitor. */
  virtual void acceptVisitor(Visitor& v) const { v.visit(*this); };

  /** Return the value of the constant under the form of an #INTERVAL. 
   *  If the constant is a matrix, the first entry is returned (no error). */
  const INTERVAL& get_value() const { return value(1,1); }
  
  /** Return the value of the constant under the form of an INTERVAL_MATRIX.
   *  If the constant is not a matrix, the returned matrix is 1-row x 1-col. */
  const INTERVAL_MATRIX& get_matrix_value() const { return value; }

 private:
  friend class Visitor;

  Constant(Env& env, const INTERVAL& value);

  Constant(Env& env, const INTERVAL_VECTOR& value);

  Constant(Env& env, const INTERVAL_MATRIX& value);

  Constant(Env& env, const Constant& c);

  const INTERVAL_MATRIX value;
};

/**
 * \brief Unary expression
 * \author Gilles Chabert
 * \date March 2007
 *
 */ 
class UnaOpExpr : public Expr {

 public:
  /** Create an equality constraint unary-expr=expr. */
  const Equality& operator=(const Expr& expr) const { return ((Expr&) *this)=expr; } 

  /** Create an equality constraint unary-expr=value. */
  const Equality& operator=(const INTERVAL& value) const  { return ((Expr&) *this)=value; } 

  static const UnaOpExpr& new_(UnaOpType op, const Expr& expr) {
    return *new UnaOpExpr(op,expr);
  }

  /** Accept an #ibex::Expr visitor. */
  virtual void acceptVisitor(Visitor& v) const { v.visit(*this); };

  /** The subexpression */
  const Expr& expr;

  /** The unary operator internal code */
  const UnaOpType op;

 private:
  friend class Visitor;

  UnaOpExpr(UnaOpType op, const Expr& expr);

  UnaOpExpr(const UnaOpExpr&); // copy constructor forbidden  

  static Dim get_dim(UnaOpType op, const Dim& dim);
  static UnaOpType get_op(UnaOpType op, const Dim& dim);
};

/**
 * \brief Binary expression
 * \author Gilles Chabert
 * \date March 2007
 *
 */
class BinOpExpr : public Expr {

 public:
  /** Create an equality constraint binary-expr=expr. */
  const Equality& operator=(const Expr& expr) const { return ((Expr&) *this)=expr; }

  /** Create an equality constraint binary-expr=value. */
  const Equality& operator=(const INTERVAL& value) const  { return ((Expr&) *this)=value; }
  
  /** Accept an #ibex::Expr visitor. */
  virtual void acceptVisitor(Visitor& v) const { v.visit(*this); };

  static const BinOpExpr& new_(const Expr& left, BinOpType op, const Expr& right) {
    return *new BinOpExpr(left,op,right);
  }

  /** Left subexpression. */
  const Expr& left;

  /** Right subexpression. */
  const Expr& right;

  /** Binary operator internal code. */
  const BinOpType op;

 private:
  friend class Visitor;

  BinOpExpr(const Expr& left, BinOpType op, const Expr& right);
    
  BinOpExpr(const BinOpExpr&); // copy constructor forbidden  

  static Dim get_dim(const Dim& ldim, BinOpType op, const Dim& rdim);

  static BinOpType get_op(const Dim& ldim, BinOpType op, const Dim& rdim);
};

/**
 * \brief Power expression.
 * \author Gilles Chabert
 * \date March 2007
 *
 */
class PowerExpr : public Expr {

 public:
  const Equality& operator=(const Expr& expr) const { return ((Expr&) *this)=expr; }
  const Equality& operator=(const INTERVAL& value) const  { return ((Expr&) *this)=value; }

  static const PowerExpr& new_(const Expr& expr, int expon) {
    return *new PowerExpr(expr,expon);
  }

  /** Accept an #ibex::Expr visitor. */
  virtual void acceptVisitor(Visitor& v) const { v.visit(*this); };

  const Expr& expr;
  const int expon;

 private:
  friend const Expr& pow(const Expr&, int);
  friend class Visitor;

  PowerExpr(const Expr& expr, int expon);

  PowerExpr(const PowerExpr&); // copy constructor forbidden
};

/**
 * \brief Function application expression
 *
 * In the following Quimper example:
 * \code
 * Variables
 *   a in [0,10];
 *
 * function z=foo(x,y)
 *   z=sqrt(x^2+y^2);
 * end;
 * 
 * Constraints
 *   foo(a,1-a)=1
 * ...
 * \endcode
 * \a f(a,1-a) is an instance of Apply, where \link Apply::f f \endlink is "foo" and the
 * actual arguments arg(0) and arg(1) are the subexpressions \a a and \a 1-a.
 * \author Gilles Chabert
 * \date December 2007
 *
 */
class Apply : public Expr {

 public:

  /** Create an equality constraint apply=expr. */
  const Equality& operator=(const Expr& expr) const { return ((Expr&) *this)=expr; }

  /** Create an equality constraint apply=value. */
  const Equality& operator=(const INTERVAL& value) const  { return ((Expr&) *this)=value; }

  /** Deletes this instance. */
  ~Apply();
  
  /** Create a new function application. */
  static const Apply& new_(Env& env, const char* f, const vector<const Expr*>& arguments) {
    return *new Apply(env,f,arguments);
  }

  /** Return a reference to the ith argument expression. */
  const Expr& arg(int i) const { return *args[i]; } //return env.expression(args[i]); }

  /** Accept an #ibex::Expr visitor. */
  virtual void acceptVisitor(Visitor& v) const { v.visit(*this); };

  /** The name of the applied function. */
  const char* f;

  /** The number of arguments. */
  const int n;

  /** The actual arguments. */
  const Expr** args;
  
 private:
  static unsigned int max_depth(const vector<const Expr*>&);
  static Dim get_dim(Env& env, const char* f, const vector<const Expr*>& arguments);

  Apply(Env& env, const char* f, const vector<const Expr*>& arguments);
};

class Space;


/**
 * \brief Domain operator expression
 * \author Gilles Chabert
 * \date February 2008
 *
 */ 
class DomainOpExpr : public Expr {

 public:
  static const DomainOpExpr& new_(DomainOpType op, const Expr& expr) {
    return *new DomainOpExpr(op,expr);
  }

  /** Accept an #ibex::Expr visitor. */
  virtual void acceptVisitor(Visitor& v) const { v.visit(*this); };

  /** The subexpression */
  const Expr& expr;

  /** The unary operator internal code */
  const DomainOpType op;

 private:
  friend class Visitor;

  DomainOpExpr(DomainOpType op, const Expr& expr);

  DomainOpExpr(const DomainOpExpr&); // copy constructor forbidden  

  static DomainOpType get_op(DomainOpType op, const Dim& dim);
};


/** Addition */
inline const BinOpExpr& operator+(const Expr& left, const Expr& right) { 
  return BinOpExpr::new_(left, ADD, right); }

/** Subtraction */
inline const BinOpExpr& operator-(const Expr& left, const Expr& right) { 
  return BinOpExpr::new_(left, SUB, right); }

/** Multiplication */
inline const BinOpExpr& operator*(const Expr& left, const Expr& right) { 
  return BinOpExpr::new_(left, MUL, right); }

/** Division */
inline const BinOpExpr& operator/(const Expr& left, const Expr& right) { 
  return BinOpExpr::new_(left, DIV, right); }

/** Maximum */
 inline const BinOpExpr& max(const Expr& left, const Expr& right) { 
  return BinOpExpr::new_(left, MAX, right); }

/** Minimum */
inline const BinOpExpr& min(const Expr& left, const Expr& right) { 
  return BinOpExpr::new_(left, MIN, right); }

/** Addition of an expression to a constant */
inline const BinOpExpr& operator+(const Expr& left, const INTERVAL& value) { 
  return BinOpExpr::new_(left, ADD, Constant::new_scalar(left.env, value)); }

/** Subtraction of an expression from a constant */
inline const BinOpExpr& operator-(const Expr& left, const INTERVAL& value) { 
  return BinOpExpr::new_(left, SUB, Constant::new_scalar(left.env, value)); }

/** Multiplication of an expression by a constant */
inline const BinOpExpr& operator*(const Expr& left, const INTERVAL& value) { 
  return BinOpExpr::new_(left, MUL, Constant::new_scalar(left.env, value)); }

/** Division of an expression by a constant */
inline const BinOpExpr& operator/(const Expr& left, const INTERVAL& value) { 
  return BinOpExpr::new_(left, DIV, Constant::new_scalar(left.env, value)); }

/** Maximum of an expression and a constant */
 inline const BinOpExpr& max(const Expr& left, const INTERVAL& value) { 
   return BinOpExpr::new_(left, MAX, Constant::new_scalar(left.env, value)); }

/** Minimum of an expression and a constant */
 inline const BinOpExpr& min(const Expr& left, const INTERVAL& value) { 
   return BinOpExpr::new_(left, MIN, Constant::new_scalar(left.env, value)); }

/** Addition of a constant to an expression */
inline const BinOpExpr& operator+(const INTERVAL& value, const Expr& right) { 
  return BinOpExpr::new_(Constant::new_scalar(right.env, value), ADD, right); }

/** Subtraction of a constant from an expression */
inline const BinOpExpr& operator-(const INTERVAL& value, const Expr& right) { 
  return BinOpExpr::new_(Constant::new_scalar(right.env, value), SUB, right); }

/** Multiplication of a constant by an expression*/
inline const BinOpExpr& operator*(const INTERVAL& value, const Expr& right) { 
  return BinOpExpr::new_(Constant::new_scalar(right.env, value), MUL, right); }

/** Division of a constant by an expression */
inline const BinOpExpr& operator/(const INTERVAL& value, const Expr& right) { 
  return BinOpExpr::new_(Constant::new_scalar(right.env, value), DIV, right); }

/** Maximum of a constant and an expression */
inline const BinOpExpr& max (const INTERVAL& value, const Expr& right) { 
  return BinOpExpr::new_(Constant::new_scalar(right.env, value), MAX, right); }

/** Minimum of a constant and an expression */
inline const BinOpExpr& min (const INTERVAL& value, const Expr& right) { 
  return BinOpExpr::new_(Constant::new_scalar(right.env, value), MIN, right); }

/** Raises \a left to the power \a expnon.
    \note operator ^ is not used because its associativity and priority defined in C++ does
    not match mathematical usage. */
inline const Expr& pow(const Expr& left, int expon) { 
  if (expon==1) return left;
  if (expon==2) return UnaOpExpr::new_(SQR, left);
  return PowerExpr::new_(left, expon); 
}

/** Minus sign */
inline const UnaOpExpr& operator-(const Expr& expr) {
  return UnaOpExpr::new_(MINUS, expr);
}

/** Sign */
inline const UnaOpExpr& sign(const Expr& expr) {
  return UnaOpExpr::new_(SIGN, expr);
}

/** Absolute value */
inline const UnaOpExpr& abs(const Expr& expr) {
  return UnaOpExpr::new_(ABS, expr);
}

/** Expression raised to the power of an integer */
inline const UnaOpExpr& pow(const Expr& left, const Expr& right) { 
  return UnaOpExpr::new_(EXP, BinOpExpr::new_(right, MUL, UnaOpExpr::new_(LOG, left))); 
}

/** Expression raised to the power of a constant */
inline const UnaOpExpr& pow(const Expr& left, const INTERVAL& value) { 
  return UnaOpExpr::new_(EXP, BinOpExpr::new_(Constant::new_scalar(left.env, value), MUL, UnaOpExpr::new_(LOG, left))); 
}
/** Constant raised to the power of an expression */
inline const UnaOpExpr& pow(const INTERVAL& value, const Expr& right) { 
  return UnaOpExpr::new_(EXP, BinOpExpr::new_(right, MUL, UnaOpExpr::new_(LOG, Constant::new_scalar(right.env, value)))); 
}

/** Square of an expresion */
inline const UnaOpExpr& sqr    (const Expr& exp) { return UnaOpExpr::new_(SQR,    exp); }
/** Square root of an expresion */
inline const UnaOpExpr& sqrt   (const Expr& exp) { return UnaOpExpr::new_(SQRT,   exp); }
/** Exponential of an expression */
inline const UnaOpExpr& exp    (const Expr& exp) { return UnaOpExpr::new_(EXP,    exp); }
/** Neperian logarithm of an expression */
inline const UnaOpExpr& ln     (const Expr& exp) { return UnaOpExpr::new_(LOG,    exp); }
/** Cosine of an expression */
inline const UnaOpExpr& cos    (const Expr& exp) { return UnaOpExpr::new_(COS,    exp); }
/** Sine of an expression */
inline const UnaOpExpr& sin    (const Expr& exp) { return UnaOpExpr::new_(SIN,    exp); }
/** Tangent of an expression */
inline const UnaOpExpr& tan    (const Expr& exp) { return UnaOpExpr::new_(TAN,    exp); }
/** Arccosine of an expression */
inline const UnaOpExpr& arccos (const Expr& exp) { return UnaOpExpr::new_(ARCCOS, exp); }
/** Arcsine of an expression */
inline const UnaOpExpr& arcsin (const Expr& exp) { return UnaOpExpr::new_(ARCSIN, exp); }
/** Arctangent of an expression */
inline const UnaOpExpr& arctan (const Expr& exp) { return UnaOpExpr::new_(ARCTAN, exp); }
/** Arctangent2 of two expressions */
inline const BinOpExpr& arctan2(const Expr& exp1, const Expr& exp2) { return BinOpExpr::new_(exp1, ARCTAN2, exp2); }
/** Hyperbolic cosine of an expression */
inline const UnaOpExpr& cosh   (const Expr& exp) { return UnaOpExpr::new_(COSH,   exp); }
/** Hyperbolic sine of an expression */
inline const UnaOpExpr& sinh   (const Expr& exp) { return UnaOpExpr::new_(SINH,   exp); }
/** Hyperbolic tangent of an expression */
inline const UnaOpExpr& tanh   (const Expr& exp) { return UnaOpExpr::new_(TANH,   exp); }
/** Inverse hyperbolic cosine of an expression */
inline const UnaOpExpr& arccosh(const Expr& exp) { return UnaOpExpr::new_(ARCCOSH,exp); }
/** Inverse hyperbolic sine of an expression */
inline const UnaOpExpr& arcsinh(const Expr& exp) { return UnaOpExpr::new_(ARCSINH,exp); }
/** Inverse hyperbolic tangent of an expression */
inline const UnaOpExpr& arctanh(const Expr& exp) { return UnaOpExpr::new_(ARCTANH,exp); }
/** Transpose of a matrix expression */
inline const UnaOpExpr& transpose(const Expr& exp) { return UnaOpExpr::new_(M_TRANS,exp); }
/** Infimum */
inline const DomainOpExpr& inf(const Expr& expr) { return DomainOpExpr::new_(INF, expr); }
/** Midpoint bound */
inline const DomainOpExpr& mid(const Expr& expr) { return DomainOpExpr::new_(MID, expr); }
/** Supremum */
inline const DomainOpExpr& sup(const Expr& expr) { return DomainOpExpr::new_(SUP, expr); }

/*@}*/ // end exprgroup

} // end namespace ibex

#endif
