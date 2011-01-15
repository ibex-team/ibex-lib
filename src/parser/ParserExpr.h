/*---------------------------------------------------------------------------------
 * 
 * Parser-level expressions
 * ------------------------------
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
#ifndef __PARSER_EXPR
#define __PARSER_EXPR

#include "Interval.h"
#include "IntervalVector.h"
#include "IntervalMatrix.h"
#include "IbexToken.h"
#include <vector>
#include <string.h>
#include <stdlib.h>

namespace ibex {

namespace parser {

class P_Expr;
class P_Symbol;
class P_Constant;
class P_UnaOpExpr;
class P_BinOpExpr;
class P_PowerExpr;
class P_Apply;
class P_DomainOpExpr;
class P_IterExpr;
class P_IterOp;         // operation between P_IterExpr (+ * -)
class P_IterCst;        // integer constant
class P_IterCstSymbolWithIndex;  // constant symbol with iterator-dependent subscript
class P_IterSymbol;     // iterator symbol, e.g.: i
class P_SymboWithIterIndex; // e.g.: x[i+1]

class P_ExprVisitor {
 public:
  virtual void visit(const P_Expr&)       =0;
  virtual void visit(const P_Symbol&)     =0;
  virtual void visit(const P_IterExpr&)=0;
  virtual void visit(const P_IterOp&)=0;
  virtual void visit(const P_IterCst&)=0;
  virtual void visit(const P_IterCstSymbolWithIndex&)=0;
  virtual void visit(const P_IterSymbol&)=0;
  virtual void visit(const P_SymboWithIterIndex&)=0;
  virtual void visit(const P_Constant&)   =0;
  virtual void visit(const P_UnaOpExpr&)  =0;
  virtual void visit(const P_BinOpExpr&)  =0;
  virtual void visit(const P_PowerExpr&)  =0;
  virtual void visit(const P_Apply&)      =0;
  virtual void visit(const P_DomainOpExpr&)=0;
};

class P_Equality;
class P_Inequality;

/* 
 * \brief Parser expression base class
 *
 * \author Gilles Chabert
 * \date March 2007
 *
 * Example of <i>arithmetical expression</i>: sin(x+y)^2*3.14.
 */
class P_Expr { 

 public:  
  P_Expr();

  /* Accepts an #P_Expr visitor. */
  virtual void acceptVisitor(P_ExprVisitor& v) const = 0;

  virtual ~P_Expr() { }

  friend class P_ExprVisitor;

  /* Streams out this expression. */
  friend std::ostream& operator<<(std::ostream&, const P_Expr&);

  /* Returns true if this expression is the constant 0. */
  virtual bool is_zero() const { return false; }

  /* Creates an equality constraint expr=expr. */
  const P_Equality& operator=(const P_Expr& right) const;
  
  /* Creates an equality constraint expr=value. */
  const P_Equality& operator=(const P_Constant& value) const;
  
  /* Creates an inequality constraint expr<=expr. */
  const P_Inequality& operator<=(const P_Expr& right) const;
  
  /* Creates an inequality constraint expr<=value. */
  const P_Inequality& operator<=(const P_Constant& value) const;
  
  /* Creates an inequality constraint expr>=expr. */
  const P_Inequality& operator>=(const P_Expr& right) const;
  
  /* Creates an inequality constraint expr>=value. */
  const P_Inequality& operator>=(const P_Constant& value) const;
  
  /* Creates an inequality constraint expr<value. */
  const P_Inequality& operator<(const P_Expr& right) const;
  
  /* Creates an inequality constraint expr<value. */
  const P_Inequality& operator<(const P_Constant& value) const;
  
  /* Creates an inequality constraint expr>expr. */
  const P_Inequality& operator>(const P_Expr& right) const;
  
  /* Creates an inequality constraint expr>value. */
  const P_Inequality& operator>(const P_Constant& value) const;

  int line;
};

