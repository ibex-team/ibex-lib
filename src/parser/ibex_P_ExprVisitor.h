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

class P_ExprNode;
class P_ExprWithIndex;
class P_ExprPower;
class P_ExprSum;
class P_ExprVarSymbol;
class P_ExprCstSymbol;
class P_ExprConstant;
class P_ExprIter;
class P_ExprApply;

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Woverloaded-virtual"
#endif

class P_ExprVisitor : public virtual ExprVisitor {
public:

	virtual ~P_ExprVisitor() {
	}

	/** Visit a parser leaf. */
	virtual void visit(const P_ExprNode&) {
		ibex_error("P_ExprVisitor: Missing implementation for some node type");
	}

	/** Visit a parser indexed expression. */
	virtual void visit(const P_ExprWithIndex& e) {
		visit((const P_ExprNode&) e);
	}

	/** Visit a parser power expression. */
	virtual void visit(const P_ExprPower& e) {
		visit((const P_ExprNode&) e);
	}

	/** Visit a parser sum expression. */
	virtual void visit(const P_ExprSum& e) {
		visit((const P_ExprNode&) e);
	}

	/** Visit a variable symbol expression. */
	virtual void visit(const P_ExprVarSymbol& e) {
		visit((const P_ExprNode&) e);
	}

	/** Visit a constant symbol expression. */
	virtual void visit(const P_ExprCstSymbol& e) {
		visit((const P_ExprNode&) e);
	}

	/** Visit a constant symbol expression. */
	virtual void visit(const P_ExprConstant& e) {
		visit((const P_ExprNode&) e);
	}

	/** Visit an iterator symbol. */
	virtual void visit(const P_ExprIter& e) {
		visit((const P_ExprNode&) e);
	}

	/** Visit an iterator symbol. */
	virtual void visit(const P_ExprApply& e) {
		visit((const P_ExprNode&) e);
	}
};

#ifdef __clang__
#pragma clang diagnostic pop
#endif

} // end namespace parser
} // end namespace ibex

#endif // __IBEX_PARSER_EXPR_VISITOR_H__
