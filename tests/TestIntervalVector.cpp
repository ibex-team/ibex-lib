/* ============================================================================
 * I B E X - Interval Vector Tests
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Mar 2, 2012
 * ---------------------------------------------------------------------------- */

#include "TestIntervalVector.h"
#include "ibex_Interval.h"
#include "utils.h"

using namespace std;

void TestIntervalVector::cons01() {
	IntervalVector x(2);
	x[0]=Interval::ALL_REALS;
	x[1]=Interval::ALL_REALS;
	check(x,IntervalVector(2));
	check(x,IntervalVector(x));
	check(x,IntervalVector(2)=x);
}

void TestIntervalVector::cons02() {
	IntervalVector x(2);
	x[0]=Interval(0,1);
	x[1]=Interval(0,1);
	check(x,IntervalVector(2,Interval(0,1)));
	check(x,IntervalVector(x));
	check(x,IntervalVector(2)=x);
}

void TestIntervalVector::cons03() {
	IntervalVector x(2);
	x[0]=Interval(0,1);
	x[1]=Interval(2,3);
	check(x,IntervalVector(x));
	check(x,IntervalVector(2)=x);
}

void TestIntervalVector::cons04() {
	double bounds[][2] = {{0,1},{2,3}};
	IntervalVector x(2);
	x[0]=Interval(0,1);
	x[1]=Interval(2,3);
	check(x,IntervalVector(2,bounds));
	check(x,IntervalVector(2)=x);
}

void TestIntervalVector::cons05() {
	IntervalVector x(2);
	x[0].set_empty();
	x[1].set_empty();
	check(x,IntervalVector::empty(2));
	check(x,IntervalVector(2)=x);
}

void TestIntervalVector::set_empty01() {
	IntervalVector x(2);
	TEST_ASSERT(!x.is_empty());
	x.set_empty();
	TEST_ASSERT(x.is_empty());
}


void TestIntervalVector::is_empty01() {
	TEST_ASSERT(IntervalVector::empty(2).is_empty());
}

void TestIntervalVector::is_empty02() {
	TEST_ASSERT(!IntervalVector(2).is_empty());
}

void TestIntervalVector::resize01() {
	IntervalVector x(1);
	x[0]=Interval(1,2);
	x.resize(3);
	TEST_ASSERT(x.size()==3);
	check(x[0],Interval(1,2));
	check(x[1],Interval::ALL_REALS);
	check(x[2],Interval::ALL_REALS);
}

void TestIntervalVector::resize02() {
	IntervalVector x(1);
	x[0]=Interval(1,2);
	x.resize(1);
	TEST_ASSERT(x.size()==1);
	check(x[0],Interval(1,2));
}

void TestIntervalVector::resize03() {
	IntervalVector x(2);
	x[0]=Interval(1,2);
	x.set_empty();
	x.resize(3);
	TEST_ASSERT(x.size()==3);
	TEST_ASSERT(x.is_empty());
	TEST_ASSERT(x[2].is_empty());
}

void TestIntervalVector::resize04() {
	IntervalVector x(5);
	x[0]=Interval(1,2);
	x[1]=Interval(3,4);
	x.resize(2);
	TEST_ASSERT(x.size()==2);
	check(x[0],Interval(1,2));
	check(x[1],Interval(3,4));
}

static double _x[][2]={{0,1},{2,3},{4,5}};

void TestIntervalVector::subvector01() {
	double _x01[][2]={{0,1},{2,3}};
	TEST_ASSERT(IntervalVector(3,_x).subvector(0,1)==IntervalVector(2,_x01));
}

void TestIntervalVector::subvector02() {
	double _x12[][2]={{2,3},{4,5}};
	TEST_ASSERT(IntervalVector(3,_x).subvector(1,2)==IntervalVector(2,_x12));
}

void TestIntervalVector::subvector03() {
	double _x11[][2]={{2,3}};
	TEST_ASSERT(IntervalVector(3,_x).subvector(1,1)==IntervalVector(1,_x11));
}

void TestIntervalVector::subvector04() {
	double _x22[][2]={{4,5}};
	TEST_ASSERT(IntervalVector(3,_x).subvector(2,2)==IntervalVector(1,_x22));
}

void TestIntervalVector::subvector05() {
	TEST_ASSERT(IntervalVector(3,_x).subvector(0,2)==IntervalVector(3,_x));
}

