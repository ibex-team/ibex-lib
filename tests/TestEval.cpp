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
#include "ibex_BasicDecorator.h"

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
	TEST_ASSERT(e.deco!=NULL);
	TEST_ASSERT((dynamic_cast<Domain*>(e.deco))!=NULL);
	Domain* dx = (Domain*) e.deco;
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
	Function f;
	const ExprSymbol& x = f.add_symbol("x");
	const ExprSymbol& y = f.add_symbol("y");
	const ExprNode&   e = x+y;
	f.set_expr(e);

	BasicDecorator d;
	d.decorate(f);
	check_deco(x);
	check_deco(y);
	check_deco(e);
}

void TestEval::deco02() {
	Function f;
	const ExprSymbol& x = f.add_symbol("x",Dim(0,3,0));
	const ExprSymbol& y = f.add_symbol("y",Dim(0,3,0));
	const ExprNode&   e = x+y;
	f.set_expr(e);

	BasicDecorator d;
	d.decorate(f);
	check_deco(x);
	check_deco(y);
	check_deco(e);
}

void TestEval::add01() {

	Function f;
	const ExprSymbol& x = f.add_symbol("x");
	const ExprSymbol& y = f.add_symbol("y");
	f.set_expr(x+y);

	Eval e(f);

	IntervalVector box(2);
	box[0]=Interval(1,2);
	box[1]=Interval(3,4);

	Interval res=e.eval_scalar(box);
	//cout << e.f << endl;
	TEST_ASSERT(res==Interval(4,6));
}

void TestEval::add02() {

	Function f;
	const ExprSymbol& x = f.add_symbol("x",Dim(0,3,0));
	const ExprSymbol& y = f.add_symbol("y",Dim(0,3,0));
	f.set_expr(x+y);

	Eval e(f);

	double _xy[][2] = { {1,2}, {3,4}, {5,6} ,
						 {1,1}, {2,2}, {3,3} };
	IntervalVector xy(6,_xy);

	double _z[][2] = { {2,3}, {5,6}, {8,9} };
	IntervalVector z(3,_z);

	IntervalVector res=e.eval_vector(xy);
	//cout << e.f << endl;
	TEST_ASSERT(res==z);
}

void TestEval::add03() {

	Function f;
	const ExprSymbol& x = f.add_symbol("x",Dim(0,0,3));
	const ExprSymbol& y = f.add_symbol("y",Dim(0,0,3));
	f.set_expr(x+y);

	Eval e(f);

	double _xy[][2] = { {1,2}, {3,4}, {5,6} ,
						 {1,1}, {2,2}, {3,3} };
	IntervalVector xy(6,_xy);

	double _z[][2] = { {2,3}, {5,6}, {8,9} };
	IntervalVector z(3,_z);

	IntervalVector res=e.eval_vector(xy);
	//cout << e.f << endl;
	TEST_ASSERT(res==z);
}

void TestEval::add04() {

	Function f;
	const ExprSymbol& x = f.add_symbol("x",Dim(0,2,3));
	const ExprSymbol& y = f.add_symbol("y",Dim(0,2,3));
	f.set_expr(x+y);

	Eval e(f);

	double _xy[][2] = { {1,2}, {3,4}, {5,6},
						{1,1}, {2,2}, {3,3},
						{1,1}, {2,2}, {3,3},
						{1,2}, {3,4}, {5,6} };
	IntervalVector xy(12,_xy);

	double _z[][2] = { {2,3}, {5,6}, {8,9},
			           {2,3}, {5,6}, {8,9} };
	IntervalMatrix z(2, 3,_z);

	IntervalMatrix res=e.eval_matrix(xy);
	//cout << e.f << endl;
	TEST_ASSERT(res==z);
}

void TestEval::mul01() {

	Function f;
	const ExprSymbol& x = f.add_symbol("x",Dim(0,2,3));
	const ExprSymbol& y = f.add_symbol("y",Dim(0,3,2));
	f.set_expr(x*y);

	Eval e(f);


	IntervalVector xy(12);
	IntervalMatrix mx=M1();
	IntervalMatrix my=M2();
	for (int i=0; i<6; i++) xy[i]=mx[i/3][i%3];
	for (int i=0; i<6; i++) xy[i+6]=my[i/2][i%2];

	IntervalMatrix mz=mx*my;

	IntervalMatrix res=e.eval_matrix(xy);
	//cout << e.f << endl;
	TEST_ASSERT(res==mz);
}


void TestEval::dist01() {

	Function f;
	const ExprSymbol& xa = f.add_symbol("xa");
	const ExprSymbol& ya = f.add_symbol("ya");
	const ExprSymbol& xb = f.add_symbol("xb");
	const ExprSymbol& yb = f.add_symbol("yb");

	f.set_expr(sqrt(sqr(xa-ya)+sqr(xb-yb)));

	Eval e(f);

	double _xy[][2] = { {3,3}, {4,4},
						{4,4}, {5,5} };
	IntervalVector box(4,_xy);

	Interval res=e.eval_scalar(box);
	//cout << e.f << endl;
	check(res,Interval(::sqrt(2),::sqrt(2)));
}

} // end namespace

