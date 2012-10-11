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
#include "IbexEvaluatorAffine.h"
#include "IbexToken.h"
#include "IbexProjections.h"
#include "IbexLinear.h"
#include "IbexFunction.h"
#include "IbexInnerProjections.h"

namespace ibex {

//static const char* tokentostring(int tok);



EvaluatorAffine::~EvaluatorAffine() {
  //Evaluator::~Evaluator();
  if (itvl>0) delete[] AF;
}

void EvaluatorAffine::init(){
   if (itvl>0) AF = new AF2_expr[itvl]; 
}

void EvaluatorAffine::eval_AF(const Space& space, AF2_expr::linear_mode mode) const{
  int i   = infol-1;
  int ic  = itvl-1;
  int icc = icstl-1;
  int mc  = matl-1;
  int mcc = mcstl-1;
  int f   = funcl-1;
  bool sat;

  for (int c=codel-1; c>=0; c--) {
//     cout << code[c]<< endl;
    switch (code[c]) {
    case CST      : AF[ic] = AF2_expr(Ic[icc--],space.nb_var(),space.nb_var()); sat = !AF[ic--].empty(); break;
    case SUB      : sub(AF[info[i-1]], AF[info[i]], AF[ic]);  i-=2;  sat = !AF[ic--].empty(); break;
    case ADD      : add(AF[info[i-1]], AF[info[i]], AF[ic]); i-=2;  sat = !AF[ic--].empty(); break;
    case DIV      : div(AF[info[i-1]], AF[info[i]], AF[ic], mode);  i-=2;   sat = !AF[ic--].empty(); break;
    case MUL      : mult(AF[info[i-1]], AF[info[i]], AF[ic]);  i-=2;  sat = !AF[ic--].empty(); break;
    case MINUS    : minus(AF[info[i--]],AF[ic]);  sat = !AF[ic--].empty(); break;
    case POW      : pow(AF[info[i]], info[i-1], AF[ic], mode); i-=2; sat = !AF[ic--].empty(); break;
    case SQR      : sqr(AF[info[i--]], AF[ic], mode); sat = !AF[ic--].empty(); break;
    case SQRT     : sqrt(AF[info[i--]], AF[ic], mode); sat = !AF[ic--].empty(); break;
    case EXP      : exp(AF[info[i--]], AF[ic], mode);  sat = !AF[ic--].empty(); break;
    case LOG      : log(AF[info[i--]], AF[ic], mode);  sat = !AF[ic--].empty(); break;
    case SYMBOL   : AF[ic] = AF2_expr(space.domain(info[i]), info[i], space.nb_var()); i--; sat = !AF[ic--].empty(); break;
    default       : 
      throw NonRecoverableException("Internal bug: unknown evaluator code (please report this bug).");
    }
//     cout << "out" << endl;

    if (!sat) throw EmptyBoxException();

    /* warning: empty results not yet implemented for matrix
     * addition, etc. An empty matrix can only be raised from 
     * operator&=*/
  }

  // ----------------------- debug --------------------
  
}


}



 // end namespace
