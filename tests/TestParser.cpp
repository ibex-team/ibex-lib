//============================================================================
//                                  I B E X
// File        : TestParser.cpp
// Author      : Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Jun 22, 2012
// Last Update : May 22, 2019
//============================================================================

#include <sstream>

#include "TestParser.h"
#include "ibex_System.h"
#include "ibex_SystemFactory.h"
#include "ibex_SyntaxError.h"
#include "ibex_CtcFwdBwd.h"
#include "ibex_DefaultSolver.h"

#include "Ponts30.h"

#include <cstdio>

using namespace std;

namespace ibex {

static const int simpl = 2;

void TestParser::var01() {
	System sys(SRCDIR_TESTS "/quimper/var01.qpr");
	CPPUNIT_ASSERT(sys.args.size()==1);
	CPPUNIT_ASSERT(strcmp(sys.args[0].name,"x")==0);
	CPPUNIT_ASSERT(sys.args[0].dim.is_scalar());
	CPPUNIT_ASSERT(sys.box.size()==1);
	CPPUNIT_ASSERT(sys.box[0]==Interval::all_reals());
	CPPUNIT_ASSERT(sys.f_ctrs.nb_arg()==1);
	CPPUNIT_ASSERT(&sys.f_ctrs.arg(0) == &sys.args[0]);
	CPPUNIT_ASSERT(sameExpr(sys.f_ctrs.expr(),"x"));
	CPPUNIT_ASSERT(sys.ctrs.size()==1);
	CPPUNIT_ASSERT(sys.ctrs[0].f.nb_arg()==1);
	CPPUNIT_ASSERT(sameExpr(sys.ctrs[0].f.expr(),"x"));
	CPPUNIT_ASSERT(sys.ctrs[0].op==GEQ);
}

void TestParser::const_var_idx() {
	int n=18;
	try {
		System sys(SRCDIR_TESTS "/minibex/const_var_idx.mbx");
		CPPUNIT_ASSERT(sys.args.size()==n);
		int i=0;
		CPPUNIT_ASSERT(strcmp(sys.args[i++].name,"xs")==0);
		CPPUNIT_ASSERT(strcmp(sys.args[i++].name,"xs_1")==0);
		CPPUNIT_ASSERT(strcmp(sys.args[i++].name,"xs_2")==0);
		CPPUNIT_ASSERT(strcmp(sys.args[i++].name,"xs_3")==0);
		CPPUNIT_ASSERT(strcmp(sys.args[i++].name,"xs_4")==0);
		CPPUNIT_ASSERT(strcmp(sys.args[i++].name,"xcv2")==0);
		CPPUNIT_ASSERT(strcmp(sys.args[i++].name,"xcv2_1")==0);
		CPPUNIT_ASSERT(strcmp(sys.args[i++].name,"xcv2_2")==0);
		CPPUNIT_ASSERT(strcmp(sys.args[i++].name,"xcv3")==0);
		CPPUNIT_ASSERT(strcmp(sys.args[i++].name,"xrv3")==0);
		CPPUNIT_ASSERT(strcmp(sys.args[i++].name,"xrv3_1")==0);
		CPPUNIT_ASSERT(strcmp(sys.args[i++].name,"xrv3_2")==0);
		CPPUNIT_ASSERT(strcmp(sys.args[i++].name,"xm22")==0);
		CPPUNIT_ASSERT(strcmp(sys.args[i++].name,"xm23")==0);
		CPPUNIT_ASSERT(strcmp(sys.args[i++].name,"xm23_1")==0);
		CPPUNIT_ASSERT(strcmp(sys.args[i++].name,"xm23_2")==0);
		CPPUNIT_ASSERT(strcmp(sys.args[i++].name,"xm23_3")==0);
		CPPUNIT_ASSERT(strcmp(sys.args[i++].name,"xm32")==0);
		i=0;
		CPPUNIT_ASSERT(sys.args[i++].dim.is_scalar());
		CPPUNIT_ASSERT(sys.args[i++].dim.is_scalar());
		CPPUNIT_ASSERT(sys.args[i++].dim.is_scalar());
		CPPUNIT_ASSERT(sys.args[i++].dim.is_scalar());
		CPPUNIT_ASSERT(sys.args[i++].dim.is_scalar());
		CPPUNIT_ASSERT(sys.args[i++].dim==Dim::col_vec(2));
		CPPUNIT_ASSERT(sys.args[i++].dim==Dim::col_vec(2));
		CPPUNIT_ASSERT(sys.args[i++].dim==Dim::col_vec(2));
		CPPUNIT_ASSERT(sys.args[i++].dim==Dim::col_vec(3));
		CPPUNIT_ASSERT(sys.args[i++].dim==Dim::row_vec(3));
		CPPUNIT_ASSERT(sys.args[i++].dim==Dim::row_vec(3));
		CPPUNIT_ASSERT(sys.args[i++].dim==Dim::row_vec(3));
		CPPUNIT_ASSERT(sys.args[i++].dim==Dim::matrix(2,2));
		CPPUNIT_ASSERT(sys.args[i++].dim==Dim::matrix(2,3));
		CPPUNIT_ASSERT(sys.args[i++].dim==Dim::matrix(2,3));
		CPPUNIT_ASSERT(sys.args[i++].dim==Dim::matrix(2,3));
		CPPUNIT_ASSERT(sys.args[i++].dim==Dim::matrix(2,3));
		CPPUNIT_ASSERT(sys.args[i++].dim==Dim::matrix(3,2));
		i=0;
		CPPUNIT_ASSERT(sys.box[i++]==Interval::all_reals());
		CPPUNIT_ASSERT(sys.box[i++]==Interval(0,1));
		CPPUNIT_ASSERT(sys.box[i++]==Interval::neg_reals());
		CPPUNIT_ASSERT(sys.box[i++]==Interval::pos_reals());
		CPPUNIT_ASSERT(sys.box[i++]==3.14);

		CPPUNIT_ASSERT(sys.box[i++]==Interval::all_reals());
		CPPUNIT_ASSERT(sys.box[i++]==Interval::all_reals());
		CPPUNIT_ASSERT(sys.box[i++]==Interval(0,1));
		CPPUNIT_ASSERT(sys.box[i++]==Interval(0,2));
		CPPUNIT_ASSERT(sys.box[i++]==Interval(0,0));
		CPPUNIT_ASSERT(sys.box[i++]==Interval(1,1));

		// xcv3
		CPPUNIT_ASSERT(sys.box.subvector(i,i+2)==IntervalVector(3,Interval::all_reals()));
		i+=3;

		CPPUNIT_ASSERT(sys.box.subvector(i,i+2)==IntervalVector(3,Interval::all_reals()));
		i+=3;
		CPPUNIT_ASSERT(sys.box[i++]==Interval(0,1));
		CPPUNIT_ASSERT(sys.box[i++]==Interval(0,2));
		CPPUNIT_ASSERT(sys.box[i++]==Interval(0,3));
		CPPUNIT_ASSERT(sys.box[i++]==Interval::zero());
		CPPUNIT_ASSERT(sys.box[i++]==Interval::one());
		CPPUNIT_ASSERT(sys.box[i++]==Interval(2,2));

		// xm22
		CPPUNIT_ASSERT(sys.box.subvector(i,i+3)==IntervalVector(4,Interval::all_reals()));
		i+=4;
		CPPUNIT_ASSERT(sys.box.subvector(i,i+5)==IntervalVector(6,Interval::all_reals()));
		i+=6;
		CPPUNIT_ASSERT(sys.box.subvector(i,i+5)==IntervalVector(6,Interval::neg_reals()));
		i+=6;
		CPPUNIT_ASSERT(sys.box[i++]==Interval::pos_reals());
		CPPUNIT_ASSERT(sys.box[i++]==Interval::zero());
		CPPUNIT_ASSERT(sys.box[i++]==Interval(0,1));
		CPPUNIT_ASSERT(sys.box[i++]==Interval(0,1));
		CPPUNIT_ASSERT(sys.box[i++]==Interval::zero());
		CPPUNIT_ASSERT(sys.box[i++]==Interval::neg_reals());
		CPPUNIT_ASSERT(sys.box[i++]==0);
		CPPUNIT_ASSERT(sys.box[i++]==1);
		CPPUNIT_ASSERT(sys.box[i++]==2);
		CPPUNIT_ASSERT(sys.box[i++]==3);
		CPPUNIT_ASSERT(sys.box[i++]==4);
		CPPUNIT_ASSERT(sys.box[i++]==5);
		CPPUNIT_ASSERT(sys.box.subvector(i,i+5)==IntervalVector(6,Interval::neg_reals()));
		i+=6;

	} catch(SyntaxError& e) {
		cout << e << endl;
		CPPUNIT_ASSERT(false);
	}
}

void TestParser::const08() {
	FILE *fin = std::fopen (SRCDIR_TESTS "/minibex/const08.mbx", "r");
	CPPUNIT_ASSERT (fin != NULL);
	Function f(fin);

	double x=0.1;
	const ExprConstant* c=dynamic_cast<const ExprConstant*>(&f.expr());
	CPPUNIT_ASSERT(c!=NULL);
	CPPUNIT_ASSERT(c->get_value().mid()==x);
  std::fclose (fin);
}

void TestParser::func01() {
	try {
		Function f(SRCDIR_TESTS "/quimper/func01.qpr");

		CPPUNIT_ASSERT(strcmp(f.name,"foo")==0);
		CPPUNIT_ASSERT(f.nb_arg()==1);
		CPPUNIT_ASSERT(f.arg(0).dim.is_scalar());
		CPPUNIT_ASSERT(strcmp(f.arg(0).name,"x2")==0);
		CPPUNIT_ASSERT(sameExpr(f.expr(),"x2"));
	} catch(SyntaxError& e) {
		cout << e << endl;
		CPPUNIT_ASSERT(false);
	}
}

void TestParser::func02() {
	try {
		System sys(SRCDIR_TESTS "/quimper/func02.qpr");

		//cout << "sys nb ctr=" << sys.nb_ctr << endl;
		//CPPUNIT_ASSERT(sys.nb_ctr==12);
		CtcFwdBwd* c[24];
		for (int i=0; i<sys.ctrs.size(); i++)
			c[i]=new CtcFwdBwd(sys.ctrs[i]);

		for (int i=0; i<sys.ctrs.size(); i++) {
			IntervalVector subbox(2);
			subbox[0]=11;
			subbox[1]=12;
			IntervalVector box(8);
			box.put(1,subbox); // load x1[1] and x1[2]
			box.put(4,subbox); // load z1[1] and z1[2]

			c[i]->contract(box);

			CPPUNIT_ASSERT(!box.is_empty());
			check(box.subvector(6,7),subbox); // check x2
		}
		for (int i=0; i<sys.ctrs.size(); i++)
			delete c[i];

	} catch(SyntaxError& e) {
		cout << e << endl;
		CPPUNIT_ASSERT(false);
	}
}


void TestParser::func03() {
	try {
		Function f(SRCDIR_TESTS "/quimper/func03.qpr");

		CPPUNIT_ASSERT(strcmp(f.name,"f")==0);
		CPPUNIT_ASSERT(f.nb_arg()==10);
	} catch(SyntaxError& e) {
		cout << e << endl;
		CPPUNIT_ASSERT(false);
	}
}

void TestParser::ponts() {
	System sys(SRCDIR_TESTS "/quimper/ponts.qpr");
	Ponts30 sys2;
	CPPUNIT_ASSERT(sys.args.size()==30);
	for (int i=0; i<30; i++) {
		CPPUNIT_ASSERT(strcmp(sys.args[i].name,sys2.f->arg_name(i))==0);
		CPPUNIT_ASSERT(sys.args[0].dim.is_scalar());
		CPPUNIT_ASSERT(&sys.f_ctrs.arg(i) == &sys.args[i]);
	}
	CPPUNIT_ASSERT(sys.box.size()==30);
	CPPUNIT_ASSERT(almost_eq(sys.box, sys2.init_box,1e-12));
	CPPUNIT_ASSERT(sys.f_ctrs.nb_arg()==30);
	CPPUNIT_ASSERT(sameExpr(sys.f_ctrs.expr(),sys2.f->expr()));
	CPPUNIT_ASSERT(sys.ctrs.size()==30);

	for (int i=0; i<30; i++) {
		Function* fi=dynamic_cast<Function*>(&(sys.f_ctrs[i]));
		CPPUNIT_ASSERT(fi!=NULL);
		CPPUNIT_ASSERT(sameExpr(sys.ctrs[i].f.expr(),fi->expr()));
		CPPUNIT_ASSERT(sys.ctrs[i].op==EQ);
	}
}

void TestParser::choco01() {
	System sys(2,"{1}+{0}=0", simpl);
	CPPUNIT_ASSERT(sys.args.size()==2);
	CPPUNIT_ASSERT(strcmp(sys.args[0].name,"{0}")==0);
	CPPUNIT_ASSERT(strcmp(sys.args[1].name,"{1}")==0);
	CPPUNIT_ASSERT(sys.box.size()==2);
	CPPUNIT_ASSERT(sys.f_ctrs.nb_arg()==2);
	CPPUNIT_ASSERT(sameExpr(sys.f_ctrs.expr(),"({0}+{1})"));
	CPPUNIT_ASSERT(sys.ctrs.size()==1);
	CPPUNIT_ASSERT(sameExpr(sys.ctrs[0].f.expr(),sys.f_ctrs.expr()));
}

void TestParser::loop01() {
	try {
		System sys(SRCDIR_TESTS "/quimper/loop01.qpr", simpl);
		cout << sys << endl;
		double a = 1;
		double b = 2;
		int c=0;
		double error=1e-15;

		for (int i=1;i<=3;i++) {
			for (int j=1; j<=i; j++) {

				for (int k=j; k<=i; k++) {
					CPPUNIT_ASSERT(dynamic_cast<const ExprAdd*>(&sys.ctrs[c].f.expr()));
					const ExprAdd& add=(const ExprAdd&) sys.ctrs[c++].f.expr();
					// note: ExprSimplify will move constant on the right side
					const ExprConstant& cst=(const ExprConstant&) add.left;
					CPPUNIT_ASSERT_DOUBLES_EQUAL(-(a+i+j*k),cst.get_value().mid(),error);
				}
			}

			for (int j=1; j<=i; j++) {
				if (i+j-b==0) {
					CPPUNIT_ASSERT(sameExpr(sys.ctrs[c++].f.expr(),"x"));
				} else {
					CPPUNIT_ASSERT(dynamic_cast<const ExprAdd*>(&sys.ctrs[c].f.expr()));
					const ExprAdd& add=(const ExprAdd&) sys.ctrs[c++].f.expr();

					// note: ExprSimplify will move constant on the right side
					const ExprConstant& cst=(const ExprConstant&) add.left;
					CPPUNIT_ASSERT_DOUBLES_EQUAL(-(i+j-b),cst.get_value().mid(),error);
				}
			}
		}

	} catch(SyntaxError& s)  {
		cout << s << endl;
	}
}

void TestParser::error01() {
	CPPUNIT_ASSERT_THROW(System(SRCDIR_TESTS "/quimper/error01.qpr", simpl),SyntaxError);
}

void TestParser::issue245_1() {
	Function f(SRCDIR_TESTS "/minibex/issue245_1.mbx");
	CPPUNIT_ASSERT(sameExpr(f.expr(),"((3*x^2)+y^2)"));
}

void TestParser::issue245_2() {
	CPPUNIT_ASSERT_THROW(System(SRCDIR_TESTS "/minibex/issue245_2.mbx", simpl),SyntaxError);
}

void TestParser::issue245_3() {
	System sys(SRCDIR_TESTS "/minibex/issue245_3.mbx", simpl);
	CPPUNIT_ASSERT(sameExpr(sys.ctrs[0].f.expr(),"((-3+(3*x^2))+y^2)"));
}

void TestParser::nary_max() {
	Function f("x","y","z","max(x,y,z)");

	double _v[3] = {1,3,2};
	Vector v(3,_v);
	check(f.eval(v),3.0);

	v[2]=4;
	check(f.eval(v),4.0);

	v[0]=5;
	check(f.eval(v),5.0);

}

void TestParser::sum01() {
	try {
		Function f("x[4]","sum(i=1:4,x(i))");
		CPPUNIT_ASSERT(sameExpr(f.expr(),"(((x(1)+x(2))+x(3))+x(4))"));
	} catch(SyntaxError&) {
		CPPUNIT_ASSERT(false);
	}
}

void TestParser::sum02() {
	CPPUNIT_ASSERT_THROW(Function("x","sum(x=1:4,0)"), SyntaxError);
	CPPUNIT_ASSERT_THROW(Function("x[4]","sum(i=1:0,x(i))"), SyntaxError);
	CPPUNIT_ASSERT_THROW(Function("x[4]","sum(i=1:2,sum(i=1:2,x(i)))"), SyntaxError);
}

void TestParser::sum03() {
	try {
		Function f("x[4]","sum(i=1:4,i)");
		CPPUNIT_ASSERT(dynamic_cast<const ExprConstant*>(&f.expr())!=NULL);
		CPPUNIT_ASSERT(sameExpr(f.expr(),"10"));
	} catch(SyntaxError&) {
		CPPUNIT_ASSERT(false);
	}
}

void TestParser::sum04() {
	try {
		Function f("x[4]","sum(i=1:2,sum(j=1:2,x(i+j)))");
		CPPUNIT_ASSERT(sameExpr(f.expr(),"((x(2)+x(3))+(x(3)+x(4)))"));
	} catch(SyntaxError&) {
		CPPUNIT_ASSERT(false);
	}
}

void TestParser::temp_in_loop() {
	System sys(SRCDIR_TESTS "/minibex/issue380.mbx", simpl);
	CPPUNIT_ASSERT(sameExpr(sys.f_ctrs.expr(),"((-1+x(1));(-2+x(2));(-3+x(3));(-4+x(4)))"));
}

void TestParser::diff_lock() {
	System sys(SRCDIR_TESTS "/minibex/diff_lock.mbx", simpl);
	CPPUNIT_ASSERT(sameExpr(sys.f_ctrs.expr(),"((2*x);((-3+x^2)+y))"));
}

void TestParser::issue365() {
	System sys(SRCDIR_TESTS "/minibex/issue365.mbx", simpl);
	CPPUNIT_ASSERT(sys.f_ctrs.expr().size==7);
}

void TestParser::issue474() {
	CPPUNIT_ASSERT_NO_THROW(System(SRCDIR_TESTS "/minibex/issue474.mbx", simpl));
}

void TestParser::mutable_cst_1() {
	const ExprSymbol& x=ExprSymbol::new_("x");
	Interval _c1(1);
	IntervalVector _c2(2);
	const ExprConstant& c1=ExprConstant::new_mutable(_c1);
	const ExprConstant& c2=ExprConstant::new_mutable(_c2,true);
	_c1=1; _c2[0]=2; _c2[1]=3;
	SystemFactory fac;
	fac.add_var(x,Interval(0,10));
	const ExprCtr& ctr=(x-c1)*(x-c2[0])*(x-c2[1])=0;
	fac.add_ctr(ctr);
	System sys(fac);
	DefaultSolver solver(sys);

	_c1=4; _c2[0]=5; _c2[1]=6;
	solver.solve(sys.box);
	CPPUNIT_ASSERT(solver.get_data().solution(0)[0]==Interval(4));
	CPPUNIT_ASSERT(solver.get_data().solution(1)[0]==Interval(5));
	CPPUNIT_ASSERT(solver.get_data().solution(2)[0]==Interval(6));

	_c1=7; _c2[0]=8; _c2[1]=9;
	solver.solve(sys.box);
	CPPUNIT_ASSERT(solver.get_data().solution(0)[0]==Interval(7));
	CPPUNIT_ASSERT(solver.get_data().solution(1)[0]==Interval(8));
	CPPUNIT_ASSERT(solver.get_data().solution(2)[0]==Interval(9));
	cleanup(ctr.e,true);
	delete &ctr;
}

void TestParser::mutable_cst_2() {
	const ExprSymbol& x=ExprSymbol::new_("x");
	Interval _c(1);
	const ExprConstant& c=ExprConstant::new_mutable(_c);
	const ExprNode& y=c*x;

	Function f(x,y);
	_c=2;

	CPPUNIT_ASSERT(f.gradient(IntervalVector(1,1))[0]==2);
	CPPUNIT_ASSERT(f.jacobian(IntervalVector(1,1))[0][0]==2);
	_c=3;
	CPPUNIT_ASSERT(f.gradient(IntervalVector(1,1))[0]==3);
	CPPUNIT_ASSERT(f.jacobian(IntervalVector(1,1))[0][0]==3);
}

} // end namespace
