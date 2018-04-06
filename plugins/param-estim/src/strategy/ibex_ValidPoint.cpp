//============================================================================
//                                  I B E X                                   
// File        : ibex_ValidPoint.cpp
// Author      : Bertrand Neveu
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Apr 15, 2015
// Last Update : Apr 15, 2015
//============================================================================


#include "ibex_ValidPoint.h"
#include <stdlib.h>

using namespace std;

namespace ibex {
pair<Backtrackable*,Backtrackable*> ValidPoint::down(const BisectionPoint& b) {
	return pair<Backtrackable*,Backtrackable*> (new ValidPoint(*this),new ValidPoint(*this));
}
  ValidPoint::ValidPoint():  validpoints_number(0){};
  


  ValidPoint::~ValidPoint() {delete point;}

  ValidPoint::ValidPoint (const ValidPoint& qp) {
    point= new Vector (*(qp.point));
    validpoints_number=qp.validpoints_number;
  }

  Backtrackable*  ValidPoint::copy() const{
    return new ValidPoint(*this);
  }
}
