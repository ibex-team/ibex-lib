/* ============================================================================
 * I B E X - Function Tests
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Mar 23, 2012
 * ---------------------------------------------------------------------------- */

#include "TestFunction.h"
#include "ibex_Function.h"
#include "ibex_NumConstraint.h"
#include "ibex_Expr.h"
#include "ibex_SyntaxError.h"
#include <sstream>
#include <cstdio>

// fmemopen doesn't exist on no POSIX system
// The function is defined here
#if defined(_MSC_VER) || defined(__clang__)
inline FILE* fmemopen(void* data, int len, const char *mode ){
       FILE * tempfile = tmpfile();
       fwrite(data, len, 1, tempfile);
       rewind(tempfile);
       return tempfile;
}
#endif

using namespace std;

namespace ibex {

void TestFunction::setUp() {
	  symbol_01     = new Function("x","x");
	  symbol_02     = new Function("x","y", "x");
	  symbol_vec_01 = new Function("x[2]","x");
	  symbol_vec_02 = new Function("x[1][2]","x");
	  symbol_mat_01 = new Function("x[2][3]","x");
	  add_scalar    = new Function("x","y","x+y");
	  add_vector_01 = new Function("x[3]","y[3]","x+y");
	  add_vector_02 = new Function("x[1][3]","y[1][3]","x+y");
	  add_mat       = new Function("x[2][3]","y[2][3]","x+y");
	  mul_mat       = new Function("x[2][3]","y[3][2]","x*y");
	  row_vec       = new Function("x","y[1][3]","(x,y,x)");
	  col_vec       = new Function("x","y[3]","(x;y;x)");
}

void TestFunction::tearDown() {
	delete symbol_01;
	delete symbol_02;
	delete symbol_vec_01;
	delete add_scalar;
}

bool TestFunction::test_evl(Function* f, const Interval& x_in, const Interval& y_out) {
	return almost_eq(f->eval(IntervalVector(1,x_in)),y_out, ERROR);
}

bool TestFunction::test_evl(Function* f, const Interval& x1_in, const Interval& x2_in, const Interval& y_out) {
	IntervalVector x(2);
	x[0]=x1_in;
	x[1]=x2_in;
	return almost_eq(f->eval(x),y_out, ERROR);
}
// ===========================================================================
//                            PARSER
// ===========================================================================

void TestFunction::parser_symbol_01() {
	CPPUNIT_ASSERT(sameExpr(symbol_01->expr(),"x"));
}

void TestFunction::parser_symbol_02() {
	CPPUNIT_ASSERT(sameExpr(symbol_02->expr(),"x"));
}

void TestFunction::parser_symbol_vec_01() {
	CPPUNIT_ASSERT(sameExpr(symbol_vec_01->expr(),"x"));
}

void TestFunction::parser_symbol_vec_02() {
	CPPUNIT_ASSERT(sameExpr(symbol_vec_02->expr(),"x"));
}

void TestFunction::parser_symbol_mat_01() {
	CPPUNIT_ASSERT(sameExpr(symbol_mat_01->expr(),"x"));
}

void TestFunction::parser_add_scalar() {
	CPPUNIT_ASSERT(sameExpr(add_scalar->expr(),"(x+y)"));
}

void TestFunction::parser_add_vector_01() {
	CPPUNIT_ASSERT(sameExpr(add_vector_01->expr(),"(x+y)"));
}

void TestFunction::parser_add_vector_02() {
	CPPUNIT_ASSERT(sameExpr(add_vector_02->expr(),"(x+y)"));
}

void TestFunction::parser_add_mat() {
	CPPUNIT_ASSERT(sameExpr(add_mat->expr(),"(x+y)"));
}

void TestFunction::parser_mul_mat() {
	CPPUNIT_ASSERT(sameExpr(mul_mat->expr(),"(x*y)"));
}

void TestFunction::parser_row_vec() {
	CPPUNIT_ASSERT(sameExpr(row_vec->expr(),"(x,y,x)"));
}

void TestFunction::parser_col_vec() {
	CPPUNIT_ASSERT(sameExpr(col_vec->expr(),"(x;y;x)"));
}
// ===========================================================================
//                            EVAL
// ===========================================================================


void TestFunction::eval_symbol_01() {
	CPPUNIT_ASSERT(test_evl(symbol_01,1,1));
}

void TestFunction::eval_symbol_02() {
	CPPUNIT_ASSERT(test_evl(symbol_01,1,2,1));
}

void TestFunction::eval_symbol_vec_01() {
	IntervalVector x(3);
	x[0]=1;
	x[1]=2;
	x[2]=1;
	CPPUNIT_ASSERT(almost_eq(symbol_vec_01->eval_vector(x),x,0));
}

void TestFunction::eval_symbol_vec_02() {
	IntervalVector x(3);
	x[0]=1;
	x[1]=2;
	x[2]=1;
	CPPUNIT_ASSERT(almost_eq(symbol_vec_02->eval_vector(x),x,0));
}

void TestFunction::eval_symbol_mat_01() {
	double _x[][2] = { {1,2}, {3,4}, {5,6} ,
			           {1,1}, {2,2}, {3,3} };
	IntervalMatrix xout(2,3,_x);
	IntervalVector xin(6,_x);
	CPPUNIT_ASSERT(almost_eq(symbol_mat_01->eval_matrix(xin),xout,0));
}

void TestFunction::eval_add_scalar() {
	CPPUNIT_ASSERT(test_evl(add_scalar,1,2,3));
	CPPUNIT_ASSERT(test_evl(add_scalar,Interval(1,2),Interval(3,4),Interval(4,6)));
}

void TestFunction::eval_add_vector_01() {
	double _xy[][2] = { {1,2}, {3,4}, {5,6} ,
			            {1,1}, {2,2}, {3,3} };
	IntervalVector xy(6,_xy);

	double _z[][2] = { {2,3}, {5,6}, {8,9} };
	IntervalVector z(3,_z);

	IntervalVector res=add_vector_01->eval_vector(xy);
	CPPUNIT_ASSERT(almost_eq(res,z,ERROR));
	CPPUNIT_ASSERT(res.is_superset(z));
}

void TestFunction::eval_add_vector_02() {
	double _xy[][2] = { {1,2}, {3,4}, {5,6} ,
			            {1,1}, {2,2}, {3,3} };
	IntervalVector xy(6,_xy);

	double _z[][2] = { {2,3}, {5,6}, {8,9} };
	IntervalVector z(3,_z);

	IntervalVector res=add_vector_02->eval_vector(xy);
	CPPUNIT_ASSERT(almost_eq(res,z,ERROR));
	CPPUNIT_ASSERT(res.is_superset(z));
}

void TestFunction::eval_add_mat() {
	double _xy[][2] = { {1,2}, {3,4}, {5,6},
			{1,1}, {2,2}, {3,3},
			{1,1}, {2,2}, {3,3},
			{1,2}, {3,4}, {5,6} };
	IntervalVector xy(12,_xy);

	double _z[][2] = { {2,3}, {5,6}, {8,9},
			{2,3}, {5,6}, {8,9} };
	IntervalMatrix z(2, 3,_z);

	IntervalMatrix res=add_mat->eval_matrix(xy);

	CPPUNIT_ASSERT(almost_eq(res[0],z[0],ERROR));
	CPPUNIT_ASSERT(almost_eq(res[1],z[1],ERROR));
	CPPUNIT_ASSERT(res.is_superset(z));
}


void TestFunction::eval_mul_mat() {
	double _x[][2]={{0,1},{0,2},{0,3},
					{-1,0},{-2,0},{-3,0}};
	IntervalMatrix x(2,3,_x);

	double _y[][2]={{0,1},{-1,0},
			        {0,2},{-2,0},
					{0,3},{-3,0}};
	IntervalMatrix y(3,2,_y);

	IntervalVector xy(12);

	for (int i=0; i<6; i++) xy[i]=x[i/3][i%3];
	for (int i=0; i<6; i++) xy[i+6]=y[i/2][i%2];

	IntervalMatrix z=x*y;

	IntervalMatrix res=mul_mat->eval_matrix(xy);

	CPPUNIT_ASSERT(almost_eq(res,z,ERROR));
	CPPUNIT_ASSERT(res.is_superset(z));
}

void TestFunction::eval_row_vec() {
	IntervalVector x(1,Interval(1,2));
	double _y[][2] = { {3,4}, {5,6}, {7,8} };
	IntervalVector y(3,_y);

	IntervalVector xy = cart_prod(x,y);
	IntervalVector z  = cart_prod(xy,x);
	IntervalVector res=row_vec->eval_vector(xy);

	CPPUNIT_ASSERT(almost_eq(res,z,0));
}

void TestFunction::eval_col_vec() {
	IntervalVector x(1,Interval(1,2));
	double _y[][2] = { {3,4}, {5,6}, {7,8} };
	IntervalVector y(3,_y);

	IntervalVector xy = cart_prod(x,y);
	IntervalVector z  = cart_prod(xy,x);
	IntervalVector res=row_vec->eval_vector(xy);

	CPPUNIT_ASSERT(almost_eq(res,z,0));
}

void TestFunction::build01() {
	delete new Function();
	CPPUNIT_ASSERT(true);
}

void TestFunction::build02() {
	const char* x[3]={"x","y","z" };
	Function f(3, x,"(x+y)+z");
	CPPUNIT_ASSERT(strcmp(f.arg_name(0),x[0])==0);
	CPPUNIT_ASSERT(strcmp(f.arg_name(1),x[1])==0);
	CPPUNIT_ASSERT(strcmp(f.arg_name(2),x[2])==0);
	CPPUNIT_ASSERT(sameExpr(f.expr(),"((x+y)+z)"));
}

void TestFunction::add_symbol() {
	const ExprSymbol& x=ExprSymbol::new_("x");
	const ExprSymbol& y=ExprSymbol::new_("y");

	Function f(x,y,x);

	CPPUNIT_ASSERT(f.nb_nodes()==2);
	CPPUNIT_ASSERT(f.nb_arg()==2);
	CPPUNIT_ASSERT(strcmp(f.arg_name(0),"x")==0);
	CPPUNIT_ASSERT(strcmp(f.arg_name(1),"y")==0);

	vector<int>::const_iterator it=f.used_vars.begin();
	assert(it!=f.used_vars.end() && *it==0);
	it++;
	assert(it==f.used_vars.end());

//	CPPUNIT_ASSERT(f.used_vars[0]);
//	CPPUNIT_ASSERT(!f.used_vars[1]);

	CPPUNIT_ASSERT(&f.node(0)==&x);
	CPPUNIT_ASSERT(&f.node(1)==&y);
}

void TestFunction::copy() {
	const ExprSymbol& x=ExprSymbol::new_("x",Dim::col_vec(3));
	const ExprSymbol& A=ExprSymbol::new_("A",Dim::matrix(2,3));

	Function f(x,A,A*x);

	Function f2(f);
	CPPUNIT_ASSERT(f2.nb_nodes()==3);
	CPPUNIT_ASSERT(f2.nb_arg()==2);

	const ExprMul* e2=dynamic_cast<const ExprMul*>(&f2.node(0));
	CPPUNIT_ASSERT(e2!=NULL);
	CPPUNIT_ASSERT(sameExpr(f2.expr(),"(A*x)"));

	const ExprSymbol* A2=dynamic_cast<const ExprSymbol*>(&f2.node(1));
	CPPUNIT_ASSERT(A2!=NULL);
	CPPUNIT_ASSERT(strcmp(A2->name,"A")==0);
	CPPUNIT_ASSERT(A2->dim==A.dim);

	const ExprSymbol* x2=dynamic_cast<const ExprSymbol*>(&f2.node(2));
	CPPUNIT_ASSERT(strcmp(x2->name,"x")==0);
	CPPUNIT_ASSERT(x2->dim==x.dim);

}

void TestFunction::generate_comp01() {
	const ExprSymbol& x=ExprSymbol::new_("x");
	const ExprSymbol& y=ExprSymbol::new_("y");
	const ExprSymbol& z=ExprSymbol::new_("z");

	const ExprNode& e1=x+y-z;
	const ExprNode& e2=x*z;
	const ExprNode& e3=y-z;
	Array<const ExprNode> v(e1, e2, e3);

	const ExprVector& e=ExprVector::new_col(v);

	Function f(x,y,z,e);

	CPPUNIT_ASSERT(f[0].nb_arg()==3);
	CPPUNIT_ASSERT(sameExpr(f[0].expr(),"((x+y)-z)"));

	CPPUNIT_ASSERT(f[1].nb_arg()==3);
	CPPUNIT_ASSERT(sameExpr(f[1].expr(),"(x*z)"));

	CPPUNIT_ASSERT(f[2].nb_arg()==3);
	CPPUNIT_ASSERT(sameExpr(f[2].expr(),"(y-z)"));
}

void TestFunction::generate_comp02() {
	const ExprSymbol& x=ExprSymbol::new_("x");

	const ExprNode& e1=x;
	const ExprNode& e2=ExprConstant::new_scalar(0);
	const ExprNode& e3=ExprConstant::new_scalar(0);

	Array<const ExprNode> v(e1, e2, e3);

	const ExprVector& e=ExprVector::new_col(v);

	Function f(x,e);

	const ExprConstant* c=dynamic_cast<const ExprConstant*>(&f[1].expr());
	CPPUNIT_ASSERT(c);
	CPPUNIT_ASSERT(c->get_value()==Interval::ZERO);
	c=dynamic_cast<const ExprConstant*>(&f[2].expr());
	CPPUNIT_ASSERT(c);
	CPPUNIT_ASSERT(c->get_value()==Interval::ZERO);
}

void TestFunction::used() {
	const ExprSymbol& x=ExprSymbol::new_("x");
	const ExprSymbol& y=ExprSymbol::new_("y");
	const ExprSymbol& z=ExprSymbol::new_("z");

	const ExprNode& e1=x+y-z;
	const ExprNode& e2=x*z;
	const ExprNode& e3=y-z;
	Array<const ExprNode> v(e1, e2, e3);

	const ExprVector& e=ExprVector::new_col(v);

	Function f(x,y,z,e);

	vector<int>::const_iterator it=f[0].used_vars.begin();
	assert(it!=f[0].used_vars.end() && *it==0);
	it++;
	assert(it!=f[0].used_vars.end() && *it==1);
	it++;
	assert(it!=f[0].used_vars.end() && *it==2);
	it++;
	assert(it==f[0].used_vars.end());

//	CPPUNIT_ASSERT(f[0].used_vars.size()==3);
//	CPPUNIT_ASSERT(f[0].used_vars[0]);
//	CPPUNIT_ASSERT(f[0].used_vars[1]);
//	CPPUNIT_ASSERT(f[0].used_vars[2]);

	it=f[1].used_vars.begin();
	assert(it!=f[1].used_vars.end() && *it==0);
	it++;
	assert(it!=f[1].used_vars.end() && *it==2);
	it++;
	assert(it==f[1].used_vars.end());

//	CPPUNIT_ASSERT(f[1].used_vars.size()==2);
//	CPPUNIT_ASSERT(f[1].used_vars[0]);
//	CPPUNIT_ASSERT(f[1].used_vars[2]);

	it=f[2].used_vars.begin();
	assert(it!=f[2].used_vars.end() && *it==1);
	it++;
	assert(it!=f[2].used_vars.end() && *it==2);
	it++;
	assert(it==f[2].used_vars.end());

//	CPPUNIT_ASSERT(f[2].used_vars.size()==2);
//	CPPUNIT_ASSERT(f[2].used_vars[1]);
//	CPPUNIT_ASSERT(f[2].used_vars[2]);
}

void TestFunction::used02() {
	Variable x(3,"x");
	Variable y(2,3,"y");

	Function f(x,y,x[0]+x[2]-(y[1][1]*y[1][2]));

	vector<int>::const_iterator it=f.used_vars.begin();
	assert(it!=f.used_vars.end() && *it==0);
	it++;
	assert(it!=f.used_vars.end() && *it==2);
	it++;
	assert(it!=f.used_vars.end() && *it==7);
	it++;
	assert(it!=f.used_vars.end() && *it==8);
	it++;
	assert(it==f.used_vars.end());

//	CPPUNIT_ASSERT(f.used_vars[0]);
//	CPPUNIT_ASSERT(!f.used_vars[1]);
//	CPPUNIT_ASSERT(f.used_vars[2]);
//
//	CPPUNIT_ASSERT(!f.used_vars[3]);
//	CPPUNIT_ASSERT(!f.used_vars[4]);
//	CPPUNIT_ASSERT(!f.used_vars[5]);
//
//	CPPUNIT_ASSERT(!f.used_vars[6]);
//	CPPUNIT_ASSERT(f.used_vars[7]);
//	CPPUNIT_ASSERT(f.used_vars[8]);

}

void TestFunction::numctr01() {
	Variable x("x");
	Variable y("y");
	Function f(x,y,x+y);
	NumConstraint c(f,EQ);
	CPPUNIT_ASSERT(true); // TO DO
}

void TestFunction::apply01() {
	Variable x("x");
	Variable y("y");
	Function f(x,y,x+y,"f");
	Interval c(1,1);
	Variable z("z");
	Function f2(z,f(z,c));
	CPPUNIT_ASSERT(sameExpr(f2.expr(),"(z+1)"));
}

void TestFunction::from_string01() {
	try {
		Function f("x","sin(x)");
		CPPUNIT_ASSERT(f.nb_arg()==1);
		CPPUNIT_ASSERT(sameExpr(f.expr(),"sin(x)"));
	}
	catch(SyntaxError& e) {
		CPPUNIT_ASSERT(false);
	}
}
void TestFunction::from_string02() {
	try {
		Function f("x","y","x+y");

		CPPUNIT_ASSERT(f.nb_arg()==2);
		CPPUNIT_ASSERT(sameExpr(f.expr(),"(x+y)"));
	} catch(SyntaxError& e) {
		CPPUNIT_ASSERT(false);
	}
}

void TestFunction::from_string03() {
	try {
		Function f("x[2]","y[1][3]","x(1)+y(2)");

		CPPUNIT_ASSERT(f.nb_arg()==2);
		CPPUNIT_ASSERT(f.arg(0).dim.nb_rows()==2);
		CPPUNIT_ASSERT(f.arg(1).dim.nb_cols()==3);
		CPPUNIT_ASSERT(sameExpr(f.expr(),"(x(1)+y(2))"));
	} catch(SyntaxError& e) {
		CPPUNIT_ASSERT(false);
	}
}

void TestFunction::from_string04() {
	try {
		Function f("a","b","c","d","e","f","g","h","a+e");

		CPPUNIT_ASSERT(f.nb_arg()==8);
		CPPUNIT_ASSERT(sameExpr(f.expr(),"(a+e)"));
	} catch(SyntaxError& e) {
		CPPUNIT_ASSERT(false);
	}
}

void TestFunction::minibex01() {
	Variable x("x"),y("y");
	const ExprNode& e1=x+y;
	const ExprNode& e=e1+e1;
	Function f(x,y,e,"f");
	std::string m = f.minibex();
	char* d=strdup(m.c_str());
	CPPUNIT_ASSERT(strcmp(m.c_str(),"function f(x,y)\n  _tmp_0_ = (x+y);\n  return (_tmp_0_+_tmp_0_);\nend")==0);
}

void TestFunction::minibex02() {
	Variable x("x"),y("y");
	const ExprNode& e1=x+y;
	const ExprNode& e=e1+e1;
	Function f(x,y,e,"f");
	std::string m = f.minibex();

	FILE *fin = fmemopen((void*) m.c_str(), m.length(), "r");
	Function f2(fin);

	m = f2.minibex();
	CPPUNIT_ASSERT(strcmp(m.c_str(),"function f(x,y)\n  _tmp_0_ = (x+y);\n  return (_tmp_0_+_tmp_0_);\nend")==0);
}

void TestFunction::minibex03() {
	Variable x(Dim::matrix(2,3),"x");
	double _m[][2] = {{0,0},{1,1},{2,2},{3,3},{4,4},{5,5}};
	IntervalMatrix M(2,3,_m);
	Function f(x,x+M,"f");
	std::string m = f.minibex();
	CPPUNIT_ASSERT(strcmp(m.c_str(),"function f(x[2][3])\n  return (x+((0 , 1 , 2) ; (3 , 4 , 5)));\nend")==0);

	FILE *fin = fmemopen((void*) m.c_str(), m.length(), "r");
	Function f2(fin);

	m = f2.minibex();
	CPPUNIT_ASSERT(strcmp(m.c_str(),"function f(x[2][3])\n  return (x+((0 , 1 , 2) ; (3 , 4 , 5)));\nend")==0);
}

void TestFunction::issue43() {
	Variable x("x"),y("y");
	Function f(x,y,x);
	Function g(x,y,y);
}

void TestFunction::issue43_bis() {
	Variable x,y;
	Function f(x,y,x);
	Function g(x,y,f(x,y));
}

} // end namespace
