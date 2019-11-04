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

/*
extern int TK_CONSTANT;

extern const int TK_FUNC_SYMBOL;
extern const int TK_FUNC_RET_SYMBOL;
extern const int TK_EXPR_TMP_SYMBOL;

extern const int TK_ENTITY;
extern const int TK_ITERATOR;
 */

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

	S_Cst(const Domain& domain) : node(ExprConstant::new_(domain)), cst(domain) { }

//	void bind(const ExprConstant& c) { node = &c; }

	S_Object* copy() const { return new S_Cst(*this); }

	int token() const { return TK_CONSTANT; }

	void print(ostream& os) const { os << "constant " << cst; }

	const ExprConstant& node;
	Domain cst;

private:
	/**
	 * Important: since, in the parser expressions, constants are ExprConstantRef
	 * instead of ExprConstant, we must keep the same references when duplicating constants here.
	 * Otherwise, in a "for" block, the ExprConstantRef inside expressions will point to domains
	 * that will no longer exist after parsing (that is, once the scope of the "for" will be deleted).
	 */
	S_Cst(const S_Cst& c) : node(c.node), cst(c.cst,true) { }
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

	void print(ostream& os) const { os << "expression tmp " << *expr; }

	const ExprNode* expr;
};

class P_Scope::S_Entity : public P_Scope::S_Object {
public:

	S_Entity(const char* name, const Dim* dim, const Domain& value) : symbol(ExprSymbol::new_(name,*dim)), d(*dim) {
		init_symbol_domain(name, (Domain&) d, value);
	}

	S_Object* copy() const { return new S_Entity(*this); }

	int token() const { return TK_ENTITY; }

	void print(ostream& os) const { os << "entity"; }

	const ExprSymbol& symbol;
	const Domain d;

private:
	S_Entity(const S_Entity& e) : symbol(e.symbol), d(e.d) {  }
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


P_Scope::P_Scope() { }

P_Scope::P_Scope(const P_Scope& scope) {
  for (IBEXMAP(S_Object*)::const_iterator it=scope.tab.begin(); it!=scope.tab.end(); it++) {
      tab.insert_new(it->first, it->second->copy());
  }
}

P_Scope::P_Scope(const P_Scope& scope, bool global) {
  for (IBEXMAP(S_Object*)::const_iterator it=scope.tab.begin(); it!=scope.tab.end(); it++) {
    if (global && it->second->token()!=TK_ENTITY)
      tab.insert_new(it->first, it->second->copy());
  }
  /** TODO copy of vars/csts */
}

P_Scope::~P_Scope() {
  for (IBEXMAP(S_Object*)::const_iterator it=tab.begin(); it!=tab.end(); it++)
    delete it->second;
}

/*====================================================================================================*/


void P_Scope::add_cst(const char* id, const Domain& domain) {
	//cout << "[parser] add cst " << id << "=" << domain << endl;
	const char* id_copy=tab.insert_new(id, new S_Cst(domain));
	cst.push_back(id_copy);
}

void P_Scope::add_cst(const char* id, const Dim* dim, const Domain& dom) {
	Domain tmp(*dim);
	//cout << "[parser] add cst " << id << "=" << dom.dim << "<>" << *dim << endl;
	init_symbol_domain(id, tmp, dom);
	const char* id_copy=tab.insert_new(id, new S_Cst(tmp));
	cst.push_back(id_copy);
}

//void P_Scope::bind_cst_node(const char* id, const ExprConstant& node) {
//	const S_Object& s=*tab[id];
//	assert(s.token()==TK_CONSTANT);
//	((S_Cst&) s).bind(node);
//}

void P_Scope::rem_cst(const char* id) {
	assert(tab[id]->token()==TK_CONSTANT);
	delete &get_cst(id);
	delete tab[id];
	tab.erase(id);
}

void P_Scope::add_func(const char* id, Function* f) {
	tab.insert_new(id, new S_Func(f));
	//cout << "[parser] add function " << *f << endl;
}

void P_Scope::add_expr_tmp_symbol(const char* id, const ExprNode* expr) {
	tab.insert_new(id, new S_ExprTmp(expr));
}

void P_Scope::add_var(const char* id, const Dim* dim) {
	Domain d(Dim::scalar());
	d.i()=Interval::all_reals();
	add_var(id,dim,d);
	//cout << "[parser] add var " << id << endl;
}

void P_Scope::add_var(const char* id, const Dim* d, const Domain& domain) {
	S_Entity* s = new S_Entity(id,d,domain);
	tab.insert_new(id,s);
	vars.push_back(s);
}

void P_Scope::add_iterator(const char* id) {
	tab.insert_new(id, new S_Iterator(-1));
}

void P_Scope::rem_iterator(const char* id) {
	assert(tab[id]->token()==TK_ITERATOR);
	delete tab[id];
	tab.erase(id);
}

//std::pair<const ExprConstant*, const Domain*> P_Scope::get_cst(const char* id) const {
const ExprConstant& P_Scope::get_cst(const char* id) const {
	const S_Object& o=*tab[id];
	assert(o.token()==TK_CONSTANT);
	const S_Cst& s=(const S_Cst&) o;
	//return std::pair<const ExprConstant*,const Domain*>(s.node,&s.cst);
	return s.node;
}

Function& P_Scope::get_func(const char* id) {
	S_Object& s=*tab[id];
	assert(s.token()==TK_FUNC_SYMBOL);
	return *((const S_Func&) s).f;
}

const ExprNode* P_Scope::get_expr_tmp_expr(const char* id) const {
	const S_Object& s=*tab[id];
	assert(s.token()==TK_EXPR_TMP_SYMBOL);
	return ((const S_ExprTmp&) s).expr;
}

std::pair<const ExprSymbol*,const Domain*> P_Scope::get_var(const char* id) const {
	const S_Object& o=*tab[id];
	assert(o.token()==TK_ENTITY);
	const S_Entity& s=(const S_Entity&) o;
	return std::pair<const ExprSymbol*,const Domain*>(&s.symbol,&s.d);
}

const char* P_Scope::var(int i) const {
	return vars[i]->symbol.name;
}

int P_Scope::nb_var() const {
	return vars.size();
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
	const S_Object& s=*tab[id];
	assert(s.token()==TK_ITERATOR);
	return ((const S_Iterator&) s).value;
}


void P_Scope::set_iter_value(const char* id, int value) {
	S_Object& s=*tab[id];
	assert(s.token()==TK_ITERATOR);
	((S_Iterator&) s).value=value;
}


bool P_Scope::is_cst_symbol(const char* id) const {
	return tab[id]->token()==TK_CONSTANT;
}

bool P_Scope::is_iter_symbol(const char* id) const {
	return tab[id]->token()==TK_ITERATOR;
}

int P_Scope::token(const char* id) const {
  if (tab.used(id))
    return tab[id]->token();
  else
    return TK_NEW_SYMBOL;
}


ostream& operator<<(ostream& os, const P_Scope& scope) {
	os << "current scope :\n";
	os << "--------------------\n";
	for (IBEXMAP(P_Scope::S_Object*)::const_iterator it=scope.tab.begin(); it!=scope.tab.end(); it++) {
		os << "  " << it->first << " ";
		it->second->print(os);
		os << endl;
	}
	os << "--------------------\n";
	return os;
}

} // end namespace parser
} // end namespace ibex
