/*---------------------------------------------------------------------------------

 * Optimizer
 * ---------------------------
 *
 * Copyright (C) 2007-2010 Gilles Chabert
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


#include "IbexConstraint.h"
#include "IbexSequence.h"
#include "IbexEvaluatorFactory.h"

#include "IbexHC4Revise.h"
#include "IbexHC4.h"
#include "IbexCellStack.h"
#include "IbexOptimizer.h"


#include <math.h>

namespace ibex {
 

/*! Default number of samples used to bound the minimum on a box. */
#define __IBEX_DEFAULT_MINIMIZER_SAMPLE_SIZE     10

const int Optimizer::default_sample_size = __IBEX_DEFAULT_MINIMIZER_SAMPLE_SIZE;


/**
 * Each node in the search tree of the optimizer
 * has an associated inner box, stored as a
 * backtrackable stucture.
 *
 * last update: GCH
 */
class InnerBox : public Backtrackable {
public:
  // warning: we must resize the "inner_box" field the
  //          first time an inner box is found
  //          (by default the size is 1)
  InnerBox() : inner_box(1) { inner_box(1) = INTERVAL::EMPTY; }

  InnerBox(InnerBox& father, bool side) : inner_box(father.inner_box) { }

  INTERVAL_VECTOR inner_box;
};

/** Transform a system of inequalities
 * (eg: g_1(x)<=0 ^ ... ^g_m(x)<=0)
 * into the union of all the reversed inequalities 
 * (=> g_1(x)>0 v ... v g_m(x)>0)
 *
 * The special equation "goal_ctr" is not considered in this process 
*/
  Sequence inverse(const System& sys, int goal_ctr) {

  Env& env=(Env&) sys.space.env;

  vector<const Contractor*> new_ctcs; // the new contractors for inverse inequalities

  for (int i=0; i<sys.nb_ctr(); i++) {

    // constraint n�goal_ctr =  criterion to optimize -> skip it
    if (i==goal_ctr) continue;

    const ArithConstraint& c=sys.ctr(i);
      
    if (c.is_equality()) throw NonRecoverableException("Only inequality constraints accepted for optimization");
      
    const Inequality& ineq=(const Inequality&) c; // safe cast

    CmpOpType new_op; // the inverse operator
    switch (ineq.op) {      
    case LT  : new_op=GEQ; break;
    case LEQ : new_op=GT;  break;
    case GEQ : new_op=LT;  break;
    case GT  : new_op=LEQ; break;
    default  : stringstream s;
      s << "Inequality: unexpected comparison operator \"" << ineq.op << "\"";
      throw NonRecoverableException(s.str());
    }

    int num=env.add_ctr(Inequality::new_(ineq.expr,new_op));

    new_ctcs.push_back(new HC4Revise(env.constraint(num), sys.space));
  }
  
  ContractorList new_sys(new_ctcs);

  for (int i=0; i<sys.nb_ctr()-1; i++) {
    delete new_ctcs[i];
  }

  return Sequence(new_sys, sys.space, false);
  
}


/**
 *
 * \param goal_ctr - The number of the constraint y=f(x) in \a sys
 * \param y_num - The number of the objective variable y in \a sys.space
 */
Optimizer::Optimizer(const System& sys,  int y_num, Contractor& contractor, 
		       Bisector & bisect,
		     bool maximize, REAL prec, REAL goal_ceil, int sample_size, int goal_ctr1) :
  Paver(sys.space, ContractorList(contractor, Precision(sys.space,prec)), bisect, *new CellHeapForOptim(y_num)),  
  sys(sys),
  y_num(y_num),
  goal_ctr(goal_ctr1),
  prec_num(1), goal_ceil(goal_ceil),
  loup(BiasPosInf), sample_size(sample_size), loup_changed(false),
  uplo(BiasPosInf), uplo_of_solutions(BiasPosInf)
{ 

  /* get the goal function from the constraint y=f(x) */
  const Expr& goal_function=(((const BinOpExpr&)sys.ctr(goal_ctr).expr).right);
  EvaluatorFactory fac(goal_function);
  fac.build(&goal);

  m.add<InnerBox>();

}


ConstrainedOptimizer::ConstrainedOptimizer(const System& sys, int goal_ctr, int y_num, Contractor& contractor, 
					   Bisector & bisect,
					   bool maximize, REAL prec, REAL goal_ceil, int sample_size) :
  Optimizer (sys, y_num, contractor, bisect, maximize, prec, goal_ceil, sample_size, goal_ctr),
  is_inside(inverse(sys,goal_ctr))
{;}  


ConstrainedOptimizer::~ConstrainedOptimizer(){}
  
Optimizer::~Optimizer() {
  delete &cells;
}

