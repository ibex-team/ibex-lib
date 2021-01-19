//============================================================================
//                                  I B E X                                   
// File        : ibex_P_Scope.cpp
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Jun 12, 2012
// Last Update : Jun 12, 2012
//============================================================================

#include "ibex_P_Scope.h"
#include "ibex_P_Expr.h"
#include "ibex_P_NumConstraint.h"
#include "parser.tab.hh"

#include <sstream>

using namespace std;

namespace ibex {
namespace parser {

extern void init_symbol_domain(const char* destname, Domain& dest, const Domain& src);

/* root class of all objects linked to symbols */
class P_Scope::S_Object {
public:

	virtual ~S_Object() { }

	virtual S_Object* copy() const=0;

	virtual int token() const=0;

	virtual void print(ostream& os) const=0;
};

class P_Scope::S_Cst : public P_Scope::S_Object {
public:

	/*
	 * Initial constant creation
	 *
	 * --- Note for non-mutable constants: --
	 * The domain is passed by copy and the future
	 * node's domain will not be a reference to none of both:
	 * all objects created during parsing can be safely deleted.
	 *
	 * Caveat: domains are copied twice - could be heavy for
	 * large data. We cannot pass by reference here to avoid a
	 * copy because the domain in argument is built (parsed
	 * an generated) on the fly and will not remain in memory.
	 */
	S_Cst(const Domain& domain, bool _mutable) : domain(domain, _mutable), is_mutable(_mutable), _node(NULL) { }

	/*
	 * Creates and return the expression node.
	 *
	 * The creation is made on demand to avoid memory leaks
	 * (ex: when opening the scope of a function, all constants
	 * are duplicated. If nodes were created at this point,
	 * all unused constants will have their node lost in memory).
	 */
	const ExprConstant& node() {
		if (!_node) {
			if (is_mutable)
				_node = &ExprConstant::new_mutable(domain);
			else
				_node = &ExprConstant::new_(domain); // not a reference, see comment in constructor
		}
		return *_node;
	}

	bool node_built() const {
		return _node!=NULL;
	}

	S_Object* copy() const {
		return new S_Cst(*this);
	}

	int token() const { return TK_CONSTANT; }

	void print(ostream& os) const { os << "constant " << domain; }

	Domain domain;

	const bool is_mutable;

private:

	/**
	 * Copy constructor.
	 *
	 * Copies of constants are made for parsing an other and independent
	 * object (-> a function), so constants nodes must be re-created.
	 * By default: set to NULL to avoid memory leaks.
	 */
	S_Cst(const S_Cst& c) : domain(c.domain, c.is_mutable), is_mutable(c.is_mutable), _node(NULL) { }

	const ExprConstant* _node;

};

class P_Scope::S_Func : public P_Scope::S_Object {
public:
	S_Func(Function* f) : f(f) { }

	virtual S_Func* copy() const { return new S_Func(f); }

	int token() const { return TK_FUNC_SYMBOL; }

	void print(ostream& os) const { os << "function " << *f; }

	Function* f;
};

class P_Scope::S_ExprTmp : public P_Scope::S_Object {
public:
	S_ExprTmp(const ExprNode* expr) : expr(expr) { }

	S_Object* copy() const { return new S_ExprTmp(expr); }

	int token() const { return TK_EXPR_TMP_SYMBOL; }

	void print(ostream& os) const {
		if (expr)
			os << "expression tmp " << *expr;
		else
			os << "expression tmp (NULL)";
	}

	const ExprNode* expr;
};

class P_Scope::S_Var : public P_Scope::S_Object {
public:

	S_Var(const char* name, const Dim* dim, const Domain& value) : symbol(ExprSymbol::new_(name,*dim)), d(*dim) {
		init_symbol_domain(name, (Domain&) d, value);
	}

	S_Object* copy() const { return new S_Var(*this); }

	int token() const { return TK_ENTITY; }

	void print(ostream& os) const { os << "var " << symbol.name; }

