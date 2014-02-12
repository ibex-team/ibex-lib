//============================================================================
//                                  I B E X                                   
// File        : ibex_Entity.cpp
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Jun 26, 2012
// Last Update : Jun 26, 2012
//============================================================================

#include "ibex_Entity.h"

extern void ibexerror (const std::string& msg);

namespace ibex {
namespace parser {

void init_symbol_domain(const char* destname, Domain& dest, const Domain& src);


// VAR is a temporary value
Entity::Entity(const char* name, const Dim& dim, const Domain& d) :
				symbol(ExprSymbol::new_(name,dim)), domain(dim), type(VAR) {

	init_symbol_domain(name, domain, d);
}

Entity::Entity(const char* name, const Dim& dim, const Interval& x) :
				symbol(ExprSymbol::new_(name,dim)), domain(dim), type(VAR) {

	Domain tmp(Dim::scalar());
	tmp.i()=x;
	init_symbol_domain(name, domain, tmp);
}

} // end namespace parser
} // end namespace ibex

