/* ============================================================================
 * I B E X - utils.cpp
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Mar 7, 2012
 * ---------------------------------------------------------------------------- */

#include "utils.h"

void TestIbex::check(double y_actual, double y_expected) {
	TEST_ASSERT_DELTA(y_actual,y_expected,ERROR);
}

void TestIbex::check(const Interval& y_actual, const Interval& y_expected) {
	//cout << "TestIbex::check:    " << y_expected << " (expected)        " << y_actual << " (actual)"<< endl;
	TEST_ASSERT_DELTA(y_actual.lb(),y_expected.lb(),ERROR);
	TEST_ASSERT_DELTA(y_actual.ub(),y_expected.ub(),ERROR);
}

void TestIbex::check(const IntervalVector& y_actual, const IntervalVector& y_expected) {
	TEST_ASSERT(y_actual.size()==y_actual.size());
	if (y_actual.is_empty() && y_expected.is_empty()) { TEST_ASSERT(true); return; }
	for (int i=0; i<y_actual.size(); i++)
		TestIbex::check(y_actual[i],y_expected[i]);
}

bool TestIbex::almost_eq(const IntervalVector& y_actual, const IntervalVector& y_expected, double err) {
	if (y_actual.size()!=y_actual.size()) return false;
	if (y_actual.is_empty() && y_expected.is_empty()) return true;

	for (int i=0; i<y_actual.size(); i++) {
		if (y_actual[i].lb()==NEG_INFINITY)
			if (y_expected[i].lb()!=NEG_INFINITY) return false;
			else;
		else if (fabs(y_actual[i].lb()-y_expected[i].lb())> err) return false;

		if (y_actual[i].ub()==POS_INFINITY)
			if (y_expected[i].ub()!=POS_INFINITY) return false;
			else;
		else if (fabs(y_actual[i].ub()-y_expected[i].ub())> err) return false;
	}
	return true;
}


