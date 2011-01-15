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
#include "IbexEvaluator.h"
#include "IbexToken.h"
#include "IbexProjections.h"
#include "IbexLinear.h"
#include "IbexFunction.h"

namespace ibex {

static const char* tokentostring(int tok);

Evaluator::~Evaluator() {
  delete[] code;
  delete[] info;
  if (itvl>0) {
    delete[] I;
    delete[] G;
  }
  if (matl>0) 
    delete[] M;
  if (icstl>0) 
    delete[] Ic;
  if (mcstl>0) 
    delete[] Mc;  

  /* Delete domains of temporary variables */
  for (int i=0; i<funcl; i++) {
    for (int key=func[i]->first_tmp_key; key<func[i]->env.nb_keys(); key++)
      delete args[i]->entity(key).domain;
  }
  delete[] func;
  for (int i=0; i<funcl; i++) delete args[i];
  delete[] args;
}

void Evaluator::read_matrix(const Space& space, int mc, int i) const {
  int size2=info[i-2];
  int size3=info[i-1];
  int num=info[i];
  for (int j=0; j<size2; j++)       
    for (int k=0; k<size3; k++)
      M[mc](j+1,k+1) = space.domain(num+j*size3+k); 
  //cout << "read:" << M[mc] << endl;
}

void Evaluator::write_matrix(Space& space, int mc, int i) const {
  int size2=info[i];
  int size3=info[i+1];
  int num=info[i+2];

  for (int j=0; j<size2; j++)       
    for (int k=0; k<size3; k++) {
      //      if (space.entity(num+j*size3+k).type==IBEX_VAR || space.entity(num+j*size3+k).type==IBEX_TMP) 
      if (space.entity(num+j*size3+k).type!=IBEX_SYB && space.entity(num+j*size3+k).type!=IBEX_UPR) {
	if (!(space.domain(num+j*size3+k) &= M[mc](j+1,k+1))) throw EmptyBoxException();      
//	cout << space.domain(num+j*size3+k) << "<-" endl;
      }
    }
}

// void Evaluator::write_matrix(Space& space, int i) const throw(EmptyBoxException) {
//   int size2=info[i-3];
//   int size3=info[i-2];
//   int num=info[i-1];
//   for (int j=0; j<size2; j++)       
//     for (int k=0; k<size3; k++)
//       if (!(space.domain(num+j*size3+k) &= M[info[i]](j+1,k+1))) throw EmptyBoxException();
// }

// void Evaluator::read_matrix(Space& space, int i) const throw(EmptyBoxException) {
//   int size2=info[i];
//   int size3=info[i+1];
//   int num=info[i+2];
//   for (int j=0; j<size2; j++)       
//     for (int k=0; k<size3; k++)
//       if (!(M[info[i+3]](j+1,k+1) &= space.domain(num+j*size3+k))) throw EmptyBoxException();
// }

void Evaluator::forward(const Space& space) const {
  int i   = infol-1;
  int ic  = itvl-1;
  int icc = icstl-1;
  int mc  = matl-1;
  int mcc = mcstl-1;
  int f   = funcl-1;
  bool sat;

  for (int c=codel-1; c>=0; c--) {
    
    switch (code[c]) {
      //case LT       : 
    //case LEQ      : 
    //case EQU      : 
    //case GEQ      : 
    //case GT       : I[0]   &= Ic[0];                             sat = !I[ic--].empty(); break;
    case CST      : I[ic] = Ic[icc--];                         sat = !I[ic--].empty(); break;
    case SUB      : I[ic] = I[info[i-1]] - I[info[i]]; i-=2;   sat = !I[ic--].empty(); break;
    case ADD      : I[ic] = I[info[i-1]] + I[info[i]]; i-=2;   sat = !I[ic--].empty(); break;
    case DIV      : I[ic] = I[info[i-1]] / I[info[i]]; i-=2;   sat = !I[ic--].empty(); break;
    case MUL      : I[ic] = I[info[i-1]] * I[info[i]]; i-=2;   sat = !I[ic--].empty(); break;
    case MIN      : I[ic] = Min(I[info[i-1]], I[info[i]]); i-=2; sat = !I[ic--].empty(); break;
    case MAX      : I[ic] = Max(I[info[i-1]], I[info[i]]); i-=2; sat = !I[ic--].empty(); break;
    case ARCTAN2  : I[ic] = ArcTan2(I[info[i-1]], I[info[i]]); i-=2; sat = !I[ic--].empty(); break;
    case MINUS    : I[ic] = -I[info[i--]];                     sat = !I[ic--].empty(); break;
    case SIGN     : I[ic] = Sign(I[info[i--]]);                sat = !I[ic--].empty(); break;
    case ABS      : I[ic] = IAbs(I[info[i--]]);                sat = !I[ic--].empty(); break;
    case POW      : I[ic] = Power(I[info[i]],info[i-1]); i-=2; sat = !I[ic--].empty(); break;
    case SQR      : I[ic] = Sqr(I[info[i--]]);                 sat = !I[ic--].empty(); break;
    case SQRT     : I[ic] = Sqrt(I[info[i--]]);                sat = !I[ic--].empty(); break;
    case LOG      : I[ic] = Log(I[info[i--]]);                 sat = !I[ic--].empty(); break;
    case EXP      : I[ic] = Exp(I[info[i--]]);                 sat = !I[ic--].empty(); break;
    case COS      : I[ic] = Cos(I[info[i--]]);                 sat = !I[ic--].empty(); break;
    case SIN      : I[ic] = Sin(I[info[i--]]);                 sat = !I[ic--].empty(); break;
    case TAN      : I[ic] = Tan(I[info[i--]]);                 sat = !I[ic--].empty(); break;
    case ARCCOS   : I[ic] = ArcCos(I[info[i--]]);              sat = !I[ic--].empty(); break;
    case ARCSIN   : I[ic] = ArcSin(I[info[i--]]);              sat = !I[ic--].empty(); break;
    case ARCTAN   : I[ic] = ArcTan(I[info[i--]]);              sat = !I[ic--].empty(); break;
    case COSH     : I[ic] = Cosh(I[info[i--]]);                sat = !I[ic--].empty(); break;
    case SINH     : I[ic] = Sinh(I[info[i--]]);                sat = !I[ic--].empty(); break;
    case TANH     : I[ic] = Tanh(I[info[i--]]);                sat = !I[ic--].empty(); break;
    case ARCCOSH  : I[ic] = ArCosh(I[info[i--]]);              sat = !I[ic--].empty(); break; 
    case ARCSINH  : I[ic] = ArSinh(I[info[i--]]);              sat = !I[ic--].empty(); break;
    case ARCTANH  : I[ic] = ArTanh(I[info[i--]]);              sat = !I[ic--].empty(); break;
    case SYMBOL   : I[ic] = space.domain(info[i--]);           sat = !I[ic--].empty(); break;
    case APPLY    : I[ic] = INTERVAL(BiasNegInf,BiasPosInf);  //  cout << "call " << func[f]->name << " forward\n";
                    func[f]->forward(*args[f]); f--;           sat = !I[ic--].empty(); break;
    //case M_EQU    : M[0]   &= Mc[0];                             sat = !I[ic--].empty(); break;
    case INF      : I[ic--] = Inf(I[info[i--]]);                 break;
    case MID      : I[ic--] = Mid(I[info[i--]]); /*cout << I[info[i+1]] << " mid->" << I[ic+1] << endl;*/                break;
    case SUP      : I[ic--] = Sup(I[info[i--]]);                 break;
    case M_CST    : M[mc] = Mc[mcc--];                         sat = !M[mc--].empty(); break;
    case M_ADD    : M[mc] = M[info[i-1]] + M[info[i]]; i-=2;   sat = !M[mc--].empty(); break;
    case M_SUB    : M[mc] = M[info[i-1]] - M[info[i]]; i-=2;   sat = !M[mc--].empty(); break;
    case M_MUL    : M[mc] = M[info[i-1]] * M[info[i]]; i-=2;   sat = !M[mc--].empty(); break;
    case M_SCAL   : M[mc] = I[info[i-1]] * M[info[i]]; i-=2;   sat = !M[mc--].empty(); break;
    case M_VEC    : SetRow(M[mc], 1, M[info[i-1]] * Row(M[info[i]],1)); i-=2;  sat = !M[mc--].empty(); break;
    case V_DOT    : I[ic] =   Row(M[info[i-1]],1) * Row(M[info[i]],1);  i-=2;  sat = !I[ic--].empty(); break;
    case M_MINUS  : M[mc] = -M[info[i--]];                     sat = !M[mc--].empty(); break;
    case M_SYMBOL : read_matrix(space, mc,i); i-=3;            sat = !M[mc--].empty(); break;
    case M_TRANS  : M[mc] = ::Transpose(M[info[i--]]);         sat = !M[mc--].empty(); break; 
    case M_APPLY  : Initialize(M[mc],INTERVAL(BiasNegInf,BiasPosInf));
                    func[f]->forward(*args[f]); f--;           sat = !M[mc--].empty(); break;
    case M_INF    : M[ic--] = Inf(M[info[i--]]);                 break;
    case M_MID    : M[ic--] = Mid(M[info[i--]]);                 break;
    case M_SUP    : M[ic--] = Sup(M[info[i--]]);                 break;
    default       : throw NonRecoverableException("Internal bug: unknown evaluator code (please report this bug).");
    }

    if (!sat) throw EmptyBoxException(); 
    /* warning: empty results not yet implemented for matrix
     * addition, etc. An empty matrix can only be raised from 
     * operator&=*/
  }
  

  // ----------------------- debug --------------------
  {
//   cout << "\nforward: ----------\n";
//   int mc=0;
//   int ic=0;
//   for (int c=0; c<=codel-1; c++) {
//     cout << tokentostring(code[c]) << " ";
//     switch(code[c]) {
//     case M_CST    : 
//     case M_ADD    : 
//     case M_SUB    : 
//     case M_MUL    : 
//     case M_SCAL   : 
//     case M_VEC    : 
//     case V_DOT    :
//     case M_MINUS  : 
//     case M_SYMBOL : 
//     case M_APPLY  : cout << M[mc++] << "   "; break;
//     default : cout << I[ic++] << "   "; break;
//     }
//   }
//   cout << endl;
  }
  // ----------------------- debug --------------------

}

void Evaluator::backward(Space& space) const { 
  int i   = 0;
  int ic  = 0;
  int mc  = 0;
  int f   = 0;

  bool sat=true;

  for (int c=0; c<codel; c++) {

    switch (code[c]) {
      
    case CST      : ic++; break;
    case ADD      : sat  = (I[info[i]] &= (I[ic] - I[info[i+1]]));
                    sat &= (I[info[i+1]] &= (I[ic] - I[info[i]])); 
		    ic++; i+=2; break;
    case SUB      : sat  = (I[info[i]] &= (I[ic] + I[info[i+1]]));
                    sat &= (I[info[i+1]] &= (I[info[i]] - I[ic])); 
		    ic++; i+=2; break;
    case MUL      : sat = proj_mult(I[ic],I[info[i]],I[info[i+1]]);
		    ic++; i+=2; break;
    case DIV      : sat  = (I[info[i]] &= (I[ic] * I[info[i+1]])); 
                    sat &= (I[info[i+1]].div_intersect(I[info[i]], I[ic])); 
		    ic++; i+=2; break;
    case MIN      : sat = proj_min(I[ic++], I[info[i]], I[info[i+1]]); i+=2; break;
    case MAX      : sat = proj_max(I[ic++], I[info[i]], I[info[i+1]]); i+=2; break;
    case ARCTAN2  : /* TO DO!!! */ sat = true; ic++; i+=2;                  break;
    case SIGN     : sat = proj_sign(I[ic++], I[info[i++]]);                 break;
    case ABS      : sat = proj_abs(I[ic++], I[info[i++]]);                 break;
    case MINUS    : sat = I[info[i++]] &= -1.0*I[ic++];                     break;
    case SQRT     : sat = I[info[i++]] &= Sqr(I[ic++]);                     break;
    case LOG      : sat = I[info[i++]] &= Exp(I[ic++]);                     break;
    case EXP      : sat = proj_exp(I[ic++], I[info[i++]]);                  break;
    case POW      : sat = proj_power(I[ic++], I[info[i+1]], info[i]); i+=2; break;
    case SQR      : sat = proj_sqr  (I[ic++], I[info[i++]]);                break;
    case COS      : sat = proj_trigo(I[ic++], I[info[i++]], COS);           break;
    case SIN      : sat = proj_trigo(I[ic++], I[info[i++]], SIN);           break;
    case TAN      : sat = proj_trigo(I[ic++], I[info[i++]], TAN);           break;
    case ARCCOS   : sat = I[info[i++]] &= Cos(I[ic++]);                     break;
    case ARCSIN   : sat = I[info[i++]] &= Sin(I[ic++]);                     break;
    case ARCTAN   : sat = I[info[i++]] &= Tan(I[ic++]);                     break;
    case COSH     : sat = proj_cosh (I[ic++], I[info[i++]]);                break;
    case SINH     : sat = I[info[i++]] &= ArSinh(I[ic++]);                  break;
    case TANH     : sat = proj_tanh(I[ic++], I[info[i++]]);                 break;
    case ARCCOSH  : sat = proj_arccosh(I[ic++], I[info[i++]]);              break;
    case ARCSINH  : sat = I[info[i++]] &= Sinh(I[ic++]);                    break;
    case ARCTANH  : sat = I[info[i++]] &= Tanh(I[ic++]);                    break;
    case SYMBOL   : //if (space.entity(info[i]).type==IBEX_VAR || space.entity(info[i]).type==IBEX_TMP) 
//      if (space.entity(info[i]).type==IBEX_EPR) 
//	cout << space.domain(info[i]) << " ^ " <<  I[ic] << endl;
      if (space.entity(info[i]).type!=IBEX_SYB && space.entity(info[i]).type!=IBEX_UPR) 
		      sat = space.domain(info[i]) &= I[ic];  i++; ic++;     break;
    case APPLY    : func[f]->backward(*args[f]); f++; ic++;                 break;
    case INF      : 
    case MID      : 
    case SUP      : i++; ic++; break;
    case M_CST    : mc++; break;
    case M_ADD    : sat = (M[info[i]] &= (M[mc] - M[info[i+1]]));
                    sat &= (M[info[i+1]] &= (M[mc] - M[info[i]])); 
		    mc++; i+=2; break;
    case M_SUB    : sat = (M[info[i]] &= (M[mc] + M[info[i+1]]));
                    sat &= (M[info[i+1]] &= (M[info[i]] - M[mc])); 
		    mc++; i+=2; break;
    case M_MUL    : ContractMult(M[info[i]], M[info[i+1]], M[mc], 0.1); 
		    mc++; i+=2; break;
    case M_SCAL   : ContractMult(I[info[i]], M[info[i+1]], M[mc], 0.1);
                    mc++; i+=2; break;
    case M_VEC    : { INTERVAL_MATRIX right(ColDimension(M[info[i+1]]),1);
		      INTERVAL_MATRIX res(RowDimension(M[info[i]]),1);
		      SetCol(right,1,Row(M[info[i+1]],1));
		      SetCol(res,1,Row(M[mc],1));
		      ContractMult(M[info[i]], right, res, 0.1);
		      SetRow(M[info[i+1]],1,Col(right,1)); 
                      mc++; i+=2; 
                    } break;
    case V_DOT    : { INTERVAL_MATRIX right(ColDimension(M[info[i+1]]),1);
		      INTERVAL_MATRIX res(1,1);
		      SetCol(right,1,Row(M[info[i+1]],1));
		      res(1,1)=I[ic]; 
		      ContractMult(M[info[i]], right, res, 0.1);
		      SetRow(M[info[i+1]],1,Col(right,1)); 
		      I[ic]=res(1,1);
                      ic++; i+=2; break;		      
                    } 
    case M_MINUS  : M[info[i++]] &= -M[mc++];                    break;
    case M_SYMBOL : write_matrix(space, mc++,i); i+=3;           break;
    case M_TRANS  : M[info[i++]] &= ::Transpose(M[mc++]);        break;
    case M_APPLY  : func[f]->backward(*args[f]); f++; mc++;      break;
    case M_INF    : 
    case M_MID    : 
    case M_SUP    : i++; mc++; break;
    default       : throw NonRecoverableException("Internal bug: unknown evaluator code (please report this bug).");  
    }

    if (!sat) throw EmptyBoxException();    
  }

  // ----------------------- debug --------------------
  {
//   cout << "\nbackward: ----------\n";
//   int mc=0;
//   int ic=0;
//   for (int c=0; c<=codel-1; c++) {
//     cout << tokentostring(code[c]) << "  \n";
//     switch(code[c]) {
//     case M_CST    : 
//     case M_ADD    : 
//     case M_SUB    : 
//     case M_MUL    : 
//     case M_SCAL   : 
//     case M_VEC    : 
//     case V_DOT    : 
//     case M_MINUS  : 
//     case M_SYMBOL : 
//     case M_APPLY  : cout << M[mc++] << "   "; break;
//     default : cout << I[ic++] << "   "; break;
//     }
//   }
//   cout << endl;
  }
  // ----------------------- debug --------------------

}

void Evaluator::gradient(const Space& space) const { 

  int i   = 0;
  int ic  = 0;
  //int mc  = 0;
  int f   = 0;

  forward(space);

  G[0] = 1.0;
  for (int i=1; i<itvl; i++) G[i]=0;

  for (int c=0; c<codel; c++) {

    if (G[ic].empty()) throw NotDifferentiableException();

    if (G[ic].unbounded()) throw UnboundedResultException();
    
    switch (code[c]) {
    case CST     : ic++; break;
    case ADD     : G[info[i]]   += G[ic]; 
                   G[info[i+1]] += G[ic]; 
		   i+=2; ic++; break;
    case SUB     : G[info[i]]   += G[ic]; 
                   G[info[i+1]] += -G[ic];
		   i+=2; ic++; break;
    case MUL     : G[info[i]]   += G[ic] * I[info[i+1]]; 
                   G[info[i+1]] += G[ic] * I[info[i]];                  
		   i+=2; ic++; break;
    case DIV     : G[info[i]]   += G[ic] / I[info[i+1]]; 
                   G[info[i+1]] += G[ic]*(-I[info[i]])/Sqr(I[info[i+1]]); 
		   i+=2; ic++; break;
    case MIN     : 
    case MAX     : 
    case ARCTAN2 : 
    case INF     :
    case MID     :
    case SUP     :
    case M_INF   :
    case M_MID   :
    case M_SUP   :
    case SIGN    : 
    case ABS     : throw NotDifferentiableException();                                        break;
    case MINUS   : G[info[i]] += -1.0 * G[ic++]; i++;                                         break;
    case SQRT    : G[info[i]] += G[ic++] * 0.5 / Sqrt(I[info[i]]); i++;                       break;
    case LOG     : G[info[i]] += G[ic++] / I[info[i]]; i++;                                   break;
    case EXP     : G[info[i]] += G[ic++] * Exp(I[info[i]]); i++;                              break;      
    case POW     : G[info[i+1]] += G[ic++] * info[i] * Power(I[info[i+1]], info[i]-1); i+=2;  break;
    case SQR     : G[info[i]] += G[ic++] * 2.0 * I[info[i]]; i++;                             break;
    case COS     : G[info[i]] += G[ic++] * -Sin(I[info[i]]); i++;                             break;
    case SIN     : G[info[i]] += G[ic++] * Cos(I[info[i]]); i++;                              break;
    case TAN     : G[info[i]] += G[ic++] * (1.0 + Sqr(Tan(I[info[i]]))); i++;                 break;
    case ARCCOS  : G[info[i]] += G[ic++] * -1.0 / Sqrt(1.0-Sqr(I[info[i]])); i++;             break;
    case ARCSIN  : G[info[i]] += G[ic++] * 1.0 / Sqrt(1.0-Sqr(I[info[i]])); i++;              break;
    case ARCTAN  : G[info[i]] += G[ic++] * 1.0 / (1.0+Sqr(I[info[i]])); i++;                  break;
    case COSH    : G[info[i]] += G[ic++] * Sinh(I[info[i]]); i++;                             break;
    case SINH    : G[info[i]] += G[ic++] * Cosh(I[info[i]]); i++;                             break;
    case TANH    : G[info[i]] += G[ic++] * (1.0 - Sqr(Tanh(I[info[i]]))); i++;                break;
    case ARCCOSH : G[info[i]] += G[ic++] * 1.0 / Sqrt(Sqr(I[info[i]]) -1.0); i++;             break;
    case ARCSINH : G[info[i]] += G[ic++] * 1.0 / Sqrt(1.0+Sqr(I[info[i]])); i++;              break;
    case ARCTANH : G[info[i]] += G[ic++] * 1.0 / (1.0-Sqr(I[info[i]])); i++;                  break;
    case SYMBOL  : *space.entity(info[i]).deriv += G[ic++]; i++;                              break;
    case APPLY   : func[f]->gradient(*args[f]); f++; break;
    default : throw NonRecoverableException("Matrix differentiation not implemented yet");
    }
  }
}


/* for debug purposes only */
std::ostream& operator<<(std::ostream& os, const Evaluator& eval) {
  int i=0;
  int icc=0;
  int mcc=0;

  int width[eval.codel];
  int totalwidth=0;

  os.setf(ios::left);
  os << endl << "[EVAL]  ";
  for (int c=0; c<=eval.codel-1; c++) {
    switch (eval.code[c]) {
    case LT       : 
    case LEQ      : 
    case EQU      : 
    case GEQ      : 
    case GT       : width[c]=3; break;
    case SYMBOL   : 
    case M_MINUS  : 
    case M_EQU    : width[c]=4; break;
    case CST      : 
    case M_CST    : width[c]=7; break;
    case SUB      : 
    case ADD      : 
    case DIV      : 
    case POW      : 
    case MIN      : 
    case MAX      : 
    case M_ADD    : 
    case M_SUB    : 
    case M_MUL    : 
    case M_SCAL   : 
    case M_VEC    : 
    case V_DOT    :
    case MUL      : width[c]=7; break;
    case MINUS    : 
    case INF      : 
    case MID      : 
    case SUP      : 
    case M_INF    : 
    case M_MID    : 
    case M_SUP    : 
    case SQR      :    
    case LOG      : 
    case EXP      : 
    case COS      : 
    case SIN      : 
    case TAN      : width[c]=4; break;
    case SIGN     :
    case SQRT     : 
    case COSH     : 
    case SINH     : 
    case TANH     : width[c]=5; break;
    case ARCCOS   : 
    case ARCSIN   : 
    case ARCTAN   :  
    case ARCTAN2  : width[c]=7; break;
    case ARCCOSH  : 
    case ARCSINH  : 
    case ARCTANH  : width[c]=8; break;
    case APPLY    : width[c]=6; break;
    case M_SYMBOL : width[c]=9; break;
    }    
    totalwidth+=width[c];
    os << setw(width[c]) << c;
  }
  os << endl << "--------";
  for (int c=0; c<totalwidth; c++) os << "-";
  os << endl << "codes: |";
  for (int c=0; c<=eval.codel-1; c++) os << setw(width[c]) << tokentostring(eval.code[c]);
  os << "    length=" <<  eval.codel;
  os << endl << "info:  |";
  for (int c=0; c<=eval.codel-1; c++) {
    switch (eval.code[c]) {
    case LT       : 
    case LEQ      : 
    case EQU      : 
    case GEQ      : 
    case GT       : 
    case CST      : 
    case M_CST    : 
    case M_EQU    : os << setw(width[c]) << " "; break;
    case SUB      : 
    case ADD      : 
    case DIV      : 
    case POW      : 
    case M_ADD    : 
    case M_SUB    : 
    case M_MUL    : 
    case M_SCAL   : 
    case M_VEC    : 
    case V_DOT    :
    case MIN      :
    case MAX      :
    case MUL      : 
    case ARCTAN2  : os << setw(3) << eval.info[i]; 
                    os << setw(4) << eval.info[i+1]; i+=2; break;
    case SIGN     :
    case MINUS    : 
    case INF      : 
    case MID      : 
    case SUP      : 
    case M_INF    : 
    case M_MID    : 
    case M_SUP    : 
    case SQR      : 
    case SQRT     : 
    case LOG      : 
    case EXP      : 
    case COS      : 
    case SIN      : 
    case TAN      : 
    case ARCCOS   : 
    case ARCSIN   : 
    case ARCTAN   : 
    case COSH     : 
    case SINH     : 
    case TANH     : 
    case ARCCOSH  : 
    case ARCSINH  : 
    case ARCTANH  : 
    case SYMBOL   : 
    case M_MINUS  : os << setw(width[c]) << eval.info[i++]; break;
    case M_SYMBOL : os << setw(3) << eval.info[i];
                    os << setw(3) << eval.info[i+1]; 
                    os << setw(3) << eval.info[i+2]; i+=3; break;
    case APPLY    : os << setw(width[c]) << " "; break;
    default : os << "????????";
    }    
  }
  os << "    length=" <<  eval.infol;
  os << endl << "const: |";
  for (int c=0; c<=eval.codel-1; c++) {
    if (eval.code[c] == CST) 
      os << setw(7) << Mid(eval.Ic[icc++]);
    else if (eval.code[c] == M_CST) { 
      os << setw(3) << RowDimension(eval.Mc[mcc]);
      os << "x";
      os << setw(3) << ColDimension(eval.Mc[mcc]); 
      mcc++;
    }
    else os << setw(width[c]) << "";
  }    
  os << "    length=" <<  eval.icstl << " (int)  " << eval.mcstl << " (mat)";


  os << endl << "I    : |";
  for (int ic=0; ic<=eval.itvl-1; ic++) {
    os << ic << ":" << eval.I[ic] << "  ";
  }
  os << endl;

  os << "M    : |";
  for (int mc=0; mc<=eval.matl-1; mc++) {
    os << mc << ":" << eval.M[mc] << "  ";
  }
  os << endl;

  return os;
}

/* for debug purposes only */
static const char* tokentostring(int tok) {
  switch (tok) {
  case APPLY    : return "apply";     break;
  case CST      : return "const";     break;
  case SYMBOL   : return "sym";       break;
  case LT       : return "<";         break;
  case LEQ      : return "<=";        break;
  case EQU      : return "=";         break;
  case GEQ      : return ">=";        break;
  case GT       : return ">";         break;
  case SUB      : return "-";         break;
  case ADD      : return "+";         break;
  case DIV      : return "/";         break;
  case MUL      : return "*";         break;
  case MAX      : return "max";       break;
  case MIN      : return "min";       break;
  case SIGN     : return "sgn";       break;
  case MINUS    : return "~";         break;
  case POW      : return "^";         break;
  case SQR      : return "sqr";       break;
  case SQRT     : return "sqrt";      break;
  case LOG      : return "ln";        break;
  case EXP      : return "exp";       break;
  case COS      : return "cos";       break;
  case SIN      : return "sin";       break;
  case TAN      : return "tan";       break;
  case ARCCOS   : return "arccos";    break;
  case ARCSIN   : return "arcsin";    break;
  case ARCTAN   : return "arctan";    break;
  case ARCTAN2  : return "arctan2";   break;
  case COSH     : return "cosh";      break;
  case SINH     : return "sinh";      break;
  case TANH     : return "tanh";      break;
  case ARCCOSH  : return "arccosh";   break;
  case ARCSINH  : return "arcsinh";   break;
  case ARCTANH  : return "arctanh";   break;
  case INF      : return "inf";     break;
  case MID      : return "mid";     break;  
  case SUP      : return "sup";     break;
  case M_APPLY  : return "m_apply";   break;
  case M_CST    : return "m_const";   break;
  case M_SYMBOL : return "m_sym";     break;
  case M_EQU    : return "m=";        break;
  case M_ADD    : return "m+";        break;
  case M_SUB    : return "m-";        break;
  case M_MUL    : return "m*";        break;
  case M_SCAL   : return "s*";        break;
  case M_VEC    : return "v*";        break;
  case V_DOT    : return ".";        break;
  case M_MINUS  : return "m~";        break;
  case M_INF    : return "m_inf";     break;
  case M_MID    : return "m_mid";     break;  
  case M_SUP    : return "m_sup";     break;
  default       : return "?";         break;     
 }
}

} // end namespace
