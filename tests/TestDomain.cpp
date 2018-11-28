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

void TestDomain::setUp() {
	int r=5;
	int c=4;
	m = new Domain(Dim::matrix(r,c));
	for (int i=0; i<r; i++)
		for (int j=0; j<c; j++)
			m->m()[i][j]=i*c+j;

	rv = new Domain(Dim::row_vec(r));
	for (int i=0; i<r; i++)
		rv->v()[i] = i;

	cv = new Domain(Dim::col_vec(c));
	for (int i=0; i<c; i++)
		cv->v()[i] = i;
}

void TestDomain::tearDown() {
	delete cv;
	delete rv;
	delete m;
}

void TestDomain::row_vec() {
	Interval x1=rv->v()[0];
	IntervalVector x2(rv->v().subvector(1,3));
	Interval x3=rv->v()[4];

	Domain d1(x1);
	Domain d2(x2,true);
	Domain d3(x3);
	Array<const Domain> args(d1,d2,d3);
	Domain d(args,true);
	CPPUNIT_ASSERT(d==*rv);
}

void TestDomain::col_vec() {
	Interval x1=cv->v()[0];
	IntervalVector x2(cv->v().subvector(1,3));
	Interval x3=cv->v()[4];

	Domain d1(x1);
	Domain d2(x2,false);
	Domain d3(x3);
	Array<const Domain> args(d1,d2,d3);
	Domain d(args,false);
	CPPUNIT_ASSERT(d==*cv);
}

void TestDomain::matrix_01() {;
	IntervalVector x1=m->m().row(0);
	IntervalMatrix x2(m->m().rows(1,m->dim.nb_rows()-2));
	IntervalVector x3=m->m().row(m->dim.nb_rows()-1);

	Domain d1(x1,true);
	Domain d2(x2);
	Domain d3(x3,true);
	Array<const Domain> args(d1,d2,d3);
	Domain d(args,false);
	CPPUNIT_ASSERT(d==*m);
}

void TestDomain::matrix_02() {;
	IntervalVector x1=m->m().row(0);
	IntervalMatrix x2(m->m().rows(1,m->dim.nb_cols()-2));
	IntervalVector x3=m->m().row(m->dim.nb_cols()-1);

	Domain d1(x1,false);
	Domain d2(x2);
	Domain d3(x3,false);
	Array<const Domain> args(d1,d2,d3);
	Domain d(args,true);
	CPPUNIT_ASSERT(d==*m);
}

void TestDomain::index_vec_elt() {

	int c=1;
	DoubleIndex idx=DoubleIndex::one_elt(rv->dim,0,c);
	Domain d2=(*rv)[idx];
	CPPUNIT_ASSERT(d2.dim==Dim::scalar());
	CPPUNIT_ASSERT(d2.i()==rv->v()[c]);
	CPPUNIT_ASSERT(d2.is_reference);
}

void TestDomain::index_vec_subrow() {
	int c1=1,c2=3;
	DoubleIndex idx=DoubleIndex::subrow(rv->dim,0,c1,c2);
	Domain d2=(*rv)[idx];
	CPPUNIT_ASSERT(d2.dim==Dim::row_vec(c2-c1+1));
	CPPUNIT_ASSERT(!d2.is_reference);
	CPPUNIT_ASSERT(d2.v()==rv->v().subvector(c1,c2));
}

void TestDomain::index_vec_subcol() {
	int r1=1,r2=3;
	DoubleIndex idx=DoubleIndex::subcol(cv->dim,r1,r2,0);
	Domain d2=(*cv)[idx];
	CPPUNIT_ASSERT(d2.dim==Dim::col_vec(r2-r1+1));
	CPPUNIT_ASSERT(!d2.is_reference);
	CPPUNIT_ASSERT(d2.v()==cv->v().subvector(r1,r2));
}

void TestDomain::index_mat_elt() {
	int r=1, c=0;
	DoubleIndex idx=DoubleIndex::one_elt(m->dim,r,c);
	Domain d2=(*m)[idx];
	CPPUNIT_ASSERT(d2.dim==Dim::scalar());
	CPPUNIT_ASSERT(d2.i()==m->m()[r][c]);
	CPPUNIT_ASSERT(d2.is_reference);
}

