/*---------------------------------------------------------------------------------
 * 
 * Expression evaluator (evaluation, projection, differentiation)
 * --------------------------------------------------------------
 *
 * Copyright (C) 2007-2009 Gilles Chabert
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

#include <iostream>
#include <stdlib.h>
#include <iomanip>
#include "Functions.h"
#include "Utilities.h"
#include "mohc/IbexEvaluatorOG.h"
#include "IbexToken.h"
#include "IbexProjections.h"
#include "IbexLinear.h"
#include "IbexFunction.h"

namespace ibex {

  //static const char* tokentostring(int tok);

//Create the arrays ic2var and ic2occ to obtain the variable and total occurrence number
//from a leave node. It returns the total number of occurrences.
  int EvaluatorOG::initForwardOG(Space &space, int nmax, int* entity2var) const {

//   if(ic2var!=NULL) return -1; //already done
  int i   = infol-1;
  int ic  = itvl-1;
  int icc = icstl-1;
  //int mc  = matl-1;
  //int mcc = mcstl-1;
  //int f   = funcl-1;

  int var=0; //variable id.
  int occ=0;

  ic2var=new int[itvl];
  ic2occ=new int*[itvl];
  int*  info2var= new int[space.nb_var()];
  for (int k=0;k<space.nb_var();k++)
    info2var[k]=-1;
  Ip = new INTERVAL[itvl]; 
//   cout<<"itvl:"<<itvl<<endl;
  
  for (int k=0;k<itvl;k++)
     ic2var[k]=-1;

  n=nmax;
  var_occ=new int[nmax];

  //bool sat;
  //int v;

//   //  cout << "matl=" << matl << endl;
  for (int c=codel-1; c>=0; c--) {


    switch (code[c]) {
    case CST      : icc--; ic--; break;
    case SUB      : ic2occ[ic]=new int[2];
                    if(ic2var[info[i-1]]!=-1) ic2occ[ic][0]=occ++;
                    if(ic2var[info[i]]!=-1) ic2occ[ic][1]=occ++;
                    ic--; i-=2; break;
    case ADD      : ic2occ[ic]=new int[2];
                    if(ic2var[info[i-1]]!=-1) ic2occ[ic][0]=occ++;
                    if(ic2var[info[i]]!=-1) ic2occ[ic][1]=occ++;
                    ic--; i-=2; break;
    case DIV      : ic2occ[ic]=new int[2];
                    if(ic2var[info[i-1]]!=-1) ic2occ[ic][0]=occ++;
                    if(ic2var[info[i]]!=-1) ic2occ[ic][1]=occ++;
                    ic--; i-=2; break;
    case MUL      : ic2occ[ic]=new int[2];
                    if(ic2var[info[i-1]]!=-1) ic2occ[ic][0]=occ++;
                    if(ic2var[info[i]]!=-1) ic2occ[ic][1]=occ++;
                    ic--; i-=2; break;
    case MIN      : ic2occ[ic]=new int[2];
                    if(ic2var[info[i-1]]!=-1) ic2occ[ic][0]=occ++;
                    if(ic2var[info[i]]!=-1) ic2occ[ic][1]=occ++;
                    ic--; i-=2; break;
    case MAX      : ic2occ[ic]=new int[2];
                    if(ic2var[info[i-1]]!=-1) ic2occ[ic][0]=occ++;
                    if(ic2var[info[i]]!=-1) ic2occ[ic][1]=occ++;
                    ic--; i-=2; break;
    case MINUS    :  if(ic2var[info[i]]!=-1) ic2occ[ic]=new int(occ++);
                     ic--; i--; break;
    case SIGN     : if(ic2var[info[i]]!=-1) ic2occ[ic]=new int(occ++);
                    ic--; i--; break;
    case POW      :if(ic2var[info[i]]!=-1) ic2occ[ic]=new int(occ++);
                    ic--; i-=2; break;
    case SQR      : if(ic2var[info[i]]!=-1) ic2occ[ic]=new int(occ++);
                    ic--; i--; break;
    case SQRT     : if(ic2var[info[i]]!=-1) ic2occ[ic]=new int(occ++);
                     ic--; i--; break;
    case LOG      : if(ic2var[info[i]]!=-1) ic2occ[ic]=new int(occ++);
                     ic--; i--; break;
    case EXP      : if(ic2var[info[i]]!=-1) ic2occ[ic]=new int(occ++);
                     ic--; i--; break;
    case COS      : if(ic2var[info[i]]!=-1) ic2occ[ic]=new int(occ++);
                     ic--; i--; break;
    case SIN      : if(ic2var[info[i]]!=-1) ic2occ[ic]=new int(occ++);
                     ic--; i--; break;
    case TAN      : if(ic2var[info[i]]!=-1) ic2occ[ic]=new int(occ++);
                     ic--; i--; break;
    case ARCCOS   : if(ic2var[info[i]]!=-1) ic2occ[ic]=new int(occ++);
                     ic--; i--; break;
    case ARCSIN   : if(ic2var[info[i]]!=-1) ic2occ[ic]=new int(occ++);
                     ic--; i--; break;
    case ARCTAN   : if(ic2var[info[i]]!=-1) ic2occ[ic]=new int(occ++);
                     ic--; i--; break;
    case COSH     : if(ic2var[info[i]]!=-1) ic2occ[ic]=new int(occ++);
                     ic--; i--; break;
    case SINH     : if(ic2var[info[i]]!=-1) ic2occ[ic]=new int(occ++);
                     ic--; i--; break;
    case TANH     : if(ic2var[info[i]]!=-1) ic2occ[ic]=new int(occ++);
                     ic--; i--; break;
    case ARCCOSH  : if(ic2var[info[i]]!=-1) ic2occ[ic]=new int(occ++);
                     ic--; i--; break;
    case ARCSINH  : if(ic2var[info[i]]!=-1) ic2occ[ic]=new int(occ++);
                     ic--; i--; break;
    case ARCTANH  : if(ic2var[info[i]]!=-1) ic2occ[ic]=new int(occ++);
                     ic--; i--; break;
    case SYMBOL   : if (space.entity(info[i]).type==IBEX_VAR){
		       ic2var[ic]=entity2var[info[i]];
		       var++;
                     }
    i--; ic--; break;

    default       : throw NonRecoverableException("Internal bug: unknown evaluator code (please report this bug).");
    }
  }

  inf=new REAL[var];
  sup=new REAL[var];
  ev_occ=new INTERVAL[occ];
  return occ;
}


void EvaluatorOG::evaluateOccurrence(INTERVAL& t, int i, int occ){
   if(minRevise)
       t &=A[occ]*inf[i] + B[occ]*sup[i] + C[occ]*t ;
   else
       t &=A[occ]*sup[i] + B[occ]*inf[i] + C[occ]*t ; 

   ev_occ[occ]=t;
}

INTERVAL EvaluatorOG::projectOccurrence(INTERVAL& t, int x, int occ){
   if(minRevise)
       return (t - (A[occ]*inf[x] + B[occ]*sup[x])) / C[occ] ;
   else
       return (t - (A[occ]*sup[x] + B[occ]*inf[x])) / C[occ] ;
}


void EvaluatorOG::forwardOG(Space& space, INTERVAL *r_a, INTERVAL *r_b, INTERVAL *r_c, bool minRevise1) {

   int i   = infol-1;
   int ic  = itvl-1;
   int icc = icstl-1;
   //int mc  = matl-1;
   //int mcc = mcstl-1;
   //int f   = funcl-1;
   //int v;
   INTERVAL t1,t2;
   bool sat;
   int x;
   A=r_a; B=r_b; C=r_c; minRevise=minRevise1;

   for (int c=codel-1; c>=0; c--) {
      switch (code[c]) {
                         sat = !I[ic--].empty(); break;
         case CST      : I[ic] = Ic[icc--];                         sat = !I[ic--].empty(); break;
         case SUB      : t1=I[info[i-1]]; t2=I[info[i]];
                         if((x=ic2var[info[i-1]])!=-1) evaluateOccurrence(t1, x, ic2occ[ic][0]);
                         if((x=ic2var[info[i]])!=-1) evaluateOccurrence(t2, x, ic2occ[ic][1]);
                         I[ic] = t1-t2;
//                          cout << t1 << "-" << t2 << "=" << I[ic] << endl; 

                         i-=2;   sat = !I[ic--].empty(); break;
         case ADD      : t1=I[info[i-1]]; t2=I[info[i]];
                         if((x=ic2var[info[i-1]])!=-1) evaluateOccurrence(t1, x, ic2occ[ic][0]);
                         if((x=ic2var[info[i]])!=-1) evaluateOccurrence(t2, x, ic2occ[ic][1]);
                         I[ic] = t1+t2;
//                          cout << t1 << "+" << t2 << "=" << I[ic] << endl; 
                         i-=2;   sat = !I[ic--].empty(); break;
         case DIV      : t1=I[info[i-1]]; t2=I[info[i]];
                         if((x=ic2var[info[i-1]])!=-1)  evaluateOccurrence(t1, x, ic2occ[ic][0]);
                         if((x=ic2var[info[i]])!=-1) evaluateOccurrence(t2, x, ic2occ[ic][1]);
                         I[ic] = t1/t2;
//                          cout << t1 << "/" << t2 << "=" << I[ic] << endl; 
                         i-=2;   sat = !I[ic--].empty(); break;
         case MUL      : t1=I[info[i-1]]; t2=I[info[i]];
                         if((x=ic2var[info[i-1]])!=-1)  evaluateOccurrence(t1, x, ic2occ[ic][0]);
                         if((x=ic2var[info[i]])!=-1) evaluateOccurrence(t2, x, ic2occ[ic][1]);
                         I[ic] = t1*t2;
//                          cout << t1 << "*" << t2 << "=" << I[ic] << endl; 
                         i-=2;   sat = !I[ic--].empty(); break;
                         
         case MIN      : t1=I[info[i-1]]; t2=I[info[i]];
                         if((x=ic2var[info[i-1]])!=-1)  evaluateOccurrence(t1, x, ic2occ[ic][0]);
                         if((x=ic2var[info[i]])!=-1) evaluateOccurrence(t2, x, ic2occ[ic][1]);
                         I[ic] = Min(t1, t2);
                         i-=2;   sat = !I[ic--].empty(); break;
         case MAX      : t1=I[info[i-1]]; t2=I[info[i]];
                         if((x=ic2var[info[i-1]])!=-1)  evaluateOccurrence(t1, x, ic2occ[ic][0]);
                         if((x=ic2var[info[i]])!=-1) evaluateOccurrence(t2, x, ic2occ[ic][1]);
                         I[ic] = Max(t1, t2);
                         i-=2;   sat = !I[ic--].empty(); break;
         case MINUS    : t1=I[info[i]];
                         if((x=ic2var[info[i]])!=-1) evaluateOccurrence(t1, x, *ic2occ[ic]);
                         I[ic]= -t1; i--;  sat = !I[ic--].empty(); break;
         case SIGN     : t1=I[info[i]];
                         if((x=ic2var[info[i]])!=-1) evaluateOccurrence(t1, x, *ic2occ[ic]);
                         I[ic] = Sign(t1);   i--; sat = !I[ic--].empty(); break;
         case POW      : t1=I[info[i]];
                         if((x=ic2var[info[i]])!=-1) evaluateOccurrence(t1, x, *ic2occ[ic]);
                         I[ic] = Power(t1,info[i-1]); 
//                          cout << t1 << "^" << info[i-1] << "=" << I[ic] << endl; 
                         i-=2; sat = !I[ic--].empty(); break;
         case SQR      : t1=I[info[i]];
                         if((x=ic2var[info[i]])!=-1) evaluateOccurrence(t1, x, *ic2occ[ic]);
                         I[ic] = Sqr(t1); 
//                          cout << t1 << "^" << 2 << "=" << I[ic] << endl; 
                         i--; sat = !I[ic--].empty(); break;
         case SQRT     : t1=I[info[i]];
                         if((x=ic2var[info[i]])!=-1) evaluateOccurrence(t1, x, *ic2occ[ic]);
                         I[ic] = Sqrt(t1); i--; sat = !I[ic--].empty(); break;
         case LOG      : t1=I[info[i]];
                         if((x=ic2var[info[i]])!=-1) evaluateOccurrence(t1, x, *ic2occ[ic]);
                         I[ic] = Log(t1);  i--; sat = !I[ic--].empty(); break;
         case EXP      : t1=I[info[i]];
                         if((x=ic2var[info[i]])!=-1) evaluateOccurrence(t1, x, *ic2occ[ic]);
                         I[ic] = Exp(t1); i--;  sat = !I[ic--].empty(); break;
         case COS      : t1=I[info[i]];
                         if((x=ic2var[info[i]])!=-1) evaluateOccurrence(t1, x, *ic2occ[ic]);
                         I[ic] = Cos(t1); i--;  sat = !I[ic--].empty(); break;
         case SIN      : t1=I[info[i]];
                         if((x=ic2var[info[i]])!=-1) evaluateOccurrence(t1, x, *ic2occ[ic]);
                         I[ic] = Sin(t1); i--;  sat = !I[ic--].empty(); break;
         case TAN      : t1=I[info[i]];
                         if((x=ic2var[info[i]])!=-1) evaluateOccurrence(t1, x, *ic2occ[ic]);
                         I[ic] = Tan(t1); i--;  sat = !I[ic--].empty(); break;
         case ARCCOS   : t1=I[info[i]];
                         if((x=ic2var[info[i]])!=-1) evaluateOccurrence(t1, x, *ic2occ[ic]);
                         I[ic] = ArcCos(t1); i--; sat = !I[ic--].empty(); break;
         case ARCSIN   : t1=I[info[i]];
                         if((x=ic2var[info[i]])!=-1) evaluateOccurrence(t1, x, *ic2occ[ic]);
                         I[ic] = ArcSin(t1); i--; sat = !I[ic--].empty(); break;
         case ARCTAN   : t1=I[info[i]];
                         if((x=ic2var[info[i]])!=-1) evaluateOccurrence(t1, x, *ic2occ[ic]);
                         I[ic] = ArcTan(t1); i--; sat = !I[ic--].empty(); break;
         case COSH     : t1=I[info[i]];
                         if((x=ic2var[info[i]])!=-1) evaluateOccurrence(t1, x, *ic2occ[ic]);
                         I[ic] = Cosh(t1); i--; sat = !I[ic--].empty(); break;
         case SINH     : t1=I[info[i]];
                         if((x=ic2var[info[i]])!=-1) evaluateOccurrence(t1, x, *ic2occ[ic]);
                         I[ic] = Sinh(t1); i--; sat = !I[ic--].empty(); break;
         case TANH     : t1=I[info[i]];
                         if((x=ic2var[info[i]])!=-1) evaluateOccurrence(t1, x, *ic2occ[ic]);
                         I[ic] = Tanh(t1); i--; sat = !I[ic--].empty(); break;
         case ARCCOSH  : t1=I[info[i]];
                         if((x=ic2var[info[i]])!=-1) evaluateOccurrence(t1, x, *ic2occ[ic]);
                         I[ic] = ArCosh(t1); i--; sat = !I[ic--].empty(); break; 
         case ARCSINH  : t1=I[info[i]];
                         if((x=ic2var[info[i]])!=-1) evaluateOccurrence(t1, x, *ic2occ[ic]);
                         I[ic] = ArSinh(t1); i--; sat = !I[ic--].empty(); break;
         case ARCTANH  : t1=I[info[i]];
                         if((x=ic2var[info[i]])!=-1) evaluateOccurrence(t1, x, *ic2occ[ic]);
                         I[ic] = ArTanh(t1); i--; sat = !I[ic--].empty(); break;
         case SYMBOL   : I[ic] = space.domain(info[i--]);
                         if (ic2var[ic]!=-1) /*space.entity(info[i+1]).type==IBEX_VAR*/
			   {
			     inf[ic2var[ic]]=Inf(I[ic]);
			     sup[ic2var[ic]]=Sup(I[ic]);
			   }
                         sat = !I[ic--].empty(); break;

         default       : throw NonRecoverableException("Internal bug: unknown evaluator code (please report this bug).");
      }
      if (!sat) throw EmptyBoxException(); 
    /* warning: empty results not yet implemented for matrix
      * addition, etc. An empty matrix can only be raised from 
    * operator&=*/
   }
}

