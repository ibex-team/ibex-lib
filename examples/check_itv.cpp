/* ============================================================================
 * I B E X - Test of interval operations
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Dec 07, 2011
 * ---------------------------------------------------------------------------- */

#include <float.h>
#include <math.h>
#include <sstream>
#include <string.h>
#include <ibex.h>
#include <cmath>
#include <cfenv>
using namespace std;
using namespace ibex;

double ERROR = 1e-10;


void CPPUNIT_ASSERT(bool cond) {
	cout << "[assert] " << cond << endl;
}


void CPPUNIT_ASSERT_DOUBLES_EQUAL(double x, double y, double eps) {
	cout << "[assert] ";
	if (x==POS_INFINITY) cout << (y==POS_INFINITY) << endl;
	else if (x==NEG_INFINITY) cout << (y==NEG_INFINITY) << endl;
	else cout << (fabs(x-y) <= eps) << endl;
}

void CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE(const char* msg, double x, double y, double eps) {
	cout << "[assert] " << msg;
	if (x==POS_INFINITY) cout << (y==POS_INFINITY) << endl;
	else if (x==NEG_INFINITY) cout << (y==NEG_INFINITY) << endl;
	else cout << (fabs(x-y) <= eps) << endl;	
}

bool sameExpr(const ExprNode& node, const char* expr);
bool sameExpr(const ExprNode& node, const ExprNode& node2);
void check(double y_actual, double y_expected, double err = ERROR);
void check(const Vector& y_actual, const Vector& y_expected, double err = ERROR);
void check(const Matrix& y_actual, const Matrix& y_expected, double err = ERROR);
void check(const Interval& y_actual, const Interval& y_expected, double err = ERROR);
void check(const IntervalVector& y_actual, const IntervalVector& y_expected, double err = ERROR);
void check(const IntervalMatrix& y_actual, const IntervalMatrix& y_expected, double err = ERROR);
bool almost_eq(const Interval& y_actual, const Interval& y_expected, double err = ERROR);
bool almost_eq(const IntervalVector& y_actual, const IntervalVector& y_expected, double err = ERROR);
bool almost_eq(const IntervalMatrix& y_actual, const IntervalMatrix& y_expected, double err = ERROR);

void check_relatif(double y_actual, double y_expected, double err = ERROR);
void check_relatif(const Vector& y_actual, const Vector& y_expected, double err = ERROR);
void check_relatif(const Matrix& y_actual, const Matrix& y_expected, double err = ERROR);
void check_relatif(const Interval& y_actual, const Interval& y_expected, double err = ERROR);
void check_relatif(const IntervalVector& y_actual, const IntervalVector& y_expected, double err = ERROR);
void check_relatif(const IntervalMatrix& y_actual, const IntervalMatrix& y_expected, double err = ERROR);

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


void cons01() {
	check(Interval(),Interval(NEG_INFINITY,POS_INFINITY));
	check((Interval())=Interval(),Interval(NEG_INFINITY,POS_INFINITY));
}
void cons02() {
	check(Interval(1.0),Interval(1.0,1.0));
	check((Interval())=1.0, Interval(1.0,1.0));
}
void cons03() {
	check(Interval(NEG_INFINITY),Interval::empty_set());
	check((Interval())=NEG_INFINITY, Interval::empty_set());
}
void cons04() {
	check(Interval(POS_INFINITY),Interval::empty_set());
	check((Interval())=POS_INFINITY, Interval::empty_set());
}
void cons05() {
	check(Interval(1,0), Interval::empty_set());
	check((Interval())=Interval(1,0), Interval::empty_set());
} // reverse bounds

void consCppArray() {
	check(Interval{1.0}, Interval(1.0, 1.0));
	check(Interval{1.0, 2.0}, Interval(1.0, 2.0));
}

void check_eq(const Interval& x, const Interval& y, bool is_eq) {
	CPPUNIT_ASSERT(is_eq? x==y : !(x==y));
	CPPUNIT_ASSERT(is_eq? !(x!=y) : x!=y);
	CPPUNIT_ASSERT(is_eq? y==x : !(y==x));
	CPPUNIT_ASSERT(is_eq? !(y!=x) : y!=x);
}

void eq01() { check_eq(Interval::all_reals(),Interval::all_reals(), true); }
void eq02() { check_eq(Interval::all_reals(),Interval::pos_reals(), false); }
void eq03() { check_eq(Interval(0,1), Interval(0,1), true); }
void eq04() { check_eq(Interval(0,2), Interval(0,1), false); }
void eq05() { check_eq(Interval(0,2), Interval(1,3), false); }
void eq06() { check_eq(Interval::pos_reals(),Interval::pos_reals(), true); }
void eq07() { check_eq(Interval::empty_set(),Interval::empty_set(), true); }
void eq08() { check_eq(Interval(0,0),Interval(0,0), true); }

