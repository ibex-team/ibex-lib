//============================================================================
//                                  I B E X                                   
// File        : ibex_ExprGenerator.h
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Jun 19, 2012
// Last Update : Jun 19, 2012
//============================================================================

#ifndef __IBEX_EXPR_GENERATOR_H__
#define __IBEX_EXPR_GENERATOR_H__

#include "ibex_ParserExprVisitor.h"
#include "ibex_ParserScope.h"
#include "ibex_ExprCopy.h"

namespace ibex {
namespace parser {

class ExprGenerator : private ExprCopy, public virtual P_ExprVisitor {
public:
	ExprGenerator(const Scope& scope);

	const ExprNode& generate(const Array<const ExprSymbol>& x, const ExprNode& y);

protected:

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

	const Scope& scope;
};

} // end namespace parser
} // end namespace ibex
#endif // IBEX_EXPRGENERATOR_H_
