/*---------------------------------------------------------------------------------
 * XNewton Contractor
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
#include "optim/IbexOptimizer.h"

namespace ibex {

const REAL X_Newton::default_ratio_fp = 0.1;
const REAL X_Newton::default_ratio_fp2 = 0.01;
const REAL X_Newton::default_var_min_width = 1.e-11;
const REAL X_Newton::default_max_diam_deriv =1e5;
const REAL X_Newton::default_max_diam_box =1e4;



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
  /*
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
  */

void X_Newton::contract() {
 REAL gain;
 if (space.box.max_diameter() > max_diam_box) return;
 // if (space.box.max_diameter() > 10*max_diam_deriv) return;
 do{

    INTERVAL_VECTOR savebox=space.box;
    REAL saveobj= Inf (savebox(space.nb_var()));
    X_NewtonIter();

    if(ctc) {
       gain = ratio_maxreduction(savebox,space.box);
       if (gain >= ratio_fp2) ctc->contract(space,Indicators(ALL_VARS,ALL_VARS));
       // if (gain >= ratio_fp2) ctc->contract(space,Indicators(space.nb_var()-1,ALL_VARS));
     }
		       
    gain = ratio_maxreduction(savebox,space.box);
    // variant : gain on objective only
    /*
    if (fabs(saveobj) > 1) 
      gain = (Inf (space.box(space.nb_var())) - saveobj)/fabs(saveobj);
    else      
      gain = Inf (space.box(space.nb_var())) - saveobj;
    */

 }while(gain >= ratio_fp);
 
}


void X_Newton::best_corner(int ctr, int op, INTERVAL_VECTOR& G, bool* corner){
   int n=space.nb_var();
    //0000000000
   int total_backtracks=1; int nb_var=0;
   for(int i=0;i<n;i++){
      corner[i]=false;
      if(i==n-1 && goal_ctr!=-1) continue;
      if(!isvar[ctr][i]) continue;
      if(Diam(G(i+1))<=1e-11) continue; //linear variable
      total_backtracks*=2;
      nb_var++;
   }

   REAL best_eval=eval_corner(ctr, op, G, corner);
   bool corner_tmp[n];

   for(int i=1;i<total_backtracks;i++){
      int ii=i;

      for(int j=0;j<nb_var;j++){
         if(j==n-1 && goal_ctr!=-1) continue;
         if(!isvar[ctr][j]) continue;
         if(Diam(G(j+1))<=1e-11) continue; //linear variable
         corner_tmp[j]=(ii%2==1);
         ii/=2;
      }
      REAL eval=eval_corner(ctr, op, G, corner_tmp);
      if(eval>best_eval){
         best_eval=eval;
         for(int k=0;k<n;k++){
            corner[k]=corner_tmp[k];
         }
      }
   }
}


REAL X_Newton::eval_corner(int ctr, int op, INTERVAL_VECTOR& G, bool* corner){
   INTERVAL_VECTOR savebox(space.box);
   INTERVAL ev(0.0);
   int n=space.nb_var();

   for (int j=0; j<n; j++){
      if(j==n-1 && goal_ctr!=-1) continue; //the variable y! 
      if(!isvar[ctr][j]) continue;
      if(Diam(G(j+1))> max_diam_deriv)
	{
         space.box=savebox;
         return 0; //to avoid problems with SoPleX
      }
      bool inf_x=corner[j];
      space.box(j+1)=inf_x? Inf(savebox(j+1)):Sup(savebox(j+1));
      INTERVAL a = ((inf_x && (op == LEQ || op== LT)) ||
            (!inf_x && (op == GEQ || op== GT)))? Inf(G(j+1)):Sup(G(j+1));
      ev+=(inf_x)? 0.5*a*Diam(savebox(j+1)):-0.5*a*Diam(savebox(j+1)); 
   }

    //f(x^c)
   if(ctr==goal_ctr){
      goal->forward(space);
      ev+=goal->output();
   }else{
      ev+=sys.ctr(ctr).eval(space);
          
   }
   space.box=savebox;
   return Mid(ev);

}

  int X_Newton::X_Linearization(SoPlex& mysoplex, int ctr, corner_point cpoint, vector<INTERVAL>& taylor_ev,
INTERVAL_VECTOR& G, int id_point, int& nb_nonlinear_vars){
  int op=(dynamic_cast<const Inequality*>(&sys.ctr(ctr)))? (dynamic_cast<const Inequality*>(&sys.ctr(ctr)))->op:EQU;

  if(op!=EQU && isInner(sys, ctr)) return 0; //the constraint is satified

    int cont=0;
    if(ctr==goal_ctr) op = LEQ;
    if(op==EQU){
       cont+=X_Linearization(mysoplex, ctr, cpoint, LEQ, taylor_ev, G, id_point, nb_nonlinear_vars);
       cont+=X_Linearization(mysoplex, ctr, cpoint, GEQ, taylor_ev, G, id_point, nb_nonlinear_vars);
       
    }else
       cont+=X_Linearization(mysoplex, ctr, cpoint, op, taylor_ev, G, id_point, nb_nonlinear_vars);
    return cont;
}

