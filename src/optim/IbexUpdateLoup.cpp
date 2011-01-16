/*---------------------------------------------------------------------------------
 * 
 * Contractor that updates the "loup", i.e., that reduces the
 * upper bound of the minimum
 * ----------------------------------------------------------
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

#include "IbexUpdateLoup.h"
#include <stdlib.h>

namespace ibex {

void MonotonicityAnalysis(const Space& space, const Evaluator& goal) {
  
  // MonotonicityAnalysis (quand f est monotone par rapport a une variable x_i
  // l'intervalle [x_i] est remplace par un point (Inf([x_i]) si x_i est croissante
  // ou Sup([x_i]) si x_i es decroissante.
  INTERVAL_VECTOR G(space.nb_var());
  for (int j=0; j<space.nb_var(); j++) {
    G(j+1) = 0.0;
    space.ent(IBEX_VAR,j).deriv = &G(j+1);     
  }
  goal.gradient(space);
    
    
  for (int j=1; j<space.nb_var(); j++) {
    if(Inf(G(j+1))>=0) space.box(j+1)=Inf(space.box(j+1));
    if(Sup(G(j+1))<=0) space.box(j+1)=Sup(space.box(j+1));
  }
}

bool isInner(const System& sys){
    
  bool inner=true;
  INTERVAL_VECTOR eval(sys.nb_ctr());
  sys.eval(eval);
  for (int j=1; j<sys.nb_ctr(); j++) {
    const  Inequality* ineq=dynamic_cast<const Inequality*> (&sys.ctr(j));
	
    if((ineq->op==LEQ && Sup(eval(j+1)) > 0) || (ineq->op==LT && Sup(eval(j+1)) >= 0) ||
       (ineq->op==GEQ && Inf(eval(j+1)) < 0) || (ineq->op==GT && Inf(eval(j+1)) <= 0)){
      return false;
    }
  }
  return true;
}
 

/** see IbexUpdateLoup.h */


bool inHC4(const System& sys, REAL loup){

  bool innerfound=true;
  int goal_var=sys.nb_var()-1;
  INTERVAL y_ini=sys.space.box(goal_var+1);
  sys.space.box(goal_var+1)=INTERVAL(BiasNegInf,loup);

  INTERVAL_VECTOR last_nonemptybox(sys.space.box);
  for (int j=0; j<sys.nb_ctr(); j++) {
    const ArithConstraint& ineq=sys.ctr(j);;
    if(!ineq.feasible(sys.space)){ 
      //nothing to do with the box
      sys.space.box.set_empty();
      return false;
    }
    try{
      ineq.inner_backward(sys.space); 
      last_nonemptybox=sys.space.box;
      // 	  hc4.contract(-1);
    }catch(EmptyBoxException){
      if(j>0) innerfound=false; //the constraint is not the objective
      sys.space.box=last_nonemptybox;
    }
  }

  sys.space.box(goal_var+1)=y_ini;
  return innerfound;
     
}


bool in_HC4=true;
bool mono_analysis=true;

bool update_loup(const System& sys, const Space& space, const Evaluator& goal, Contractor& is_inside, REAL& loup, VECTOR& loup_point, int sample_size) {

  bool loup_changed=false; // the return value
  int n=space.nb_var();
  INTERVAL_VECTOR savebox = space.box;
  bool innerfound=(in_HC4)? inHC4(sys, loup): isInner(sys);

  if(space.box.empty()) //if innerHC4 finds nothing, we try with the full current box
    sys.space.box=savebox;
  else if(innerfound){
    if(mono_analysis)
      MonotonicityAnalysis(space, goal);
  }

  INTERVAL_VECTOR savebox2 = space.box;
  VECTOR mid = Mid(space.box);
  VECTOR diam = Diam(space.box);
  VECTOR pt(n);

  for(int i=0; i<sample_size ; i++){
    // will contain an upper bound of the criterion
    // set each coordinate randomly
    for (int j=1; j<=space.nb_var(); j++) {

      // get a random number in [-0.5,0.5]
      REAL r=rand()/(REAL) RAND_MAX -0.5;
      // pick a point in the domain of the jth variable
      pt(j) = mid(j) + r*diam(j);
      //pt(j)=mid(j);

      // watch dog
      if (pt(j)<Inf(savebox(j))) pt(j)=Inf(savebox(j));
      else if (pt(j)>Sup(savebox(j))) pt(j)=Sup(savebox(j));
    }


    // set the domain to the sample vector 
    (INTERVAL_VECTOR&) space.box = pt;

    goal.forward(space);
    REAL res = Sup(goal.output());

    // check if f(x) is below the "loup" (the current upper bound).
    //
    // The "loup" and the corresponding "loup_point" (the current miminizer) 
    // will be updated if the constraints are satisfied.
    // The test of the constraints is done only when the evaluation of the criterion 
    // is better than the loup (a cheaper test).
    if (_LT(res,loup)) {

      if(isInner(sys)){
	loup = res;
	loup_point = pt;
	loup_changed = true;
	//         cout << loup << endl;
	//============================== debug =============================
	int prec=cout.precision();
	cout.precision(12);
	cout << "loup update " << loup  << " loup point  " << loup_point << endl;
	cout.precision(prec);
	//==================================================================


        (INTERVAL_VECTOR&) space.box = savebox;
        return loup_changed; 
      }
      else if(innerfound && in_HC4) //error 
	cout << "the box returned by inHC4 is not an innerbox!" << endl;
     
	
    }

    (INTERVAL_VECTOR&) space.box = savebox2;
  }
  if (loup_changed) {
    //============================== debug =============================
    int prec=cout.precision();
    cout.precision(12);
    cout << "loup update " << loup  << " loup point  " << loup_point << endl;
    cout.precision(prec);
    //==================================================================
  }

    
  (INTERVAL_VECTOR&) space.box = savebox;
  return loup_changed;
}
    

} // end namespace
