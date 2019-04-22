//============================================================================
//                                  I B E X                                   
// File        : ibex_ExprGenerator.h
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Jun 19, 2012
// Last Update : May 06, 2016
//============================================================================

#ifndef __IBEX_PARSER_EXPR_GENERATOR_H__
#define __IBEX_PARSER_EXPR_GENERATOR_H__

#include "ibex_P_ExprVisitor.h"
#include "ibex_Expr.h"
#include "ibex_Scope.h"
#include "ibex_DoubleIndex.h"

namespace ibex {
namespace parser {

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Woverloaded-virtual"
#endif

class ExprGenerator : public virtual P_ExprVisitor {
public:
	ExprGenerator();

	Domain generate_cst(const P_ExprNode& y);

	int generate_int(const P_ExprNode& y);

	double generate_dbl(const P_ExprNode& y, bool round_downward);

	const ExprNode& generate(const P_ExprNode& y);

protected:
	void generate();

	void visit(const P_ExprNode&);

	// some nodes require more specific code:
	void visit(const P_ExprWithIndex&);
	void visit(const P_ExprPower&);
	void visit(const P_ExprSum&);
	const ExprNode& diff(const Array<const ExprNode>& args);

	std::pair<int,int> visit_index_tmp(const Dim& dim, const P_ExprNode& idx, bool matlab_style);
	DoubleIndex visit_index(const Dim& dim, const P_ExprNode& idx1, bool matlab_style);
	DoubleIndex visit_index(const Dim& dim, const P_ExprNode& idx1, const P_ExprNode& idx2, bool matlab_style);

	Scope& scope;
};

#ifdef __clang__
#pragma clang diagnostic pop
#endif

} // end namespace parser
} // end namespace ibex

#endif // __IBEX_PARSER_EXPR_GENERATOR_H__
