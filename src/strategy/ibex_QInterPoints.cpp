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
pair<Backtrackable*,Backtrackable*> QInterPoints::down() {
	return pair<Backtrackable*,Backtrackable*> (new QInterPoints(*this),new QInterPoints(*this));
}
  QInterPoints::QInterPoints() {};

  QInterPoints::~QInterPoints(){delete points;}

  QInterPoints::QInterPoints (const QInterPoints& qp) {
    list<int>::const_iterator iter = qp.points->begin() ;
    points= new list<int> ();
    while (iter != qp.points->end())
      {points->push_back(*iter);
      iter++;
      }
    qmax=qp.qmax;
    depth=qp.depth+1;
  }
}
