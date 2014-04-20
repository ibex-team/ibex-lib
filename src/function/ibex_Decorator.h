//============================================================================
//                                  I B E X                                   
// File        : ibex_Decorator.h
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Apr 4, 2012
// Last Update : Jul 16, 2012
//============================================================================

#ifndef __IBEX_DECORATOR_H__
#define __IBEX_DECORATOR_H__

#include "ibex_Domain.h"
#include "ibex_ExprVisitor.h"
#include "ibex_NodeMap.h"
#include "ibex_Affine2Domain.h"

namespace ibex {

class Function;

/**
 * \ingroup symbolic
 *
 * \brief Decorates a function.
 *
 * Decorating a node means initializing the #ibex::Expr::deco
 * field of this node.
 *
 * This class will decorate all the nodes of a function
 * <b>including the symbols nodes</b>.
 * Hence, not only the sub-nodes of the expression f(x) are
 * decorated: all the input variables, as nodes, are also
 * decorated. This is an important difference when some input symbols
 * do not appear in the expression, like (x,y)->x.
 */
class Decorator : public ExprVisitor {
public:
	/**
	 * \brief Decorates f.
	 */
	void decorate(const Array<const ExprSymbol>& x, const ExprNode& y);

	/**
	 * \brief Delete *this.
	 */
	virtual ~Decorator() { }

protected:
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
	/* Visit a constant. */
	virtual void visit(const ExprConstant&);
	/* Visit a symbol. */
	virtual void visit(const ExprSymbol&);

	// mark who is visited
	NodeMap<bool> map;
};

} // end namespace ibex

#endif // __IBEX_DECORATOR_H__
