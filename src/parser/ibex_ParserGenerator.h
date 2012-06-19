//============================================================================
//                                  I B E X                                   
// File        : ibex_ParserGenerator.h
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Jun 12, 2012
// Last Update : Jun 12, 2012
//============================================================================

#ifndef __IBEX_PARSER_GENERATOR_H__
#define __IBEX_PARSER_GENERATOR_H__

#include <vector>
#include <stack>
#include <utility>
#include "ibex_ParserScope.h"
#include "ibex_NumConstraint.h"
#include "ibex_Expr.h"
#include "ibex_Function.h"
#include "ibex_ParserExprVisitor.h"
#include "ibex_ExprCopy.h"

namespace ibex {

namespace parser {

class P_NumConstraint;
class P_OneConstraint;
class P_ConstraintLoop;
class P_Source;
class P_Result;

class Generator : public ExprCopy, public virtual P_ExprVisitor {
public:
	Generator(const P_Source& source, P_Result& result);

	void visit(const P_NumConstraint& c);
	void visit(const P_OneConstraint& c);
	void visit(const P_ConstraintLoop& l);

protected:
	P_Result& result;

	const ExprNode& generate(const ExprNode& expr);

	void visit(const ExprNode& e);
	void visit(const ExprNAryOp& e);
	void visit(const ExprBinaryOp& b);
	void visit(const ExprUnaryOp& u);

	void visit(const ExprPower& e) ;
	void visit(const ExprIndex& i);
	void visit(const ExprSymbol& x);

	void visit(const P_ExprPower& c);
	void visit(const P_ExprIndex& e);
	void visit(const P_ExprSymbol& e);

	virtual ~Generator();

protected:
	std::stack<Scope> scopes;

	std::vector<std::pair<const ExprNode*, NumConstraint::CompOp> > ctrs;
};

} // end namespace parser
} // end namespace ibex

#endif // __IBEX_PARSER_GENERATOR_H__
