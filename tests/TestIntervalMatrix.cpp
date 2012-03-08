/* ============================================================================
 * I B E X - Interval Matrix Tests
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Mar 2, 2012
 * ---------------------------------------------------------------------------- */

#include "TestIntervalMatrix.h"
#include "ibex_IntervalMatrix.h"
#include "utils.h"

using namespace std;

void TestIntervalMatrix::cons01() {
	IntervalMatrix m(2,3);
	TEST_ASSERT(m.nb_rows()==2);
	TEST_ASSERT(m.nb_cols()==3);
	TEST_ASSERT(m[0][0]==Interval::ALL_REALS);
	TEST_ASSERT(m[0][1]==Interval::ALL_REALS);
	TEST_ASSERT(m[0][2]==Interval::ALL_REALS);
	TEST_ASSERT(m[1][0]==Interval::ALL_REALS);
	TEST_ASSERT(m[1][1]==Interval::ALL_REALS);
	TEST_ASSERT(m[1][2]==Interval::ALL_REALS);
}


void TestIntervalMatrix::cons02() {
	IntervalMatrix m(2,3);
	double _r1[][2]={{0,1},{0,2},{0,3}};
	double _r2[][2]={{-1,0},{-2,0},{-3,0}};
	IntervalVector r1(3,_r1);
	IntervalVector r2(3,_r2);
	m[0]=r1;
	m[1]=r2;

	double _c1[][2]={{0,1},{-1,0}};
	double _c2[][2]={{0,2},{-2,0}};
	double _c3[][2]={{0,3},{-3,0}};
	IntervalVector c1(3,_c1);
	IntervalVector c2(3,_c2);
	IntervalVector c3(3,_c3);

	TEST_ASSERT(m.nb_rows()==2);
	TEST_ASSERT(m.nb_cols()==3);
	TEST_ASSERT(m[0]==r1);
	TEST_ASSERT(m[1]==r1);
	TEST_ASSERT(m.row(0)==r1);
	TEST_ASSERT(m.row(1)==r1);
	TEST_ASSERT(m.col(0)==c1);
	TEST_ASSERT(m.col(1)==c2);
	TEST_ASSERT(m.col(2)==c3);
	TEST_ASSERT(m[0][0]==Interval(0,1));
	TEST_ASSERT(m[0][1]==Interval(0,2));
	TEST_ASSERT(m[0][2]==Interval(0,3));
	TEST_ASSERT(m[1][0]==Interval(-1,0));
	TEST_ASSERT(m[1][1]==Interval(-2,0));
	TEST_ASSERT(m[1][2]==Interval(-3,0));

	IntervalMatrix m2(m);
	for (int i=0; i<2; i++) {
		for (int j=0; j<3; j++)
			TEST_ASSERT(m[i][j]==m2[i][j]);
	}

	IntervalMatrix m3(2,3);
	m3=m;
	for (int i=0; i<2; i++) {
		for (int j=0; j<3; j++)
			TEST_ASSERT(m[i][j]==m3[i][j]);
	}

}

void TestIntervalMatrix::cons03() {
	Interval x(-1,2);
	IntervalMatrix m(2,3,x);

	TEST_ASSERT(m.nb_rows()==2);
	TEST_ASSERT(m.nb_cols()==3);
	for (int i=0; i<2; i++) {
			for (int j=0; j<3; j++)
				TEST_ASSERT(m[i][j]==x);
	}

	IntervalMatrix m2(m);
	for (int i=0; i<2; i++) {
		for (int j=0; j<3; j++)
			TEST_ASSERT(m2[i][j]==x);
	}

	IntervalMatrix m3(2,3);
	m3=m;
	for (int i=0; i<2; i++) {
		for (int j=0; j<3; j++)
			TEST_ASSERT(m3[i][j]==x);
	}
}

