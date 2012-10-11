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


#include "IbexAffineSimplex.h"
//#include "optim/IbexOptimizer.h"
//#include "optim/IbexUpdateLoup.h"

namespace ibex {



  int AA_Simplex::AA_Linearization(SoPlex& mysoplex, int ctr, AF2_expr::linear_mode lmode){

    if(ops[ctr]!=EQU && goal_ctr!=ctr && isInner(sys, ctr)) return 0; //the constraint is satisfied

    int n=space.nb_var();


    evls[ctr]->eval_AF(space, lmode);
    
    int cont=0;

    if(ops[ctr]==EQU){
       cont+=AA_Linearization(mysoplex, ctr, LEQ);
       cont+=AA_Linearization(mysoplex, ctr, GEQ);
    }else
       cont+=AA_Linearization(mysoplex, ctr, ops[ctr]);
    
    return cont;
  }


int AA_Simplex::AA_Linearization(SoPlex& mysoplex, int ctr, int op){
    int n=space.nb_var();
    DSVector row1(n);
    
    AF2_expr x= evls[ctr]->AF[0];
    //cout << x << "=" << x.toI() << endl;
    //exit(0);

    if(x.type==AF2_expr::UNBOUNDED) return 0;

    if(ctr==goal_ctr){
       row1.add(n-1, -1.0);
    }


    for (int j=0; j<n; j++){
          if(j==n-1 && goal_ctr!=-1) continue; //the variable y!    
          if(!isvar[ctr][j]) continue;

	  //if(Abs(x[j+1])> max_diam_deriv )   return 0; //to avoid problems with SoPleX
          
         INTERVAL a = (op == LEQ || op == LT)? Inf(x[j+1]) : Sup(x[j+1]);
         row1.add(j, Mid(a)); //Inf(a)=Mid(a)=Sup(a)
    }
      
  bool added=false;

  INTERVAL ev(x.ev_linear() - ((goal_ctr==ctr)? space.box(n):0)); //??

  //INTERVAL bounds= 0;

    if(op == LEQ || op == LT){
      INTERVAL sup=x[n+1];// + Sup(bounds);
      if(Inf(ev) > Sup(sup))   throw EmptyBoxException(); //the constraint is not satisfied
      if(Sup(ev) > Sup(sup)){ //otherwise the constraint is satisfied for any point in the box
           LPRow lp=LPRow(-infinity, row1, Sup(-x[0]+ sup ));
           mysoplex.addRow(lp);	   
           added=true;
      }
    }else{
      INTERVAL inf=-x[n+1];// Inf(bounds);
       if(Sup(ev)< Inf(inf))  throw EmptyBoxException();
       if(Inf(ev)< Inf(inf)){
	  LPRow lp=LPRow(Inf(-x[0] + inf  ) , row1, infinity);
          mysoplex.addRow(lp);
          added=true;
       }
    }

    
    return (added)? 1:0;
    
}



AF2_expr::linear_mode AA_Simplex::lmode=AF2_expr::MINR;

int AA_Simplex::Linearization(SoPlex& mysoplex){
//  cout << space.box << endl;
  int n=space.nb_var();
  
  DSVector dummycol(0);

  /*********generation of the linearized system*********/

  for (int j=0; j<n; j++){
  	mysoplex.addCol(LPCol(0.0, dummycol, infinity, - infinity ));
  }

  int nb_ctrs=0;    
  for(int ctr=0; ctr<sys.nb_ctr();ctr++){
    nb_ctrs+=AA_Linearization(mysoplex, ctr, lmode);
  }

  if(nb_ctrs<1)  return 0;

    
  // cout << sys.space.box  << endl;
  INTERVAL_VECTOR tbox=space.box;
//   cout << space.box << endl;
  for (int j=0; j<n; j++){
        DSVector row1(n);
        row1.add (j,1.0);

        var[j]=AF2_expr(space.box(j+1),j,n);

// 	if(j!=n-1 || goal_ctr==-1)  cout << var[j] << endl;
	if(j!=n-1 || goal_ctr==-1) space.box(j+1)=INTERVAL(-1,1);
        mysoplex.addRow(LPRow(Inf(space.box(j+1)), row1, Sup(space.box(j+1))));

  }

  return nb_ctrs;
}

void AA_Simplex::Map(INTERVAL_VECTOR& box, INTERVAL_VECTOR& tbox){
 int n=Dimension(box);
 for (int j=0; j<n; j++){
    if(j!=n-1 || goal_ctr==-1){
      tbox(j+1)&=var[j][0]+var[j][j+1]*box(j+1);
    }else
      tbox(j+1)&=box(j+1);
 }
 box=tbox;
}

}
