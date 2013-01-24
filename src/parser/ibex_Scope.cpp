//============================================================================
//                                  I B E X                                   
// File        : ibex_Scope.cpp
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Jun 12, 2012
// Last Update : Jun 12, 2012
//============================================================================

#include "ibex_Scope.h"
#include "ibex_P_NumConstraint.h"
#include "parser.tab.hh"

/*
extern int TK_CONSTANT;

extern const int TK_FUNC_SYMBOL;
extern const int TK_FUNC_INP_SYMBOL;
extern const int TK_FUNC_RET_SYMBOL;
extern const int TK_FUNC_TMP_SYMBOL;

extern const int TK_ENTITY;
extern const int TK_ITERATOR;
 */

using namespace std;

namespace ibex {
namespace parser {

extern void init_symbol_domain(const char* destname, Domain& dest, const Domain& src);

/* root class of all objects linked to symbols */
class Scope::S_Object {
public:

	virtual ~S_Object() { }

	virtual S_Object* copy() const=0;

	virtual int token() const=0;

	virtual void print(ostream& os) const=0;
};

class S_Cst : public Scope::S_Object {
public:

	S_Cst(const Domain& domain) : domain(domain) { }

	S_Cst(const Dim& d) : domain(d) { }

	S_Object* copy() const { return new S_Cst(*this); }

	int token() const { return TK_CONSTANT; }

	void print(ostream& os) const { os << "constant " << domain; }

	Domain domain;

private:
	/**
	 * Important: since, in the parser expressions, constants are ExprConstantRef
	 * instead of ExprConstant, we must keep the same references when duplicating constants here.
	 * Otherwise, in a "for" block, the ExprConstantRef inside expressions will point to domains
	 * that will no longer exist after parsing (that is, once the scope of the "for" will be deleted).
	 */
	S_Cst(const S_Cst& c) : domain(c.domain,true) { }
};

class S_Func : public Scope::S_Object {
public:
	S_Func(Function* f) : f(f) { }

	virtual S_Func* copy() const { return new S_Func(f); }

	int token() const { return TK_FUNC_SYMBOL; }

	void print(ostream& os) const { os << "function " << *f; }

	Function* f;
};

class S_FuncInput : public Scope::S_Object {
public:

	S_FuncInput(const ExprSymbol* symbol) : symbol(symbol) { }

	S_Object* copy() const { return new S_FuncInput(symbol); }

	int token() const { return TK_FUNC_INP_SYMBOL; }

	void print(ostream& os) const { os << "function input"; }

	const ExprSymbol* symbol;
};

class S_FuncTmp : public Scope::S_Object {
public:
	S_FuncTmp(const ExprNode* expr) : expr(expr) { }

	S_Object* copy() const { return new S_FuncTmp(expr); }

	int token() const { return TK_FUNC_TMP_SYMBOL; }

	void print(ostream& os) const { os << "function tmp " << *expr; }

	const ExprNode* expr;
};

class S_Entity : public Scope::S_Object {
public:

	S_Entity(const Entity* symbol) : symbol(symbol) { }

	S_Object* copy() const { return new S_Entity(symbol); }

	int token() const { return TK_ENTITY; }

	void print(ostream& os) const { os << "entity"; }

	const Entity* symbol;
};

class S_Iterator : public Scope::S_Object {
public:

	S_Iterator(int value) : value(value) { }

	S_Object* copy() const { return new S_Iterator(value); }

	int token() const { return TK_ITERATOR; }

	void print(ostream& os) const { os << "iterator " << value; }

