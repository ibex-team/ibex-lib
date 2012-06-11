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


  /*********** amohc ******/
  //parameters
  const double Mohc::RHO_INTERESTING=0.65; //0.65
  const double Mohc::TAU_FREQ=0.10; //0.10
  
  
  
  //for analysis purposes only
  bool MohcRevise::_minmax=true;
  bool MohcRevise::_monobox=true;
  bool MohcRevise::_opt=true;
  bool MohcRevise::_og=true;
  bool MohcRevise::_mohc2=true;
  
  
MohcRevise::MohcRevise(ConstraintOG& ctr_mohc, Space& space, REAL epsilon, REAL univ_newton_min_width, REAL tau_mohc, bool amohc) :
      Operator(space), ctr_mohc(ctr_mohc), tau_mohc(tau_mohc), epsilon(epsilon), univ_newton_min_width(univ_newton_min_width),
      active_mono_proc(1), amohc(amohc), LB(NULL), RB(NULL), nb_calls(0), nb_interesting(0) { 
   ApplyFmin=new int[ctr_mohc.nb_vars()];
   ApplyFmax=new int[ctr_mohc.nb_vars()];
   LB=new INTERVAL[ctr_mohc.nb_vars()];
   RB=new INTERVAL[ctr_mohc.nb_vars()];
}

  MohcRevise::MohcRevise (const MohcRevise& mhrv) : Operator(mhrv.space), Contractor(mhrv),  
  ctr_mohc(mhrv.ctr_mohc), tau_mohc(mhrv.tau_mohc), epsilon(mhrv.epsilon), 
  univ_newton_min_width(mhrv.univ_newton_min_width), active_mono_proc(1), amohc(mhrv.amohc), LB(NULL), RB(NULL), nb_calls(0), nb_interesting(0)
   { ApplyFmin=new int[ctr_mohc.nb_vars()];
   ApplyFmax=new int[ctr_mohc.nb_vars()];
   LB=new INTERVAL[ctr_mohc.nb_vars()];
   RB=new INTERVAL[ctr_mohc.nb_vars()];
   }
  




