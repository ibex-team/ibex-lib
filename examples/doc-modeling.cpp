//============================================================================
//                                  I B E X                                   
// File        : modeling.cpp
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Jun 18, 2013
// Last Update : Jun 18, 2013
//============================================================================


#include "ibex.h"

using namespace std;
using namespace ibex;

/**
 * This file contains all the examples of the modeling guide.
 */

int main() {

  {
  //! [func-hansen]
  Variable x,y;
  Function f(x,y,Return(sqr(x)*y,sqr(y)*x));
  IntervalMatrix H(2,2);
  IntervalVector box(2,Interval(1,2));
  f.hansen_matrix(box,H);
  cout << "Hansen matrix:\n" << H << endl;
  //! [func-hansen]
  }

  {
  //! [func-cpp1]
  Variable x("x");
  Variable y("y");
  Function f(x,y,sin(x+y)); // create the function (x,y)->sin(x+y)
  cout << f << endl;
  //! [func-cpp1]
  }

  {
  //! [func-cpp2]
  Variable a,b,c,d,e,f;
  Function _f(a,b,c,d,e,f,a+b+c+d+e+f);
  //! [func-cpp2]
  }

  {
  //! [func-cpp3]
  Variable x[7]; // not to be confused with x(7)
  Array<const ExprSymbol> vars(7);

  for (int i=0; i<7; i++)
    vars.set_ref(i,x[i]);

  Function f(vars, x[0]+x[1]+x[2]+x[3]+x[4]+x[5]+x[6]);
  cout << f << endl;
  //! [func-cpp3]
  }

  {
  //! [func-cpp-symbols]
  Variable xa,xb,ya,yb;
  Function dist(xa,xb,ya,yb, sqrt(sqr(xa-xb)+sqr(ya-yb)));
  //! [func-cpp-symbols]
  }

  {
  //! [func-cpp-vec-args]
  Variable a(2);
  Variable b(2);
  Function dist(a,b,sqrt(sqr(a[0]-b[0])+sqr(a[1]-b[1])),"dist");
  //! [func-cpp-vec-args]
  }

  {
  //! [func-cpp-compo]

  //! [func-cpp-compo]
  }


  {
  // ![func-dag]
  Variable x;
  const ExprNode& e=cos(x)+1;
  Function f(x,Return(pow(e,2),pow(e,3)));
  // ![func-dag]
  }

  {
  // ![func-iterated-sum]
  int N=10;
  Variable x(N,"x");

  const ExprNode* e=&(sqr(x[0]));
  for (int i=1; i<N; i++)
	  e = & (*e + sqr(x[i]));

  Function f(x,*e,"f");

  cout << f << endl;
  // ![func-iterated-sum]
  }

}
