/*---------------------------------------------------------------------------------
 * XNewton Contractor
 * ----------------------------------------------------------------------------
 *
 * Authors  Ignacio Araya, Gilles Trombettoni, Bertrand Neveu
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


#include "ibex_XNewton.h"

using std::vector;

namespace ibex {

const double X_Newton::default_ratio_fp = 0.1;
const double X_Newton::default_ratio_fp2 = 0.1;
const double X_Newton::default_max_diam_deriv =1e5;
const double X_Newton::default_max_diam_box =1e4;


using namespace soplex;

// the constructor
X_Newton::X_Newton(const System& sys, Ctc* ctc, vector<corner_point>& cpoints, int goal_ctr, Function* fgoal, 
     		   double ratio_fp, double ratio_fp2,
		   ctc_mode cmode, linear_mode lmode, int max_iter_soplex, double max_diam_deriv, double max_diam_box) : 
  Ctc(sys.nb_var), sys(sys), ctc(ctc? ctc:NULL), goal_ctr(goal_ctr), 
  ratio_fp(ratio_fp), ratio_fp2(ratio_fp2),  cmode(cmode), cpoints(cpoints), lmode(lmode), max_iter_soplex(max_iter_soplex),
  max_diam_deriv(max_diam_deriv), max_diam_box(max_diam_box),
  LinearCoef(sys.nb_ctr, sys.nb_var){
   if(goal_ctr!=-1)
     {goal= fgoal;}

    // ============================================================
  linear = new bool[sys.nb_ctr];
  last_rnd = new int[sys.nb_var];
  base_coin = new int[sys.nb_var]; 

  for(int ctr=0; ctr<sys.nb_ctr;ctr++){
    
     IntervalVector G(sys.nb_var);


     if(ctr==goal_ctr)
       {IntervalVector G1(sys.nb_var-1);
       goal->gradient(sys.box,G1);
       for (int i=0; i<sys.nb_var-1; i++)
	 G[i]=G1[i];
       G[sys.nb_var-1]=0;
       }
     else
       sys.ctrs[ctr].f.gradient(sys.box,G);
    
     linear[ctr]=true;
     // for testing if a function is linear (with scalar coefficients) w.r.t all its variables, we test the diameter of the gradient components.
     for(int i=0;i<sys.nb_var;i++){
       if (G[i].diam() >1e-10) {
	 linear[ctr]=false;
	 break;
       }
       
     }
     LinearCoef[ctr]=G;  // in case of a linear function, the coefficients are stored.
  }



  }


double ratio_maxreduction(IntervalVector& box1, IntervalVector& box2){
   double maxratio=0.0;
   for(int i=0;i<box1.size();i++){
      double ratio=0.0;

      if(box1[i].diam()>1e-20)
     ratio=1.0-box2[i].diam()/box1[i].diam();

      if(ratio>maxratio)
        maxratio=ratio;
  }
  return maxratio;
}

  bool X_Newton::isInner(IntervalVector & box,const System& sys, int j){
        Interval eval=sys.ctrs[j].f.eval(box);

        if((sys.ctrs[j].op==LEQ && eval.ub() > 0) || (sys.ctrs[j].op==LT && eval.ub() >= 0) ||
	   (sys.ctrs[j].op==GEQ && eval.lb() < 0) || (sys.ctrs[j].op==GT && eval.lb() <= 0))
	  return false;
	return true;
  }


void X_Newton::contract(IntervalVector & box) {
 double gain;
 // cout << " box avant " << box << endl;
 if (box.max_diam() > max_diam_box) 
   return;
 do{

    IntervalVector savebox=box;

    X_NewtonIter(box);

    if(ctc) {
       gain = ratio_maxreduction(savebox,box);
       if (gain >= ratio_fp2) ctc->contract(box);
     }
		       
    gain = ratio_maxreduction(savebox,box);

 }while(gain >= ratio_fp);
 //   cout << " box apres " << box << endl;
}

  /*  pas implanté en version 2 

void X_Newton::best_corner(int ctr, int op, INTERVAL_VECTOR& G, bool* corner){
   int n=sys.nb_var;
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
   INTERVAL_VECTOR savebox(sys.box);
   INTERVAL ev(0.0);
   int n=sys.nb_var;

   for (int j=0; j<n; j++){
      if(j==n-1 && goal_ctr!=-1) continue; //the variable y! 
      if(!isvar[ctr][j]) continue;
      
      if(Diam(G(j+1))> max_diam_deriv)
	{
         sys.box=savebox;
         return 0; //to avoid problems with SoPleX
	}
      
      bool inf_x=corner[j];
      sys.box[j]=inf_x? Inf(savebox(j+1)):Sup(savebox(j+1));
      INTERVAL a = ((inf_x && (op == LEQ || op== LT)) ||
            (!inf_x && (op == GEQ || op== GT)))? Inf(G(j+1)):Sup(G(j+1));
      ev+=(inf_x)? 0.5*a*Diam(savebox(j+1)):-0.5*a*Diam(savebox(j+1)); 
   }

    //f(x^c)
   try {
   if(ctr==goal_ctr){
      goal->forward(space);
      ev+=goal->output();
   }else{
      ev+=sys.ctr(ctr).eval(space);
          
   }
   }
   catch (EmptyBoxException e)
     {cout  << endl; space.box=savebox; return 0;}

   space.box=savebox;
   return Mid(ev);

}
  */

 

