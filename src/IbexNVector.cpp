/*---------------------------------------------------------------------------------
 * 
 * Constraints on the number of Distinct Vectors
 * ----------------------------------------------
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

#include <algorithm>
#include "IbexNVector.h"
#include "IbexExprAdjFactory.h"
#include "IbexEvaluatorFactory.h"

namespace ibex {

AtMostNVector::AtMostNVector(const vector<const Expr*>& expr, int N) : N(N), dim((*expr.begin())->dim), nb(expr.size()) {
    
  ExprAdjFactory(expr, adj);

  _expr = new const Expr*[nb];
  evl = new Evaluator[nb];
  kernels = new INTERVAL_VECTOR[dim.size()];
  nb_kernels = new int[dim.size()];
  domains = new Component[nb];

  for (int i=0; i<dim.size(); i++) {
    Resize(kernels[i],nb);
  }

  int i=0;

  for (vector<const Expr*>::const_iterator it=expr.begin(); it!=expr.end(); it++) {
    if (!((*it)->dim==dim)) {
      throw NonRecoverableException("Cannot use \"atmost-nvalue\" with expressions of different dimensions");
    }
    _expr[i] = *it;
    EvaluatorFactory fac(**it);
    fac.build(&evl[i]);
    i++;
  }
}

AtMostNVector::~AtMostNVector() {
  delete[] domains;
  delete[] nb_kernels;
  delete[] kernels;
  delete[] evl;
  delete[] _expr;
}

/*
 * input:        nb_intervals - the number of intervals constrained to share not more than N values
 * input-output: intervals - the array of [nb_intervals] pointers to the intervals
 *                           the pointers are reordered so that i<=j => lb(*intervals[i])<=lb(*intervals[j])
 *               M - the matching matrix (M[i][j]=1 iff *intervals[i] and *intervals[j] shares the same value
 * output:       kernel - the vector of ("recomputed") kernels.
 *               nb_kernels - the number of kernels 
 *
 * returns: true if a new association has been made between two intervals
 */
bool AtMostNVector::forward(Component* intervals, int nb_intervals, INTERVAL_VECTOR& kernels, int& nb_kernels, INTERVAL_MATRIX& M) const {

  /* Sort components by increasing lower bound */
  sort(intervals, intervals + nb_intervals);  

  int start_previous_group=0;
  REAL up=BiasPosInf;

  // for each kernel, this array stores the indexes of the n matched intervals. 
  // each kernel is ended by a "-1". Example:
  //
  // match:    [3 2 6 x x x x x x -1 x x x x x x x x x x x  -1 ...   ]
  //            <-- kernel #1 --->   <---- kernel #2 ----->  ...
  // which means that kernel 1 contains intervals 3, 2, 6, etc. and so on.
  int match[nb_intervals][nb_intervals]; 

  bool new_assoc = false;

  INTERVAL dom_n(BiasPosInf,BiasPosInf);

  nb_kernels=0; // kernel counter

  for (int i=0; i<=nb_intervals; i++) {
    INTERVAL& Ui = i<nb_intervals? intervals[i] : dom_n;

    if (Inf(Ui)>up) { // new group
      REAL kinf = BiasNegInf;
      REAL ksup = BiasPosInf;

      int m=0; // matching counter - start a new matching sequence
      for (int j=start_previous_group; j<i; j++) {
	INTERVAL& Uj = intervals[j];
	if (Sup(Uj)<Inf(Ui)) {
	  match[nb_kernels][m++]=intervals[j].index;  // the jth interval is included in the sequence
	  if (Inf(Uj)>kinf) kinf=Inf(Uj);
	  if (Sup(Uj)<ksup) ksup=Sup(Uj);	  
	}
      }

      match[nb_kernels][m]=-1;

      if (nb_kernels==N) throw EmptyBoxException(); // too many distinct values

      kernels(++nb_kernels) = INTERVAL(kinf,ksup);

      start_previous_group = i;

      if (i<nb_intervals) up = Sup(intervals[i]);
    } else { // still in the same group
      if (Sup(Ui)<up) up = Sup(Ui);
    }
  }

  if (nb_kernels==N) {
    for (int i=0; i<nb_intervals; i++) {
      INTERVAL& Ui = intervals[i];
      INTERVAL newUi = INTERVAL::EMPTY;
      for (int k=0; k<nb_kernels; k++) {
	newUi |= (Ui & kernels(k+1));
      }
      Ui = newUi;
    }

    for (int k=0; k<nb_kernels; k++) {      
      for (int m2=0; match[k][m2]!=-1; m2++) {
	for (int m3=m2+1; match[k][m3]!=-1; m3++) {	  
	  int i=match[k][m2];
	  int j=match[k][m3];
	  if (M(i+1,j+1).contains(0)) new_assoc = true;
	  if ( !(M(i+1,j+1) &= INTERVAL(1,1))) throw EmptyBoxException();
	  if ( !(M(j+1,i+1) &= INTERVAL(1,1))) throw EmptyBoxException();
	  /* The domains of the ith and jth objects have to be synchronized on every dimension */
	  if (dim.scalar()) {
	    if (!(evl[i].output() &= evl[j].output())) throw EmptyBoxException();
	    evl[j].output() &= evl[i].output();
	  } else {
	    if (!(evl[i].m_output() &= evl[j].m_output())) throw EmptyBoxException();
	    evl[j].m_output() &= evl[i].m_output();
	  }
	}
      }
    }
  }
  /*--------------------------------------------------------------------*/

  return new_assoc; // true => at least one additional matching has been done
}