void set_empty01() { Interval x(0,1); x.set_empty(); check(x,Interval::empty_set()); }
void set_empty02() { Interval x(Interval::empty_set()); x.set_empty(); check(x,Interval::empty_set()); }

void distance01() { check(ibex::distance(Interval(0,10), Interval(-5,5)), 5); }
void distance02() { check(ibex::distance(Interval(0,10), Interval(5,10)), 5); }
void distance03() { check(ibex::distance(Interval(0,10), Interval(5,15)), 5); }
void distance04() { check(ibex::distance(Interval(0,10), Interval(1,2)), 8); /* 9 */}
void distance05() { check(ibex::distance(Interval(0,10), Interval(0,10)), 0); }
void distance06() { check(ibex::distance(Interval(0,10), Interval(-10,20)), 10); /* 0 */}

/* with infinite bounds */
void distance07() { check(ibex::distance(Interval::pos_reals(), Interval(0,10)), POS_INFINITY); }
void distance08() { check(ibex::distance(Interval::all_reals(), Interval(0,10)), POS_INFINITY); }
void distance09() { check(ibex::distance(Interval::all_reals(), Interval::all_reals()), 0); }
void distance10() { check(ibex::distance(Interval::all_reals(), Interval::neg_reals()), POS_INFINITY); }
void distance11() { check(ibex::distance(Interval::all_reals(), Interval::pos_reals()), POS_INFINITY); }
void distance12() { check(ibex::distance(Interval::pos_reals(), Interval(NEG_INFINITY,1)), POS_INFINITY); }
void distance13() { check(ibex::distance(Interval::pos_reals(), Interval(0,1)), POS_INFINITY); }
void distance14() { check(ibex::distance(Interval::neg_reals(), Interval(-1,POS_INFINITY)), POS_INFINITY); }
void distance15() { check(ibex::distance(Interval::neg_reals(), Interval(NEG_INFINITY,-1)), 1); }
void distance15b(){ check(ibex::distance(Interval::pos_reals(), Interval(1,POS_INFINITY)), 1); }

/* with degenerated intervals */
void distance16() { check(ibex::distance(Interval(0,10), Interval(5,5)), 5); /* 10 */}
void distance17() { check(ibex::distance(Interval(0,10), Interval(0,0)), 10); }
void distance18() { check(ibex::distance(Interval(0,10), Interval(10,10)), 10); }
void distance19() { check(ibex::distance(Interval(0,0), Interval(0,0)), 0); }

/* with empty intervals */

void distance20() { check(ibex::distance(Interval(0,10), Interval::empty_set()), 5); /* 10 */}
void distance21() { check(ibex::distance(Interval(0,0), Interval::empty_set()), 0); }

void distance22() { check(ibex::distance(Interval(0,10), Interval(11,20)), 11); }
void distance23() { check(ibex::distance(Interval(0,10), Interval(0,5)), 5); }

void rel_distance01() { check(Interval(0,10).rel_distance(Interval(-5,5)), 0.5); }
void rel_distance02() { check(Interval(0,10).rel_distance(Interval(5,10)), 0.5); }
void rel_distance03() { check(Interval(0,10).rel_distance(Interval(5,15)), 0.5); }
void rel_distance04() { check(Interval(0,10).rel_distance(Interval(1,2)), 0.8); }
void rel_distance05() { check(Interval(0,10).rel_distance(Interval(0,10)), 0); }
void rel_distance06() { check(Interval(0,10).rel_distance(Interval(-10,20)), 1);/*0*/ }

/* with infinite bounds */
void rel_distance07() { check(Interval::pos_reals().rel_distance(Interval(0,10)), 1); }
void rel_distance08() { check(Interval::all_reals().rel_distance(Interval(0,10)), 1); }
void rel_distance09() { check(Interval::all_reals().rel_distance(Interval::all_reals()), 0); }
void rel_distance10() { check(Interval::all_reals().rel_distance(Interval::neg_reals()), 1); }
void rel_distance11() { check(Interval::all_reals().rel_distance(Interval::pos_reals()), 1); }
void rel_distance12() { check(Interval::pos_reals().rel_distance(Interval(NEG_INFINITY,1)), 1); }
void rel_distance13() { check(Interval::pos_reals().rel_distance(Interval(0,1)), 1); }
void rel_distance14() { check(Interval::neg_reals().rel_distance(Interval(-1,POS_INFINITY)), 1); }
void rel_distance15() { check(Interval::neg_reals().rel_distance(Interval(NEG_INFINITY,-1)), 0); }

