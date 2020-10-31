//============================================================================
//                                  I B E X                                   
// File        : ibex_P_Struct.h
// Author      : Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Sep 9, 2012
// Last Update : Nov 3, 2019
//============================================================================

#ifndef __IBEX_PARSER_STRUCT_H__
#define __IBEX_PARSER_STRUCT_H__

#include "ibex_P_NumConstraint.h"
#include "ibex_P_Source.h"
#include "ibex_P_Scope.h"

#include "ibex_System.h"

#include <stack>

int ibexlex(void);

// note: do not confuse with ibex_error in tools/ibex_Exception.h
void ibexerror (const std::string& msg);

namespace ibex {

namespace parser {

void init_symbol_domain(const char* destname, Domain& dest, const Domain& src);

void init_function_by_copy(Function& dest, const Function& src);

/**
 * \ingroup parser
 *
 * Result of parsing.
 */
class P_Struct {
public:

	P_Struct();

	/**
	 * Get the current scope.
	 */
	P_Scope& scope();

	/**
	 * Delete source and all scopes.
	 */
	virtual ~P_Struct();

	/**
	 * Initialize parsing of a Minibex file
	 *
	 * Set locale, create new scope, etc.
	 */
	virtual void begin();

	/**
	 * Create a new scope for a function declaration
	 */
	void begin_function();

	/**
	 * End of parsing
	 *
	 * Generates the result.
	 */
	virtual void end();

	/**
	 * Cleanup the scope created for the function
	 * and set the current scope to the global one.
	 */
	void end_function(const char* name, const P_ExprNode& y);

	/**
	 * The Minibex source
	 */
	P_Source source;

protected:
	/*
	 * The stack of all scopes
	 *
	 * A bottom scope is automatically created and is
	 * kept for both parsing and generation (the constants and functions
	 * are shared by the two steps)
	 *
	 * note: other scopes are created for parsing the constraints block,
	 * to store temporary variables and iterators.
	 */
	P_Scope scopes;

	/**
	 * Stack of scopes for function declaration;
	 */
	P_Scope* func_scope;

	/**
	 * Current stack of scopes.
	 */
	P_Scope* current_scope;
};



/**
 * ================================================
 * \brief Main parser structure
 * ================================================
 */
extern P_Struct* pstruct;



/**
 * \ingroup parser
 *
 * \brief System parsing.
 *
 * A Minibex system (AMPL-like syntax).
 */
class P_StructSystem : public P_Struct {
public:

	/**
	 * Build the parser structure for a system
	 *
	 * \param sys - the (uninitialized) system to be built
	 */
	P_StructSystem(System& sys, int simpl_level);

	virtual void begin();

	/**
	 * End parsing of a Minibex system.
	 */
	virtual void end();

	/* system to be built */
	System& system;

	/* simplification level */
	int simpl_level;
};

/**
 * \ingroup parser
 *
 * \brief Function parsing.
 *
 * Used when a single function is loaded
 * either from a file or using a string in C++ directly
 * e.g., Function f("x","y","x+y");
 */
class P_StructFunction : public P_Struct {
public:
	/**
	 * Build the parser structure for a function
	 *
	 * \param f - the (uninitialized) function to be built
	 */
	P_StructFunction(Function& f);

	virtual void begin();

	/**
	 * End parsing of a Minibex function.
	 */
	virtual void end();

	/*
	 * Function to be built
	 */
	Function& function;
};

/**
 * \ingroup parser
 *
 * \brief Choco constraint expression parsing.
 */
class P_StructChoco : public P_Struct {
public:
	/**
	 * Build the parser structure for a Choco constraint
	 *
	 * \param sys - the (uninitialized) system to be built
	 */
	P_StructChoco(System& sys, int simpl_level);

	/**
	 * Initialize parsing of CHOCO input constraint.
	 */
	virtual void begin();

	/**
	 * End parsing of CHOCO constraint.
	 */
	virtual void end();

	/**
	 * Constraint to be built.
	 *
	 * Note: when a stand-alone constraint is read by CHOCO
	 * the field system->nb_var must be set *before* calling the parser
	 */
	System& system;

	/* simplification level */
	int simpl_level;
};

} // end namespace parser
} // end namespace ibex

#endif // __IBEX_PARSER_RESULT_H__
