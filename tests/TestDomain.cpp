/* ============================================================================
 * I B E X - Domain Tests
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Mar 2, 2012
 * ---------------------------------------------------------------------------- */

#include "TestDomain.h"
#include "ibex_Interval.h"

using namespace std;

namespace ibex {

#define ERROR 1e-10

void TestDomain::cons01() {
	Domain d;
	TEST_ASSERT(d.size()==0);
}

static IntervalVector v0() {
	double vec0[][2] = { {0,3}, {0,4}, {0,5} };
	return IntervalVector(3,vec0);
}

static IntervalVector v1() {
	double vec1[][2] = { {1,3}, {1,4}  };
	return IntervalVector(2,vec1);
}

static IntervalVector v2() {
	double vec2[][2] = { {2,3}, {2,4}, {2,5}, {2,6} };
	return IntervalVector(4,vec2);
}

static IntervalMatrix M1() {
	double m[][2]={{0,1},{0,2},{0,3},
			       {-1,0},{-2,0},{-3,0}};
	return IntervalMatrix(2,3,m);
}

static IntervalMatrix M2() {
	double m[][2]={{0,1},{-1,0},
	               {0,2},{-2,0},
	               {0,3},{-3,0}};
	return IntervalMatrix(3,2,m);
}

static IntervalMatrix M3() {
	double m[][2]={{1,2},{1,2},{2,4},
	               {-2,-1},{-2,-1},{-4,-2}};
	return IntervalMatrix(2,3,m);
}

void TestDomain::scal01() {
	Domain d;
	int i=d.add(Dim(0,0,0));
	TEST_ASSERT(i==0);
	TEST_ASSERT(d.size()==1);
	i=d.add(Dim(0,0,0));
	TEST_ASSERT(i==1);
	TEST_ASSERT(d.size()==2);
	i=d.add(Dim(0,0,0));
	TEST_ASSERT(i==2);
	TEST_ASSERT(d.size()==3);

	d.get(0)=Interval(1,2);
	d.get(1)=Interval(1,3);
	d.get(2)=Interval(1,4);

	TEST_ASSERT(d.get(0)==Interval(1,2));
	TEST_ASSERT(d.get(1)==Interval(1,3));
	TEST_ASSERT(d.get(2)==Interval(1,4));
	TEST_ASSERT(d[0]==Interval(1,2));
	TEST_ASSERT(d[1]==Interval(1,3));
	TEST_ASSERT(d[2]==Interval(1,4));

	IntervalVector v=v0();
	d=v0();
	TEST_ASSERT(d[0]==v[0]);
	TEST_ASSERT(d[1]==v[1]);
	TEST_ASSERT(d[2]==v[2]);
	IntervalVector v2(3);
	v2=d;
	TEST_ASSERT(v2[0]==v[0]);
	TEST_ASSERT(v2[1]==v[1]);
	TEST_ASSERT(v2[2]==v[2]);
}

void TestDomain::vector01() {
	Domain d;
	d.add(Dim(0,0,3));
	TEST_ASSERT(d.size()==3);
	d.add(Dim(0,0,2));
	TEST_ASSERT(d.size()==5);
	d.add(Dim(0,4,0)); // row vector
	TEST_ASSERT(d.size()==9);

	d.vector(0)=v0();
	d.vector(1)=v1();
	d.vector(2)=v2();

	TEST_ASSERT(d.vector(0)==v0());
	TEST_ASSERT(d.vector(1)==v1());
	TEST_ASSERT(d.vector(2)==v2());

	IntervalVector v(9);
	for (int i=0; i<9; i++) v[i]=Interval(1,i+2);
	d=v;

	TEST_ASSERT(d.vector(0)[0]==Interval(1,2));
	TEST_ASSERT(d.vector(0)[1]==Interval(1,3));
	TEST_ASSERT(d.vector(0)[2]==Interval(1,4));
	TEST_ASSERT(d.vector(1)[0]==Interval(1,5));
	TEST_ASSERT(d.vector(1)[1]==Interval(1,6));
	TEST_ASSERT(d.vector(2)[0]==Interval(1,7));
	TEST_ASSERT(d.vector(2)[1]==Interval(1,8));
	TEST_ASSERT(d.vector(2)[2]==Interval(1,9));
	TEST_ASSERT(d.vector(2)[3]==Interval(1,10));

	IntervalVector v2(9);
	v2=d;
	for (int i=0; i<9; i++)
		TEST_ASSERT(v2[i]==Interval(1,i+2));
}

void TestDomain::matrix01() {
	Domain d;
	d.add(Dim(0,2,3));
	TEST_ASSERT(d.size()==6);
	d.add(Dim(0,3,2));
	TEST_ASSERT(d.size()==12);
	d.add(Dim(0,2,3));
	TEST_ASSERT(d.size()==18);

	d.matrix(0)=M1();
	d.matrix(1)=M2();
	d.matrix(2)=M3();

	TEST_ASSERT(d.matrix(0)==M1());
	TEST_ASSERT(d.matrix(1)==M2());
	TEST_ASSERT(d.matrix(2)==M3());

	IntervalVector v(18);
	for (int i=0; i<18; i++) v[i]=Interval(1,i+2);
	d=v;

	TEST_ASSERT(d.matrix(0)[0][0]==Interval(1,2));
	TEST_ASSERT(d.matrix(0)[0][1]==Interval(1,3));
	TEST_ASSERT(d.matrix(0)[0][2]==Interval(1,4));
	TEST_ASSERT(d.matrix(0)[1][0]==Interval(1,5));
	TEST_ASSERT(d.matrix(0)[1][1]==Interval(1,6));
	TEST_ASSERT(d.matrix(0)[1][2]==Interval(1,7));

	TEST_ASSERT(d.matrix(1)[0][0]==Interval(1,8));
	TEST_ASSERT(d.matrix(1)[0][1]==Interval(1,9));
	TEST_ASSERT(d.matrix(1)[1][0]==Interval(1,10));
	TEST_ASSERT(d.matrix(1)[1][1]==Interval(1,11));
	TEST_ASSERT(d.matrix(1)[2][0]==Interval(1,12));
	TEST_ASSERT(d.matrix(1)[2][1]==Interval(1,13));

	TEST_ASSERT(d.matrix(2)[0][0]==Interval(1,14));
	TEST_ASSERT(d.matrix(2)[0][1]==Interval(1,15));
	TEST_ASSERT(d.matrix(2)[0][2]==Interval(1,16));
	TEST_ASSERT(d.matrix(2)[1][0]==Interval(1,17));
	TEST_ASSERT(d.matrix(2)[1][1]==Interval(1,18));
	TEST_ASSERT(d.matrix(2)[1][2]==Interval(1,19));

	IntervalVector v2(18);
	v2=d;
	for (int i=0; i<18; i++)
		TEST_ASSERT(v2[i]==Interval(1,i+2));

}

void TestDomain::mix01() {
	Domain d;
	d.add(Dim(0,0,0));
	TEST_ASSERT(d.size()==1);
	d.add(Dim(0,2,3));
	TEST_ASSERT(d.size()==7);
	d.add(Dim(0,0,3));
	TEST_ASSERT(d.size()==10);

	d.get(0)=Interval(1,2);
	d.matrix(1)=M1();
	d.vector(2)=v0();

	TEST_ASSERT(d.get(0)==Interval(1,2));
	TEST_ASSERT(d.matrix(1)==M1());
	TEST_ASSERT(d.vector(2)==v0());

	IntervalVector v(10);
	for (int i=0; i<10; i++) v[i]=Interval(1,i+2);
	d=v;

	TEST_ASSERT(d.get(0)==Interval(1,2));
	TEST_ASSERT(d.matrix(1)[0][0]==Interval(1,3));
	TEST_ASSERT(d.matrix(1)[0][1]==Interval(1,4));
	TEST_ASSERT(d.matrix(1)[0][2]==Interval(1,5));
	TEST_ASSERT(d.matrix(1)[1][0]==Interval(1,6));
	TEST_ASSERT(d.matrix(1)[1][1]==Interval(1,7));
	TEST_ASSERT(d.matrix(1)[1][2]==Interval(1,8));

	TEST_ASSERT(d.vector(2)[0]==Interval(1,9));
	TEST_ASSERT(d.vector(2)[1]==Interval(1,10));
	TEST_ASSERT(d.vector(2)[2]==Interval(1,11));

	IntervalVector v2(10);
	v2=d;
	for (int i=0; i<10; i++)
		TEST_ASSERT(v2[i]==Interval(1,i+2));
}

} // namespace ibex