void TestDomain::index_mat_subrow() {
	int r=1,c1=1,c2=3;
	DoubleIndex idx=DoubleIndex::subrow(m->dim,r,c1,c2);
	Domain d2=(*m)[idx];
	CPPUNIT_ASSERT(d2.dim==Dim::row_vec(c2-c1+1));
	CPPUNIT_ASSERT(!d2.is_reference);
	CPPUNIT_ASSERT(d2.v()==m->m().row(r).subvector(c1,c2));
}

void TestDomain::index_mat_row() {
	int r=1;
	DoubleIndex idx=DoubleIndex::one_row(m->dim,r);
	Domain d2=(*m)[idx];
	CPPUNIT_ASSERT(d2.dim==Dim::row_vec(m->dim.nb_cols()));
	CPPUNIT_ASSERT(d2.is_reference);
	CPPUNIT_ASSERT(d2.v()==m->m().row(r));
}

void TestDomain::index_mat_subcol() {
	int c=1,r1=1,r2=3;
	DoubleIndex idx=DoubleIndex::subcol(m->dim,r1,r2,c);
	Domain d2=(*m)[idx];
	CPPUNIT_ASSERT(d2.dim==Dim::col_vec(r2-r1+1));
	CPPUNIT_ASSERT(!d2.is_reference);
	CPPUNIT_ASSERT(d2.v()==m->m().col(c).subvector(r1,r2));
}

void TestDomain::index_mat_col() {
	int c=1;
	DoubleIndex idx=DoubleIndex::one_col(m->dim,c);
	Domain d2=(*m)[idx];
	CPPUNIT_ASSERT(d2.dim==Dim::col_vec(m->dim.nb_rows()));
	CPPUNIT_ASSERT(!d2.is_reference);
	CPPUNIT_ASSERT(d2.v()==m->m().col(c));
}

void TestDomain::index_mat_rows() {
	int r1=1,r2=3;
	DoubleIndex idx=DoubleIndex::rows(m->dim,r1,r2);
	Domain d2=(*m)[idx];
	CPPUNIT_ASSERT(d2.dim==Dim::matrix(r2-r1+1,m->dim.nb_cols()));
	CPPUNIT_ASSERT(!d2.is_reference);
	CPPUNIT_ASSERT(d2.m()==m->m().rows(r1,r2));
}

void TestDomain::index_mat_cols() {
	int c1=1,c2=3;
	DoubleIndex idx=DoubleIndex::cols(m->dim,c1,c2);
	Domain d2=(*m)[idx];
	CPPUNIT_ASSERT(d2.dim==Dim::matrix(m->dim.nb_rows(),c2-c1+1));
	CPPUNIT_ASSERT(!d2.is_reference);
	CPPUNIT_ASSERT(d2.m()==m->m().cols(c1,c2));
}

void TestDomain::index_mat_submat() {
	int r1=1,r2=2,c1=0,c2=1;
	DoubleIndex idx=DoubleIndex::submatrix(m->dim,r1,r2,c1,c2);
	Domain d2=(*m)[idx];
	CPPUNIT_ASSERT(d2.dim==Dim::matrix(r2-r1+1,c2-c1+1));
	CPPUNIT_ASSERT(!d2.is_reference);
	CPPUNIT_ASSERT(d2.m()==m->m().submatrix(r1,r2,c1,c2));
}


void TestDomain::load01() {
	Domain x1(Dim::col_vec(3));
	Domain x2(Dim::col_vec(2));
	Domain x3(Dim::scalar());
	Domain x4(Dim::matrix(2,2));
	Domain x5(Dim::scalar());

	Array<Domain> x(x1,x2,x3,x4,x5);

	IntervalVector box(11,Interval(0,1));

	vector<int> vars;
	vars.push_back(3);
	vars.push_back(5);
	vars.push_back(7);
	vars.push_back(9);
	load(x,box,vars);

	CPPUNIT_ASSERT(x[0].v()[0]==Interval::ALL_REALS);
	CPPUNIT_ASSERT(x[0].v()[1]==Interval::ALL_REALS);
	CPPUNIT_ASSERT(x[0].v()[2]==Interval::ALL_REALS);

	CPPUNIT_ASSERT(x[1].v()[0]==Interval(0,1));
	CPPUNIT_ASSERT(x[1].v()[1]==Interval::ALL_REALS);

	CPPUNIT_ASSERT(x[2].i()==Interval(0,1));

	CPPUNIT_ASSERT(x[3].m()[0][0]==Interval::ALL_REALS);
	CPPUNIT_ASSERT(x[3].m()[0][1]==Interval(0,1));
	CPPUNIT_ASSERT(x[3].m()[1][0]==Interval::ALL_REALS);
	CPPUNIT_ASSERT(x[3].m()[1][1]==Interval(0,1));

	CPPUNIT_ASSERT(x[4].i()==Interval::ALL_REALS);
}


