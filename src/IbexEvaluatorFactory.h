/*---------------------------------------------------------------------------------
 * 
 * Evaluator factory
 * -----------------
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

#ifndef _IBEX_EVALUATOR_BUILDER_H
#define _IBEX_EVALUATOR_BUILDER_H

#include <set>
#include <list>
#include "IbexExpr.h"
#include "IbexVisitor.h"

namespace ibex {

class Assign;
class Evaluator;
class Env;

/* \ingroup exprgroup
 *
 * \brief Expression evaluator factory.
 *
 * \author Gilles Chabert
 * \date March 2007 
 */
class EvaluatorFactory : private Visitor {
 public:

  /** Create a factory for a given expression. */
  EvaluatorFactory(const Expr& expr);

  /** Build the evaluator. */
  Evaluator* build(Evaluator* ev);

 private:
  Env& env;

  void visit(const Expr&);
  void visit(const SymbolWithIndex&);
  void visit(const Symbol&);
  void visit(const Constant&);
  void visit(const UnaOpExpr&);
  void visit(const BinOpExpr&);
  void visit(const PowerExpr&);
  void visit(const Apply&);
  void visit(const DomainOpExpr&);
/*   void visit(const Assign&); */
/*   void visit(const Constraint&); */

  list<int> code;
  list<int> info;           

  list<const Apply*> calls;
  list<INTERVAL> Ic;
  list<INTERVAL_MATRIX> Mc;

  /* Expressions that are interval or matrix operand. Ex: in A[2][3]*x[1],
   * A[2][3], x[1] and A[2][3]*x[1] are interval operands, while A, x and A[2] 
   * are not. */
  set<int> itv_operand;
  set<int> mat_operand;

  /* nodes sorted by level (depth). */
  vector<set<int> > levels; 

  /* pos[i] is the position (either in the array I or M) of the 
   * domain corresponding to the expression whose id is i. */
  //int* pos; 
  hash_map<int,int> pos; 
};

} // end namespace

#endif