void MohcRevise::contract() {
  const Indicators* indic=current_indic();
  if (indic!=NULL && indic->impact==NO_VAR) return;
  
     INTERVAL z=hc4Revise();
     if(!ctr_mohc.hasMultOcc())
       active_mono_proc=0; 
  
  
  if(active_mono_proc != 0){ //monotonic procedures     

     bool y_set=_minmax;//the Y set is created only if minmax is used
     INTERVAL_VECTOR initbox=space.box;

     
     
     if (ctr_mohc.gradientOG(space, y_set, _og)){
        initialize_apply(); //initialize applyfmin and applyfmax arrays
	if(ctr_mohc.op==EQU || ctr_mohc.op==LEQ || ctr_mohc.op==LT){
	  if(_minmax)
             minRevise();
	  else{
	     // only the existence test
	    ctr_mohc.forwardOG(space, true); 
	    bool sat = ctr_mohc.evlog->output() &= INTERVAL(BiasNegInf,0);
	    if (!sat) throw EmptyBoxException();
	  }
	}else
	  apply_fmin_to_false_except(-1);
	
	if(ctr_mohc.op==EQU || ctr_mohc.op==GEQ || ctr_mohc.op==GT){
	  if(_minmax)
	     maxRevise(); // contract Y, W
	  else{
	    // only the existence test
	    ctr_mohc.forwardOG(space, false);
	    bool sat = ctr_mohc.evlog->output() &= INTERVAL(0,BiasPosInf);
	    if (!sat) throw EmptyBoxException();
	  }
	}else
	  apply_fmax_to_false_except(-1);

     
        if(active_mono_proc==-1){ //compute_rho_mohc
                       
           INTERVAL ev_mono(0,0);
           if(ctr_mohc.op==EQU || ctr_mohc.op==LEQ || ctr_mohc.op==LT)
	     ev_mono=Hull(ev_mono,ctr_mohc.zmin);
	   if(ctr_mohc.op==EQU || ctr_mohc.op==GEQ || ctr_mohc.op==GT)
	     ev_mono=Hull(ev_mono,ctr_mohc.zmax);	     
	   REAL diam_mono=Diam(ev_mono);
	   REAL diam_nat=Diam(z);
	   double rho;
           if(diam_nat>0)
             rho=diam_mono/diam_nat;
           else
	     rho=1.01;
	   
	   if(amohc){
	     nb_calls++;
	     if(nb_calls%500==100){
                 cout << "FREQ:" << ((REAL)nb_interesting/(REAL)nb_calls) << endl;
             } 
//cout << rho << endl;
	     if(rho < Mohc::RHO_INTERESTING){
             //  cout << rho << endl;
	       nb_interesting++;
             }
	     if(nb_calls > 50 && (REAL)nb_interesting/(REAL)nb_calls < Mohc::TAU_FREQ)
	       tau_mohc=0.5;
	     else 
	       tau_mohc=0.9999;
	   }
           //cout << tau_mohc << endl;
	   

	   if(rho<tau_mohc) active_mono_proc=1;
	   else active_mono_proc=0;
        }

        if(epsilon>0 && _monobox)
           MonoBoxNarrow();
        
	
     }
  }
}

  void MohcRevise::MonoBoxNarrow(){
     for(int var=0; var<ctr_mohc.nb_vars(); var++){
        if(ctr_mohc.nb_occ[var]==1 && _minmax) continue;
        INTERVAL& x (*space.entity(ctr_mohc.var2entity[var]).domain);
        LB[var]=x;
        RB[var]=x;  
     }
     if(_opt){
	LazyMonoBoxNarrow();
     }
     
//      if(active_mono_proc==0) return;
    
     INTERVAL zmin=(ctr_mohc.op==EQU || ctr_mohc.op==LEQ || ctr_mohc.op==LT)? ctr_mohc.zmin : INTERVAL::EMPTY;
     INTERVAL zmax=(ctr_mohc.op==EQU || ctr_mohc.op==GEQ || ctr_mohc.op==GT)? ctr_mohc.zmax : INTERVAL::EMPTY;
     
     if(_opt){
       if(zmin.contains(0)) apply_fmax_to_false_except(-1);
       if(zmax.contains(0)) apply_fmin_to_false_except(-1);
     }
     
     for(int var=0; var<ctr_mohc.nb_vars(); var++){
        if(ctr_mohc.nb_occ[var]==1 && _minmax) continue;
        INTERVAL& x (*space.entity(ctr_mohc.var2entity[var]).domain);
        if(Diam(x) < 1e-8) continue;
        MonoBoxNarrow(x, var);
     }
     for(int var=0; var<ctr_mohc.nb_vars(); var++){
       if(ctr_mohc.nb_occ[var]==1 && _minmax) continue;
       *space.entity(ctr_mohc.var2entity[var]).domain=INTERVAL(Inf(LB[var]), Sup(RB[var]));
     }
  }
  
  void MohcRevise::MonoBoxNarrow(INTERVAL& x, int var){
     //Si la variable no ha sido tratado completamente por MinMaxRevise
     //Proyeccion usando MinMaxRevise.


     
     if(ctr_mohc.in_Xplus(var)){ //x is increasing
        if(ApplyFmax[var]!=NO)
           LeftNarrowFmax(x,var);
        
        if(ApplyFmin[var]!=NO)
           RightNarrowFmin(x,var);
	return;
     }
     
     if(ctr_mohc.in_Xminus(var)){ //x is decreasing
        if(ApplyFmin[var]!=NO)
           LeftNarrowFmin(x,var);
        
        if(ApplyFmax[var]!=NO)
           RightNarrowFmax(x,var);
	return;
     }

  if(_mohc2)
     if(ApplyFmax[var]!=NO || ApplyFmin[var]!=NO){
      bool og_treated=false;
      for(int occ=0;occ<ctr_mohc.nb_occ[var];occ++)
           if(Sup(ctr_mohc.r_c[ctr_mohc.vocc2cocc[var][occ]]) < 1.0){og_treated=true; break;}
//            if(Inf(ctr_mohc.r_c[ctr_mohc.vocc2cocc[var][occ]]) <= 0.0){og_treated=true; break;}
      if(og_treated){
         LeftNarrow(x, var);
	 RightNarrow(x, var);
     }
     }
  }

  void MohcRevise::RightNarrow(INTERVAL& x, int var){
    INTERVAL ini(x);
    
    /**********existence test*************/
    x=Sup(RB[var]);
    INTERVAL zmax=INTERVAL::EMPTY;
    INTERVAL zmin=INTERVAL::EMPTY;
    
    if(ApplyFmax[var]!=NO){
      ctr_mohc.forwardOG(space,false); //max eval
      zmax=ctr_mohc.zmax;
      if(_opt && Inf(zmax)<=0)  //the lower bound of x_i is solution or will be.
         apply_fmin_to_false_except(var);
    }
    
    if(ApplyFmin[var]!=NO){
      ctr_mohc.forwardOG(space,true); //min eval
      zmin=ctr_mohc.zmin;
      if(_opt && Sup(zmin)>=0)  //the lower bound of x_i is solution or will be.
         apply_fmax_to_false_except(var);      
    }
    
    if((zmax.empty() || Sup(zmax)>=0) && (zmin.empty() || Inf(zmin)<=0)){
      x=ini;
      return;
    }
    /************************************/
    REAL w=epsilon*Diam(ini);
    RB[var]=INTERVAL(Mid(RB[var]),Sup(RB[var]));
    while(2*Diam(RB[var])>w && 2*Diam(RB[var])>univ_newton_min_width){
        x=RB[var];
        ctr_mohc.forwardOG(space,false);
	ctr_mohc.forwardOG(space,true);
	if(Hull(ctr_mohc.zmin,ctr_mohc.zmax).contains(0))
	  RB[var]=INTERVAL(Mid(RB[var]),Sup(RB[var]));
	else
	  RB[var]=INTERVAL(Inf(RB[var])-Diam(RB[var])/2,Inf(RB[var]));
    }
    
    RB[var]&=ini;
    if(RB[var].empty()) throw EmptyBoxException();
    
    x=ini; 
  }

  //LeftNarrow of a variable not completely monotone!
  void MohcRevise::LeftNarrow(INTERVAL& x, int var){
    INTERVAL ini(x);
    
    /**********existence test*************/
    x=Inf(LB[var]);
    INTERVAL zmax=INTERVAL::EMPTY;
    INTERVAL zmin=INTERVAL::EMPTY;
    
    if(ApplyFmax[var]!=NO){
      ctr_mohc.forwardOG(space,false); //max eval
      zmax=ctr_mohc.zmax;
      if(_opt && Inf(zmax)<=0)  //the lower bound of x_i is solution or will be.
         apply_fmin_to_false_except(var);
    }
    
    if(ApplyFmin[var]!=NO){
      ctr_mohc.forwardOG(space,true); //min eval
      zmin=ctr_mohc.zmin;
      if(_opt && Sup(zmin)>=0)  //the lower bound of x_i is solution or will be.
         apply_fmax_to_false_except(var);      
    }
    
    if((zmax.empty() || Sup(zmax)>=0) && (zmin.empty() || Inf(zmin)<=0)){
      x=ini;
      return;
    }
    /************************************/
    REAL w=epsilon*Diam(ini);
    LB[var]=INTERVAL(Inf(LB[var]),Mid(LB[var]));
    while(2*Diam(LB[var])>w && 2*Diam(LB[var])>univ_newton_min_width){
        x=LB[var];
        ctr_mohc.forwardOG(space,false);
	ctr_mohc.forwardOG(space,true);
	if(Hull(ctr_mohc.zmin,ctr_mohc.zmax).contains(0))
	  LB[var]=INTERVAL(Inf(LB[var]),Mid(LB[var]));
	else
	  LB[var]=INTERVAL(Sup(LB[var]),Sup(LB[var])+Diam(LB[var])/2);
    }
    
    LB[var]&=ini;
    if(LB[var].empty()) throw EmptyBoxException();
    
    x=ini; 
  }
  
  void MohcRevise::LeftNarrowFmax(INTERVAL& x, int var){
     INTERVAL ini(x);
     if(ApplyFmax[var]==MAYBE){ //the first free Newton didn't contract the variable i.
      //existence test
        x=Inf(LB[var]); //point A
        ctr_mohc.forwardOG(space,false); //max eval
        INTERVAL zmax=ctr_mohc.zmax;
        if(_opt && Inf(zmax)<=0)  //the lower bound of x_i is solution or will be.
            apply_fmin_to_false_except(var);
   
        if(Sup(zmax)<0){ //there are a solution for L in the interval
           //second free Newton call
           if(_opt) LB[var]=ctr_mohc.Newton_it_cert(LB[var],Inf(LB[var]),Sup(zmax),var);
        }else{
           //ApplyFmax[i]=NO;
           x=ini;
           return;
        }
     }

     REAL w=epsilon*Diam(ini); //should be here or before LazyMohcRevise? or before Hc4Revise?

     ctr_mohc.MonoUnivNewton(space, x, var, LB[var], false, w, univ_newton_min_width);
     x=ini;
//      x=INTERVAL(Inf(LB[var]), Sup(ini));
  }
  
  void MohcRevise::LeftNarrowFmin(INTERVAL& x, int var){
     INTERVAL ini(x);
     if(ApplyFmin[var]==MAYBE){ //the first free Newton didn't contract the variable i.
      //existence test
        x=Inf(LB[var]); //point A
        ctr_mohc.forwardOG(space,true); //min eval
        INTERVAL zmin=ctr_mohc.zmin;
        if(_opt && Sup(zmin)>=0)  //the lower bound of x_i is solution or will be.
           apply_fmax_to_false_except(var);
   
        if(Inf(zmin)>0){ //there are a solution for L in the interval
           //second free Newton call
           if(_opt){
	     LB[var]=ctr_mohc.Newton_it_cert(LB[var],Inf(LB[var]),Inf(zmin),var);
	   }
        }else{
           //ApplyFmax[i]=NO;
           x=ini;
           return;
        }
     }

     REAL w=epsilon*Diam(ini); //should be here or before LazyMohcRevise? or before Hc4Revise?

     ctr_mohc.MonoUnivNewton(space,x, var, LB[var], true, w, univ_newton_min_width);
     x=ini;
//      x=INTERVAL(Inf(LB[var]), Sup(ini));
  }
  
  void MohcRevise::RightNarrowFmin(INTERVAL& x, int var){
     INTERVAL ini(x);
     if(ApplyFmin[var]==MAYBE){ //the first free Newton didn't contract the variable i.
      //existence test
        x=Sup(RB[var]); //point A
        ctr_mohc.forwardOG(space,true); //min eval
        INTERVAL zmin=ctr_mohc.zmin;
        if(_opt && Sup(zmin)>=0)  //the lower bound of x_i is solution or will be.
           apply_fmax_to_false_except(var);
   
        if(Inf(zmin)>0){ //there are a solution for L in the interval
           //second free Newton call
           if(_opt)  RB[var]=ctr_mohc.Newton_it_cert(RB[var],Sup(RB[var]),Inf(zmin),var);
        }else{
           //ApplyFmax[var]=NO;
           x=ini;
           return;
        }
     }

     REAL w=epsilon*Diam(ini); //should be here or before LazyMohcRevise? or before Hc4Revise?
     ctr_mohc.MonoUnivNewton(space, x, var, RB[var], true, w, univ_newton_min_width);
     x=ini;
//      x=INTERVAL(Inf(ini), Sup(RB[var]));
  }
  
  void MohcRevise::RightNarrowFmax(INTERVAL& x, int var){
     INTERVAL ini(x);
     if(ApplyFmax[var]==MAYBE){ //the first free Newton didn't contract the variable i.
      //existence test
        x=Sup(RB[var]); //point A
        ctr_mohc.forwardOG(space,false); //max eval
        INTERVAL zmax=ctr_mohc.zmax;
        if(_opt && Inf(zmax)<=0)  //the lower bound of x_i is solution or will be.
           apply_fmin_to_false_except(var);
   
        if(Sup(zmax)<0){ //there are a solution for L in the interval
           //second free Newton call
           if(_opt)  RB[var]=ctr_mohc.Newton_it_cert(RB[var],Sup(RB[var]),Sup(zmax),var);
        }else{
           //ApplyFmax[i]=NO;
           x=ini;
           return;
        }
     }

     REAL w=epsilon*Diam(ini); //should be here or before LazyMohcRevise? or before Hc4Revise?

     ctr_mohc.MonoUnivNewton(space,x, var, RB[var], false, w, univ_newton_min_width);
     x=ini;
//      x=INTERVAL(Inf(ini), Sup(RB[var]));
  }

  void MohcRevise::LazyMonoBoxNarrow(){
     for(int var=0; var<ctr_mohc.nb_vars(); var++){
        if(ctr_mohc.nb_occ[var]==1 && _minmax) continue;

        INTERVAL& x (*space.entity(ctr_mohc.var2entity[var]).domain);

	INTERVAL ini(x);
        if(ctr_mohc.in_Xplus(var)){
	  if(ApplyFmax[var]!=NO)
	     initLeftNarrowFmax(x, var);
	  if(ApplyFmin[var]!=NO)
	     initRightNarrowFmin(x, var);
        }else  if(ctr_mohc.in_Xminus(var)){
	  if(ApplyFmin[var]!=NO)
	     initLeftNarrowFmin(x, var);
	  if(ApplyFmax[var]!=NO)
	     initRightNarrowFmax(x, var);
        }else if(_og){  
	  initLeftNarrow(x, var);
	  initRightNarrow(x, var);  
	}
        x=ini;
     }
     
     
     
  }
  
  
  void MohcRevise::initLeftNarrow(INTERVAL x, int var){
     INTERVAL t_a=LB[var];
     INTERVAL t_b=LB[var];
     if(ApplyFmax[var]!=NO)
       t_a=ctr_mohc.Newton_it(LB[var],Sup(LB[var]),Sup(ctr_mohc.zmax),var,true); //x_a
     if(ApplyFmin[var]!=NO)
       t_b=ctr_mohc.Newton_it(LB[var],Inf(LB[var]),Inf(ctr_mohc.zmin),var,false); //x_b
     
       
     if(!t_a.empty() && (Inf(t_a) > Inf(LB[var]))){
        apply_fmax_to_false_except(var);
        ApplyFmin[var]=YES;
     }

     if(!t_b.empty() && (Inf(t_b) > Inf(LB[var]))){
        apply_fmin_to_false_except(var);
        ApplyFmax[var]=YES;
     }
     if(t_a.empty()) ApplyFmax[var]=NO;
     if(t_b.empty()) ApplyFmin[var]=NO;
     
     if(!t_a.empty() || !t_b.empty())
     {
       if(!t_a.empty()) LB[var]&=t_a;
       if(!t_b.empty()) LB[var]=INTERVAL((Inf(LB[var])>=Inf(t_b))?Inf(LB[var]):Inf(t_b), Sup(LB[var])) ;
       
//        if(Diam(LB[var])>0){
//          REAL deriv_a=Sup(ctr_mohc.zmax)/Diam(LB[var]);
//          REAL deriv_b=Inf(ctr_mohc.zmin)/Diam(LB[var]);       
//          if(Sup(ctr_mohc.deriv_a[var])>0 && deriv_a/Sup(ctr_mohc.deriv_a[var]) < tau_mono) ApplyFmax[var]=NO;
//          if(Inf(ctr_mohc.deriv_b[var])<0 && deriv_b/Inf(ctr_mohc.deriv_b[var]) < tau_mono) ApplyFmin[var]=NO; 
//        }
     }
  }

  void MohcRevise::initRightNarrow(INTERVAL x, int var){
     INTERVAL t_a=RB[var];
     INTERVAL t_b=RB[var];
     if(ApplyFmin[var]!=NO)
       t_a=ctr_mohc.Newton_it(RB[var],Sup(RB[var]),Inf(ctr_mohc.zmin),var,true);  //x_a
     if(ApplyFmax[var]!=NO)
       t_b=ctr_mohc.Newton_it(RB[var],Inf(RB[var]),Sup(ctr_mohc.zmax),var,false); //x_b
       
     if(t_a==RB[var] && t_b==RB[var]) return;
     
     if(!t_a.empty() && (Sup(t_a) < Sup(LB[var]))){
        apply_fmax_to_false_except(var);
        ApplyFmin[var]=YES;
     }

     if(!t_b.empty() && (Sup(t_b) < Sup(LB[var]))){
        apply_fmin_to_false_except(var);
        ApplyFmax[var]=YES;
     }

     
     if(t_a.empty()) ApplyFmin[var]=NO;
     if(t_b.empty()) ApplyFmax[var]=NO;
     if(!t_a.empty() || !t_b.empty())
     {
       if(!t_a.empty()) RB[var]&=t_a;
       if(!t_b.empty()) RB[var]=INTERVAL(Inf(RB[var]),  (Sup(LB[var])<=Sup(t_b))?Sup(LB[var]):Sup(t_b)) ;
       
//        if(Diam(RB[var])>0){
//           REAL deriv_a=-Inf(ctr_mohc.zmin)/Diam(RB[var]);
//           REAL deriv_b=-Sup(ctr_mohc.zmax)/Diam(RB[var]);       
//           if(Sup(ctr_mohc.deriv_a[var])>0 && deriv_a/Sup(ctr_mohc.deriv_a[var]) < tau_mono) ApplyFmin[var]=NO;
//           if(Inf(ctr_mohc.deriv_b[var])<0 && deriv_b/Inf(ctr_mohc.deriv_b[var]) < tau_mono) ApplyFmax[var]=NO;       
//        }
       
     }
  }

  
  //initialize LB projecting fmax (or fmin) in X[i] using deriv[i].
