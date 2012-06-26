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
Entity::Entity(const char* name, const Dim& dim, const Domain& domain) :
				symbol(ExprSymbol::new_(name,dim)), domain(domain,false), type(VAR) {

}

// VAR is a temporary value
Entity::Entity(const char* name, const Dim& dim, const Interval& x) :
				symbol(ExprSymbol::new_(name,dim)), domain(dim), type(VAR) {

	switch(dim.type()) {
	case Dim::SCALAR :      domain.i()=x; break;
	case Dim::ROW_VECTOR:
	case Dim::COL_VECTOR:   domain.v().init(x); break;
	case Dim::MATRIX:       domain.m().init(x); break;
	case Dim::MATRIX_ARRAY: for (int i=0; i<domain.ma().size(); i++) domain.ma()[i].init(x); break;
	}

}

} // end namespace parser
} // end namespace ibex

