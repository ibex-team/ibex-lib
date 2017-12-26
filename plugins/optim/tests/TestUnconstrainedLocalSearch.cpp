//============================================================================
//                                  I B E X                                   
// File        : TestUnconstrainedLocalSearch.cpp
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Mar 21, 2014
// Last Update : Mar 21, 2014
//============================================================================

#include "TestUnconstrainedLocalSearch.h"
#include "ibex_UnconstrainedLocalSearch.h"

using namespace std;

namespace ibex {


using namespace ibex;


void TestUnconstrainedLocalSearch::simple01() {
	Variable x;
	int n=1;
	Function f(x,pow(x,2)+3*x+2);

	Vector x0(n); // 0,0,...
	IntervalVector box(n,Interval(-10,10));
	UnconstrainedLocalSearch o(f,box);
	double eps=1e-6;
	int max_iter=10;
	Vector xk(n);
	UnconstrainedLocalSearch::ReturnCode ret=o.minimize(x0,xk,eps,max_iter);
	CPPUNIT_ASSERT(ret==UnconstrainedLocalSearch::SUCCESS);
	CPPUNIT_ASSERT(almost_eq(xk[0],Interval(-1.5),1.e-5));
}

void TestUnconstrainedLocalSearch::almost_diag() {
	int n=5;
	Matrix Q(n,n);
	for (int i=0; i<n; i++)
		for (int j=0; j<n; j++)
			Q[i][j]=(i==j)?1:-1.0/(n+1);

	double _xsol[5]={1,2,3,4,5};
	Vector x0(n); // 0,0,...
	Vector xsol(n,_xsol);
	Variable x(n);
	Function f(x,transpose(x-xsol)*(Q*(x-xsol)));

	IntervalVector box(n,Interval(-10,10));
	UnconstrainedLocalSearch o(f,box);
	double eps=1e-10;
	int max_iter=1000;
	Vector xk(n);
	UnconstrainedLocalSearch::ReturnCode ret=o.minimize(x0,xk,eps,max_iter);

	CPPUNIT_ASSERT(ret==UnconstrainedLocalSearch::SUCCESS);
	CPPUNIT_ASSERT(almost_eq(IntervalVector(xk),IntervalVector(xsol), 1.e-9));

}

} // end namespace ibex
