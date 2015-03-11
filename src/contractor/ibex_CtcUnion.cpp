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
		Ctc(c1.nb_var),	list(Array<Ctc>(c1,c2)) {
	assert(check_nb_var_ctc_list(list));
}

CtcUnion::CtcUnion(Ctc& c1, Ctc& c2, Ctc& c3) :
		Ctc(c1.nb_var),	list(Array<Ctc>(c1,c2,c3)) {
	assert(check_nb_var_ctc_list(list));
}


CtcUnion::CtcUnion(Ctc& c1, Ctc& c2, Ctc& c3, Ctc& c4) :
		Ctc(c1.nb_var),	list(Array<Ctc>(c1,c2,c3,c4)) {
	assert(check_nb_var_ctc_list(list));
}

CtcUnion::CtcUnion(Ctc& c1, Ctc& c2, Ctc& c3, Ctc& c4, Ctc& c5) :
		Ctc(c1.nb_var),	list(Array<Ctc>(c1,c2,c3,c4,c5)) {
	assert(check_nb_var_ctc_list(list));
}

CtcUnion::CtcUnion(Ctc& c1, Ctc& c2, Ctc& c3, Ctc& c4, Ctc& c5, Ctc& c6) :
		Ctc(c1.nb_var),	list(Array<Ctc>(c1,c2,c3,c4,c5,c6)) {
	assert(check_nb_var_ctc_list(list));
}

CtcUnion::CtcUnion(Ctc& c1, Ctc& c2, Ctc& c3, Ctc& c4, Ctc& c5, Ctc& c6, Ctc& c7) :
		Ctc(c1.nb_var),	list(Array<Ctc>(c1,c2,c3,c4,c5,c6,c7)){
	assert(check_nb_var_ctc_list(list));

}

CtcUnion::CtcUnion(Ctc& c1, Ctc& c2, Ctc& c3, Ctc& c4, Ctc& c5, Ctc& c6, Ctc& c7, Ctc& c8) :
		Ctc(c1.nb_var),	list(Array<Ctc>(c1,c2,c3,c4,c5,c6,c7,c8)){
	assert(check_nb_var_ctc_list(list));

}

CtcUnion::CtcUnion(Ctc& c1, Ctc& c2, Ctc& c3, Ctc& c4, Ctc& c5, Ctc& c6, Ctc& c7, Ctc& c8, Ctc& c9) :
		Ctc(c1.nb_var),	list(Array<Ctc>(c1,c2,c3,c4,c5,c6,c7,c8,c9)){
	assert(check_nb_var_ctc_list(list));

}

CtcUnion::CtcUnion(Ctc& c1, Ctc& c2, Ctc& c3, Ctc& c4, Ctc& c5, Ctc& c6, Ctc& c7, Ctc& c8, Ctc& c9, Ctc& c10) :
		Ctc(c1.nb_var),	list(Array<Ctc>(c1,c2,c3,c4,c5,c6,c7,c8,c9,c10)){
	assert(check_nb_var_ctc_list(list));

}

CtcUnion::CtcUnion(Ctc& c1, Ctc& c2, Ctc& c3, Ctc& c4, Ctc& c5, Ctc& c6, Ctc& c7, Ctc& c8, Ctc& c9, Ctc& c10, Ctc& c11) :
		Ctc(c1.nb_var),	list(Array<Ctc>(c1,c2,c3,c4,c5,c6,c7,c8,c9,c10,c11)){
	assert(check_nb_var_ctc_list(list));

}

CtcUnion::CtcUnion(Ctc& c1, Ctc& c2, Ctc& c3, Ctc& c4, Ctc& c5, Ctc& c6, Ctc& c7, Ctc& c8, Ctc& c9, Ctc& c10, Ctc& c11, Ctc& c12) :
		Ctc(c1.nb_var),	list(Array<Ctc>(c1,c2,c3,c4,c5,c6,c7,c8,c9,c10,c11,c12)){
	assert(check_nb_var_ctc_list(list));

}

CtcUnion::CtcUnion(Ctc& c1, Ctc& c2, Ctc& c3, Ctc& c4, Ctc& c5, Ctc& c6, Ctc& c7, Ctc& c8, Ctc& c9, Ctc& c10, Ctc& c11, Ctc& c12, Ctc& c13) :
		Ctc(c1.nb_var),	list(Array<Ctc>(c1,c2,c3,c4,c5,c6,c7,c8,c9,c10,c11,c12,c13)){
	assert(check_nb_var_ctc_list(list));

}

CtcUnion::CtcUnion(Ctc& c1, Ctc& c2, Ctc& c3, Ctc& c4, Ctc& c5, Ctc& c6, Ctc& c7, Ctc& c8, Ctc& c9, Ctc& c10, Ctc& c11, Ctc& c12, Ctc& c13, Ctc& c14) :
		Ctc(c1.nb_var),	list(Array<Ctc>(c1,c2,c3,c4,c5,c6,c7,c8,c9,c10,c11,c12,c13,c14)){
	assert(check_nb_var_ctc_list(list));

}

