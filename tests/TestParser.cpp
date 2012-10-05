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
#include "ibex_CtcProj.h"
#include "Ponts30.h"

using namespace std;

namespace ibex {

void TestParser::var01() {
	System sys("quimper/var01.qpr");
	TEST_ASSERT(sys.func.is_empty());
	TEST_ASSERT(sys.args.size()==1);
	TEST_ASSERT(strcmp(sys.args[0].name,"x")==0);
	TEST_ASSERT(sys.args[0].dim.is_scalar());
	TEST_ASSERT(sys.eprs.empty());
	TEST_ASSERT(sys.sybs.empty());
	TEST_ASSERT(sys.box.size()==1);
	TEST_ASSERT(sys.box[0]==Interval::ALL_REALS);
	TEST_ASSERT(sys.f.nb_arg()==1);
	TEST_ASSERT(&sys.f.arg(0) == &sys.args[0]);
	TEST_ASSERT(sameExpr(sys.f.expr(),"x"));
	TEST_ASSERT(sys.ctrs.size()==1);
	TEST_ASSERT(sys.ctrs[0].f.nb_arg()==1);
	TEST_ASSERT(sameExpr(sys.ctrs[0].f.expr(),"x"));
	TEST_ASSERT(sys.ctrs[0].op==GEQ);
}

void TestParser::const01() {
	try {System sys("quimper/const01.qpr");
	check(sys.box[0],Interval(3.14159));
	} catch(SyntaxError& e) {
		cout << e << endl;
		TEST_ASSERT(false);
	}
}

void TestParser::const02() {
	try {
		System sys("quimper/const02.qpr");
		double _box[3][2]={{0,0},{1,1},{2,2}};
		IntervalVector box(3,_box);
		check(sys.box,box);
	} catch(SyntaxError& e) {
		cout << e << endl;
		TEST_ASSERT(false);
	}
}

void TestParser::const03() {
	try {
		System sys("quimper/const03.qpr");
		double _m[6][2]={{0,0},{1,1},{2,2},
				{3,3},{4,4},{5,5}};
		IntervalVector box(6,_m);
		check(sys.box,box);
	} catch(SyntaxError& e) {
		cout << e << endl;
		TEST_ASSERT(false);
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
		TEST_ASSERT(false);
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
		TEST_ASSERT(false);
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
		TEST_ASSERT(false);
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

	} catch(SyntaxError& e) {
		cout << e << endl;
		TEST_ASSERT(false);
	}
}

void TestParser::func01() {
	try {
		System sys("quimper/func01.qpr");

		TEST_ASSERT(sys.args.size()==1);
		TEST_ASSERT(strcmp(sys.args[0].name,"x")==0);
		TEST_ASSERT(sys.args[0].dim.is_scalar());
		TEST_ASSERT(sys.eprs.empty());
		TEST_ASSERT(sys.sybs.empty());
		TEST_ASSERT(sys.box.size()==1);

		TEST_ASSERT(sys.func.size()==1);
		TEST_ASSERT(strcmp(sys.func[0].name,"foo")==0);
		TEST_ASSERT(sys.func[0].nb_arg()==1);
		TEST_ASSERT(sys.func[0].arg(0).dim.is_scalar());
		TEST_ASSERT(strcmp(sys.func[0].arg(0).name,"x2")==0);
		TEST_ASSERT(sameExpr(sys.func[0].expr(),"x2"));
		TEST_ASSERT(sameExpr(sys.f.expr(),"(x-foo(x))"));
		TEST_ASSERT(sys.ctrs.size()==1);
		TEST_ASSERT(sameExpr(sys.ctrs[0].f.expr(),sys.f.expr()));
		TEST_ASSERT(sys.ctrs[0].op==EQ);
	} catch(SyntaxError& e) {
		cout << e << endl;
		TEST_ASSERT(false);
	}
}

void TestParser::func02() {
	try {
		System sys("quimper/func02.qpr");

		//cout << "sys nb ctr=" << sys.nb_ctr << endl;
		//TEST_ASSERT(sys.nb_ctr==12);
		CtcProj* c[24];
		for (int i=0; i<sys.ctrs.size(); i++)
			c[i]=new CtcProj(sys.ctrs[i]);

		for (int i=0; i<sys.ctrs.size(); i++) {
			IntervalVector subbox(2);
			subbox[0]=11;
			subbox[1]=12;
			IntervalVector box(8);
			box.put(1,subbox); // load x1[1] and x1[2]
			box.put(4,subbox); // load z1[1] and z1[2]
			try {
				c[i]->contract(box);
			} catch(EmptyBoxException&) {
				TEST_ASSERT(false);
			}
			check(box.subvector(6,7),subbox); // check x2
		}

	} catch(SyntaxError& e) {
		cout << e << endl;
		TEST_ASSERT(false);
	}
}

void TestParser::ponts() {
	System sys("quimper/ponts.qpr");
	Ponts30 sys2;
	TEST_ASSERT(sys.func.is_empty());
	TEST_ASSERT(sys.args.size()==30);
	for (int i=0; i<30; i++) {
		TEST_ASSERT(strcmp(sys.args[i].name,sys2.f->arg_name(i))==0);
		TEST_ASSERT(sys.args[0].dim.is_scalar());
		TEST_ASSERT(&sys.f.arg(i) == &sys.args[i]);
	}
	TEST_ASSERT(sys.eprs.empty());
	TEST_ASSERT(sys.sybs.empty());
	TEST_ASSERT(sys.box.size()==30);
	TEST_ASSERT(sys.box==sys2.init_box);
	TEST_ASSERT(sys.f.nb_arg()==30);
	TEST_ASSERT(sameExpr(sys.f.expr(),sys2.f->expr()));
	TEST_ASSERT(sys.ctrs.size()==30);

	for (int i=0; i<30; i++) {
		TEST_ASSERT(sameExpr(sys.ctrs[i].f.expr(),sys.f[i].expr()));
		TEST_ASSERT(sys.ctrs[i].op==EQ);
	}
}

void TestParser::choco01() {
	System sys(2,"{1}+{0}=0");
	TEST_ASSERT(sys.func.is_empty());
	TEST_ASSERT(sys.args.size()==2);
	TEST_ASSERT(strcmp(sys.args[0].name,"{0}")==0);
	TEST_ASSERT(strcmp(sys.args[1].name,"{1}")==0);
	TEST_ASSERT(sys.eprs.empty());
	TEST_ASSERT(sys.sybs.empty());
	TEST_ASSERT(sys.box.size()==2);
	TEST_ASSERT(sys.f.nb_arg()==2);
	TEST_ASSERT(sameExpr(sys.f.expr(),"({1}+{0})"));
	TEST_ASSERT(sys.ctrs.size()==1);
	TEST_ASSERT(sameExpr(sys.ctrs[0].f.expr(),sys.f.expr()));
}

void TestParser::error01() {
	TEST_THROWS(System("quimper/error01.qpr"),SyntaxError&);
}

} // end namespace


