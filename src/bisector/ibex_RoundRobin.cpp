//============================================================================
//                                  I B E X                                   
// File        : ibex_RoundRobin.cpp
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : May 8, 2012
// Last Update : January 8, 2015
//============================================================================

#include "ibex_RoundRobin.h"
#include "ibex_NoBisectableVariableException.h"
using std::pair;

namespace ibex {

RoundRobin::RoundRobin(double prec, double ratio) : Bsc(prec), ratio(ratio) {

}

RoundRobin::RoundRobin(const Vector& prec, double ratio) : Bsc(prec), ratio(ratio) {

}

pair<IntervalVector,IntervalVector> RoundRobin::bisect(const IntervalVector& box, int& last_var) {
  //  cout << " appel var select " << last_var << endl;
 int var = var_select (box, box.size(), last_var);
 //  cout << " rr selected var " << var << endl;
  return box.bisect(var,ratio);
}

int RoundRobin::var_select(const IntervalVector& box, int n, int& last_var)
  {
    if (last_var >=n) last_var = n-1; // cas qui peut arriver quand n (appelé par RoundRobinNvar est plus petit que le nb de var)

  if (last_var == -1) last_var = n-1;

  int var = (last_var+1)%n;


  while (var != last_var && too_small(box,var))
    var = (var + 1)%n;  

  // if no variable can be bisected an exception is thrown
  if (var==last_var && too_small(box,var))
	  throw NoBisectableVariableException();

  last_var = var; // output
  //  std::cout << " var " << var << box[var] << std::endl;
  return var;
}


pair<IntervalVector,IntervalVector> RoundRobin::bisect(const IntervalVector& box) {
	int i=-1;
	return bisect(box,i);
}

pair<IntervalVector,IntervalVector> RoundRobin::bisect(Cell& cell) {
	BisectedVar& v=cell.get<BisectedVar>();
	// the following instruction will update v.var
	// and the new value of v.var will be copied to child nodes
	return bisect(cell.box,v.var);
}
  

pair<IntervalVector,IntervalVector> RoundRobinNvar::bisect(const IntervalVector& box, int& last_var) {

  //  cout << " appel bissect " << endl;
  int var;
  try {var= var_select (box, nbvars, last_var);}
  catch (NoBisectableVariableException& e) { return RoundRobin::bisect (box, last_var);}
  //  cout << " fin bissect " << var <<  endl;
  return box.bisect(var,ratio);
  
}


void RoundRobin::add_backtrackable(Cell& root) {
	root.add<BisectedVar>();
}


  /*
pair<IntervalVector,IntervalVector> RoundRobinQInter::bisect(Cell& cell) {
  BisectedVar& v=cell.get<BisectedVar>();
  std::cout << " varbis " << cell.var_biss << " small " << too_small(cell.box,cell.var_biss) << std::endl;
  if (cell.var_biss ==-1  || too_small(cell.box,cell.var_biss))
    {

	// the following instruction will update v.var
	// and the new value of v.var will be copied to child nodes
	return RoundRobin::bisect(cell.box,v.var);
    }
  else
    {

      int var = cell.var_biss;
      std::cout << " var " << var << std::endl; 
      //   v.var=var;  on ne met pas a jour get<BisectedVar> pour ne pas biaiser le roundrobin
      //  cout << " var " << var << "  inter " << cell.box[var] << "pt " << cell.val_biss << endl;
      //      double ratio= (cell.val_biss-cell.box[var].lb())/cell.box[var].diam();
      //   if (ratio <0.2 || ratio >0.7) 
      double ratio=0.5;
      return cell.box.bisect(var, ratio);
    }
  
}
  */

} // end namespace ibex
