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

namespace {

IntervalMatrix M1() {
	IntervalMatrix m(2,3);
	double _r1[][2]={{0,1},{0,2},{0,3}};
	double _r2[][2]={{-1,0},{-2,0},{-3,0}};
	IntervalVector r1(3,_r1);
	IntervalVector r2(3,_r2);
	m[0]=r1;
	m[1]=r2;
	return m;
}


IntervalMatrix M2() { // the transpose of M1
	IntervalMatrix m(3,2);
	double _c1[][2]={{0,1},{-1,0}};
	double _c2[][2]={{0,2},{-2,0}};
	double _c3[][2]={{0,3},{-3,0}};
	IntervalVector c1(2,_c1);
	IntervalVector c2(2,_c2);
	IntervalVector c3(2,_c3);
	m[0]=c1;
	m[1]=c2;
	m[2]=c3;
	return m;
}

IntervalMatrix M3() { // non-null intersection with M1
	IntervalMatrix m(2,3);
	double _r1[][2]={{1,2},{1,2},{2,4}};
	double _r2[][2]={{-2,-1},{-2,-1},{-4,-2}};
	IntervalVector r1(3,_r1);
	IntervalVector r2(3,_r2);
	m[0]=r1;
	m[1]=r2;
	return m;
}

}

void TestIntervalMatrix::eq01() {
	IntervalMatrix m(2,3);
	IntervalMatrix m2(3,2);
	TEST_ASSERT(m!=m2);
	TEST_ASSERT(!(m==m2));
}

void TestIntervalMatrix::eq02() {
	IntervalMatrix m(3,2);
	IntervalMatrix m2(2,2);
	TEST_ASSERT(m!=m2);
	TEST_ASSERT(!(m==m2));
}

void TestIntervalMatrix::eq03() {
	IntervalMatrix m(2,3);
	IntervalMatrix m2(2,3);

	TEST_ASSERT(m.nb_rows()==2);
	TEST_ASSERT(m.nb_cols()==3);
	TEST_ASSERT(m2.nb_rows()==2);
	TEST_ASSERT(m2.nb_cols()==3);

	m[0][0]=1;
	m[0][1]=2;
	m[0][2]=3;
	m[1][0]=4;
	m[1][1]=5;
	m[1][2]=6;
	m2[0][0]=1;
	m2[0][1]=2;
	m2[0][2]=3;
	m2[1][0]=4;
	m2[1][1]=5;
	m2[1][2]=6;

	TEST_ASSERT(m==m2);
	TEST_ASSERT(!(m!=m2));

	m2[1][2]=7;
	TEST_ASSERT(m!=m2);
	TEST_ASSERT(!(m==m2));
}

void TestIntervalMatrix::eq04() {
	IntervalMatrix m(2,3);
	IntervalMatrix m2(2,3);
	m[1][1]=-1;
	m2[1][1]=-2;
	TEST_ASSERT(m!=m2);
	TEST_ASSERT(!(m==m2));
	m.set_empty();
	m2.set_empty();
	TEST_ASSERT(m==m2);
	TEST_ASSERT(!(m!=m2));
}

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

	TEST_ASSERT(m==IntervalMatrix(m));
	TEST_ASSERT(m==(IntervalMatrix(2,3)=m));
}

void TestIntervalMatrix::cons02() {
	IntervalMatrix m(2,3);
	double _r1[][2]={{0,1},{0,2},{0,3}};
	double _r2[][2]={{-1,0},{-2,0},{-3,0}};
	IntervalVector r1(3,_r1);
	IntervalVector r2(3,_r2);
	m[0]=r1;
	m.set_row(1,r2);

	double _c1[][2]={{0,1},{-1,0}};
	double _c2[][2]={{0,2},{-2,0}};
	double _c3[][2]={{0,3},{-3,0}};
	IntervalVector c1(2,_c1);
	IntervalVector c2(2,_c2);
	IntervalVector c3(2,_c3);

	TEST_ASSERT(m.nb_rows()==2);
	TEST_ASSERT(m.nb_cols()==3);
	TEST_ASSERT(m[0]==r1);
	TEST_ASSERT(m[1]==r2);
	TEST_ASSERT(m.row(0)==r1);
	TEST_ASSERT(m.row(1)==r2);

	TEST_ASSERT(m.col(0)==c1);
	TEST_ASSERT(m.col(1)==c2);
	TEST_ASSERT(m.col(2)==c3);
	TEST_ASSERT(m[0][0]==Interval(0,1));
	TEST_ASSERT(m[0][1]==Interval(0,2));
	TEST_ASSERT(m[0][2]==Interval(0,3));
	TEST_ASSERT(m[1][0]==Interval(-1,0));
	TEST_ASSERT(m[1][1]==Interval(-2,0));
	TEST_ASSERT(m[1][2]==Interval(-3,0));

	TEST_ASSERT(m==IntervalMatrix(m));
	TEST_ASSERT(m==(IntervalMatrix(2,3)=m));
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

	TEST_ASSERT(m==IntervalMatrix(m));
	TEST_ASSERT(m==(IntervalMatrix(2,3)=m));
}

