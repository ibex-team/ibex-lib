//============================================================================
//                                  I B E X                                   
// File        : ibex_ExprSize.cpp
// Author      : Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Jun 26, 2012
// Last Update : May 04, 2020
//============================================================================

#include "ibex_ExprSize.h"
#include "ibex_Expr.h"

using namespace std;

namespace ibex {

int ExprSize::bin_size(const ExprNode& l, const ExprNode& r) {
	return visit(l) + visit(r) +1;
}

int ExprSize::nary_size(const Array<const ExprNode>& args) {
	int acc=0;
	for (int i=0; i<args.size(); i++) acc+=visit(args[i]);
	return acc +1;
}
int ExprSize::visit(const ExprNode& e)     {
	IBEX_NODE_MAP(int)::iterator it = cache.find(e);
	if (it==cache.end()) {
		int size=e.accept_visitor(*this);
		cache.insert(e, size); // but the value will never be read again, in fact
		return size;
	} else {
		return 0;
	}
}
int ExprSize::visit(const ExprIndex& e)    { return visit(e.expr) +1; }
int ExprSize::visit(const ExprLeaf& e)     { return 1; }
int ExprSize::visit(const ExprNAryOp& e)   {
	int acc=0;
	for (int i=0; i<e.nb_args; i++) acc+=visit(e.arg(i));
	return acc+1;
}
int ExprSize::visit(const ExprBinaryOp& e) { return visit(e.left) + visit(e.right) +1; }
int ExprSize::visit(const ExprUnaryOp& e)  { return visit(e.expr) +1; }

} // end namespace ibex
