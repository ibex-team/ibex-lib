/*---------------------------------------------------------------------------------
 * 
 * Contractor that updates the "loup", i.e., that reduces the
 * upper bound of the minimum
 * ----------------------------------------------------------
 *
 * Copyright (C) 2007-2010 Gilles Chabert
 * 
 * This file is part Sof IBEX.
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

#include "IbexOptimizer.h"
#include <stdlib.h>
#include <math.h> // for fabs

namespace ibex {


/* last update: IAR  */
  void Optimizer::monotonicity_analysis(const Space& space, const Evaluator& goal) {
  
  // monotonicity_analysis (quand f est monotone par rapport a une variable x_i
  // l'intervalle [x_i] est remplacé par un point (Inf([x_i]) si x_i est croissante
  // ou Sup([x_i]) si x_i est décroissante.
  INTERVAL_VECTOR G(space.nb_var());
  for (int j=0; j<space.nb_var(); j++) {
    G(j+1) = 0.0;
    space.ent(IBEX_VAR,j).deriv = &G(j+1);     
  }
  try{
    goal.gradient(space);
    
    
  for (int j=0; j<space.nb_var()-1; j++) {
    if(Inf(G(j+1))>=0) space.box(j+1)=Inf(space.box(j+1));
    if(Sup(G(j+1))<=0) space.box(j+1)=Sup(space.box(j+1));
  }
  }
  catch (UnboundedResultException e)
    {;}
}

  bool ConstrainedOptimizer::isInner(const System& sys, const Space& space, int j){
        const  Inequality* ineq=dynamic_cast<const Inequality*> (&sys.ctr(j));
	//	ineq->forward(space);
        INTERVAL eval=ineq->eval(space);
        if((ineq->op==LEQ && Sup(eval) > 0) || (ineq->op==LT && Sup(eval) >= 0) ||
	    (ineq->op==GEQ && Inf(eval) < 0) || (ineq->op==GT && Inf(eval) <= 0)){
            
	    return false;
	}
     return true;
  }

  // goal is the first constraint -> start with j=1
  bool ConstrainedOptimizer::isInner(const System& sys, const Space& space){
      for (int j=1; j<sys.nb_ctr(); j++)
        if(!isInner(sys,space,j)) return false;
     return true;
  }

/* last update: IAR  */
  bool ConstrainedOptimizer::inHC4(const System& sys, REAL loup){

  bool innerfound=true;
  INTERVAL y_ini=sys.space.box(y_num+1);
  sys.space.box(y_num+1)=INTERVAL(BiasNegInf,loup);

  INTERVAL_VECTOR last_nonemptybox(sys.space.box);
  // goal is the first constraint -> start with j=1
  for (int j=1; j<sys.nb_ctr(); j++) {
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
      innerfound=false; 
      sys.space.box=last_nonemptybox;
    }
  }

  sys.space.box(y_num+1)=y_ini;
  return innerfound;
     
}

/* added by: IAR  */
/*
void inHC4_expand(const System& sys, INTERVAL_VECTOR& inner_box){
  INTERVAL_VECTOR savebox=sys.space.box;
  sys.space.box=inner_box;
//   assert(isInner(sys));
  sys.space.box=savebox;
  for (int j=1; j<sys.nb_ctr(); j++) {
    const ArithConstraint& ineq=sys.ctr(j);;
    try{
      INTERVAL_VECTOR lastbox=sys.space.box;
      ineq.expand(sys.space,inner_box); 
//       assert (sys.space.box<=lastbox);
    }catch(EmptyBoxException){
      cout << "error!" << endl;
    }
  }
  inner_box=sys.space.box;
//   assert(isInner(sys));
  sys.space.box=savebox; 

//   cout << "expanded:" << inner_box << endl;
}

*/

  bool in_HC4=true;
 bool mono_analysis=true;
  //  bool in_HC4=false;
  //  bool mono_analysis=false;
  