void EvaluatorOG::backwardOG(Space& space, INTERVAL *r_a, INTERVAL *r_b, INTERVAL *r_c, bool minRevise1) { 
   int i   = 0;
   int ic  = 0;
   //int mc  = 0;
   //int f   = 0;
   //int v;
   bool sat=true;
   int x1, x2, occ1, occ2;
   INTERVAL t1,t2;
   A=r_a; B=r_b; C=r_c; minRevise=minRevise1;

   for (int c=0; c<codel; c++) {

      switch (code[c]) {
      
         case CST      : ic++; break;
         case ADD      : t1=I[info[i]]; t2=I[info[i+1]];

                         if((x1=ic2var[info[i]])!=-1){occ1=ic2occ[ic][0]; t1=ev_occ[occ1];}
                         if((x2=ic2var[info[i+1]])!=-1){occ2=ic2occ[ic][1]; t2=ev_occ[occ2];}
                         sat  = (t1 &= (I[ic] - t2));
                         sat &= (t2 &= (I[ic] - t1));

                         if(x1!=-1 && Inf(C[occ1])>1e-5) sat&=(I[info[i]]&=projectOccurrence(t1, x1, occ1));
                         else if(x1==-1) I[info[i]]=t1;
                         if(x2!=-1 && Inf(C[occ2])>1e-5) sat&=(I[info[i+1]]&=projectOccurrence(t2, x2, occ2));
                         else if(x2==-1) I[info[i+1]]=t2;
                         ic++; i+=2; break;

         case SUB      : t1=I[info[i]]; t2=I[info[i+1]];

                         if((x1=ic2var[info[i]])!=-1){occ1=ic2occ[ic][0]; t1=ev_occ[occ1];}
                         if((x2=ic2var[info[i+1]])!=-1){occ2=ic2occ[ic][1]; t2=ev_occ[occ2];}

                         sat  = (t1 &= (I[ic] + t2));
                         sat &= (t2 &= (t1 - I[ic])); 

                         if(x1!=-1 && Inf(C[occ1])>1e-5)sat&=(I[info[i]]&=projectOccurrence(t1, x1, occ1));
                         else if(x1==-1) I[info[i]]=t1;

                         if(x2!=-1 && Inf(C[occ2])>1e-5) sat&=(I[info[i+1]]&=projectOccurrence(t2, x2, occ2));
                         else if(x2==-1) I[info[i+1]]=t2;

                         ic++; i+=2; break;

         case MUL      : t1=I[info[i]]; t2=I[info[i+1]];
                         if((x1=ic2var[info[i]])!=-1){occ1=ic2occ[ic][0]; t1=ev_occ[occ1];}
                         if((x2=ic2var[info[i+1]])!=-1){occ2=ic2occ[ic][1]; t2=ev_occ[occ2];}
                         sat = proj_mult(I[ic],t1,t2);

                         if(x1!=-1 && Inf(C[occ1])>1e-5)sat&=(I[info[i]]&=projectOccurrence(t1, x1, occ1));
                         else if(x1==-1) I[info[i]]=t1;
                         if(x2!=-1 && Inf(C[occ2])>1e-5) sat&=(I[info[i+1]]&=projectOccurrence(t2, x2, occ2));
                         else if(x2==-1) I[info[i+1]]=t2;

                         ic++; i+=2; break;

         case DIV      : t1=I[info[i]]; t2=I[info[i+1]];
                         if((x1=ic2var[info[i]])!=-1){occ1=ic2occ[ic][0]; t1=ev_occ[occ1];}
                         if((x2=ic2var[info[i+1]])!=-1){occ2=ic2occ[ic][1]; t2=ev_occ[occ2];}
                         sat  = (t1 &= (I[ic] * t2)); 
                         sat &= (t2.div_intersect(t1, I[ic])); 

                         if(x1!=-1 && Inf(C[occ1])>1e-5) sat&= (I[info[i]]&=projectOccurrence(t1, x1, occ1));
                         else if(x1==-1) I[info[i]]=t1;
                         if(x2!=-1 && Inf(C[occ2])>1e-5) sat&=(I[info[i+1]]&=projectOccurrence(t2, x2, occ2));
                         else if(x2==-1) I[info[i+1]]=t2;
                         ic++; i+=2; break;

         case MIN      : t1=I[info[i]]; t2=I[info[i+1]];
                         if((x1=ic2var[info[i]])!=-1){occ1=ic2occ[ic][0]; t1=ev_occ[occ1];}
                         if((x2=ic2var[info[i+1]])!=-1){occ2=ic2occ[ic][1]; t2=ev_occ[occ2];}
                         sat = proj_min(I[ic++], t1, t2);

                         if(x1!=-1 && Inf(C[occ1])>1e-5)sat&=(I[info[i]]&=projectOccurrence(t1, x1, occ1));
                         else if(x1==-1) I[info[i]]=t1;
                         if(x2!=-1 && Inf(C[occ2])>1e-5) sat&=(I[info[i+1]]&=projectOccurrence(t2, x2, occ2));
                         else if(x2==-1) I[info[i+1]]=t2;
                         i+=2; break;

         case MAX      : t1=I[info[i]]; t2=I[info[i+1]];
                         if((x1=ic2var[info[i]])!=-1){occ1=ic2occ[ic][0]; t1=ev_occ[occ1];}
                         if((x2=ic2var[info[i+1]])!=-1){occ2=ic2occ[ic][1]; t2=ev_occ[occ2];}       
                         sat = proj_max(I[ic++], t1, t2);
                         if(x1!=-1 && Inf(C[occ1])>1e-5)sat&=(I[info[i]]&=projectOccurrence(t1, x1, occ1));
                         else if(x1==-1) I[info[i]]=t1;
                         if(x2!=-1 && Inf(C[occ2])>1e-5) sat&=(I[info[i+1]]&=projectOccurrence(t2, x2, occ2));
                         else if(x2==-1) I[info[i+1]]=t2;
                         i+=2; break;

         case SIGN     : t1=I[info[i]];
                         if((x1=ic2var[info[i]])!=-1){occ1=*ic2occ[ic]; t1=ev_occ[occ1];}
                         sat = proj_sign(I[ic++], t1);
                         if(x1!=-1 && Inf(C[occ1])>1e-5)sat&=(I[info[i]]&=projectOccurrence(t1, x1, occ1));
                         else if(x1==-1) I[info[i]]=t1;

                         i++;  break;
         case MINUS    : t1=I[info[i]];
                         if((x1=ic2var[info[i]])!=-1){occ1=*ic2occ[ic]; t1=ev_occ[occ1];}
                         sat = t1 &= -1.0*I[ic++];
                         if(x1!=-1 && Inf(C[occ1])>1e-5)sat&=(I[info[i]]&=projectOccurrence(t1, x1, occ1));
                         else if(x1==-1) I[info[i]]=t1;
                         i++;  break;
         case SQRT     : t1=I[info[i]];
                         if((x1=ic2var[info[i]])!=-1){occ1=*ic2occ[ic]; t1=ev_occ[occ1];}
                         sat = t1 &= Sqr(I[ic++]);
                         if(x1!=-1 && Inf(C[occ1])>1e-5)sat&=(I[info[i]]&=projectOccurrence(t1, x1, occ1));
                         else if(x1==-1) I[info[i]]=t1;
                         i++;  break;
         case LOG      : t1=I[info[i]];
                         if((x1=ic2var[info[i]])!=-1){occ1=*ic2occ[ic]; t1=ev_occ[occ1];}
                         sat = t1 &= Exp(I[ic++]);
                         if(x1!=-1 && Inf(C[occ1])>1e-5)sat&=(I[info[i]]&=projectOccurrence(t1, x1, occ1));
                         else if(x1==-1) I[info[i]]=t1;
                         i++;  break;
         case EXP      : t1=I[info[i]];
                         if((x1=ic2var[info[i]])!=-1){occ1=*ic2occ[ic]; t1=ev_occ[occ1];}
                         sat = proj_exp(I[ic++], t1);
                         if(x1!=-1 && Inf(C[occ1])>1e-5)sat&=(I[info[i]]&=projectOccurrence(t1, x1, occ1));
                         else if(x1==-1) I[info[i]]=t1;
                         i++;  break;
         case POW      : t1=I[info[i+1]];
                         if((x1=ic2var[info[i+1]])!=-1){occ1=*ic2occ[ic]; t1=ev_occ[occ1];}
                         sat = proj_power(I[ic++], t1, info[i]);
                         if(x1!=-1 && Inf(C[occ1])>1e-5) sat&=(I[info[i+1]]&=projectOccurrence(t1, x1, occ1));
                         else if(x1==-1) I[info[i+1]]=t1;
                         i+=2;      break;
         case SQR      : t1=I[info[i]];
                         if((x1=ic2var[info[i]])!=-1){occ1=*ic2occ[ic]; t1=ev_occ[occ1];}
                         sat = proj_sqr  (I[ic++], t1);
                         if(x1!=-1 && Inf(C[occ1])>1e-5)sat&=(I[info[i]]&=projectOccurrence(t1, x1, occ1));
                         else if(x1==-1) I[info[i]]=t1;
                         i++;  break;
         case COS      : t1=I[info[i]];
                         if((x1=ic2var[info[i]])!=-1){occ1=*ic2occ[ic]; t1=ev_occ[occ1];}
                         sat = proj_trigo(I[ic++], t1, COS);
                         if(x1!=-1 && Inf(C[occ1])>1e-5)sat&=(I[info[i]]&=projectOccurrence(t1, x1, occ1));
                         else if(x1==-1) I[info[i]]=t1;
                         i++;  break;
         case SIN      : t1=I[info[i]];
                         if((x1=ic2var[info[i]])!=-1){occ1=*ic2occ[ic]; t1=ev_occ[occ1];}
                         sat = proj_trigo(I[ic++], t1, SIN);
                         if(x1!=-1 && Inf(C[occ1])>1e-5)sat&=(I[info[i]]&=projectOccurrence(t1, x1, occ1));
                         else if(x1==-1) I[info[i]]=t1;
                         i++;  break;
         case TAN      : t1=I[info[i]];
                         if((x1=ic2var[info[i]])!=-1){occ1=*ic2occ[ic]; t1=ev_occ[occ1];}
                         sat = proj_trigo(I[ic++], t1, TAN);
                         if(x1!=-1 && Inf(C[occ1])>1e-5)sat&=(I[info[i]]&=projectOccurrence(t1, x1, occ1));
                         else if(x1==-1) I[info[i]]=t1;
                         i++;  break;
         case ARCCOS   : t1=I[info[i]];
                         if((x1=ic2var[info[i]])!=-1){occ1=*ic2occ[ic]; t1=ev_occ[occ1];}
                         sat = t1 &= Cos(I[ic++]);
                         if(x1!=-1 && Inf(C[occ1])>1e-5)sat&=(I[info[i]]&=projectOccurrence(t1, x1, occ1));
                         else if(x1==-1) I[info[i]]=t1;
                         i++;  break;
         case ARCSIN   : t1=I[info[i]];
                         if((x1=ic2var[info[i]])!=-1){occ1=*ic2occ[ic]; t1=ev_occ[occ1];}
                         sat = t1 &= Sin(I[ic++]);
                         if(x1!=-1 && Inf(C[occ1])>1e-5)sat&=(I[info[i]]&=projectOccurrence(t1, x1, occ1));
                         else if(x1==-1) I[info[i]]=t1;
                         i++;  break;
         case ARCTAN   : t1=I[info[i]];
                         if((x1=ic2var[info[i]])!=-1){occ1=*ic2occ[ic]; t1=ev_occ[occ1];}
                         sat = t1 &= Tan(I[ic++]);
                         if(x1!=-1 && Inf(C[occ1])>1e-5)sat&=(I[info[i]]&=projectOccurrence(t1, x1, occ1));
                         else if(x1==-1) I[info[i]]=t1;
                         i++;  break;
         case COSH     : t1=I[info[i]];
                         if((x1=ic2var[info[i]])!=-1){occ1=*ic2occ[ic]; t1=ev_occ[occ1];}
                         sat = proj_cosh (I[ic++], t1);
                         if(x1!=-1 && Inf(C[occ1])>1e-5)sat&=(I[info[i]]&=projectOccurrence(t1, x1, occ1));
                         else if(x1==-1) I[info[i]]=t1;
                         i++;  break;
         case SINH     : t1=I[info[i]];
                         if((x1=ic2var[info[i]])!=-1){occ1=*ic2occ[ic]; t1=ev_occ[occ1];}
                         sat = t1 &= ArSinh(I[ic++]);
                         if(x1!=-1 && Inf(C[occ1])>1e-5)sat&=(I[info[i]]&=projectOccurrence(t1, x1, occ1));
                         else if(x1==-1) I[info[i]]=t1;
                         i++;  break;
         case TANH     : t1=I[info[i]];
                         if((x1=ic2var[info[i]])!=-1){occ1=*ic2occ[ic]; t1=ev_occ[occ1];}
                         sat = proj_tanh(I[ic++], t1);
                         if(x1!=-1 && Inf(C[occ1])>1e-5)sat&=(I[info[i]]&=projectOccurrence(t1, x1, occ1));
                         else if(x1==-1) I[info[i]]=t1;
                         i++;  break;
         case ARCCOSH  : t1=I[info[i]];
                         if((x1=ic2var[info[i]])!=-1){occ1=*ic2occ[ic]; t1=ev_occ[occ1];}
                         sat = proj_arccosh(I[ic++], t1);
                         if(x1!=-1 && Inf(C[occ1])>1e-5)sat&=(I[info[i]]&=projectOccurrence(t1, x1, occ1));
                         else if(x1==-1) I[info[i]]=t1;
                         i++;  break;
         case ARCSINH  : t1=I[info[i]];
                         if((x1=ic2var[info[i]])!=-1){occ1=*ic2occ[ic]; t1=ev_occ[occ1];}
                         sat = t1 &= Sinh(I[ic++]);
                         if(x1!=-1 && Inf(C[occ1])>1e-5)sat&=(I[info[i]]&=projectOccurrence(t1, x1, occ1));
                         else if(x1==-1) I[info[i]]=t1;
                         i++;  break;
         case ARCTANH  : t1=I[info[i]];
                         if((x1=ic2var[info[i]])!=-1){occ1=*ic2occ[ic]; t1=ev_occ[occ1];}
                         sat = t1 &= Tanh(I[ic++]);
                         if(x1!=-1 && Inf(C[occ1])>1e-5)sat&=(I[info[i]]&=projectOccurrence(t1, x1, occ1));
                         else if(x1==-1) I[info[i]]=t1;
                         i++;  break;
         case SYMBOL   : //if (space.entity(info[i]).type==IBEX_VAR || space.entity(info[i]).type==IBEX_TMP) 
//      if (space.entity(info[i]).type==IBEX_EPR) 
//	cout << space.domain(info[i]) << " ^ " <<  I[ic] << endl;
            if (space.entity(info[i]).type!=IBEX_SYB && space.entity(info[i]).type!=IBEX_UPR) {
               sat = space.domain(info[i]) &= I[ic];
//                if(trace) cout <<"var:"<<space.domain(info[i])<<endl;
            }
            i++; ic++;     break;
            
            default       : throw NonRecoverableException("Internal bug: unknown evaluator code (please report this bug).");  
      }

      if (!sat) throw EmptyBoxException();    
   }
}


