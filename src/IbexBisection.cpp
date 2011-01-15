/*---------------------------------------------------------------------------------
 * 
 * Bisection structure
 * -------------------
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

#include <float.h>
#include "IbexBisection.h"

namespace ibex {

Bisection::Bisection(const INTERVAL_VECTOR& box, int var, REAL ratio) : box1(box), box2(box), var(var), ratio(ratio) {
  REAL point;

  if (Inf(box(var+1))==BiasNegInf) {

    if (Sup(box(var+1))<-DBL_MAX) {
      std::ostringstream oss;
      oss << "Unable to bisect " << box;
      throw NonRecoverableException(oss.str());
    } 

    else if (Sup(box(var+1))==BiasPosInf) {
      box1(var+1) = INTERVAL(BiasNegInf,0);
      box2(var+1) = INTERVAL(0,BiasPosInf);      
    } 

    else {
      box1(var+1) = INTERVAL(BiasNegInf,-DBL_MAX);
      box2(var+1) = INTERVAL(-DBL_MAX,Sup(box(var+1)));
    }

  } 

  else if (Sup(box(var+1))==BiasPosInf) {    

    if (Inf(box(var+1))>DBL_MAX) {
      std::ostringstream oss;
      oss << "Unable to bisect " << box;
      throw NonRecoverableException(oss.str());
    } 

    else {
      box1(var+1) = INTERVAL(Inf(box(var+1)),DBL_MAX);
      box2(var+1) = INTERVAL(DBL_MAX,BiasPosInf);
    }
    
  } 

  else {

    if (ratio==0.5) 
      point = Mid(box1(var+1));
    else {
      point = Inf(box1(var+1))+ratio*Diam(box1(var+1));
      if (point > Sup(box1(var+1))) return; // do nothing
    }
    box1(var+1) = INTERVAL(Inf(box1(var+1)), point);
    box2(var+1) = INTERVAL(point, Sup(box2(var+1)));
  }
  
}

} // end namespace
