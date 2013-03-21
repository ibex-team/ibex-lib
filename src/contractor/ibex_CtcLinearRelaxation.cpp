//============================================================================
//                                  I B E X
// Linear Relaxation Contractor                                   
// File        : ibex_CtcLinearRelaxation.cpp     
// Author      : Bertrand Neveu , Gilles Trombettoni
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Nov 14, 2012
// Last Update : March 21, 2013
//============================================================================ 

 #include "ibex_CtcLinearRelaxation.h"

using namespace std;

using namespace soplex;
namespace ibex {


  const double CtcLinearRelaxation::default_max_diam_box =1e4;
    

  CtcLinearRelaxation::CtcLinearRelaxation(const System& sys, int goal_ctr,Function* fgoal,
				 ctc_mode cmode, int max_iter_soplex, double max_diam_box) : 

     Ctc(sys.nb_var), sys(sys),  goal_ctr(goal_ctr),
     cmode(cmode),  max_iter_soplex(max_iter_soplex), max_diam_box(max_diam_box),
     LinearCoef(sys.nb_ctr, sys.nb_var)

 {
    /* in case of optimization the objective function */
   if(goal_ctr!=-1){goal=fgoal;} 
    // ============================================================
    
   linear = new bool[sys.nb_ctr];
   

    for(int ctr=0; ctr<sys.nb_ctr;ctr++){

      IntervalVector G(sys.nb_var);
      
      if(ctr==goal_ctr)
	{IntervalVector box1(sys.nb_var-1);
	  for (int i=0; i<sys.nb_var-1; i++)
	    box1[i]=sys.box[i];

	  
	  IntervalVector G1(sys.nb_var-1);
	  goal->gradient(box1,G1);
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
  


void CtcLinearRelaxation::contract (IntervalVector & box){
  if (box.max_diam() > max_diam_box) return; // is it necessary?  YES (BNE) Soplex can give false infeasible results with large numbers
  int n=sys.nb_var;
  
  SoPlex mysoplex;
  
  IntervalVector initbox=box;
  //returns the number of constraints in the linearized system
  int nb_ctrs;
  try {nb_ctrs = Linearization(box,mysoplex);
    if(nb_ctrs<1)  return;
    optimizer(box, mysoplex, n, nb_ctrs);
  }  
  catch(EmptyBoxException& e){box.set_empty(); // empty the box before exiting in case of EmptyBoxException
    throw EmptyBoxException();
  }  
  

}

  void CtcLinearRelaxation::optimizer(IntervalVector & box, SoPlex& mysoplex, int n, int nb_ctrs){

  Interval opt(0);
  int* inf_bound = new int[n]; // indicator inf_bound = 1 means the inf bound is feasible or already contracted , call to simplex useless (cf Baharev)
  int* sup_bound = new int[n]; //indicator sup_bound = 1 means the sup bound is feasible or already contracted, call to simplex useless

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
	SPxSolver::Status stat = run_simplex(box,mysoplex, SPxLP::MINIMIZE, i, n, opt,box[i].lb()/*, taylor_ev*/);
	//	cout << " stat " <<  stat << endl;
        if( stat == SPxSolver::OPTIMAL ){

          if(opt.lb()>box[i].ub())  throw EmptyBoxException();
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
	SPxSolver::Status stat= run_simplex(box,mysoplex, SPxLP::MAXIMIZE, i, n, opt, box[i].ub()/*, taylor_ev*/);
        if( stat == SPxSolver::OPTIMAL ){

          if(opt.ub() <box[i].lb())   throw EmptyBoxException();
          choose_next_variable(box, mysoplex ,nexti,infnexti, inf_bound, sup_bound);
	  if (opt.ub() < box[i].ub()) {
	    box[i] =Interval( box[i].lb(), opt.ub());
	    mysoplex.changeRhs(nb_ctrs+i,opt.ub());
	  }
        } 
        else if(stat == SPxSolver::INFEASIBLE)  throw EmptyBoxException();
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

  delete [] inf_bound;
  delete [] sup_bound;
    
  }





  SPxSolver::Status CtcLinearRelaxation::run_simplex(IntervalVector& box,SoPlex& mysoplex, SPxLP::SPxSense sense, int var, int n, \
					  Interval& obj, double bound){
    //  mysoplex.writeFile("dump.lp", NULL, NULL, NULL);                                                                    

    if(sense==SPxLP::MINIMIZE)
      mysoplex.changeObj(var, 1.0);
    else
      mysoplex.changeObj(var, -1.0);



    SPxSolver::Status stat;
    mysoplex.changeSense(SPxLP::MINIMIZE);
    mysoplex.setTerminationIter(max_iter_soplex);
    mysoplex.setDelta(1e-10);
    //    mysoplex.writeFile("dump.lp", NULL, NULL, NULL);              
    //    system("cat dump.lp");
    try{
      stat = mysoplex.solve();
    }catch(SPxException&){
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
	B[i]=Interval((mysoplex.lhs()[i]!=-infinity)? mysoplex.lhs()[i]:-1.e20,(mysoplex.rhs()[i]!=infinity)?  
		      mysoplex.rhs()[i]:1.e20); 
	//Idea: replace 1e20 (resp. -1e20) by Sup([g_i]) (resp. Inf([g_i])), where [g_i] is an evaluation of the nonlinear function <-- IA 

	//           cout << B(i+1) << endl;                                                                                    
      }

      DVector dual(mysoplex.nRows());
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


      if(sense==SPxLP::MINIMIZE)
	obj = Lambda * B - Rest * box;
      else
	obj = -(Lambda * B - Rest * box);

      //     cout << sense << endl;                                                                                           
      //     cout << obj << endl;                                                                                             
      //     cout << mysoplex.objValue()<< endl;                                                                              

    }
    mysoplex.changeObj(var, 0.0);
    //    cout << "stat " << stat << endl;
    return stat;
  }






  bool CtcLinearRelaxation::isInner(IntervalVector & box,const System& sys, int j){
    Interval eval=sys.ctrs[j].f.eval(box);

    if((sys.ctrs[j].op==LEQ && eval.ub() > 0) || (sys.ctrs[j].op==LT && eval.ub() >= 0) ||
       (sys.ctrs[j].op==GEQ && eval.lb() < 0) || (sys.ctrs[j].op==GT && eval.lb() <= 0))
      return false;
    return true;
  }



  // The Achterberg heuristic for choosing the next variable (nexti) and its bound (infnexti) to be contracted (cf Baharev paper)
  // and updating the indicators if a bound has been found feasible (with the precision prec_bound)

  void CtcLinearRelaxation::choose_next_variable ( IntervalVector & box, SoPlex& mysoplex , int & nexti, int & infnexti, int* inf_bound, int* sup_bound)
  {
    double prec_bound = 1.e-8; // relative precision for the indicators                                                 
    int n=sys.nb_var;
    DVector primal(n);
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