//contract the left bound of X[i] using the new value Inf(LB).
  void MohcRevise::initLeftNarrowFmin(INTERVAL x, int var){
     INTERVAL t=ctr_mohc.Newton_it(LB[var],Sup(LB[var]),Inf(ctr_mohc.zmin),var);
     if(!t.empty() && (Inf(t) > Inf(LB[var]))){
        apply_fmax_to_false_except(var);
        ApplyFmin[var]=YES;
	LB[var]&=t;
     }else if(t.empty()){ //no contraction is possible
        ApplyFmin[var]=NO;
     }else{
        LB[var]&=t;
//         if(Diam(LB[var])>0 && Inf(ctr_mohc.deriv_b[var])<0){
//            REAL deriv_b=Inf(ctr_mohc.zmin)/Diam(LB[var]);       
//            if(deriv_b/Inf(ctr_mohc.deriv_b[var]) < tau_mono) ApplyFmin[var]=NO; 
//        }
     }
  }
  
  void MohcRevise::initLeftNarrowFmax(INTERVAL x, int var){
     INTERVAL t=ctr_mohc.Newton_it(LB[var],Sup(LB[var]),Sup(ctr_mohc.zmax),var);
     if(!t.empty() && (Inf(t) > Inf(LB[var]))){
        apply_fmin_to_false_except(var);
        ApplyFmax[var]=YES;
	LB[var]&=t;
     }else if(t.empty()){ //no contraction is possible
        ApplyFmax[var]=NO;
     }else{
       LB[var]&=t;
//        if(Diam(LB[var])>0 && Sup(ctr_mohc.deriv_a[var])>0){
//          REAL deriv_a=Sup(ctr_mohc.zmax)/Diam(LB[var]);
//          if(deriv_a/Sup(ctr_mohc.deriv_a[var]) < tau_mono) ApplyFmax[var]=NO;
//        }
     }
  }

  void MohcRevise::initRightNarrowFmax(INTERVAL x, int var){
     INTERVAL t=ctr_mohc.Newton_it(RB[var],Inf(RB[var]),Sup(ctr_mohc.zmax),var);
     if(!t.empty() && (Sup(t) < Sup(RB[var]))){
        apply_fmin_to_false_except(var);
        ApplyFmax[var]=YES;
	RB[var]&=t;
     }else if(t.empty()){ //no contraction is possible
        ApplyFmax[var]=NO;
     }else{
       RB[var]&=t;
//        if(Diam(RB[var])>0 && Inf(ctr_mohc.deriv_b[var])<0){
//           REAL deriv_b=-Sup(ctr_mohc.zmax)/Diam(RB[var]);       
//           if(deriv_b/Inf(ctr_mohc.deriv_b[var]) < tau_mono) ApplyFmax[var]=NO;       
//        }
     }

    
  }

  void MohcRevise::initRightNarrowFmin(INTERVAL x, int var){
     INTERVAL t=ctr_mohc.Newton_it(RB[var],Inf(RB[var]),Inf(ctr_mohc.zmin),var);
     if(!t.empty() && (Sup(t) < Sup(RB[var]))){
        apply_fmax_to_false_except(var);
        ApplyFmin[var]=YES;
	RB[var]&=t;
     }else if(t.empty()){
        ApplyFmin[var]=NO;
     }else{
       RB[var]&=t;
//        if(Diam(RB[var])>0 && Sup(ctr_mohc.deriv_a[var])>0){
//           REAL deriv_a=-Inf(ctr_mohc.zmin)/Diam(RB[var]);
//           if(deriv_a/Sup(ctr_mohc.deriv_a[var]) < tau_mono) ApplyFmin[var]=NO;
//        }
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
  
  
  INTERVAL MohcRevise::hc4Revise() {
     ctr_mohc.forward(space);
     INTERVAL z=ctr_mohc.evlog->output();
     ctr_mohc.backward(space);
     return z;
  }

  static vector<const Contractor*> vec;
//   static vector<ConstraintOG*> ctrsog;
  
  static vector<const Contractor*>& Mohc_contractor_list(const System& sys,  REAL epsilon, REAL univ_newton_min_width, REAL tau_mohc, 
    bool amohc){
  
     for(int i=0; i<sys.nb_ctr() ; i++){
       ConstraintOG* ctr_mohc=new ConstraintOG(sys.ctr(i),sys.space);
       vec.push_back(new MohcRevise(*ctr_mohc, sys.space, epsilon, univ_newton_min_width, tau_mohc, amohc));
     }
     return vec;
  }

  Mohc::Mohc(const System& sys, REAL ratio, bool incremental,  REAL epsilon, 
        REAL univ_newton_min_width, REAL activation):
	Operator(sys.space),      
        Propagation(Mohc_contractor_list(sys, epsilon, univ_newton_min_width, 
		activation, (activation==ADAPTIVE)/*amohc*/), sys.space, ratio, incremental),  
        update_active_mono_proc(activation <= 1.0){ 
	  for (vector<const Contractor*>::const_iterator it=vec.begin(); it!=vec.end(); it++)
             delete *it;
          active_mono_proc=new int[vec.size()];
          for(int i=0;i<vec.size();i++) active_mono_proc[i]=1;
          vec.clear();
  }

  Mohc::Mohc(const System& sys, REAL ratio, bool incremental,  REAL epsilon, 
        REAL univ_newton_min_width, int* active_mono_proc):
	Operator(sys.space),      
        Propagation(Mohc_contractor_list(sys, epsilon, univ_newton_min_width, 
		1.0, false), sys.space, ratio, incremental),  
        update_active_mono_proc(false), active_mono_proc(active_mono_proc){ 
	  for (vector<const Contractor*>::const_iterator it=vec.begin(); it!=vec.end(); it++)
             delete *it;
	  if(!active_mono_proc){
             active_mono_proc=new int[vec.size()];
             for(int i=0;i<vec.size();i++) active_mono_proc[i]=1;
	  }
             vec.clear();
  }
    
   
  // copy constructor

  Mohc::Mohc (const Mohc & p) :  Operator(p.space), Propagation (p), 
  update_active_mono_proc(p. update_active_mono_proc), active_mono_proc(p.active_mono_proc) {
    
  };
  
  //  constructor with all parameters
  Mohc_All::Mohc_All(const System& sys, REAL ratio, bool incremental, 
          REAL tau_mohc, REAL epsilon, REAL univ_newton_min_width, bool with3BCID, int s3b, int scid, 
          int vhandled, REAL var_min_width) : Operator(sys.space){
	   
	mohc= new Mohc(sys, ratio, incremental,  epsilon, univ_newton_min_width, tau_mohc);
	
     if(with3BCID)
       {     Mohc mohc1 (sys, ratio, true,  epsilon,  univ_newton_min_width, mohc->active_mono_proc) ; // Mohc inside 3BCID is always incremental
	 m3b = new _3BCID(sys.space, mohc1, s3b, scid, vhandled, var_min_width);}
     else 
       m3b=NULL;
  }

  
    // constructor using the default value for the paramaters, excepting prec used for univ_newton_min_width and var_min_width and
  // tau_mohc 

  Mohc_All::Mohc_All(const System& sys, bool with3BCID, REAL prec, REAL tau_mohc) : Operator(sys.space)
  {mohc = new Mohc(sys, Propagation::default_ratio, true,  Mohc::default_epsilon, prec, tau_mohc);

     if(with3BCID)
       { Mohc mohc1 (sys, Propagation::default_ratio, true,   Mohc::default_epsilon,  prec, mohc->active_mono_proc) ; // Mohc inside 3BCID is always incremental
	 m3b = new _3BCID(sys.space, mohc1, _3BCID::default_s3b, _3BCID::default_scid, sys.nb_var(), prec);}
     else 
       m3b=NULL;
  }

  Mohc_All::Mohc_All (const Mohc_All & c) :  
  Operator(c.space) , Contractor (c)  { 
    m3b=(c.m3b)? c.m3b->copy() : NULL; 
    mohc=c.mohc->copy();};
  
  void Mohc_All::contract(){
     mohc->space.sync(space);
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
