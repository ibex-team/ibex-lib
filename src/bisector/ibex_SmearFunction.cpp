//============================================================================
//                               I B E X                                   
// File        : ibex_SmearFunction.cpp
// Author      : Bertrand Neveu
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : July 19 2012
// Last Update : May 9 2019
//============================================================================

#include "ibex_SmearFunction.h"
#include "ibex_ExtendedSystem.h"

using namespace std;

namespace ibex {


  void SmearFunction::add_property(const IntervalVector& init_box, BoxProperties& map) {
	lf->add_property(init_box, map);
  }


  SmearFunction::~SmearFunction() {if (lftodelete) delete lf;}


  // used  in optimization with an ExtendedSystem sys ; returns the index of the goal ctr in sys
  // if sys is not an Extended system , returns -1
  int  SmearFunction::goal_ctr() const {
    if (dynamic_cast<ExtendedSystem*> (&sys)) {
      return dynamic_cast<ExtendedSystem*> (&sys)->goal_ctr();
    }
    else  return -1; 
  }
   // used  in optim plugin by LSmear bisector
  int  SmearFunction::goal_var() const {
    if (dynamic_cast<ExtendedSystem*> (&sys)) {
      return dynamic_cast<ExtendedSystem*> (&sys)->goal_var();
    }
    else  return -1; 
  }
  
  // Used in optimization to not consider an inactive constraint 
  // returns true if it is not an extended system , the constraint is inactive or it is the objective 
  bool SmearFunction::constraint_to_consider (int i, const IntervalVector & box) const {
    if (i==goal_ctr() && _goal_to_consider==false ) return 0;
    return (goal_ctr()==-1 || i== goal_ctr() || ((sys.ops[i]==LEQ || sys.ops[i]==LT) && sys.f_ctrs[i].eval(box).ub() >= 0.0));
  }

  // test to not consider the objective when it is equal  to a variable 
  bool SmearFunction::goal_to_consider(const IntervalMatrix& J, int i) const{
    int nvar0=0;
    int nvar1=0;
    for (int j=0; j<sys.nb_var; j++){
      if (J[i][j].mag() > 1.e-10) nvar0++;
      if (J[i][j].diam() ==0 && J[i][j].mag() ==1) nvar1++;
    }
    if (nvar0==2 && nvar1==2) return false;
    else return true;
  }

    
  BisectionPoint SmearFunction::choose_var(const Cell& cell) {
    const IntervalVector& box=cell.box;
    
    IntervalMatrix J(sys.f_ctrs.image_dim(), sys.nb_var);

    sys.f_ctrs.jacobian(box,J);
    // in case of infinite derivatives  changing to largestfirst  bisection

    for (int i=0; i<sys.f_ctrs.image_dim(); i++){
      for (int j=0; j<sys.nb_var; j++)
	if (J[i][j].mag() == POS_INFINITY ||((J[i][j].mag() ==0) && box[j].diam()== POS_INFINITY ))
	  return lf->choose_var(cell);
      // check if the goal is to be considered
      if (i==goal_ctr()){
	_goal_to_consider=goal_to_consider(J,i);
      }
    }
    
    int var = var_to_bisect (J,box);
	
    // in case of selected var with infinite domain, change to largestfirst bisection
    if (var == -1 || !(box[var].is_bisectable()))
	return lf->choose_var(cell);
    else
      return BisectionPoint(var,lf->ratio,true);
  }


  // computes the variable with the greatest maximal impact
  int SmearMax::var_to_bisect (IntervalMatrix& J, const IntervalVector& box) const {
	double max_magn = NEG_INFINITY;
	int var=-1;
	for (int j=0; j<nbvars; j++) {
	  if ((!too_small(box,j))) { // && (box[j].mag() <1 ||  box[j].diam()/ box[j].mag() >= prec(j))) {
	    for (int i=0; i<sys.f_ctrs.image_dim(); i++) {
	      if (constraint_to_consider (i, box))
		if ( J[i][j].mag() * box[j].diam() > max_magn ) {
		  max_magn = J[i][j].mag()* box[j].diam();
		  var = j;
		}
	    }
	  }
	}
	return var;
  }

  // computes the variable with the greatest  sum of impacts
  int SmearSum::var_to_bisect(IntervalMatrix& J, const IntervalVector& box) const {
    double max_magn = NEG_INFINITY;
    int var = -1;
    for (int j=0; j<nbvars; j++) {
      if ((!too_small(box,j))) { // && (box[j].mag() <1 ||  box[j].diam()/ box[j].mag() >= prec(j))) {
	double sum_smear=0;
	for (int i=0; i<sys.f_ctrs.image_dim(); i++) {
	  if (constraint_to_consider (i, box))
	    sum_smear+= J[i][j].mag() *box[j].diam();
	}
	if (sum_smear > max_magn) {
	  max_magn = sum_smear;
	  var = j;
	}
      }
    }
    return var;
  }
  
  int SmearSumRelative::var_to_bisect(IntervalMatrix& J, const IntervalVector& box) const {
    double max_magn = NEG_INFINITY;
    int var = -1;
    // the normalizing factor per constraint
    double* ctrjsum = new double[sys.f_ctrs.image_dim()];
    
    for (int i=0; i<sys.f_ctrs.image_dim(); i++) {
      ctrjsum[i]=0;
      // not an extended system or constraint is active or it is the objective 
      if (constraint_to_consider(i, box))
	for (int j=0; j<nbvars ; j++) {
	  ctrjsum[i]+= J[i][j].mag() * box[j].diam();
	}
    }
    // computes the variable with the maximal sum of normalized impacts
    for (int j=0; j<nbvars; j++) {
      if ((!too_small(box,j))) { //&&  (box[j].mag() <1 ||  box[j].diam()/ box[j].mag() >= prec(j))) {
	double sum_smear=0;
	for (int i=0; i<sys.f_ctrs.image_dim(); i++) {
	  if (ctrjsum[i]!=0)
	    sum_smear+= J[i][j].mag() * box[j].diam() / ctrjsum[i];
	}

	  
	if (sum_smear > max_magn) {
	  max_magn = sum_smear;
	  var = j;
	}
      }
	
    }
    delete[] ctrjsum;
    return var;
  }

  int SmearMaxRelative::var_to_bisect(IntervalMatrix& J, const IntervalVector& box) const {

    double max_magn = NEG_INFINITY;
    int var = -1;

	double* ctrjsum = new double[sys.f_ctrs.image_dim()]; // the normalizing factor per constraint
	for (int i=0; i<sys.f_ctrs.image_dim(); i++) {
		ctrjsum[i]=0;
		for (int j=0; j<nbvars ; j++) {
			ctrjsum[i]+= J[i][j].mag() * box[j].diam() ;
		}
	}

	// computes the variable with the greatest normalized impact
	double maxsmear=0;
	for (int j=0; j<nbvars; j++) {
		if ((!too_small(box,j))) { // && (box[j].mag() <1 ||  box[j].diam()/ box[j].mag() >= prec(j))) {
		  for (int i=0; i<sys.f_ctrs.image_dim(); i++) {
		    if (constraint_to_consider(i,box))
		      if (ctrjsum[i]!=0)
			maxsmear = J[i][j].mag() * box[j].diam() / ctrjsum[i];
		    if (maxsmear > max_magn) {
			max_magn = maxsmear;
			var = j;
		      }
		    }
		}
	}
	delete[] ctrjsum;
	return var;
  }

} // end namespace ibex
