#include <vector>
#include "mohc/IbexConstraintOG.h"
#include "IbexExprAdjFactory.h"
#include "IbexEvaluator.h"
#include "IbexEvaluatorFactory.h"
#include "IbexOperatorPrinter.h"

namespace ibex {


/* -------------------------------------------------------------------------------------------*/
hash_map<int, REAL> ConstraintOG::h;



ConstraintOG::ConstraintOG(const ArithConstraint& ctr, Space &space) : 
      expr(ctr.expr), rho_mohc(1.1),
 op((dynamic_cast<const Inequality*>(&ctr))? (dynamic_cast<const Inequality*>(&ctr))->op:EQU){
 adj = ctr.adj;
 evlog=new EvaluatorOG();

 EvaluatorFactory fac(expr);

 fac.build((Evaluator*)evlog);


//partial derivatives of each variable
  deriv=new INTERVAL[space.nb_var()];
  deriv_a=new INTERVAL[space.nb_var()];
  deriv_b=new INTERVAL[space.nb_var()];

  
  
//partial derivatives of each occurrence of variable
  derivs=new INTERVAL*[space.nb_var()];

//number of occurrences of variables
  nb_occ=new int[space.nb_var()];

//vocc2cocc[x][i] obtains the id of the i-th occurrence of x in the constraint.
  vocc2cocc=  new int*[space.nb_var()];
  var2entity= new int[space.nb_var()];
int*  entity2var= new int[space.nb_var()];
  
  int i=0;
  has_occ_mult=false;
  for (hash_map<int,int>::const_iterator it = adj.begin(); it!=adj.end(); it++) {
     int v = it->first; 
 
     if (space.entity(v).type!=IBEX_VAR) continue;
     var2entity[i]=v;
     entity2var[v]=i;
     if(it!=adj.end()){
          nb_occ[i] = it->second;
	
          derivs[i]=new INTERVAL[nb_occ[i]];
          vocc2cocc[i]=new int[nb_occ[i]];
     }else{
          nb_occ[i] = 0;
     }

     has_occ_mult=(has_occ_mult||nb_occ[i]>1);
     i++;
  }
  nb_var=i;
 
  
  //the ic2var and ic2occ arrays are initialized (only once). Also for r_a,r_b,r_c.
  nb_occ_ctr=evlog->initForwardOG(space,nb_var,entity2var);
  delete[] entity2var;
  if(nb_occ_ctr!=-1){
     r_a=new INTERVAL[nb_occ_ctr];
     r_b=new INTERVAL[nb_occ_ctr];
     r_c=new INTERVAL[nb_occ_ctr];
  }

}

//obtains the arrays r_a, r_b, r_c that minimize G
void ConstraintOG::Occurrence_Grouping(int var, bool y_set, bool og){
   for(int occ=0;occ<nb_occ[var];occ++){
      r_a[vocc2cocc[var][occ]]=0;
      r_b[vocc2cocc[var][occ]]=0;
      r_c[vocc2cocc[var][occ]]=1;
   }
   deriv_a[var]=0;
   deriv_b[var]=0;
   
   //only variables with multiple occurrences are treated
   if(y_set && nb_occ[var]<=1) return;

   if(Inf(deriv[var]) >= 0 || Sup(deriv[var]) <= 0){ //G_0 does not contain 0
      OG_case1(var);
      return;
   }
   
   if(!og) return;
   


   INTERVAL* g=derivs[var];
   INTERVAL G_plus=0, G_minus=0;

   list<int> X_m, X_nm;

     
   for(int occ=0;occ<nb_occ[var];occ++){ //FOR EACH occurrence j

      if (Inf(g[occ])>=0){
         X_m.push_back(occ);
         G_plus += g[occ];
      }else if(Sup(g[occ])<=0){
         X_m.push_back(occ);
         G_minus += g[occ];
      }else{
         X_nm.push_back(occ);
      }
   }

   INTERVAL G_m=G_plus + G_minus;


   
   if(Inf(G_m)<0 && Sup(G_m)>0){ //G_m contains 0
      OG_case2(var, g, Inf(G_plus), Inf(G_minus), Sup(G_plus), Sup(G_minus));
      deriv_a[var]=INTERVAL(0,Sup(G_m));
      deriv_b[var]=INTERVAL(Inf(G_m),0);
   }else{
      OG_case3(var, g, X_m, X_nm, G_m);
      if(Sup(G_m)>0) deriv_a[var]=G_m;
      else deriv_b[var]=G_m;
   }
   
}

void ConstraintOG::OG_case1(int var){
   if(Inf(deriv[var])>=0){
      for(int occ=0;occ<nb_occ[var];occ++){ //FOR EACH occurrence occ
         r_a[vocc2cocc[var][occ]]=1;
         r_c[vocc2cocc[var][occ]]=0;
      }
   }else if(Sup(deriv[var])<=0){
      for(int occ=0;occ<nb_occ[var];occ++){ //FOR EACH occurrence occ
         r_b[vocc2cocc[var][occ]]=1;
         r_c[vocc2cocc[var][occ]]=0;
      }
   }
}

void ConstraintOG::OG_case2(int var, INTERVAL* g, INTERVAL inf_G_Xa, INTERVAL inf_G_Xb, INTERVAL sup_G_Xa, INTERVAL sup_G_Xb){
         INTERVAL m1=( inf_G_Xa*sup_G_Xb + sup_G_Xb*inf_G_Xb )/
               ( inf_G_Xa*sup_G_Xb - inf_G_Xb*sup_G_Xa );
          
         INTERVAL m2=( inf_G_Xa*sup_G_Xa + sup_G_Xb*inf_G_Xa )/
               ( inf_G_Xa*sup_G_Xb - inf_G_Xb*sup_G_Xa );

         for(int occ=0;occ<nb_occ[var];occ++){
            if(Inf(g[occ])>=0){
               r_a[vocc2cocc[var][occ]]=1-m1;
               r_b[vocc2cocc[var][occ]]=m1;
               r_c[vocc2cocc[var][occ]]=0;
            }else if(Sup(g[occ])<=0){
               r_a[vocc2cocc[var][occ]]=m2;
               r_b[vocc2cocc[var][occ]]=1-m2;
               r_c[vocc2cocc[var][occ]]=0;
            }
         }
}



void ConstraintOG::OG_case3(int var, INTERVAL* g, list<int>& X_m, list<int>& X_nm, INTERVAL& G_m){

   if(Inf(G_m)>=0){
      for (list<int>::iterator occ = X_nm.begin(); occ != X_nm.end(); occ++)
         h[*occ]=-Sup(g[*occ])/Inf(g[*occ]);
   }else if(Sup(G_m)<=0){
      for (list<int>::iterator occ = X_nm.begin(); occ != X_nm.end(); occ++)
         h[*occ]=-Inf(g[*occ])/Sup(g[*occ]);
   }
   X_nm.sort(Isort());

   //the monotone occurrences are grouped in xa or xb
   if(Inf(G_m)>=0){
      for (list<int>::iterator occ = X_m.begin(); occ != X_m.end(); occ++){
         r_a[vocc2cocc[var][*occ]]=1;
         r_c[vocc2cocc[var][*occ]]=0;
      }
   }else{
      for (list<int>::iterator occ = X_m.begin(); occ != X_m.end(); occ++){
         r_b[vocc2cocc[var][*occ]]=1;
         r_c[vocc2cocc[var][*occ]]=0;
      }
   }

   bool sat=true;
   
   //the non monotone occurrences are put in xa or xb, until the constraint is unsatisfied
   if(Inf(G_m)>=0){
      for (list<int>::iterator occ = X_nm.begin(); occ != X_nm.end(); occ++){
         INTERVAL m=(sat)? 1:0;
         if(sat && Inf(G_m)+Inf(g[*occ])<0){
            m=-INTERVAL(Inf(G_m))/INTERVAL(Inf(g[*occ]));
            sat=false;
         }
         r_a[vocc2cocc[var][*occ]]=m;
         r_c[vocc2cocc[var][*occ]]=1-m;
         G_m+=g[*occ];
      }
   }else{
      for (list<int>::iterator occ = X_nm.begin(); occ != X_nm.end(); occ++){
         INTERVAL m=(sat)? 1:0;
         if(sat && Sup(G_m)+Sup(g[*occ])>0){
            m=-INTERVAL(Sup(G_m))/INTERVAL(Sup(g[*occ]));
            sat=false;
         }
         r_b[vocc2cocc[var][*occ]]=m;
         r_c[vocc2cocc[var][*occ]]=1-m;
         G_m+=g[*occ];
      }
   }
}

