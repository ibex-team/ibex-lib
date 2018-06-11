//============================================================================
//                                  I B E X                                   
// File        : ibex_QInterPoints.cpp
// Author      : Bertrand Neveu
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Apr 15, 2013
// Last Update : Apr 15, 2013
//============================================================================


#include "ibex_QInterPoints.h"
#include <stdlib.h>

using namespace std;

namespace ibex {
pair<Backtrackable*,Backtrackable*> QInterPoints::down(const BisectionPoint& b) {
	return pair<Backtrackable*,Backtrackable*> (new QInterPoints(*this),new QInterPoints(*this));
}
  QInterPoints::QInterPoints() {;};

  QInterPoints::~QInterPoints(){
  }

  QInterPoints::QInterPoints (const QInterPoints& qp) {
    qmax=qp.qmax;
    depth=qp.depth+1;
  }

  Backtrackable* QInterPoints::copy() const{
    return new QInterPoints(*this);
  }
}
