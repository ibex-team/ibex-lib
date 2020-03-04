//============================================================================
//                                  I B E X                                   
// File        : ibex_P_Scope.h
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Jun 12, 2012
// Last Update : Jun 12, 2012
//============================================================================

#ifndef __IBEX_PARSER_SCOPE_H__
#define __IBEX_PARSER_SCOPE_H__

#include "ibex_P_Expr.h"
#include "ibex_Expr.h"
#include "ibex_SymbolMap.h"

#include <iostream>
#include <list>

namespace ibex {

namespace parser {

class P_ExprConstant;

class P_Scope {

public:

	/**
	 * Create the stack of scopes, add the "bottom" scope
	 * and duplicates constants and function declared in
	 * another scope.
	 */
	P_Scope(const P_Scope&);

	/**
	 * Create a stack of scopes and push a new one.
	 */
	P_Scope();

	/**
	 * Create a new scope.
	 */
	void push();

	/**
	 * Pop a scope
	 */
	void pop();

	/** Delete this scope. */
	~P_Scope();

	/*------------- addition of new symbols in the current scope -----------*/
	/** Add a constant */
	void add_cst(const char* id, const Domain& domain, bool is_mutable);

	/** Add a function. */
	void add_func(const char* id, Function* f);

	/** Add a local variable in a function/constraint declaration.
	 * At parse time, expr can be set to NULL. */
	void add_expr_tmp_symbol(const char* tmp_symbol, const ExprNode* expr=NULL);

	/** Add a variable symbol (domain is (-oo,oo)x...). */
	void add_var(const char* id, const Dim* dim);

	/** Add a variable symbol. */
	void add_var(const char* id, const Dim* dim, const Domain& d);

	/** Add an (uninitialized) iterator. */
	void add_iterator(const char* id);

	/*------------- get data associated to symbols in the current scope -----------*/
	/*
	 * Return the domain associated to a constant.
	 */
	const Domain& get_cst(const char* id) const;

	/*
	 * Return the expression node associated to a constant.
	 * The node is created on first call.
	 */
	const ExprConstant& get_cst_node(const char* id);

	/* Return the function */
	Function& get_func(const char* id);

	/* Return the expression bound to a tmp symbol */
	const ExprNode* get_tmp_expr_node(const char* id) const;

	/*
	 * Return all the expression nodes that exist so far in the scope
	 * (either bound to tmp symbols or constant symbols).
	 *
	 * This function is useful to prevent these nodes from being
	 * destroyed if the appear in (non-const) expression that
	 * are generated and simplified on-the-fly (happens with "diff"
	 * operator).
	 *
	 * Does not include variables (but it could) as simplification of
	 * expression does not destroy variables.
	 *
	 * Note: nodes are not created for constants. They are only included
	 * in the vector if they already exist (i.e., if constants are already
	 * "generated" in a non-const expression).
	 */
	std::vector<const ExprNode*> get_all_existing_nodes() const;

	/* Return the symbol attached to a string */
	std::pair<const ExprSymbol*,const Domain*> get_var(const char* id) const;

	/** All the variable domains (in declaration order). */
	Array<const Domain> var_domains() const;

	/** All the variable symbols (in declaration order). */
	Array<const ExprSymbol> var_symbols() const;

	/* Return the value of the iterator */
	int get_iter_value(const char* id) const;

	/* Set the value of the iterator */
	void set_iter_value(const char* id, int value);

	/*---------------------------------------------------------------------*/
	/* Return the token associated to a symbol (used by lexer)
	 * either:
	 *    TK_ENTITY:           symbol of a variable (from the "variables" block)
	 *    TK_CONSTANT:         symbol of a constant (from the "constants" block)
	 *    TK_FUNC_SYMBOL:      name of a function declared in minibex.
	 *    TK_EXPR_TMP_SYMBOL:  left-hand symbol in an expression assignment like expr=f(x)
	 *    TK_ITERATOR:         name of an iterator in a "for" constraint loop or "sum" expression
	 */
	int token(const char* id) const;
	/*---------------------------------------------------------------------*/

	/* Return true if id is the symbol of a constant (mutable or not). */
	bool is_cst_symbol(const char* id) const;

	/* Return true if id is a mutable constant. */
	bool is_mutable_cst_symbol(const char* id) const;

	/* Return if id is the symbol of an interator */
	bool is_iter_symbol(const char* id) const;

private:

	/* classes used to contain objects bound to symbols */
	class S_Object;
	class S_Var;
	class S_Cst;
	class S_Func;
	class S_ExprTmp;
	class S_Iterator;

	/*
	 * Look up for and id, from top to bottom scope.
	 * Return NULL if not found.
	 */
	S_Object* lookup(const char* id);

	const S_Object* lookup(const char* id) const;

	friend std::ostream& operator<<(std::ostream& os, const P_Scope& scope);

	/*
	 * The list is filled in reverse order so that the
	 * "top" scope (the last created) is always front().
	 * This allows traversing scopes in the right order
	 * (the more recent first) by iterating in the list.
	 */
	std::list<SymbolMap<S_Object*> > tab;

	/**
	 * All the variables (in declaration order)
	 */
	std::vector<S_Var*> vars;
};

std::ostream& operator<<(std::ostream& os, const P_Scope& scope);

} // end namespace parser
} // end namespace ibex

#endif // __IBEX_PARSER_SCOPE_H__
