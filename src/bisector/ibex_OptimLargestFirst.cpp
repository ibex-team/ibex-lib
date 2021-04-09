//============================================================================
//                                  I B E X                                   
// File        : ibex_OptimLargestFirst.cpp
// Author      : Bertrand Neveu, Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Dec 3, 2018
// Last Update : Apr 9, 2021
//============================================================================
#include "float.h"
#include "ibex_OptimLargestFirst.h"
#include "ibex_NoBisectableVariableException.h"

using namespace std;

namespace ibex {

  double objectivebisect_ratiolimit=1.e10;

  OptimLargestFirst::OptimLargestFirst(int goal_var,bool choose_obj, double prec,  double ratio) : LargestFirst(prec, ratio), goal_var(goal_var), choose_obj(choose_obj)  {
}

  OptimLargestFirst::OptimLargestFirst(int goal_var,bool choose_obj,const Vector& prec,double ratio) :LargestFirst(prec, ratio), goal_var(goal_var), choose_obj(choose_obj) {

}


BisectionPoint OptimLargestFirst::choose_var(const Cell& cell) {
const IntervalVector& box=cell.box;
	int var =-1;
	double l=0.0;
	for (int i=0; i< box.size(); i++){
	  if (i!= goal_var){
	    if ( ! nobisectable (box,i)){
	      if (var==-1) {
		var=i;
		l = uniform_prec()? box[i].diam() : (box[i].diam()/prec(i));
	      }
	      else {
		double l_tmp = uniform_prec()? box[i].diam() : (box[i].diam()/prec(i));
		if (l_tmp>l) {
		  var = i;
		  l = l_tmp;
		}
	      }
	    }
	  }
	}
	// special conditions for choosing the objective as variable to bisect	
  if ((choose_obj == true)
      &&  !(nobisectable (box,goal_var))
      && (l < box[goal_var].diam())
      && box[goal_var].diam()/l < objectivebisect_ratiolimit)
    var=goal_var;
  //  cout << " bisected var " << var  << " l " << l << endl ;
  if (var !=-1){
    return BisectionPoint(var,ratio,true);
  }
  else {
    throw NoBisectableVariableException();
  }

}
  

} // end namespace ibex
