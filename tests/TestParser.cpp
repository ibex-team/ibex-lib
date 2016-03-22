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

using namespace std;

namespace ibex {

void TestParser::var01() {
	System sys("quimper/var01.qpr");
	CPPUNIT_ASSERT(sys.func.is_empty());
	CPPUNIT_ASSERT(sys.args.size()==1);
	CPPUNIT_ASSERT(strcmp(sys.args[0].name,"x")==0);
	CPPUNIT_ASSERT(sys.args[0].dim.is_scalar());
	CPPUNIT_ASSERT(sys.eprs.empty());
	CPPUNIT_ASSERT(sys.sybs.empty());
	CPPUNIT_ASSERT(sys.box.size()==1);
	CPPUNIT_ASSERT(sys.box[0]==Interval::ALL_REALS);
	CPPUNIT_ASSERT(sys.f.nb_arg()==1);
	CPPUNIT_ASSERT(&sys.f.arg(0) == &sys.args[0]);
	CPPUNIT_ASSERT(sameExpr(sys.f.expr(),"x"));
	CPPUNIT_ASSERT(sys.ctrs.size()==1);
	CPPUNIT_ASSERT(sys.ctrs[0].f.nb_arg()==1);
	CPPUNIT_ASSERT(sameExpr(sys.ctrs[0].f.expr(),"x"));
	CPPUNIT_ASSERT(sys.ctrs[0].op==GEQ);
}

void TestParser::const01() {
	try {
		System sys("quimper/const01.qpr");
		check(sys.box[0],Interval(3.14159));
		sys.box.init(Interval::ALL_REALS);
		CtcFwdBwd c0(sys.ctrs[0]);
		c0.contract(sys.box);
		CPPUNIT_ASSERT(sys.box[0]==Interval::ZERO);
	} catch(SyntaxError& e) {
		cout << e << endl;
		CPPUNIT_ASSERT(false);
	}
}

void TestParser::const02() {
	try {
		System sys("quimper/const02.qpr");
		double _box[3][2]={{0,0},{1,1},{2,2}};
		IntervalVector box(3,_box);
		check(sys.box,box);
		sys.box.init(Interval::ALL_REALS);
		CtcFwdBwd c0(sys.ctrs[0]);
		c0.contract(sys.box);
		CPPUNIT_ASSERT(sys.box[0]==Interval::ZERO);
	} catch(SyntaxError& e) {
		cout << e << endl;
		CPPUNIT_ASSERT(false);
	}
}

void TestParser::const03() {
	try {
		System sys("quimper/const03.qpr");
		double _m[6][2]={{0,0},{1,1},{2,2},
				{3,3},{4,4},{5,5}};
		IntervalVector box(6,_m);
		check(sys.box,box);

		sys.box.init(Interval::ALL_REALS);
		CtcFwdBwd(sys.ctrs[0]).contract(sys.box);
		check(sys.box[0],Interval::ZERO);
		check(sys.box[1],Interval::ALL_REALS);

		sys.box.init(Interval::ALL_REALS);
		CtcFwdBwd(sys.ctrs[1]).contract(sys.box);
		IntervalVector zero(3);
		zero.init(0);
		check(sys.box.subvector(0,2),zero);

	} catch(SyntaxError& e) {
		cout << e << endl;
		CPPUNIT_ASSERT(false);
	}
}

void TestParser::const04() {
	try {
		System sys("quimper/const04.qpr");
		double _m[6][2]={{0,0},{3,3},{1,1},
				         {4,4},{2,2},{5,5}};
		IntervalVector box(6,_m);
		check(sys.box,box);
	} catch(SyntaxError& e) {
		cout << e << endl;
		CPPUNIT_ASSERT(false);
	}
}

void TestParser::const05() {
	try {
		System sys("quimper/const05.qpr");
		double _m[6][2]={{0,0},{3,3},{1,1},
				         {4,4},{2,2},{5,5}};
		IntervalVector box(6,_m);
		check(sys.box,box.inflate(0.1));
	} catch(SyntaxError& e) {
		cout << e << endl;
		CPPUNIT_ASSERT(false);
	}
}

void TestParser::const06() {
	try {
		System sys("quimper/const06.qpr");
		check(sys.box[0],Interval(1.0,POS_INFINITY));
		check(sys.box[1],Interval(NEG_INFINITY,0));
		check(sys.box[2],Interval::ALL_REALS);
	} catch(SyntaxError& e) {
		cout << e << endl;
		CPPUNIT_ASSERT(false);
	}
}

void TestParser::const07() {
	try {
		System sys("quimper/const07.qpr");

		double _m1[6][2]={{0,0},{1,1},{2,2},
				{3,3},{4,4},{5,5}};
		IntervalVector box1(6,_m1);
		double _m2[6][2]={{6,6},{7,7},{8,8},
				{9,9},{10,10},{11,11}};
		IntervalVector box2(6,_m2);
		check(sys.box.subvector(0,5), box1);
		check(sys.box.subvector(6,11), box2);
		sys.box.init(Interval::ALL_REALS);

		CtcFwdBwd c0(sys.ctrs[0]);
		c0.contract(sys.box);
		CPPUNIT_ASSERT(!sys.box.is_empty());
		IntervalVector zero(6);
		zero.init(0);

		CtcFwdBwd c1(sys.ctrs[1]);
		sys.box[5]=Interval::ALL_REALS;
		c1.contract(sys.box);
		CPPUNIT_ASSERT(!sys.box.is_empty());
		check(sys.box[5],Interval(1,1));

		CtcFwdBwd c2(sys.ctrs[2]);
		sys.box.init(Interval::ALL_REALS);
		c2.contract(sys.box);
		CPPUNIT_ASSERT(!sys.box.is_empty());
		check(sys.box[5],Interval(1,1));

		CtcFwdBwd c3(sys.ctrs[3]);
		sys.box.init(Interval::ALL_REALS);
		c3.contract(sys.box);
		CPPUNIT_ASSERT(!sys.box.is_empty());
		double _c21[][2]={{6,6},{7,7},{8,8}};
		IntervalVector c21(3,_c21);
		check(sys.box.subvector(3,5),c21);

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
		Function f("quimper/func01.qpr");

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
		System sys("quimper/func02.qpr");

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
		Function f("quimper/func03.qpr");

		CPPUNIT_ASSERT(strcmp(f.name,"f")==0);
		CPPUNIT_ASSERT(f.nb_arg()==10);
	} catch(SyntaxError& e) {
		cout << e << endl;
		CPPUNIT_ASSERT(false);
	}
}

void TestParser::ponts() {
	System sys("quimper/ponts.qpr");
	Ponts30 sys2;
	CPPUNIT_ASSERT(sys.func.is_empty());
	CPPUNIT_ASSERT(sys.args.size()==30);
	for (int i=0; i<30; i++) {
		CPPUNIT_ASSERT(strcmp(sys.args[i].name,sys2.f->arg_name(i))==0);
		CPPUNIT_ASSERT(sys.args[0].dim.is_scalar());
		CPPUNIT_ASSERT(&sys.f.arg(i) == &sys.args[i]);
	}
	CPPUNIT_ASSERT(sys.eprs.empty());
	CPPUNIT_ASSERT(sys.sybs.empty());
	CPPUNIT_ASSERT(sys.box.size()==30);
	CPPUNIT_ASSERT(almost_eq(sys.box, sys2.init_box,1e-12));
	CPPUNIT_ASSERT(sys.f.nb_arg()==30);
	CPPUNIT_ASSERT(sameExpr(sys.f.expr(),sys2.f->expr()));
	CPPUNIT_ASSERT(sys.ctrs.size()==30);

	for (int i=0; i<30; i++) {
		Function* fi=dynamic_cast<Function*>(&(sys.f[i]));
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
	CPPUNIT_ASSERT(sys.eprs.empty());
	CPPUNIT_ASSERT(sys.sybs.empty());
	CPPUNIT_ASSERT(sys.box.size()==2);
	CPPUNIT_ASSERT(sys.f.nb_arg()==2);
	CPPUNIT_ASSERT(sameExpr(sys.f.expr(),"({1}+{0})"));
	CPPUNIT_ASSERT(sys.ctrs.size()==1);
	CPPUNIT_ASSERT(sameExpr(sys.ctrs[0].f.expr(),sys.f.expr()));
}

void TestParser::loop01() {
	try {
		System sys("quimper/loop01.qpr");
		double a = 1;
		double b = 2;
		int c=0;
		double error=1e-15;
		for (int i=1;i<=3;i++) {
		    for (int j=1; j<=i; j++) {

		      for (int k=j; k<=i; k++) {
		    	  const ExprSub& sub=(const ExprSub&) sys.ctrs[c++].f.expr();
		    	  const ExprConstant& cst=(const ExprConstant&) sub.right;
		    	  CPPUNIT_ASSERT_DOUBLES_EQUAL(a+i+j*k,cst.get_value().mid(),error);
		      }
		    }

		    for (int j=1; j<=i; j++) {
		    	const ExprSub& sub=(const ExprSub&) sys.ctrs[c++].f.expr();
		    	const ExprConstant& cst=(const ExprConstant&) sub.right;
		    	CPPUNIT_ASSERT_DOUBLES_EQUAL(i+j-b,cst.get_value().mid(),error);

		    }
		}

	} catch(SyntaxError& s)  {
		cout << s << endl;
	}
}

void TestParser::error01() {
	CPPUNIT_ASSERT_THROW(System("quimper/error01.qpr"),SyntaxError);
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


