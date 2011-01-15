/*---------------------------------------------------------------------------------
 * 
 * Q-Intersection contractor 
 * -----------------------------------------------------------------
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

#include "IbexQInter.h"
#include <algorithm>
#include <stack>
#include <iostream>

namespace ibex {

QInter::QInter(Space& space, const ExtendedSymbol& x, const ExtendedSymbol& y, int q) : 
  Operator(space), x(x), _y(y), n(x.dim.size3()), p(y.dim.size2()), q(q) { 

  if (x.dim.dim2>0 || y.dim.dim1>0)  
    throw NonRecoverableException("Cannot use q-intersection with 3D-vectors");
}
   
QInter::QInter(const QInter& q) : Operator(q.space), Contractor(q), x(q.x), _y(q._y), n(q.n), p(q.p), q(q.q) { }
   
void QInter::accept_printer(const OperatorPrinter& op) const {
  op.os << "q-inter(" << x << "," << _y << ")";
}

const INTERVAL& QInter::y(int j, int i) {
  return space.domain(_y.key + j*n + i);
}

bool QInter::y_contains(int j, const VECTOR& m) {
  for (int i=0; i<n; i++) {
    if (! y(j,i).contains(m(i+1))) return false;
  }
  return true;
}

void QInter::set_x(const INTERVAL_VECTOR& b) {
  for (int i=0; i<n; i++) {
    if (! (space.domain(x.key + i) &= b(i+1))) 
      throw EmptyBoxException();
  }
}

void QInter::contract() {

  REAL x[n][2*p];
  int ind[n];     // ind[i] is the current index in x[(d+i)%n]
  INTERVAL_VECTOR cell(n);  // current cell on the grid, to be tested

  for (int i=0; i<n; i++) {
    for (int j=0; j<p; j++) {
      x[i][2*j]   = Inf(y(j,i));
      x[i][2*j+1] = Sup(y(j,i));
    }

    sort(x[i],x[i]+2*p);
  }

  /* An inner box of the hull of the q-intersection.
   * At the end, the inner box coincide with the intersection. */
  INTERVAL_VECTOR inner_box(n);
  inner_box.set_empty();

  /* k is the number of intervals in a dimension i.
   * Variables x[ind[i]] take successively the lower bounds of these intervals,
   * either in the increasing or decreasing order.
   * Hence, the index variable ind[i] varies within [0,k-1]. */
  int k=2*p-1; 


  /* d is the dimension we want to calculate
   * the projection of the q-intersection, i.e.,
   * the first dimension to be swept over. */
  for (int d=0; d<n; d++) {

    // bounds to be determined
    REAL lb0 = d==0? BiasPosInf : Inf(inner_box(d+1)); // start with an upper bound

    /*===================== calculate lower bound =========================== */
    for (int i=0; i<n; i++) {
      ind[i]=0;
      cell(i+1) = INTERVAL(x[i][0],x[i][1]);
    }

    while (ind[0]<k && Inf(cell(d+1))<lb0) {
      //cout << "cell=" << cell << endl;
      // count the number of boxes that contains mid
      int count=0;
      for (int j=0; count<q && j<p; j++) {
	if (y_contains(j,Mid(cell))) count++;
      }
      if (count==q) {
	inner_box |= cell;	// update inner box	
	lb0=x[d][ind[0]];       // equivalent to Inf(cell(d+1))
	break;
      }

      int dim=n-1; // current dimension swept
      while (dim>0 && ind[dim]==k-1) {          // dimension saturated
	ind[dim]=0;                             // restart the dimension
	cell(((d+dim)%n)+1)=INTERVAL(x[(d+dim)%n][0],
				     x[(d+dim)%n][1]); // reinitialize cell
	dim--;                                  // sweep in the lower dimension
      }
      if (dim>=0) {
	ind[dim]++;
	cell(((d+dim)%n)+1)=INTERVAL(x[(d+dim)%n][ind[dim]],
				     x[(d+dim)%n][ind[dim]+1]); // update cell
      }
    }

    //cout << "inner box=" << inner_box << endl;

    if (lb0==BiasPosInf) { 
      inner_box.set_empty();
      break;
    }

    /*===================== calculate upper bound =========================== */

    REAL ub0 = d==0? BiasNegInf : Sup(inner_box(d+1)); // start with a lower bound

    for (int i=0; i<n; i++) {
      ind[i]=k-1;
      cell(i+1) = INTERVAL(x[i][k-1],x[i][k]);
    }

    while (ind[0]>=0 && Sup(cell(d+1))>ub0) {
      //cout << "cell=" << cell << endl;
      // count the number of boxes that contains mid
      int count=0;
      for (int j=0; count<q && j<p; j++) {
	if (y_contains(j,Mid(cell))) count++;
      }
      if (count==q) {
	inner_box |= cell;	// update inner box	
	ub0=x[0][ind[0]+1];
	break;
      }

      int dim=n-1; // current dimension swept
      while (dim>0 && ind[dim]==0) {              // dimension saturated
	ind[dim]=k-1;                             // restart the dimension
	cell(((d+dim)%n)+1)=INTERVAL(x[(d+dim)%n][k-1],
				     x[(d+dim)%n][k]); // reinitialize cell
	dim--;                                    // sweep in the lower dimension
      }
      if (dim>=0) {
	ind[dim]--;
	cell(((d+dim)%n)+1)=INTERVAL(x[(d+dim)%n][ind[dim]],
				     x[(d+dim)%n][ind[dim]+1]); // update cell
      }
    }

    inner_box(d+1)=INTERVAL(lb0,ub0); // useful???

    //cout << "inner box=" << inner_box << endl;
  }

  set_x(inner_box);
}

} // end namespace