void TestIntervalVector::inter01() {
	double _x1[][2]={{0,2},{4,6}};
	double _x2[][2]={{1,3},{5,7}};
	double _res[][2]={{1,2},{5,6}};
	TEST_ASSERT(((IntervalVector(2,_x1)) &=IntervalVector(2,_x2))==IntervalVector(2,_res));
	TEST_ASSERT(((IntervalVector(2,_x1)) & IntervalVector(2,_x2))==IntervalVector(2,_res));
}

void TestIntervalVector::inter02() {
	double _x1[][2]={{0,2},{4,6}};
	double _x2[][2]={{1,3},{7,8}};
	TEST_ASSERT(((IntervalVector(2,_x1)) &=IntervalVector(2,_x2)).is_empty());
	TEST_ASSERT(((IntervalVector(2,_x1)) & IntervalVector(2,_x2)).is_empty());
}

void TestIntervalVector::inter03() {
	double _x1[][2]={{0,2},{4,6}};
	TEST_ASSERT(((IntervalVector(2,_x1)) &=IntervalVector::empty(2)).is_empty());
	TEST_ASSERT(((IntervalVector(2,_x1)) & IntervalVector::empty(2)).is_empty());
}

void TestIntervalVector::inter04() {
	double _x1[][2]={{0,2},{4,6}};
	TEST_ASSERT(((IntervalVector::empty(2)) &=IntervalVector(2,_x1)).is_empty());
	TEST_ASSERT(((IntervalVector::empty(2)) & IntervalVector(2,_x1)).is_empty());
}

void TestIntervalVector::hull01() {
	double _x1[][2]={{0,1},{4,5}};
	double _x2[][2]={{2,3},{6,7}};
	double _res[][2]={{0,3},{4,7}};
	TEST_ASSERT(((IntervalVector(2,_x1)) |=IntervalVector(2,_x2))==IntervalVector(2,_res));
	TEST_ASSERT(((IntervalVector(2,_x1)) | IntervalVector(2,_x2))==IntervalVector(2,_res));
}

void TestIntervalVector::hull02() {
	double _x1[][2]={{0,1},{4,5}};
	IntervalVector x1(2,_x1);
	TEST_ASSERT((x1 |= x1)==x1);
	TEST_ASSERT((x1 | x1)==x1);
}

void TestIntervalVector::hull03() {
	double _x1[][2]={{0,2},{4,6}};
	IntervalVector x1(2,_x1);
	TEST_ASSERT((x1 |=IntervalVector::empty(2))==x1);
	TEST_ASSERT((x1 | IntervalVector::empty(2))==x1);
}

void TestIntervalVector::hull04() {
	double _x1[][2]={{0,2},{4,6}};
	IntervalVector x1(2,_x1);
	TEST_ASSERT(((IntervalVector::empty(2)) |= x1)==x1);
	TEST_ASSERT(((IntervalVector::empty(2)) | x1)==x1);
}

void TestIntervalVector::eq01() {
	IntervalVector x(3,_x);
	TEST_ASSERT(x==x);
	TEST_ASSERT(!(x!=x));
}

void TestIntervalVector::eq02() {
	IntervalVector x(3,_x);
	double _x01[][2]={{0,1},{2,3}};
	IntervalVector x1(2,_x01);
	TEST_ASSERT(!(x==x1));
	TEST_ASSERT(x!=x1);
}

void TestIntervalVector::eq03() {
	double _x1[][2]={{0,1},{4,5}};
	double _x2[][2]={{2,3},{6,7}};
	IntervalVector x1(2,_x1);
	IntervalVector x2(2,_x2);
	x1.set_empty();
	x2.set_empty();
	TEST_ASSERT(x1==x2);
	TEST_ASSERT(!(x1!=x2));
}

void TestIntervalVector::mid01() {
	IntervalVector x(3,_x);
	IntervalVector m=x.mid();
	check(m[0],0.5);
	check(m[1],2.5);
	check(m[2],4.5);
}

void TestIntervalVector::is_flat01() {
	TEST_ASSERT(!IntervalVector(3,_x).is_flat());
}

void TestIntervalVector::is_flat02() {
	TEST_ASSERT(IntervalVector::empty(3).is_flat());
}

void TestIntervalVector::is_flat03() {
	TEST_ASSERT(IntervalVector(1,Interval(0,0)).is_flat());
	TEST_ASSERT(!IntervalVector(1,Interval(0,1)).is_flat());
}

void TestIntervalVector::is_flat04() {
	double _x1[][2]={{0,1},{2,2},{3,4}};
	TEST_ASSERT(IntervalVector(3,_x1).is_flat());
}

void TestIntervalVector::is_flat05() {
	double _x1[][2]={{0,1},{2,3},{4,4}};
	TEST_ASSERT(IntervalVector(3,_x1).is_flat());
}

