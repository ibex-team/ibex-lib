//============================================================================
//                                  I B E X                                   
// File        : swim01.cpp
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Jun 4, 2013
// Last Update : Jun 4, 2013
//============================================================================


#include "ibex.h"
#include "data.h"
using namespace std;

using namespace ibex;

int main() {

  init_data();

  // ![dist-decl]
  // create the distance function beforehand
  Variable a(2);       // "local" variable
  Variable b(2);
  Function dist(a,b,sqrt(sqr(a[0]-b[0])+sqr(a[1]-b[1])));
  // ![dist-decl]
  // the initial box [0,L]x[0,L]x[0,L]x[0,L]
  IntervalVector box(T*2,Interval(0,L));
  Variable x(T,2);

  vector<Ctc*> ctc;
  for (int t=0; t<T; t++) {
    // ![qinter]
    // create a temporary subvector
    // for collecting all the contractors corresponding
    // to the detections at time t
    vector<Ctc*> cdist;
    for (int b=0; b<N; b++) {
      NumConstraint* c=new NumConstraint(
          x,dist(x[t],beacons[b])=d[t][b]);
      // push the detection of beacon n°b
      cdist.push_back(new CtcFwdBwd(*c));
    }
    // Push the q-intersection of all
    // the contractors in "cdist" in the main
    // vector "ctc"
    ctc.push_back(new CtcQInter(cdist,N-NB_OUTLIERS));
    // ![qinter]

    if (t<T-1) {
      NumConstraint* c=new NumConstraint(x,x[t+1]-x[t]=v[t]);
      ctc.push_back(new CtcFwdBwd(*c));
    }
  }
  // Fixpoint
  CtcCompo compo(ctc);

  // FixPoint
  CtcFixPoint fix(compo);

  cout << endl << "initial box =" << box << endl;
  fix.contract(box);
  cout << endl << "final box =" << box << endl << endl;

  return 0;
}
