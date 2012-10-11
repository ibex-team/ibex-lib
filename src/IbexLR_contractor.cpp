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
 
 #include "IbexLR_contractor.h"
#include "optim/IbexOptimizer.h"

namespace ibex {

const REAL LR_contractor::default_ratio_fp = 0.1;
const REAL LR_contractor::default_ratio_fp2 = 0.01;
const REAL LR_contractor::default_var_min_width = 1.e-11;
const REAL LR_contractor::default_max_diam_box =1e4;

  /** Creates the LR_contractor
   *
   * \param sys The system
   * \param ctc (Pre)contractor. (e.g., Mohc, NULL)
   * \param goal_ctr The id of the goal function, -1 in CSPs
   * \param ratio_fp fixpoint precision for X-Newton
   * \param var_min_width The minimum width an interval must have to be contracted
   * \param cmode ALL_BOX (contracts all the box) | ONLY_Y (only improves the left bound of the variable y) 
 */
   LR_contractor::LR_contractor(const System& sys, Contractor* ctc, int goal_ctr,
				REAL ratio_fp, REAL ratio_fp2, REAL var_min_width, ctc_mode cmode, int max_iter_soplex, REAL max_diam_box) : 
    Operator(sys.space), sys(sys), ctc(ctc? ctc->copy():NULL), goal_ctr(goal_ctr), 
    ratio_fp(ratio_fp), ratio_fp2(ratio_fp2), var_min_width(var_min_width), cmode(cmode), max_iter_soplex(max_iter_soplex), max_diam_box(max_diam_box) {

    /* get the goal function from the constraint y=f(x) */
      if(goal_ctr!=-1){
       const Expr& goal_function=(((const BinOpExpr&)sys.ctr(goal_ctr).expr).right);
       EvaluatorFactory fac(goal_function);
       goal=new Evaluator();
       fac.build(goal);
      }else
        goal=NULL;

    // =============== isvar array initialization  ======================
    //isvar[i][j] indicates if the j^th variable is related to the i^th constraint

    isvar=new bool*[sys.nb_ctr()];
    for(int i=0;i<sys.nb_ctr();i++)
        isvar[i]=new bool[space.nb_var()];

    for(int i=0;i<sys.nb_ctr();i++)
      for(int j=0;j<space.nb_var();j++)
        isvar[i][j]=false;

    for(int i=0;i<sys.nb_ctr();i++){
      int nvar=0;                            
      for (hash_map<int,int>::const_iterator it = sys.ctr(i).adj.begin(); it!=sys.ctr(i).adj.end(); it++) {
        int j = it->first; 
        if (sys.space.entity(j).type!=IBEX_VAR) continue;
        isvar[i][j]=true;
        nvar++;
      }

    } 
    // ============================================================
    
    linear = new bool[sys.nb_ctr()];

    for(int ctr=0; ctr<sys.nb_ctr();ctr++){
      INTERVAL_VECTOR G(space.nb_var());
      for (int jj=0; jj<space.nb_var(); jj++) {
         G(jj+1) = 0.0;
         space.ent(IBEX_VAR,jj).deriv = &G(jj+1);     
      }
      try {
         if(ctr==goal_ctr) goal->gradient(space);
         else sys.ctr(ctr).gradient(space);
      }
      catch (UnboundedResultException e)
        {linear[ctr]=false; break;}

      linear[ctr]=true;
      for(int i=0;i<space.nb_var();i++){
        if(Diam(G(i+1))>1e-10) {
          linear[ctr]=false;
          break;
        }
      }

    }
  } 