INTERVAL EvaluatorOG::projectOccurrence2(INTERVAL& t, int x, int occ, 
                                         INTERVAL& derivx, INTERVAL& derivocc){
   if(Sup(C[occ])>=1) return t;
   if(Inf(A[occ])==1 && Inf(derivx) > 0 && Inf(derivocc) > 0){//A[occ]=1
      INTERVAL alpha=INTERVAL(Inf(derivx))/INTERVAL(Inf(derivocc));
      if(Inf(alpha)>=1) {
//          alpha=(INTERVAL(Inf(derivx))-INTERVAL(Inf(derivocc)))/INTERVAL(Sup(derivocc))+INTERVAL(1);
         return t;
      }
      if(minRevise)
         return ( (t - INTERVAL(inf[x]))/alpha + INTERVAL(inf[x])) ;
      else
         return ( (t - INTERVAL(sup[x]))/alpha + INTERVAL(sup[x])) ;
   }else if(Inf(B[occ])==1 && Sup(derivx) < 0 && Sup(derivocc) < 0){//A[occ]=1
       INTERVAL alpha=INTERVAL(Sup(derivx))/INTERVAL(Sup(derivocc));
       if(Inf(alpha)>=1) {
             return t;
       }
       if(minRevise)
          return ( (t - INTERVAL(sup[x]))/alpha + INTERVAL(sup[x])) ;
       else
          return ( (t - INTERVAL(inf[x]))/alpha + INTERVAL(inf[x])) ;
  }else if(Inf(C[occ]) >= 1e-5) { 
      if(minRevise)
         return (t - (A[occ]*inf[x] + B[occ]*sup[x])) / C[occ] ;
      else
         return (t - (A[occ]*sup[x] + B[occ]*inf[x])) / C[occ] ;
   }
   
   return INTERVAL(BiasNegInf,BiasPosInf);
}

  /*
void EvaluatorOG::backward2OG(Space& space, INTERVAL *r_a, INTERVAL *r_b, INTERVAL *r_c, bool minRevise1, INTERVAL* deriv, INTERVAL** derivs) { 
   int i   = 0;
   int ic  = 0;
   int mc  = 0;
   int f   = 0;
   int v;
   bool sat=true;
   int x1, x2, occ1, occ2;
   INTERVAL t1,t2,p1,p2;
   INTERVAL infty(BiasNegInf,BiasPosInf);


   for(int k=1;k<itvl;k++)
      Ip[k]=infty;
   
   for (int k=0;k<n;k++){
      var_occ[k]=0;
   }
   
   A=r_a; B=r_b; C=r_c; minRevise=minRevise1;
   for (int c=0; c<codel; c++) {
      sat=true;
      switch (code[c]) {
      
         case CST      : ic++; break;
         case ADD      : t1=I[info[i]]; t2=I[info[i+1]];
                        if((x1=ic2var[info[i]])!=-1){occ1=ic2occ[ic][0]; t1=ev_occ[occ1];}
                        if((x2=ic2var[info[i+1]])!=-1){occ2=ic2occ[ic][1]; t2=ev_occ[occ2];}
                        p1 = (Ip[ic] - t2);
                        p2 = (Ip[ic] - t1);
                 if(x1!=-1){ 
                    sat=(Ip[info[i]]&=projectOccurrence2(p1, x1, occ1, deriv[x1],derivs[x1][var_occ[x1]++]));}
                 else Ip[info[i]]=p1;
                 if(x2!=-1) 
                    sat&=(Ip[info[i+1]]&=projectOccurrence2(p2, x2, occ2, deriv[x2],derivs[x2][var_occ[x2]++]));
                  else Ip[info[i+1]]=p2;
                  ic++; i+=2; break;

         case SUB      : t1=I[info[i]]; t2=I[info[i+1]];

                         if((x1=ic2var[info[i]])!=-1){occ1=ic2occ[ic][0]; t1=ev_occ[occ1];}
                         if((x2=ic2var[info[i+1]])!=-1){occ2=ic2occ[ic][1]; t2=ev_occ[occ2];}

                         p1 = (Ip[ic] + t2);
                         p2 = (t1 - Ip[ic]); 
                         
                   if(x1!=-1)sat=(Ip[info[i]]&=
                            projectOccurrence2(p1, x1, occ1, deriv[x1],derivs[x1][var_occ[x1]++]));
                         else Ip[info[i]]=p1;

                  if(x2!=-1) sat&=(Ip[info[i+1]]&=
                           projectOccurrence2(p2, x2, occ2, deriv[x2],derivs[x2][var_occ[x2]++]));
                         else Ip[info[i+1]]=p2;

                         ic++; i+=2; break;

         case MUL      : t1=I[info[i]]; t2=I[info[i+1]];
                         if((x1=ic2var[info[i]])!=-1){occ1=ic2occ[ic][0]; t1=ev_occ[occ1];}
                         if((x2=ic2var[info[i+1]])!=-1){occ2=ic2occ[ic][1]; t2=ev_occ[occ2];}
                         p1=infty;p2=infty;
			 // proj_mult(Ip[ic],t1,t2,p1,p2);

                         if(x1!=-1)sat=(Ip[info[i]]&=
                                  projectOccurrence2(p1, x1, occ1, deriv[x1],derivs[x1][var_occ[x1]++]));
                         else Ip[info[i]]=p1;
                         if(x2!=-1) sat&=(Ip[info[i+1]]&=
                                  projectOccurrence2(p2, x2, occ2, deriv[x2],derivs[x2][var_occ[x2]++]));
                         else Ip[info[i+1]]=p2;

                         ic++; i+=2; break;

         case DIV      : t1=I[info[i]]; t2=I[info[i+1]];
                         if((x1=ic2var[info[i]])!=-1){occ1=ic2occ[ic][0]; t1=ev_occ[occ1];}
                         if((x2=ic2var[info[i+1]])!=-1){occ2=ic2occ[ic][1]; t2=ev_occ[occ2];}
                         p2=infty;
                         p1 = (Ip[ic] * t2); 
                         sat = (p2.div_intersect(t1, Ip[ic])); 

                         if(x1!=-1) sat&= (Ip[info[i]]&=
                                  projectOccurrence2(p1, x1, occ1, deriv[x1],derivs[x1][var_occ[x1]++]));
                         else Ip[info[i]]=p1;
                         if(x2!=-1) sat&= (Ip[info[i+1]]&=
                                  projectOccurrence2(p2, x2, occ2, deriv[x2],derivs[x2][var_occ[x2]++]));
                         else Ip[info[i+1]]=p2;
                         ic++; i+=2; break;

         case MIN      : t1=I[info[i]]; t2=I[info[i+1]];
                         if((x1=ic2var[info[i]])!=-1){occ1=ic2occ[ic][0]; t1=ev_occ[occ1];}
                         if((x2=ic2var[info[i+1]])!=-1){occ2=ic2occ[ic][1]; t2=ev_occ[occ2];}
                         sat = proj_min(Ip[ic++], t1, t2);

                         if(x1!=-1) sat&=(Ip[info[i]]&=
                                  projectOccurrence2(t1, x1, occ1, deriv[x1],derivs[x1][var_occ[x1]++]));
                         else Ip[info[i]]=t1;
                         if(x2!=-1) sat&=(Ip[info[i+1]]&=
                                  projectOccurrence2(t2, x2, occ2, deriv[x2],derivs[x2][var_occ[x2]++]));
                         else Ip[info[i+1]]=t2;
                         i+=2; break;

         case MAX      : t1=I[info[i]]; t2=I[info[i+1]];
                         if((x1=ic2var[info[i]])!=-1){occ1=ic2occ[ic][0]; t1=ev_occ[occ1];}
                         if((x2=ic2var[info[i+1]])!=-1){occ2=ic2occ[ic][1]; t2=ev_occ[occ2];}       
                         sat = proj_max(Ip[ic++], t1, t2);
                         if(x1!=-1) sat&=(Ip[info[i]]&=
                                  projectOccurrence2(t1, x1, occ1, deriv[x1],derivs[x1][var_occ[x1]++]));
                         else Ip[info[i]]=t1;
                         if(x2!=-1) sat&=(Ip[info[i+1]]&=
                                  projectOccurrence2(t2, x2, occ2, deriv[x2],derivs[x2][var_occ[x2]++]));
                         else Ip[info[i+1]]=t2;
                         i+=2; break;

         case SIGN     : //t1=I[info[i]];
                         if((x1=ic2var[info[i]])!=-1){occ1=*ic2occ[ic];}
                         p1=infty;
                         proj_sign(Ip[ic++], p1);
                         if(x1!=-1) sat=(Ip[info[i]]&=
                                  projectOccurrence2(p1, x1, occ1, deriv[x1],derivs[x1][var_occ[x1]++]));
                         else Ip[info[i]]=p1;

                         i++;  break;
         case MINUS    : //t1=I[info[i]];
                         if((x1=ic2var[info[i]])!=-1){occ1=*ic2occ[ic];}
                         p1 = -1.0*Ip[ic++];
                         if(x1!=-1)sat&=(Ip[info[i]]&=
                                  projectOccurrence2(p1, x1, occ1, deriv[x1],derivs[x1][var_occ[x1]++]));
                         else Ip[info[i]]=p1;
                         i++;  break;
         case SQRT     : //t1=I[info[i]];
                         if((x1=ic2var[info[i]])!=-1){occ1=*ic2occ[ic];}
                         p1 = Sqr(Ip[ic++]);
                         if(x1!=-1)sat=(Ip[info[i]]&=
                                  projectOccurrence2(p1, x1, occ1, deriv[x1],derivs[x1][var_occ[x1]++]));
                         else Ip[info[i]]=p1;
                         i++;  break;
         case LOG      : //t1=I[info[i]];
                         if((x1=ic2var[info[i]])!=-1){occ1=*ic2occ[ic];}
                         p1 = Exp(Ip[ic++]);
                         if(x1!=-1)sat=(Ip[info[i]]&=
                                  projectOccurrence2(p1, x1, occ1, deriv[x1],derivs[x1][var_occ[x1]++]));
                         else Ip[info[i]]=p1;
                         i++;  break;
         case EXP      : //t1=I[info[i]];
                         if((x1=ic2var[info[i]])!=-1){occ1=*ic2occ[ic];}
                         p1=infty;
                         sat = proj_exp(Ip[ic++], p1);
                         if(x1!=-1)sat&=(Ip[info[i]]&=
                                  projectOccurrence2(p1, x1, occ1, deriv[x1],derivs[x1][var_occ[x1]++]));
                         else Ip[info[i]]=p1;
                         i++;  break;
         case POW      : //t1=I[info[i+1]];
                         if((x1=ic2var[info[i+1]])!=-1){occ1=*ic2occ[ic];}
                         p1=infty;
                         sat = proj_power(Ip[ic++], p1, info[i]);
                         
                        
                         if(x1!=-1) sat&=(Ip[info[i+1]]&=
                                  projectOccurrence2(p1, x1, occ1, deriv[x1],derivs[x1][var_occ[x1]++]));
                         else Ip[info[i+1]]=p1;
                         i+=2;      break;
         case SQR      : //t1=I[info[i]];
                         if((x1=ic2var[info[i]])!=-1){occ1=*ic2occ[ic];}
                         p1=infty;
                         sat = proj_sqr  (Ip[ic++], p1);
                         
                         
                         if(x1!=-1)sat&=(Ip[info[i]]&=
                                  projectOccurrence2(p1, x1, occ1, deriv[x1],derivs[x1][var_occ[x1]++]));
                         else Ip[info[i]]=p1;
                         i++;  break;
         case COS      : //t1=I[info[i]];
                         if((x1=ic2var[info[i]])!=-1){occ1=*ic2occ[ic];}
                         p1=infty;
                         sat = proj_trigo(Ip[ic++], p1, COS);
                         if(x1!=-1)sat&=(Ip[info[i]]&=
                                  projectOccurrence2(p1, x1, occ1, deriv[x1],derivs[x1][var_occ[x1]++]));
                         else Ip[info[i]]=p1;
                         i++;  break;
         case SIN      : //t1=I[info[i]];
                         if((x1=ic2var[info[i]])!=-1){occ1=*ic2occ[ic];}
                         p1=infty;
                         sat = proj_trigo(Ip[ic++], p1, SIN);

                         if(x1!=-1)sat&=(Ip[info[i]]&=
                                  projectOccurrence2(p1, x1, occ1, deriv[x1],derivs[x1][var_occ[x1]++]));
                         else Ip[info[i]]=p1;
                         i++;  break;
         case TAN      : //t1=I[info[i]];
                         if((x1=ic2var[info[i]])!=-1){occ1=*ic2occ[ic];}
                         p1=infty;
                         sat = proj_trigo(Ip[ic++], p1, TAN);
                         if(x1!=-1)sat&=(Ip[info[i]]&=
                                  projectOccurrence2(p1, x1, occ1, deriv[x1],derivs[x1][var_occ[x1]++]));
                         else Ip[info[i]]=p1;
                         i++;  break;
         case ARCCOS   : //t1=I[info[i]];
                         if((x1=ic2var[info[i]])!=-1){occ1=*ic2occ[ic];}
                         p1 = Cos(Ip[ic++]);
                         if(x1!=-1)sat=(Ip[info[i]]&=
                                  projectOccurrence2(p1, x1, occ1, deriv[x1],derivs[x1][var_occ[x1]++]));
                         else Ip[info[i]]=p1;
                         i++;  break;
         case ARCSIN   : //t1=I[info[i]];
                         if((x1=ic2var[info[i]])!=-1){occ1=*ic2occ[ic];}
                         p1 = Sin(Ip[ic++]);
                         if(x1!=-1)sat=(Ip[info[i]]&=
                                  projectOccurrence2(p1, x1, occ1, deriv[x1],derivs[x1][var_occ[x1]++]));
                         else Ip[info[i]]=p1;
                         i++;  break;
         case ARCTAN   : //t1=I[info[i]];
                         if((x1=ic2var[info[i]])!=-1){occ1=*ic2occ[ic]; t1=ev_occ[occ1];}
                         p1 = Tan(Ip[ic++]);
                         if(x1!=-1)sat=(Ip[info[i]]&=
                                  projectOccurrence2(p1, x1, occ1, deriv[x1],derivs[x1][var_occ[x1]++]));
                         else Ip[info[i]]=t1;
                         i++;  break;
         case COSH     : //t1=I[info[i]];
                         if((x1=ic2var[info[i]])!=-1){occ1=*ic2occ[ic];}
                         p1=infty;
                         sat = proj_cosh (Ip[ic++], p1);
                         if(x1!=-1)sat&=(Ip[info[i]]&=
                                  projectOccurrence2(p1, x1, occ1, deriv[x1],derivs[x1][var_occ[x1]++]));
                         else Ip[info[i]]=p1;
                         i++;  break;
         case SINH     : //t1=I[info[i]];
                         if((x1=ic2var[info[i]])!=-1){occ1=*ic2occ[ic];}
                         p1 &= ArSinh(Ip[ic++]);
                         if(x1!=-1)sat=(Ip[info[i]]&=
                                  projectOccurrence2(p1, x1, occ1, deriv[x1],derivs[x1][var_occ[x1]++]));
                         else Ip[info[i]]=p1;
                         i++;  break;
         case TANH     : //t1=I[info[i]];
                         if((x1=ic2var[info[i]])!=-1){occ1=*ic2occ[ic];}
                         p1=infty;
                         sat = proj_tanh(Ip[ic++], p1);
                         if(x1!=-1)sat&=(Ip[info[i]]&=
                                  projectOccurrence2(p1, x1, occ1, deriv[x1],derivs[x1][var_occ[x1]++]));
                         else Ip[info[i]]=p1;
                         i++;  break;
         case ARCCOSH  : //t1=I[info[i]];
                         if((x1=ic2var[info[i]])!=-1){occ1=*ic2occ[ic];}
                         p1=infty;
                         sat = proj_arccosh(Ip[ic++], t1);
                         if(x1!=-1)sat&=(Ip[info[i]]&=
                                  projectOccurrence2(p1, x1, occ1, deriv[x1],derivs[x1][var_occ[x1]++]));
                         else Ip[info[i]]=p1;
                         i++;  break;
         case ARCSINH  : //t1=I[info[i]];
                         if((x1=ic2var[info[i]])!=-1){occ1=*ic2occ[ic]; }
                         p1 = Sinh(Ip[ic++]);
                         if(x1!=-1) sat&=(Ip[info[i]]&=
                                  projectOccurrence2(p1, x1, occ1, deriv[x1],derivs[x1][var_occ[x1]++]));
                         else Ip[info[i]]=p1;
                         i++;  break;
         case ARCTANH  : //t1=I[info[i]];
                         if((x1=ic2var[info[i]])!=-1){occ1=*ic2occ[ic]; }
                         p1 = Tanh(Ip[ic++]);
                         if(x1!=-1)sat&=(Ip[info[i]]&=
                                  projectOccurrence2(p1, x1, occ1, deriv[x1],derivs[x1][var_occ[x1]++]));
                         else Ip[info[i]]=p1;
                         i++;  break;
         case SYMBOL   : //if (space.entity(info[i]).type==IBEX_VAR || space.entity(info[i]).type==IBEX_TMP) 
//      if (space.entity(info[i]).type==IBEX_EPR) 
//	cout << space.domain(info[i]) << " ^ " <<  I[ic] << endl;
            if (space.entity(info[i]).type!=IBEX_SYB && space.entity(info[i]).type!=IBEX_UPR) {
               sat = space.domain(info[i]) &= Ip[ic];
               
//                if(trace) cout <<"var:"<<space.domain(info[i])<<endl;
            }
            i++; ic++;     break;
            
            default       : throw NonRecoverableException("Internal bug: unknown evaluator code (please report this bug).");  
      }

      if (!sat) throw EmptyBoxException();    
   }
}
*/

