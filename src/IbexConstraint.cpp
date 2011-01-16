/*---------------------------------------------------------------------------------
 * 
 * Constraints defined by a DAG of expressions
 * -------------------------------------------
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

/* ------------------------------------------------------------------------------*/
/* last update : chabert, 12 Sept 09                                             */
/* ------------------------------------------------------------------------------*/

#include <vector>
#include "IbexConstraint.h"
#include "IbexExprAdjFactory.h"
#include "IbexEvaluator.h"
#include "IbexEvaluatorFactory.h"
#include "IbexOperatorPrinter.h"

namespace ibex {


// const Constant& Equality::get_cst(const Expr& expr, CmpOpType op) {
  
//   if (expr.scalar()) {
//     switch (op) {
//     case EQU : return *new Constant(INTERVAL(0,0)); break;
//     case LT  : 
//     case LEQ : return *new Constant(INTERVAL(BiasNegInf,0)); break;
//     case GT  : 
//     case GEQ : return *new Constant(INTERVAL(0,BiasPosInf)); break;
//     }
//   }

//   INTERVAL_MATRIX zero(expr.dim.size2(),expr.dim.size3());
//   Initialize(zero,0.);
//   return *new Constant(zero);    
// }
  

/* name is set by the environment. */
Constraint::Constraint() : env_num(-1), name(NULL) { }

ostream& operator<<(ostream& os, const Constraint& ctr) {
  ctr.accept_printer(OperatorPrinter(os));
  return os;
//  return os << ctr; ---> warning: provokes infinite call loop
}

ConstraintSpaceFactory::ConstraintSpaceFactory(const Constraint& ctr, bool only_multi_occ) : SpaceFactory(ctr.get_env()) {

  for (hash_map<int,int>::const_iterator it=ctr.adj.begin(); it!=ctr.adj.end(); it++) {
    set_entity(it->first, (!only_multi_occ || it->second>1) ? IBEX_VAR : IBEX_SYB);
  }
}


/* -------------------------------------------------------------------------------------------*/

ArithConstraint::ArithConstraint(const Expr& expr) : expr(expr) {
  ExprAdjFactory(expr, adj);

  EvaluatorFactory fac(expr);
  fac.build(&evl);
}

INTERVAL ArithConstraint::eval(const Space& space) const {
  evl.forward(space);
  return evl.output();
}

/* some work is still left: catch exceptions in case of non differentiability, etc. */
INTERVAL ArithConstraint::eval_mono1(const Space& space) const {

  int nmax=adj.size();  // nmax is the number of entities (but monotonicity is only applied for variables)

  INTERVAL savebox[nmax];                  // save domains
  INTERVAL* X[nmax];                       // domains of variables (introduced for convenience)
  INTERVAL deriv[nmax];                    // partial derivatives (introduced for convenience)
  bool first=true;                         // true at first evaluation (on the whole initial box)
  int firstfixed[nmax];                    // monotonicity wrt variables on the initial box (firstfixed[i]= {-1->decreasing, 0->unknown, 1->increasing})
  bool fixed[nmax];                        // variables with an instanciated bound
  bool loop=true;                          // true if at least an extra variable has been instanciated in the loop

  /* init */
  int n=0;                                 // n is the number of variables
  for (hash_map<int,int>::const_iterator it = adj.begin(); it!=adj.end(); it++) {
    int v = it->first; 
    if (space.entity(v).type!=IBEX_VAR) continue;
    X[n] = space.entity(v).domain;
    savebox[n]=*X[n];
    space.entity(v).deriv = &deriv[n];
    fixed[n]=false;                        // no variable fixed at start
    firstfixed[n]=0;                       // by default
    deriv[n]=0;                            // gradient init (required?)
    n++;
  }

  /* find the best minimizer */            // cout << "minimizing...\n";
  while (loop) {
    loop=false;
    gradient(space);
    for (int i=0; i<n; i++) {
            //cout << "var " << i << " domain=[" << Inf(*X[i]) << "," << Sup(*X[i]) << "]       ";
            //cout << " deriv=[" << Inf(deriv[i]) << "," << Sup(deriv[i]) << "]" << endl;
    }
    for (int i=0; i<n; i++) {
      if (!fixed[i]) {
	if (Inf(deriv[i])>=0) { 
	  if (first) firstfixed[i]=1;
	  loop=fixed[i]=true;
	  *X[i]=Inf(*X[i]);
	}
	else if(Sup(deriv[i])<=0) {
	  if (first) firstfixed[i]=-1;
	  loop=fixed[i]=true;
	  *X[i]=Sup(*X[i]);
	}
      }
      deriv[i]=0;                       // gradient init required by Evaluator (for next call)
    }
    if (first) {
      if (!loop) return eval(space);                  // can't do better
      first=false;
    }
  }
  
//   bool nonoptimal=false;
//   for (int i=0; i<n; i++) {
//     if (!fixed[i]) { nonoptimal=true; }
//   }
//   if (nonoptimal) {
//     cout << "non optimal " << endl; 
//   }

  REAL min = Inf(eval(space));
  //cout << "maximizing...\n";
  /* reinit for max computation */
  for (int i=0; i<n; i++) {
    if (firstfixed[i]==1) *X[i]=Sup(savebox[i]);
    else if (firstfixed[i]==-1) *X[i]=Inf(savebox[i]);
    else if (fixed[i]) {
      *X[i]=savebox[i];
      fixed[i]=false;
    }
  }
  loop=true;

  /* find the best maximizer */
  while (loop) {
    loop=false;
    gradient(space);
    for (int i=0; i<n; i++) {
      if (!fixed[i]) {
	if (Inf(deriv[i])>=0) { 
	  loop=fixed[i]=true;
	  *X[i]=Sup(*X[i]);
	}
	else if(Sup(deriv[i])<=0) {
	  loop=fixed[i]=true;
	  *X[i]=Inf(*X[i]);
	}
      }
      deriv[i]=0;      // gradient init required by Evaluator (for next call)
    }
  }
  
//   if (!nonoptimal) {
//     for (int i=0; i<n; i++) {
//       if (!fixed[i]) { nonoptimal=true; }
//     }
//     if (nonoptimal) {
//       cout << "non optimal " << endl; 
//     }
//   }

  REAL max = Sup(eval(space));

  /* restore domains */
  for (int i=0; i<n; i++) {
    if (fixed[i]) *X[i]=savebox[i];
  }


  INTERVAL x1(min,max);
  INTERVAL x2(eval(space));

//   if (!x1.included(x2)) { cout <<" bug\n"; 
//     cout << "x1=" << x1 << " x2=" << x2 << endl;
//     exit(-1); 
//   }
  
//   cout << "RESULT : " << x1 << " instead of " << x2 << "  ratiodelta=" << x2.ratiodelta(x1) << endl;


  return INTERVAL(min,max);
}

INTERVAL_MATRIX ArithConstraint::m_eval(const Space& space) const {
  evl.forward(space);
  return evl.m_output();
}

INTERVAL ArithConstraint::eval_pt(const Space& space, const VECTOR& pt) const {
  if (Dimension(pt)!=Dimension(space.box)) throw NonRecoverableException("Bad vector argument dimension in \"eval_pt\"");
  INTERVAL_VECTOR savebox = space.box;
  (INTERVAL_VECTOR&) space.box = pt;
  INTERVAL res = eval(space);
  (INTERVAL_VECTOR&) space.box = savebox;
  return res;
}

void ArithConstraint::gradient(const Space& space, INTERVAL_VECTOR& G) const {

  if (Dimension(G)!=space.nb_var()) throw NonRecoverableException("Bad vector argument dimension in \"gradient\"");
  
  for (int j=0; j<space.nb_var(); j++) {
    G(j+1) = 0.0;
    space.ent(IBEX_VAR,j).deriv = &G(j+1);     
  }

  gradient(space);
}

INTERVAL ArithConstraint::derivative(const Space& space, int var) const {

  INTERVAL res(0);
  Entity::IGNORE=0;

  for (int j=0; j<space.nb_var(); j++) {
    space.ent(IBEX_VAR,j).deriv = (j==var)? &res : &Entity::IGNORE;
  }

  gradient(space);

  return res;
}


/*================================================================================*/

Equality::Equality(const Expr& expr) : ArithConstraint(expr), op(expr.scalar()? EQU : M_EQU), zero(expr.dim.size2(), expr.dim.size3()) { 
  Initialize(zero,0.);    
}

bool Equality::feasible(const Space& space) const { 

  if (op==EQU) {
    return eval(space).contains(0);
  } else {
    return m_eval(space).contains(Mid(zero));
  }
}

void Equality::backward(Space& space) const {

  bool sat;
  if (op==EQU) {
    sat = evl.output() &= INTERVAL(0,0);       
  } else { // M_EQU 
    sat = evl.m_output() &= zero;
  }
  if (!sat) throw EmptyBoxException();
  evl.backward(space);
}


/*================================================================================*/



Inequality::Inequality(const Expr& expr, CmpOpType op) : ArithConstraint(expr), op(op) {  //, cst(get_cst(expr,op)) { }
  if (!expr.scalar()) throw NonRecoverableException("Inequalities with vector operands not implemented yet");
}

bool Inequality::feasible(const Space& space) const {
  return (((op==LT || op==LEQ) && Inf(eval(space))<=0) 
	  || 
	  ((op==GT || op==GEQ) && Sup(eval(space))>=0));
}

void Inequality::backward(Space& space) const {

  //cout << "backward " << *this << endl;

  switch (op) {      
  case LT       : 
  case LEQ      : if (evl.output().included(INTERVAL(BiasNegInf,0))) return;
                  break;
  case GEQ      : 
  case GT       : if (evl.output().included(INTERVAL(0,BiasPosInf))) return;
                  break;
  default       : stringstream s;
                  s << "Inequality: unexpected comparison operator \"" << op << "\"";
		  throw NonRecoverableException(s.str());
  }

  bool sat;
  switch (op) {      
  case LT       : 
  case LEQ      : sat = evl.output() &= INTERVAL(BiasNegInf,0); break;
  case GEQ      : 
  case GT       : sat = evl.output() &= INTERVAL(0,BiasPosInf); break;    
  default       : stringstream s;
                  s << "Inequality: unexpected comparison operator \"" << op << "\"";
		  throw NonRecoverableException(s.str());
  }
  if (!sat) throw EmptyBoxException();
  evl.backward(space);
}


// Inequality::Inequality(const Expr& left, CmpOpType op, const Expr& right) : 
//   Expr(max(left.depth, right.depth)+1, left.width+right.width+1, BinOpExpr::get_dim(left.dim, SUB, right.dim)),
//   expr(right.is_zero() ? left : (*new BinOpExpr(left, SUB, right))), op(op), cst(get_cst(expr,op)) {
  // Removing the right expression also avoids to cast "0" into the appropriate null vector/matrix.
  // Hence, somebody can enter "A=0" (A being a matrix) instead of building a zero matrix Z and
  // entering "A=Z".
// }  

/*================================================================================*/


NotIn::NotIn(const Expr& left, const Expr& right) : left(left), right(right) { 

  ExprAdjFactory(left-right, adj);

  EvaluatorFactory(left).build(&l_evl);

  EvaluatorFactory(right).build(&r_evl);
}

/*bool NotIn::feasible(const Space& space) const {
  if (cst.scalar()) {
    return (!eval(space).included(cst.get_value()));
  } else {
    return (!m_eval(space).included(cst.get_matrix_value()));
  }
}*/

void NotIn::forward(const Space& space) const {
  l_evl.forward(space);
  r_evl.forward(space);
}

void NotIn::backward(Space& space) const {
  if (left.scalar()) { 
    INTERVAL img=l_evl.output(); // a copy 
    const INTERVAL& v=r_evl.output();    

    if ((v & img).empty()) return;

    if (img.included(v)) throw EmptyBoxException();

    if (v.included(img)) {
      INTERVAL_VECTOR epx=space.epx;
      INTERVAL_VECTOR box=space.box;

      l_evl.output() = INTERVAL(Inf(img),Inf(v));

      INTERVAL_VECTOR box1(space.nb_var());
      try {
	l_evl.backward(space);
	box1=space.box;
      }
      catch(EmptyBoxException) {
	box1.set_empty();
      }
      
      space.box = box;
      space.epx = epx;

      l_evl.forward(space);
      l_evl.output() = INTERVAL(Sup(v),Sup(img));

      try {
	l_evl.backward(space);
	space.box |= box1;
      }	
      catch(EmptyBoxException) {
	space.box = box1;
      }
      space.epx = epx;

    } else {

      if (Inf(img)<Inf(v)) 
	l_evl.output() = INTERVAL(Inf(img),Inf(v));
      else 
	l_evl.output() = INTERVAL(Sup(v),Sup(img));

      l_evl.backward(space);
    }
  } else {

    /* we avoid combinatory here (induced by the previous schema) */
    for (int i=1; i<=left.dim.dim2; i++) {
      for (int j=1; j<=left.dim.dim3; j++) {
	INTERVAL& img=l_evl.m_output()(i,j); // *not* a copy
	const INTERVAL& v=r_evl.m_output()(i,j);    

	if (img.included(v)) throw EmptyBoxException();
	if (v.included(img)) continue;
	if (Inf(img)<Inf(v)) img=INTERVAL(Inf(img),Inf(v));
	else img=INTERVAL(Sup(v),Sup(img));
      }
    }
    l_evl.backward(space);
  }
}
void Equality::inner_backward(Space& space) const {

  //rien pour le moment
}

void Inequality::inner_backward(Space& space) const {

  //cout << "backward " << *this << endl;

  switch (op) {     
  case LT       :
  case LEQ      : if (evl.output().included(INTERVAL(BiasNegInf,0))) return;
                  break;
  case GEQ      :
  case GT       : if (evl.output().included(INTERVAL(0,BiasPosInf))) return;
                  break;
  default       : stringstream s;
                  s << "Inequality: unexpected comparison operator \"" << op << "\"";
          throw NonRecoverableException(s.str());
  }

  bool sat;
  switch (op) {     
  case LT       :
  case LEQ      : sat = evl.output() &= INTERVAL(BiasNegInf,0); break;
  case GEQ      :
  case GT       : sat = evl.output() &= INTERVAL(0,BiasPosInf); break;   
  default       : stringstream s;
                  s << "Inequality: unexpected comparison operator \"" << op << "\"";
          throw NonRecoverableException(s.str());
  }
  if (!sat) throw EmptyBoxException();
  evl.inner_backward(space);
}



/*================================================================================*/

const Equality& Expr::operator=(const Expr& right) const { 
  return Equality::new_(right.is_zero()? *this : *this-right); 
}
  
const Equality& Expr::operator=(const INTERVAL& value) const { 
  return Equality::new_(Inf(value)==0 && Sup(value)==0? *this : *this-value); 
}

const Inequality& Expr::operator<=(const Expr& right) const { 
  return Inequality::new_(right.is_zero()? *this : *this-right, LEQ); 
}

const Inequality& Expr::operator<=(const INTERVAL& value) const { 
  return Inequality::new_(Inf(value)==0 && Sup(value)==0? *this : *this-value, LEQ); 
}

const Inequality& Expr::operator>=(const Expr& right) const { 
  return Inequality::new_(right.is_zero()? *this : *this-right, GEQ); 
}

const Inequality& Expr::operator>=(const INTERVAL& value) const { 
  return Inequality::new_(Inf(value)==0 && Sup(value)==0? *this : *this-value, GEQ); 
}

const Inequality& Expr::operator<(const Expr& right) const { 
  return Inequality::new_(right.is_zero()? *this : *this-right, LT); 
}

const Inequality& Expr::operator<(const INTERVAL& value) const { 
  return Inequality::new_(Inf(value)==0 && Sup(value)==0? *this : *this-value, LT); 
}

const Inequality& Expr::operator>(const Expr& right) const { 
  return Inequality::new_(right.is_zero()? *this : *this-right, GT); 
}

const Inequality& Expr::operator>(const INTERVAL& value) const { 
  return Inequality::new_(Inf(value)==0 && Sup(value)==0? *this : *this-value, GT); 
}

/* -------------------------------------------------------------------------------------------*/

} // end namespace ibex
