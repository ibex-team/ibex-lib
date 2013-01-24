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

void TestEval::check_deco(const ExprNode& e) {
	TEST_ASSERT(e.deco.d!=NULL);
	Domain* dx = (Domain*) e.deco.d;
	TEST_ASSERT(dx->dim==e.dim);
	switch (e.dim.type()) {
	case Dim::SCALAR:
		dx->i()=Interval(1,2); // try to write
		TEST_ASSERT(dx->i()==Interval(1,2)); // try to read
		break;
	case Dim::ROW_VECTOR:
		{ IntervalVector box(e.dim.dim3);
		for (int i=0; i<e.dim.dim3; i++) box[i]=Interval(i,i+1);
		dx->v()=box; // try to write
		TEST_ASSERT(dx->v()==box); // try to read
		}
		break;
	case Dim::COL_VECTOR:
		{ IntervalVector box(e.dim.dim2);
		for (int i=0; i<e.dim.dim2; i++) box[i]=Interval(i,i+1);
		dx->v()=box; // try to write
		TEST_ASSERT(dx->v()==box); // try to read
		}
		break;
	case Dim::MATRIX:
		{ IntervalMatrix m(e.dim.dim2,e.dim.dim3);
		for (int i=0; i<e.dim.dim2; i++)
			for (int j=0; j<e.dim.dim3; j++)
				m[i][j]=Interval(i,j);
		dx->m()=m; // try to write
		TEST_ASSERT(dx->m()==m); // try to read
		}
		break;
	case Dim::MATRIX_ARRAY:
		{ IntervalMatrixArray ma(e.dim.dim1, e.dim.dim2,e.dim.dim3);
		for (int k=0; k<e.dim.dim1; k++)
			for (int i=0; i<e.dim.dim2; i++)
				for (int j=0; j<e.dim.dim3; j++)
					ma[k][i][j]=Interval(i,j);
		dx->ma()=ma; // try to write
		for (int k=0; k<e.dim.dim1; k++)
			TEST_ASSERT(dx->ma()[k]==ma[k]); // try to read
		}
		break;
	default:
		TEST_ASSERT(false);
		break;
	}

}

void TestEval::deco01() {

	const ExprSymbol& x = ExprSymbol::new_("x");
	const ExprSymbol& y = ExprSymbol::new_("y");
	const ExprNode&   e = x+y;
	Function f(x,y,e);
	check_deco(x);
	check_deco(y);
	check_deco(e);
}

void TestEval::deco02() {
	const ExprSymbol& x = ExprSymbol::new_("x",Dim::col_vec(3));
	const ExprSymbol& y = ExprSymbol::new_("y",Dim::col_vec(3));
	const ExprNode&   e = x+y;
	Function f(x,y,e);

	check_deco(x);
	check_deco(y);
	check_deco(e);
}

void TestEval::add01() {

	const ExprSymbol& x = ExprSymbol::new_("x");
	const ExprSymbol& y = ExprSymbol::new_("y");
	Function f(x,y,x+y);

	IntervalVector box(2);
	box[0]=Interval(1,2);
	box[1]=Interval(3,4);

	Interval res=f.eval(box);
	//e.f.cf.print<Domain>();
	//cout << "res=" << res << endl;
	TEST_ASSERT(res==Interval(4,6));
}

void TestEval::add02() {

	const ExprSymbol& x = ExprSymbol::new_("x",Dim::col_vec(3));
	const ExprSymbol& y = ExprSymbol::new_("y",Dim::col_vec(3));
	Function f(x,y,x+y);

	double _xy[][2] = { {1,2}, {3,4}, {5,6} ,
						 {1,1}, {2,2}, {3,3} };
	IntervalVector xy(6,_xy);

	double _z[][2] = { {2,3}, {5,6}, {8,9} };
	IntervalVector z(3,_z);

	IntervalVector res=f.eval_vector(xy);
	//cout << e.f << endl;
	TEST_ASSERT(res==z);
}

void TestEval::add03() {

	const ExprSymbol& x = ExprSymbol::new_("x",Dim::row_vec(3));
	const ExprSymbol& y = ExprSymbol::new_("y",Dim::row_vec(3));
	Function f(x,y,x+y);

	double _xy[][2] = { {1,2}, {3,4}, {5,6} ,
						 {1,1}, {2,2}, {3,3} };
	IntervalVector xy(6,_xy);

	double _z[][2] = { {2,3}, {5,6}, {8,9} };
	IntervalVector z(3,_z);

	IntervalVector res=f.eval_vector(xy);
	//cout << e.f << endl;
	TEST_ASSERT(res==z);
}

void TestEval::add04() {

	const ExprSymbol& x = ExprSymbol::new_("x",Dim::matrix(2,3));
	const ExprSymbol& y = ExprSymbol::new_("y",Dim::matrix(2,3));
	Function f(x,y,x+y);

	double _xy[][2] = { {1,2}, {3,4}, {5,6},
						{1,1}, {2,2}, {3,3},
						{1,1}, {2,2}, {3,3},
						{1,2}, {3,4}, {5,6} };
	IntervalVector xy(12,_xy);

	double _z[][2] = { {2,3}, {5,6}, {8,9},
			           {2,3}, {5,6}, {8,9} };
	IntervalMatrix z(2, 3,_z);

	IntervalMatrix res=f.eval_matrix(xy);
	//cout << e.f << endl;
	TEST_ASSERT(res==z);
}

void TestEval::mul01() {

	const ExprSymbol& x = ExprSymbol::new_("x",Dim::matrix(2,3));
	const ExprSymbol& y = ExprSymbol::new_("y",Dim::matrix(3,2));
	Function f(x,y,x*y);

	IntervalVector xy(12);
	IntervalMatrix mx=M1();
	IntervalMatrix my=M2();
	for (int i=0; i<6; i++) xy[i]=mx[i/3][i%3];
	for (int i=0; i<6; i++) xy[i+6]=my[i/2][i%2];

	IntervalMatrix mz=mx*my;

	IntervalMatrix res=f.eval_matrix(xy);
	//cout << e.f << endl;
	TEST_ASSERT(res==mz);
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
}

void TestEval::apply01() {

	const ExprSymbol& x1 = ExprSymbol::new_("x1");
	const ExprSymbol& x2 = ExprSymbol::new_("x2");

	Function f1(x1,x1,"f1");

	Function f2(x2,f1(x2));

	IntervalVector _x2(1,Interval(2,2));
	check(f2.eval(_x2), Interval(2,2));
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
}

}
