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
  Variable x(T,2);  // create a Tx2 variable

  // ![ctc-vec]
  vector<Ctc*> ctc;
  for (int t=0; t<T; t++) {
    vector<Ctc*> cdist;
    for (int b=0; b<N; b++) {
      // Push the contractor corresponding to 
      // the detection of beacon n°b at time t
      NumConstraint* c=new NumConstraint(
               x,dist(x[t],beacons[b])=d[t][b]);
      ctc.push_back(new CtcFwdBwd(*c));
    }

    if (t<T-1) {
      // Push the contractor corresponding to
      // the speed measurement at time t
      NumConstraint* c=new NumConstraint(x,x[t+1]-x[t]=v[t]);
      ctc.push_back(new CtcFwdBwd(*c));
    }
  }
  // ![ctc-vec]

  // ![ctc-fix]
  // Composition
  CtcCompo compo(ctc);

  // FixPoint
  CtcFixPoint fix(compo);
  // ![ctc-fix]

  // ![ctc-contract]
  // the initial box [0,L]x[0,L]x[0,L]x[0,L]
  IntervalVector box(T*2,Interval(0,L));

  cout << "  initial box =" << box << endl << endl;
  fix.contract(box);
  cout << "  final box =" << box << endl << endl << endl;
  // ![ctc-contract]

  return 0;
}
