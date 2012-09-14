//============================================================================
//                                  I B E X                                   
// File        : ibex_ParserExprVisitor.h
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Jun 12, 2012
// Last Update : Jun 12, 2012
//============================================================================

#ifndef __IBEX_PARSER_EXPR_VISITOR_H__
#define __IBEX_PARSER_EXPR_VISITOR_H__

#include "ibex_ExprVisitor.h"

namespace ibex {

namespace parser {

class P_ExprPower;
class P_ExprIndex;
class ExprIter;
class ExprInfinity;

class P_ExprVisitor : public virtual ExprVisitor {
public:
	virtual ~P_ExprVisitor() { }

	/** Visit a parser power expression. */
	virtual void visit(const P_ExprPower&)=0;

	/** Visit a parser indexed expression. */
	virtual void visit(const P_ExprIndex&)=0;

	/** Visit an iterator symbol. */
	virtual void visit(const ExprIter&)=0;

	/** Visit +/-oo. */
	virtual void visit(const ExprInfinity&)=0;
};

} // end namespace parser
} // end namespace ibex

#endif // __IBEX_PARSER_EXPR_VISITOR_H__
