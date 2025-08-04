//============================================================================
//                                  I B E X
// File        : ibex_LoupFinderDefaultIpopt.cpp
// Author      : Bertrand Neveu
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Aug 03, 2023
// Last update : Mar 07, 2024
//============================================================================

#include <limits>
#include "ibex_LoupFinderDefaultIpopt.h"

using namespace std;

namespace ibex {

  LoupFinderDefaultIpopt::LoupFinderDefaultIpopt(const System& sys, const System& normsys, const ExtendedSystem& extsys, bool inHC4,bool xtaylor
						 //,bool integerobjective
						 )  : sys(sys), normsys(normsys), extsys(extsys),	xtaylor(xtaylor),								 //	finder_probing(inHC4? (LoupFinder&) *new LoupFinderInHC4(sys) : (LoupFinder&) *new LoupFinderFwdBwd(sys)),
        finder_probing(inHC4? (LoupFinder&) *new LoupFinderInHC4(normsys) : (LoupFinder&) *new LoupFinderProbing(normsys)),
        finder_x_taylor(normsys),
	finder_ipopt(sys,normsys,extsys){
    /*
    finder_probing.integerobj=integerobjective;
    finder_x_taylor.integerobj=integerobjective;
    finder_ipopt.integerobj=integerobjective;
    */
}

  /*
  bool LoupFinderDefaultIpopt::integer_check(Vector& pt) {return integer_and_bound_check(normsys,pt);}
  bool LoupFinderDefaultIpopt::is_inner (Vector& pt) {return is_inner0(normsys,pt);}
  double LoupFinderDefaultIpopt::goal_ub (Vector& pt) {return goal_ub0(normsys,pt);}
  void LoupFinderDefaultIpopt::sysbound(Vector& pt) {bound_check(normsys,pt);}
  void LoupFinderDefaultIpopt::sysbound(IntervalVector& vec) {bound_check(normsys,vec);}
  */
void LoupFinderDefaultIpopt::add_property(const IntervalVector& init_box, BoxProperties& prop) {
	finder_probing.add_property(init_box,prop);
	finder_x_taylor.add_property(init_box,prop);
	finder_ipopt.add_property(init_box,prop);

}

std::pair<IntervalVector, double> LoupFinderDefaultIpopt::find(const IntervalVector& box, const IntervalVector& old_loup_point, double old_loup, BoxProperties& prop) {

	pair<IntervalVector,double> p=make_pair(old_loup_point, old_loup);

	bool found=false;

	IntervalVector box1(box);
	
	try {   // if (!(finder_ipopt.recursive_call)) cout << " recursive box " << box << endl; 
		p=finder_probing.find(box,p.first,p.second,prop);
		found=true;
			if (finder_ipopt.recursive_call) cout << "probing " << p.second << endl;
		else cout << " probing recursive " << p.second << endl;
	} catch(NotFound&) { }
        if (xtaylor){
	try {
		// TODO
		// in_x_taylor.set_inactive_ctr(entailed->norm_entailed);
		p=finder_x_taylor.find(box,p.first,p.second,prop);
		found=true;
		
			if (finder_ipopt.recursive_call) cout << "xtaylor " << p.second << endl;
		else cout << " xtaylor recursive " << p.second << endl;
		
	} catch(NotFound&) { }
	}
	
        if (found && finder_ipopt.recursive_call) {
	  double ymax=finder_ipopt.optimizer->compute_ymax();
	  //cout << " ymax " << ymax << " p.second " << p.second << endl;
	  if (p.second <= ymax){
	    	    finder_ipopt.force=true;
	   	    finder_ipopt.solution=p.first.mid();
	  }
	}
	  

    
	try { p=finder_ipopt.find(box1,p.first,p.second);
	      found=true;
	} catch(NotFound&) {
	}
	if (found) {
	  return p;
	}
	else
	  throw NotFound();
}

LoupFinderDefaultIpopt::~LoupFinderDefaultIpopt() {
	delete &finder_probing;
}

} /* namespace ibex */