typedef enum { _int_, _real_, _itv_, _int_vec_, _real_vec_, _itv_vec_, _itv_mat_ } csttype;

/* Integer, real and interval constants at parse time. 
 *
 * remark: private inheritance to avoid conflits between the
 * operator= between 2 constants and 2 expressions.
*/
class P_Constant : private P_Expr {
 public:
  P_Constant(int);
  P_Constant(REAL);
  P_Constant(const INTERVAL&);
  P_Constant(const INTERVAL_VECTOR&);
  P_Constant(const INTERVAL_MATRIX&);
  P_Constant(const vector<P_Constant>&);

  P_Constant& operator=(const P_Constant& c);

  ~P_Constant();

  P_Constant operator[](int i) const;

  const P_Expr& _2expr() const { return *this; }

  bool scalar() const;

  virtual bool is_zero() const;

  virtual void acceptVisitor(P_ExprVisitor& v) const { v.visit(*this); };

  /* Conversions */
  int _2int() const;
  REAL _2real() const;
  INTERVAL _2itv() const;
  INTERVAL_VECTOR _2itvv() const;
  INTERVAL_MATRIX _2itvM() const;

  csttype type;
  int n;
  REAL r;
  INTERVAL x;
  INTERVAL_VECTOR v;
  INTERVAL_MATRIX M;

  /*  int* intv;
  REAL* realv;
  int size; */
};

P_Constant operator+(const P_Constant& c1, const P_Constant& c2);
P_Constant operator-(const P_Constant& c1, const P_Constant& c2);
P_Constant operator-(const P_Constant& c1);
P_Constant sign(const P_Constant& c1);
P_Constant operator*(const P_Constant& c1, const P_Constant& c2);
P_Constant operator/(const P_Constant& c1, const P_Constant& c2);
P_Constant transpose(const P_Constant& c);
P_Constant max(const P_Constant& c1, const P_Constant& c2);
P_Constant min(const P_Constant& c1, const P_Constant& c2);
P_Constant inf(const P_Constant& c);
P_Constant mid(const P_Constant& c);
P_Constant sup(const P_Constant& c);
P_Constant pow(const P_Constant& c, const P_Constant& p);
P_Constant abs(const P_Constant& c);
P_Constant sqrt(const P_Constant& c);
P_Constant exp(const P_Constant& c);
P_Constant log(const P_Constant& c);
P_Constant cos(const P_Constant& c);
P_Constant sin(const P_Constant& c);
P_Constant tan(const P_Constant& c);
P_Constant arccos(const P_Constant& c);
P_Constant arcsin(const P_Constant& c);
P_Constant arctan(const P_Constant& c);
P_Constant arctan2(const P_Constant& c1, const P_Constant& c2);
P_Constant cosh(const P_Constant& c);
P_Constant sinh(const P_Constant& c);
P_Constant tanh(const P_Constant& c);
P_Constant arccosh(const P_Constant& c);
P_Constant arcsinh(const P_Constant& c);
P_Constant arctanh(const P_Constant& c);

/* 
 * \brief Extended symbol (symbol with subscripts)
 *
 * \author Gilles Chabert
 * \date 2007
 */
class P_ExtendedSymbol : public P_Expr {

 public:
  const P_Equality& operator=(const P_Expr& expr) const { return ((P_Expr&) *this)=expr; }
  const P_Equality& operator=(const P_Constant& value) const  { return ((P_Expr&) *this)=value; }

  /* Accepts an #P_Expr visitor. */
  virtual void acceptVisitor(P_ExprVisitor& v) const=0;

  /* Returns the base name (without subscripts) */
  virtual const char* base_name() const=0;
};


/* Expression involving iterators and integer constants */
class P_IterExpr : public P_Expr {
 public:
  virtual void acceptVisitor(P_ExprVisitor& v) const { v.visit(*this); }
}; 

