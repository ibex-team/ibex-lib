//============================================================================
//                                  I B E X                                   
// File        : Domains
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Dec 13, 2011
// Last Update : 
//============================================================================

#include "ibex_Domain.h"

namespace ibex {

std::ostream& operator<<(std::ostream& os, const Domain& d) {

	for (size_t v=0; v<d.doms. size(); v++) {
		const Dim& dim=d.symbol_dims[v];
		switch (dim.type()) {
		case Dim::SCALAR:       os << d.get(v);          break;
		case Dim::VECTOR:       os << d.vector(v);       break;
		case Dim::MATRIX:       os << d.matrix(v);       break;
		case Dim::MATRIX_ARRAY: os << d.matrix_array(v); break;
		}

		os << " - ";
	}
	return os;
}

} // namespace ibex