/** Try to reduce the loup with the candidate point "pt".
 * Return true in case of success.
 * The function does not modify space.box.
 * last update: GCH  */
  bool Optimizer::check_candidate(const System& sys, const Space& space, const VECTOR& pt) {
  // save the box
  INTERVAL_VECTOR savebox = space.box;  
  // set the domain to the sample vector 
  (INTERVAL_VECTOR&)  space.box = pt;

  bool loup_found=false;

 try{
  goal.forward(space);
 }catch(EmptyBoxException){

   (INTERVAL_VECTOR&) space.box = savebox;
   return false;
 }

  // "res" will contain an upper bound of the criterion
  REAL res = Sup(goal.output());

  // check if f(x) is below the "loup" (the current upper bound).
  //
  // The "loup" and the corresponding "loup_point" (the current miminizer) 
  // will be updated if the constraints are satisfied.
  // The test of the constraints is done only when the evaluation of the criterion 
  // is better than the loup (a cheaper test).

  //  cout << " res " <<  res << " loup " << loup << endl;
  if (_LT(res,loup)) {
 
    if (check_constraints (sys,  space))
      {loup = res;
       loup_point = pt;
	loup_point(y_num+1)=loup;
	loup_found = true;
      }
  }
  
  (INTERVAL_VECTOR&) space.box = savebox;
  
  return loup_found;
  }


/**  Constraint checking : for unconstrained optimization  return true */
bool Optimizer::check_constraints (const System& sys, const Space& space){return true;}


/**  Constraint checking : return true if the current box is an innerbox (constraints are satisfied)
 * This function is called by check_candidate with the box (space) reduced to a point 
* The param "innerfound" has been set to true if the box has 
* already been proven to be inner (avoid unecessary tests)
*/
bool ConstrainedOptimizer::check_constraints (const System& sys, const Space& space){
  bool constraints_ok=false;
    // to apply [check1] comment next line
    //       if (!innerfound) {
    try {
//       is_inside.contract();
      if(!isInner(sys,space)){ //the is_inside contractor doesn't refute thing like 2>2    ??? BNE
	if(innerfound && in_HC4) //error 
	  cout << "the box returned by inHC4 is not an innerbox!" << endl; 
        constraints_ok= false;  
	innerfound=false;

      }
      else 
	constraints_ok= true; 
      
    } 

    catch(EmptyBoxException) {
        constraints_ok = true; // local assignment (valid for pt only)  ??? encore utile sans is_inside ???  BNE
    }

    return constraints_ok;
  }



/** Return a random point inside a box.
 *
 * last update: GCH
 */

VECTOR random_point(const INTERVAL_VECTOR& box) {
  int n=Dimension(box);
  VECTOR mid = Mid(box);
  VECTOR diam = Diam(box);
  VECTOR pt(n);

  for (int j=1; j<=n; j++) {    
    // get a random number in [-0.5,0.5]
    REAL r=rand()/(REAL) RAND_MAX -0.5;
    // pick a point in the domain of the jth variable
    pt(j) = mid(j) + r*diam(j);
      // watch dog
    if (pt(j)<Inf(box(j))) pt(j)=Inf(box(j));
    else if (pt(j)>Sup(box(j))) pt(j)=Sup(box(j));
  }
  return pt;
}


/* 1st method for probing: take random points in any directions
 *
 * return true if the loup has been modified 
 *
 * last update: GCH 
 */

  bool Optimizer::random_probing (const System& sys, const Space& space) {
  VECTOR pt(space.nb_var());
  bool loup_changed=false;
  for(int i=0; i<sample_size ; i++){
    pt =   random_point(space.box);
    loup_changed |= check_candidate (sys, space, pt);
  }

  return loup_changed;
  }



