//============================================================================
//                                  I B E X                                   
// File        : ibex_ConstantEvaluator.h
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Jun 19, 2012
// Last Update : Jun 19, 2012
//============================================================================

#ifndef __IBEX_CONSTANT_EVALUATOR_H__
#define __IBEX_CONSTANT_EVALUATOR_H__

#include "ibex_P_ExprVisitor.h"
#include "ibex_Expr.h"
#include "ibex_Scope.h"
#include "ibex_NodeMap.h"

namespace ibex {

namespace parser {

//int to_integer(const Domain& d);

class ConstantGenerator : public P_ExprVisitor {
public:
	ConstantGenerator(const Scope& scope);

	virtual ~ConstantGenerator();

	Domain eval(const ExprNode& expr);

	int eval_integer(const ExprNode& expr);

	double eval_double(const ExprNode& expr);

protected:
	void visit(const ExprNode& e);
	void visit(const ExprIndex& i);
	void visit(const ExprConstantRef&);
	void visit(const ExprLeaf& e);
	void visit(const ExprNAryOp& e);
	void visit(const ExprBinaryOp& b);
	void visit(const ExprUnaryOp& u);
	void visit(const ExprSymbol& x);
	void visit(const ExprConstant& c);
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
	void visit(const ExprTrans& e) ;
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

	void visit(const P_ExprPower& e);
	void visit(const P_ExprIndex& e);
	void visit(const ExprIter& i);
	void visit(const ExprInfinity&);

	void unary_eval(const ExprUnaryOp& e, Domain (*f)(const Domain&));
	void binary_eval(const ExprBinaryOp& e, Domain (*f)(const Domain&, const Domain&));

	const Scope& scope;
	NodeMap<Domain*> map;

	int number_type;
};

} // end namespace parser
} // end namespace ibex

#endif // __IBEX_CONSTANT_EVALUATOR_H__
