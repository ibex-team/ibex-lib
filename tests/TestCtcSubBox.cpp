/* ============================================================================
 * I B E X - Dim Tests
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Mar 2, 2012
 * ---------------------------------------------------------------------------- */

#include "TestCtcSubBox.h"
#include "ibex_CtcSubBox.h"
#include "ibex_CtcProj.h"

using namespace std;

namespace ibex {

void TestCtcSubBox::test01() {
	BoolMask mask(4);
	mask[0]=false;
	mask[1]=true;
	mask[2]=false;
	mask[3]=true;

	Variable x,y;
	Function f(x,y,x-y);
	CtcProj c(f);

	CtcSubBox c2(c,mask);
	double _box[][2] = {{0,1},{1,3},{0,1},{2,4}};

	IntervalVector box(4,_box);

	double _resbox[][2] = {{0,1},{2,3},{0,1},{2,3}};
	IntervalVector resbox(4,_resbox);

	c2.contract(box);

	check(box,resbox);
}

void TestCtcSubBox::test02() {
	BoolMask mask(4);
	mask[0]=false;
	mask[1]=true;
	mask[2]=false;
	mask[3]=true;

	Variable x,y;
	Function f(x,y,x-y);
	CtcProj c(f);

	CtcSubBox c2(c,mask);

	double _box[][2] = {{0,1},{1,3},{0,1},{4,5}};

	IntervalVector box(4,_box);

	TEST_THROWS(c2.contract(box), EmptyBoxException&);
}

} // end namespace