/* with degenerated intervals */
void rel_distance16() { check(Interval(0,10).rel_distance(Interval(5,5)), 0.5); /*1*/}
void rel_distance17() { check(Interval(0,10).rel_distance(Interval(0,0)), 1); }
void rel_distance18() { check(Interval(0,10).rel_distance(Interval(10,10)), 1); }
void rel_distance19() { check(Interval(0,0).rel_distance(Interval(0,0)), 0); }
void rel_distance20() { check(Interval(1,4).rel_distance(Interval(1.5,3)), 1.0/3.0); }

void rel_distance21() { check(Interval(0,10).rel_distance(Interval(0,5)), 0.5); }

void check_hull(const Interval& x, const Interval& z, const Interval& y_expected) {
	//std::cout << x<< "  " << z<< "  "<<  (x | z) << std::endl;
	check(x|z, y_expected);
	check(z|x, y_expected);
    check((Interval(x)|=z), y_expected);
    check((Interval(z)|=x), y_expected);
}

void hull01() { check_hull(Interval(0,1), Interval(2,3), Interval(0,3)); }
void hull02() { check_hull(Interval(0,1), Interval::empty_set(), Interval(0,1)); }
void hull03() { check_hull(Interval(-1), Interval::pos_reals(), Interval(-1,POS_INFINITY)); }

void check_inter(const Interval& x, const Interval& z, const Interval& y_expected) {
	check(x&z, y_expected);
	check(z&x, y_expected);
    check((Interval(x)&=z), y_expected);
    check((Interval(z)&=x), y_expected);
}

void inter01() { check_inter(Interval(0,1), Interval(2,3), Interval::empty_set()); }
void inter02() { check_inter(Interval(0,1), Interval::empty_set(), Interval::empty_set()); }
void inter03() { check_inter(Interval(0,2), Interval(-1,1), Interval(0,1)); }
void inter04() { check_inter(Interval(0,1), Interval::pos_reals(), Interval(0,1)); }
void inter05() { check_inter(Interval(0,1), Interval::neg_reals(), Interval::zero()); }
void inter06() { check_inter(Interval(0,1), Interval(1,2), Interval(1,1)); }

void getters01() {
	Interval x(0,2);
	CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("lb",0,x.lb(), ERROR);
	CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("ub",2,x.ub(), ERROR);
	CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("mid",1,x.mid(), ERROR);
	CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("rad",1,x.rad(), ERROR);
	CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("diam",2,x.diam(), ERROR);
	CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("mig",0,x.mig(), ERROR);
	CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("mag",2,x.mag(), ERROR);
}

void getters02() {
	Interval x(-3,-1);
	CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("lb",-3,x.lb(), ERROR);
	CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("ub",-1,x.ub(), ERROR);
	CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("mid",-2,x.mid(), ERROR);
	CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("rad",1,x.rad(), ERROR);
	CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("diam",2,x.diam(), ERROR);
	CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("mig",1,x.mig(), ERROR);
	CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("mag",3,x.mag(), ERROR);
}

void getters03() {
	Interval x(-3,1);
	CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("lb",-3,x.lb(), ERROR);
	CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("ub",1,x.ub(), ERROR);
	CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("mid",-1,x.mid(), ERROR);
	CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("rad",2,x.rad(), ERROR);
	CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("diam",4,x.diam(), ERROR);
	CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("mig",0,x.mig(), ERROR);
	CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("mag",3,x.mag(), ERROR);
}

void getters04() {
	Interval x(Interval::neg_reals());
	CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("lb",NEG_INFINITY,x.lb(), ERROR);
	CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("ub",0,x.ub(), ERROR);
	CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("mid",-DBL_MAX,x.mid(), ERROR);
	CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("rad",POS_INFINITY,x.rad(), ERROR);
	CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("diam",POS_INFINITY,x.diam(), ERROR);
	CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("mig",0,x.mig(), ERROR);
	CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("mag",POS_INFINITY,x.mag(), ERROR);
}

void getters05() {
	Interval x(Interval::all_reals());
	CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("lb",NEG_INFINITY,x.lb(), ERROR);
	CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("ub",POS_INFINITY,x.ub(), ERROR);
	CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("mid",0,x.mid(), ERROR);
	CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("rad",POS_INFINITY,x.rad(), ERROR);
	CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("diam",POS_INFINITY,x.diam(), ERROR);
	CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("mig",0,x.mig(), ERROR);
	CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("mag",POS_INFINITY,x.mag(), ERROR);
}