/* 2nd method for probing: pick equidistant points in the opposite direction of the 
 * gradient calculated at a starting point "start".
 *
 * This function is simpler than a "gradient descent" since the step is calculated
 * by the simple geometrical rule explained below (we do not use rules like Wolfe's criterion, etc.)
 *
 * This function can be called in two different modes:
 * - recursive=false: all the points are taken on the same half-line, starting from "start"
 *                    and ending at a facet of the box.
 * - recursive=true:  as soon as the "loup" is modified, a new gradient is calculated and the 
 *                    line changes accordingly. This variant is more similar to a gradient descent.
 *
 * return true if the loup has been modified
 *
 * last update: GCH
 */
  bool Optimizer::line_probing(const System& sys, const Space& space, const VECTOR& start, int sample_size, bool recursive) {
  int n=space.nb_var();
  VECTOR diam = Diam(space.box);

  VECTOR pt(n);
  bool loup_changed=false;

  INTERVAL_VECTOR savebox = space.box;  

  // --------------------------------------------------
  // Calculates the gradient of f at the startpoint of the box (once for all)
  // --------------------------------------------------
  INTERVAL_VECTOR iG(n);            // interval gradient

  ((Space&) space).box = start;     // take the startpoint of the box

  for (int j=0; j<n; j++) {
    iG(j+1) = 0.0;
    space.ent(IBEX_VAR,j).deriv = &iG(j+1);     
  }
  try{
  goal.gradient(space);

  ((Space&) space).box = savebox;   // restore domains

  VECTOR G=Mid(iG);                 // same as iG, but real-typed

  // --------------------------------------------------
  // Build the (signed) distance vector. This vector gives 
  // in each direction, the distance between the starting
  // point and the extreme facet pointed by -G 
  // (the opposite of the gradient, that is, the facet
  // successive candidate points will move to) 
  // --------------------------------------------------
  VECTOR dist(n); // the distance to reach the facet along the j axis
  for (int j=1; j<=n; j++) {
    // if start is midpoint, dist is equal to +/- rad(j+1) !
    // ----------------------------
    // NOTE : dist(j)<0 <=> G(j)>0    
    // ----------------------------
    if (G(j)>0) dist(j)=Inf(space.box(j))-start(j);
    else dist(j)=Sup(space.box(j))-start(j);
  }

  // --------------------------------------------------
  // Finds the "main axis", that is the coordinate along
  // which the descent direction is maximal with respect 
  // to the corresponding distance.
  // --------------------------------------------------
  REAL max_ratio=0; // the maximal ratio 
  int mj=-1;         // the index of the "main direction"
  for (int j=0; j<n; j++) {

    if (dist(j+1)==0) continue;   

    REAL r=-G(j+1)/dist(j+1); // space.box is bounded (precondition) => 0<|dist|<inf

    if (r<0) {  // r is necessarily >=0
      cout << "[gch] bug: negative ratio.\n";
      exit(-1);
    }

    if (r > max_ratio) {
      mj=j;
      max_ratio = r;
    }
  }

  if (mj==-1) {
    // There is a problem: either the inner box is a single point or
    // the gradient is null in all the non-degenerated directions
    //cout << "warning: no gradient descent possible" << endl;
    // Use random probing (by default)
    return random_probing(sys, space);
  }

  // max_coeff determines the point where the half-line starting 
  // from the startpoint and directed by -G gets "out of" the box. 
  // The (mj)th coordinate of this point is 
  //      start(mj+1)-max_coeff(mj+1)*G(mj+1)
  // and either belongs to the facet x_{mj}^+ or x_{mj}^-, 
  // (mj is the "main axis")

  REAL max_coeff = -dist(mj+1)/G(mj+1);

  /*-------------- debug ------------------*/
//    for (int j=1; j<=n; j++) {  
//      if (diam(j)==0) continue;
//      cout << " j=" << j << " " << Inf(space.box(j)) << " " << start(j)-max_coeff*fabs(G(j)) << "  " << start(j) << "  " << start(j)+max_coeff*fabs(G(j)) << "  " << Sup(space.box(j)) << endl;
//    }
  /*---------------------------------------*/
    

  for(int i=0; i<sample_size ; i++){

    // pick a point in the direction -G  
    for (int j=1; j<=n; j++) {

      if (dist(j)==0) 
	pt(j) = start(j);
      else 
	// pick a point in the gradient-directed 
	// half-line emanating from the startpoint	
	pt(j) = start(j) - (i*max_coeff/((double) sample_size))*G(j);

      // watch dog
      if (pt(j)<Inf(space.box(j))) {
	cout << "watchdog! j=" << j << endl;
	pt(j)=Inf(space.box(j));
      } else if (pt(j)>Sup(space.box(j))) {
	cout << "watchdog! j=" << j << endl;
	pt(j)=Sup(space.box(j));
      }
    }
    
    if (recursive) {
      if (check_candidate(sys, space,  pt))  {
	if (i<sample_size-1) 
	  line_probing(sys, space,  pt, sample_size-i-1, true);
	return true;
      }
    }
    else {
      loup_changed |= check_candidate(sys, space, pt);
    }

  }
  
  return loup_changed;
  }
  catch (UnboundedResultException e) {
   return random_probing(sys, space);}
}



