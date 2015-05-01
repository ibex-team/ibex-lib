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

#include "ibex_P_ExprVisitor.h"
#include "ibex_Scope.h"
#include "ibex_ExprCopy.h"

namespace ibex {
namespace parser {

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Woverloaded-virtual"
#endif

class ExprGenerator : private ExprCopy, public virtual P_ExprVisitor {
public:
	ExprGenerator(const Scope& scope);

	const ExprNode& generate(const Array<const ExprSymbol>& old_x, const Array<const ExprSymbol>& new_x, const ExprNode& y);

protected:

	void visit(const ExprNode& e);
	void visit(const P_ExprPower& c);
	void visit(const P_ExprIndex& e);
	void visit(const ExprConstantRef&);
	void visit(const ExprIter& i);
	void visit(const ExprInfinity&);

	const Scope& scope;
};

#ifdef __clang__
#pragma clang diagnostic pop
#endif

} // end namespace parser
} // end namespace ibex
#endif // IBEX_EXPRGENERATOR_H_
