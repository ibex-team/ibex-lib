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

#include <cstring>

namespace ibex {

void Expr2Minibex::print(std::ostream& os, const ExprNode& e, bool human) {
	ExprSubNodes nodes(e);

	// does a node have several fathers? --> if yes, create a temporary variable
	unsigned int* father_count = new unsigned int[nodes.size()];
	memset(father_count,0,nodes.size()*sizeof(unsigned int));

	for (int i=0; i<nodes.size(); i++) {
		if (dynamic_cast<const ExprNAryOp*>(&nodes[i])) {
			const ExprNAryOp& nary=(const ExprNAryOp&) nodes[i];
			for (int j=0; j<nary.nb_args; j++)
				father_count[nodes.rank(nary.args[j])]++;
		} else if (dynamic_cast<const ExprBinaryOp*>(&nodes[i])) {
			const ExprBinaryOp& b=(const ExprBinaryOp&) nodes[i];
			father_count[nodes.rank(b.left)]++;
			father_count[nodes.rank(b.right)]++;
		} else if (dynamic_cast<const ExprUnaryOp*>(&nodes[i])) {
			const ExprUnaryOp& u=(const ExprUnaryOp&) nodes[i];
			father_count[nodes.rank(u.expr)]++;
		} else if (dynamic_cast<const ExprIndex*>(&nodes[i])) {
			const ExprIndex& index=(const ExprIndex&) nodes[i];
			father_count[nodes.rank(index.expr)]++;
		}
	}

	int tmp=0; // counter for the intermediate symbols

	this->os = &os;
	this->human = human;

	for (int i=nodes.size()-1; i>=0; i--) {
		const ExprNode& node=nodes[i];
		if (father_count[i]>1) {

			// we don't create new symbols for the arguments of the expression,
			// or scalar constants
			if (node.height==0) {
				const ExprConstant* c = dynamic_cast<const ExprConstant*>(&node);
				if (!c || c->dim.is_scalar()) continue;
			}

			os << "  _tmp_" << std::dec << tmp << "_ = ";
			visit(node);
			os << ";" << std::endl;
			map.insert(node, tmp++);
		}
	}

	os << "  return ";

	visit(e);

	os << ";";

	delete[] father_count;
}

void Expr2Minibex::visit(const ExprNode& e) {
	if (map.found(e))
		(*os) << "_tmp_" << std::dec << map[e] << "_";
	else
		e.accept_visitor(*this);
}


} // namespace ibex