/* ====================== 3nd method: make a real gradient descent ===============================
 * TO DO........... 
 * It probably requires Hessian matrix.
 */

/* Main procedure
 *
 * warning: space.box must be *bounded*
 *
 * last update: IAZ
 */
  bool ConstrainedOptimizer:: update_loup(const System& sys, const Space& space) {

  bool loup_changed=false; // the return value
  INTERVAL_VECTOR savebox = space.box;

  innerfound=false;
  if (in_HC4) {
    innerfound=inHC4(sys, loup);
    
  }  
  else 
    try {
      is_inside.contract();
      innerfound=false;
    } catch(EmptyBoxException) {
      Resize(innerbox, space.nb_var());
      innerbox = space.box;
      innerfound=true;
    }

  if(innerfound) {//the current inner_box is updated
        if(Dimension(innerbox)==1) Resize(innerbox, space.nb_var()); //first innerbox found!
        innerbox = space.box;
      }

  if(space.box.empty()){ //if innerHC4 finds nothing, we try with the full current box
    sys.space.box=savebox; // restore the full box
//   //=============the innerbox expansion==============
//     if(!innerfound && !inner_box.empty()){
//         inner_box(sys.nb_var())=INTERVAL(-1e8,1e8);
//         if(inner_box&=sys.space.box){
//           inHC4_expand(sys, inner_box);
//           sys.space.box=inner_box;
//           innerfound=true;
//         }else 
//           inner_box.set_empty();
//    }
//    //=================================================

  }else if(innerfound){
    if(mono_analysis)
      monotonicity_analysis(space, goal);
  }
  //  if (innerfound)
    // first option: startpoint = midpoint
    //
  //  loup_changed = line_probing(sys, space,   Mid(space.box), 3*sample_size, true);
  // other option: chose startpoint randomly
  //  loup_changed = line_probing(sys, space,  random_point(space.box), 5* sample_size, true);

  //  else
  //    if (!loup_changed)
   loup_changed = random_probing(sys, space);

   if (loup_changed) { trace_loup();
     if (innerfound)
       {
	 nb_inhc4++;
	 diam_inhc4 = ((nb_inhc4-1) *diam_inhc4 + Max(Diam(savebox))) / nb_inhc4;}

     else 
       {
	 nb_rand++;
	 diam_rand = ((nb_rand-1) *diam_rand + Max(Diam(savebox))) / nb_rand;}
   }
  (INTERVAL_VECTOR&) space.box = savebox;
  return loup_changed;
  }


  /** Main procedure for unconstrained optimization 
   */

  bool Optimizer::update_loup(const System& sys, const Space& space) {

  bool loup_changed=false; // the return value
  INTERVAL_VECTOR savebox = space.box;

  if(mono_analysis)
    monotonicity_analysis(space, goal);

    // first option: startpoint = midpoint
    //
  //     loup_changed = line_probing(sys, space,  Mid(space.box), 5* sample_size, true);
  // other option: chose startpoint randomly
  //  loup_changed = line_probing(sys, space,  random_point(space.box), 5* sample_size,  true);

  //  else
  loup_changed = random_probing (sys, space);

     if (loup_changed) trace_loup();
    
  (INTERVAL_VECTOR&) space.box = savebox;
  return loup_changed;
}

  void Optimizer::trace_loup ()
{
    int prec=cout.precision();
    cout.precision(12);
    cout  << " loup update " << loup  << " loup point  " << loup_point << endl;
    cout.precision(prec);
  }


  void ConstrainedOptimizer::trace_loup ()
{
    int prec=cout.precision();
    cout.precision(12);
    cout << (innerfound? "[in!]" : "") << " loup update " << loup  << " loup point  " << loup_point << endl;
    cout.precision(prec);
  }
    

} // end namespace
