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
#include <cppunit/TestAssert.h>
#include <math.h>
#include <sstream>

using namespace std;

double ERROR = 1e-10;

bool sameExpr(const ExprNode& node, const char* expr) {
	std::stringstream s;
	s << node;
	if (strcmp(s.str().c_str(),expr)==0)
		return true;
	else  {
	/*	std::cout<< "Expr tested : " << std::endl;
		std::cout<< s.str().c_str() << std::endl;
		std::cout<< "Expr wanted : " << std::endl;
		std::cout<< expr << std::endl;
	*/	return false;
	}
}

bool sameExpr(const ExprNode& node, const ExprNode& node2) {
	std::stringstream s,s2;
	s << node;
	s2 << node2;
	if (strcmp(s.str().c_str(),s2.str().c_str())==0)
		return true;
	else  {
	/*	std::cout<< "Expr tested : " << std::endl;
		std::cout<< s.str().c_str() << std::endl;
		std::cout<< "Expr wanted : " << std::endl;
		std::cout<< s2.str().c_str() << std::endl;
	*/	return false;
	}
}

void check(double y_actual, double y_expected, double err) {
	CPPUNIT_ASSERT(!std::isnan(y_expected));
	if (y_expected==POS_INFINITY) { CPPUNIT_ASSERT(y_actual==POS_INFINITY); }
	else if (y_expected==NEG_INFINITY) { CPPUNIT_ASSERT(y_actual==NEG_INFINITY); }
	else {
		CPPUNIT_ASSERT(y_actual!=POS_INFINITY);
		CPPUNIT_ASSERT(y_actual!=NEG_INFINITY);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(y_expected,y_actual,err);
	}
}

void check(const Interval& y_actual, const Interval& y_expected, double err) {
	//std::cout << "check:    " << y_expected << " (expected)        " << y_actual << " (actual)"<< std::endl;
	if (y_expected.is_empty()) { CPPUNIT_ASSERT(y_actual.is_empty()); return; }

	CPPUNIT_ASSERT(!y_actual.is_empty());
	CPPUNIT_ASSERT(!std::isnan(y_actual.lb()));
	CPPUNIT_ASSERT(!std::isnan(y_actual.ub()));
	CPPUNIT_ASSERT_DOUBLES_EQUAL(y_expected.lb(),y_actual.lb(),err);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(y_expected.ub(),y_actual.ub(),err);
}

void check(const IntervalVector& y_actual, const IntervalVector& y_expected, double err) {
	CPPUNIT_ASSERT(y_actual.size()==y_expected.size());
	if (y_actual.is_empty() && y_expected.is_empty()) { CPPUNIT_ASSERT(true); return; }
	for (int i=0; i<y_actual.size(); i++)
		check(y_actual[i],y_expected[i], err);
}



void check(const IntervalMatrix& y_actual, const IntervalMatrix& y_expected, double err) {
	CPPUNIT_ASSERT(y_actual.nb_rows()!=y_actual.nb_rows());
	CPPUNIT_ASSERT(y_actual.nb_cols()!=y_actual.nb_cols());
	if (y_actual.is_empty() && y_expected.is_empty()) { CPPUNIT_ASSERT(true); return; }
	for (int i=0; i<y_actual.nb_rows(); i++) {
		check(y_actual.row(i), y_expected.row(i),err);
	}
}


void check(const Vector& y_actual, const Vector& y_expected, double err) {
	CPPUNIT_ASSERT(y_actual.size()==y_expected.size());
	for (int i=0; i<y_actual.size(); i++)
		check(y_actual[i],y_expected[i],err);
}

void check(const Matrix& y_actual, const Matrix& y_expected, double err) {
	CPPUNIT_ASSERT(y_actual.nb_rows()!=y_actual.nb_rows());
	CPPUNIT_ASSERT(y_actual.nb_cols()!=y_actual.nb_cols());
	for (int i=0; i<y_actual.nb_rows(); i++) {
		check(y_actual.row(i), y_expected.row(i),err);
	}
}

