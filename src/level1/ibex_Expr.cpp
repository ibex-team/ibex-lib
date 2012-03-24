/* ============================================================================
 * I B E X - Expressions
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Jan 5, 2012
 * ---------------------------------------------------------------------------- */

#include "ibex_Expr.h"
#include "ibex_Function.h"
#include "ibex_NonRecoverableException.h"
#include "ibex_ExprPrinter.cpp_"
#include <limits.h>

namespace ibex {

ExprNode::ExprNode(Function& env, int height, int size, const Dim& dim) :
  context(env), height(height), size(size), id(context.nb_nodes()), dim(dim), deco(NULL) {
  env.add_node(*this);
}

namespace {
int max_height(int n, const ExprNode** args) {
	int max=0;
	for (int i=0; i<n; i++)
		if (args[i]->height > max) max = args[i]->height;
	return max;
}

int sum_size(int n, const ExprNode** args) {
	int sum=0;
	for (int i=0; i<n; i++)
		sum += args[i]->size;
	return sum;
}
}

ExprApply::ExprApply(const Function& f, const ExprNode** args) :
		ExprNode(	args[0]->context,
					max_height(f.nb_symbols(),args),
					sum_size(f.nb_symbols(),args),
					f.expr().dim ),
		func(f), args(args ) {
}

int ExprApply::nb_args() const {
	return func.nb_symbols();
}

std::ostream& operator<<(std::ostream& os, const ExprNode& expr) {
  ExprPrinter(os).visit(expr);
  return os;
}
} // end namespace ibex