/* Binary operation (inside an expression involving iterators) */
class P_IterOp : public P_IterExpr {
public:
  P_IterOp(P_IterExpr& left, BinOpType op, P_IterExpr& right) : left(left), op(op), right(right) {}

  ~P_IterOp() { delete &left; delete &right; }

  virtual void acceptVisitor(P_ExprVisitor& v) const { v.visit(*this); }

  P_IterExpr& left;
  BinOpType op;
  P_IterExpr& right;
}; 

/* Integer constant (inside an expession involving iterators) */
class P_IterCst : public P_IterExpr {
public:
  P_IterCst(int value) : value(value) {}

  virtual void acceptVisitor(P_ExprVisitor& v) const { v.visit(*this); }
  
  int value;
}; 

/* Iterator symbol (inside an expession involving iterators) */
class P_IterSymbol : public P_IterExpr {
public:
  P_IterSymbol(const char* name) : name(strdup(name)) { }

  ~P_IterSymbol() { free(name); }

  virtual void acceptVisitor(P_ExprVisitor& v) const { v.visit(*this); }

  char* name;
}; 

/* Used when a constant array is indexed by an expression involving iterators */
class P_IterCstSymbolWithIndex : public P_IterExpr {
public:
  P_IterCstSymbolWithIndex(const P_Constant& cst, P_IterExpr& subscript) : cst(cst), subscript(subscript) { }

  ~P_IterCstSymbolWithIndex() { delete &subscript; }

  virtual void acceptVisitor(P_ExprVisitor& v) const { v.visit(*this); }

  P_Constant cst;
  P_IterExpr& subscript;
}; 


inline P_IterOp& operator+(P_IterExpr& l, P_IterExpr& r) { return *new P_IterOp(l, ADD, r); }
inline P_IterOp& operator-(P_IterExpr& l, P_IterExpr& r) { return *new P_IterOp(l, SUB, r); }
inline P_IterOp& operator*(P_IterExpr& l, P_IterExpr& r) { return *new P_IterOp(l, MUL, r); }
inline P_IterOp& operator+(P_IterExpr& l, int r) { return *new P_IterOp(l, ADD, *new P_IterCst(r)); }
inline P_IterOp& operator-(P_IterExpr& l, int r) { return *new P_IterOp(l, SUB, *new P_IterCst(r)); }
inline P_IterOp& operator*(P_IterExpr& l, int r) { return *new P_IterOp(l, MUL, *new P_IterCst(r)); }
inline P_IterOp& operator+(int l, P_IterExpr& r) { return *new P_IterOp(*new P_IterCst(l), ADD, r); }
inline P_IterOp& operator-(int l, P_IterExpr& r) { return *new P_IterOp(*new P_IterCst(l), SUB, r); }
inline P_IterOp& operator*(int l, P_IterExpr& r) { return *new P_IterOp(*new P_IterCst(l), MUL, r); }

/* A symbol with a subscript (potentially involving iterators) */
class P_SymboWithIterIndex : public P_ExtendedSymbol {
 public:
  P_SymboWithIterIndex(const P_ExtendedSymbol& symbol, P_IterExpr& iter) : symbol(symbol), iter(iter) { }

  ~P_SymboWithIterIndex() { delete &iter; delete &symbol; }

  /* Accepts an #P_Expr visitor. */
  virtual void acceptVisitor(P_ExprVisitor& v) const { v.visit(*this); }

  /* Returns the base name (without subscripts) */
  virtual const char* base_name() const { return symbol.base_name(); }

  const P_ExtendedSymbol& symbol;
  P_IterExpr& iter;
};

/* 
 * \brief Symbol
 *
 * An instance of this class represents a leaf in the syntax tree. This leaf
 * merely contains the symbol of an entity.
 * \author Gilles Chabert
 * \date 2007
 */
class P_Symbol : public P_ExtendedSymbol {

 public:
  const P_Equality& operator=(const P_Expr& expr) const { return ((P_Expr&) *this)=expr; }
  const P_Equality& operator=(const P_Constant& value) const  { return ((P_Expr&) *this)=value; }