void Optimizer::contract_and_bound(Cell* c) {
  space.box = c->box; // rem: c.space == space
  space.epx = c->epx;
  //cout << c->box << endl;
  
  /*======================== contract with loup ========================*/
  INTERVAL& y=space.box(y_num+1); //  y_num is the objective
  REAL ymax=loup;
  
  if ((fabs (loup) < 1) || (goal_ceil > 1))  // goal_ceil is a relative precision  and should be less than 1
    ymax=loup-goal_ceil;
  else 
    ymax=loup*(1-goal_ceil);
  
  if (Inf (y) > ymax) { 
    delete c; // if this procedure was in a contractor ---> throw EmptyBoxException();
    return;
  }
  if (Sup (y) > ymax) y = INTERVAL(Inf(y),ymax);  
  /*====================================================================*/
    

  /*======================== contract with g(x)<=0 =====================*/
  int ctc_num=contract(*c);
  /*====================================================================*/


  if (ctc_num == -1)  { // there is still something left to be contracted in the box
    
    /*========================= update loup ==============================*/

    int box_loup_changed= loup_change(*c);
    
    

    if (! loup_changed) loup_changed=box_loup_changed;

    /*====================================================================*/    


    c->box = space.box;
    c->epx = space.epx;
    
    _nb_cells++; 
    cells.push(c);  // this sub-box is put onto the heap.
  

  } else if (ctc_num == prec_num) { // the box is a "solution"

    REAL lo = Inf(space.box(y_num+1));   // the var y_num is the objective
    if (lo<uplo_of_solutions) uplo_of_solutions = lo;    
  
  }
  
  else {
    delete c;

  }
}

 bool ConstrainedOptimizer::loup_change(Cell& c)
      {

	//	innerbox= c.get<InnerBox>().inner_box; // get the current inner box (to be inflated by update_loup)  ?????

 
	int box_loup_changed = update_loup(sys, space);
	innerfound=false;
	box_loup_changed |= simplex_update_loup(sys);
	if(box_loup_changed){
	  if(Dimension(innerbox)==1) Resize(innerbox, space.nb_var()); 
	  innerbox = loup_point;
    }

	return box_loup_changed;
      }

 bool Optimizer::loup_change(Cell& c)
      {
 	int box_loup_changed = update_loup (sys, space);
	box_loup_changed |= simplex_update_loup(sys);
	return box_loup_changed;
      }



// The schema bisection-contraction is inversed wrt the schema in Paver
// First the bisection is performed and then each subbox is contracted before being inserted in the heap.

int Optimizer::next_box() {
  if (trace) show_stack();
  
  if (!cells.empty()) { 
    Cell& c(*cells.pop());
    contract_and_bound(&c);
  }
  
  while (!cells.empty()) {
    
    if (trace) show_stack();

    Cell& c(*cells.pop());
    loup_changed=false;

    space.box = c.box;
    space.epx = c.epx;
    //    cout << " c " << c.box << endl;
    /*************** bisection ********************/  
    Bisection bis=bsc->bisect(c);

    Cell* c1 = new Cell(bis, false, c, m);
    Cell* c2 = new Cell(bis, true, c, m);

    contract_and_bound(c1);
    contract_and_bound(c2);
    REAL new_uplo;
    if (!cells.empty()) 
      {new_uplo= ((CellHeap&) cells).minimum();}
    if ((cells.empty() && loup != BiasPosInf) || (new_uplo > loup))
           // empty heap : uplo is set to loup - precision if a loup has been found
      {if ((fabs (loup) < 1) || (goal_ceil > 1))  // goal_ceil is a relative precision  and should be less than 1
	  new_uplo=loup-goal_ceil;
	else 
	  new_uplo=loup*(1-goal_ceil);
      }
    if (new_uplo < uplo_of_solutions)
      uplo=new_uplo;
    else
      uplo=uplo_of_solutions;
  
    if (loup_changed) 
      { int prec=cout.precision();
	cout.precision(12);
        // cout << "loup " << loup << endl;
 	cout << "uplo " << uplo << endl;
	cout.precision(prec);}
    manage_cell_buffer();      

    check_limits();

    delete &c;

    if (loup != BiasPosInf) {
      if (((fabs(loup)> 1) && (loup-uplo)/(fabs (loup)) < goal_ceil)
	  || (loup-uplo) <  goal_ceil)
	{throw GoalPrecisionException();}
    }
    

  }
  return -1; // means empty stack.
}


// the heap is updated : in case of a new upperbound (loupchange = true)
// all boxes with a lower bound greater than (loup - precision)  are removed and deleted.
void Optimizer::manage_cell_buffer() {

  if (loup_changed) {
    REAL ymax=loup;

    if ((fabs (loup) < 1) || (goal_ceil > 1))  // goal_ceil is a relative precision  and should be less than 1
      ymax=loup-goal_ceil;
    else 
      ymax=loup*(1-goal_ceil);

    ((CellHeap&) cells).contract_heap(ymax);
  }

}

INTERVAL Optimizer::optimum() const { 
  return INTERVAL(uplo,loup); 
}

VECTOR Optimizer::global_optimizer() const {
  return loup_point;
}


void Optimizer:: check_limits()
{Paver::check_limits();

  if ((space.nb_var() + 10) * ((CellHeap&) cells).size() *2 * sizeof(double) >  3.e8)
    { throw MemoryException();}

}

void Optimizer::report() {
  Paver::report();
  cout << " best bound in: " << optimum() << endl;
  if (loup != BiasPosInf) {
    if ((fabs(loup)> 1) && (loup-uplo)/(fabs (loup)) < goal_ceil)
      cout << " required relative  precision on objective function obtained " << (loup-uplo)/(fabs (loup)) << endl;
    else if (loup-uplo < goal_ceil)
      cout << " required  absolute precision  on objective function obtained " << loup-uplo << endl;
    else if (fabs(loup)> 1)
      cout << " relative  precision on objective function " << (loup-uplo)/(fabs (loup)) << endl;
    else
      cout << " absolute  precision on objective function " << (loup-uplo) << endl;
  }
}
  
} //end namespace
