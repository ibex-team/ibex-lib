//============================================================================
//                                  I B E X                                   
// File        : ibex_Scope.h
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Jun 12, 2012
// Last Update : Jun 12, 2012
//============================================================================

#ifndef __IBEX_SCOPE_H__
#define __IBEX_SCOPE_H__

#include <iostream>
#include "ibex_P_Expr.h"
#include "ibex_Expr.h"
#include "ibex_SymbolMap.h"

namespace ibex {

namespace parser {

class P_ExprConstant;

class Scope {

public:

	/** New empty scope */
	Scope();

	/** Duplicacte the scope */
	Scope(const Scope& scope);

	/** Only copy global data (constants and functions) */
	Scope(const Scope& scope, bool global);

	/** Delete this scope. */
	~Scope();

	/*------------- addition of new symbols in the current scope -----------*/
	/** Add a constant */
	void add_cst(const char* id, const Domain& domain);

	/** Add a constant */
	void add_cst(const char* id, const Dim* d, const Domain& dom);

	/** Remove a constant (constants can be overridden
	 * by local variables in functions)*/
	void rem_cst(const char* id);

	/** Add a function. */
	void add_func(const char* id, Function* f);

	/** Add a local variable in a function/constraint declaration. */
	void add_expr_tmp_symbol(const char* tmp_symbol, const P_ExprNode* expr);

	/** Add a variable symbol (domain is (-oo,oo)x...). */
	void add_var(const char* id, const Dim* dim);

	/** Add a variable symbol. */
	void add_var(const char* id, const Dim* dim, const Domain& d);

	/** Add an (uninitialized) iterator. */
	void add_iterator(const char* id);

	/** Remove an iterator
	 */
	void rem_iterator(const char* id);

	//void add_operator(const char* id);

	/*------------- get data associated to symbols in the current scope -----------*/

//	/* Bind an ExprNode object to a constant symbol */
//	void bind_cst_node(const char* id, const ExprConstant&);

	/* Return the constant */
	const ExprConstant& get_cst(const char* id) const;

	/* Return the function */
	Function& get_func(const char* id);

	/* Return the expression bound to a tmp symbol */
	const P_ExprNode& get_expr_tmp_expr(const char* id) const;

	/* Return the symbol attached to a string */
	std::pair<const ExprSymbol*,const Domain*> get_var(const char* id) const;

	/** Name of the ith variable (for CHOCO) */
	const char* var(int i) const;

	/** Number of variables. */
	int nb_var() const;

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
	 *    TK_ITERATOR:         name of an iterator in a "for" constraint loop
	 */
	int token(const char* id) const;
	/*---------------------------------------------------------------------*/

	/* Return if id is the symbol of a constant */
	bool is_cst_symbol(const char* id) const;

	/* Return if id is the symbol of an interator */
	bool is_iter_symbol(const char* id) const;

	/** All the constant symbols (in declaration order).*/
	std::vector<const char*> cst;

private:
	/* classes used to contain objects bound to symbols */
	class S_Object;
	class S_Entity;
	class S_Cst;
	class S_Func;
	class S_ExprTmp;
	class S_Iterator;

	/** All the variables (in declaration order).*/
	std::vector<S_Entity*> vars;

	friend std::ostream& operator<<(std::ostream& os, const Scope& scope);

	SymbolMap<S_Object*> tab;
};

std::ostream& operator<<(std::ostream& os, const Scope& scope);

} // end namespace parser
} // end namespace ibex

#endif // __IBEX_SCOPE_H__