void getters06() {
	Interval x(DBL_MAX,POS_INFINITY);
	CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("lb",DBL_MAX,x.lb(), ERROR);
	CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("ub",POS_INFINITY,x.ub(), ERROR);
	CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("mid",DBL_MAX,x.mid(), ERROR);
	CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("rad",POS_INFINITY,x.rad(), ERROR);
	CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("diam",POS_INFINITY,x.diam(), ERROR);
	CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("mig",DBL_MAX,x.mig(), ERROR);
	CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("mag",POS_INFINITY,x.mag(), ERROR);
}


void is_subset01()          { CPPUNIT_ASSERT(Interval(0,1).is_subset(Interval(0,2))); }
void is_subset02()          { CPPUNIT_ASSERT(!Interval(0,1).is_subset(Interval(1,2))); }
void is_strict_interior_subset01()   { CPPUNIT_ASSERT(!Interval(0,1).is_strict_interior_subset(Interval(0,2))); }
void is_strict_interior_subset02()   { CPPUNIT_ASSERT(Interval::neg_reals().is_strict_interior_subset(Interval::all_reals())); }
void is_strict_interior_subset03()   { CPPUNIT_ASSERT(Interval::empty_set().is_strict_interior_subset(Interval(0,1))); }
void is_superset01()        { CPPUNIT_ASSERT(Interval(0,2).is_superset(Interval(0,1))); }
void is_superset02()        { CPPUNIT_ASSERT(!Interval(1,2).is_superset(Interval(0,1))); }
void is_strict_superset01() { CPPUNIT_ASSERT(!Interval(0,2).is_strict_superset(Interval(0,2))); }
void is_strict_superset02() { CPPUNIT_ASSERT(Interval::all_reals().is_strict_superset(Interval::neg_reals())); }
void is_strict_superset03() { CPPUNIT_ASSERT(!Interval::empty_set().is_strict_superset(Interval::empty_set())); }
void contains01()           { CPPUNIT_ASSERT(Interval(0,2).contains(0)); }
void contains02()           { CPPUNIT_ASSERT(Interval::all_reals().contains(POS_INFINITY)); }
void interior_contains01()  { CPPUNIT_ASSERT(!Interval(0,2).interior_contains(0)); }
void interior_contains02()  { CPPUNIT_ASSERT(Interval(0,2).interior_contains(1)); }
void is_disjoint01()        { CPPUNIT_ASSERT(!Interval::neg_reals().is_disjoint(Interval::pos_reals())); }
void is_disjoint02()        { CPPUNIT_ASSERT(Interval(0,1).is_disjoint(Interval(2,3))); }
void is_empty01()           { CPPUNIT_ASSERT(Interval(POS_INFINITY).is_empty()); }
void is_empty02()           { CPPUNIT_ASSERT(!Interval(0,1).is_empty()); }
void is_degenerated01()     { CPPUNIT_ASSERT(Interval::zero().is_degenerated()); }
void is_degenerated02()     { CPPUNIT_ASSERT(Interval::empty_set().is_degenerated()); }
void is_unbounded01()       { CPPUNIT_ASSERT(!Interval::empty_set().is_unbounded()); }
void is_unbounded02()       { CPPUNIT_ASSERT(!Interval(0,1).is_unbounded()); }
void is_unbounded03()       { CPPUNIT_ASSERT(Interval::neg_reals().is_unbounded()); }
void is_unbounded04()       { CPPUNIT_ASSERT(Interval::pos_reals().is_unbounded()); }
void is_bisectable01()      { CPPUNIT_ASSERT(!Interval::empty_set().is_bisectable()); }
void is_bisectable02()      { CPPUNIT_ASSERT(!Interval(DBL_MAX,POS_INFINITY).is_bisectable()); }
void is_bisectable03()      { CPPUNIT_ASSERT(!Interval(NEG_INFINITY,-DBL_MAX).is_bisectable()); }
void is_bisectable04()      { CPPUNIT_ASSERT(!Interval(0,ibex::next_float(0)).is_bisectable()); }
void is_bisectable05()      {	CPPUNIT_ASSERT(Interval(0,ibex::next_float(ibex::next_float(0))).is_bisectable());}
void is_bisectable06()      {CPPUNIT_ASSERT(Interval(10,ibex::next_float(ibex::next_float(10))).is_bisectable());}
void is_bisectable07()      {CPPUNIT_ASSERT(Interval(ibex::previous_float(ibex::previous_float(0)),0).is_bisectable());}
void is_bisectable08()      {CPPUNIT_ASSERT(Interval(ibex::previous_float(ibex::previous_float(10)),10).is_bisectable());}
void is_bisectable09()      {CPPUNIT_ASSERT(!Interval(10,ibex::next_float(10)).is_bisectable()); }
void is_bisectable10()      {CPPUNIT_ASSERT(!Interval(ibex::previous_float(0),0).is_bisectable()); }

