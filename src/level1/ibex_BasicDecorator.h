//============================================================================
//                                  I B E X                                   
// File        : ibex_BasicDecorator.h
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Apr 4, 2012
// Last Update : Apr 4, 2012
//============================================================================

#ifndef __IBEX_BASIC_DECORATOR_H__
#define __IBEX_BASIC_DECORATOR_H__

#include "ibex_Domain.h"
//#include "ibex_Function.h"
#include "ibex_Expr.h"
#include "ibex_HC4ReviseAlgo.h"
#include "ibex_Decorator.h"

namespace ibex {

/**
 * \ingroup level1
 * \brief Decorates a function for evaluation/HC4Revise.
 */
class BasicDecorator : public Decorator, public FunctionVisitor {
public:

	virtual ~BasicDecorator() { }

	virtual void decorate(Function& f) const;

protected:
	/* Visit an expression. */
	virtual void visit(const ExprNode& n);
	/* Visit an indexed expression. */
	virtual void visit(const ExprIndex& idx);
	/* Visit a symbol. */
	virtual void visit(const ExprSymbol& s);
	/* Visit a constant. */
	virtual void visit(const ExprConstant& c);
	/* Visit an n-ary operator. */
	virtual void visit(const ExprNAryOp&);
	/* Visit an unary operator. */
	virtual void visit(const ExprUnaryOp&);
	/* Visit a binary operator. */
	virtual void visit(const ExprBinaryOp&);
	/* Visit a vector of expressions. */
	virtual void visit(const ExprVector& v);
	/* Visit a function application. */
	virtual void visit(const ExprApply&);

};

/**
 * \ingroup level1
 * \brief Label of function application nodes for eval/HC4Revise.
 *
 * A function application node has a specific label that, in addition
 * to the domain, contains an evaluator of the function called.
 */
class BasicApplyLabel : public Domain {
public:
	BasicApplyLabel(const Dim& dim, Function& f);

	Domains args_doms;  // domains of the arguments (references)

	NumConstraint feq;  // the constraint f(x)=...

	HC4ReviseAlgo fevl; //  for each function, there is an associated evaluator
};

} // end namespace ibex

#endif // __IBEX_BASIC_DECORATOR_H__
