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
#include "ibex_System.h"
#include "ibex_P_Scope.h"

#include <stack>

int ibexlex(void);

// note: do not confuse with ibex_error in tools/ibex_Exception.h
void ibexerror (const std::string& msg);

namespace ibex {

namespace parser {

/**
 * \ingroup parser
 *
 * Result of parsing.
 */
class P_Struct {
public:
	/**
	 * The Minibex source
	 */
	P_Source source;

	/*
	 * The stack of all scopes
	 */
	std::stack<P_Scope> scopes;

	/**
	 * Possible types of objects built
	 */
	typedef enum {
		FUNCTION, SYSTEM, CHOCO, PACKAGE
	} ResultType;

	/**
	 * Delete source and all scopes.
	 */
	virtual ~P_Struct();

	/**
	 * The type of result
	 */
	virtual ResultType type() const=0;

	/**
	 * Initialize parsing of a Minibex file
	 *
	 * Set locale, create new scope, etc.
	 */
	virtual void begin();

	/**
	 * End of parsing
	 *
	 * Generates the result.
	 */
	virtual void end();
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
	P_StructSystem(System& sys);

	virtual ResultType type() const;

	virtual void begin();

	/**
	 * End parsing of a Minibex system.
	 */
	virtual void end();

	/* system to be built */
	System& system;
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

	virtual ResultType type() const;

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
	P_StructChoco(System& sys);

	virtual ResultType type() const;

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
};


} // end namespace parser
} // end namespace ibex

#endif // __IBEX_PARSER_RESULT_H__
