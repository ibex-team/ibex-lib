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

#include <iostream>
#include <limits.h>
#include "IbexExpr.h"
#include "IbexExprPrinter.h"
#include "IbexEnv.h"
#include "IbexFunction.h"

namespace ibex {

/*================================================================================*/


Expr::Expr(Env& env, int depth, int size, const Dim& dim) : 
  env(env), depth(depth), size(size), id(env.nb_expressions()), dim(dim) { 
  if (env.nb_expressions()==INT_MAX) throw NonRecoverableException("Expression length exceeds "+ UINT_MAX);
  env.add_expression(*this);
}

std::ostream& operator<<(std::ostream& os, const Expr& expr) {
  ExprPrinter(os).visit(expr);
  return os;
}

/*================================================================================*/

const SymbolWithIndex& ExtendedSymbol::operator[](int i) const {
  return *new SymbolWithIndex(*this,i);
}

/** Create an indexed expression. */  
// SymbolWithIndex::SymbolWithIndex(const ExtendedSymbol& expr, int index) : ExtendedSymbol(expr.depth+1, expr.size+1, 
// 									     index_info(expr.dim, expr.num, index).first,
// 									     index_info(expr.dim, expr.num, index).second),
// 							      expr(expr), index(index) { }

/*================================================================================*/

Constant::Constant(Env& env, const INTERVAL& v) : Expr(env, 0,1,Dim(0,0,0)), value(1,1) { 
  value(1,1)=v; 
}

Constant::Constant(Env& env, const INTERVAL_VECTOR& v) : Expr(env, 0,1,Dim(0,0,Dimension(v))), value(1,Dimension(v)) { 
  SetRow((INTERVAL_MATRIX&) value,1,v); 
}

Constant::Constant(Env& env, const INTERVAL_MATRIX& v) : Expr(env, 0,1,Dim(0,RowDimension(v),ColDimension(v))), value(v) { }

Constant::Constant(Env& env, const Constant& c) : Expr(env, 0,1,dim), value(c.value) { }

bool Constant::is_zero() const { 
  INTERVAL_MATRIX zero(dim.size2(),dim.size3());
  Initialize(zero,0.);
  return (value==zero); //Inf(value)==0 && Sup(value)==0); 
}

/*================================================================================*/

Dim UnaOpExpr::get_dim(UnaOpType op, const Dim& dim) {
  if (dim.dim1>0) throw NonRecoverableException("Cannot use three-dimensional symbols into expressions");
  if (op==M_TRANS) 
    return Dim(0,dim.dim3,dim.dim2);
  if (op!=MINUS && op!=M_MINUS && op!=M_TRANS && !dim.scalar()) 
    throw NonRecoverableException("Cannot use built-in functions with a vector or matrix expression");
  return dim;
}

UnaOpType UnaOpExpr::get_op(UnaOpType op, const Dim& dim) {
  if (op==M_MINUS && dim.scalar()) return MINUS;
  if (op==MINUS && !dim.scalar()) return M_MINUS;
  else return op;
}

UnaOpExpr::UnaOpExpr(UnaOpType op, const Expr& expr) : Expr(expr.env, expr.depth+1,expr.size+1,get_dim(op,expr.dim)), expr(expr), op(op) { 
}

/*================================================================================*/

Dim BinOpExpr::get_dim(const Dim& l, BinOpType op, const Dim& r) {
  if (l.size1()>1 || r.size1()>1) throw NonRecoverableException("Cannot use three-dimensional symbols into expressions");
  switch (op) {
  case SUB : 
  case ADD : 
  case M_SUB : 
  case M_ADD : 
    if (l.size2()!=r.size2() || l.size3()!=r.size3()) throw NonRecoverableException("Cannot add matrices with different dimensions");
    
    // dimension is 0 is "abosrbing". E.g.: if x is a vector of 1 element and y a scalar, x+y is scalar.
    // but if y is also a vector of 1 element, x+y is a vector of 1 element. this is what the following line computes.
    return Dim(0, r.dim2==0? 0:l.dim2, r.dim3==0? 0:l.dim3);
    break; 
  case MUL : 
  case M_MUL : 
    if (l.scalar()) // scalar multiplication.
      return r;
    else {
      if (r.dim2==0)  // matrix-vector multiplication
	if (l.dim2==0)
	  return Dim(0,0,0); // dot product
      //	  throw NonRecoverableException("Dot product not yet implemented");      
	else if (l.dim3!=r.dim3) 
	  throw NonRecoverableException("Vector dimension does not match the number of matrix columns in matrix-vector multiplication");      
	else return Dim(0,0,l.dim2);
      else 
	if (l.dim3!=r.dim2) throw NonRecoverableException("Mistmaching dimensions in matrix multiplication");
	else return Dim(0,l.dim2,r.dim3);
    }
    break;     	        
  default :  // DIV, MAX, MIN
    if (!l.scalar() || !r.scalar()) throw NonRecoverableException("Cannot use \"max\", \"min\" or division with matrix operands");
    return l;
  }
}

BinOpType BinOpExpr::get_op(const Dim& l, BinOpType op, const Dim& r) {

  switch (op) {
  case SUB : 
  case M_SUB : 
    if (l.scalar()) return SUB; 
    else return M_SUB;    
  case ADD : 
  case M_ADD : 
    if (l.scalar()) return ADD;
    else return M_ADD;    
    break; 
  case MUL : 
  case M_MUL : 
    if (l.scalar())
      if (r.scalar()) return MUL;  // scalar multiplication.
      else return M_SCAL;          // scalar-matrix multiplication.
    else 
      if (r.dim2==0) 
	if (l.dim2==0) return V_DOT; // vector dot product
	else return M_VEC;           // matrix-vector multiplication
      else return M_MUL;             // matrix multiplication
  default : // DIV, MAX, MIN
    return op;
  }
}

class SizeComputer : public Visitor {
public:

