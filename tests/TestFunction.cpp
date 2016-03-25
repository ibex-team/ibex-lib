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

using namespace std;

namespace ibex {

void TestFunction::build01() {
	delete new Function();
	CPPUNIT_ASSERT(true);
}

void TestFunction::add_symbol() {
	const ExprSymbol& x=ExprSymbol::new_("x");
	const ExprSymbol& y=ExprSymbol::new_("y");

	Function f(x,y,x);

	CPPUNIT_ASSERT(f.nb_nodes()==2);
	CPPUNIT_ASSERT(f.nb_arg()==2);
	CPPUNIT_ASSERT(strcmp(f.arg_name(0),"x")==0);
	CPPUNIT_ASSERT(strcmp(f.arg_name(1),"y")==0);
	CPPUNIT_ASSERT(f.used(0));
	CPPUNIT_ASSERT(f.used(0));
	CPPUNIT_ASSERT(!f.used(1));
	CPPUNIT_ASSERT(!f.used(1));
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

	const ExprVector& e=ExprVector::new_(v, false);

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

	const ExprVector& e=ExprVector::new_(v, false);

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

	const ExprVector& e=ExprVector::new_(v, false);

	Function f(x,y,z,e);

	CPPUNIT_ASSERT(f[0].nb_used_vars()==3);
	CPPUNIT_ASSERT(f[0].used_var(0)==0);
	CPPUNIT_ASSERT(f[0].used_var(1)==1);
	CPPUNIT_ASSERT(f[0].used_var(2)==2);

	CPPUNIT_ASSERT(f[1].nb_used_vars()==2);
	CPPUNIT_ASSERT(f[1].used_var(0)==0);
	CPPUNIT_ASSERT(f[1].used_var(1)==2);

	CPPUNIT_ASSERT(f[2].nb_used_vars()==2);
	CPPUNIT_ASSERT(f[2].used_var(0)==1);
	CPPUNIT_ASSERT(f[2].used_var(1)==2);
}

void TestFunction::used02() {
	Variable x(3,"x");
	Variable y(2,3,"y");

	Function f(x,y,x[0]+x[2]-(y[1][1]*y[1][2]));

	CPPUNIT_ASSERT(f.used(0));
	CPPUNIT_ASSERT(!f.used(1));
	CPPUNIT_ASSERT(f.used(2));

	CPPUNIT_ASSERT(!f.used(3));
	CPPUNIT_ASSERT(!f.used(4));
	CPPUNIT_ASSERT(!f.used(5));

	CPPUNIT_ASSERT(!f.used(6));
	CPPUNIT_ASSERT(f.used(7));
	CPPUNIT_ASSERT(f.used(8));

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
	CPPUNIT_ASSERT(sameExpr(f2.expr(),"f(z,1)"));
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
		CPPUNIT_ASSERT(f.arg(0).dim.dim2==2);
		CPPUNIT_ASSERT(f.arg(1).dim.dim3==3);
		CPPUNIT_ASSERT(sameExpr(f.expr(),"(x[0]+y[1])"));
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
