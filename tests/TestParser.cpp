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

namespace ibex {

static bool sameExpr(const ExprNode& node, const char* expr) {
	std::stringstream s;
	s << node;
	return strcmp(s.str().c_str(),expr)==0;
}

void TestParser::file01() {
	System sys("file01.txt");
	TEST_ASSERT(sys.func.empty());
	TEST_ASSERT(sys.vars.size()==1);
	TEST_ASSERT(strcmp(sys.vars[0].name,"x")==0);
	TEST_ASSERT(sys.vars[0].dim.is_scalar());
	TEST_ASSERT(sys.eprs.empty());
	TEST_ASSERT(sys.sybs.empty());
	TEST_ASSERT(sys.box.size()==1);
	TEST_ASSERT(sys.box[0]==Interval::ALL_REALS);
	TEST_ASSERT(sys.f->nb_symbols()==1);
	TEST_ASSERT(&sys.f->symbol(0) == &sys.vars[0]);
	TEST_ASSERT(sameExpr(sys.f->expr(),"x"));
	TEST_ASSERT(sys.ctrs.size()==1);
	TEST_ASSERT(&sys.ctrs[0]->f==sys.f);
	TEST_ASSERT(sys.ctrs[0]->op==NumConstraint::GEQ);
}

} // end namespace


