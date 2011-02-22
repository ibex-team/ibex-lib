#include "IbexUpdateLoup.h"
#include "IbexSimplex.h"
#include <stdlib.h>
#include "soplex.h"
using namespace soplex;

namespace ibex {

REAL abs(REAL a){
  return (a>=0)? a:-a;
}

//The system is relaxed by using the Taylor extension.
//The relaxation is performed by using as point a vertex of the box heuristically chosen.
//Then the simplex algorithm is applied to obtain a new uploup loup
//If a new loup is found the method returns true
 bool simplex_upper_bounding_at(const System& sys, const Evaluator& goal, Contractor& is_inside, REAL& loup, VECTOR& loup_point) {

  int n=sys.space.nb_var();
  INTERVAL_VECTOR G(sys.nb_var()); // vector to be used by the partial derivatives
  INTERVAL_VECTOR savebox(sys.space.box); 

  SoPlex mysoplex;
  /* set the objective sense */
  mysoplex.changeSense(SPxLP::MINIMIZE);  
  DSVector dummycol(0);

  //The linear system is generated
  for (int i=0; i<sys.nb_ctr();i++){
    if(i>0 && isInner(sys, i)) continue; //the constraint is satified :)

  // =============== Gradient calculation ======================
      for (int j=0; j<n; j++) {
        G(j+1) = 0.0;
        sys.space.ent(IBEX_VAR,j).deriv = &G(j+1);     
      }
    if(i==0)
      goal.gradient(sys.space);
    else
      sys.ctr(i).gradient(sys.space);
  // ============================================================


    DSVector row1(n);

    INTERVAL ev(0.0);
    INTERVAL_VECTOR box1(savebox);

    if(i==0){
       //Linear variable yl is created
       //-inf <= y <= loup
       mysoplex.addCol(LPCol(1.0, dummycol, loup, -infinity ));
       row1.add(0, -1.0);
 

       for (int j=0; j<n-1; j++){
	  //The linear variables are generated
	  //inf([x_j]) <= xl_j <= sup([x_j])

          mysoplex.addCol(LPCol(0.0, dummycol, Sup(savebox(j+1)),Inf(savebox(j+1)) ));
          bool left_bound=(abs(Inf(G(j+1))) < abs(Sup(G(j+1))))? true: false;
          //left_bound=true;  //to use inf([x])

          INTERVAL a=left_bound? Inf(G(j+1)):Sup(G(j+1)); //a=minBoundAbs([G_j])
          row1.add(j+1, Mid(a)); //Inf(a)=Mid(a)=Sup(a)
          box1(j+1)=left_bound? Sup(savebox(j+1)):Inf(savebox(j+1));
          ev-=a*box1(j+1);
       }
       sys.space.box=box1;
       goal.forward(sys.space);
       ev+=goal.output();
       //ev= f(x') - a1'*x1' +... + an'*xn'

       //the first constraint:
       //ev + a1' x1 + ... + an xn <= y
       mysoplex.addRow(LPRow(-infinity, row1, Inf(-ev)));
    }else{
       int op=-1;
       if(i!=0) op=(dynamic_cast<const Inequality*> (&sys.ctr(i)))->op;
   
       //The contraints i is generated:
      // c_i:  inf([g_i]([x]) + sup(dg_i/dx_1) * xl_1 + ... + sup(dg_i/dx_n) + xl_n  <= -eps_error
       for (int j=0; j<n-1; j++){

          bool left_bound=(abs(Inf(G(j+1))) < abs(Sup(G(j+1))))? true: false;
//           left_bound=(op == LEQ || op== LT);
          left_bound=rand()%2==0;

         INTERVAL a=left_bound? Inf(G(j+1)):Sup(G(j+1)); //minBoundAbs
         row1.add(j+1, Mid(a)); //Inf(a)=Mid(a)=Sup(a)
         box1(j+1)=((left_bound && (op == LEQ || op== LT)) ||
                            (!left_bound && !(op == LEQ || op== LT)))? Sup(savebox(j+1)):Inf(savebox(j+1));

         ev-=a*box1(j+1);
       }
       //ev= g(x') - a1'*x1' +... + an'*xn'
       sys.space.box=box1;
       ev+=sys.ctr(i).eval(sys.space);

       if(op == LEQ || op== LT)
           mysoplex.addRow(LPRow(-infinity, row1, Inf(-ev)-1e-10));
       else
           mysoplex.addRow(LPRow(Sup(-ev)+1e-10, row1, infinity));

    }
    sys.space.box=savebox;
  }

  SPxSolver::Status stat;

  mysoplex.setTerminationIter(30);
  mysoplex.setDelta(1e-10);

  try{
    stat = mysoplex.solve();
  }catch(SPxException){
  sys.space.box=savebox;
  return false;
  }
// mysoplex.writeFile("dump.lp", NULL, NULL, NULL);
  if( stat == SPxSolver::OPTIMAL ){
      INTERVAL f= mysoplex.objValue();

        //the linear solution is mapped to intervals and evaluated
        DVector prim(n);
        mysoplex.getPrimal(prim);
        for (int j=0; j<n-1; j++)
            sys.space.box(j+1)= prim[j+1];
        
        bool ret= check_candidate(sys, sys.space, goal, is_inside, loup, loup_point, Mid(sys.space.box), false);
	sys.space.box=savebox;
        if (ret)
	  {int prec=cout.precision();
	    cout.precision(12);
// 	    cout << "[simplex2]"  << " loup update " << loup  << " loup point  " << loup_point << endl;
	    cout.precision(prec);
	  }
	return ret;
  }

//   if(stat == SPxSolver::ABORT_TIME) cout << "Simplex spent too much time" << endl;
  sys.space.box=savebox;
  return false;

}

//The system is relaxed by using the Taylor extension.
//The relaxation is performed by using as point a vertex of the box heuristically chosen.
//Then the simplex algorithm is applied to obtain a new lower bound of [y]
//If simplex does not find any solution, the method returns false, otherwise true
bool simplex_lower_bounding_at(const System& sys, const Evaluator& goal, REAL& loup) {
  int n=sys.space.nb_var();
  INTERVAL_VECTOR G(sys.space.nb_var()); // vector to be used by the partial derivatives
  INTERVAL_VECTOR savebox=sys.space.box; 

  SoPlex mysoplex;
  /* set the objective sense */
  mysoplex.changeSense(SPxLP::MINIMIZE);  
  DSVector dummycol(0);

  //The linear system is generated
  for (int i=0; i<sys.nb_ctr();i++){
     if(i>0 && isInner(sys, i)) continue; //the constraint is satified :(
// =============== Gradient calculation ======================
      for (int j=0; j<n; j++) {
        G(j+1) = 0.0;
        sys.space.ent(IBEX_VAR,j).deriv = &G(j+1);     
      }
    if(i==0)
      goal.gradient(sys.space);
    else
      sys.ctr(i).gradient(sys.space);
// ============================================================

    DSVector row1(n);
//     DSVector row2(n);


    INTERVAL ev(0.0);
//     INTERVAL ev2(0.0);
    INTERVAL_VECTOR box1(savebox);
//     INTERVAL_VECTOR box2(box1);

    if(i==0){
       //Linear variable y<=loup (should be y<=Sup(y)?)
       mysoplex.addCol(LPCol(1.0, dummycol, loup, -infinity ));
       row1.add(0, -1.0);
//        row2.add(0, -1.0);

       for (int j=0; j<n-1; j++){
	  if(Diam(G(j+1))>1e5) return true; //to avoid problems with SoPleX

	  //The variables are generated
	  //Inf([x]) <= x <= Sup([x])
          mysoplex.addCol(LPCol(0.0, dummycol, Sup(savebox(j+1)),Inf(savebox(j+1)) ));
          bool left_bound=(abs(Inf(G(j+1))) < abs(Sup(G(j+1))))? true: false;
//           left_bound=true;

          INTERVAL a=left_bound? Inf(G(j+1)):Sup(G(j+1)); //minBoundAbs
//           INTERVAL a2=!left_bound? Inf(G(j+1)):Sup(G(j+1)); //minBoundAbs

          row1.add(j+1, Mid(a)); //Inf(a)=Mid(a)=Sup(a)
//           row2.add(j+1,Mid(a2));

           box1(j+1)=left_bound? Inf(savebox(j+1)):Sup(savebox(j+1));
//            box2(j+1)=!left_bound? Inf(savebox(j+1)):Sup(savebox(j+1));

          ev-=a*box1(j+1);
//           ev2-=a2*box2(j+1);
       }

       sys.space.box=box1;
       goal.forward(sys.space);
       ev+=goal.output();
       //ev= f(x') - a1'*x1' +... + an'*xn'

//        sys.space.box=box2;
//        goal.forward(sys.space);
//        ev2+=goal.output();

       //the first constraint:
       //ev + a1' x1 + ... + an xn <= y
       mysoplex.addRow(LPRow(-infinity, row1, Sup(-ev)));
//        mysoplex.addRow(LPRow(-infinity, row2, Sup(-ev2)));
    }else{
       int op=-1;
       if(i!=0) op=(dynamic_cast<const Inequality*> (&sys.ctr(i)))->op;
   
       //The contraints i is generated:
      // c_i:  inf([g_i]([x]) + inf(dg_i/dx_1) * xl_1 + ... + inf(dg_i/dx_n) + xl_n  <= 0
       bool add_constraint=true;
       for (int j=0; j<n-1; j++){
	 if(Diam(G(j+1))>1e5){
	   add_constraint=false; //to avoid problems with SoPleX
	   break;
	 }

          bool left_bound=(abs(Inf(G(j+1))) < abs(Sup(G(j+1))))? true: false;
//           left_bound=(op == LEQ || op== LT);

         INTERVAL a=left_bound? Inf(G(j+1)):Sup(G(j+1)); //minBoundAbs
//          INTERVAL a2=!left_bound? Inf(G(j+1)):Sup(G(j+1)); //minBoundAbs

         row1.add(j+1, Mid(a)); //Inf(a)=Mid(a)=Sup(a)
//          row2.add(j+1,Mid(a2));
         box1(j+1)=((left_bound && (op == LEQ || op== LT)) ||
                            (!left_bound && !(op == LEQ || op== LT)))? Inf(savebox(j+1)):Sup(savebox(j+1));
//          box2(j+1)=!((left_bound && (op == LEQ || op== LT)) ||
//                             (!left_bound && !(op == LEQ || op== LT)))? Inf(savebox(j+1)):Sup(savebox(j+1));


         ev-=a*box1(j+1);
//          ev2-=a2*box2(j+1);
       }
       //ev= f(x') - a1'*x1' +... + an'*xn'
       sys.space.box=box1;
       ev+=sys.ctr(i).eval(sys.space);
//        sys.space.box=box2;
//        ev2+=sys.ctr(i).eval(sys.space);

       if(add_constraint){
         if(op == LEQ || op== LT){
           mysoplex.addRow(LPRow(-infinity, row1, Sup(-ev)));
//            mysoplex.addRow(LPRow(-infinity, row2, Sup(-ev2)));
         }else{
           mysoplex.addRow(LPRow(Inf(-ev), row1, infinity));
//            mysoplex.addRow(LPRow(Inf(-ev2), row2, infinity));
         }
       }
    }
    sys.space.box=savebox;
  }

  SPxSolver::Status stat;
//    mysoplex.writeFile("dump.lp", NULL, NULL, NULL);
//   cout << "in" << endl;
  mysoplex.setTerminationIter(30);
  mysoplex.setDelta(1e-10);

  try{
    stat = mysoplex.solve();
  }catch(SPxException){
    return true;
  }
//   cout << stat << endl;
  
  if( stat == SPxSolver::OPTIMAL ){
      //The optimum solution found in the linear system is mapped to the nonlinear system
      //y_opt <-- inf([f]([x])) + yl_opt + eps_error
      //for the moment, I added an epsilon due to the nonfiability of the simplex algorithm.
      INTERVAL f= mysoplex.objValue() + INTERVAL(-1e-8,1e-8);
//       cout << "L2:" << Inf(f) << endl;
//       sys.space.box=savebox;
      if(Inf(f) > Inf(sys.space.box(n)) ){
            if(Inf(f) > Sup(sys.space.box(n)) ) return false;
            //if y_opt in [y], then we update the lower bound:
            //[y] <-- [y_opt, sup([y])]
            
           sys.space.box(n)=INTERVAL( Inf(f),Sup(sys.space.box(n) ));
      }
      return true;
  }else if(stat == SPxSolver::INFEASIBLE)
      return false;
//   if(stat == SPxSolver::ABORT_ITER) cout << "Simplex spent too much time" << endl;
  return true;

}

//The system is relaxed by using the Taylor extension.
//Then the simplex algorithm is applied to obtain a new lower bound of [y]
//If simplex does not find any solution, the method returns false, otherwise true
bool simplex_lower_bounding(const System& sys, const Evaluator& goal, REAL& loup) {
  int n=sys.space.nb_var();
  INTERVAL_VECTOR G(sys.space.nb_var()); // vector to be used by the partial derivatives
  INTERVAL_VECTOR savebox=sys.space.box; 
  INTERVAL_VECTOR eval_inf(sys.nb_ctr());
  INTERVAL_VECTOR eval_sup(sys.nb_ctr());
  
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
  for (int i=0; i<sys.nb_ctr();i++){
     if(i>0 && isInner(sys, i)) continue; //the constraint is satified :(
// =============== Gradient calculation ======================
      for (int j=0; j<n; j++) {
        G(j+1) = 0.0;
        sys.space.ent(IBEX_VAR,j).deriv = &G(j+1);     
      }
    if(i==0)
      goal.gradient(sys.space);
    else
      sys.ctr(i).gradient(sys.space);
// ============================================================

    DSVector row1(n);

    if(i==0){
       //Linear variable yl is created
       // yl <= diam([y])
       mysoplex.addCol(LPCol(1.0, dummycol, (loup - Inf(inf_f)), -infinity ));
       row1.add(0, -1.0);
 

       for (int j=0; j<n-1; j++){
	  if(Diam(G(j+1))>1e5) return true; //to avoid problems with SoPleX
	  //The linear variables are generated
	  //0 <= xl_j <= diam([x_j])
          mysoplex.addCol(LPCol(0.0, dummycol, Diam(sys.space.box(j+1)),0.0 ));
          row1.add(j+1, Inf(G(j+1)));
       }
       //the first constraint:
       //inf(df/dx_1) xl_1 + ... + inf(df/dx_n) xl_n <= loup -Inf(eval_inf)
       mysoplex.addRow(LPRow(-infinity, row1, 0));
    }else{
       int op=-1;
       if(i!=0) op=(dynamic_cast<const Inequality*> (&sys.ctr(i)))->op;
   
       //The contraints i is generated:
      // c_i:  inf([g_i]([x]) + inf(dg_i/dx_1) * xl_1 + ... + inf(dg_i/dx_n) + xl_n  <= 0
       bool add_constraint=true;
       for (int j=0; j<n-1; j++){
	 if(Diam(G(j+1))>1e5){
	   add_constraint=false; //to avoid problems with SoPleX
	   break;
	 }
	 
         if(op == LEQ || op== LT){
	    row1.add(j+1, Inf(G(j+1)));
	 }else{
            row1.add(j+1, Sup(G(j+1)));
	 }
       }
       
       if(add_constraint)
         if(op == LEQ || op== LT){
           mysoplex.addRow(LPRow(-infinity, row1, Sup(-eval_inf(i+1))));
         }else{
           mysoplex.addRow(LPRow(Inf(-eval_inf(i+1)), row1, infinity));
         }
      
    }
  }


  SPxSolver::Status stat;
//    mysoplex.writeFile("dump.lp", NULL, NULL, NULL);
//   cout << "in" << endl;
  mysoplex.setTerminationIter(30);
  mysoplex.setDelta(1e-10);
  try{
    stat = mysoplex.solve();
  }catch(SPxException){
    return true;
  }
//   cout << stat << endl;
  
  if( stat == SPxSolver::OPTIMAL ){
      //The optimum solution found in the linear system is mapped to the nonlinear system
      //y_opt <-- inf([f]([x])) + yl_opt + eps_error
      //for the moment, I added an epsilon due to the nonfiability of the simplex algorithm.
      INTERVAL f= inf_f+ mysoplex.objValue() + INTERVAL(-1e-8,1e-8);
//       sys.space.box=savebox;
      if(Inf(f) > Inf(sys.space.box(n)) ){
            if(Inf(f) > Sup(sys.space.box(n)) )
                 return false;
            //if y_opt in [y], then we update the lower bound:
            //[y] <-- [y_opt, sup([y])]
//            cout << "in:" << Inf(sys.space.box(n)) << endl;
//            cout << "LB:" << Inf(f) << endl;
           sys.space.box(n)=INTERVAL( Inf(f),Sup(sys.space.box(n) ));
      }
      return true;
  }else if(stat == SPxSolver::INFEASIBLE)
      return false;
  if(stat == SPxSolver::ABORT_TIME) cout << "Simplex spent too much time" << endl;
  return true;

 }
 
//The system is relaxed by using the Taylor extension.
//Then the simplex algorithm is applied to obtain a new uploup loup
//If a new loup is found the method returns true
 bool simplex_update_loup(const System& sys, const Evaluator& goal, Contractor& is_inside, REAL& loup, VECTOR& loup_point) {

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
  for (int i=0; i<sys.nb_ctr();i++){
    if(i>0 && isInner(sys, i)) continue; //the constraint is satified :)

// =============== Gradient calculation ======================
      for (int j=0; j<n; j++) {
        G(j+1) = 0.0;
        sys.space.ent(IBEX_VAR,j).deriv = &G(j+1);     
      }
    if(i==0)
      goal.gradient(sys.space);
    else
      sys.ctr(i).gradient(sys.space);
// ============================================================
//     if(G.max_diameter()>1e8) return false; //to avoid problems with SoPleX
    DSVector row1(n);

    if(i==0){
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
       if(i!=0) op=(dynamic_cast<const Inequality*> (&sys.ctr(i)))->op;
   
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
        
        bool ret= check_candidate(sys, sys.space, goal, is_inside, loup, loup_point, Mid(sys.space.box), false);
	sys.space.box=savebox;
        if (ret)
	  {int prec=cout.precision();
	    cout.precision(12);
// 	    cout << "[simplex1]"  << " loup update " << loup  << " loup point  " << loup_point << endl;
	    cout.precision(prec);
	  }
	return ret;
  }

  if(stat == SPxSolver::ABORT_TIME) cout << "Simplex spent too much time" << endl;
  sys.space.box=savebox;
  return false;

}
}