  REAL ConstraintOG::compute_rho_mohc(Space& space, bool existence_test){
     REAL diam_mon, diam_nat;
     rho_mohc=1;
     if(hasMultOcc() && gradientOG(space)){
        forwardOG(space, true);
        forwardOG(space, false);
        INTERVAL ev_mon=INTERVAL(Inf(zmin),Sup(zmax));
        if(existence_test)
	  switch (op) { 
	    case EQU      : if(!ev_mon.contains(0)) throw EmptyBoxException();
	           break;
	    case LT       : 
	    case LEQ      : if (Inf(zmin) >= 0) throw EmptyBoxException();
                  break;
	    case GEQ      : 
	    case GT       : if (Sup(zmax) <= 0) throw EmptyBoxException();
	}
//        if(existence_test && !ev_mon.contains(0)) 
//           throw EmptyBoxException();

        diam_mon=Diam(ev_mon);
        diam_nat=Diam(eval(space));
        if(diam_nat>0){
           rho_mohc=diam_mon/diam_nat;

        }
        
     }

     is_monotonic=true;
     for(int i=0; i<nb_var; i++){
        if ((nb_occ[i]>1) && (Inf(deriv[i])>=0 || Sup(deriv[i])<=0) ) is_monotonic=false;
     }

     return rho_mohc;
  }

INTERVAL ConstraintOG::evalOG(Space& space){
   if(gradientOG(space)){
     forwardOG(space, true);
     forwardOG(space, false);
     return INTERVAL(Inf(zmin),Sup(zmax));
   }

   return eval(space);
}

void ConstraintOG::forwardOG(Space& space, bool minRevise) {
   evlog->forwardOG(space,r_a,r_b,r_c,minRevise);
   if(minRevise)
      zmin=evlog->output();
   else
      zmax=evlog->output();
}


void ConstraintOG::backwardOG(Space& space, bool minRevise) {
   bool sat;
   if(minRevise){
      if(zmin.included(INTERVAL(BiasNegInf,0))) return;
      sat = evlog->output() &= INTERVAL(BiasNegInf,0);
   }else{
      if(zmax.included(INTERVAL(0,BiasPosInf))) return;
      sat = evlog->output() &= INTERVAL(0,BiasPosInf);
   }

   if (!sat) throw EmptyBoxException();
   evlog->backwardOG(space,r_a,r_b,r_c,minRevise);
}

void ConstraintOG::backward2OG(Space& space, bool minRevise) {

   bool sat=true;
   if(minRevise){
//       if(zmin.included(INTERVAL(BiasNegInf,0))) return;
      evlog->Ip[0]=last_nateval;
      sat=(evlog->Ip[0] &= INTERVAL(BiasNegInf,0));
   }else{
//       if(zmax.included(INTERVAL(0,BiasPosInf))) return;
      evlog->Ip[0]=last_nateval;
      sat=(evlog->Ip[0] &= INTERVAL(0,BiasPosInf));
   }

   if (!sat) throw EmptyBoxException();


   try{
     evlog->backwardOG(space,r_a,r_b,r_c,minRevise);
     //evlog->backward2OG(space,r_a,r_b,r_c,minRevise,deriv,derivs);
   }catch(EmptyBoxException e){
     throw e;
   }
}


void ConstraintOG::gradient(Space& space){
   evlog->gradient(space,deriv,derivs,vocc2cocc);
   last_nateval=evlog->output();
}


} // end namespace ibex
