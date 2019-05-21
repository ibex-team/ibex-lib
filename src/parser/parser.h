//============================================================================
//                                  I B E X                                   
// File        : parser.h
// Author      : Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Sep 9, 2012
// Last Update : May 9, 2019
//============================================================================

#include "ibex_P_Source.h"
#include "ibex_Scope.h"

int ibexlex(void);

// note: do not confuse with ibex_error in tools/ibex_Exception.h
void ibexerror (const std::string& msg);

namespace ibex {

namespace parser {

/**
 * Return the minibex source
 */
P_Source& source();

/*
 * Return a reference to the stack of all scopes
 */
std::stack<Scope>& scopes();

/**
 * Initialize parsing of a Minibex file
 *
 * Set locale, create new scope, etc.
 */
void begin();

/**
 * Initialize parsing of CHOCO input constraint.
 */
void begin_choco();

/**
 * End parsing of a Minibex system.
 */
void end_system();

/**
 * End parsing of CHOCO constraint.
 */
void end_choco();

/**
 * End parsing of a Minibex function.
 */
void end_function();

/**
 * Return a domain created as a "ball" midpoint +radius.
 */
Domain ball(const Domain& mid, double rad);

} // end namespace parser

} // end namespace ibex