void mid01() {
	check(Interval(0,2).mid(),1.0);
}

void mid02() {
	CPPUNIT_ASSERT(Interval(NEG_INFINITY,POS_INFINITY).mid()==0);
}

void mid03() {
	CPPUNIT_ASSERT(Interval::pos_reals().mid()==DBL_MAX);
}

void mid04() {
	CPPUNIT_ASSERT(Interval::neg_reals().mid()==-DBL_MAX);
}

void mid05() {
	double d=Interval(DBL_MAX,POS_INFINITY).mid();
	CPPUNIT_ASSERT(d==DBL_MAX || d==-DBL_MAX);
}

void mid06() {
	CPPUNIT_ASSERT(Interval::zero().mid()==0.0);
}

void mid07() {
	Interval x(2.7629416257693772963,2.7629416257693835135);
	Interval y=x;
	//cout.precision(15);
	//cout << "x=" << y << endl;
	static double m=x.mid();
	//cout << "milieu = " << m << endl;
	Interval x2=x+m;
	//cout << "x+m=" << x2 << endl;
	Interval x3=x2-m;
	//cout << "(x+m)-m" <<x3 << endl;
	CPPUNIT_ASSERT(x3.is_superset(y));
}

void mid08() {
	double _box[][2]= {
			{3.4836061062102725572,3.4836061062102761099},
			{3.5754039600153961764,3.5754039600154006173},
			{3.2401050837195057852,3.2401050837195093379},
			{3.5187713541601914891,3.5187713541601937095},
			{3.4109008703180077049,3.410900870318013034},
			{3.336209585763303842,3.3362095857633069507},
			{3.3970878856415889224,3.3970878856415973601},
			{3.0865914745327103219,3.0865914745327134305},
			{3.1673998478272404888,3.1673998478272467061},
			{3.2795769799080982665,3.2795769799081013751},
			{2.9966040612287390132,2.996604061228742566},
			{3.4621387483049854694,3.4621387483049890221},
			{0.39999999999999980016,0.40000000000000074385},
			{1.9595917942265419498,1.959591794226542838},
			{2.4033959387712586775,2.4033959387712604538},
			{-1.5025469540784459621,-1.5025469540784437417},
			{3.4474903944979735648,3.4474903944979780057},
			{2.8417250066344248438,2.8417250066344270643},
			{3.6343497218046629804,3.6343497218046700858},
			{3.0078080936676636625,3.0078080936676689916},
			{3.8716115579677374825,3.8716115579677432557},
			{2.9290247128026156709,2.929024712802626329},
			{3.6847522306610471787,3.6847522306610511755},
			{2.7629416257693772963,2.7629416257693835135},
			{3.4978929033543559868,3.4978929033543604277},
			{2.5968585387361389216,2.5968585387361411421},
			{5,5},
			{-0,0},
			{-0,0},
			{-0,0} };

	IntervalVector box(30,_box);
	CPPUNIT_ASSERT((box+box.mid()-box.mid()).is_superset(box));
}

void delta01() {
	Interval x(-1.7976931348623157081e+308,6.1000000000000005329);
	//cout << "delta=" << x.delta(x) << " x.diam()=" << (x.diam()==POS_INFINITY) << endl;
	CPPUNIT_ASSERT(x.delta(x)==0.0);
}

void delta02() {
	double a=-3.6841361702068902e+17; // == DBL_MAX
	Interval x(a,POS_INFINITY);
	//cout << "delta=" << x.delta(x) << " x.diam()=" << (x.diam()==POS_INFINITY) << endl;
	Interval y(Interval::all_reals());
	Interval z(NEG_INFINITY,a);
	//cout << y.delta(x) << " " << z.diam() << endl;
	CPPUNIT_ASSERT(y.delta(x)==z.diam());
}

void delta03() {
	Interval x(-DBL_MAX,DBL_MAX); // so that diam()>DBL_MAX
	//cout << x.diam() << " " << x.diam() << endl;
	//cout << "delta=" << x.delta(x) << endl;
	CPPUNIT_ASSERT(x.delta(x)==0);
}

void diff01() {
	Interval c1,c2;
	Interval::zero().diff(Interval::zero(),c1,c2);
	CPPUNIT_ASSERT(c1.is_empty() && c2.is_empty());
 }


