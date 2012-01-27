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
#include <limits.h>

namespace ibex {

ExprNode::ExprNode(Function& env, int height, int size, const Dim& dim) :
  context(context), height(height), size(size), id(context.nb_nodes()), dim(dim), deco(NULL) {
  env.add_node(*this);
}

std::ostream& operator<<(std::ostream& os, const Function& expr) {
  //ExprPrinter(os).visit(expr);
	// TO DO
  return os;
}
} // end namespace ibex