  /** Duplicates this instance (space is passed by reference). */
     LR_contractor::LR_contractor(const LR_contractor& lrc) : Operator(lrc.space), sys(lrc.sys), 
         ctc((lrc.ctc)? lrc.ctc->copy():NULL), ratio_fp(lrc.ratio_fp), ratio_fp2(lrc.ratio_fp2),
							      var_min_width(lrc.var_min_width), goal_ctr(lrc.goal_ctr), cmode(lrc.cmode), max_iter_soplex(lrc.max_iter_soplex), max_diam_box(lrc.max_diam_box) {
       isvar=new bool*[sys.nb_ctr()];
       for(int i=0;i<sys.nb_ctr();i++)
           isvar[i]=new bool[space.nb_var()];

       for(int i=0;i<sys.nb_ctr();i++)
         for(int j=0;j<space.nb_var();j++)
           isvar[i][j]=lrc.isvar[i][j];

       /* get the goal function from the constraint y=f(x) */
       if(goal_ctr!=-1){
         const Expr& goal_function=(((const BinOpExpr&)sys.ctr(goal_ctr).expr).right);
         EvaluatorFactory fac(goal_function);
         goal=new Evaluator();
         fac.build(goal);
       }else
         goal=NULL;

      linear = new bool[sys.nb_ctr()];
      for(int ctr=0; ctr<sys.nb_ctr();ctr++)
        linear[ctr]=lrc.linear[ctr];
  }
  
  
void  LR_contractor::contract() {
   REAL gain;
   if (space.box.max_diameter() > max_diam_box) return; // is it necessary?  YES (BNE) Soplex can give false infeasible results with large numbers
   
   do{
     INTERVAL_VECTOR savebox=space.box;
     REAL saveobj= Inf (savebox(space.nb_var()));
     iter();

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


void LR_contractor::iter(){

  int n=space.nb_var();
  
  SoPlex mysoplex;
  
  INTERVAL_VECTOR initbox=space.box;
  //returns the number of constraints in the linearized system
  //cout << space.box << endl;
  int nb_ctrs = Linearization(mysoplex);
 // cout << nb_ctrs << endl;
  if(nb_ctrs<1)  return;
  
  try{
     optimizer(mysoplex, n, nb_ctrs);
  }catch(EmptyBoxException e){
     throw EmptyBoxException();
  }  
  
  Map(space.box, initbox);

//  space.box&=initbox;

}

 void LR_contractor::optimizer(SoPlex& mysoplex, int n, int nb_ctrs){



  INTERVAL opt(0);
  int inf_bound[n]; // indicator inf_bound = 1 means the inf bound is feasible or already contracted , call to simplex useless (cf Baharev)
  int sup_bound[n]; //indicator sup_bound = 1 means the sup bound is feasible or already contracted, call to simplex useless

  for (int i=0; i<n ;i++) {inf_bound[i]=0;sup_bound[i]=0;}
  if (goal_ctr !=-1)   sup_bound[n-1]=1;  // in case of optimization, for y the left bound only is contracted
  // in the case of lower_bounding, only the left bound of y is contracted
  int firsti=(cmode==ONLY_Y)? 2*n-1:0;

  int nexti=-1;  // the next variable to be contracted
  int infnexti=0; // the bound to be contracted contract  infnexti=0 for the lower bound, infnexti=1 for the upper bound
  for(int ii=firsti;ii<2*n;ii++){  // at most 2*n calls
    int i = ii/2; 
    if (nexti != -1) i=nexti;
    if (infnexti==0 && inf_bound[i]==0)
      {  
	inf_bound[i]=1;
        SPxSolver::Status stat = run_simplex(mysoplex, SPxLP::MINIMIZE, i, n, opt,Inf(space.box(i+1))/*, taylor_ev*/);
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
      SPxSolver::Status stat= run_simplex(mysoplex, SPxLP::MAXIMIZE, i, n, opt, Sup(space.box(i+1)));
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

 SPxSolver::Status LR_contractor::run_simplex(SoPlex& mysoplex, SPxLP::SPxSense sense, int var, int n, INTERVAL& obj, REAL bound){

   int nb_rows=mysoplex.nRows()-n;

   if(sense==SPxLP::MINIMIZE)
      mysoplex.changeObj(var, 1.0); 
   else
      mysoplex.changeObj(var, -1.0); 



  SPxSolver::Status stat;
  mysoplex.changeSense(SPxLP::MINIMIZE); 
  mysoplex.setTerminationIter(max_iter_soplex);
  mysoplex.setDelta(1e-10);
  
  
  //mysoplex.writeFile("dump.lp", NULL, NULL, NULL); 
  //system("cat dump.lp");

  try{
    stat = mysoplex.solve();
  }catch(SPxException){
    stat = SPxSolver::UNKNOWN;
  }

  if(stat == SPxSolver::OPTIMAL){ //the optimum is lower than [y] (no contraction!)
     if( (sense==SPxLP::MINIMIZE && mysoplex.objValue()<=bound) || 
    (sense==SPxLP::MAXIMIZE && -mysoplex.objValue()>=bound) )   
      stat = SPxSolver::UNKNOWN;
     }//else if(SPxSolver::ABORT_ITER) stat = SPxSolver::OPTIMAL;
     

  if( stat == SPxSolver::OPTIMAL){

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
          B(i+1)=INTERVAL((mysoplex.lhs()[i]!=-infinity)? mysoplex.lhs()[i]: -1e+20,(mysoplex.rhs()[i]!=infinity)? mysoplex.rhs()[i]: 1e+20);
          //Idea: replace 1e20 (resp. -1e20) by Sup([g_i]) (resp. Inf([g_i])), where [g_i] is an evaluation of the nonlinear function <-- IA
    }

    DVector dual(mysoplex.nRows());
    mysoplex.getDual(dual);


    // Shchberbina - Neumaier postprocessing 
    INTERVAL_VECTOR Lambda(mysoplex.nRows());
    for (int i =0; i< mysoplex.nRows() ; i++){
      if( (Sup(B(i+1))==1e+20 && dual[i]<0 ) || (Inf(B(i+1))==-1e+20 && dual[i]>0 )) //Modificated by IA
	Lambda(i+1)=0;
      else
        Lambda(i+1)=dual[i];
    }

//     cout << Lambda << endl;
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
 
 

  }

  mysoplex.changeObj(var, 0.0);
  return stat;
}

//This method should be implemented (if it is not) in the Inequality class (IA)
  bool LR_contractor::isInner(const System& sys, int j){
        const  Inequality* ineq=dynamic_cast<const Inequality*> (&sys.ctr(j));
        ineq->forward(sys.space);
        INTERVAL eval=ineq->eval(sys.space);
        if((ineq->op==LEQ && Sup(eval) > 0) || (ineq->op==LT && Sup(eval) >= 0) ||
	    (ineq->op==GEQ && Inf(eval) < 0) || (ineq->op==GT && Inf(eval) <= 0)){
            
	    return false;
	}
     return true;
  }


  // The Achterberg heuristic for choosing the next variable (nexti) and its bound (infnexti) to be contracted (cf Baharev paper)
  // and updating the indicators if a bound has been found feasible (with the precision prec_bound)

  void LR_contractor::choose_next_variable ( SoPlex& mysoplex , int & nexti, int & infnexti, int* inf_bound, int* sup_bound)
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

REAL LR_contractor::ratio_maxreduction(INTERVAL_VECTOR& box1, INTERVAL_VECTOR& box2){
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

}
