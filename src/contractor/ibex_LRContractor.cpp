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
 
 #include "ibex_LRContractor.h"

using namespace std;
namespace ibex {

const double LR_contractor::default_ratio_fp = 0.1;
const double LR_contractor::default_ratio_fp2 = 0.01;
const double LR_contractor::default_max_diam_box =1e4;


  LR_contractor::LR_contractor(const System& sys, Ctc* ctc, int goal_ctr,Function* fgoal,
				double ratio_fp, double ratio_fp2, ctc_mode cmode, int max_iter_soplex, double max_diam_box) : 

     Ctc(sys.nb_var), sys(sys), ctc(ctc? ctc:NULL), goal_ctr(goal_ctr),
     ratio_fp(ratio_fp), ratio_fp2(ratio_fp2),  cmode(cmode),  max_iter_soplex(max_iter_soplex), max_diam_box(max_diam_box),
     LinearCoef(sys.nb_ctr, sys.nb_var)

 {
    /* get the goal function from the constraint y=f(x) */
   if(goal_ctr!=-1){goal=fgoal;} 
    // ============================================================
    
   linear = new bool[sys.nb_ctr];


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
  

  double LR_contractor::ratio_maxreduction(IntervalVector& box1, IntervalVector& box2){
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
  
void  LR_contractor::contract( IntervalVector & box) {
   double gain;

   if (box.max_diam() > max_diam_box) return; // is it necessary?  YES (BNE) Soplex can give false infeasible results with large numbers
   
   do{
     IntervalVector savebox=box;
     iter(box);

     if(ctc) {
       gain = ratio_maxreduction(savebox,box);
       if (gain >= ratio_fp2) ctc->contract(box);
     }

     gain = ratio_maxreduction(savebox,box);
   }
   while(gain >= ratio_fp);

}


void LR_contractor::iter(IntervalVector & box){

  int n=sys.nb_var;
  
  soplex::SoPlex mysoplex;
  
  IntervalVector initbox=box;
  //returns the number of constraints in the linearized system
  int nb_ctrs = Linearization(box,mysoplex);
 // cout << nb_ctrs << endl;
  if(nb_ctrs<1)  return;
  
  try{
    optimizer(box, mysoplex, n, nb_ctrs);
  }catch(EmptyBoxException e){
     throw EmptyBoxException();
  }  
  

}

  void LR_contractor::optimizer(IntervalVector & box, soplex::SoPlex& mysoplex, int n, int nb_ctrs){

  Interval opt(0);
  int inf_bound[n]; // indicator inf_bound = 1 means the inf bound is feasible or already contracted , call to simplex useless (cf Baharev)
  int sup_bound[n]; //indicator sup_bound = 1 means the sup bound is feasible or already contracted, call to simplex useless

  for (int i=0; i<n ;i++) {inf_bound[i]=0;sup_bound[i]=0;}
  if (goal_ctr !=-1)   sup_bound[n-1]=1;  // in case of optimization, for y the left bound only is contracted
  // in the case of lower_bounding, only the left bound of y is contracted
  int firsti=(cmode==ONLY_Y)? 2*n-1:0;

  int nexti=-1;  // the next variable to be contracted
  int infnexti=0; // the bound to be contracted contract  infnexti=0 for the lower bound, infnexti=1 for the upper bound

  for(int ii=firsti;ii<2*n;ii++){  // at most 2*n calls                                                                 
    int i= ii/2;
    if (nexti != -1) i=nexti;
    //    cout << " i "<< i << " infnexti " << infnexti << " infbound " << inf_bound[i] << " supbound " << sup_bound[i] << endl;
    if (infnexti==0 && inf_bound[i]==0)
      {
	inf_bound[i]=1;
	soplex::SPxSolver::Status stat = run_simplex(box,mysoplex, soplex::SPxLP::MINIMIZE, i, n, opt,box[i].lb()/*, taylor_ev*/);
	//	cout << " stat " <<  stat << endl;
        if( stat == soplex::SPxSolver::OPTIMAL ){

          if(opt.lb()>box[i].ub())   throw EmptyBoxException();
          choose_next_variable(box,mysoplex ,nexti,infnexti, inf_bound, sup_bound);
          if(opt.lb() > box[i].lb() ){
            box[i]=Interval(opt.lb(),box[i].ub());
            mysoplex.changeLhs(nb_ctrs+i,opt.lb());
          }
        }
        else if(stat == soplex::SPxSolver::INFEASIBLE) throw EmptyBoxException();
        else if (stat == soplex::SPxSolver::UNKNOWN)
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
	soplex::SPxSolver::Status stat= run_simplex(box,mysoplex, soplex::SPxLP::MAXIMIZE, i, n, opt, box[i].ub()/*, taylor_ev*/);
        if( stat == soplex::SPxSolver::OPTIMAL ){

          if(opt.ub() <box[i].lb()) throw EmptyBoxException();
          choose_next_variable(box,mysoplex ,nexti,infnexti, inf_bound, sup_bound);
	  if (opt.ub() < box[i].ub()) {
	    box[i] =Interval( box[i].lb(), opt.ub());
	    mysoplex.changeRhs(nb_ctrs+i,opt.ub());
	  }
        } else if(stat == soplex::SPxSolver::INFEASIBLE)  throw EmptyBoxException();
        else if (stat == soplex::SPxSolver::UNKNOWN)
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





  soplex::SPxSolver::Status LR_contractor::run_simplex(IntervalVector& box,soplex::SoPlex& mysoplex, soplex::SPxLP::SPxSense sense, int var, int n, \
					  Interval& obj, double bound){
    //  mysoplex.writeFile("dump.lp", NULL, NULL, NULL);                                                                    

    int nb_rows=mysoplex.nRows()-n;

    if(sense==soplex::SPxLP::MINIMIZE)
      mysoplex.changeObj(var, 1.0);
    else
      mysoplex.changeObj(var, -1.0);



    soplex::SPxSolver::Status stat;
    mysoplex.changeSense(soplex::SPxLP::MINIMIZE);
    mysoplex.setTerminationIter(max_iter_soplex);
    mysoplex.setDelta(1e-10);
    //    mysoplex.writeFile("dump.lp", NULL, NULL, NULL);              
    //    system("cat dump.lp");
    try{
      stat = mysoplex.solve();
    }catch(soplex::SPxException){
      stat = soplex::SPxSolver::UNKNOWN;
    }

    if(stat == soplex::SPxSolver::OPTIMAL){
      if( (sense==soplex::SPxLP::MINIMIZE && mysoplex.objValue()<=bound) ||
	  (sense==soplex::SPxLP::MAXIMIZE && -mysoplex.objValue()>=bound) )
	stat = soplex::SPxSolver::UNKNOWN;
    }//else if(soplex::SPxSolver::ABORT_ITER) stat = soplex::SPxSolver::OPTIMAL;                                                          

    if(stat == soplex::SPxSolver::OPTIMAL){

      IntervalMatrix As (mysoplex.nRows(),n);
      IntervalVector C(n);
      for(int i=0;i<n;i++){
	C[i]=0;
	if(i==var){
	  if(sense==soplex::SPxLP::MINIMIZE)
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
	B[i]=Interval((mysoplex.lhs()[i]!=-soplex::infinity)? mysoplex.lhs()[i]:-1.e20,(mysoplex.rhs()[i]!=soplex::infinity)?  
		      mysoplex.rhs()[i]:1.e20); 
	//Idea: replace 1e20 (resp. -1e20) by Sup([g_i]) (resp. Inf([g_i])), where [g_i] is an evaluation of the nonlinear function <-- IA 

	//           cout << B(i+1) << endl;                                                                                    
      }

      soplex::DVector dual(mysoplex.nRows());
      mysoplex.getDual(dual);
      // Shchberbina - Neumaier postprocessing                                                                            
      IntervalVector Lambda(mysoplex.nRows());
      for (int i =0; i< mysoplex.nRows() ; i++)
	{if( 
	    (B[i].ub()==1e+20 && dual[i]<0 ) || (B[i].lb() ==-1e+20 && dual[i]>0 )) //Modified by IA  
	    Lambda[i]=0;
	  else
	    Lambda[i]=dual[i];
	}

      IntervalVector Rest(n);
      IntervalMatrix AsTranspose (n, mysoplex.nRows());
      for (int i =0; i< mysoplex.nRows() ; i++)
	for (int j= 0; j<n ;j++)
	  AsTranspose[j][i]= As[i][j];
      Rest = AsTranspose *Lambda - C;


      if(sense==soplex::SPxLP::MINIMIZE)
	obj = Lambda * B - Rest * box;
      else
	obj = -(Lambda * B - Rest * box);

      //     cout << sense << endl;                                                                                           
      //     cout << obj << endl;                                                                                             
      //     cout << mysoplex.objValue()<< endl;                                                                              

    }
    mysoplex.changeObj(var, 0.0);
    return stat;
  }






  bool LR_contractor::isInner(IntervalVector & box,const System& sys, int j){
    Interval eval=sys.ctrs[j].f.eval(box);

    if((sys.ctrs[j].op==LEQ && eval.ub() > 0) || (sys.ctrs[j].op==LT && eval.ub() >= 0) ||
       (sys.ctrs[j].op==GEQ && eval.lb() < 0) || (sys.ctrs[j].op==GT && eval.lb() <= 0))
      return false;
    return true;
  }



  // The Achterberg heuristic for choosing the next variable (nexti) and its bound (infnexti) to be contracted (cf Baharev paper)
  // and updating the indicators if a bound has been found feasible (with the precision prec_bound)

  void LR_contractor::choose_next_variable ( IntervalVector & box, soplex::SoPlex& mysoplex , int & nexti, int & infnexti, int* inf_bound, int* sup_bound)
  {
    double prec_bound = 1.e-8; // relative precision for the indicators                                                 
    int n=sys.nb_var;
    soplex::DVector primal(n);
    mysoplex.getPrimal(primal);
    double delta=1.e100;
    double deltaj=delta;
    for (int j=0;j<n;j++)

      { 
	//	cout << " j " << j << " infbound " << inf_bound[j]    << " supbound " << sup_bound[j] << endl;  
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
          {     deltaj = fabs (primal[j]- box[j].ub());
            if ((fabs (box[j].ub()) < 1 && deltaj < prec_bound)
            ||
                (fabs (box[j].ub()) >= 1 && fabs (deltaj/(box[j].ub())) < prec_bound))
	      sup_bound[j]=1;
	    if (sup_bound[j]==0 && deltaj < delta)
	      {nexti=j; infnexti=1;delta=deltaj;}
	  }

      }
    //    cout << " nexti " <<nexti <<  " infnexti " << infnexti << endl ;   
  }





}