void TestDomain::load02() {
	Domain x1(Dim::col_vec(3));
	Domain x2(Dim::col_vec(2));
	Domain x3(Dim::scalar());
	Domain x4(Dim::matrix(2,2));
	Domain x5(Dim::scalar());

	Array<Domain> x(x1,x2,x3,x4,x5);

	IntervalVector box(11,Interval::ALL_REALS);

	vector<int> vars;
	vars.push_back(3);
	vars.push_back(5);
	vars.push_back(7);
	vars.push_back(9);

	x[1].v()[0]=Interval(0,1);
	x[2].i()=Interval(0,2);
	x[3].m()[0][1]=Interval(0,3);
	x[3].m()[1][1]=Interval(0,4);

	load(box,x,vars);

	CPPUNIT_ASSERT(box[0]==Interval::ALL_REALS);
	CPPUNIT_ASSERT(box[1]==Interval::ALL_REALS);
	CPPUNIT_ASSERT(box[2]==Interval::ALL_REALS);
	CPPUNIT_ASSERT(box[3]==Interval(0,1));
	CPPUNIT_ASSERT(box[4]==Interval::ALL_REALS);
	CPPUNIT_ASSERT(box[5]==Interval(0,2));
	CPPUNIT_ASSERT(box[6]==Interval::ALL_REALS);
	CPPUNIT_ASSERT(box[7]==Interval(0,3));
	CPPUNIT_ASSERT(box[8]==Interval::ALL_REALS);
	CPPUNIT_ASSERT(box[9]==Interval(0,4));
	CPPUNIT_ASSERT(box[10]==Interval::ALL_REALS);
}

void TestDomain::load03() {
	Domain x1(Dim::col_vec(3));
	Domain x2(Dim::col_vec(2));
	Domain x3(Dim::scalar());
	Domain x4(Dim::matrix(2,2));
	Domain x5(Dim::scalar());

	Array<Domain> x(x1,x2,x3,x4,x5);

	IntervalVector box(11,Interval(0,1));

	load(x,box);

	CPPUNIT_ASSERT(x[0].v()[0]==Interval(0,1));
	CPPUNIT_ASSERT(x[0].v()[1]==Interval(0,1));
	CPPUNIT_ASSERT(x[0].v()[2]==Interval(0,1));

	CPPUNIT_ASSERT(x[1].v()[0]==Interval(0,1));
	CPPUNIT_ASSERT(x[1].v()[1]==Interval(0,1));

	CPPUNIT_ASSERT(x[2].i()==Interval(0,1));

	CPPUNIT_ASSERT(x[3].m()[0][0]==Interval(0,1));
	CPPUNIT_ASSERT(x[3].m()[0][1]==Interval(0,1));
	CPPUNIT_ASSERT(x[3].m()[1][0]==Interval(0,1));
	CPPUNIT_ASSERT(x[3].m()[1][1]==Interval(0,1));

	CPPUNIT_ASSERT(x[4].i()==Interval(0,1));
}

void TestDomain::load04() {
	Domain x1(Dim::col_vec(3));
	Domain x2(Dim::col_vec(2));
	Domain x3(Dim::scalar());
	Domain x4(Dim::matrix(2,2));
	Domain x5(Dim::scalar());

	Array<Domain> x(x1,x2,x3,x4,x5);

	IntervalVector box(11,Interval::ALL_REALS);

	x[1].v()[0]=Interval(0,1);
	x[2].i()=Interval(0,2);
	x[3].m()[0][1]=Interval(0,3);
	x[3].m()[1][1]=Interval(0,4);

	load(box,x);

	CPPUNIT_ASSERT(box[0]==Interval::ALL_REALS);
	CPPUNIT_ASSERT(box[1]==Interval::ALL_REALS);
	CPPUNIT_ASSERT(box[2]==Interval::ALL_REALS);
	CPPUNIT_ASSERT(box[3]==Interval(0,1));
	CPPUNIT_ASSERT(box[4]==Interval::ALL_REALS);
	CPPUNIT_ASSERT(box[5]==Interval(0,2));
	CPPUNIT_ASSERT(box[6]==Interval::ALL_REALS);
	CPPUNIT_ASSERT(box[7]==Interval(0,3));
	CPPUNIT_ASSERT(box[8]==Interval::ALL_REALS);
	CPPUNIT_ASSERT(box[9]==Interval(0,4));
	CPPUNIT_ASSERT(box[10]==Interval::ALL_REALS);
}