CtcUnion::CtcUnion(Ctc& c1, Ctc& c2, Ctc& c3, Ctc& c4, Ctc& c5, Ctc& c6, Ctc& c7, Ctc& c8, Ctc& c9, Ctc& c10, Ctc& c11, Ctc& c12, Ctc& c13, Ctc& c14, Ctc& c15) :
		Ctc(c1.nb_var),	list(Array<Ctc>(c1,c2,c3,c4,c5,c6,c7,c8,c9,c10,c11,c12,c13,c14,c15)){
	assert(check_nb_var_ctc_list(list));

}

CtcUnion::CtcUnion(Ctc& c1, Ctc& c2, Ctc& c3, Ctc& c4, Ctc& c5, Ctc& c6, Ctc& c7, Ctc& c8, Ctc& c9, Ctc& c10, Ctc& c11, Ctc& c12, Ctc& c13, Ctc& c14, Ctc& c15, Ctc& c16) :
		Ctc(c1.nb_var),	list(Array<Ctc>(c1,c2,c3,c4,c5,c6,c7,c8,c9,c10,c11,c12,c13,c14,c15,c16)){
	assert(check_nb_var_ctc_list(list));

}

CtcUnion::CtcUnion(Ctc& c1, Ctc& c2, Ctc& c3, Ctc& c4, Ctc& c5, Ctc& c6, Ctc& c7, Ctc& c8, Ctc& c9, Ctc& c10, Ctc& c11, Ctc& c12, Ctc& c13, Ctc& c14, Ctc& c15, Ctc& c16, Ctc& c17) :
		Ctc(c1.nb_var),	list(Array<Ctc>(c1,c2,c3,c4,c5,c6,c7,c8,c9,c10,c11,c12,c13,c14,c15,c16,c17)){
	assert(check_nb_var_ctc_list(list));

}

CtcUnion::CtcUnion(Ctc& c1, Ctc& c2, Ctc& c3, Ctc& c4, Ctc& c5, Ctc& c6, Ctc& c7, Ctc& c8, Ctc& c9, Ctc& c10, Ctc& c11, Ctc& c12, Ctc& c13, Ctc& c14, Ctc& c15, Ctc& c16, Ctc& c17, Ctc& c18) :
		Ctc(c1.nb_var),	list(Array<Ctc>(c1,c2,c3,c4,c5,c6,c7,c8,c9,c10,c11,c12,c13,c14,c15,c16,c17,c18)){
	assert(check_nb_var_ctc_list(list));

}

CtcUnion::CtcUnion(Ctc& c1, Ctc& c2, Ctc& c3, Ctc& c4, Ctc& c5, Ctc& c6, Ctc& c7, Ctc& c8, Ctc& c9, Ctc& c10, Ctc& c11, Ctc& c12, Ctc& c13, Ctc& c14, Ctc& c15, Ctc& c16, Ctc& c17, Ctc& c18, Ctc& c19) :
		Ctc(c1.nb_var),	list(Array<Ctc>(c1,c2,c3,c4,c5,c6,c7,c8,c9,c10,c11,c12,c13,c14,c15,c16,c17,c18,c19)){
	assert(check_nb_var_ctc_list(list));

}

CtcUnion::CtcUnion(Ctc& c1, Ctc& c2, Ctc& c3, Ctc& c4, Ctc& c5, Ctc& c6, Ctc& c7, Ctc& c8, Ctc& c9, Ctc& c10, Ctc& c11, Ctc& c12, Ctc& c13, Ctc& c14, Ctc& c15, Ctc& c16, Ctc& c17, Ctc& c18, Ctc& c19, Ctc& c20) :
		Ctc(c1.nb_var),	list(Array<Ctc>(c1,c2,c3,c4,c5,c6,c7,c8,c9,c10,c11,c12,c13,c14,c15,c16,c17,c18,c19,c20)){
	assert(check_nb_var_ctc_list(list));

}


void CtcUnion::contract(IntervalVector& box) {
	IntervalVector savebox(box);
	IntervalVector result(IntervalVector::empty(box.size()));
	BitSet flags(BitSet::empty(Ctc::NB_OUTPUT_FLAGS));
	BitSet impact(BitSet::all(nb_var)); // always set to "all" for the moment (to be improved later)

	for (int i=0; i<list.size(); i++) {
		if (i>0) box=savebox;
		try {
			flags.clear();
			list[i].contract(box,impact,flags);
			result |= box;
		}
		catch(EmptyBoxException&) {
		}

		if (flags[INACTIVE]) { set_flag(INACTIVE); break; }
	}

	box = result;
	if (box.is_empty()) throw EmptyBoxException();
} // end namespace ibex

}
