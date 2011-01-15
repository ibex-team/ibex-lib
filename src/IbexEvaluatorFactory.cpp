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

#include "Utilities.h"
#include "IbexEvaluatorFactory.h"
#include "IbexEnv.h"
#include "IbexToken.h"
#include "IbexEvaluator.h"
#include "IbexFunction.h"

/*================================================================================*/

#include <vector>
#include <list>

#define idtype int

namespace ibex {

/* This visitor divides all the nodes of an expression DAG up 
 * into "levels". 
 * A level represents a given depth in the DAG.
 */
class TopoSort : private Visitor {

 public:

  TopoSort(const Expr& expr, vector<set<idtype> >& levels) : levels(levels) {
    visit(expr);
  }

 private:

  vector<set<idtype> >& levels; // nodes sorted by level (depth).
  int shift;

  void visit(const Expr& e) {   
    //cout << "looking for " << e.id << " in level " << e.depth << endl;
    if (levels[e.depth].find(e.id)==levels[e.depth].end()) {
      levels[e.depth].insert(e.id);
      e.acceptVisitor(*this);
    }
  }

  void visit(const SymbolWithIndex& c) { visit(c.expr); }

  void visit(const Symbol&) { }

  void visit(const Constant& cst) { }

  void visit(const UnaOpExpr& u) { visit(u.expr); }

  void visit(const BinOpExpr& b) { visit(b.left); visit(b.right); }

  void visit(const PowerExpr& p) { visit(p.expr); }

  void visit(const Apply& a) { 
    for (int i=0; i<a.n; i++) visit(a.arg(i));
  }

  void visit(const DomainOpExpr& d) { visit(d.expr); }

//   void visit(const Assign& a) { 
//     visit(a.symbol); 
//     visit(a.right); 
//   }

//   void visit(const Constraint& c) { visit(c.expr); }
};


#define itv_op(expr_id) (itv_operand.find(expr_id)!=itv_operand.end())
#define mat_op(expr_id) (mat_operand.find(expr_id)!=mat_operand.end())


/*================================================================================*/

/* This visitor checks whether a node is an "interval" or "matrix" operand. E.g., if x and y are matrices
 * then, the node (x+y) is labeled as "matrix" operand, as well as x and y themselves.
 * This distinction allows to speed up the evaluator, since in case of scalar operations, some work 
 * is spared (no need to manage matrices.)
 * Furthermore, in case of indexed variables, storing the domain of intermediate symbols can be skipped.
 * E.g., if x is a matrix (or 2-dimension array) and y a real variable, then, to compute (y*x[2][3]), the 
 * evaluator requires that the interval domain of each operand is stored (in the table called I). 
 * Hence, x[2][3] must be stored, but it is useless to store x[2] and x. OperandCheck detects
 * that both of the latters are not operand, so that heavy array copies will be spared. Note that in
 * the same DAG, both x[2][3] and x[2] or even x may be operands. E.g.:
 *
 *    function foo(x[3][3]) returns y
 *      tmp1 := x;
 *      tmp2 := x[2];
 *      y := tmp1[2][3]+tmp2[3]+x[2][3]
 *    end
 *
 * Rem: In an Assign object y=f(x), the symbol y is not considered as an operand, because the domain of
 * y and f are the same (they share the same place in memory)  (see EvaluatorFactory::visit(const Assign& a) for more details).
 */
class OperandCheck : private Visitor {

 public:
  OperandCheck(const Expr& expr, set<int>& itv_operand, set<int>& mat_operand) : 
    itv_operand(itv_operand), mat_operand(mat_operand) {

    visit(expr);
    if (expr.scalar()) itv_operand.insert(expr.id);
    else mat_operand.insert(expr.id);
  }
  
 private:

  set<int>& itv_operand;
  set<int>& mat_operand;

  void visit(const Expr& e) {   
    if (!itv_op(e.id) && !mat_op(e.id))
      e.acceptVisitor(*this);
  }

  void visit(const SymbolWithIndex& c) {   
    visit(c.expr);
  }

  void visit(const Symbol&) { }

  void visit(const Constant&) { }

  void visit(const UnaOpExpr& u) {
    visit(u.expr);
    if (u.scalar()) itv_operand.insert(u.expr.id);
    else mat_operand.insert(u.expr.id);
  }

  void visit(const BinOpExpr& b) {   
    visit(b.left);
    visit(b.right);

    if ((b.scalar() && b.op!=V_DOT) || b.op==M_SCAL) itv_operand.insert(b.left.id);
    else mat_operand.insert(b.left.id);
    if (b.scalar() && b.op!=V_DOT) itv_operand.insert(b.right.id);
    else mat_operand.insert(b.right.id);
  }

