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
#include "Ponts30.h"

namespace ibex {

static bool sameExpr(const ExprNode& node, const char* expr) {
	std::stringstream s;
	s << node;
	return strcmp(s.str().c_str(),expr)==0;
}

static bool sameExpr(const ExprNode& node, const ExprNode& node2) {
	std::stringstream s,s2;
	s << node;
	s2 << node2;
	return strcmp(s.str().c_str(),s2.str().c_str())==0;
}

void TestParser::var01() {
	System sys("quimper/var01.qpr");
	TEST_ASSERT(sys.func.is_empty());
	TEST_ASSERT(sys.vars.size()==1);
	TEST_ASSERT(strcmp(sys.vars[0].name,"x")==0);
	TEST_ASSERT(sys.vars[0].dim.is_scalar());
	TEST_ASSERT(sys.eprs.empty());
	TEST_ASSERT(sys.sybs.empty());
	TEST_ASSERT(sys.box.size()==1);
	TEST_ASSERT(sys.box[0]==Interval::ALL_REALS);
	TEST_ASSERT(sys.f.nb_symbols()==1);
	TEST_ASSERT(&sys.f.symbol(0) == &sys.vars[0]);
	TEST_ASSERT(sameExpr(sys.f.expr(),"x"));
	TEST_ASSERT(sys.ctrs.size()==1);
	TEST_ASSERT(&sys.ctrs[0].f==&sys.f);
	TEST_ASSERT(sys.ctrs[0].op==GEQ);
}


void TestParser::func01() {
	System sys("quimper/func01.qpr");

	TEST_ASSERT(sys.vars.size()==1);
	TEST_ASSERT(strcmp(sys.vars[0].name,"x")==0);
	TEST_ASSERT(sys.vars[0].dim.is_scalar());
	TEST_ASSERT(sys.eprs.empty());
	TEST_ASSERT(sys.sybs.empty());
	TEST_ASSERT(sys.box.size()==1);

	TEST_ASSERT(sys.func.size()==1);
	TEST_ASSERT(strcmp(sys.func[0].name,"foo")==0);
	TEST_ASSERT(sys.func[0].nb_symbols()==1);
	TEST_ASSERT(sys.func[0].symbol(0).dim.is_scalar());
	TEST_ASSERT(strcmp(sys.func[0].symbol(0).name,"x2")==0);
	TEST_ASSERT(sameExpr(sys.func[0].expr(),"x2"));
	TEST_ASSERT(sameExpr(sys.f.expr(),"(x-foo(x))"));
	TEST_ASSERT(sys.ctrs.size()==1);
	TEST_ASSERT(&sys.ctrs[0].f==&sys.f);
	TEST_ASSERT(sys.ctrs[0].op==EQ);
}

void TestParser::ponts() {
	System sys("quimper/ponts.qpr");
	Ponts30 sys2;
	TEST_ASSERT(sys.func.is_empty());
	TEST_ASSERT(sys.vars.size()==30);
	for (int i=0; i<30; i++) {
		TEST_ASSERT(strcmp(sys.vars[i].name,sys2.f->symbol_name(i))==0);
		TEST_ASSERT(sys.vars[0].dim.is_scalar());
		TEST_ASSERT(&sys.f.symbol(i) == &sys.vars[i]);
	}
	TEST_ASSERT(sys.eprs.empty());
	TEST_ASSERT(sys.sybs.empty());
	TEST_ASSERT(sys.box.size()==30);
	TEST_ASSERT(sys.box==sys2.init_box);
	TEST_ASSERT(sys.f.nb_symbols()==30);
	TEST_ASSERT(sameExpr(sys.f.expr(),sys2.f->expr()));
	TEST_ASSERT(sys.ctrs.size()==30);

	for (int i=0; i<30; i++) {
		TEST_ASSERT(&(sys.ctrs[i].f)==&(sys.f[i]));
		TEST_ASSERT(sys.ctrs[i].op==EQ);
	}
}

void TestParser::choco01() {
	System sys(2,"{1}+{0}=0");
	TEST_ASSERT(sys.func.is_empty());
	TEST_ASSERT(sys.vars.size()==2);
	TEST_ASSERT(strcmp(sys.vars[0].name,"{0}")==0);
	TEST_ASSERT(strcmp(sys.vars[1].name,"{1}")==0);
	TEST_ASSERT(sys.eprs.empty());
	TEST_ASSERT(sys.sybs.empty());
	TEST_ASSERT(sys.box.size()==2);
	TEST_ASSERT(sys.f.nb_symbols()==2);
	TEST_ASSERT(sameExpr(sys.f.expr(),"({1}+{0})"));
	TEST_ASSERT(sys.ctrs.size()==1);
	TEST_ASSERT(&(sys.ctrs[0].f)==&(sys.f));
}

void TestParser::error01() {
	TEST_THROWS(System("quimper/error01.qpr"),SyntaxError&);
}

} // end namespace