bool almost_eq(const Interval& y_actual, const Interval& y_expected, double err) {
	if (y_actual.is_empty() && y_expected.is_empty()) return true;
	if (y_actual.is_empty() || y_expected.is_empty()) return false;

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

bool almost_eq(const IntervalVector& y_actual, const IntervalVector& y_expected, double err) {
	if (y_actual.size()!=y_actual.size()) return false;
	if (y_actual.is_empty() && y_expected.is_empty()) return true;
	if (y_actual.is_empty() || y_expected.is_empty()) return false;

	for (int i=0; i<y_actual.size(); i++) {
		if (!almost_eq(y_actual[i], y_expected[i],err)) return false;
	}

	return true;
}

bool almost_eq(const IntervalMatrix& y_actual, const IntervalMatrix& y_expected, double err) {
	if (y_actual.nb_rows()!=y_actual.nb_rows()) return false;
	if (y_actual.nb_cols()!=y_actual.nb_cols()) return false;
	if (y_actual.is_empty() && y_expected.is_empty()) return true;
	if (y_actual.is_empty() || y_expected.is_empty()) return false;

	for (int i=0; i<y_actual.nb_rows(); i++) {
		if (!almost_eq(y_actual.row(i), y_expected.row(i),err)) return false;
	}

	return true;
}



void check_relatif(double y_actual, double y_expected, double err) {
	CPPUNIT_ASSERT(!std::isnan(y_expected));
	if (y_expected==POS_INFINITY) { CPPUNIT_ASSERT(y_actual==POS_INFINITY); }
	else if (y_expected==NEG_INFINITY) { CPPUNIT_ASSERT(y_actual==NEG_INFINITY); }
	else {
		CPPUNIT_ASSERT(y_actual!=POS_INFINITY);
		CPPUNIT_ASSERT(y_actual!=NEG_INFINITY);
		if (fabs(y_expected)<1)
			CPPUNIT_ASSERT_DOUBLES_EQUAL(y_expected,y_actual,err);
		else
			CPPUNIT_ASSERT_DOUBLES_EQUAL(y_expected,y_actual,err*fabs(y_expected));
	}
}

void check_relatif(const Interval& y_actual, const Interval& y_expected, double err) {
	//std::cout << "check:    " << y_expected << " (expected)        " << y_actual << " (actual)"<< std::endl;
	if (y_expected.is_empty()) { CPPUNIT_ASSERT(y_actual.is_empty()); return; }

	CPPUNIT_ASSERT(!y_actual.is_empty());
	CPPUNIT_ASSERT(!std::isnan(y_actual.lb()));
	CPPUNIT_ASSERT(!std::isnan(y_actual.ub()));
	if (fabs(y_expected.lb())<1)
		CPPUNIT_ASSERT_DOUBLES_EQUAL(y_expected.lb(),y_actual.lb(),err);
	else
		CPPUNIT_ASSERT_DOUBLES_EQUAL(y_expected.lb(),y_actual.lb(),err*fabs(y_expected.lb()));
	if (fabs(y_expected.ub())<1)
		CPPUNIT_ASSERT_DOUBLES_EQUAL(y_expected.ub(),y_actual.ub(),err);
	else
		CPPUNIT_ASSERT_DOUBLES_EQUAL(y_expected.ub(),y_actual.ub(),err*fabs(y_expected.ub()));
}

void check_relatif(const IntervalVector& y_actual, const IntervalVector& y_expected, double err) {
	CPPUNIT_ASSERT(y_actual.size()==y_expected.size());
	if (y_actual.is_empty() && y_expected.is_empty()) { CPPUNIT_ASSERT(true); return; }
	for (int i=0; i<y_actual.size(); i++)
		check_relatif(y_actual[i],y_expected[i], err);
}



void check_relatif(const IntervalMatrix& y_actual, const IntervalMatrix& y_expected, double err) {
	CPPUNIT_ASSERT(y_actual.nb_rows()!=y_actual.nb_rows());
	CPPUNIT_ASSERT(y_actual.nb_cols()!=y_actual.nb_cols());
	if (y_actual.is_empty() && y_expected.is_empty()) { CPPUNIT_ASSERT(true); return; }
	for (int i=0; i<y_actual.nb_rows(); i++) {
		check_relatif(y_actual.row(i), y_expected.row(i),err);
	}
}


void check_relatif(const Vector& y_actual, const Vector& y_expected, double err) {
	CPPUNIT_ASSERT(y_actual.size()==y_expected.size());
	for (int i=0; i<y_actual.size(); i++)
		check_relatif(y_actual[i],y_expected[i],err);
}

void check_relatif(const Matrix& y_actual, const Matrix& y_expected, double err) {
	CPPUNIT_ASSERT(y_actual.nb_rows()!=y_actual.nb_rows());
	CPPUNIT_ASSERT(y_actual.nb_cols()!=y_actual.nb_cols());
	for (int i=0; i<y_actual.nb_rows(); i++) {
		check_relatif(y_actual.row(i), y_expected.row(i),err);
	}
}
