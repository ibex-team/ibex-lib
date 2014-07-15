//============================================================================
//                                  I B E X                                   
// File        : ibex_Pdc.cpp
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Nov 27, 2012
// Last Update : Nov 27, 2012
//============================================================================

#include "ibex_Pdc.h"

namespace {
	int nb_var_pdc_list(const ibex::Array<ibex::Pdc>& l) {
		int i=0, n=-1;
		while ((n==-1)&&(i<l.size())) {
			n=l[i].nb_var;
		}
		return n;
	}
}


namespace ibex {

Pdc::Pdc() : nb_var(-1) { }

Pdc::Pdc(int n) : nb_var(n) { }

Pdc::Pdc(const Array<Pdc>& l) : nb_var(nb_var_pdc_list(l)) { }

Pdc::~Pdc() {
}

} // end namespace ibex
