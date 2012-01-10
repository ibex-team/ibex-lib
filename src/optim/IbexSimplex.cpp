#include "IbexOptimizer.h"
#include <stdlib.h>
#include "soplex.h"
using namespace soplex;

namespace ibex {

REAL abs(REAL a){
  return (a>=0)? a:-a;
}

 
//The system is relaxed by using the Taylor extension.
//Then the simplex algorithm is applied to obtain a new upper bound loup
//If a new loup is found the method returns true
// The objective is the first constraint in system sys


  bool Optimizer::simplex_update_loup(const System& sys) {


  int n=sys.space.nb_var();
  INTERVAL_VECTOR G(sys.nb_var()); // vector to be used by the partial derivatives
  INTERVAL_VECTOR savebox(sys.space.box); 
  INTERVAL_VECTOR eval_inf(sys.nb_ctr());
  INTERVAL_VECTOR eval(sys.nb_ctr());
  goal.forward(sys.space);
 

//   if(Diam(goal.output())<1e-8){
//     sys.space.box=savebox;  
//     return false;
//   }

  sys.space.box=Inf(sys.space.box);
  sys.eval(eval_inf);
  goal.forward(sys.space);


  INTERVAL inf_f = goal.output();
  sys.space.box=savebox;

  
  SoPlex mysoplex;
  /* set the objective sense */
  mysoplex.changeSense(SPxLP::MINIMIZE);  
  DSVector dummycol(0);

  //The linear system is generated
 try{
  for (int i=0; i<sys.nb_ctr();i++){
    if(i>0 && isInner(sys, i)) continue; //the constraint is satified :)

// =============== Gradient calculation ======================
      for (int j=0; j<n; j++) {
        G(j+1) = 0.0;
        sys.space.ent(IBEX_VAR,j).deriv = &G(j+1);     
      }
    if(i==goal_ctr)
      goal.gradient(sys.space);
    else
      sys.ctr(i).gradient(sys.space);
// ============================================================
//     if(G.max_diameter()>1e8) return false; //to avoid problems with SoPleX
    DSVector row1(n);

    if(i==goal_ctr){
       //Linear variable yl is created
       //0 <= yl <= loup
       mysoplex.addCol(LPCol(1.0, dummycol, infinity, -infinity ));
       row1.add(0, -1.0);
 

       for (int j=0; j<n-1; j++){
	  //The linear variables are generated
	  //0 <= xl_j <= diam([x_j])
          mysoplex.addCol(LPCol(0.0, dummycol, Diam(sys.space.box(j+1)),0.0 ));
          row1.add(j+1, Sup(G(j+1)));
       }
       //the first constraint:
       //sup(df/dx_1) xl_1 + ... + sup(df/dx_n) xl_n <= yl
       mysoplex.addRow(LPRow(-infinity, row1, 0.0));
    }else{
       int op=-1;
       if(i!=goal_ctr) op=(dynamic_cast<const Inequality*> (&sys.ctr(i)))->op;
   
       //The contraints i is generated:
      // c_i:  inf([g_i]([x]) + sup(dg_i/dx_1) * xl_1 + ... + sup(dg_i/dx_n) + xl_n  <= -eps_error
       for (int j=0; j<n-1; j++){
         if(op == LEQ || op== LT){
	    row1.add(j+1, Sup(G(j+1)));
	 }else{
            row1.add(j+1, Inf(G(j+1)));
	 }
       }
       if(op == LEQ || op== LT){
	 mysoplex.addRow(LPRow(-infinity, row1, Inf(-eval_inf(i+1))-1e-10));
       }else{
         mysoplex.addRow(LPRow(Sup(-eval_inf(i+1))+1e-10, row1, infinity)); 
       }
    }
  }

  SPxSolver::Status stat;

  mysoplex.setTerminationIter(30);
  mysoplex.setDelta(1e-10);
  try{;
    stat = mysoplex.solve();
  }catch(SPxException){
  sys.space.box=savebox;
  return false;
  }

  if( stat == SPxSolver::OPTIMAL ){
      INTERVAL f= inf_f+ mysoplex.objValue();

        //the linear solution is mapped to intervals and evaluated
        DVector prim(n);
        mysoplex.getPrimal(prim);
        for (int j=0; j<n-1; j++)
            sys.space.box(j+1)= Inf(sys.space.box(j+1))+prim[j+1];
        
        bool ret= check_candidate(sys, sys.space, Mid(sys.space.box));
	sys.space.box=savebox;
        if (ret)
	  {int prec=cout.precision();
	    cout.precision(12);
 	    cout << "[simplex1]"  << " loup update " << loup  << " loup point  " << loup_point << endl;
	    cout.precision(prec);
	  }
	return ret;
  }

  if(stat == SPxSolver::ABORT_TIME) cout << "Simplex spent too much time" << endl;
  sys.space.box=savebox;
  return false;
}
  catch (UnboundedResultException e)
    { return false;}
}



}
