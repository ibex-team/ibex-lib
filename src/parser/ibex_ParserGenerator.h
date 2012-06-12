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
#include "ibex_ParserExpr.h"
#include "ibex_ParserScope.h"
#include "ibex_NumConstraint.h"
#include "ibex_Expr.h"

namespace ibex {

namespace parser {

class P_NumConstraint;
class P_OneConstraint;
class P_ConstraintLoop;

class P_Generator : public P_ExprVisitor {
public:
	P_Generator();

	const ExprNode& generate(const ExprNode& expr);

	void visit(const P_NumConstraint& c);
	void visit(const P_OneConstraint& c);
	void visit(const P_ConstraintLoop& l);

protected:

	void visit(const ExprNode& e);
	void visit(const ExprIndex& i);
	void visit(const ExprSymbol& x);
	void visit(const ExprConstant& c);
	void visit(const ExprNAryOp& e);
	void visit(const ExprBinaryOp& b);
	void visit(const ExprUnaryOp& u);
	void visit(const ExprVector& e);
	void visit(const ExprApply& e);

	void visit(const ExprAdd& e)   ;
	void visit(const ExprMul& e)   ;
	void visit(const ExprSub& e)   ;
	void visit(const ExprDiv& e)   ;
	void visit(const ExprMax& e)   ;
	void visit(const ExprMin& e)   ;
	void visit(const ExprAtan2& e) ;

	void visit(const ExprMinus& e) ;
	void visit(const ExprSign& e)  ;
	void visit(const ExprAbs& e)   ;
	void visit(const ExprPower& e) ;
	void visit(const ExprSqr& e)   ;
	void visit(const ExprSqrt& e)  ;
	void visit(const ExprExp& e)   ;
	void visit(const ExprLog& e)   ;
	void visit(const ExprCos& e)   ;
	void visit(const ExprSin& e)   ;
	void visit(const ExprTan& e)   ;
	void visit(const ExprCosh& e)  ;
	void visit(const ExprSinh& e)  ;
	void visit(const ExprTanh& e)  ;
	void visit(const ExprAcos& e)  ;
	void visit(const ExprAsin& e)  ;
	void visit(const ExprAtan& e)  ;
	void visit(const ExprAcosh& e) ;
	void visit(const ExprAsinh& e) ;
	void visit(const ExprAtanh& e) ;

	void visit_index(const ExprAdd& e)   ;
	void visit_index(const ExprMul& e)   ;
	void visit_index(const ExprSub& e)   ;
	void visit_index(const ExprDiv& e)   ;
	void visit_index(const ExprMax& e)   ;
	void visit_index(const ExprMin& e)   ;
	void visit_index(const ExprAtan2& e) ;

	void visit_index(const ExprMinus& e) ;
	void visit_index(const ExprSign& e)  ;
	void visit_index(const ExprAbs& e)   ;
	void visit_index(const ExprPower& e) ;
	void visit_index(const ExprSqr& e)   ;
	void visit_index(const ExprSqrt& e)  ;
	void visit_index(const ExprExp& e)   ;
	void visit_index(const ExprLog& e)   ;
	void visit_index(const ExprCos& e)   ;
	void visit_index(const ExprSin& e)   ;
	void visit_index(const ExprTan& e)   ;
	void visit_index(const ExprCosh& e)  ;
	void visit_index(const ExprSinh& e)  ;
	void visit_index(const ExprTanh& e)  ;
	void visit_index(const ExprAcos& e)  ;
	void visit_index(const ExprAsin& e)  ;
	void visit_index(const ExprAtan& e)  ;
	void visit_index(const ExprAcosh& e) ;
	void visit_index(const ExprAsinh& e) ;
	void visit_index(const ExprAtanh& e) ;

	void visit(const P_ExprConstant& c);
	void visit(const P_ExprIndex& e);

	virtual ~P_Generator();

protected:
	std::stack<Scope> scopes;

	std::vector<std::pair<const ExprNode*, NumConstraint::CompOp> > ctrs;

	/* Indicates the number of embedded indexed expression.
	 * When =0 means we are not inside an indexed expression.
	 */
	int index_expr;

	void check_not_index_expr(const char* op);
};

} // end namespace parser
} // end namespace ibex

#endif // __IBEX_PARSER_GENERATOR_H__