	int value;
};
/*====================================================================================================*/


Scope::Scope() { }

Scope::Scope(const Scope& scope) {
  for (IBEXMAP(S_Object*)::const_iterator it=scope.tab.begin(); it!=scope.tab.end(); it++) {
      tab.insert_new(it->first, it->second->copy());
  }
}

Scope::Scope(const Scope& scope, bool global) {
  for (IBEXMAP(S_Object*)::const_iterator it=scope.tab.begin(); it!=scope.tab.end(); it++) {
    if (global && it->second->token()!=TK_ENTITY)
      tab.insert_new(it->first, it->second->copy());
  }
}

Scope::~Scope() {
  for (IBEXMAP(S_Object*)::const_iterator it=tab.begin(); it!=tab.end(); it++)
    delete it->second;
}

/*====================================================================================================*/


void Scope::add_cst(const char* id, const Domain& domain) {
	tab.insert_new(id, new S_Cst(domain));
}

void Scope::add_cst(const char* id, const Dim& d, const Domain& dom) {
	S_Cst* c=new S_Cst(d);
	tab.insert_new(id, c);
	init_symbol_domain(id, c->domain, dom);
}

void Scope::rem_cst(const char* id) {
	assert(tab[id]->token()==TK_CONSTANT);
	delete tab[id];
	tab.erase(id);
}

void Scope::add_func(const char* id, Function* f) {
	tab.insert_new(id, new S_Func(f));
}

void Scope::add_func_input(const char* id, const ExprSymbol* symbol) {
	tab.insert_new(id, new S_FuncInput(symbol));
}

void Scope::add_func_tmp_symbol(const char* id, const ExprNode* expr) {
	tab.insert_new(id, new S_FuncTmp(expr));
}

void Scope::add_entity(const char* id, const Entity* symbol) {
	tab.insert_new(id, new S_Entity(symbol));
}

void Scope::add_iterator(const char* id) {
	tab.insert_new(id, new S_Iterator(-1));
}

const Domain& Scope::get_cst(const char* id) const {
	const S_Object& s=*tab[id];
	assert(s.token()==TK_CONSTANT);
	return ((const S_Cst&) s).domain;
}

Function& Scope::get_func(const char* id) {
	S_Object& s=*tab[id];
	assert(s.token()==TK_FUNC_SYMBOL);
	return *((const S_Func&) s).f;
}

const ExprSymbol& Scope::get_func_input_symbol(const char* id) const {
	const S_Object& s=*tab[id];
	assert(s.token()==TK_FUNC_INP_SYMBOL);
	return *((const S_FuncInput&) s).symbol;
}

const ExprNode& Scope::get_func_tmp_expr(const char* id) const {
	const S_Object& s=*tab[id];
	assert(s.token()==TK_FUNC_TMP_SYMBOL);
	return *((const S_FuncTmp&) s).expr;
}

const Entity& Scope::get_entity(const char* id) const {
	const S_Object& s=*tab[id];
	assert(s.token()==TK_ENTITY);
	return *((const S_Entity&) s).symbol;
}

/*
const Scope::S_Iterator& Scope::get_iterator(const char* id) const {
	const S_Object& s=*tab[id];
	assert(s.token()!=TK_ITERATOR);
	return (const S_Iterator&) s;
}*/

int Scope::get_iter_value(const char* id) const {
	const S_Object& s=*tab[id];
	assert(s.token()==TK_ITERATOR);
	return ((const S_Iterator&) s).value;
}


void Scope::set_iter_value(const char* id, int value) {
	S_Object& s=*tab[id];
	assert(s.token()==TK_ITERATOR);
	((S_Iterator&) s).value=value;
}


bool Scope::is_cst_symbol(const char* id) const {
	return tab[id]->token()==TK_CONSTANT;
}

bool Scope::is_iter_symbol(const char* id) const {
	return tab[id]->token()==TK_ITERATOR;
}

int Scope::token(const char* id) const {
  if (tab.used(id))
    return tab[id]->token();
  else
    return TK_NEW_SYMBOL;
}


ostream& operator<<(ostream& os, const Scope& scope) {
	os << "current scope :\n";
	os << "--------------------\n";
	for (IBEXMAP(Scope::S_Object*)::const_iterator it=scope.tab.begin(); it!=scope.tab.end(); it++) {
		os << "  " << it->first << " ";
		it->second->print(os);
		os << endl;
	}
	os << "--------------------\n";
	return os;
}

} // end namespace parser
} // end namespace ibex