void diff02() {
	Interval c1,c2;
	Interval(0,0).diff(Interval(1,2),c1,c2);
	CPPUNIT_ASSERT(c1==Interval::zero() && c2.is_empty());
}

void diff03() {
	Interval c1,c2;
	Interval(0,0).diff(Interval(0,1),c1,c2);
	CPPUNIT_ASSERT(c1.is_empty() && c2.is_empty());
}

void diff04() {
	Interval c1,c2;
	Interval(0,0).diff(Interval::empty_set(),c1,c2);
	CPPUNIT_ASSERT(c1==Interval::zero() && c2.is_empty());
}

void diff05() {
	Interval c1,c2;
	Interval::empty_set().diff(Interval::zero(),c1,c2);
	CPPUNIT_ASSERT(c1.is_empty() && c2.is_empty());
}

void diff06() {
	Interval c1,c2;
	Interval(0,1).diff(Interval(0,1),c1,c2);
	CPPUNIT_ASSERT(c1.is_empty() && c2.is_empty());
}

void diff07() {
	Interval c1,c2;
	Interval(0,1).diff(Interval::empty_set(),c1,c2);
	CPPUNIT_ASSERT(c1==Interval(0,1) && c2.is_empty());
}

void diff08() {
	Interval c1,c2;
	Interval(0,1).diff(Interval(1,2),c1,c2);
	CPPUNIT_ASSERT(c1==Interval(0,1) && c2.is_empty());
}

void diff09() {
	Interval c1,c2;
	Interval(0,1).diff(Interval(-1,1),c1,c2);
	CPPUNIT_ASSERT(c1.is_empty() && c2.is_empty());
}

void diff10() {
	Interval c1,c2;
	Interval(0,1).diff(Interval(0,0),c1,c2);
	CPPUNIT_ASSERT(c1==Interval(0,1) && c2.is_empty());
}

void diff11() {
	Interval c1,c2;
	Interval(0,1).diff(Interval(1,1),c1,c2);
	CPPUNIT_ASSERT(c1==Interval(0,1) && c2.is_empty());
}

void diff12() {
	Interval c1,c2;
	Interval(0,1).diff(Interval(2,3),c1,c2);
	CPPUNIT_ASSERT(c1==Interval(0,1) && c2.is_empty());
}

void diff13() {
	Interval c1,c2;
	Interval(0,3).diff(Interval(1,2),c1,c2);
	CPPUNIT_ASSERT(c1==Interval(0,1) && c2==Interval(2,3));
}

void diff14() {
	Interval c1,c2;
	Interval(0,2).diff(Interval(1,4),c1,c2);
	CPPUNIT_ASSERT(c1==Interval(0,1) && c2.is_empty());
}

void diff15() {
	Interval c1,c2;
	Interval(0,2).diff(Interval(-1,1),c1,c2);
	CPPUNIT_ASSERT(c1==Interval(1,2) && c2.is_empty());
}

void diff16() {
	Interval c1,c2;
	Interval(0,2).diff(Interval(1,1),c1,c2,false);
	CPPUNIT_ASSERT(c1==Interval(0,1) && c2==Interval(1,2));
}

void diff17() {
	Interval c1,c2;
	Interval(1,1).diff(Interval(0,2),c1,c2,false);
	CPPUNIT_ASSERT(c1.is_empty() && c2.is_empty());
}

void issue_463() {
	Interval a = Interval(3,8);
	Interval b = Interval(1,3) & Interval(6,7); // [b] is empty
	Interval c = a+b;
	CPPUNIT_ASSERT(c.is_empty());
}

