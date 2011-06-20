/*---------------------------------------------------------------------------------
 * 

 * Bisectors: base class and some standard implementations 
 * -------------------------------------------------------
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

#include "IbexBisector.h"

#define __IBEX_DEFAULT_PRECISION  1e-07
#define __IBEX_DEFAULT_RATIO_BISS 0.45

namespace ibex {

Bisection Bisector::bisect(Cell& cell) {
  this->cell = &cell;
  cell.space.box = cell.get_box();
  this->space.sync(cell.space);

  Bisection b=bisect();

  this->cell = NULL;

  if (&cell.space!=&this->space) {
    int key = this->space.key(IBEX_VAR, b.var);

    if (cell.space.entity(key).type!=IBEX_VAR) 
      throw NonRecoverableException(string("Cannot bisect ")+space.env.symbol_name(key)+", which is not a variable");
    
    return Bisection(cell.get_box(), cell.space.component(key), b.ratio);
  }
  else {
    return b;
  }
}

/*--------------------------------------------------------------------------------*/

Bisection RoundRobin::bisect(int last_var) const {

  INTERVAL_VECTOR& box(space.box);

  int n = Dimension(box);

  if (last_var == -1) last_var = n-1;
  
  int var = (last_var+1)%n;

  while (var != last_var && Diam(box(var+1)) < w) var = (var + 1)%n;

//   while (var != last_var && Diam(box(var+1)) < w) var = (var + 1)%n;

  if (var==last_var) var = (last_var+1)%n;

  return Bisection(box, var, ratio);
}

Bisection LargestFirst::bisect() {

  INTERVAL_VECTOR& box(space.box);

  int var = 0;

  for (int i=0; i<Dimension(box); i++)
    if (Diam(box(i+1))>Diam(box(var+1))) var = i;    

  return Bisection(box, var, ratio);
}




Bisection SmearFunction::bisect(int last_var) const {

  // rem: sys space is already synchronized.
  INTERVAL_MATRIX J(sys.nb_ctr(), sys.nb_var());
  try {

    sys.jacobian(J);

  } catch(EvalException) {
    return RoundRobin::bisect(last_var);
  }
  int var = var_to_bisect (J);  // call the var_to_bisect 
  /*
// to not call the last variable
  if (var == last_var)
    return Bisection(sys.box, random()% sys.nb_var(),ratio);
  */
  if (var == -1)// throw BoxTooSmallException();
    return RoundRobin::bisect(last_var);
  else
    return Bisection(sys.box, var, ratio);

}


int SmearMax::var_to_bisect (INTERVAL_MATRIX& J) const { 
  REAL max_magn = BiasNegInf;
  int var=-1;
  for (int j=0; j<nbvars; j++) {
    if (Diam(sys.box(j+1))>= w)
      {
	for (int i=0; i<sys.nb_ctr(); i++)
	  if ( Abs(J(i+1,j+1)) * Diam(sys.box(j+1)) > max_magn )
	    {
             max_magn = Abs(J(i+1,j+1))* Diam(sys.box(j+1));
	      var = j;
	    }
      }
  }
  //  cout << " var " << var << endl;
  return var;
}


int SmearSum::var_to_bisect(INTERVAL_MATRIX& J) const {
    // computes the variable with the maximal sum of impacts
  REAL max_magn = BiasNegInf;
  int var = -1;

  for (int j=0; j<nbvars; j++) {
    if (Diam(sys.box(j+1))>= w) 
      {
	double sum_smear=0;
	for (int i=0; i<sys.nb_ctr(); i++) 
	  sum_smear+= Abs(J(i+1,j+1)) * Diam(sys.box(j+1)); 
	if (sum_smear > max_magn)
	  { max_magn = sum_smear;
	    var = j;
	  }
      }
  }
  return var;
}


int SmearSumRelative::var_to_bisect(INTERVAL_MATRIX & J) const {

  REAL max_magn = BiasNegInf;
  int var = -1;
  // the normalizing factor per constraint
  REAL ctrjsum[sys.nb_ctr()];
  for (int i=0; i<sys.nb_ctr(); i++) 
    {ctrjsum[i]=0;
      for (int j=0; j<nbvars ; j++)
	{ctrjsum[i]+= Abs(J(i+1,j+1)) * Diam(sys.box(j+1));
	}
    }
      // computes the variable with the maximal sum of normalized impacts
  for (int j=0; j<nbvars; j++) {
    if (Diam(sys.box(j+1))>= w) 
      {
	double sum_smear=0;
	for (int i=0; i<sys.nb_ctr(); i++) 
	  {if (ctrjsum[i]!=0)
	      sum_smear+= Abs(J(i+1,j+1)) * Diam(sys.box(j+1)) / ctrjsum[i]; 
	  }
	if (sum_smear > max_magn)
	  { max_magn = sum_smear;
	    var = j;
	  }
      }
  }
  return var;
}




int SmearMaxRelative::var_to_bisect(INTERVAL_MATRIX & J) const {

  REAL max_magn = BiasNegInf;
  int var = -1;

  
  REAL ctrjsum[sys.nb_ctr()]; // the normalizing factor per constraint
  for (int i=0; i<sys.nb_ctr(); i++) 
    {ctrjsum[i]=0;
      for (int j=0; j<nbvars ; j++)
	{ctrjsum[i]+= Abs(J(i+1,j+1)) * Diam(sys.box(j+1));
	}
    } 

  // computes the variable with the greatest normalized impact
  double maxsmear=0;
  for (int j=0; j<nbvars; j++) {
    if (Diam(sys.box(j+1))>= w) 

	for (int i=0; i<sys.nb_ctr(); i++) 
	  {
	    if (ctrjsum[i]!=0)
	      maxsmear = Abs(J(i+1,j+1)) * Diam(sys.box(j+1)) / ctrjsum[i]; 
	    if (maxsmear > max_magn)
	      { max_magn = maxsmear;
		var = j;
	      }
	  }

  }
  return var;
}


Bisection BlockRoundRobin::bisect(int last_var) const throw(BlockSetException) {

  INTERVAL_VECTOR& box(space.box);

  //int n = Dimension(box);
  int blk;
  int var;
  
  //  cout << "\n\n last-var=" << last_var << " nb-blk=" << bs.nb_blk << endl;
  
  if (last_var == -1) {
    blk = 0;
    while (blk<bs.nb_blk && bs.empty(blk)) blk++;
    if (blk==bs.nb_blk) throw BlockSetException(BlockSetException::ALL_EMPTY);
    var = bs.first(blk);
    last_var = bs.last(blk);
  } else {
    blk = bs.blk(last_var);  // may throw NO_BLK exception
    var = bs.next(last_var);
  }

  while (blk<bs.nb_blk) {
    //    cout << "try block " << blk << endl;
    // iterate within a block
    while (var != last_var) {
      if (Diam(box(var+1)) >= w) return Bisection(box, var, ratio);  
      var = bs.next(var);
    } 
   
    if (Diam(box(var+1)) >= w) return Bisection(box, var, ratio);  

    //find another block
    blk++;
    while (blk<bs.nb_blk && bs.empty(blk)) blk++;
    if (blk<bs.nb_blk) {
      var = bs.first(blk);
      last_var = bs.last(blk);
    }   
  }

  blk = 0;

  while (blk<bs.nb_blk && bs.empty(blk)) blk++;
  return Bisection(box, bs.first(blk), ratio);
 
}

const REAL Bisector::default_prec = __IBEX_DEFAULT_PRECISION;
const REAL Bisector::default_ratio = __IBEX_DEFAULT_RATIO_BISS;

} // end namespace