//return 0 only when the linearization is not performed
int X_Newton::X_Linearization(SoPlex& mysoplex, int ctr, corner_point cpoint, int op, vector<INTERVAL>& taylor_ev,
INTERVAL_VECTOR& G, int id_point, int& nb_nonlinear_vars){

    int n=space.nb_var();
    int nonlinear_var=0;
    
    if(id_point!=0 && linear[ctr]) return 0;
    //Linear variable y<=loup (should be y<=Sup(y)?)
    
    
    bool* corner=NULL;

    if(!linear[ctr]){
       if(cpoint==BEST){
          corner= new bool[n];
          best_corner(ctr, op, G, corner);
       }
    }
    
    INTERVAL_VECTOR savebox(space.box);
    INTERVAL ev(0.0);
    INTERVAL tot_ev(0.0);
    DSVector row1(n);

    if(ctr==goal_ctr){
       row1.add(n-1, -1.0);
    }
    try{
    for (int j=0; j<n; j++){
          if(j==n-1 && goal_ctr!=-1) continue; //the variable y! 
             
          if(isvar[ctr][j]){
           if(lmode==HANSEN && !linear[ctr]){
            for (int jj=0; jj<n; jj++) {
              G(jj+1) = 0.0;
              space.ent(IBEX_VAR,jj).deriv = &G(jj+1);     
            }
            if(ctr==goal_ctr)
              goal->gradient(space);
            else
              sys.ctr(ctr).gradient(space);
	    }
         }
	  else continue;

	  if(Diam(G(j+1))> max_diam_deriv ){
              space.box=savebox;
              return 0; //to avoid problems with SoPleX
          }
          
          

          if(linear[ctr])
             cpoint=INF_X;
          else if(Diam(G(j+1))>1e-10)
	     nonlinear_var++;
	  
          bool inf_x;
	  INTERVAL_VECTOR save;
	  REAL fh_inf, fh_sup;
	  REAL D;
	  

          switch(cpoint){
             case BEST:
                inf_x=corner[j]; last_rnd[j]=inf_x? 0:1;  break;
	     case INF_X:
	       inf_x=true; break;
             case SUP_X:
	       inf_x=false; break;
             case RANDOM:
	       last_rnd[j]=rand(); inf_x=(last_rnd[j]%2==0); 
	       break;
	     case K4:

	       if(id_point==0){
		  inf_x=(rand()%2==0);
		  base_coin[j]=inf_x;
	       }else if(nb_nonlinear_vars < 3){
		 if(id_point==1) inf_x = !base_coin[j];
		 else return 0;
	       }else if(Diam(G(j+1))<=1e-10 ){
		 inf_x=(rand()%2==0);
	       }else if(id_point==1){
		 if((REAL)nonlinear_var <= (REAL)nb_nonlinear_vars / 3.0)
		   inf_x=base_coin[j];
		 else
		   inf_x=!base_coin[j];
	       }else if(id_point==2){
		 if((REAL)nonlinear_var > (REAL)nb_nonlinear_vars / 3.0 && 
		   (REAL)nonlinear_var <= 2*(REAL)nb_nonlinear_vars / 3.0)
		   inf_x=base_coin[j];
		 else
		   inf_x=!base_coin[j];		 
	       }else if(id_point==3){
		 if((REAL)nonlinear_var > 2*(REAL)nb_nonlinear_vars / 3.0)
		   inf_x=base_coin[j];
		 else
		   inf_x=!base_coin[j];			 
	       }
	     break;
	     case RANDOM_INV:
	       inf_x=(last_rnd[j]%2!=0);
	       break;
             case NEG:
	       inf_x=(last_rnd[j]%2!=0);
	       break;
             case GREEDY1:
	       inf_x=((abs(Inf(G(j+1))) < abs(Sup(G(j+1))) && (op == LEQ || op== LT)) ||
                        (abs(Inf(G(j+1))) >= abs(Sup(G(j+1))) && (op == GEQ || op== GT))  )? true:false;
	       break;
	  case MONO:
	    if (ctr==goal_ctr && ((Inf(G(j+1)) >0 && (op == LEQ || op== LT)) || 
				  (Sup (G(j+1)) < 0 && (op == GEQ || op== GT))))
	      inf_x = true;
	    else if 
	      (ctr == goal_ctr  &&
	      ((Sup(G(j+1)) <0 && (op == LEQ || op== LT)) || 
	       (Inf(G(j+1)) > 0 && (op == GEQ || op== GT))))
	      inf_x=false;
	    else inf_x = (rand()%2==0);
	    last_rnd[j]=inf_x? 0:1;
	    break;
	  case NEGMONO:
	    if ((Inf(G(j+1)) >0 && (op == LEQ || op== LT)) || 
		(Sup (G(j+1)) < 0 && (op == GEQ || op== GT)))
	      inf_x = false;
	    else if 
	      ((Sup(G(j+1)) <0 && (op == LEQ || op== LT)) || 
	       (Inf(G(j+1)) > 0 && (op == GEQ || op== GT)))
	      inf_x=true;
	    else inf_x = (rand()%2==0);
	    last_rnd[j]=inf_x? 0:1;
	    break;


	    

		  /*
              case GREEDY7:
                 //select the coin nearest to the loup 
                 if(goal_ctr!=-1 && Dimension(Optimizer::global_optimizer())>0){
//                     if(j==0)cout<< Optimizer::global_optimizer()<<end;
                    inf_x=(abs(Optimizer::global_optimizer()(j+1)-Inf(savebox(j+1))) < 
                      abs(Optimizer::global_optimizer()(j+1)-Sup(savebox(j+1))))? true:false;
                 }else{ 
                   inf_x=(rand()%2==0);
             07/0 07/05/20115/2011     }
                 break;
		  */
             case GREEDY6:
                 save=space.box;
                 fh_inf, fh_sup;
                 D=Diam(savebox(j+1));
                 space.box=Mid(space.box);
                 space.box(j+1)=Inf(savebox(j+1));
                 fh_inf=Mid(sys.ctr(ctr).eval(space));
                 space.box(j+1)=Sup(savebox(j+1));
                 fh_sup=Mid(sys.ctr(ctr).eval(space));
                 if (op == LEQ || op== LT)
                    inf_x=((fh_sup-fh_inf)/(REAL)(n-1) - 0.5*D*(Inf(G(j+1))+Sup(G(j+1)))  > 0)? false:true;
                 else
                    inf_x=((fh_sup-fh_inf)/(REAL)(n-1) - 0.5*D*(Inf(G(j+1))+Sup(G(j+1))) < 0)? false:true;
		 last_rnd[j]=inf_x? 0:1;  
		 space.box=save;
              break;
             case GREEDY5:
                 save=space.box;
                 fh_inf, fh_sup;
                 D=Diam(savebox(j+1));
                 space.box=Mid(space.box);
                 space.box(j+1)=Inf(savebox(j+1));
                 fh_inf=Mid(sys.ctr(ctr).eval(space));
                 space.box(j+1)=Sup(savebox(j+1));
                 fh_sup=Mid(sys.ctr(ctr).eval(space));
                 if (op == LEQ || op== LT)
                    inf_x=(fh_sup-fh_inf - 0.5*D*(Inf(G(j+1))+Sup(G(j+1)))  > 0)? false:true;
                 else
                    inf_x=(fh_sup-fh_inf - 0.5*D*(Inf(G(j+1))+Sup(G(j+1))) < 0)? false:true;
		 last_rnd[j]=inf_x? 0:1;  
		 space.box=save;
              break;


          }
         
//          if(!linear[ctr] && Diam(G(j+1))>1e-10) cout << inf_x;
         space.box(j+1)=inf_x? Inf(savebox(j+1)):Sup(savebox(j+1));
         INTERVAL a = ((inf_x && (op == LEQ || op== LT)) ||
                            (!inf_x && (op == GEQ || op== GT)))? Inf(G(j+1)):Sup(G(j+1));
         row1.add(j, Mid(a)); //Inf(a)=Mid(a)=Sup(a)
         ev-=a*space.box(j+1); 
        
    }
    }
   catch (UnboundedResultException e)
   {  if(corner) delete[] corner; 
      space.box=savebox; return 0;}
      
      if(corner) delete[] corner;
      
    if(ctr==goal_ctr){
          goal->forward(space);
          ev+=goal->output();
    }else{
          INTERVAL tmp=sys.ctr(ctr).eval(space);
          ev+=tmp;
    }


   if(id_point==0) nb_nonlinear_vars=nonlinear_var;
  
   
   for(int j=0;j<n;j++)
      tot_ev+=row1[j]*savebox(j+1); //natural evaluation of the left side of the linear constraint


  bool added=false;
    if(op == LEQ || op== LT){
        //g(xb) + a1' x1 + ... + an xn <= 0
      if(Inf(tot_ev)>Sup(-ev))   throw EmptyBoxException(); //the constraint is not satisfied
        if(Sup(-ev)<Sup(tot_ev)){ //otherwise the constraint is satisfied for any point in the box
           mysoplex.addRow(LPRow(-infinity, row1, Sup(-ev)));
           added=true;
        }
    }else{
      if(Sup(tot_ev)<Inf(-ev))  throw EmptyBoxException();
       if(Inf(-ev)>Inf(tot_ev)){
          mysoplex.addRow(LPRow(Inf(-ev), row1, infinity));
          added=true;
       }
    }
    if(added)  taylor_ev.push_back(tot_ev);

    space.box=savebox;
    
    
    return (added)? 1:0;
  


  }

  // The Achterberg heuristic for choosing the next variable (nexti) and its bound (infnexti) to be contracted (cf Baharev paper)
  // and updating the indicators if a bound has been found feasible (with the precision prec_bound)

  void X_Newton::choose_next_variable ( SoPlex& mysoplex , int & nexti, int & infnexti, int* inf_bound, int* sup_bound)
  {
    REAL prec_bound = 1.e-8; // relative precision for the indicators
    int n=space.nb_var();
    DVector primal(n);
    mysoplex.getPrimal(primal);
    REAL delta=1.e100;
    REAL deltaj=delta;
    for (int j=0;j<n;j++)
      
      {
	if (inf_bound[j]==0)
	  {deltaj= fabs (primal[j]- Inf(space.box(j+1)));
	    if ((fabs (Inf(space.box(j+1))) < 1 && deltaj < prec_bound)
		||
		(fabs (Inf(space.box(j+1))) >= 1 && fabs (deltaj /Inf(space.box(j+1))) < prec_bound))
	      inf_bound[j]=1;
	    if (inf_bound[j]==0 && deltaj < delta)
	      {nexti=j; infnexti=0;delta=deltaj;}
	  }
	if (sup_bound[j]==0)
	  {	deltaj = fabs (primal[j]- Sup(space.box(j+1)));
	if ((fabs (Sup(space.box(j+1))) < 1 && deltaj < prec_bound)
	    ||
	    (fabs (Sup(space.box(j+1))) >= 1 && fabs (deltaj/Sup(space.box(j+1))) < prec_bound))
	  sup_bound[j]=1;
	if (sup_bound[j]==0 && deltaj < delta)
	  {nexti=j; infnexti=1;delta=deltaj;}
      }

      }
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
    if(lmode==TAYLOR || linear[ctr]){ //derivatives are computed once (Taylor)
     for (int jj=0; jj<n; jj++) {
              G(jj+1) = 0.0;
              space.ent(IBEX_VAR,jj).deriv = &G(jj+1);     
            }
     try{
            if(ctr==goal_ctr)
              goal->gradient(space);
            else
              sys.ctr(ctr).gradient(space);
     }
     catch (UnboundedResultException e) {return;}
   }

    int nb_nonlinear_vars;
//        cout << "coins:" << endl;
    if(cpoints[0]==K4){
	 for(int j=0; j<4; j++)
          nb_ctrs+=X_Linearization(mysoplex, ctr, K4, taylor_ev, G, j, nb_nonlinear_vars);
    }else
       for(int k=0;k<cpoints.size();k++){
	   nb_ctrs+=X_Linearization(mysoplex, ctr, cpoints[k], taylor_ev, G, k, nb_nonlinear_vars);
// 	cout << endl;
     }
     
  }


  for (int j=0; j<n; j++){
        DSVector row1(n);
        row1.add (j,1.0);
        mysoplex.addRow(LPRow(Inf(space.box(j+1)), row1, Sup(space.box(j+1))));

  }

  if(nb_ctrs<1)return;

  /***************************************************/

  INTERVAL opt(0);
  int inf_bound[n]; // indicator inf_bound = 1 means the inf bound is feasible or already contracted , call to simplex useless (cf Baharev)
  int sup_bound[n];// indicator sup_bound = 1 means the sup bound is feasible or already contracted, call to simplex useless

  for (int i=0; i<n ;i++) {inf_bound[i]=0;sup_bound[i]=0;}
  if (goal_ctr !=-1)   sup_bound[n-1]=1;  // in case of optimization, for y the left bound only is contracted
  // in the case of lower_bounding, only the left bound of y is contracted
  int firsti=(cmode==LOWER_BOUNDING)? 2*n-1:0;

  int nexti=-1;  // the next variable to be contracted
  int infnexti=0; // the bound to be contracted contract  infnexti=0 for the lower bound, infnexti=1 for the upper bound
  for(int ii=firsti;ii<2*n;ii++){  // at most 2*n calls
    int i = ii/2; 
    if (nexti != -1) i=nexti;
    if (infnexti==0 && inf_bound[i]==0)
      {  
	  inf_bound[i]=1;
	SPxSolver::Status stat = run_simplex(mysoplex, SPxLP::MINIMIZE, i, n, opt,Inf(space.box(i+1)), taylor_ev);
	if( stat == SPxSolver::OPTIMAL ){

	  if(Inf(opt)>Sup(space.box(i+1)))    throw EmptyBoxException();
	  choose_next_variable(mysoplex ,nexti,infnexti, inf_bound, sup_bound);
	
	  if(Inf(opt) > Inf(space.box(i+1)) ){
	    space.box(i+1)=INTERVAL( Inf(opt),Sup(space.box(i+1) ));
	    mysoplex.changeLhs(nb_ctrs+i,Inf(opt));
	  }
	}
	else if(stat == SPxSolver::INFEASIBLE) throw EmptyBoxException();
	else if (stat == SPxSolver::UNKNOWN)
	  {int next=-1;
	    for (int j=0;j<n;j++)
	      {if (inf_bound[j]==0) {nexti=j; next=0;infnexti=0;break;}
	      else if  (sup_bound[j]==0) {nexti=j;next=0; infnexti=1;break;}
	      }
	    if (next==-1) break;
	  }

		    
      }
    else
      if(infnexti==1 && sup_bound[i]==0 ){ 
        //max x
	  sup_bound[i]=1;
      SPxSolver::Status stat= run_simplex(mysoplex, SPxLP::MAXIMIZE, i, n, opt, Sup(space.box(i+1)), taylor_ev);
        if( stat == SPxSolver::OPTIMAL ){

	  if(Sup(opt)<Inf(space.box(i+1))) throw EmptyBoxException();
	 choose_next_variable(mysoplex ,nexti,infnexti, inf_bound, sup_bound);
        
	 if(Sup(opt) < Sup(space.box(i+1)) ){
	   space.box(i+1)=INTERVAL( Inf(space.box(i+1)), Sup(opt));
	   mysoplex.changeRhs(nb_ctrs+i,Sup(opt));
           }
        } else if(stat == SPxSolver::INFEASIBLE)  throw EmptyBoxException();
	else if (stat == SPxSolver::UNKNOWN)
	  {int next=-1;
	    for (int j=0;j<n;j++)
	      {if (inf_bound[j]==0) {nexti=j; next=0;infnexti=0;break;}
	      else if  (sup_bound[j]==0) {nexti=j; next=0;infnexti=1;break;}
	      }
	    if (next==-1) break;
	  }


      }
      else break;  // no more call to soplex
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
  mysoplex.setTerminationIter(max_iter_soplex);
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
  }//else if(SPxSolver::ABORT_ITER) stat = SPxSolver::OPTIMAL;
     

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
