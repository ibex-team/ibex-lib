/*---------------------------------------------------------------------------------
 * 
 * HC4Revise
 * --------------------------------------
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

#include "mohc/IbexMohc.h"

namespace ibex {

  const REAL Mohc::default_tau_mohc=0.9;
  const REAL Mohc::default_epsilon=0.1;
  const REAL Mohc::default_univ_newton_min_width=1e-8;
  

  
MohcRevise::MohcRevise(ConstraintOG& ctr_mohc, Space& space, REAL tau_mohc, REAL epsilon, REAL univ_newton_min_width) : 
      Operator(space), ctr_mohc(ctr_mohc), tau_mohc(tau_mohc), epsilon(epsilon), univ_newton_min_width(univ_newton_min_width) { 
   ApplyFmin=new int[ctr_mohc.nb_vars()];
   ApplyFmax=new int[ctr_mohc.nb_vars()];
   LB=new INTERVAL[ctr_mohc.nb_vars()];
   RB=new INTERVAL[ctr_mohc.nb_vars()];
}

  MohcRevise::MohcRevise (const MohcRevise& mhrv) : Operator(mhrv.space), Contractor(mhrv),  
  ctr_mohc(mhrv.ctr_mohc), tau_mohc(mhrv.tau_mohc), epsilon(mhrv.epsilon), 
  univ_newton_min_width(mhrv.univ_newton_min_width)
   { ApplyFmin=new int[ctr_mohc.nb_vars()];
   ApplyFmax=new int[ctr_mohc.nb_vars()];
   LB=new INTERVAL[ctr_mohc.nb_vars()];
   RB=new INTERVAL[ctr_mohc.nb_vars()];
   }

  MohcPreproc::MohcPreproc (const MohcPreproc & mopr) : Operator(mopr.space), Contractor(mopr), ctrs (mopr.ctrs) { };

  

void MohcRevise::contract() {
  const Indicators* indic=current_indic();
  if (indic!=NULL && indic->impact==NO_VAR) return;
  if(ctr_mohc.hasMultOcc() && (ctr_mohc.rho_mohc < tau_mohc)){ //monotonic procedures
     INTERVAL_VECTOR initbox=space.box;

     hc4Revise();

     REAL max_reductionHC4=0;
     for(int i=0;i<Dimension(initbox);i++){
        if(Diam(initbox(i+1))>0){
           REAL reduction=1.0-Diam(space.box(i+1))/Diam(initbox(i+1));
           if(max_reductionHC4<reduction) max_reductionHC4=reduction;
        }
     }
       
     if(ctr_mohc.gradientOG(space)){
        initbox=space.box;
        initialize_apply(); //initialize applyfmin and applyfmax arrays
	if(ctr_mohc.op==EQU || ctr_mohc.op==LEQ || ctr_mohc.op==LT){
          minRevise(); 
          LazyMonoBoxNarrow(true);
	}else
	  apply_fmin_to_false_except(-1);
	
	if(ctr_mohc.op==EQU || ctr_mohc.op==GEQ || ctr_mohc.op==GT){
	  maxRevise(); // contract Y, W
          LazyMonoBoxNarrow(false);
	}else
	  apply_fmax_to_false_except(-1);
        
        if(epsilon>0)
           MonoBoxNarrow();
        
     }

  }
  else{
    hc4Revise();
  }
}

  void MohcRevise::MonoBoxNarrow(){
     for(int var=0; var<ctr_mohc.nb_vars(); var++){
        if(ctr_mohc.nb_occ[var]==1) continue;
        INTERVAL& x (*space.entity(ctr_mohc.var2entity[var]).domain);
        if(Diam(x) < 1e-8) continue;
        MonoBoxNarrow(x, var);
     }
  }
  
  void MohcRevise::MonoBoxNarrow(INTERVAL& x, int var){
     INTERVAL zmin=ctr_mohc.zmax, zmax=ctr_mohc.zmax;

     if(ctr_mohc.in_Xplus(var)){ //x is increasing
        if(!zmin.contains(0) && (ApplyFmax[var]!=NO))
           LeftNarrowFmax(x,var);
        
        if(!zmax.contains(0) && (ApplyFmin[var]!=NO))
           RightNarrowFmin(x,var);
     }
     
     if(ctr_mohc.in_Xminus(var)){ //x is decreasing
        if(!zmax.contains(0) && (ApplyFmin[var]!=NO)) 
           LeftNarrowFmin(x,var);
        
        if(!zmin.contains(0) && (ApplyFmax[var]!=NO)) 
           RightNarrowFmax(x,var);
     }
  }
  
  void MohcRevise::LeftNarrowFmax(INTERVAL& x, int var){
     INTERVAL ini(x);
     if(ApplyFmax[var]==MAYBE){ //the first free Newton didn't contract the variable i.
      //existence test
        x=Inf(LB[var]); //point A
        ctr_mohc.forwardOG(space,false); //max eval
        INTERVAL zmax=ctr_mohc.zmax;
        if(Inf(zmax)<=0)  //the lower bound of x_i is solution or will be.
            apply_fmin_to_false_except(var);
   
        if(Sup(zmax)<0){ //there are a solution for L in the interval
           //second free Newton call
           LB[var]=ctr_mohc.Newton_it_cert(LB[var],Inf(LB[var]),Sup(zmax),var);
        }else{
           //ApplyFmax[i]=NO;
           x=ini;
           return;
        }
     }

     REAL w=epsilon*Diam(ini); //should be here or before LazyMohcRevise? or before Hc4Revise?

     ctr_mohc.MonoUnivNewton(space, x, var, LB[var], false, w, univ_newton_min_width);

     x=INTERVAL(Inf(LB[var]), Sup(ini));
  }
  
  void MohcRevise::LeftNarrowFmin(INTERVAL& x, int var){
     INTERVAL ini(x);
     if(ApplyFmin[var]==MAYBE){ //the first free Newton didn't contract the variable i.
      //existence test
        x=Inf(LB[var]); //point A
        ctr_mohc.forwardOG(space,true); //min eval
        INTERVAL zmin=ctr_mohc.zmin;
        if(Sup(zmin)>=0)  //the lower bound of x_i is solution or will be.
           apply_fmax_to_false_except(var);
   
        if(Inf(zmin)>0){ //there are a solution for L in the interval
           //second free Newton call
           LB[var]=ctr_mohc.Newton_it_cert(LB[var],Inf(LB[var]),Inf(zmin),var);
        }else{
           //ApplyFmax[i]=NO;
           x=ini;
           return;
        }
     }

     REAL w=epsilon*Diam(ini); //should be here or before LazyMohcRevise? or before Hc4Revise?

     ctr_mohc.MonoUnivNewton(space,x, var, LB[var], true, w, univ_newton_min_width);

     x=INTERVAL(Inf(LB[var]), Sup(ini));
  }
  
  void MohcRevise::RightNarrowFmin(INTERVAL& x, int var){
     INTERVAL ini(x);
     if(ApplyFmin[var]==MAYBE){ //the first free Newton didn't contract the variable i.
      //existence test
        x=Sup(RB[var]); //point A
        ctr_mohc.forwardOG(space,true); //min eval
        INTERVAL zmin=ctr_mohc.zmin;
        if(Sup(zmin)>=0)  //the lower bound of x_i is solution or will be.
           apply_fmax_to_false_except(var);
   
        if(Inf(zmin)>0){ //there are a solution for L in the interval
           //second free Newton call
           RB[var]=ctr_mohc.Newton_it_cert(RB[var],Sup(RB[var]),Inf(zmin),var);
        }else{
           //ApplyFmax[var]=NO;
           x=ini;
           return;
        }
     }

     REAL w=epsilon*Diam(ini); //should be here or before LazyMohcRevise? or before Hc4Revise?
     ctr_mohc.MonoUnivNewton(space, x, var, RB[var], true, w, univ_newton_min_width);
     x=INTERVAL(Inf(ini), Sup(RB[var]));
  }
  
  void MohcRevise::RightNarrowFmax(INTERVAL& x, int var){
     INTERVAL ini(x);
     if(ApplyFmax[var]==MAYBE){ //the first free Newton didn't contract the variable i.
      //existence test
        x=Sup(RB[var]); //point A
        ctr_mohc.forwardOG(space,false); //max eval
        INTERVAL zmax=ctr_mohc.zmax;
        if(Inf(zmax)<=0)  //the lower bound of x_i is solution or will be.
           apply_fmin_to_false_except(var);
   
        if(Sup(zmax)<0){ //there are a solution for L in the interval
           //second free Newton call
           RB[var]=ctr_mohc.Newton_it_cert(RB[var],Sup(RB[var]),Sup(zmax),var);
        }else{
           //ApplyFmax[i]=NO;
           x=ini;
           return;
        }
     }

     REAL w=epsilon*Diam(ini); //should be here or before LazyMohcRevise? or before Hc4Revise?

     ctr_mohc.MonoUnivNewton(space,x, var, RB[var], false, w, univ_newton_min_width);

     x=INTERVAL(Inf(ini), Sup(RB[var]));
  }

  void MohcRevise::LazyMonoBoxNarrow(bool minEval){
     for(int var=0; var<ctr_mohc.nb_vars(); var++){
        if(ctr_mohc.nb_occ[var]==1) continue;
        
        INTERVAL& x (*space.entity(ctr_mohc.var2entity[var]).domain);
        LB[var]=x;
        RB[var]=x;
        if(ctr_mohc.in_Xplus(var)){
           if(!minEval) initLeftNarrowFmax(x, var);
           else initRightNarrowFmin(x, var);
        }

        if(ctr_mohc.in_Xminus(var)){
           if(minEval) initLeftNarrowFmin(x, var);
           else initRightNarrowFmax(x, var);
        }
        
        x=INTERVAL(Inf(LB[var]), Sup(RB[var]));
     }
  }
  
  //initialize LB projecting fmax (or fmin) in X[i] using deriv[i].
//contract the left bound of X[i] using the new value Inf(LB).
  void MohcRevise::initLeftNarrowFmin(INTERVAL x, int var){
     LB[var]=ctr_mohc.Newton_it(x,Sup(LB[var]),Inf(ctr_mohc.zmin),var);
     if(!LB[var].empty() && (Inf(LB[var]) > Inf(x))){
        apply_fmax_to_false_except(var);
        ApplyFmin[var]=YES;
     }else if(LB[var].empty()){
        ApplyFmin[var]=NO;
        LB[var]=Inf(x);
     }
  }
  
  void MohcRevise::initLeftNarrowFmax(INTERVAL x, int var){
     LB[var]=ctr_mohc.Newton_it(x,Sup(LB[var]),Sup(ctr_mohc.zmax),var);

     
     if(!LB[var].empty() && (Inf(LB[var]) > Inf(x))){
        apply_fmin_to_false_except(var);
        ApplyFmax[var]=YES;
     }else if(LB[var].empty()){
        ApplyFmax[var]=NO;
        LB[var]=Inf(x);
     }
  }

  void MohcRevise::initRightNarrowFmax(INTERVAL x, int var){

     RB[var]=ctr_mohc.Newton_it(x,Inf(RB[var]),Sup(ctr_mohc.zmax),var);
     if(!RB[var].empty() && (Sup(RB[var]) < Sup(x))){
        apply_fmin_to_false_except(var);
        ApplyFmax[var]=YES;
     }else if(RB[var].empty()){
        ApplyFmax[var]=NO;
        RB[var]=Sup(x);
     }
  }

  void MohcRevise::initRightNarrowFmin(INTERVAL x, int var){
     RB[var]=ctr_mohc.Newton_it(x,Inf(RB[var]),Inf(ctr_mohc.zmin),var);
     if(!RB[var].empty() && (Sup(RB[var]) < Sup(x))){
        apply_fmax_to_false_except(var);
        ApplyFmin[var]=YES;
     }else if(RB[var].empty()){
        ApplyFmin[var]=NO;
        RB[var]=Sup(x);
     }
  }

  void MohcRevise::initialize_apply(){
     for (int i=0; i<ctr_mohc.nb_vars(); i++){
        ApplyFmin[i]=MAYBE;
        ApplyFmax[i]=MAYBE;
     }
  }
  
  void MohcRevise::apply_fmax_to_false_except(int var){
     for (int i=0; i<ctr_mohc.nb_vars(); i++)
        if(i!=var)
           ApplyFmax[i]=NO;
  }
  
  void MohcRevise::apply_fmin_to_false_except(int var){
     for (int i=0; i<ctr_mohc.nb_vars(); i++)
        if(i!=var)
           ApplyFmin[i]=NO;
  }
  
  void MohcRevise::minRevise() {
     ctr_mohc.forwardOG(space, true);
     ctr_mohc.backwardOG(space, true);
  }
  
  void MohcRevise::maxRevise() {
     ctr_mohc.forwardOG(space, false);
     ctr_mohc.backwardOG(space, false);
  }
  
  void MohcRevise::minRevise2() {
     ctr_mohc.forwardOG(space, true);
     ctr_mohc.backward2OG(space, true);
  }
  
  void MohcRevise::maxRevise2() {
     ctr_mohc.forwardOG(space, false);
     ctr_mohc.backward2OG(space, false);
  }
  
  void MohcRevise::hc4Revise() {
     ctr_mohc.forward(space);
     ctr_mohc.backward(space);
  }

  void MohcPreproc::contract(){
     for (vector<ConstraintOG*>::const_iterator it=ctrs.begin(); it!=ctrs.end(); it++) 
        (*it)->compute_rho_mohc(space, true);
  }

  static vector<const Contractor*> vec;
  static vector<ConstraintOG*> ctrsog;
  
  static vector<const Contractor*>& Mohc_contractor_list(const System& sys, REAL tau_mohc, REAL epsilon, REAL univ_newton_min_width){

     
     for(int i=0; i<sys.nb_ctr() ; i++){
        ConstraintOG* ctr_mohc=new ConstraintOG(sys.ctr(i),sys.space);
        vec.push_back(new MohcRevise(*ctr_mohc, sys.space, tau_mohc, epsilon, univ_newton_min_width));
        ctrsog.push_back(ctr_mohc);
     }

     return vec;
  }

static vector<const Contractor*>& Mohc_contractor_list(MohcPreproc* preproc, const System& sys, REAL tau_mohc, REAL epsilon, REAL univ_newton_min_width){ 
     for(int i=0; i<sys.nb_ctr() ; i++)
        vec.push_back(new MohcRevise(*(preproc->ctrs[i]), sys.space, tau_mohc, epsilon, univ_newton_min_width));
     
     return vec;
  }

Mohc::Mohc(MohcPreproc* preproc, const System& sys, REAL ratio, bool incremental,  REAL tau_mohc, REAL epsilon, 
  REAL univ_newton_min_width):Operator(sys.space), 
    Propagation(Mohc_contractor_list(preproc, sys, tau_mohc, epsilon, univ_newton_min_width), 
sys.space, ratio, incremental), preproc(preproc->copy())
{
vec.clear();
}


  
Mohc::Mohc(const System& sys, REAL ratio, bool incremental,  REAL tau_mohc, REAL epsilon, 
	   REAL univ_newton_min_width): 
        Operator(sys.space), 
        Propagation(Mohc_contractor_list(sys, tau_mohc, epsilon, univ_newton_min_width), sys.space, ratio, incremental),
	preproc(NULL){
    for (vector<const Contractor*>::const_iterator it=vec.begin(); it!=vec.end(); it++) {
      delete *it;
    }
    preproc=new MohcPreproc(ctrsog, space);
    
    vec.clear();
          
          
    ctrsog.clear();
  }
   
  // copy constructor

  Mohc::Mohc (const Mohc & p) :  Operator(p.space) , Propagation (p)  { preproc=p.preproc->copy();};
  
  
  

  //  constructor with all parameters
  Mohc_All::Mohc_All(const System& sys, REAL ratio, bool incremental, 
          REAL tau_mohc, REAL epsilon, REAL univ_newton_min_width, bool with3BCID, int s3b, int scid, 
          int vhandled, REAL var_min_width) : Operator(sys.space){
     mohc = new Mohc(sys, ratio, incremental, tau_mohc, epsilon, univ_newton_min_width);

     if(with3BCID)
       {     Mohc mohc1 (mohc->get_preproc(), sys, ratio, true , tau_mohc, epsilon, univ_newton_min_width) ; // Mohc inside 3BCID is always incremental
	 m3b = new _3BCID(sys.space, mohc1, s3b, scid, vhandled, var_min_width);}
     else 
       m3b=NULL;
  }


  // constructor using the default value for the paramaters, excepting prec used for univ_newton_min_width and var_min_width and
  // tau_mohc 

  Mohc_All::Mohc_All(const System& sys, bool with3BCID, REAL prec, REAL tau_mohc) : Operator(sys.space)
  {mohc = new Mohc(sys, Propagation::default_ratio, true, tau_mohc, Mohc::default_epsilon, prec);

     if(with3BCID)
       {     Mohc mohc1 (mohc->get_preproc(), sys, Propagation::default_ratio, true , tau_mohc,
			 Mohc::default_epsilon, prec) ; // Mohc inside 3BCID is always incremental
	 m3b = new _3BCID(sys.space, mohc1, _3BCID::default_s3b, _3BCID::default_scid, sys.nb_var(), prec);}
     else 
       m3b=NULL;
  }



  
  Mohc_All::Mohc_All (const Mohc_All & c) :  
  Operator(c.space) , Contractor (c)  { 
    m3b=(c.m3b)? c.m3b->copy() : NULL; 
    mohc=c.mohc->copy();};
  
  void Mohc_All::contract(){
     mohc->get_preproc()->space.sync(space);
     mohc->get_preproc()->contract();
     
     mohc->space.sync(mohc->get_preproc()->space);
     mohc->contract_floor=-1;
     const Indicators* indic=current_indic();


     if (indic && mohc->incremental && indic->impact != Contractor::NO_VAR ) mohc->contract(indic->impact);
     else mohc->contract();
     
     if(m3b){
       m3b->contract_floor=-1;
        m3b->space.sync(mohc->space);
        m3b->contract();

     }
     
     space.sync((m3b)? m3b->space : mohc->space);     
  }
  
} // end namespace
