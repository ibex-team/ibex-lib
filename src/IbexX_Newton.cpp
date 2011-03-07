/*---------------------------------------------------------------------------------
 * X-Newton Contractor
 * ----------------------------------------------------------------------------
 *
 * Copyright (C) 2007 Gilles Chabert
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


#include "IbexX_Newton.h"

namespace ibex {

const REAL X_Newton::default_ratio_fp = 0.1;
const REAL X_Newton::default_ratio_fp2 = 0.01;
const REAL X_Newton::default_var_min_width = 1.e-11;
const REAL X_Newton::default_max_diam_deriv =1e5;



REAL ratio_maxreduction(INTERVAL_VECTOR& box1, INTERVAL_VECTOR& box2){
   REAL maxratio=0.0;
  for(int i=0;i<Dimension(box1);i++){
      REAL ratio=0.0;

      if(Diam(box1(i+1))>1e-20)
       ratio=1.0-Diam(box2(i+1))/Diam(box1(i+1));

      if(ratio>maxratio)
        maxratio=ratio;
  }
  return maxratio;
}

  bool X_Newton::isInner(const System& sys, int j){
        const  Inequality* ineq=dynamic_cast<const Inequality*> (&sys.ctr(j));
        ineq->forward(sys.space);
        INTERVAL eval=ineq->eval(sys.space);
        if((ineq->op==LEQ && Sup(eval) > 0) || (ineq->op==LT && Sup(eval) >= 0) ||
	    (ineq->op==GEQ && Inf(eval) < 0) || (ineq->op==GT && Inf(eval) <= 0)){
            
	    return false;
	}
     return true;
  }

void X_Newton::contract() {
  //const Indicators* indic=current_indic();
  REAL gain1, gain2;

  do{
     INTERVAL_VECTOR box1=space.box;
     if(ctc) ctc->contract(space,Indicators(ALL_VARS,ALL_VARS));
     INTERVAL_VECTOR box2=space.box;
     X_NewtonIter();

     gain1 = ratio_maxreduction(box1,space.box);
     gain2 = ratio_maxreduction(box2,space.box);
  }while(gain1 >= ratio_fp && gain2 >= ratio_fp2);

}

int X_Newton::X_Linearization(SoPlex& mysoplex, int ctr, corner_point cpoint, vector<INTERVAL>& taylor_ev,
INTERVAL_VECTOR& G  ){
  int op=(dynamic_cast<const Inequality*>(&sys.ctr(ctr)))? (dynamic_cast<const Inequality*>(&sys.ctr(ctr)))->op:EQU;

  if(op!=EQU && isInner(sys, ctr)) return 0; //the constraint is satified

    int cont=0;
    if(ctr==goal_ctr) op = LEQ;
    if(op==EQU){
       cont+=X_Linearization(mysoplex, ctr, cpoint, LEQ, taylor_ev, G);
       cont+=X_Linearization(mysoplex, ctr, cpoint, GEQ, taylor_ev, G);
       
    }else
       cont+=X_Linearization(mysoplex, ctr, cpoint, op, taylor_ev, G);
    return cont;
}

//retutn 0 only when the linearization is not performed
int X_Newton::X_Linearization(SoPlex& mysoplex, int ctr, corner_point cpoint, int op, vector<INTERVAL>& taylor_ev,
INTERVAL_VECTOR& G ){

    int n=space.nb_var();

    //Linear variable y<=loup (should be y<=Sup(y)?)
    INTERVAL_VECTOR savebox(space.box);
    INTERVAL ev(0.0);
    INTERVAL tot_ev(0.0);
    DSVector row1(n);

    if(ctr==goal_ctr){
       row1.add(n-1, -1.0);
    }

    for (int j=0; j<n; j++){
          if(j==n-1 && goal_ctr!=-1) continue; //the variable y! 
             
          if(isvar[ctr][j]){
           if(lmode==HANSEN){
            for (int jj=0; jj<n; jj++) {
              G(jj+1) = 0.0;
              space.ent(IBEX_VAR,jj).deriv = &G(jj+1);     
            }
            if(ctr==goal_ctr)
              goal->gradient(space);
            else
              sys.ctr(ctr).gradient(space);
           }
         }else continue;

	  if(Diam(G(j+1))>max_diam_deriv){ //quasi-linear constraints
              space.box=savebox;
              return 0; //to avoid problems with SoPleX
          }
        
          bool inf_x;
          
          switch(cpoint){
             case INF_X:
                 inf_x=true; break;
             case SUP_X:
                 inf_x=false; break;
             case RANDOM:
                 inf_x=(rand()%2==0); break;
             case GREEDY1:
                 inf_x=((abs(Inf(G(j+1))) < abs(Sup(G(j+1))) && (op == LEQ || op== LT)) ||
                        (abs(Inf(G(j+1))) >= abs(Sup(G(j+1))) && (op == GEQ || op== GT))  )? true:false; break;
             case GREEDY5:
                 INTERVAL_VECTOR save=space.box;
                 REAL fh_inf, fh_sup;
                 REAL D=Diam(savebox(j+1));
                 space.box=Mid(space.box);
                 space.box(j+1)=Inf(savebox(j+1));
                 fh_inf=Mid(sys.ctr(ctr).eval(space));
                 space.box(j+1)=Sup(savebox(j+1));
                 fh_sup=Mid(sys.ctr(ctr).eval(space));
                 if (op == LEQ || op== LT)
                    inf_x=(fh_sup-fh_inf - 0.5*D*(Inf(G(j+1))+Sup(G(j+1)))  > 0)? false:true;
                 else
                    inf_x=(fh_sup-fh_inf - 0.5*D*(Inf(G(j+1))+Sup(G(j+1))) < 0)? false:true;
                space.box=save;
              break;
          }
 
         space.box(j+1)=inf_x? Inf(savebox(j+1)):Sup(savebox(j+1));
         INTERVAL a = ((inf_x && (op == LEQ || op== LT)) ||
                            (!inf_x && (op == GEQ || op== GT)))? Inf(G(j+1)):Sup(G(j+1));
         row1.add(j, Mid(a)); //Inf(a)=Mid(a)=Sup(a)
         ev-=a*space.box(j+1); 
        
    }
    
    if(ctr==goal_ctr){
          goal->forward(space);
          ev+=goal->output();
    }else{
          INTERVAL tmp=sys.ctr(ctr).eval(space);
          ev+=tmp;
    }



    for(int j=0;j<n;j++)
      tot_ev+=row1[j]*savebox(j+1); //natural evaluation of the left side of the linear constraint


  bool added=false;
    if(op == LEQ || op== LT){
        //g(xb) + a1' x1 + ... + an xn <= 0
        if(Inf(tot_ev)>Sup(-ev)) throw EmptyBoxException(); //the constraint is not satisfied
        if(Sup(-ev)<Sup(tot_ev)){ //otherwise the constraint is satisfied for any point in the box
           mysoplex.addRow(LPRow(-infinity, row1, Sup(-ev)));
           added=true;
        }
    }else{
        if(Sup(tot_ev)<Inf(-ev)) throw EmptyBoxException();
       if(Inf(-ev)>Inf(tot_ev)){
          mysoplex.addRow(LPRow(Inf(-ev), row1, infinity));
          added=true;
       }
    }
    if(added)  taylor_ev.push_back(tot_ev);

    space.box=savebox;
    return (added)? 1:0;
  


  }

//the X_NewtonIteration
void X_Newton::X_NewtonIter(){

  int n=space.nb_var();

  SoPlex mysoplex;
  DSVector dummycol(0);
 vector<INTERVAL> taylor_ev;

  /*********generation of the linearized system*********/
  for (int j=0; j<n; j++){
	mysoplex.addCol(LPCol(0.0, dummycol, infinity, - infinity ));
  }

  int nb_ctrs=0;



  for(int ctr=0; ctr<sys.nb_ctr();ctr++){

    INTERVAL_VECTOR G(space.nb_var());
    if(lmode==TAYLOR){ //derivatives are computed once (Taylor)
     for (int jj=0; jj<n; jj++) {
              G(jj+1) = 0.0;
              space.ent(IBEX_VAR,jj).deriv = &G(jj+1);     
            }
            if(ctr==goal_ctr)
              goal->gradient(space);
            else
              sys.ctr(ctr).gradient(space);
   }

     for(int k=0;k<cpoints.size();k++)
        nb_ctrs+=X_Linearization(mysoplex, ctr, cpoints[k], taylor_ev, G);
  }


  for (int j=0; j<n; j++){
        DSVector row1(n);
        row1.add (j,1.0);
        mysoplex.addRow(LPRow(Inf(space.box(j+1)), row1, Sup(space.box(j+1))));

  }

  if(nb_ctrs<1)return;

  /***************************************************/

  INTERVAL opt(0);
  // in the case of lower_bounding, only the left bound of y is contracted
  for(int i=(cmode==LOWER_BOUNDING)? n-1:0;i<n;i++){
      SPxSolver::Status stat = run_simplex(mysoplex, SPxLP::MINIMIZE, i, n, opt,Inf(space.box(i+1)), taylor_ev);
      if( stat == SPxSolver::OPTIMAL ){

         if(Inf(opt)>Sup(space.box(i+1))) throw EmptyBoxException();

         if(Inf(opt) > Inf(space.box(i+1)) ){
           space.box(i+1)=INTERVAL( Inf(opt),Sup(space.box(i+1) ));
           mysoplex.changeLhs(nb_ctrs+i,Inf(opt));
         }
      }else if(stat == SPxSolver::INFEASIBLE) throw EmptyBoxException();


      if(goal_ctr==-1 || (i!=n-1)){ //for any variable excepting y
        //max x
        stat= run_simplex(mysoplex, SPxLP::MAXIMIZE, i, n, opt, Sup(space.box(i+1)), taylor_ev);
//         stat = SPxSolver::UNKNOWN;
        if( stat == SPxSolver::OPTIMAL ){
         if(Sup(opt)<Inf(space.box(i+1))) throw EmptyBoxException();

           if(Sup(opt) < Sup(space.box(i+1)) ){
             space.box(i+1)=INTERVAL( Inf(space.box(i+1)), Sup(opt));
             mysoplex.changeRhs(nb_ctrs+i,Sup(opt));
           }
        } else if(stat == SPxSolver::INFEASIBLE) throw EmptyBoxException();
      }

  }

}

 SPxSolver::Status X_Newton::run_simplex(SoPlex& mysoplex, SPxLP::SPxSense sense, int var, int n, INTERVAL& obj, REAL bound, vector<INTERVAL>& taylor_ev){
//  mysoplex.writeFile("dump.lp", NULL, NULL, NULL);
   int nb_rows=mysoplex.nRows()-n;

   if(sense==SPxLP::MINIMIZE)
      mysoplex.changeObj(var, 1.0); 
   else
      mysoplex.changeObj(var, -1.0); 



  SPxSolver::Status stat;
  mysoplex.changeSense(SPxLP::MINIMIZE); 
  mysoplex.setTerminationIter(30);
  mysoplex.setDelta(1e-10);
//   mysoplex.writeFile("dump.lp", NULL, NULL, NULL);

  try{
    stat = mysoplex.solve();
  }catch(SPxException){
    stat = SPxSolver::UNKNOWN;
  }

  if(stat == SPxSolver::OPTIMAL){
     if( (sense==SPxLP::MINIMIZE && mysoplex.objValue()<=bound) || 
    (sense==SPxLP::MAXIMIZE && -mysoplex.objValue()>=bound) )
      stat = SPxSolver::UNKNOWN;
  }

  if(stat == SPxSolver::OPTIMAL){
     
     INTERVAL_MATRIX As (mysoplex.nRows(),n);
     INTERVAL_VECTOR C(n);
     for(int i=0;i<n;i++){
         C(i+1)=0;
         if(i==var){
           if(sense==SPxLP::MINIMIZE)
              C(var+1)=1.0;
           else 
              C(var+1)=-1.0;
         }
     }

     for (int i=0;i<mysoplex.nRows(); i++){
       for(int j=0; j<n; j++)
         As(i+1,j+1)=mysoplex.rowVector(i)[j];
     }
     // cout << As << endl;

     INTERVAL_VECTOR B(mysoplex.nRows());
     for (int i=0;i<mysoplex.nRows(); i++){
//          cout << taylor_ev[i] << endl;
          B(i+1)=INTERVAL((mysoplex.lhs()[i]!=-infinity)? mysoplex.lhs()[i]:Inf(taylor_ev[i]),(mysoplex.rhs()[i]!=infinity)? mysoplex.rhs()[i]:Sup(taylor_ev[i])); //Idea: replace 1e8 (resp. -1e8) by Sup([g_i]_t) (resp. Inf([g_i]_t) ) 
//           cout << B(i+1) << endl;
    }

    DVector dual(mysoplex.nRows());
    mysoplex.getDual(dual);


    // Shchberbina - Neumaier postprocessing 
    INTERVAL_VECTOR Lambda(mysoplex.nRows());
    for (int i =0; i< mysoplex.nRows() ; i++)
      Lambda(i+1)=dual[i];

    INTERVAL_VECTOR Rest(n);
    INTERVAL_MATRIX AsTranspose (n, mysoplex.nRows());
    for (int i =0; i< mysoplex.nRows() ; i++)
      for (int j= 0; j<n ;j++)
	AsTranspose(j+1,i+1)= As(i+1,j+1);
    Rest = AsTranspose *Lambda - C;


   if(sense==SPxLP::MINIMIZE)
     obj = Lambda * B - Rest * space.box;
   else 
     obj = -(Lambda * B - Rest * space.box);

//     cout << sense << endl;
//     cout << obj << endl;
//     cout << mysoplex.objValue()<< endl;
//     exit(0);
  }

  mysoplex.changeObj(var, 0.0);
  return stat;
}



}
