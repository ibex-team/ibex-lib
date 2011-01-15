/*---------------------------------------------------------------------------------
 * 
 * Generator (which basically instanciate iterators)
 * -------------------------------------------------
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

#ifndef __GENERATOR_H
#define __GENERATOR_H

#include <vector>
#include <stack>
#include "IbexIdTable.h"
#include "IbexExpr.h"
#include "IbexContractor.h"
#include "IbexConstraint.h"

#include "ParserExpr.h"
#include "ParserConstraint.h" 
#include "ParserContractor.h" 
#include "ParserFunction.h"
#include "ParserBlock.h" 
#include "Scope.h"

namespace ibex {

namespace parser {

extern void generate_error (const std::string& msg);

extern stack<Scope> scopes;

extern IdTable<int> top_level;                                 // top level context (no iterators)

class ExprGenerator : public P_ExprVisitor {

 public:
  /* Creates a new expression generator in a given environment. */
  ExprGenerator(Env& env, const IdTable<int>& context=top_level) : value(0), env(env), context(context) { }

  /* Creates an Expr from a P_Expr, in particular by instanciating
   * all iterators to their actual value in the #context. */
  const Expr& generate(const P_Expr& expr) {
    visit(expr);
    return *new_expr;
  }

  /* Contains the result of the last call to generate
   * in case of a constant expression */
  P_Constant value;

 private:
  void visit(const P_Expr&);
  void visit(const P_Symbol&);
  void visit(const P_IterExpr&);
  void visit(const P_IterOp&);
  void visit(const P_IterCst&);
  void visit(const P_IterCstSymbolWithIndex&);
  void visit(const P_IterSymbol&);
  void visit(const P_SymboWithIterIndex&);
  void visit(const P_Constant&);
  void visit(const P_UnaOpExpr&);
  void visit(const P_BinOpExpr&);
  void visit(const P_PowerExpr&);
  void visit(const P_Apply&);
  void visit(const P_DomainOpExpr&);

  Env& env;

  const IdTable<int>& context; // current iterator values.

  /* true if last visited expression is a constant expression. */
  bool cst_expr; 

  /* Instantiation of P_Expr */
  const Expr* new_expr;

};

class FuncGenerator {
 public:
  /* Creates a new function generator. */
  FuncGenerator() { }

  /* Creates a function from a P_Function. */
  const Function& generate(const P_Function& f);
};

class CtrGenerator : public P_CtrVisitor {

 public:
  /* Creates a new constraint generator in a given environment. */
  CtrGenerator(Env& env, const IdTable<int>& context=top_level) : env(env), context(context) { }

  /* Creates a Constraint from a P_Ctr, in particular by instanciating
   * all iterators to their actual value in the #context. */
  const Constraint& generate(const P_Ctr& ctr) {
    visit(ctr);
    return *new_ctr;
  }

 private:
  void visit(const P_Ctr&);
  void visit(const P_Equality&);
  void visit(const P_Inequality&);
  void visit(const P_AtMostNVector&);
  void visit(const P_NotIn&);
  void visit(const P_CtrRef&);
  void visit(const P_CtrListElemRef&);

  Env& env;

  const IdTable<int>& context; // current iterator values.

  /* Instantiation of P_Ctr */
  const Constraint* new_ctr;

};

/* This class transforms a contractor expression potentially involving iterators
   into an iterator-free expression, by instantiating all iterators to their
   associated values contained in the context structure. */
class CtcGenerator : public P_CtcVisitor {
 public:
  /* Creates a new contractor generator in a given environment. */
  CtcGenerator(Env& env, IdTable<int>& context=top_level) : env(env), context(context) { }

  /* Creates a contractor from a P_Ctc expression, in particular by 
   * instanciating all iterators to their actual value in the #context. */
  const Contractor& generate(const P_Ctc& ctr) {
    visit(ctr);
    return *new_ctc;
  }


 protected:
  Env& env;

  void visit(const P_Ctc&);
  void visit(const P_CtcRef&);
  void visit(const P_CtcListElemRef&);
  void visit(const P_Shave&);
  void visit(const P_Newton&);
  void visit(const P_Propag&);
  void visit(const P_HC4Revise&);
  void visit(const P_BoxNarrow&);
  void visit(const P_ParamSplit&);
  void visit(const P_Precision&);
  void visit(const P_LogicOp&);
  void visit(const P_LogicOpLoop&);
  void visit(const P_QInter&);
  void visit(const P_Load&);

 protected:

  IdTable<int>& context; // current iterator values.

   /* Instantiation of P_Ctc */
  const Contractor* new_ctc;
};

template<class Tin, class Tout>
class ListGenerator : public P_ListVisitor<Tin> {
 public:
  /* Creates a new constraint block generator in a given environment and context. */
  ListGenerator(Env& env, vector<const Tout*>& vec, IdTable<int>& context=top_level) : 
    env(env), context(context), vec(vec) { }

  void visit(const P_List<Tin>& l) {
    l.accept(*this);
  }

  void visit(const P_BlkList<Tin>& list);

  void visit(const P_BlkListRef<Tin>&); // see below

  Env& env;

  IdTable<int>& context;       // current iterator values.

  vector<const Tout*>& vec;  // the vector we add constraint to
};


template<class Tin, class Tout>
class BlkGenerator : public P_BlkVisitor<Tin> {

 public:
  /* Creates a new constraint block generator in a given environment and context. */
  BlkGenerator(Env& env, vector<const Tout*>& vec, IdTable<int>& context=top_level) : 
    env(env), context(context), vec(vec) { }

  void visit(const P_Blk<Tin>& b);

  void visit(const P_Loop<Tin>& loop) { 
    const char* name     = loop.iter;
    int         begin    = loop.first_value; //scope.it_first_value(name);
    int         end      = loop.last_value; //scope.it_last_value(name);
    
    if (context.used(name)) generate_error(string(name)+": embedded redeclaration of an iterator symbol");
    
    context.insert_new(name,begin);

    ListGenerator<Tin,Tout> g(env,vec,context);
    
    for (int i=begin; i<=end; i++) {
      context.data(name)=i;
      g.visit(loop.iter_list);
    }
    
    context.erase(name);
  }

  void visit(const Tin&);    // see below

  Env& env;

  IdTable<int>& context;       // current iterator values.

  vector<const Tout*>& vec;  // the vector we add constraint to
};


template<>
void BlkGenerator<P_Ctr,Constraint>::visit(const P_Ctr& ctr);
template<>
void BlkGenerator<P_Ctc,Contractor>::visit(const P_Ctc& ctc);

template<>
void BlkGenerator<P_Ctr,Constraint>::visit(const P_Blk<P_Ctr>& b);
template<>
void BlkGenerator<P_Ctc,Contractor>::visit(const P_Blk<P_Ctc>& b);

template<>
void ListGenerator<P_Ctr,Constraint>::visit(const P_BlkListRef<P_Ctr>& ref);
template<>
void ListGenerator<P_Ctc,Contractor>::visit(const P_BlkListRef<P_Ctc>& ref);

} // end namespace

} // end namespace


#endif