  /* Creates a symbol. */
  P_Symbol(const char* name) : name(strdup(name)) { }

  /* Accepts an #P_Expr visitor. */
  virtual void acceptVisitor(P_ExprVisitor& v) const { v.visit(*this); };

  /* Returns the base name (without subscripts) */
  virtual const char* base_name() const { return name; }

  /* Deletes this instance. */    
  ~P_Symbol() { free((char*) name); }

  /* The symbol identifier. */
  const char* name;

 private:

  /* Duplicates this symbol: forbidden. */
  P_Symbol(const P_Symbol&); 
};

/*
 * \brief Unary expression
 * \author Gilles Chabert
 * \date March 2007
 *
 */ 
class P_UnaOpExpr : public P_Expr {

 public:
  const P_Equality& operator=(const P_Expr& expr) const { return ((P_Expr&) *this)=expr; } 
  const P_Equality& operator=(const P_Constant& value) const  { return ((P_Expr&) *this)=value; } 

  ~P_UnaOpExpr() { delete &expr; }

  static const P_UnaOpExpr& new_(UnaOpType op, const P_Expr& expr) {
    return *new P_UnaOpExpr(op,expr);
  }

  /* Accepts an #P_Expr visitor. */
  virtual void acceptVisitor(P_ExprVisitor& v) const { v.visit(*this); };

  const UnaOpType op;
  const P_Expr& expr;

 private:
  P_UnaOpExpr(UnaOpType op, const P_Expr& expr) : op(op), expr(expr) { }

  P_UnaOpExpr(const P_UnaOpExpr&); // copy constructor forbidden  
};

/* Minus sign */
inline const P_UnaOpExpr& operator-(const P_Expr& expr) { return P_UnaOpExpr::new_(MINUS, expr); }
/* Sign function */
inline const P_UnaOpExpr& sign   (const P_Expr& expr) { return P_UnaOpExpr::new_(SIGN, expr); }
/* Absolute value of an expression */
inline const P_UnaOpExpr& abs   (const P_Expr& expr) { return P_UnaOpExpr::new_(ABS, expr); }
/* Square of an expresion */
inline const P_UnaOpExpr& sqr    (const P_Expr& exp) { return P_UnaOpExpr::new_(SQR,    exp); }
/* Square root of an expresion */
inline const P_UnaOpExpr& sqrt   (const P_Expr& exp) { return P_UnaOpExpr::new_(SQRT,   exp); }
/* Exponential of an expression */
inline const P_UnaOpExpr& exp    (const P_Expr& exp) { return P_UnaOpExpr::new_(EXP,    exp); }
/* Neperian logarithm of an expression */
inline const P_UnaOpExpr& ln     (const P_Expr& exp) { return P_UnaOpExpr::new_(LOG,    exp); }
/* Cosine of an expression */
inline const P_UnaOpExpr& cos    (const P_Expr& exp) { return P_UnaOpExpr::new_(COS,    exp); }
/* Sine of an expression */
inline const P_UnaOpExpr& sin    (const P_Expr& exp) { return P_UnaOpExpr::new_(SIN,    exp); }
/* Tangent of an expression */
inline const P_UnaOpExpr& tan    (const P_Expr& exp) { return P_UnaOpExpr::new_(TAN,    exp); }
/* Arccosine of an expression */
inline const P_UnaOpExpr& arccos (const P_Expr& exp) { return P_UnaOpExpr::new_(ARCCOS, exp); }
/* Arcsine of an expression */
inline const P_UnaOpExpr& arcsin (const P_Expr& exp) { return P_UnaOpExpr::new_(ARCSIN, exp); }
/* Arctangent of an expression */
inline const P_UnaOpExpr& arctan (const P_Expr& exp) { return P_UnaOpExpr::new_(ARCTAN, exp); }
/* Hyperbolic cosine of an expression */
inline const P_UnaOpExpr& cosh   (const P_Expr& exp) { return P_UnaOpExpr::new_(COSH,   exp); }
/* Hyperbolic sine of an expression */
inline const P_UnaOpExpr& sinh   (const P_Expr& exp) { return P_UnaOpExpr::new_(SINH,   exp); }
/* Hyperbolic tangent of an expression */
inline const P_UnaOpExpr& tanh   (const P_Expr& exp) { return P_UnaOpExpr::new_(TANH,   exp); }
/* Inverse hyperbolic cosine of an expression */
inline const P_UnaOpExpr& arccosh(const P_Expr& exp) { return P_UnaOpExpr::new_(ARCCOSH,exp); }
/* Inverse hyperbolic sine of an expression */
inline const P_UnaOpExpr& arcsinh(const P_Expr& exp) { return P_UnaOpExpr::new_(ARCSINH,exp); }
/* Inverse hyperbolic tangent of an expression */
inline const P_UnaOpExpr& arctanh(const P_Expr& exp) { return P_UnaOpExpr::new_(ARCTANH,exp); }
/* Transpose of a matrix expression */
inline const P_UnaOpExpr& transpose(const P_Expr& exp) { return P_UnaOpExpr::new_(M_TRANS,exp); }