  /* for binary expressions (BinOpExpr). */
  SizeComputer(const Expr& l, const Expr& r) : size(0) {
    visit(l);
    visit(r);
  }

  /* for n-ary expressions (Apply). */
  SizeComputer(vector<const Expr*> args) : size(0) {
    for (vector<const Expr*>::const_iterator it=args.begin(); it!=args.end(); it++) 
      visit(**it);
  }
  
  int size;

private:
  void visit(const Expr& e) { 
    if (visited.find(e.id)==visited.end()) {
      visited.insert(e.id);
      size++;
      e.acceptVisitor(*this); 
    }
  }
  void visit(const SymbolWithIndex& c) { visit(c.expr); }
  void visit(const Symbol&) { }
  void visit(const Constant& cst) { }
  void visit(const UnaOpExpr& u) { visit(u.expr); }
  void visit(const BinOpExpr& b) { visit(b.left); visit(b.right); }
  void visit(const PowerExpr& p) { visit(p.expr); }
  void visit(const Apply& a) { for (int i=0; i<a.n; i++) visit(a.arg(i)); }
  void visit(const DomainOpExpr& d) { visit(d.expr); }

  set<int> visited;
};

BinOpExpr::BinOpExpr(const Expr& left, BinOpType op, const Expr& right) : 
  Expr(left.env, (left.depth > right.depth ? left.depth : right.depth)+1, SizeComputer(left,right).size+1, get_dim(left.dim,op,right.dim)), 
  left(left), right(right), op(get_op(left.dim,op,right.dim)) { 
}

/*================================================================================*/

PowerExpr::PowerExpr(const Expr& expr, int expon) : Expr(expr.env, expr.depth+1, expr.size+1, Dim(0,0,0)), expr(expr), expon(expon) { 
  if (!expr.scalar()) // not yet implemented
    throw NonRecoverableException("Exponents applied to non-scalar expression");
}

/*================================================================================*/


unsigned int Apply::max_depth(const vector<const Expr*>& components) {
  int max=0;
  for (unsigned int i=0; i<components.size(); i++) 
    if (components[i]->depth > max) max = components[i]->depth;
  return max;
}

Dim Apply::get_dim(Env& env, const char* f, const vector<const Expr*>& arguments) {
  const Function& func=env.function(f);

  if (func.nb_inputs() != arguments.size()) {
    stringstream s;
    s << "Number of actual arguments (" << arguments.size() << ") ";
    s << "does not match the number of formal arguments of \"" << func.name << "\" (" << func.nb_inputs() <<")";
    throw NonRecoverableException(s.str());
  }

  int i=0;
  vector<const Expr*>::const_iterator it=arguments.begin();
  for (; it!=arguments.end(); it++,i++) {
    if (! ((*it)->dim == func.arg_dim(i))) {
      stringstream s;
      s << "The dimension of the " << i << "th actual argument of \"" << func.name << "\"";
      s << "does not match the dimension of the corresponding formal argument";
      throw NonRecoverableException(s.str());
    }
  }
  return func.img_dim();
}

Apply::Apply(Env& env, const char* f, const vector<const Expr*>& arguments) : 
  Expr(env, max_depth(arguments)+1, SizeComputer(arguments).size+1, get_dim(env, f, arguments)), f(strdup(f)), n(arguments.size()) {
  //  args = new int[n];
  args = new const Expr*[n];
  for (int i=0; i<n; i++) {
    //(int&) args[i]=arguments[i]->id;
    args[i]=arguments[i];
  }
}
  
Apply::~Apply() {
  free((char*) f);
  delete[] args;
}

/*================================================================================*/

DomainOpType DomainOpExpr::get_op(DomainOpType op, const Dim& dim) {
  if (op==M_INF && dim.scalar()) return INF;
  if (op==INF && !dim.scalar()) return M_INF;
  if (op==M_MID && dim.scalar()) return MID;
  if (op==MID && !dim.scalar()) return M_MID;
  if (op==M_SUP && dim.scalar()) return SUP;
  if (op==SUP && !dim.scalar()) return M_SUP;
  else return op;
}

DomainOpExpr::DomainOpExpr(DomainOpType op, const Expr& expr) : Expr(expr.env, expr.depth+1,expr.size+1,expr.dim), expr(expr), op(get_op(op, expr.dim)) {  }

} // end namespace ibex