int X_Newton::X_Linearization(IntervalVector& box, SoPlex& mysoplex, int ctr, corner_point cpoint, vector<Interval>& taylor_ev,
			      IntervalVector& G, int id_point, int& nb_nonlinear_vars){

    CmpOp op= sys.ctrs[ctr].op;

    if(op!=EQ && isInner(box, sys, ctr)) return 0; //the constraint is satified

    int cont=0;
    if(ctr==goal_ctr) op = LEQ;
    if(op==EQ){
      cont+=X_Linearization(box,mysoplex, ctr, cpoint, LEQ, taylor_ev, G, id_point, nb_nonlinear_vars);
      cont+=X_Linearization(box,mysoplex, ctr, cpoint, GEQ, taylor_ev, G, id_point, nb_nonlinear_vars);
       
    }else
      cont+=X_Linearization(box,mysoplex, ctr, cpoint, op, taylor_ev, G, id_point, nb_nonlinear_vars);
    return cont;
}

//return 0 only when the linearization is not performed
  int X_Newton::X_Linearization(IntervalVector& box ,SoPlex& mysoplex, int ctr, corner_point cpoint, CmpOp op, 
				vector<Interval>& taylor_ev,
				IntervalVector& G, int id_point, int& nb_nonlinear_vars){
    int n=sys.nb_var;
    int nonlinear_var=0;
    
    if(id_point!=0 && linear[ctr]) return 0;  // only one corner for a linear constraint
    //Linear variable y<=loup (should be y<=Sup(y)?)
       
    

    /*
      bool* corner=NULL;
    if(!linear[ctr]){
      // best not implemented in version 2.0 BNE
      
       if(cpoint==BEST){
          corner= new bool[n];
          best_corner(ctr, op, G, corner);
       }

    }
    */    
    IntervalVector savebox(box);
    Interval ev(0.0);
    Interval tot_ev(0.0);
    DSVector row1(n);

    if(goal_ctr==ctr){
       row1.add(n-1, -1.0);
    }
    for (int j=0; j<n; j++){
      if(j==n-1 && goal_ctr!=-1) continue; //the variable y! 
             
      if(sys.ctrs[ctr].f.used(j)){
	  if(lmode==HANSEN && !linear[ctr]){
	    if(ctr==goal_ctr) // objective function in optimization	    
	      {IntervalVector G1(sys.nb_var-1);
		goal->gradient(box,G1);
		for (int i=0; i<sys.nb_var-1; i++)
		  G[i]=G1[i];
		G[sys.nb_var-1]=0;
	      }

	    else
	      sys.ctrs[ctr].f.gradient(box,G);
	  }
      }
      else continue;
      
      if(G[j].diam()> max_diam_deriv ){
	box=savebox;
	return 0; //to avoid problems with SoPleX
      }
                

      if(linear[ctr])
	cpoint=INF_X;
      else if(G[j].diam()>1e-10)
	nonlinear_var++;
	  
      bool inf_x;

      /*  for GREEDY jeuristics :not implemented in v2
	  IntervalVector save;
	  double fh_inf, fh_sup;
	  double D;
      */

	  
      switch(cpoint){
	    /*
             case BEST:
                inf_x=corner[j]; last_rnd[j]=inf_x? 0:1;  break;
	    */
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
	       }else if(G[j].diam()<=1e-10 ){
		 inf_x=(rand()%2==0);
	       }else if(id_point==1){
		 if((double)nonlinear_var <= (double)nb_nonlinear_vars / 3.0)
		   inf_x=base_coin[j];
		 else
		   inf_x=!base_coin[j];
	       }else if(id_point==2){
		 if((double)nonlinear_var > (double)nb_nonlinear_vars / 3.0 && 
		   (double)nonlinear_var <= 2*(double)nb_nonlinear_vars / 3.0)
		   inf_x=base_coin[j];
		 else
		   inf_x=!base_coin[j];		 
	       }else if(id_point==3){
		 if((double)nonlinear_var > 2*(double)nb_nonlinear_vars / 3.0)
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
	       /*  not implemented in v2.0
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

	       */
	    

		  /*
              case GREEDY7:
                 //select the coin nearest to the loup 
                 if(goal_ctr!=-1 && Dimension(Optimizer::global_optimizer())>0){
//                     if(j==0)cout<< Optimizer::global_optimizer()<<end;
                    inf_x=(abs(Optimizer::global_optimizer()(j+1)-Inf(savebox(j+1))) < 
                      abs(Optimizer::global_optimizer()(j+1)-Sup(savebox(j+1))))? true:false;
                 }else{ 
                   inf_x=(rand()%2==0);
		   }
                 break;
		  */
	       /*
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
	       */

      }

      //  cout << " j " << j <<  " " << savebox[j] << G[j] << endl;
	  box[j]=inf_x? savebox[j].lb():savebox[j].ub();
	  Interval a = ((inf_x && (op == LEQ || op== LT)) ||
			(!inf_x && (op == GEQ || op== GT)))? G[j].lb():G[j].ub();
	  row1.add(j, a.mid());
	  ev-=a*box[j]; 
        
    }
    
    // cout << " ev  " << ev << endl;
    /*  used in BEST not implemented in v2.0
	if(corner) delete[] corner;
    */
    if(ctr==goal_ctr)
      ev+= goal->eval(box);
    else
      ev+= sys.ctrs[ctr].f.eval(box);

    if(id_point==0) nb_nonlinear_vars=nonlinear_var;
  
   
    for(int j=0;j<n;j++)
      tot_ev+=row1[j]*savebox[j]; //natural evaluation of the left side of the linear constraint


  bool added=false;
    if(op ==LEQ || op== LT){
        //g(xb) + a1' x1 + ... + an xn <= 0
      if(tot_ev.lb()>(-ev).ub())   throw EmptyBoxException(); //the constraint is not satisfied
      if((-ev).ub() <tot_ev.ub()){ //otherwise the constraint is satisfied for any point in the box
	mysoplex.addRow(LPRow(-infinity, row1, (-ev).ub()));
	added=true;
      }
    }else{
      if(tot_ev.ub()<(-ev).lb())  throw EmptyBoxException();
      if((-ev).lb()>tot_ev.lb()){
	mysoplex.addRow(LPRow((-ev).lb(), row1, infinity));
          added=true;
       }
    }
    if(added)  taylor_ev.push_back(tot_ev);

    box=savebox;
    
    
    return (added)? 1:0;
  


  }


  // The Achterberg heuristic for choosing the next variable (nexti) and its bound (infnexti) to be contracted (cf Baharev paper)
  // and updating the indicators if a bound has been found feasible (with the precision prec_bound)

  void X_Newton::choose_next_variable ( IntervalVector & box, SoPlex& mysoplex , int & nexti, int & infnexti, int* inf_bound, int* sup_bound)
  {
    double prec_bound = 1.e-8; // relative precision for the indicators
    int n=sys.nb_var;
    DVector primal(n);
    mysoplex.getPrimal(primal);
    double delta=1.e100;
    double deltaj=delta;
    for (int j=0;j<n;j++)
      
      {
	if (inf_bound[j]==0)
	  {deltaj= fabs (primal[j]- box[j].lb());
	    if ((fabs (box[j].lb()) < 1 && deltaj < prec_bound)
		||
		(fabs (box[j].lb()) >= 1 && fabs (deltaj /(box[j].lb())) < prec_bound))
	      inf_bound[j]=1;
	    if (inf_bound[j]==0 && deltaj < delta)
	      {nexti=j; infnexti=0;delta=deltaj;}
	  }
	if (sup_bound[j]==0)
	  {	deltaj = fabs (primal[j]- box[j].ub());
	    if ((fabs (box[j].ub()) < 1 && deltaj < prec_bound)
	    ||
		(fabs (box[j].ub()) >= 1 && fabs (deltaj/(box[j].ub())) < prec_bound))
	  sup_bound[j]=1;
	if (sup_bound[j]==0 && deltaj < delta)
	  {nexti=j; infnexti=1;delta=deltaj;}
      }

      }
  }


