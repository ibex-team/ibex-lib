/* ============================================================================
 * I B E X - Functions
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Jan 5, 2012
 * ---------------------------------------------------------------------------- */

#include "ibex_Function.h"
#include "ibex_Expr.h"

namespace ibex {

Function::Function() : root(NULL), key_count(0) {

}

const ExprSymbol& Function::add_symbol(const char* id) {
	return add_symbol(id, Dim(0,0,1));
}

const ExprSymbol& Function::add_symbol(const char* id, const Dim& dim) {

  if (id2info.used(id)) throw NonRecoverableException(std::string("Redeclared symbol \"")+id+"\"");

  int num = key_count;

  const ExprSymbol* sbl = &ExprSymbol::new_(*this,id,dim,num);

  id2info.insert_new(id, sbl);

  key_count ++;

  order2info.push_back(sbl);

  return *sbl;
}

int Function::nb_nodes() const {
	return exprnodes.size();
}

void Function::add_node(const ExprNode& expr)  {
	exprnodes.push_back(&expr);
}


void Function::set_expr(const ExprNode& expr) {
	root = &expr;
}

const ExprApply& Function::operator()(const ExprNode** args) {
	return ExprApply::new_(*this, args);
}

const char* Function::symbol_name(int i) const {
	return order2info[i]->name;
}

const ExprNode& Function::expr() const {
	return *root;
}


} // namespace ibex
