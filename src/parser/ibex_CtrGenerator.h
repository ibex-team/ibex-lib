//============================================================================
//                                  I B E X                                   
// File        : ibex_CtrGenerator.h
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Jun 12, 2012
// Last Update : Jun 12, 2012
//============================================================================

#ifndef __IBEX_CTR_GENERATOR_H__
#define __IBEX_CTR_GENERATOR_H__

#include <vector>
#include <stack>
#include <utility>

#include "ibex_NumConstraint.h"
#include "ibex_P_Expr.h"
#include "ibex_Expr.h"
#include "ibex_Function.h"
#include "ibex_ExprSimplify.h"

namespace ibex {

namespace parser {

class P_NumConstraint;
class P_OneConstraint;
class P_ConstraintList;
class P_ConstraintLoop;
class P_ThickEquality;
class P_Source;

class CtrGenerator {
public:
	CtrGenerator();

	std::vector<ExprCtr*> generate(const P_ConstraintList& ctrs);

	void visit(const P_NumConstraint& c);
	void visit(const P_OneConstraint& c);
	void visit(const P_ConstraintList& l);
	void visit(const P_ConstraintLoop& l);
	void visit(const P_ThickEquality& e);

protected:
	ExprSimplify s;
	std::vector<ExprCtr*> ctrs;

};

} // end namespace parser
} // end namespace ibex

#endif // __IBEX_CTR_GENERATOR_H__