void TestIntervalMatrix::cons04() {
	double _m[][2]={ {0,1}, {0,2}, {0,3},
		             {-1,0},{-2,0},{-3,0} };
	IntervalMatrix m(2,3,_m);
	TEST_ASSERT(m==M1());
}

void TestIntervalMatrix::empty01() {

	TEST_ASSERT(IntervalMatrix::empty(2,3).nb_rows()==2);
	TEST_ASSERT(IntervalMatrix::empty(2,3).nb_cols()==3);

	TEST_ASSERT(IntervalMatrix(IntervalMatrix::empty(2,3))==IntervalMatrix::empty(2,3));
	TEST_ASSERT((IntervalMatrix(2,3)=IntervalMatrix::empty(2,3))==IntervalMatrix::empty(2,3));
}

void TestIntervalMatrix::is_empty01() {
	TEST_ASSERT(!IntervalMatrix(2,3).is_empty());
}

void TestIntervalMatrix::is_empty02() {
	TEST_ASSERT(IntervalMatrix::empty(2,3).is_empty());
}

void TestIntervalMatrix::set_empty01() {
	IntervalMatrix m(2,3);
	m.set_empty();
	TEST_ASSERT(m.is_empty());
}

// intersection of a matrix with itself
void TestIntervalMatrix::inter01() {
	TEST_ASSERT((M1()&=M1())==M1());
}

// intersection of two overlapping matrices
void TestIntervalMatrix::inter02() {
	double _m[][2]={{1,1},  {1,2},  {2,3},
			        {-1,-1},{-2,-1},{-3,-2}};

	TEST_ASSERT((M1()&=M3())==IntervalMatrix(2,3,_m))
}
// intersection of two non-overlapping matrices
void TestIntervalMatrix::inter03() {
	IntervalMatrix m3(M3());
	m3[1][2]=Interval(-5,-4);
	TEST_ASSERT((M1()&=m3).is_empty());

}

void TestIntervalMatrix::set_col01() {
	IntervalMatrix m(M1());

	IntervalVector v(2);
	v[0]=Interval(1,2);
	v[1]=Interval(-2,-1);

	m.set_col(1,v);

	double _m2[][2]={ {0,1}, {1,2}, {0,3},
			          {-1,0},{-2,-1},{-3,0} };
	IntervalMatrix m2(2,3,_m2);

	TEST_ASSERT(m==m2);
}

void TestIntervalMatrix::rows01() {
	TEST_ASSERT(M1().rows(0,1)==M1());
}

void TestIntervalMatrix::rows02() {
	double _r0[][2]={ {0,1}, {0,2}, {0,3} };
	TEST_ASSERT(M1().rows(0,0)==IntervalMatrix(1,3,_r0));
}

void TestIntervalMatrix::rows03() {
	double _r1[][2]={ {-1,0},{-2,0},{-3,0} };
	TEST_ASSERT(M1().rows(1,1)==IntervalMatrix(1,3,_r1));
}

void TestIntervalMatrix::cols01() {
	TEST_ASSERT(M1().cols(0,2)==M1());
}

void TestIntervalMatrix::cols02() {
	double _c0[][2]={ {0,1}, {-1,0} };
	TEST_ASSERT(M1().cols(0,0)==IntervalMatrix(2,1,_c0));
}

void TestIntervalMatrix::cols03() {
	double _c1[][2]={ {0,2}, {-2,0} };
	TEST_ASSERT(M1().cols(1,1)==IntervalMatrix(2,1,_c1));
}

void TestIntervalMatrix::cols04() {
	double _c2[][2]={ {0,3}, {-3,0} };
	TEST_ASSERT(M1().cols(2,2)==IntervalMatrix(2,1,_c2));
}

void TestIntervalMatrix::cols05() {
	double _c12[][2]={ {0,2}, {0,3}, {-2,0}, {-3,0} };
	TEST_ASSERT(M1().cols(1,2)==IntervalMatrix(2,2,_c12));
}