//the X_NewtonIteration
void X_Newton::X_NewtonIter( IntervalVector & box){
  int n = sys.nb_var;
  int nvarsimplex=n;

  SoPlex mysoplex;
  DSVector dummycol(0);
  vector<Interval> taylor_ev;

  /*********generation of the linearized system*********/
  for (int j=0; j<nvarsimplex; j++){
	mysoplex.addCol(LPCol(0.0, dummycol, infinity, - infinity ));
  }

  int nb_ctrs=0;


  for(int ctr=0; ctr<sys.nb_ctr;ctr++){

    IntervalVector G(sys.nb_var);
    if (linear[ctr]) G= LinearCoef[ctr]; // constant derivatives are already computed
    else    if(lmode==TAYLOR){ //derivatives are computed once (Taylor)
      
     try{
       if(goal_ctr==ctr)  // objective function  in optimization
	 {IntervalVector G1(sys.nb_var-1);
	   goal->gradient(sys.box,G1);
	   for (int i=0; i<sys.nb_var-1; i++)
	     G[i]=G1[i];
	   G[sys.nb_var-1]=0;
	 }

       else
	 sys.ctrs[ctr].f.gradient(box,G);
     }
     catch (EmptyBoxException e) {return;}
   }

    int nb_nonlinear_vars;

    if(cpoints[0]==K4){
	 for(int j=0; j<4; j++)
	   nb_ctrs+=X_Linearization(box,mysoplex, ctr, K4, taylor_ev, G, j, nb_nonlinear_vars);
    }else
       for(int k=0;k<cpoints.size();k++){
	 nb_ctrs+=X_Linearization(box,mysoplex, ctr, cpoints[k], taylor_ev, G, k, nb_nonlinear_vars);

     }
     
  }


  for (int j=0; j<nvarsimplex; j++){
        DSVector row1(nvarsimplex);
        row1.add (j,1.0);
        mysoplex.addRow(LPRow(box[j].lb(), row1, box[j].ub()));

  }

  if(nb_ctrs<1)return;

  /***************************************************/

  Interval opt(0);

  int inf_bound[nvarsimplex]; // indicator inf_bound = 1 means the inf bound is feasible or already contracted , call to simplex useless (cf Baharev)
  int sup_bound[nvarsimplex];// indicator sup_bound = 1 means the sup bound is feasible or already contracted, call to simplex useless

  for (int i=0; i<nvarsimplex ;i++) {inf_bound[i]=0;sup_bound[i]=0;}

   if (goal_ctr!=-1)  sup_bound[n]=1;  // in case of optimization, for y the left bound only is contracted
  // in the case of lower_bounding, only the left bound of y is contracted
  int firsti=(cmode==LOWER_BOUNDING)? 2*n-1:0;

  int nexti=-1;  // the next variable to be contracted
  int infnexti=0; // the bound to be contracted contract  infnexti=0 for the lower bound, infnexti=1 for the upper bound
  for(int ii=firsti;ii<2*n;ii++){  // at most 2*n calls
    int i= ii/2;
    if (nexti != -1) i=nexti;
    if (infnexti==0 && inf_bound[i]==0)
      {  
	  inf_bound[i]=1;
	  SPxSolver::Status stat = run_simplex(box,mysoplex, SPxLP::MINIMIZE, i, n, opt,box[i].lb(), taylor_ev);
	if( stat == SPxSolver::OPTIMAL ){

	  if(opt.lb()>box[i].ub())   throw EmptyBoxException();
	  choose_next_variable(box,mysoplex ,nexti,infnexti, inf_bound, sup_bound);
	  if(opt.lb() > box[i].lb() ){
	    box[i]=Interval(opt.lb(),box[i].ub());
	    mysoplex.changeLhs(nb_ctrs+i,opt.lb());
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
	  SPxSolver::Status stat= run_simplex(box,mysoplex, SPxLP::MAXIMIZE, i, n, opt, box[i].ub(), taylor_ev);
        if( stat == SPxSolver::OPTIMAL ){

	  if(opt.ub() <box[i].lb()) throw EmptyBoxException();
	  choose_next_variable(box,mysoplex ,nexti,infnexti, inf_bound, sup_bound);
	 if (opt.ub() < box[i].ub()) {
	   box[i] =Interval( box[i].lb(), opt.ub());
	   mysoplex.changeRhs(nb_ctrs+i,opt.ub());
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

  SPxSolver::Status X_Newton::run_simplex(IntervalVector& box,SoPlex& mysoplex, SPxLP::SPxSense sense, int var, int n, Interval& obj, double bound, vector<Interval>& taylor_ev){
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
     
     IntervalMatrix As (mysoplex.nRows(),n);
     IntervalVector C(n);
     for(int i=0;i<n;i++){
         C[i]=0;
         if(i==var){
           if(sense==SPxLP::MINIMIZE)
              C[var]=1.0;
           else 
              C[var]=-1.0;
         }
     }

     for (int i=0;i<mysoplex.nRows(); i++){
       for(int j=0; j<n; j++)
         As[i][j]=mysoplex.rowVector(i)[j];
     }
     // cout << As << endl;

     IntervalVector B(mysoplex.nRows());
     for (int i=0;i<mysoplex.nRows(); i++){
//          cout << taylor_ev[i] << endl;
       B[i]=Interval((mysoplex.lhs()[i]!=-infinity)? mysoplex.lhs()[i]:taylor_ev[i].lb(),(mysoplex.rhs()[i]!=infinity)? mysoplex.rhs()[i]:taylor_ev[i].ub()); //Idea: replace 1e8 (resp. -1e8) by Sup([g_i]_t) (resp. Inf([g_i]_t) ) 
//           cout << B(i+1) << endl;
    }

    DVector dual(mysoplex.nRows());
    mysoplex.getDual(dual);


    // Shchberbina - Neumaier postprocessing 
    IntervalVector Lambda(mysoplex.nRows());
    for (int i =0; i< mysoplex.nRows() ; i++)
      Lambda[i]=dual[i];

    IntervalVector Rest(n);
    IntervalMatrix AsTranspose (n, mysoplex.nRows());
    for (int i =0; i< mysoplex.nRows() ; i++)
      for (int j= 0; j<n ;j++)
	AsTranspose[j][i]= As[i][j];
    Rest = AsTranspose *Lambda - C;


   if(sense==SPxLP::MINIMIZE)
     obj = Lambda * B - Rest * box;
   else 
     obj = -(Lambda * B - Rest * box);

//     cout << sense << endl;
//     cout << obj << endl;
//     cout << mysoplex.objValue()<< endl;
//     exit(0);
  }

  mysoplex.changeObj(var, 0.0);
  return stat;
 }

  // for the defaultsolver


 
}