void EvaluatorOG::gradient(Space& space, 
                   INTERVAL *deriv, INTERVAL** derivs, int** vocc2cocc) {
  int i   = 0;
  int ic  = 0;
  //int mc  = 0;
  //int f   = 0;

  /*if(!eval_ready)*/ forward(space); //classical forward

  for (int k=0;k<n;k++){
     var_occ[k]=0;
     deriv[k]=0;
  }
 
  G[0] = 1.0;
  for (int i=1; i<itvl; i++) G[i]=0;


  for (int c=0; c<codel; c++) {
    int a;
    INTERVAL tmp;
    if (G[ic].empty()) throw NotDifferentiableException();

    if (G[ic].unbounded()) throw UnboundedResultException();
    switch (code[c]) {
    case CST     : ic++; break;
    case ADD     : G[info[i]]   += G[ic];  

                   if((a=ic2var[info[i]])!=-1){
                       vocc2cocc[a][var_occ[a]]=ic2occ[ic][0]; //to obtain the location of the var_occ[a]-th 
                                                           // occurrence of variable a in the constraint
                       derivs[a][var_occ[a]]=G[ic]; 
                       var_occ[a]++;
                   }
                   G[info[i+1]] += G[ic];

                   if((a=ic2var[info[i+1]])!=-1){
                       vocc2cocc[a][var_occ[a]]=ic2occ[ic][1]; //to obtain the location of the var_occ[a]-th 
                                                           // occurrence of variable a in the constraint
                       derivs[a][var_occ[a]]=G[ic]; 
                       var_occ[a]++;
                   }

		   i+=2; ic++; break;
                   
    case SUB     : G[info[i]]   += G[ic]; 
                   if((a=ic2var[info[i]])!=-1){
                       vocc2cocc[a][var_occ[a]]=ic2occ[ic][0]; //to obtain the location of the var_occ[a]-th 
                                                           // occurrence of variable a in the constraint
                       derivs[a][var_occ[a]]=G[ic]; 
                       var_occ[a]++;
                   }

                   tmp=-G[ic];
                   G[info[i+1]] += tmp; 

                   if((a=ic2var[info[i+1]])!=-1){
                       vocc2cocc[a][var_occ[a]]=ic2occ[ic][1]; //to obtain the location of the var_occ[a]-th 
                                                           // occurrence of variable a in the constraint
                       derivs[a][var_occ[a]]=tmp; 
                       var_occ[a]++;
                   }

		   i+=2; ic++; break;
                   
    case MUL     : tmp =  G[ic] * I[info[i+1]];
                   G[info[i]]   += tmp; 
                   if((a=ic2var[info[i]])!=-1){
                       vocc2cocc[a][var_occ[a]]=ic2occ[ic][0]; //to obtain the location of the var_occ[a]-th 
                                                           // occurrence of variable a in the constraint
                       derivs[a][var_occ[a]]=tmp; 
                       var_occ[a]++;
                   }

                   tmp =  G[ic] * I[info[i]];
                   G[info[i+1]] += tmp; 
                   if((a=ic2var[info[i+1]])!=-1){
                       vocc2cocc[a][var_occ[a]]=ic2occ[ic][1]; //to obtain the location of the var_occ[a]-th 
                                                           // occurrence of variable a in the constraint
                       derivs[a][var_occ[a]]=tmp; 
                       var_occ[a]++;
                   }

		   i+=2; ic++; break;
                   
    case DIV     : tmp = G[ic] / I[info[i+1]]; 
                   G[info[i]]   += tmp;

                   if((a=ic2var[info[i]])!=-1){
                       vocc2cocc[a][var_occ[a]]=ic2occ[ic][0]; //to obtain the location of the var_occ[a]-th 
                                                           // occurrence of variable a in the constraint
                       derivs[a][var_occ[a]]=tmp; 
                       var_occ[a]++;
                   }

                   tmp = G[ic]*(-I[info[i]])/Sqr(I[info[i+1]]);
                   G[info[i+1]] += tmp; 

                   if((a=ic2var[info[i+1]])!=-1){
                       vocc2cocc[a][var_occ[a]]=ic2occ[ic][1]; //to obtain the location of the var_occ[a]-th 
                                                           // occurrence of variable a in the constraint
                       derivs[a][var_occ[a]]=tmp; 
                       var_occ[a]++;
                   }

		   i+=2; ic++; break;
    case MIN     : 
    case MAX     : 
    case INF     :
    case MID     :
    case SUP     :
    case M_INF   :
    case M_MID   :
    case M_SUP   :
    case SIGN    : throw NotDifferentiableException();                                        break;
    case MINUS   : tmp = -1.0 * G[ic];
                   G[info[i]] += tmp;  
                   if((a=ic2var[info[i]])!=-1){
                       vocc2cocc[a][var_occ[a]]=*ic2occ[ic]; //to obtain the location of the var_occ[a]-th 
                                                           // occurrence of variable a in the constraint
                       derivs[a][var_occ[a]]=tmp; 
                       var_occ[a]++;
                   }
                   i++; ic++; break;
                  
    case SQRT    : tmp = G[ic] * 0.5 / Sqrt(I[info[i]]);
                   G[info[i]] += tmp; 
                   if((a=ic2var[info[i]])!=-1){
                       vocc2cocc[a][var_occ[a]]=*ic2occ[ic]; //to obtain the location of the var_occ[a]-th 
                                                           // occurrence of variable a in the constraint
                       derivs[a][var_occ[a]]=tmp; 
                       var_occ[a]++;
                   }
                   i++; ic++; break;
                 
    case LOG     :tmp= G[ic] / I[info[i]]; 
                  G[info[i]] += tmp; 
                   if((a=ic2var[info[i]])!=-1){
                       vocc2cocc[a][var_occ[a]]=*ic2occ[ic]; //to obtain the location of the var_occ[a]-th 
                                                           // occurrence of variable a in the constraint
                       derivs[a][var_occ[a]]=tmp; 
                       var_occ[a]++;
                   }
                  i++; ic++; break;

    case EXP     :tmp= G[ic] * Exp(I[info[i]]); 
                  G[info[i]] += tmp; 
                   if((a=ic2var[info[i]])!=-1){
                       vocc2cocc[a][var_occ[a]]=*ic2occ[ic]; //to obtain the location of the var_occ[a]-th 
                                                           // occurrence of variable a in the constraint
                       derivs[a][var_occ[a]]=tmp; 
                       var_occ[a]++;
                   }
                  i++; ic++; break;

    case POW     : tmp=G[ic] * info[i] * Power(I[info[i+1]], info[i]-1);
                   G[info[i+1]] += tmp; 
                   if((a=ic2var[info[i+1]])!=-1){
                       vocc2cocc[a][var_occ[a]]=*ic2occ[ic]; //to obtain the location of the var_occ[a]-th 
                                                           // occurrence of variable a in the constraint
                       derivs[a][var_occ[a]]=tmp; 
                       var_occ[a]++;
                   }
                  i+=2; ic++; break;

    case SQR     :
                  tmp= G[ic] * 2.0 * I[info[i]];
                  G[info[i]] += tmp; 
                   if((a=ic2var[info[i]])!=-1){
                       vocc2cocc[a][var_occ[a]]=*ic2occ[ic]; //to obtain the location of the var_occ[a]-th 
                                                           // occurrence of variable a in the constraint
                       derivs[a][var_occ[a]]=tmp; 
                       var_occ[a]++;
                   }
                  i++; ic++;  break;

    case COS     : tmp= G[ic] * -Sin(I[info[i]]); 
                   G[info[i]] +=tmp;
                   if((a=ic2var[info[i]])!=-1){
                       vocc2cocc[a][var_occ[a]]=*ic2occ[ic]; //to obtain the location of the var_occ[a]-th 
                                                           // occurrence of variable a in the constraint
                       derivs[a][var_occ[a]]=tmp; 
                       var_occ[a]++;
                   }
                   i++; ic++;  break;
    
    
    case SIN     : tmp= G[ic] * Cos(I[info[i]]);
                   G[info[i]] +=tmp;
                   if((a=ic2var[info[i]])!=-1){
                       vocc2cocc[a][var_occ[a]]=*ic2occ[ic]; //to obtain the location of the var_occ[a]-th 
                                                           // occurrence of variable a in the constraint
                       derivs[a][var_occ[a]]=tmp; 
                       var_occ[a]++;
                   }
                   i++; ic++;  break;
    case TAN     : tmp= G[ic] * (1.0 + Sqr(Tan(I[info[i]])));
                   G[info[i]] +=tmp;
                   if((a=ic2var[info[i]])!=-1){
                       vocc2cocc[a][var_occ[a]]=*ic2occ[ic]; //to obtain the location of the var_occ[a]-th 
                                                           // occurrence of variable a in the constraint
                       derivs[a][var_occ[a]]=tmp; 
                       var_occ[a]++;
                   }
                   i++; ic++;  break;
    case ARCCOS  : tmp= G[ic] * -1.0 / Sqrt(1.0-Sqr(I[info[i]]));
                   G[info[i]] +=tmp; 
                   if((a=ic2var[info[i]])!=-1){
                       vocc2cocc[a][var_occ[a]]=*ic2occ[ic]; //to obtain the location of the var_occ[a]-th 
                                                           // occurrence of variable a in the constraint
                       derivs[a][var_occ[a]]=tmp; 
                       var_occ[a]++;
                   }
                   i++; ic++;  break;
    case ARCSIN  : tmp= G[ic] * 1.0 / Sqrt(1.0-Sqr(I[info[i]]));
                   G[info[i]] +=tmp;
                   if((a=ic2var[info[i]])!=-1){
                       vocc2cocc[a][var_occ[a]]=*ic2occ[ic]; //to obtain the location of the var_occ[a]-th 
                                                           // occurrence of variable a in the constraint
                       derivs[a][var_occ[a]]=tmp; 
                       var_occ[a]++;
                   }
                   i++; ic++;  break;
    case ARCTAN  : tmp= G[ic] * 1.0 / (1.0+Sqr(I[info[i]]));
                   G[info[i]] +=tmp;
                   if((a=ic2var[info[i]])!=-1){
                       vocc2cocc[a][var_occ[a]]=*ic2occ[ic]; //to obtain the location of the var_occ[a]-th 
                                                           // occurrence of variable a in the constraint
                       derivs[a][var_occ[a]]=tmp; 
                       var_occ[a]++;
                   }
                   i++; ic++;  break;
    case COSH    : tmp= G[ic] * Sinh(I[info[i]]);
                   G[info[i]] +=tmp;
                   if((a=ic2var[info[i]])!=-1){
                       vocc2cocc[a][var_occ[a]]=*ic2occ[ic]; //to obtain the location of the var_occ[a]-th 
                                                           // occurrence of variable a in the constraint
                       derivs[a][var_occ[a]]=tmp; 
                       var_occ[a]++;
                   }
                   i++; ic++;  break;
    case SINH    : tmp= G[ic] * Cosh(I[info[i]]);
                   G[info[i]] +=tmp;
                   if((a=ic2var[info[i]])!=-1){
                       vocc2cocc[a][var_occ[a]]=*ic2occ[ic]; //to obtain the location of the var_occ[a]-th 
                                                           // occurrence of variable a in the constraint
                       derivs[a][var_occ[a]]=tmp; 
                       var_occ[a]++;
                   }
                   i++; ic++;  break;
    case TANH    : tmp= G[ic] * (1.0 - Sqr(Tanh(I[info[i]])));
                   G[info[i]] +=tmp;
                   if((a=ic2var[info[i]])!=-1){
                       vocc2cocc[a][var_occ[a]]=*ic2occ[ic]; //to obtain the location of the var_occ[a]-th 
                                                           // occurrence of variable a in the constraint
                       derivs[a][var_occ[a]]=tmp; 
                       var_occ[a]++;
                   }
                   i++; ic++;  break;
    case ARCCOSH : tmp= G[ic] * 1.0 / Sqrt(Sqr(I[info[i]]) -1.0);
                   G[info[i]] +=tmp;
                   if((a=ic2var[info[i]])!=-1){
                       vocc2cocc[a][var_occ[a]]=*ic2occ[ic]; //to obtain the location of the var_occ[a]-th 
                                                           // occurrence of variable a in the constraint
                       derivs[a][var_occ[a]]=tmp; 
                       var_occ[a]++;
                   }
                   i++; ic++;  break;
    case ARCSINH : tmp= G[ic] * 1.0 / Sqrt(1.0+Sqr(I[info[i]]));
                   G[info[i]] +=tmp;
                   if((a=ic2var[info[i]])!=-1){
                       vocc2cocc[a][var_occ[a]]=*ic2occ[ic]; //to obtain the location of the var_occ[a]-th 
                                                           // occurrence of variable a in the constraint
                       derivs[a][var_occ[a]]=tmp; 
                       var_occ[a]++;
                   }
                   i++; ic++;  break;
    case ARCTANH : tmp= G[ic] * 1.0 / (1.0-Sqr(I[info[i]]));
                   G[info[i]] +=tmp;
                   if((a=ic2var[info[i]])!=-1){
                       vocc2cocc[a][var_occ[a]]=*ic2occ[ic]; //to obtain the location of the var_occ[a]-th 
                                                           // occurrence of variable a in the constraint
                       derivs[a][var_occ[a]]=tmp; 
                       var_occ[a]++;
                   }
                   i++; ic++;  break;
    case SYMBOL  : tmp= G[ic];
                   deriv[ic2var[ic]] += tmp;
                   // *space.entity(info[i]).deriv += tmp; 
                                              //sum of abs values of gradients
                   i++; ic++;
                   break;
//     case FUNCTION: *space.entity(info[i]).deriv += G[ic++]; i++;                              break;
//     //case ASSIGN  : break;
//     case APPLY   : func[f]->gradient(*args[f]); f++; break;
    default : throw NonRecoverableException("Matrix differentiation not implemented yet");
    }

  }

}

} // end namespace
