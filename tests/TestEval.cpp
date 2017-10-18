/* ============================================================================
 * I B E X - Eval Tests
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Apr 02, 2012
 * ---------------------------------------------------------------------------- */

#include "TestEval.h"
#include "ibex_Function.h"
#include "ibex_Expr.h"
#include "ibex_Eval.h"

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

}

namespace ibex {

void TestEval::check_deco(Function& f, const ExprNode& e) {
	Domain& dx = f.basic_evaluator().d[f.nodes.rank(e)];
	CPPUNIT_ASSERT(dx.dim==e.dim);
	switch (e.dim.type()) {
	case Dim::SCALAR:
		dx.i()=Interval(1,2); // try to write
		CPPUNIT_ASSERT(dx.i()==Interval(1,2)); // try to read
		break;
	case Dim::ROW_VECTOR:
		{ IntervalVector box(e.dim.nb_cols());
		for (int i=0; i<e.dim.nb_cols(); i++) box[i]=Interval(i,i+1);
		dx.v()=box; // try to write
		CPPUNIT_ASSERT(dx.v()==box); // try to read
		}
		break;
	case Dim::COL_VECTOR:
		{ IntervalVector box(e.dim.nb_rows());
		for (int i=0; i<e.dim.nb_rows(); i++) box[i]=Interval(i,i+1);
		dx.v()=box; // try to write
		CPPUNIT_ASSERT(dx.v()==box); // try to read
		}
		break;
	case Dim::MATRIX:
		{ IntervalMatrix m(e.dim.nb_rows(),e.dim.nb_cols());
		for (int i=0; i<e.dim.nb_rows(); i++)
			for (int j=0; j<e.dim.nb_cols(); j++)
				m[i][j]=Interval(i,j);
		dx.m()=m; // try to write
		CPPUNIT_ASSERT(dx.m()==m); // try to read
		}
		break;
	default:
		CPPUNIT_ASSERT(false);
		break;
	}

}

void TestEval::deco01() {

	const ExprSymbol& x = ExprSymbol::new_("x");
	const ExprSymbol& y = ExprSymbol::new_("y");
	const ExprNode&   e = x+y;
	Function f(x,y,e);
	check_deco(f, x);
	check_deco(f, y);
	check_deco(f, e);
}

void TestEval::deco02() {
	const ExprSymbol& x = ExprSymbol::new_("x",Dim::col_vec(3));
	const ExprSymbol& y = ExprSymbol::new_("y",Dim::col_vec(3));
	const ExprNode&   e = x+y;
	Function f(x,y,e);

	check_deco(f, x);
	check_deco(f, y);
	check_deco(f, e);
}


void TestEval::dist01() {

	const ExprSymbol& xa = ExprSymbol::new_("xa");
	const ExprSymbol& ya = ExprSymbol::new_("ya");
	const ExprSymbol& xb = ExprSymbol::new_("xb");
	const ExprSymbol& yb = ExprSymbol::new_("yb");

	const ExprSymbol* args[4]={&xa, &ya, &xb, &yb};
	Function f(Array<const ExprSymbol>(args,4),sqrt(sqr(xa-ya)+sqr(xb-yb)));

	double _xy[][2] = { {3,3}, {4,4},
						{4,4}, {5,5} };
	IntervalVector box(4,_xy);

	Interval res=f.eval(box);
	//cout << e.f << endl;
	check(res,Interval(::sqrt(2),::sqrt(2)));
	CPPUNIT_ASSERT(res.is_superset(Interval(::sqrt(2),::sqrt(2))));
}

void TestEval::apply01() {

	const ExprSymbol& x1 = ExprSymbol::new_("x1");
	const ExprSymbol& x2 = ExprSymbol::new_("x2");

	Function f1(x1,x1,"f1");

	Function f2(x2,f1(x2));

	IntervalVector _x2(1,Interval(2,2));
	check(f2.eval(_x2), Interval(2,2));
	CPPUNIT_ASSERT((f2.eval(_x2)).is_superset(Interval(2,2)));
}

void TestEval::apply02() {

	const ExprSymbol& x1 = ExprSymbol::new_("x1");
	const ExprSymbol& y1 = ExprSymbol::new_("y1");

	const ExprSymbol& x2 = ExprSymbol::new_("x2");
	const ExprSymbol& y2 = ExprSymbol::new_("y2");

	Function f1(x1,y1,x1+y1,"f1");
	Function f2(x2,y2,f1(x2,x2+y2)+y2,"f2");

	//cout << f1 << endl;
	//cout << f2 << endl;

	IntervalVector x(2);
	x[0]=Interval(2,2);
	x[1]=Interval(3,3);

	check(f2.eval_domain(x).i(), Interval(10,10));
	CPPUNIT_ASSERT((f2.eval_domain(x).i()).is_superset(Interval(10,10)));
}

void TestEval::apply03() {

	const ExprSymbol& x1 = ExprSymbol::new_("x1");
	const ExprSymbol& y1 = ExprSymbol::new_("y1");

	const ExprSymbol& x2 = ExprSymbol::new_("x2");
	const ExprSymbol& y2 = ExprSymbol::new_("y2");

	const ExprSymbol& x3 = ExprSymbol::new_("x3");

	Function f1(x1,y1,x1+y1);
	Function f2(x2,y2,x2*y2);
	Function f3(x3,f1(x3,x3)-f2(x3,x3));

	//cout << f3 << endl;
	IntervalVector _x3(1,Interval(3,3));

	/*
	e.eval().i();
	cout << "f1:---------\n";
	f1.cf.print<Domain>();
	cout << "f2:---------\n";
	f2.cf.print<Domain>();
	cout << "f3:---------\n";
	f3.cf.print<Domain>();
	*/

	check(f3.eval_domain(_x3).i(), Interval(-3,-3));
	CPPUNIT_ASSERT((f3.eval_domain(_x3).i()).is_superset(Interval(-3,-3)));
}

void TestEval::apply04() {

	const ExprSymbol& x1 = ExprSymbol::new_("x1");
	const ExprSymbol& x2 = ExprSymbol::new_("x2");
	const ExprSymbol& x3 = ExprSymbol::new_("x3");

	Function f1(x1,sqr(x1));
	Function f2(x2,x2+Interval(1,1));
	Function f3(x3,f2(f1(x3)));

	IntervalVector _x3(1,Interval(3,3));

	check(f3.eval_domain(_x3).i(), Interval(10,10));
	CPPUNIT_ASSERT((f3.eval_domain(_x3).i()).is_superset(Interval(10,10)));
}

void TestEval::issue242() {
	Function f("x[3]","-x");
	IntervalVector x(3,Interval::ONE);
	CPPUNIT_ASSERT(almost_eq(f.eval_vector(x),-x,0));
}

void TestEval::eval_components01() {
	const ExprSymbol& x = ExprSymbol::new_("x");
	const ExprSymbol& y = ExprSymbol::new_("y");
	const ExprSymbol& z = ExprSymbol::new_("z");
	const ExprNode& e1=x+3*y;
	const ExprNode& e2=y-2*x;
	const ExprNode& e3=y+z;
	const ExprNode& e4=e1*e2;
	Function f(x,y,z,Return(e4+1,e2+3,e4-2,e4-4));

	Interval vx=Interval::ONE;
	Interval vy=2*Interval::ONE;
	IntervalVector box(3);
	box[0]=vx;
	box[1]=vy;

	BitSet components=BitSet::empty(4);
	components.add(0);
	components.add(2);

	IntervalVector res=f.eval_vector(box,components);

	CPPUNIT_ASSERT(res.size()==2);
	CPPUNIT_ASSERT(res[0]==(vx+3*vy)*(vy-2*vx)+1);
	CPPUNIT_ASSERT(res[1]==(vx+3*vy)*(vy-2*vx)-2);
}

void TestEval::eval_components02() {
	Dim d=Dim::matrix(3,3);
	const ExprSymbol& x = ExprSymbol::new_("x",Dim::col_vec(2));
	const ExprSymbol& y = ExprSymbol::new_("y",d);
	const ExprSymbol& z = ExprSymbol::new_("z",d);

	Function f(x,y,z,Return(x[1],transpose(y[DoubleIndex::one_row(d,1)]),z[DoubleIndex::one_col(d,2)]));

	IntervalVector box(20);
	for (int i=0; i<20; i++) box[i]=Interval(i,i);

	BitSet components=BitSet::empty(9);
	components.add(0);
	components.add(2);
	components.add(4);
	components.add(6);
	IntervalVector res=f.eval_vector(box,components);

	CPPUNIT_ASSERT(res.size()==4);
	CPPUNIT_ASSERT(res[0]==1);
	CPPUNIT_ASSERT(res[1]==6);
	CPPUNIT_ASSERT(res[2]==13);
	CPPUNIT_ASSERT(res[3]==19);
}

}