/*
 * \brief Binary expression.
 * \author Gilles Chabert
 * \date March 2007
 *
 */
class P_BinOpExpr : public P_Expr {

 public:
  const P_Equality& operator=(const P_Expr& expr) const { return ((P_Expr&) *this)=expr; }
  const P_Equality& operator=(const P_Constant& value) const  { return ((P_Expr&) *this)=value; }
  
  ~P_BinOpExpr() { delete &left; delete &right; }

  /* Accepts an #P_Expr visitor. */
  virtual void acceptVisitor(P_ExprVisitor& v) const { v.visit(*this); };

  static const P_BinOpExpr& new_(const P_Expr& left, BinOpType op, const P_Expr& right) {
    return *new P_BinOpExpr(left,op,right);
  }

  const P_Expr& left;
  const BinOpType op;
  const P_Expr& right;

 private:
  P_BinOpExpr(const P_Expr& left, BinOpType op, const P_Expr& right) : left(left), op(op), right(right) { }
    
  P_BinOpExpr(const P_BinOpExpr&); // copy constructor forbidden  

};

/* Addition */
inline const P_BinOpExpr& operator+(const P_Expr& left, const P_Expr& right) { 
  return P_BinOpExpr::new_(left, ADD, right); }

/* Subtraction */
inline const P_BinOpExpr& operator-(const P_Expr& left, const P_Expr& right) { 
  return P_BinOpExpr::new_(left, SUB, right); }

/* Multiplication */
inline const P_BinOpExpr& operator*(const P_Expr& left, const P_Expr& right) { 
  return P_BinOpExpr::new_(left, MUL, right); }

/* Division */
inline const P_BinOpExpr& operator/(const P_Expr& left, const P_Expr& right) { 
  return P_BinOpExpr::new_(left, DIV, right); }

/* Minimum */
inline const P_BinOpExpr& max(const P_Expr& left, const P_Expr& right) { 
  return P_BinOpExpr::new_(left, MAX, right); }

/* Maximum */
inline const P_BinOpExpr& min(const P_Expr& left, const P_Expr& right) { 
  return P_BinOpExpr::new_(left, MIN, right); }

/* Arctangent2 of two expressions */
inline const P_BinOpExpr& arctan2 (const P_Expr& exp1, const P_Expr& exp2) { 
  return P_BinOpExpr::new_(exp1, ARCTAN2, exp2); }

/* Addition of an expression to a constant */
inline const P_BinOpExpr& operator+(const P_Expr& left, const P_Constant& value) { 
  return P_BinOpExpr::new_(left, ADD, value._2expr()); }

