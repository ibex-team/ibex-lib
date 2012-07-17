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

namespace ibex {
namespace parser {

// VAR is a temporary value
Entity::Entity(const char* name, const Dim& dim, const Domain& d) :
				symbol(ExprSymbol::new_(name,dim)), domain(dim), type(VAR) {


	if (d.dim==dim) {
		domain=d;
	} else {
		// when a vector/matrix is initialized with a single interval
		assert(d.dim.is_scalar());
		load_domain(d.i());
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

