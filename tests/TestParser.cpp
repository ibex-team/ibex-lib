//============================================================================
//                                  I B E X
// File        : TestParser.cpp
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Jun 22, 2012
// Last Update : Jun 22, 2012
//============================================================================

#include <sstream>

#include "TestParser.h"
#include "ibex_System.h"
#include "ibex_SyntaxError.h"
#include "ibex_CtcFwdBwd.h"
#include "Ponts30.h"
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

void TestParser::var01() {
	System sys(SRCDIR_TESTS "/quimper/var01.qpr");
	CPPUNIT_ASSERT(sys.func.is_empty());
	CPPUNIT_ASSERT(sys.args.size()==1);
	CPPUNIT_ASSERT(strcmp(sys.args[0].name,"x")==0);
	CPPUNIT_ASSERT(sys.args[0].dim.is_scalar());
	CPPUNIT_ASSERT(sys.box.size()==1);
	CPPUNIT_ASSERT(sys.box[0]==Interval::ALL_REALS);
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
		CPPUNIT_ASSERT(sys.box[i++]==Interval::ALL_REALS);
		CPPUNIT_ASSERT(sys.box[i++]==Interval(0,1));
		CPPUNIT_ASSERT(sys.box[i++]==Interval::NEG_REALS);
		CPPUNIT_ASSERT(sys.box[i++]==Interval::POS_REALS);
		CPPUNIT_ASSERT(sys.box[i++]==3.14);

		CPPUNIT_ASSERT(sys.box[i++]==Interval::ALL_REALS);
		CPPUNIT_ASSERT(sys.box[i++]==Interval::ALL_REALS);
		CPPUNIT_ASSERT(sys.box[i++]==Interval(0,1));
		CPPUNIT_ASSERT(sys.box[i++]==Interval(0,2));
		CPPUNIT_ASSERT(sys.box[i++]==Interval(0,0));
		CPPUNIT_ASSERT(sys.box[i++]==Interval(1,1));

		// xcv3
		CPPUNIT_ASSERT(sys.box.subvector(i,i+2)==IntervalVector(3,Interval::ALL_REALS));
		i+=3;

		CPPUNIT_ASSERT(sys.box.subvector(i,i+2)==IntervalVector(3,Interval::ALL_REALS));
		i+=3;
		CPPUNIT_ASSERT(sys.box[i++]==Interval(0,1));
		CPPUNIT_ASSERT(sys.box[i++]==Interval(0,2));
		CPPUNIT_ASSERT(sys.box[i++]==Interval(0,3));
		CPPUNIT_ASSERT(sys.box[i++]==Interval::ZERO);
		CPPUNIT_ASSERT(sys.box[i++]==Interval::ONE);
		CPPUNIT_ASSERT(sys.box[i++]==Interval(2,2));

		// xm22
		CPPUNIT_ASSERT(sys.box.subvector(i,i+3)==IntervalVector(4,Interval::ALL_REALS));
		i+=4;
		CPPUNIT_ASSERT(sys.box.subvector(i,i+5)==IntervalVector(6,Interval::ALL_REALS));
		i+=6;
		CPPUNIT_ASSERT(sys.box.subvector(i,i+5)==IntervalVector(6,Interval::NEG_REALS));
		i+=6;
		CPPUNIT_ASSERT(sys.box[i++]==Interval::POS_REALS);
		CPPUNIT_ASSERT(sys.box[i++]==Interval::ZERO);
		CPPUNIT_ASSERT(sys.box[i++]==Interval(0,1));
		CPPUNIT_ASSERT(sys.box[i++]==Interval(0,1));
		CPPUNIT_ASSERT(sys.box[i++]==Interval::ZERO);
		CPPUNIT_ASSERT(sys.box[i++]==Interval::NEG_REALS);
		CPPUNIT_ASSERT(sys.box[i++]==0);
		CPPUNIT_ASSERT(sys.box[i++]==1);
		CPPUNIT_ASSERT(sys.box[i++]==2);
		CPPUNIT_ASSERT(sys.box[i++]==3);
		CPPUNIT_ASSERT(sys.box[i++]==4);
		CPPUNIT_ASSERT(sys.box[i++]==5);
		CPPUNIT_ASSERT(sys.box.subvector(i,i+5)==IntervalVector(6,Interval::NEG_REALS));
		i+=6;

	} catch(SyntaxError& e) {
		cout << e << endl;
		CPPUNIT_ASSERT(false);
	}
}

