//============================================================================
//                                  I B E X                                   
// File        : ibex_Expr2Minibex.cpp
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Oct 7, 2015
//============================================================================

#include "ibex_Expr2Minibex.h"
#include "ibex_ExprSubNodes.h"

namespace ibex {

void Expr2Minibex::print(std::ostream& os, const ExprNode& e, bool human) {
	ExprSubNodes nodes(e);

	int tmp=0; // counter for the intermediate symbols

	this->os = &os;
	this->human = human;

	for (int i=nodes.size()-1; i>=0; i--) {
		const ExprNode& node=nodes[i];
		if (node.fathers.size()>1) {

			// we don't create new symbols for the arguments of the expression,
			// or scalar constants
			if (node.height==0) {
				const ExprConstant* c = dynamic_cast<const ExprConstant*>(&node);
				if (!c || c->dim.is_scalar()) continue;
			}

			os << "  _tmp_" << tmp << "_ = ";
			visit(node);
			os << ";" << std::endl;
			map.insert(node, tmp++);
		}
	}

	os << "  return ";

	visit(e);

	os << ";";
}

void Expr2Minibex::visit(const ExprNode& e) {
	if (map.found(e))
		(*os) << "_tmp_" << map[e] << "_";
	else
		e.acceptVisitor(*this);
}


} // namespace ibex
