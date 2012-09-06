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
#include <sstream>

extern void ibexerror (const std::string& msg);

namespace ibex {
namespace parser {

// VAR is a temporary value
Entity::Entity(const char* name, const Dim& dim, const Domain& d) :
				symbol(ExprSymbol::new_(name,dim)), domain(dim), type(VAR) {


	if (d.dim==dim) {
		domain=d;
	} else {
		// when a vector/matrix is initialized with a single interval
		if (d.dim.is_scalar()) {
			load_domain(d.i());
		}
		else {
			stringstream s;
			s << "Variable \"" << name << "\"";

			if (dim.is_vector() && d.dim.is_vector()) {
				s << " is a column vector and is initialized with a row vector";
				s << " (you have probably used \",\" instead of \";\" in the constant vector)";
				ibexerror(s.str());
			}
			else {
				s << " is not initialized correctly (dimensions do not match)";
				ibexerror(s.str());
			}
		}
	}
}

Entity::Entity(const char* name, const Dim& dim, const Interval& x) :
				symbol(ExprSymbol::new_(name,dim)), domain(dim), type(VAR) {
	load_domain(x);
}

void Entity::load_domain(const Interval& x) {
	switch(domain.dim.type()) {
	case Dim::SCALAR :      domain.i()=x; break;
	case Dim::ROW_VECTOR:
	case Dim::COL_VECTOR:   domain.v().init(x); break;
	case Dim::MATRIX:       domain.m().init(x); break;
	case Dim::MATRIX_ARRAY: for (int i=0; i<domain.ma().size(); i++) domain.ma()[i].init(x); break;
	}
}

} // end namespace parser
} // end namespace ibex

