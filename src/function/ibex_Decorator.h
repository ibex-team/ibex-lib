//============================================================================
//                                  I B E X                                   
// File        : ibex_Decorator.h
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Apr 4, 2012
// Last Update : Jul 12, 2012
//============================================================================

#ifndef __IBEX_DECORATOR_H__
#define __IBEX_DECORATOR_H__

#include "ibex_Domain.h"
#include "ibex_ExprVisitor.h"

namespace ibex {

class Function;

/**
 * \ingroup symbolic
 *
 * \brief Decorates a function.
 */
class Decorator : public ExprVisitor {
public:

	void decorate(const Function& f);

	virtual ~Decorator() { }

	/* Visit an expression. */
	virtual void visit(const ExprNode& n);
	/* Visit an indexed expression. */
	virtual void visit(const ExprIndex& idx);
	/* Visit a leaf. */
	virtual void visit(const ExprLeaf&);
	/* Visit an n-ary operator. */
	virtual void visit(const ExprNAryOp&);
	/* Visit an unary operator. */
	virtual void visit(const ExprUnaryOp&);
	/* Visit a binary operator. */
	virtual void visit(const ExprBinaryOp&);
};

/**
 * \ingroup level1
 * \brief Label of function application nodes for eval/HC4Revise.
 *
 * A function application node has a specific label that, in addition
 * to the domain, contains an evaluator of the function called.
 */
/*
class BasicApplyLabel : public virtual Domain {
public:
	BasicApplyLabel(const Dim& dim, Function& f);

	Domains args_doms; // domains of the arguments x (references)

	Function& f;       // the function f(x)

	HC4Revise fevl;    //  the associated evaluator
};
*/

} // end namespace ibex

#endif // __IBEX_DECORATOR_H__