int main() {
    std::feclearexcept(FE_ALL_EXCEPT); // Clear all floating-point exceptions

    double x = 0.0;
    double y = std::nextafter(x, std::numeric_limits<double>::infinity());

    if (std::fetestexcept(FE_INVALID)) {
        std::cerr << "Invalid floating-point exception occurred!" << std::endl;
    } else {
        std::cout << "Next after 0 towards HUGE_VAL: " << y << std::endl;
    }

	double left=0;
	cout << "next float:" << endl;
	double right=ibex::next_float(0);
	
	if (std::fetestexcept(FE_INVALID)) {
        std::cerr << "Invalid floating-point exception occurred!" << std::endl;
    } else {
        std::cout << "Next after 0 towards HUGE_VAL: " << right << std::endl;
    }
	
	cout << "get fpu cw:" << endl;
    unsigned short int _save_state=get_fpu_cw(); 
    cout << "round nearest:" << endl;
    round_nearest();
    cout << "_save_state=" << _save_state << endl;
    double middle = 0.5*(left+right);
    cout << "middle=" << middle << endl;
	 if (std::isinf(middle)) {
		cout << "again" << endl;
		middle = 0.5*left + 0.5*right;
		cout << "middle=" << middle << endl;
	 }
	cout << "GOAL_RND_LEAV" << endl;
    GAOL_RND_LEAVE();
    cout << "OK!\n";
    if (left<middle && middle<right) 
    cout << "bisectable\n";
    else
    cout << "non bisectable\n";
     
    
/*	Interval x;
	Interval y(NEG_INFINITY,POS_INFINITY);
	
	cout << setprecision(52);
	cout << "x.ub()=" << x.ub() << endl;
	cout << "y.ub()=" << y.ub() << endl;
	cout << "POS_INFINITY=" << POS_INFINITY << endl;
	cout << "std::numeric_limits<double>::infinity()=" << std::numeric_limits<double>::infinity() << endl;
	cout << "GAOL_INFINITY=" << GAOL_INFINITY << endl;
	cout << "HUGE_VAL=" << HUGE_VAL << endl;
*/	
cout << "	cons01" << endl; 	cons01();
cout << "	cons02" << endl; 	cons02();
cout << "	cons03" << endl; 	cons03();
cout << "	cons04" << endl; 	cons04();
cout << "	cons05" << endl; 	cons05();
cout << "	consCppArray" << endl; 	consCppArray();

cout << "	eq01" << endl; 	eq01();
cout << "	eq02" << endl; 	eq02();
cout << "	eq03" << endl; 	eq03();
cout << "	eq04" << endl; 	eq04();
cout << "	eq05" << endl; 	eq05();
cout << "	eq06" << endl; 	eq06();
cout << "	eq07" << endl; 	eq07();
cout << "	eq08" << endl; 	eq08();

cout << "	set_empty01" << endl; 	set_empty01();
cout << "	set_empty02" << endl; 	set_empty02();

cout << "	inter01" << endl; 	inter01();
cout << "	inter02" << endl; 	inter02();
cout << "	inter03" << endl; 	inter03();
cout << "	inter04" << endl; 	inter04();
cout << "	inter05" << endl; 	inter05();
cout << "	inter06" << endl; 	inter06();

cout << "	hull01" << endl; 	hull01();
cout << "	hull02" << endl; 	hull02();
cout << "	hull03" << endl; 	hull03();

cout << "	getters01" << endl; 	getters01();
cout << "	getters02" << endl; 	getters02();
cout << "	getters03" << endl; 	getters03();
cout << "	getters04" << endl; 	getters04();
cout << "	getters05" << endl; 	getters05();
cout << "	getters06" << endl; 	getters06();

cout << "	distance01" << endl; 	distance01();
cout << "	distance02" << endl; 	distance02();
cout << "	distance03" << endl; 	distance03();
cout << "	distance04" << endl; 	distance04();
cout << "	distance05" << endl; 	distance05();
cout << "	distance06" << endl; 	distance06();
cout << "	distance07" << endl; 	distance07();
cout << "	distance08" << endl; 	distance08();
cout << "	distance09" << endl; 	distance09();
cout << "	distance10" << endl; 	distance10();
cout << "	distance11" << endl; 	distance11();
cout << "	distance12" << endl; 	distance12();
cout << "	distance13" << endl; 	distance13();
cout << "	distance14" << endl; 	distance14();
cout << "	distance15" << endl; 	distance15();
cout << "	distance15b" << endl; 	distance15b();
cout << "	distance16" << endl; 	distance16();
cout << "	distance17" << endl; 	distance17();
cout << "	distance18" << endl; 	distance18();
cout << "	distance19" << endl; 	distance19();
cout << "	distance20" << endl; 	distance20();
cout << "	distance21" << endl; 	distance21();
cout << "	distance22" << endl; 	distance22();
cout << "	distance23" << endl; 	distance23();

cout << "	rel_distance01" << endl; 	rel_distance01();
cout << "	rel_distance02" << endl; 	rel_distance02();
cout << "	rel_distance03" << endl; 	rel_distance03();
cout << "	rel_distance04" << endl; 	rel_distance04();
cout << "	rel_distance05" << endl; 	rel_distance05();
cout << "	rel_distance06" << endl; 	rel_distance06();
cout << "	rel_distance07" << endl; 	rel_distance07();
cout << "	rel_distance08" << endl; 	rel_distance08();
cout << "	rel_distance09" << endl; 	rel_distance09();
cout << "	rel_distance10" << endl; 	rel_distance10();
cout << "	rel_distance11" << endl; 	rel_distance11();
cout << "	rel_distance12" << endl; 	rel_distance12();
cout << "	rel_distance13" << endl; 	rel_distance13();
cout << "	rel_distance14" << endl; 	rel_distance14();
cout << "	rel_distance15" << endl; 	rel_distance15();
cout << "	rel_distance16" << endl; 	rel_distance16();
cout << "	rel_distance17" << endl; 	rel_distance17();
cout << "	rel_distance18" << endl; 	rel_distance18();
cout << "	rel_distance19" << endl; 	rel_distance19();
cout << "	rel_distance20" << endl; 	rel_distance20();
cout << "	rel_distance21" << endl; 	rel_distance21();

cout << "	is_subset01" << endl; 	is_subset01();
cout << "	is_subset02" << endl; 	is_subset02();
cout << "	is_strict_interior_subset01" << endl; 	is_strict_interior_subset01();
cout << "	is_strict_interior_subset02" << endl; 	is_strict_interior_subset02();
cout << "	is_strict_interior_subset03" << endl; 	is_strict_interior_subset03();
cout << "	is_superset01" << endl; 	is_superset01();
cout << "	is_superset02" << endl; 	is_superset02();
cout << "	is_strict_superset01" << endl; 	is_strict_superset01();
cout << "	is_strict_superset02" << endl; 	is_strict_superset02();
cout << "	is_strict_superset03" << endl; 	is_strict_superset03();
cout << "	contains01" << endl; 	contains01();
cout << "	contains02" << endl; 	contains02();
cout << "	interior_contains01" << endl; 	interior_contains01();
cout << "	interior_contains02" << endl; 	interior_contains02();
cout << "	is_disjoint01" << endl; 	is_disjoint01();
cout << "	is_disjoint02" << endl; 	is_disjoint02();
cout << "	is_empty01" << endl; 	is_empty01();
cout << "	is_empty02" << endl; 	is_empty02();
cout << "	is_degenerated01" << endl; 	is_degenerated01();
cout << "	is_degenerated02" << endl; 	is_degenerated02();
cout << "	is_unbounded01" << endl; 	is_unbounded01();
cout << "	is_unbounded02" << endl; 	is_unbounded02();
cout << "	is_unbounded03" << endl; 	is_unbounded03();
cout << "	is_unbounded04" << endl; 	is_unbounded04();

cout << "	is_bisectable01" << endl; 	is_bisectable01();
cout << "	is_bisectable02" << endl; 	is_bisectable02();
cout << "	is_bisectable03" << endl; 	is_bisectable03();
cout << "	is_bisectable04" << endl; 	is_bisectable04();
cout << "	is_bisectable05" << endl; 	is_bisectable05();
cout << "	is_bisectable06" << endl; 	is_bisectable06();
cout << "	is_bisectable07" << endl; 	is_bisectable07();
cout << "	is_bisectable08" << endl; 	is_bisectable08();
cout << "	is_bisectable09" << endl; 	is_bisectable09();
cout << "	is_bisectable10" << endl; 	is_bisectable10();

cout << "	mid01" << endl; 	mid01();
cout << "	mid02" << endl; 	mid02();
cout << "	mid03" << endl; 	mid03();
cout << "	mid04" << endl; 	mid04();
cout << "	mid05" << endl; 	mid05();
cout << "	mid06" << endl; 	mid06();
cout << "	mid07" << endl; 	mid07();
cout << "	mid08" << endl; 	mid08();

cout << "	delta01" << endl; 	delta01();
cout << "	delta02" << endl; 	delta02();
cout << "	delta03" << endl; 	delta03();

cout << "	diff01" << endl; 	diff01();
cout << "	diff02" << endl; 	diff02();
cout << "	diff03" << endl; 	diff03();
cout << "	diff04" << endl; 	diff04();
cout << "	diff05" << endl; 	diff05();
cout << "	diff06" << endl; 	diff06();
cout << "	diff07" << endl; 	diff07();
cout << "	diff08" << endl; 	diff08();
cout << "	diff09" << endl; 	diff09();
cout << "	diff10" << endl; 	diff10();
cout << "	diff11" << endl; 	diff11();
cout << "	diff12" << endl; 	diff12();
cout << "	diff13" << endl; 	diff13();
cout << "	diff14" << endl; 	diff14();
cout << "	diff15" << endl; 	diff15();
cout << "	diff16" << endl; 	diff16();
cout << "	diff17" << endl; 	diff17();
cout << "	issue_463" << endl; 	issue_463();
}

