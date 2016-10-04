/* ============================================================================
 * I B E X - Continuation Tests
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes and CNRS
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert, Alexandre Goldsztejn
 * Created     : Sep 06, 2016
 * ---------------------------------------------------------------------------- */

#include "TestCont.h"
#include "ibex_Cont.h"

using namespace std;

namespace ibex {

void TestCont::test01() {
	Variable x,y,z;
	Function f(x,y,z,sqr(x)+sqr(y)+sqr(z)-1);
	//Function g("x","y","z","-1");

	double __sol[][2]={{0,0},{0,0},{1,1}};
	IntervalVector sol(3,__sol);

	Cont cont(f, 1e-3, 0.5, 1.1);

	//cout << "Try to build a cell of parameter width~2.0 around the solution x*=" << sol << endl;

	ContCell cell=cont.choose(sol,2.0);

	//cout << "\nResult:\n" << cell << endl;
	CPPUNIT_ASSERT(cell.box.min_diam()>0.9);
}

} // end namespace
