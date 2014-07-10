//============================================================================
//                                  I B E X                                   
// File        : ibex_CtcUnion.cpp
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : May 14, 2012
// Last Update : May 14, 2012
//============================================================================

#include "ibex_CtcUnion.h"

namespace ibex {

CtcUnion::CtcUnion(const Array<Ctc>& list) : Ctc(list), list(list) { }


CtcUnion::CtcUnion(Ctc& c1, Ctc& c2) :
		Ctc((c1.nb_var==-1)?c2.nb_var:c1.nb_var),
		list(Array<Ctc>(c1,c2)) {
}

CtcUnion::CtcUnion(Ctc& c1, Ctc& c2, Ctc& c3) :
		Ctc((c1.nb_var==-1)?((c2.nb_var==-1)?c3.nb_var:c2.nb_var):c1.nb_var),
		list(Array<Ctc>(c1,c2,c3)) {
}


CtcUnion::CtcUnion(Ctc& c1, Ctc& c2, Ctc& c3, Ctc& c4) :
		Ctc((c1.nb_var==-1)?((c2.nb_var==-1)?((c3.nb_var==-1)?c4.nb_var:c3.nb_var):c2.nb_var):c1.nb_var),
		list(Array<Ctc>(c1,c2,c3,c4)) {
}

CtcUnion::CtcUnion(Ctc& c1, Ctc& c2, Ctc& c3, Ctc& c4, Ctc& c5) :
		Ctc((c1.nb_var==-1)?((c2.nb_var==-1)?((c3.nb_var==-1)?((c4.nb_var==-1)?c5.nb_var:c4.nb_var):c3.nb_var):c2.nb_var):c1.nb_var),
		list(Array<Ctc>(c1,c2,c3,c4,c5)) {
}

CtcUnion::CtcUnion(Ctc& c1, Ctc& c2, Ctc& c3, Ctc& c4, Ctc& c5, Ctc& c6) :
		Ctc((c1.nb_var==-1)?((c2.nb_var==-1)?((c3.nb_var==-1)?((c4.nb_var==-1)?((c5.nb_var==-1)?c6.nb_var:c5.nb_var):c4.nb_var):c3.nb_var):c2.nb_var):c1.nb_var),
		list(Array<Ctc>(c1,c2,c3,c4,c5,c6)) {
}


void CtcUnion::contract(IntervalVector& box) {
	IntervalVector savebox(box);
	IntervalVector result(IntervalVector::empty(box.size()));

	for (int i=0; i<list.size(); i++) {
		if (i>0) box=savebox;
		try {
			list[i].contract(box);
			result |= box;
		}
		catch(EmptyBoxException&) {
		}
	}
	box = result;
	if (box.is_empty()) throw EmptyBoxException();
} // end namespace ibex

}