void AtMostNVector::forward(const Space& space) const {
  for (int i=0; i<nb; i++)
    evl[i].forward(space);

  INTERVAL_MATRIX M(nb,nb);
  Initialize(M,INTERVAL(0,1)); // initialliy, all boxes may match

  int total_dim = dim.size2() * dim.size3();
  int last_improv_dim = 0; // the last dimension where a new matching has been done

  int d=0;  // global counter for components
  int d2=0; // counter for rows (only for matrices)
  int d3=0; // counter for columns (only for matrices & vectors)
  
  do {
    for (int i=0; i<nb; i++) { // the loop is executed only once if dim.scalar()==true
      if (dim.scalar())
	domains[i].x = &evl[i].output();  // here, d=0
      else
	domains[i].x = &(evl[i].m_output()(d2+1,d3+1));	
      domains[i].index = i;
    }
    
    if (forward(domains, nb, kernels[d], nb_kernels[d], M)) {
      last_improv_dim = d;
    }
    
    d = (d+1) % total_dim;
    d3=(d3+1) % dim.size3();
    if (d3==0) d2=(d2+1) % dim.size2();
  } while (d!=last_improv_dim); // fixpoint when no additional matching could be done
}
  
  /*--------------- recomputing kernels ------------------*/
  /* ===> already done! (in forward) */
  /*
  if (nb_kernels==N) {
    int k=1;             // current "recomputed" kernel
    INTERVAL new_kernel; // each time, the new kernel
    bool kernel_init;    // true until the new kernel gets initialized (first intersection)

    kernel_init = true;

    for (int i=0; i<nb; i++) {
      const INTERVAL& Ui=evl[i].output();

      // does Ui intersect only the k^th kernel? 
      if (! (Ui & kernel(k)).empty()) {   // yes if Ui intersects this kernel
	if (k==N ||                       // and there is no other kernel after
	    (Ui & kernel(k+1)).empty()) { // or Ui does not intersect the next one
	  if (kernel_init) {              // is Ui the first one like this ?
	    new_kernel = Ui;
	    kernel_init = false;
	  }
	  else {
	    new_kernel &= Ui;
	  }
	}
      }
      else {                              // kernel(k) is done
	kernel(k++) = new_kernel;
	kernel_init = true;
	i--;                              // reconsider the current interval (now that k has been incremented)
      }
    } 
  */


void AtMostNVector::backward(Space& space) const {

  bool possible=false; // is contraction possible?

  /*--------------- intersecting intervals & backward eval  ------------------*/
  int d=0;
  for (int d2=0; d2<dim.size2(); d2++) {
    for (int d3=0; d3<dim.size3(); d3++) {
      if (nb_kernels[d] == N) {   
	possible=true;
      }
      d++;	  
    }    
  }
  
  if (possible) 
    for (int i=0; i<nb; i++) {
      evl[i].backward(space);
    }
}  

} // end namespace
