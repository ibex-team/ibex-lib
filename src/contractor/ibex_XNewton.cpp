//============================================================================
//                                  I B E X                                   
// File        : ibex_XNewton.cpp
// Author      : Ignacio Araya, Bertrand Neveu , Gilles Trombettoni
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Jul 1, 2012
// Last Update : Nov 15, 2012
//============================================================================

#include "ibex_XNewton.h"

using std::vector;

namespace ibex {

const double X_Newton::default_max_diam_deriv =1e5;

  using namespace std;

using namespace soplex;

// the constructor
X_Newton::X_Newton(const System& sys, Ctc* ctc, vector<corner_point>& cpoints, int goal_ctr, Function* fgoal, 
     		   double ratio_fp, double ratio_fp2,
		   ctc_mode cmode, linear_mode lmode, int max_iter_soplex, double max_diam_deriv, double max_diam_box):
  LR_contractor(sys,ctc,goal_ctr,fgoal,ratio_fp,ratio_fp2,cmode,max_iter_soplex,max_diam_box), cpoints(cpoints),
  max_diam_deriv(max_diam_deriv), lmode(lmode){

  last_rnd = new int[sys.nb_var];
  base_coin = new int[sys.nb_var]; 
 
		}






  /*  pas implanté en version 2 

void X_Newton::best_corner(int ctr, int op, INTERVAL_VECTOR& G, bool* corner){
   int n=sys.nb_var;

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
   catch (EmptyBoxException& )
     {cout  << endl; space.box=savebox; return 0;}

   space.box=savebox;
   return Mid(ev);

}
  */

 

  int X_Newton::X_Linearization(IntervalVector& box, soplex::SoPlex& mysoplex, int ctr, corner_point cpoint, vector<Interval>& taylor_ev,
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
int X_Newton::X_Linearization(IntervalVector& box, soplex::SoPlex& mysoplex,
		int ctr, corner_point cpoint, CmpOp op, vector<Interval>& taylor_ev,
		IntervalVector& G, int id_point, int& nb_nonlinear_vars) {
	int n = sys.nb_var;
	int nonlinear_var = 0;

	if (id_point != 0 && linear[ctr])
		return 0; // only one corner for a linear constraint
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

	if (goal_ctr == ctr) {
		row1.add(n - 1, -1.0);
	}
	for (int j = 0; j < n; j++) {
		if (j == n - 1 && goal_ctr != -1)
			continue; //the variable y!

		if (sys.ctrs[ctr].f.used(j)) {
			if (lmode == HANSEN && !linear[ctr]) {
				if (ctr == goal_ctr) // objective function in optimization
						{
					IntervalVector box1(sys.nb_var - 1);
					for (int i = 0; i < sys.nb_var - 1; i++)
						box1[i] = box[i];
					IntervalVector G1(sys.nb_var - 1);
					goal->gradient(box1, G1);
					for (int i = 0; i < sys.nb_var - 1; i++)
						G[i] = G1[i];
					G[sys.nb_var - 1] = 0;
				}

				else
					sys.ctrs[ctr].f.gradient(box, G);
			}
		} else
			continue;

		if (G[j].diam() > max_diam_deriv) {
			box = savebox;
			return 0; //to avoid problems with SoPleX
		}

		if (linear[ctr])
			cpoint = INF_X;
		else if (G[j].diam() > 1e-10)
			nonlinear_var++;

		bool inf_x;

		/*  for GREEDY jeuristics :not implemented in v2
		 IntervalVector save;
		 double fh_inf, fh_sup;
		 double D;
		 */

		switch (cpoint) {
		/*
		 case BEST:
		 inf_x=corner[j]; last_rnd[j]=inf_x? 0:1;  break;
		 */
		case INF_X:
			inf_x = true;
			break;
		case SUP_X:
			inf_x = false;
			break;
		case RANDOM:
			last_rnd[j] = rand();
			inf_x = (last_rnd[j] % 2 == 0);
			break;
		case K4:

			if (id_point == 0) {
				inf_x = (rand() % 2 == 0);
				base_coin[j] = inf_x;
			} else if (nb_nonlinear_vars < 3) {
				if (id_point == 1)
					inf_x = !base_coin[j];
				else
					return 0;
			} else if (G[j].diam() <= 1e-10) {
				inf_x = (rand() % 2 == 0);
			} else if (id_point == 1) {
				if (((double) nonlinear_var) <= (((double) nb_nonlinear_vars )/ 3.0))
					inf_x = base_coin[j];
				else
					inf_x = !base_coin[j];
			} else if (id_point == 2) {
				if (((double) nonlinear_var )> ((double) nb_nonlinear_vars )/ 3.0
						&& (double) nonlinear_var
								<= 2 * (double) nb_nonlinear_vars / 3.0)
					inf_x = base_coin[j];
				else
					inf_x = !base_coin[j];
			} else if (id_point == 3) {
				if (((double) nonlinear_var)
						> 2 * ((double) nb_nonlinear_vars) / 3.0)
					inf_x = base_coin[j];
				else
					inf_x = !base_coin[j];
			}
			break;
		case RANDOM_INV:
			inf_x = (last_rnd[j] % 2 != 0);
			break;
		case NEG:
			inf_x = (last_rnd[j] % 2 != 0);
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
		default:
			last_rnd[j] = rand();
			inf_x = (last_rnd[j] % 2 == 0);
			break;

      }

      //      cout << " j " << j <<  " " << savebox[j] << G[j] << endl;
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



//the X_NewtonIteration
  int X_Newton::Linearization( IntervalVector & box, soplex::SoPlex& mysoplex){
  int n = sys.nb_var;
  int nvarsimplex=n;

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
      

       if(goal_ctr==ctr)  // objective function  in optimization
	 {IntervalVector box1(sys.nb_var-1);
	  for (int i=0; i<sys.nb_var-1; i++)
	    box1[i]=box[i];
	  IntervalVector G1(sys.nb_var-1);
	   goal->gradient(box1,G1);
	   for (int i=0; i<sys.nb_var-1; i++)
	     G[i]=G1[i];
	   G[sys.nb_var-1]=0;
	 }

       else
	 sys.ctrs[ctr].f.gradient(box,G);
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

 return nb_ctrs;
 }




 
}