/* Subtraction of an expression from a constant */
inline const P_BinOpExpr& operator-(const P_Expr& left, const P_Constant& value) { 
  return P_BinOpExpr::new_(left, SUB, value._2expr()); }

/* Multiplication of an expression by a constant */
inline const P_BinOpExpr& operator*(const P_Expr& left, const P_Constant& value) { 
  return P_BinOpExpr::new_(left, MUL, value._2expr()); }

/* Division of an expression by a constant */
inline const P_BinOpExpr& operator/(const P_Expr& left, const P_Constant& value) { 
  return P_BinOpExpr::new_(left, DIV, value._2expr()); }

/* Maximum of an expression and a constant */
inline const P_BinOpExpr& max(const P_Expr& left, const P_Constant& value) { 
  return P_BinOpExpr::new_(left, MAX, value._2expr()); }

/* Minimum of an expression and a constant */
inline const P_BinOpExpr& min(const P_Expr& left, const P_Constant& value) { 
  return P_BinOpExpr::new_(left, MIN, value._2expr()); }

/* Arctangent2 of an expression and a constant */
inline const P_BinOpExpr& arctan2 (const P_Expr& exp1, const P_Constant& value) { 
  return P_BinOpExpr::new_(exp1, ARCTAN2, value._2expr()); }

/* Addition of a constant to an expression */
inline const P_BinOpExpr& operator+(const P_Constant& value, const P_Expr& right) { 
  return P_BinOpExpr::new_(value._2expr(), ADD, right); }

/* Subtraction of a constant from an expression */
inline const P_BinOpExpr& operator-(const P_Constant& value, const P_Expr& right) { 
  return P_BinOpExpr::new_(value._2expr(), SUB, right); }

/* Multiplication of a constant by an expression*/
inline const P_BinOpExpr& operator*(const P_Constant& value, const P_Expr& right) { 
  return P_BinOpExpr::new_(value._2expr(), MUL, right); }

/* Division of a constant by an expression */
inline const P_BinOpExpr& operator/(const P_Constant& value, const P_Expr& right) { 
  return P_BinOpExpr::new_(value._2expr(), DIV, right); }

/* Maximum of a constant and an expression */
inline const P_BinOpExpr& max(const P_Constant& value, const P_Expr& right) { 
  return P_BinOpExpr::new_(value._2expr(), MAX, right); }

/* Minimum of a constant and an expression */
inline const P_BinOpExpr& min(const P_Constant& value, const P_Expr& right) { 
  return P_BinOpExpr::new_(value._2expr(), MIN, right); }

/* Arctangent2 of a constant and an expression */
inline const P_BinOpExpr& arctan2 (const P_Constant& value, const P_Expr& exp2) { 
  return P_BinOpExpr::new_(value._2expr(), ARCTAN2, exp2); }

/* Minimum of a list of expressions */
const P_BinOpExpr& min(vector<const P_Expr*>& args);

/* Maximum of a list of expressions */
const P_BinOpExpr& max(vector<const P_Expr*>& args);

/*
 * \brief Power expression.
 * \author Gilles Chabert
 * \date March 2007
 *
 */
class P_PowerExpr : public P_Expr {

 public:
  const P_Equality& operator=(const P_Expr& expr) const { return ((P_Expr&) *this)=expr; }
  const P_Equality& operator=(const P_Constant& value) const  { return ((P_Expr&) *this)=value._2expr(); }

  static const P_PowerExpr& new_(const P_Expr& expr, int expon) {
    return *new P_PowerExpr(expr, expon);
  }

  ~P_PowerExpr() { delete &expr; }

  /* Accepts an #P_Expr visitor. */
  virtual void acceptVisitor(P_ExprVisitor& v) const { v.visit(*this); };

  const P_Expr& expr;
  const int expon;

 private:
  P_PowerExpr(const P_Expr& expr, int expon) : expr(expr), expon(expon) { }

