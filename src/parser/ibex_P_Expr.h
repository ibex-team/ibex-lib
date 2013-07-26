//============================================================================
//                                  I B E X                                   
// File        : ibex_ParserExpr.h
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : May 25, 2012
// Last Update : May 25, 2012
//============================================================================

#ifndef __IBEX_PARSER_EXPR_H__
#define __IBEX_PARSER_EXPR_H__

#include <vector>
#include "ibex_Expr.h"
#include "ibex_IntervalMatrix.h"
#include "ibex_P_ExprVisitor.h"
#include "ibex_Entity.h"

namespace ibex {

namespace parser {

/**
 * \brief Power expression (for parser only).
 */
class P_ExprPower : public ExprBinaryOp {
public:
	P_ExprPower(const ExprNode& expr, const ExprNode& expon);

	virtual void acceptVisitor(ExprVisitor& v) const;
};

/**
 * \brief Indexed expression (for parser only).
 *
 * An indexed expression "at parse time" can be
 * something like x[i+1].
 *
 * If matlab_style is true, the index starts from 1;
 * otherwise it starts from 0.
 */
class P_ExprIndex : public ExprBinaryOp {
public:
	P_ExprIndex(const ExprNode& expr, const ExprNode& index, bool matlab_style);

	virtual void acceptVisitor(ExprVisitor& v) const;

	bool matlab_style;
};

/**
 * \brief Constant symbol expressions (for parser only)
 *
 * At parse time, the domains of constants
 * (those built from symbols) are references to
 * avoid duplication (consider, e.g., an interval
 * matrix array M appearing in many expressions).
 *
 * We could have also decided to adapt #ibex::ExprConstant
 * so that the Domain field "value" is a reference (Domain
 * can be references). However, it is more safe
 * to create a separate class for a separate usage.
 */
class ExprConstantRef : public ExprLeaf {
public:
	// should we add the name (for display?)
	// but then we have to build names like "x[i]"
	// when we build an ExprConstantRef from "x"
	// in ExprGenerator::visit(const P_ExprIndex&).
	ExprConstantRef(const Domain& d);

	virtual void acceptVisitor(ExprVisitor& v) const;

	// Points to the constant name (no copy)
	//const char* name;
	// A *reference* to the internal domain of d.
	Domain value;
};

/**
 * \brief Iterator symbol (for parser only).
 */
class ExprIter : public ExprLeaf {
public:
	ExprIter(const char* name);

	~ExprIter();

	virtual void acceptVisitor(ExprVisitor& v) const;

	const char* name;
};


/**
 * \brief The infinity constant (for parser only).
 *
 * This constant cannot be represented by an #ibex::ExprConstant
 * object because (+oo,+oo) is automatically
 * replaced by the empty set.
 */
class ExprInfinity : public ExprLeaf {
public:
	ExprInfinity();

	virtual void acceptVisitor(ExprVisitor& v) const;
};

void p_print(const ExprNode& e);

} // end namespace parser

} // end namespace ibex
#endif // __IBEX_PARSER_EXPR_H__
