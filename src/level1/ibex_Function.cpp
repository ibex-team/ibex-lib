/* =========é===================================================================
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

namespace {

class FindSymbolsUsed: public FunctionVisitor {
public:
	vector<int> keys;

	FindSymbolsUsed(const ExprNode& e) {
		e.acceptVisitor(*this);
	}

	virtual void visit(const ExprNode& e) {
		e.acceptVisitor(*this);
	}

	virtual void visit(const ExprIndex& e) {
		visit(e.expr);
	}

	virtual void visit(const ExprVector& e) {
		for (int i=0; i<e.size(); i++)
			visit(e.get(i));
	}

	virtual void visit(const ExprSymbol& e) {
		keys.push_back(e.key);
	}

	virtual void visit(const ExprConstant&) {
		// do nothing
	}

	virtual void visit(const ExprUnaryOp& e) {
		visit(e.expr);
	}

	virtual void visit(const ExprBinaryOp& e) {
		visit(e.left);
		visit(e.right);
	}

	virtual void visit(const ExprApply& a) {
		for (int i=0; i<a.nb_args(); i++) {
			visit(*a.args[i]);
		}
	}
};

}


Function::Function() : name(NULL), root(NULL), key_count(0) {

}

Function::Function(const char* name) : name(strdup(name)), root(NULL), key_count(0) {

}

Function* Function::separate() const {
	return 0;
}

const ExprSymbol& Function::add_symbol(const char* id) {
	return add_symbol(id, Dim(0,0,0));
}

const ExprSymbol& Function::add_symbol(const char* id, const Dim& dim) {

  if (id2info.used(id)) throw NonRecoverableException(std::string("Redeclared symbol \"")+id+"\"");

  int num = key_count;

  const ExprSymbol* sbl = &ExprSymbol::new_(*this,id,dim,num);

  id2info.insert_new(id, sbl);

  key_count ++;

  order2info.push_back(sbl);
  is_used.push_back(false); // unused by default

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
	FindSymbolsUsed fsu(expr);
	for (vector<int>::iterator it=fsu.keys.begin(); it!=fsu.keys.end(); it++) {
		is_used[*it]=true;
	}
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

std::ostream& operator<<(std::ostream& os, const Function& f) {
	if (f.name!=NULL) os << f.name << ":";
	os << "(";
	for (int i=0; i<f.nb_symbols(); i++) {
		os << f.symbol_name(i);
		if (i<f.nb_symbols()-1) os << ",";
	}
	os << ")->" << f.expr();
	return os;
}


} // namespace ibex