  P_PowerExpr(const P_PowerExpr&); // copy constructor forbidden
};

/* Power 
    \note operator ^ is not used because its associativity and priority defined in C++ does
    not match mathematical usage. */
inline const P_Expr& pow(const P_Expr& left, int expon) { 
  if (expon==1) return left;
  if (expon==2) return P_UnaOpExpr::new_(SQR, left);
  return P_PowerExpr::new_(left, expon); 
}

/* P_Expression raised to the power of an integer */
inline const P_UnaOpExpr& pow(const P_Expr& left, const P_Expr& right) { 
  return P_UnaOpExpr::new_(EXP, P_BinOpExpr::new_(right, 
							     MUL, P_UnaOpExpr::new_(LOG, left))); 
}

/* P_Expression raised to the power of a constant */
inline const P_UnaOpExpr& pow(const P_Expr& left, const P_Constant& value) { 
  return P_UnaOpExpr::new_(EXP, P_BinOpExpr::new_(value._2expr(), MUL, P_UnaOpExpr::new_(LOG, left))); 
}
/* P_Constant raised to the power of an expression */
inline const P_UnaOpExpr& pow(const P_Constant& value, const P_Expr& right) { 
  return P_UnaOpExpr::new_(EXP, P_BinOpExpr::new_(right, MUL, P_UnaOpExpr::new_(LOG, value._2expr()))); 
}


/*
 * \brief Function application expression
 * \author Gilles Chabert
 * \date December 2007
 *
 */
class P_Apply : public P_Expr {

 public:
  const P_Equality& operator=(const P_Expr& expr) const { return ((P_Expr&) *this)=expr; }
  const P_Equality& operator=(const P_Constant& value) const  { return ((P_Expr&) *this)=value._2expr(); }

  ~P_Apply() {
    free((char*) f);
    for (vector<const P_Expr*>::const_iterator it=args->begin(); it!=args->end(); it++) 
      delete *it;
    delete args;
  }
  
  /* Creates a new function application. */
  P_Apply(const char* f, vector<const P_Expr*>* arguments) : f(strdup(f)), args(arguments) { }

  /* Accepts an #P_Expr visitor. */
  virtual void acceptVisitor(P_ExprVisitor& v) const { v.visit(*this); };

  /* The name of the applied function. */
  const char* f;

  vector<const P_Expr*>* args;
  
 private:
};

/*
 * \brief Domain operation (inf,mid,sup)
 * \author Gilles Chabert
 * \date March 2007
 *
 */ 
class P_DomainOpExpr : public P_Expr {

 public:
  const P_Equality& operator=(const P_Expr& expr) const { return ((P_Expr&) *this)=expr; } 
  const P_Equality& operator=(const P_Constant& value) const  { return ((P_Expr&) *this)=value; } 

  ~P_DomainOpExpr() { delete &expr; }

  static const P_DomainOpExpr& new_(DomainOpType op, const P_Expr& expr) {
    return *new P_DomainOpExpr(op,expr);
  }

  /* Accepts an #P_Expr visitor. */
  virtual void acceptVisitor(P_ExprVisitor& v) const { v.visit(*this); };

  const DomainOpType op;
  const P_Expr& expr;

 private:
  P_DomainOpExpr(DomainOpType op, const P_Expr& expr) : op(op), expr(expr) { }

  P_DomainOpExpr(const P_DomainOpExpr&); // copy constructor forbidden  
};

/* Inf */
inline const P_DomainOpExpr& inf   (const P_Expr& expr) { return P_DomainOpExpr::new_(INF, expr); }
/* Mid */
inline const P_DomainOpExpr& mid   (const P_Expr& expr) { return P_DomainOpExpr::new_(MID, expr); }
/* Sup */
inline const P_DomainOpExpr& sup   (const P_Expr& expr) { return P_DomainOpExpr::new_(SUP, expr); }


/*@}*/

} // end namespace ibex

} // end namespace

#endif
