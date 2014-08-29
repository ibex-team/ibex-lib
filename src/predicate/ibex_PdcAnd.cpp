//============================================================================
//                                  I B E X                                   
// File        : ibex_PdcAnd.cpp
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : May 14, 2012
// Last Update : May 14, 2012
//============================================================================

#include "ibex_PdcAnd.h"

namespace ibex {

PdcAnd::PdcAnd(const Array<Pdc>& list) : Pdc(list), list(list) {
	assert(check_nb_var_pdc_list(list));
}

PdcAnd::PdcAnd(Pdc& c1, Pdc& c2) :
		Pdc(c1.nb_var),	list(Array<Pdc>(c1,c2)) {
	assert(check_nb_var_pdc_list(list));
}

PdcAnd::PdcAnd(Pdc& c1, Pdc& c2, Pdc& c3) :
		Pdc(c1.nb_var),	list(Array<Pdc>(c1,c2,c3)) {
	assert(check_nb_var_pdc_list(list));
}

PdcAnd::PdcAnd(Pdc& c1, Pdc& c2, Pdc& c3, Pdc& c4) :
		Pdc(c1.nb_var),	list(Array<Pdc>(c1,c2,c3,c4)) {
	assert(check_nb_var_pdc_list(list));
}

PdcAnd::PdcAnd(Pdc& c1, Pdc& c2, Pdc& c3, Pdc& c4, Pdc& c5) :
		Pdc(c1.nb_var),	list(Array<Pdc>(c1,c2,c3,c4,c5)) {
	assert(check_nb_var_pdc_list(list));
}

PdcAnd::PdcAnd(Pdc& c1, Pdc& c2, Pdc& c3, Pdc& c4, Pdc& c5, Pdc& c6) :
		Pdc(c1.nb_var),	list(Array<Pdc>(c1,c2,c3,c4,c5,c6)) {
	assert(check_nb_var_pdc_list(list));
}



BoolInterval PdcAnd::test(const IntervalVector& box) {
	assert(list.size()>0);
	BoolInterval res=list[0].test(box);
	for (int i=1; i<list.size(); i++) {
		res &= list[i].test(box);
	}
	return res;
}

} // end namespace ibex