void TestParser::const08() {
	const char* code="function f(x)\n  return #3fb999999999999a;end";
	FILE *fin = fmemopen((char*) code, strlen(code), "r");
	Function f(fin);

	double x=0.1;
	const ExprConstant* c=dynamic_cast<const ExprConstant*>(&f.expr());
	CPPUNIT_ASSERT(c!=NULL);
	CPPUNIT_ASSERT(c->get_value().mid()==x);
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
	CPPUNIT_ASSERT(sys.func.is_empty());
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
	System sys(2,"{1}+{0}=0");
	CPPUNIT_ASSERT(sys.func.is_empty());
	CPPUNIT_ASSERT(sys.args.size()==2);
	CPPUNIT_ASSERT(strcmp(sys.args[0].name,"{0}")==0);
	CPPUNIT_ASSERT(strcmp(sys.args[1].name,"{1}")==0);
	CPPUNIT_ASSERT(sys.box.size()==2);
	CPPUNIT_ASSERT(sys.f_ctrs.nb_arg()==2);
	CPPUNIT_ASSERT(sameExpr(sys.f_ctrs.expr(),"({1}+{0})"));
	CPPUNIT_ASSERT(sys.ctrs.size()==1);
	CPPUNIT_ASSERT(sameExpr(sys.ctrs[0].f.expr(),sys.f_ctrs.expr()));
}

void TestParser::loop01() {
	try {
		System sys(SRCDIR_TESTS "/quimper/loop01.qpr");
		double a = 1;
		double b = 2;
		int c=0;
		double error=1e-15;

		for (int i=1;i<=3;i++) {
		    for (int j=1; j<=i; j++) {

		      for (int k=j; k<=i; k++) {
		    	  const ExprSub& sub=(const ExprSub&) sys.ctrs[c++].f.expr();

		    	  // note: ExprSimplify will move constant on the right side
		    	  const ExprConstant& cst=(const ExprConstant&) sub.right;
		    	  CPPUNIT_ASSERT_DOUBLES_EQUAL(a+i+j*k,cst.get_value().mid(),error);
		      }
		    }

		    for (int j=1; j<=i; j++) {
		    	if (i+j-b==0) {
		    		CPPUNIT_ASSERT(sameExpr(sys.ctrs[c++].f.expr(),"x"));
		    	} else {
		    		const ExprSub& sub=(const ExprSub&) sys.ctrs[c++].f.expr();

		    		 // note: ExprSimplify will move constant on the right side
		    		const ExprConstant& cst=(const ExprConstant&) sub.right;
		    		CPPUNIT_ASSERT_DOUBLES_EQUAL(i+j-b,cst.get_value().mid(),error);
		    	}
		    }
		}

	} catch(SyntaxError& s)  {
		cout << s << endl;
	}
}

void TestParser::error01() {
	CPPUNIT_ASSERT_THROW(System(SRCDIR_TESTS "/quimper/error01.qpr"),SyntaxError);
}

void TestParser::issue245_1() {
	Function f(SRCDIR_TESTS "/minibex/issue245_1.mbx");
	CPPUNIT_ASSERT(sameExpr(f.expr(),"((3*x^2)+y^2)"));
}

void TestParser::issue245_2() {
	CPPUNIT_ASSERT_THROW(System(SRCDIR_TESTS "/minibex/issue245_2.mbx"),SyntaxError);
}

void TestParser::issue245_3() {
	System sys(SRCDIR_TESTS "/minibex/issue245_3.mbx");
	CPPUNIT_ASSERT(sameExpr(sys.ctrs[0].f.expr(),"(((3*x^2)+y^2)-3)"));
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

} // end namespace
