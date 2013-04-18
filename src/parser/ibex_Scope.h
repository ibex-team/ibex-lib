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
#include "ibex_SymbolMap.h"

namespace ibex {

namespace parser {

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
	void add_cst(const char* id, const Dim& d, const Domain& dom);

	/** Remove a constant (constants can be overridden
	 * by local variables in functions)*/
	void rem_cst(const char* id);

	/** Add a function. */
	void add_func(const char* id, Function* f);

	/** Add an argument of a function. */
	void add_func_input(const char* input_symbol, const ExprSymbol* symbol);

	/** Add a local variable in a function. */
	void add_func_tmp_symbol(const char* tmp_symbol, const ExprNode* expr);

	/** Add a var, a syb or an epr */
	void add_entity(const char* id, const Entity* e);

	/** Add an (uninitialized) iterator. */
	void add_iterator(const char* id);

	/*------------- get data associated to symbols in the current scope -----------*/
	/* Return the constant */
	const Domain& get_cst(const char* id) const;

	/* Return the function */
	Function& get_func(const char* id);

	/* Return the input symbol of a function */
	const ExprSymbol& get_func_input_symbol(const char* id) const;

	/* Return the expression bound to a tmp symbol in a function */
	const ExprNode& get_func_tmp_expr(const char* id) const;

	/* Return the entity attached to a symbol */
	const Entity& get_entity(const char* id) const;

	/* Return the value of the iterator */
	int get_iter_value(const char* id) const;

	/* Set the value of the iterator */
	void set_iter_value(const char* id, int value);

	/*---------------------------------------------------------------------*/
	/* return the token associated to a symbol (used by lexer)
	 * either TK_SYMBOL, TK_CONSTANT, TK_TMP_SYMBOL, TK_FUNC_RETURN or TK_FUNC. */
	int token(const char* id) const;

	/* Return if id is the symbol of a constant */
	bool is_cst_symbol(const char* id) const;

	/* Return if id is the symbol of an interator */
	bool is_iter_symbol(const char* id) const;

	/* classes used to contain objects bound to symbols */
	class S_Object;

private:

	friend std::ostream& operator<<(std::ostream& os, const Scope& scope);

	SymbolMap<S_Object*> tab;
};

std::ostream& operator<<(std::ostream& os, const Scope& scope);

} // end namespace parser
} // end namespace ibex

#endif // __IBEX_SCOPE_H__
