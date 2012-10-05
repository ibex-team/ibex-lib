//============================================================================
//                                  I B E X                                   
// File        : ibex_Entity.h
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : May 25, 2012
// Last Update : May 25, 2012
//============================================================================

#ifndef __IBEX_ENTITY_H__
#define __IBEX_ENTITY_H__

#include "ibex_Expr.h"
#include "ibex_Domain.h"

namespace ibex {

namespace parser {

/**
 * Entity (var, epr or syb)
 */
class Entity {
public:
	typedef enum { VAR, EPR, SYB } Type;

	Entity(const char* name, const Dim& dim, const Domain& domain);

	Entity(const char* name, const Dim& dim, const Interval& x);

	const ExprSymbol& symbol;
	Domain domain;

	Type type; // not specified at construction (see parser.yacc)
};

} // end namespace parser

} // end namespace ibex

#endif // __IBEX_ENTITY_H__