	const ExprSymbol& symbol;
	const Domain d;

private:
	S_Var(const S_Var& e) : symbol(e.symbol), d(e.d) {  }
};

class P_Scope::S_Iterator : public P_Scope::S_Object {
public:

	S_Iterator(int value) : value(value) { }

	S_Object* copy() const { return new S_Iterator(value); }

	int token() const { return TK_ITERATOR; }

	void print(ostream& os) const { os << "iterator " << value; }

	int value;
};
/*====================================================================================================*/


P_Scope::P_Scope() {
	push();
}

P_Scope::P_Scope(const P_Scope& scope) {
	push();
	// we know that constants are necessarily in the bottom scope
	for (IBEXMAP(S_Object*)::const_iterator it=scope.tab.back().begin(); it!=scope.tab.back().end(); ++it) {
		if (it->second->token()==TK_CONSTANT || it->second->token()==TK_FUNC_SYMBOL) {
			tab.front().insert_new(it->first, it->second->copy());
		}
	}
}

void P_Scope::push() {
	tab.push_front(SymbolMap<S_Object*>());
}

void P_Scope::pop() {
	if (tab.empty()) {
		ibex_error("P_Scope: empty stack (please report bug)");
	}

	for (IBEXMAP(P_Scope::S_Object*)::const_iterator it2=tab.front().begin(); it2!=tab.front().end(); ++it2) {
		delete it2->second;
	}

	tab.pop_front();
}

P_Scope::~P_Scope() {
	while (!tab.empty()) pop();
}

P_Scope::S_Object* P_Scope::lookup(const char* id) {
	list<SymbolMap<S_Object*> >::iterator it=tab.begin();
	while (it!=tab.end()) {
		try {
			S_Object* o=(*it)[id];
			return o;
		} catch(SymbolMap<S_Object*>::SymbolNotFound& e) {
			// try next scope below
			++it;
		}
	}
	return NULL;
}

const P_Scope::S_Object* P_Scope::lookup(const char* id) const {
	return ((P_Scope*) this)->lookup(id);
}

void P_Scope::add_cst(const char* id, const Domain& domain, bool is_mutable) {
	//cout << "[parser] add cst " << id << "=" << domain << endl;
	tab.front().insert_new(id, new S_Cst(domain, is_mutable));
}

void P_Scope::add_func(const char* id, Function* f) {
	tab.front().insert_new(id, new S_Func(f));
	//cout << "[parser] add function " << *f << endl;
}

void P_Scope::add_expr_tmp_symbol(const char* id, const ExprNode* expr) {
	if (tab.front().used(id)) {
		ostringstream s;
		s << "P_Scope: temporary symbol \"" << id << "\" re-assigned in the same scope (please report bug)";
		ibex_error(s.str().c_str());
	}
	tab.front().insert_new(id, new S_ExprTmp(expr)); // can hide something declared in wider scope
}

void P_Scope::add_var(const char* id, const Dim* dim) {
	Domain d(Dim::scalar());
	d.i()=Interval::all_reals();
	add_var(id,dim,d);
	//cout << "[parser] add var " << id << endl;
}

void P_Scope::add_var(const char* id, const Dim* d, const Domain& domain) {
	S_Var* s = new S_Var(id,d,domain);
	tab.front().insert_new(id,s);
	vars.push_back(s);
}

void P_Scope::add_iterator(const char* id) {
	tab.front().insert_new(id, new S_Iterator(-1));
}

const Domain& P_Scope::get_cst(const char* id) const {
	const S_Object& o=*lookup(id);
	assert(o.token()==TK_CONSTANT);
	return ((const S_Cst&) o).domain;
}

const ExprConstant& P_Scope::get_cst_node(const char* id) {
	S_Object& o=*lookup(id);
	assert(o.token()==TK_CONSTANT);
	return ((S_Cst&) o).node();

}

Function& P_Scope::get_func(const char* id) {
	S_Object& s=*lookup(id);
	assert(s.token()==TK_FUNC_SYMBOL);
	return *((const S_Func&) s).f;
}

const ExprNode* P_Scope::get_tmp_expr_node(const char* id) const {
	const S_Object& s=*lookup(id);
	assert(s.token()==TK_EXPR_TMP_SYMBOL);
	return ((const S_ExprTmp&) s).expr;
}

std::vector<const ExprNode*> P_Scope::get_all_existing_nodes() const {
	std::vector<const ExprNode*> vec;
	for (list<SymbolMap<S_Object*> >::const_iterator it=tab.begin(); it!=tab.end(); ++it) {
		for (IBEXMAP(P_Scope::S_Object*)::const_iterator it2=it->begin(); it2!=it->end(); ++it2) {
			S_Object& o=*it2->second;
			if (o.token()==TK_EXPR_TMP_SYMBOL) {
				//cout << " add expression " << *((const S_ExprTmp&) *(it2->second)).expr <<endl;
				vec.push_back(((const S_ExprTmp&) o).expr);
			} else if (o.token()==TK_CONSTANT && ((const S_Cst&) o).node_built()) {
				vec.push_back((const ExprNode*) &(((S_Cst&) o).node()));
			}
		}
	}
	return vec;
}

std::pair<const ExprSymbol*,const Domain*> P_Scope::get_var(const char* id) const {
	const S_Object& o=*lookup(id);
	assert(o.token()==TK_ENTITY);
	const S_Var& s=(const S_Var&) o;
	return std::pair<const ExprSymbol*,const Domain*>(&s.symbol,&s.d);
}

Array<const Domain> P_Scope::var_domains() const {
	Array<const Domain> d(vars.size());
	for (unsigned int i=0; i<vars.size(); i++)
		d.set_ref(i,vars[i]->d);
	return d;
}

Array<const ExprSymbol> P_Scope::var_symbols() const {
	Array<const ExprSymbol> x(vars.size());
	for (unsigned int i=0; i<vars.size(); i++)
		x.set_ref(i,vars[i]->symbol);
	return x;
}


/*
const P_Scope::S_Iterator& P_Scope::get_iterator(const char* id) const {
	const S_Object& s=*tab[id];std::pair<const ExprSymbol*,const Domain
	assert(s.token()!=TK_ITERATOR);
	return (const S_Iterator&) s;
}*/

int P_Scope::get_iter_value(const char* id) const {
	const S_Object& s=*lookup(id);
	assert(s.token()==TK_ITERATOR);
	return ((const S_Iterator&) s).value;
}


void P_Scope::set_iter_value(const char* id, int value) {
	S_Object& s=*lookup(id);
	assert(s.token()==TK_ITERATOR);
	((S_Iterator&) s).value=value;
}


bool P_Scope::is_cst_symbol(const char* id) const {
	return lookup(id)->token()==TK_CONSTANT;
}

bool P_Scope::is_mutable_cst_symbol(const char* id) const {
	const S_Object& s=*lookup(id);
	return s.token()==TK_CONSTANT && ((const S_Cst&) s).domain.is_reference;
}

bool P_Scope::is_iter_symbol(const char* id) const {
	return lookup(id)->token()==TK_ITERATOR;
}

int P_Scope::token(const char* id) const {
	const S_Object* o=lookup(id);
	if (o)
		return o->token();
	else
		return TK_NEW_SYMBOL;
}


ostream& operator<<(ostream& os, const P_Scope& scope) {
	os << "Scopes :\n";
	for (list<SymbolMap<P_Scope::S_Object*> >::const_iterator it=scope.tab.begin(); it!=scope.tab.end(); ++it) {
		os << "----------------------------------------\n";
		for (IBEXMAP(P_Scope::S_Object*)::const_iterator it2=it->begin(); it2!=it->end(); ++it2) {
			os << "  " << it2->first << " ";
			it2->second->print(os);
			os << endl;
		}
		os << "----------------------------------------\n";
	}
	return os;
}

} // end namespace parser
} // end namespace ibex