void TestIntervalMatrix::resize01() {
	IntervalMatrix m(2,2);
	double _r1[][2]={{0,1},{0,2}};
	double _r2[][2]={{-1,0},{-2,0}};
	IntervalVector r1(2,_r1);
	IntervalVector r2(2,_r2);
	m[0]=r1;
	m[1]=r2;
	m.resize(2,3);
	m[0][2]=Interval(0,3);
	m[1][2]=Interval(-3,0);

	TEST_ASSERT(m==M1());
}

void TestIntervalMatrix::resize02() {
	IntervalMatrix m(1,3);
	double _r1[][2]={{0,1},{0,2},{0,3}};
	IntervalVector r1(3,_r1);
	m[0]=r1;
	m.resize(2,3);
	m[1][0]=Interval(-1,0);
	m[1][1]=Interval(-2,0);
	m[1][2]=Interval(-3,0);

	TEST_ASSERT(m==M1());
}

void TestIntervalMatrix::resize03() {
	IntervalMatrix e(IntervalMatrix::empty(1,1));
	e.resize(2,3);
	TEST_ASSERT(e.is_empty());
}

void TestIntervalMatrix::minus01() {
	IntervalMatrix m(M1());
	IntervalMatrix m2(-m);
	for (int i=0; i<2; i++) {
		for (int j=0; j<3; j++) {
			TEST_ASSERT(m2[i][j]==-m[i][j]);
		}
	}
}

void TestIntervalMatrix::minus02() {
	TEST_ASSERT(-IntervalMatrix::empty(2,3).is_empty());
}

void TestIntervalMatrix::add01() {
	IntervalMatrix m(M1());
	IntervalMatrix m2(m+m);

	for (int i=0; i<2; i++) {
		for (int j=0; j<3; j++) {
			TEST_ASSERT(m2[i][j]==m[i][j]+m[i][j]);
		}
	}

	TEST_ASSERT(m2==(IntervalMatrix(m)+=m));
}

void TestIntervalMatrix::add02() {
	IntervalMatrix m1(IntervalMatrix::empty(2,3));
	IntervalMatrix m2(2,3);

	TEST_ASSERT((m1+m2).is_empty());
	TEST_ASSERT((m1+=m2).is_empty());
	TEST_ASSERT((m2+=m1).is_empty());
}

void TestIntervalMatrix::sub01() {
	IntervalMatrix m(M1());
	IntervalMatrix m2(m-m);
	for (int i=0; i<2; i++) {
		for (int j=0; j<3; j++) {
			TEST_ASSERT(m2[i][j]==m[i][j]-m[i][j]);
		}
	}

	TEST_ASSERT(m2==(IntervalMatrix(m)-=m));
}

void TestIntervalMatrix::sub02() {
	IntervalMatrix m1(IntervalMatrix::empty(2,3));
	IntervalMatrix m2(2,3);

	TEST_ASSERT((m1-m2).is_empty());
	TEST_ASSERT((m1-=m2).is_empty());
	TEST_ASSERT((m2-=m1).is_empty());
}

void TestIntervalMatrix::mul01() {
	IntervalMatrix m(M1());
	IntervalMatrix m2(M2());
	IntervalMatrix m3(m*m2);
	TEST_ASSERT(m3.nb_rows()==2);
	TEST_ASSERT(m3.nb_cols()==2);

	for (int i=0; i<2; i++) {
		for (int j=0; j<2; j++)
			TEST_ASSERT(m3[i][j]==m[i][0]*m2[0][j]+m[i][1]*m2[1][j]+m[i][2]*m2[2][j]);
	}

	TEST_ASSERT(m3==(IntervalMatrix(m)*=m2));
}

void TestIntervalMatrix::mul02() {
	IntervalMatrix m1(IntervalMatrix::empty(2,3));
	IntervalMatrix m2(3,2);

	TEST_ASSERT((m1*m2).is_empty());
	TEST_ASSERT((m1*=m2).is_empty());
	TEST_ASSERT((m2*=m1).is_empty());
}

void TestIntervalMatrix::put01() {

	IntervalMatrix M1=2*Matrix::eye(3);
	IntervalVector V1(3);
	V1[0]=3; V1[1]=4; V1[2]=5;
	IntervalMatrix res(4,4);
	res.put(0,0,M1);
	res.put(0,3,V1,false);
	res.put(3,0,Vector::ones(3),true);
	res[3][3]=6;
	double _expected[16] = { 2,0,0,3,
							 0,2,0,4,
							 0,0,2,5,
							 1,1,1,6 };
	TEST_ASSERT(res==(Matrix(4,4,_expected)));
}