/*
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

*/
/*

void TestDomain::cons01() {
	Domain d;
	CPPUNIT_ASSERT(d.size()==0);
}

void TestDomain::scal01() {
	Domain d;
	int i=d.add(Dim(0,0,0));
	CPPUNIT_ASSERT(i==0);
	CPPUNIT_ASSERT(d.size()==1);
	i=d.add(Dim(0,0,0));
	CPPUNIT_ASSERT(i==1);
	CPPUNIT_ASSERT(d.size()==2);
	i=d.add(Dim(0,0,0));
	CPPUNIT_ASSERT(i==2);
	CPPUNIT_ASSERT(d.size()==3);

	d.get(0)=Interval(1,2);
	d.get(1)=Interval(1,3);
	d.get(2)=Interval(1,4);

	CPPUNIT_ASSERT(d.get(0)==Interval(1,2));
	CPPUNIT_ASSERT(d.get(1)==Interval(1,3));
	CPPUNIT_ASSERT(d.get(2)==Interval(1,4));
	CPPUNIT_ASSERT(d[0]==Interval(1,2));
	CPPUNIT_ASSERT(d[1]==Interval(1,3));
	CPPUNIT_ASSERT(d[2]==Interval(1,4));

	IntervalVector v=v0();
	d=v0();
	CPPUNIT_ASSERT(d[0]==v[0]);
	CPPUNIT_ASSERT(d[1]==v[1]);
	CPPUNIT_ASSERT(d[2]==v[2]);
	IntervalVector v2(3);
	v2=d;
	CPPUNIT_ASSERT(v2[0]==v[0]);
	CPPUNIT_ASSERT(v2[1]==v[1]);
	CPPUNIT_ASSERT(v2[2]==v[2]);
}

void TestDomain::vector01() {
	Domain d;
	d.add(Dim(0,0,3));
	CPPUNIT_ASSERT(d.size()==3);
	d.add(Dim(0,0,2));
	CPPUNIT_ASSERT(d.size()==5);
	d.add(Dim(0,4,0)); // row vector
	CPPUNIT_ASSERT(d.size()==9);

	d.vector(0)=v0();
	d.vector(1)=v1();
	d.vector(2)=v2();

	CPPUNIT_ASSERT(d.vector(0)==v0());
	CPPUNIT_ASSERT(d.vector(1)==v1());
	CPPUNIT_ASSERT(d.vector(2)==v2());

	IntervalVector v(9);
	for (int i=0; i<9; i++) v[i]=Interval(1,i+2);
	d=v;

	CPPUNIT_ASSERT(d.vector(0)[0]==Interval(1,2));
	CPPUNIT_ASSERT(d.vector(0)[1]==Interval(1,3));
	CPPUNIT_ASSERT(d.vector(0)[2]==Interval(1,4));
	CPPUNIT_ASSERT(d.vector(1)[0]==Interval(1,5));
	CPPUNIT_ASSERT(d.vector(1)[1]==Interval(1,6));
	CPPUNIT_ASSERT(d.vector(2)[0]==Interval(1,7));
	CPPUNIT_ASSERT(d.vector(2)[1]==Interval(1,8));
	CPPUNIT_ASSERT(d.vector(2)[2]==Interval(1,9));
	CPPUNIT_ASSERT(d.vector(2)[3]==Interval(1,10));

	IntervalVector v2(9);
	v2=d;
	for (int i=0; i<9; i++)
		CPPUNIT_ASSERT(v2[i]==Interval(1,i+2));
}

void TestDomain::matrix01() {
	Domain d;
	d.add(Dim(0,2,3));
	CPPUNIT_ASSERT(d.size()==6);
	d.add(Dim(0,3,2));
	CPPUNIT_ASSERT(d.size()==12);
	d.add(Dim(0,2,3));
	CPPUNIT_ASSERT(d.size()==18);

	d.matrix(0)=M1();
	d.matrix(1)=M2();
	d.matrix(2)=M3();

	CPPUNIT_ASSERT(d.matrix(0)==M1());
	CPPUNIT_ASSERT(d.matrix(1)==M2());
	CPPUNIT_ASSERT(d.matrix(2)==M3());

	IntervalVector v(18);
	for (int i=0; i<18; i++) v[i]=Interval(1,i+2);
	d=v;

	CPPUNIT_ASSERT(d.matrix(0)[0][0]==Interval(1,2));
	CPPUNIT_ASSERT(d.matrix(0)[0][1]==Interval(1,3));
	CPPUNIT_ASSERT(d.matrix(0)[0][2]==Interval(1,4));
	CPPUNIT_ASSERT(d.matrix(0)[1][0]==Interval(1,5));
	CPPUNIT_ASSERT(d.matrix(0)[1][1]==Interval(1,6));
	CPPUNIT_ASSERT(d.matrix(0)[1][2]==Interval(1,7));

	CPPUNIT_ASSERT(d.matrix(1)[0][0]==Interval(1,8));
	CPPUNIT_ASSERT(d.matrix(1)[0][1]==Interval(1,9));
	CPPUNIT_ASSERT(d.matrix(1)[1][0]==Interval(1,10));
	CPPUNIT_ASSERT(d.matrix(1)[1][1]==Interval(1,11));
	CPPUNIT_ASSERT(d.matrix(1)[2][0]==Interval(1,12));
	CPPUNIT_ASSERT(d.matrix(1)[2][1]==Interval(1,13));

	CPPUNIT_ASSERT(d.matrix(2)[0][0]==Interval(1,14));
	CPPUNIT_ASSERT(d.matrix(2)[0][1]==Interval(1,15));
	CPPUNIT_ASSERT(d.matrix(2)[0][2]==Interval(1,16));
	CPPUNIT_ASSERT(d.matrix(2)[1][0]==Interval(1,17));
	CPPUNIT_ASSERT(d.matrix(2)[1][1]==Interval(1,18));
	CPPUNIT_ASSERT(d.matrix(2)[1][2]==Interval(1,19));

	IntervalVector v2(18);
	v2=d;
	for (int i=0; i<18; i++)
		CPPUNIT_ASSERT(v2[i]==Interval(1,i+2));

}

void TestDomain::mix01() {
	Domain d;
	d.add(Dim(0,0,0));
	CPPUNIT_ASSERT(d.size()==1);
	d.add(Dim(0,2,3));
	CPPUNIT_ASSERT(d.size()==7);
	d.add(Dim(0,0,3));
	CPPUNIT_ASSERT(d.size()==10);

	d.get(0)=Interval(1,2);
	d.matrix(1)=M1();
	d.vector(2)=v0();

	CPPUNIT_ASSERT(d.get(0)==Interval(1,2));
	CPPUNIT_ASSERT(d.matrix(1)==M1());
	CPPUNIT_ASSERT(d.vector(2)==v0());

	IntervalVector v(10);
	for (int i=0; i<10; i++) v[i]=Interval(1,i+2);
	d=v;

	CPPUNIT_ASSERT(d.get(0)==Interval(1,2));
	CPPUNIT_ASSERT(d.matrix(1)[0][0]==Interval(1,3));
	CPPUNIT_ASSERT(d.matrix(1)[0][1]==Interval(1,4));
	CPPUNIT_ASSERT(d.matrix(1)[0][2]==Interval(1,5));
	CPPUNIT_ASSERT(d.matrix(1)[1][0]==Interval(1,6));
	CPPUNIT_ASSERT(d.matrix(1)[1][1]==Interval(1,7));
	CPPUNIT_ASSERT(d.matrix(1)[1][2]==Interval(1,8));

	CPPUNIT_ASSERT(d.vector(2)[0]==Interval(1,9));
	CPPUNIT_ASSERT(d.vector(2)[1]==Interval(1,10));
	CPPUNIT_ASSERT(d.vector(2)[2]==Interval(1,11));

	IntervalVector v2(10);
	v2=d;
	for (int i=0; i<10; i++)
		CPPUNIT_ASSERT(v2[i]==Interval(1,i+2));
}
*/
} // namespace ibex
