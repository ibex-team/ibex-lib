//============================================================================
//                                  I B E X                                   
// File        : ibex_PdcOr.cpp
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : May 14, 2012
// Last Update : May 14, 2012
//============================================================================

#include "ibex_PdcOr.h"

namespace ibex {


PdcOr::PdcOr(const Array<Pdc>& list) : Pdc(list), list(list) {

}

PdcOr::PdcOr(Pdc& c1, Pdc& c2) :
		Pdc((c1.nb_var==-1)?c2.nb_var:c1.nb_var),
		list(Array<Pdc>(c1,c2)) {
}

PdcOr::PdcOr(Pdc& c1, Pdc& c2, Pdc& c3) :
		Pdc((c1.nb_var==-1)?((c2.nb_var==-1)?c3.nb_var:c2.nb_var):c1.nb_var),
		list(Array<Pdc>(c1,c2,c3)) {
}

PdcOr::PdcOr(Pdc& c1, Pdc& c2, Pdc& c3, Pdc& c4) :
		Pdc((c1.nb_var==-1)?((c2.nb_var==-1)?((c3.nb_var==-1)?c4.nb_var:c3.nb_var):c2.nb_var):c1.nb_var),
		list(Array<Pdc>(c1,c2,c3,c4)) {
}

PdcOr::PdcOr(Pdc& c1, Pdc& c2, Pdc& c3, Pdc& c4, Pdc& c5) :
		Pdc((c1.nb_var==-1)?((c2.nb_var==-1)?((c3.nb_var==-1)?((c4.nb_var==-1)?c5.nb_var:c4.nb_var):c3.nb_var):c2.nb_var):c1.nb_var),
		list(Array<Pdc>(c1,c2,c3,c4,c5)) {
}

PdcOr::PdcOr(Pdc& c1, Pdc& c2, Pdc& c3, Pdc& c4, Pdc& c5, Pdc& c6) :
		Pdc((c1.nb_var==-1)?((c2.nb_var==-1)?((c3.nb_var==-1)?((c4.nb_var==-1)?((c5.nb_var==-1)?c6.nb_var:c5.nb_var):c4.nb_var):c3.nb_var):c2.nb_var):c1.nb_var),
		list(Array<Pdc>(c1,c2,c3,c4,c5,c6)) {
}

BoolInterval PdcOr::test(const IntervalVector& box) {
	assert(list.size()>0);
	BoolInterval res=list[0].test(box);
	for (int i=1; i<list.size(); i++) {
		res |= list[i].test(box);
	}
	return res;
}

} // end namespace ibex
