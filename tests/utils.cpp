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
#include <math.h>
#include <sstream>

bool TestIbex::sameExpr(const ExprNode& node, const char* expr) {
	std::stringstream s;
	s << node;
	return strcmp(s.str().c_str(),expr)==0;
}

bool TestIbex::sameExpr(const ExprNode& node, const ExprNode& node2) {
	std::stringstream s,s2;
	s << node;
	s2 << node2;
	return strcmp(s.str().c_str(),s2.str().c_str())==0;
}

void TestIbex::check(double y_actual, double y_expected) {
	TEST_ASSERT(!isnan(y_expected));
	if (y_expected==POS_INFINITY) { TEST_ASSERT(y_actual==POS_INFINITY); }
	else if (y_expected==NEG_INFINITY) { TEST_ASSERT(y_actual==NEG_INFINITY); }
	else {
		TEST_ASSERT(y_actual!=POS_INFINITY);
		TEST_ASSERT(y_actual!=NEG_INFINITY);
		TEST_ASSERT_DELTA(y_actual,y_expected,ERROR);
	}
}

void TestIbex::check(const Interval& y_actual, const Interval& y_expected) {
	//cout << "TestIbex::check:    " << y_expected << " (expected)        " << y_actual << " (actual)"<< endl;
	if (y_expected.is_empty()) { TEST_ASSERT(y_actual.is_empty()); return; }

	TEST_ASSERT(!y_actual.is_empty());
	TEST_ASSERT(!isnan(y_actual.lb()));
	TEST_ASSERT(!isnan(y_actual.ub()));
	TEST_ASSERT_DELTA(y_actual.lb(),y_expected.lb(),ERROR);
	TEST_ASSERT_DELTA(y_actual.ub(),y_expected.ub(),ERROR);
}

void TestIbex::check(const IntervalVector& y_actual, const IntervalVector& y_expected) {
	TEST_ASSERT(y_actual.size()==y_expected.size());
	if (y_actual.is_empty() && y_expected.is_empty()) { TEST_ASSERT(true); return; }
	for (int i=0; i<y_actual.size(); i++)
		TestIbex::check(y_actual[i],y_expected[i]);
}

bool TestIbex::almost_eq(const Interval& y_actual, const Interval& y_expected, double err) {
	if (y_actual.is_empty() && y_expected.is_empty()) return true;

	if (y_actual.lb()==NEG_INFINITY)
		if (y_expected.lb()!=NEG_INFINITY) return false;
		else;
	else if (fabs(y_actual.lb()-y_expected.lb())> err) return false;

	if (y_actual.ub()==POS_INFINITY)
		if (y_expected.ub()!=POS_INFINITY) return false;
		else;
	else if (fabs(y_actual.ub()-y_expected.ub())> err) return false;

	return true;
}

bool TestIbex::almost_eq(const IntervalVector& y_actual, const IntervalVector& y_expected, double err) {
	if (y_actual.size()!=y_actual.size()) return false;
	if (y_actual.is_empty() && y_expected.is_empty()) return true;

	for (int i=0; i<y_actual.size(); i++) {
		if (!almost_eq(y_actual[i], y_expected[i],err)) return false;
	}

	return true;
}