void TestIntervalVector::extr_diam_index01() {
	 double _x1[][2]={{0,2},{0,1},{0,3}};
	IntervalVector x1(3,_x1);
	TEST_ASSERT(x1.extr_diam_index(true)==1);
	TEST_ASSERT(x1.extr_diam_index(false)==2);
	TEST_ASSERT(x1.min_diam()==1);
	TEST_ASSERT(x1.max_diam()==3);
}

void TestIntervalVector::extr_diam_index02() {
	double _x1[][2]={{0,1},{0,3},{0,2}};
	IntervalVector x1(3,_x1);
	TEST_ASSERT(x1.extr_diam_index(true)==0);
	TEST_ASSERT(x1.extr_diam_index(false)==1);
	TEST_ASSERT(x1.min_diam()==1);
	TEST_ASSERT(x1.max_diam()==3);
}

void TestIntervalVector::extr_diam_index03() {
	double _x1[][2]={{0,3},{0,2},{0,1}};
	IntervalVector x1(3,_x1);
	TEST_ASSERT(x1.extr_diam_index(true)==2);
	TEST_ASSERT(x1.extr_diam_index(false)==0);
	TEST_ASSERT(x1.min_diam()==1);
	TEST_ASSERT(x1.max_diam()==3);
}

void TestIntervalVector::extr_diam_index04() {
	double _x1[][2]={{0,1},{0,2},{NEG_INFINITY,0}};
	IntervalVector x1(3,_x1);
	TEST_ASSERT(x1.extr_diam_index(true)==0);
	TEST_ASSERT(x1.extr_diam_index(false)==2);
	TEST_ASSERT(x1.min_diam()==1);
	TEST_ASSERT(x1.max_diam()==POS_INFINITY);
}

void TestIntervalVector::extr_diam_index05() {
	double _x1[][2]={{NEG_INFINITY,0}};
	IntervalVector x1(1,_x1);
	TEST_ASSERT(x1.extr_diam_index(true)==0);
	TEST_ASSERT(x1.extr_diam_index(false)==0);
	TEST_ASSERT(x1.min_diam()==POS_INFINITY);
	TEST_ASSERT(x1.max_diam()==POS_INFINITY);
}

void TestIntervalVector::minus01() {
	double _x1[][2]={{0,3},{0,2},{0,1}};
	double _x2[][2]={{-3,0},{-2,0},{-1,0}};
	check(-IntervalVector(3,_x1),IntervalVector(3,_x2));
}

void TestIntervalVector::minus02() {
	double _x1[][2]={{0,1},{0,POS_INFINITY}};
	double _x2[][2]={{-1,0},{NEG_INFINITY,0}};
	check(-IntervalVector(2,_x1),IntervalVector(2,_x2));
}

void TestIntervalVector::minus03() {
	check(-IntervalVector::empty(2),IntervalVector::empty(2));
}

void TestIntervalVector::add01() {
	double _x1[][2]={{0,3},{0,2},{0,1}};
	double _x2[][2]={{0,1},{0,1},{0,1}};
	double _x3[][2]={{0,4},{0,3},{0,2}};

	IntervalVector x1(3,_x1);
	IntervalVector x2(3,_x2);
	IntervalVector x3(3,_x3);
	IntervalVector e(IntervalVector::empty(3));

	check(x1+x2,x3);
	check(x1+e,e);
	check(IntervalVector(x1)+=e,e);
	check(e+x1,e);
	check(e+=x1,e);
	check(e+e,e);
	check(e+=e,e);
	check(IntervalVector(x1)+=x2,x3);
	check(IntervalVector(x1)+=e,e);
	check(IntervalVector(x2)+=x1,x3);
}

void TestIntervalVector::sub01() {
	double _x1[][2]={{0,3},{0,2},{0,1}};
	double _x2[][2]={{0,1},{0,1},{0,1}};
	double _x3[][2]={{-1,3},{-1,2},{-1,1}};
	IntervalVector x1(3,_x1);
	IntervalVector x2(3,_x2);
	IntervalVector x3(3,_x3);
	IntervalVector e(IntervalVector::empty(3));

	check(x1-x2,x3);
	check(x2-x1,-x3);
	check(x1-e,e);
	check(IntervalVector(x1)-=e,e);
	check(e-x1,e);
	check(e-=x1,e);
	check(e-e,e);
	check(e-=e,e);
	check(IntervalVector(x1)-=x2,x3);
	check(IntervalVector(x2)-=x1,-x3);
}