  void visit(const PowerExpr& p) {   
    visit(p.expr);
    itv_operand.insert(p.expr.id);
  }

  void visit(const Apply& a) {   
    for (int i=0; i<a.n; i++) {
      const Expr& arg=a.arg(i);
      visit(arg);
      if (arg.scalar()) itv_operand.insert(arg.id);
      else mat_operand.insert(arg.id);
    }
  }

  void visit(const DomainOpExpr& d) {
    visit(d.expr);
    if (d.scalar()) itv_operand.insert(d.expr.id);
    else mat_operand.insert(d.expr.id);
  }

//   void visit(const Assign& a) {   
//     /* the output symbol is not an operand: it shares
//      * the same place in memory as the right expression. */
//     visit(a.right);
//     if (a.right.scalar()) itv_operand.insert(a.right.id);
//     else mat_operand.insert(a.right.id);
//   }

//   void visit(const Constraint& c) {   
//     /* the constant (either 0, [-inf,0] or [0,+inf]) is not an operand.
//      * it shares the same place in memory as the constraint expression. */
//     visit(c.expr);
//     if (c.expr.scalar()) itv_operand.insert(c.expr.id);
//     else mat_operand.insert(c.expr.id);
//   }
};



/*================================================================================*/
// EvaluatorFactory::EvaluatorFactory(const Constraint& ctr) : levels(ctr.depth+1) {
//   /* fills up itv_operand, mat_operand and levels. */
//   OperandCheck(ctr.expr, itv_operand, mat_operand);
//   TopoSort(ctr.expr, levels);
// }

// EvaluatorFactory::EvaluatorFactory(const Assign& asg) : levels(asg.depth+1) {
//   /* fills up itv_operand, mat_operand and levels. */
//   OperandCheck(asg, itv_operand, mat_operand);
//   TopoSort(asg, levels);
// }

EvaluatorFactory::EvaluatorFactory(const Expr& expr) : env(expr.env), levels(expr.depth+1) {
  /* fills up itv_operand, mat_operand and levels. */
  OperandCheck(expr, itv_operand, mat_operand);
  TopoSort(expr, levels);
}

Evaluator* EvaluatorFactory::build(Evaluator* ev) { 

  if (!ev) ev = new Evaluator();

  int itvl = itv_operand.size(); // number of intervals to store 
  int matl = mat_operand.size(); // number of matrices to store  

  ev->itvl  = itvl;
  ev->matl  = matl;
  if (itvl>0) {
    ev->I     = new INTERVAL[itvl];
    ev->G     = new INTERVAL[itvl];
    // the following loop just to avoid the "uninitialized value" error by valgrind
    for (int i=0; i<itvl; i++)
      ev->I[i]=ev->G[i]=0;
  }
  if (matl>0) {
    ev->M     = new INTERVAL_MATRIX[matl];
  }

  /* Processes the expression DAGs */
  for (unsigned int i=0; i<levels.size(); i++) { // from leaves upto roots
    for (set<int>::const_iterator it=levels[i].begin(); it!=levels[i].end(); it++) { 
      const Expr& expr = env.expression(*it);
      if (itv_op(expr.id)) 
	pos[expr.id] = --itvl;
      else if (mat_op(expr.id)) {
	pos[expr.id] = --matl; // pos is also modified in visit(Assign&)
	Resize(ev->M[matl], expr.dim.size2(),expr.dim.size3());
      }
      visit(expr);
    }
  } 
    
  ev->codel = code.size();
  ev->infol = info.size();
  ev->icstl = Ic.size();
  ev->mcstl = Mc.size();
  ev->funcl = calls.size();
  ev->code  = new int[code.size()];
  ev->info  = new int[info.size()];
  if (Ic.size()>0) {
    ev->Ic    = new INTERVAL[Ic.size()];
    // the following loop just to avoid the "uninitialized value" error by valgrind
    for (unsigned int i=0; i<Ic.size(); i++)
      ev->Ic[i]=0;
  }
  if (Mc.size()>0) {
    ev->Mc    = new INTERVAL_MATRIX[Mc.size()];
  }
  ev->func  = new const Function*[calls.size()];
  ev->args  = new Space*[calls.size()];

  int i=0;
  for (list<int>::const_iterator it=code.begin(); it!=code.end(); it++) 
    ev->code[i++] = *it;
  i=0;
  for (list<int>::const_iterator it=info.begin(); it!=info.end(); it++) 
    ev->info[i++] = *it;
  i=0;
  for (list<INTERVAL>::const_iterator it=Ic.begin(); it!=Ic.end(); it++) 
    ev->Ic[i++] = *it;
  i=0;
  for (list<INTERVAL_MATRIX>::const_iterator it=Mc.begin(); it!=Mc.end(); it++) {
    Resize(ev->Mc[i], RowDimension(*it), ColDimension(*it));
    ev->Mc[i++] = *it;
  }
  i=0;
  /**** APPLY ****/
  for (list<const Apply*>::const_iterator it=calls.begin(); it!=calls.end(); it++,i++) {
    const Apply& a = **it;
    const Function& f=env.function(a.f);
    ev->func[i] = &f;
  
    Space* apply_space = new Space(f.env);
    int key = f.env.nb_keys();

    /* creates temporary entities */
    for (int j=0; j<f.temp_size(); j++)
      apply_space->add_entity(--key, *new INTERVAL(BiasNegInf,BiasPosInf), IBEX_VAR);
    
    /* creates output entities */
    Dim fdim = f.img_dim();
    if (fdim.scalar()) {
      apply_space->add_entity(--key, ev->I[pos[a.id]], IBEX_VAR);
    } else {
      for (int k=fdim.size2(); k>=1; k--) 
	for (int l=fdim.size3(); l>=1; l--) 	  
	  apply_space->add_entity(--key, ev->M[pos[a.id]](k,l), IBEX_VAR);
    }

    /* creates input entities */
    for (int j=a.n-1; j>=0; j--) { // rem: a.n = f.n
      const Expr& arg=a.arg(j);
      if (arg.scalar()) {
	apply_space->add_entity(--key, ev->I[pos[arg.id]], IBEX_VAR);
	ev->I[pos[a.id]]=0; // just to avoid the "uninitialized value" error by valgrind
      } else {
	for (int k=arg.dim.size2(); k>=1; k--) 
	  for (int l=arg.dim.size3(); l>=1; l--) 
	    apply_space->add_entity(--key, ev->M[pos[arg.id]](k,l), IBEX_VAR);
      }
    }
    ev->args[i]=apply_space;
  }

  return ev;
}

void EvaluatorFactory::visit(const Expr& e) { 
  e.acceptVisitor(*this); 
}

void EvaluatorFactory::visit(const SymbolWithIndex& s) { 
  if (itv_op(s.id)) {
    code.push_front(SYMBOL);
    info.push_front(s.key);
  }
  else if (mat_op(s.id)) {
    code.push_front(M_SYMBOL);
    info.push_front(s.key);
    info.push_front(s.dim.size3());
    info.push_front(s.dim.size2());
  }
}

void EvaluatorFactory::visit(const Symbol& s) { 
  if (itv_op(s.id)) {
    code.push_front(SYMBOL);
    info.push_front(s.key);
  }
  else if (mat_op(s.id)) {
    code.push_front(M_SYMBOL);
    info.push_front(s.key);
    info.push_front(s.dim.size3());
    info.push_front(s.dim.size2());
  }
}

void EvaluatorFactory::visit(const Constant& c) { 
  if (itv_op(c.id)) {
    code.push_front(CST);
    Ic.push_front(c.get_value());
  } else if (mat_op(c.id)) {
    code.push_front(M_CST);
    Mc.push_front(c.get_matrix_value());
  }
}

void EvaluatorFactory::visit(const UnaOpExpr& u) {
  code.push_front(u.op);

  info.push_front(pos[u.expr.id]);
}
  
void EvaluatorFactory::visit(const BinOpExpr& b) {
  code.push_front(b.op);

  info.push_front(pos[b.right.id]);
  info.push_front(pos[b.left.id]);
}

void EvaluatorFactory::visit(const PowerExpr& p) {
  code.push_front(POW);

  info.push_front(pos[p.expr.id]);
  info.push_front(p.expon);
}

void EvaluatorFactory::visit(const Apply& a) { 
  if (itv_op(a.id)) code.push_front(APPLY);
  else code.push_front(M_APPLY);
  calls.push_front(&a);
}

void EvaluatorFactory::visit(const DomainOpExpr& d) {
  code.push_front(d.op);

  info.push_front(pos[d.expr.id]);
}

// void EvaluatorFactory::visit(const Constraint& c) {
//   if (c.scalar()) {
//     code.push_front(c.op);
//     switch (c.op) {      
//     case LT       : 
//     case LEQ      : Ic.push_front(INTERVAL(BiasNegInf,0)); break;
//     case EQU      : Ic.push_front(INTERVAL(0,0));          break;
//     case GEQ      : 
//     case GT       : Ic.push_front(INTERVAL(0,BiasPosInf)); break;    
//     } 
//   } else {
//     code.push_front(M_EQU);
//     INTERVAL_MATRIX M(c.dim.size2(),c.dim.size3());
//     Initialize(M,0.);
//     Mc.push_front(M);
//   }
// }

} // end namespace
