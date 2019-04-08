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
#include "ibex_CtcFwdBwd.h"

namespace ibex {

CtcUnion::CtcUnion(const Array<Ctc>& list) : Ctc(list), list(list), own_sys(NULL) {

}

CtcUnion::CtcUnion(const System& sys) : Ctc(sys.nb_var), list(), own_sys(new NormalizedSystem(sys)) {

	list.resize(own_sys->nb_ctr);

	// ====== build the reversed inequalities g_i(x)>0 ===============
	for (int i=0; i<own_sys->nb_ctr; i++) {
		list.set_ref(i, *new CtcFwdBwd(own_sys->ctrs[i].f,GT));
	}
}

CtcUnion::CtcUnion(Ctc& c1, Ctc& c2) :
		Ctc(c1.nb_var),	list(Array<Ctc>(c1,c2)), own_sys(NULL) {
	assert(check_nb_var_ctc_list(list));
}

CtcUnion::CtcUnion(Ctc& c1, Ctc& c2, Ctc& c3) :
		Ctc(c1.nb_var),	list(Array<Ctc>(c1,c2,c3)), own_sys(NULL) {
	assert(check_nb_var_ctc_list(list));
}


CtcUnion::CtcUnion(Ctc& c1, Ctc& c2, Ctc& c3, Ctc& c4) :
		Ctc(c1.nb_var),	list(Array<Ctc>(c1,c2,c3,c4)), own_sys(NULL) {
	assert(check_nb_var_ctc_list(list));
}

CtcUnion::CtcUnion(Ctc& c1, Ctc& c2, Ctc& c3, Ctc& c4, Ctc& c5) :
		Ctc(c1.nb_var),	list(Array<Ctc>(c1,c2,c3,c4,c5)), own_sys(NULL) {
	assert(check_nb_var_ctc_list(list));
}

CtcUnion::CtcUnion(Ctc& c1, Ctc& c2, Ctc& c3, Ctc& c4, Ctc& c5, Ctc& c6) :
		Ctc(c1.nb_var),	list(Array<Ctc>(c1,c2,c3,c4,c5,c6)), own_sys(NULL) {
	assert(check_nb_var_ctc_list(list));
}

CtcUnion::CtcUnion(Ctc& c1, Ctc& c2, Ctc& c3, Ctc& c4, Ctc& c5, Ctc& c6, Ctc& c7) :
		Ctc(c1.nb_var),	list(Array<Ctc>(c1,c2,c3,c4,c5,c6,c7)), own_sys(NULL) {
	assert(check_nb_var_ctc_list(list));
}

CtcUnion::CtcUnion(Ctc& c1, Ctc& c2, Ctc& c3, Ctc& c4, Ctc& c5, Ctc& c6, Ctc& c7, Ctc& c8) :
		Ctc(c1.nb_var),	list(Array<Ctc>(c1,c2,c3,c4,c5,c6,c7,c8)), own_sys(NULL) {
	assert(check_nb_var_ctc_list(list));
}

CtcUnion::CtcUnion(Ctc& c1, Ctc& c2, Ctc& c3, Ctc& c4, Ctc& c5, Ctc& c6, Ctc& c7, Ctc& c8, Ctc& c9) :
		Ctc(c1.nb_var),	list(Array<Ctc>(c1,c2,c3,c4,c5,c6,c7,c8,c9)), own_sys(NULL) {
	assert(check_nb_var_ctc_list(list));
}

CtcUnion::CtcUnion(Ctc& c1, Ctc& c2, Ctc& c3, Ctc& c4, Ctc& c5, Ctc& c6, Ctc& c7, Ctc& c8, Ctc& c9, Ctc& c10) :
		Ctc(c1.nb_var),	list(Array<Ctc>(c1,c2,c3,c4,c5,c6,c7,c8,c9,c10)), own_sys(NULL) {
	assert(check_nb_var_ctc_list(list));
}

CtcUnion::CtcUnion(Ctc& c1, Ctc& c2, Ctc& c3, Ctc& c4, Ctc& c5, Ctc& c6, Ctc& c7, Ctc& c8, Ctc& c9, Ctc& c10, Ctc& c11) :
		Ctc(c1.nb_var),	list(Array<Ctc>(c1,c2,c3,c4,c5,c6,c7,c8,c9,c10,c11)), own_sys(NULL) {
	assert(check_nb_var_ctc_list(list));
}

CtcUnion::CtcUnion(Ctc& c1, Ctc& c2, Ctc& c3, Ctc& c4, Ctc& c5, Ctc& c6, Ctc& c7, Ctc& c8, Ctc& c9, Ctc& c10, Ctc& c11, Ctc& c12) :
		Ctc(c1.nb_var),	list(Array<Ctc>(c1,c2,c3,c4,c5,c6,c7,c8,c9,c10,c11,c12)), own_sys(NULL) {
	assert(check_nb_var_ctc_list(list));
}

CtcUnion::CtcUnion(Ctc& c1, Ctc& c2, Ctc& c3, Ctc& c4, Ctc& c5, Ctc& c6, Ctc& c7, Ctc& c8, Ctc& c9, Ctc& c10, Ctc& c11, Ctc& c12, Ctc& c13) :
		Ctc(c1.nb_var),	list(Array<Ctc>(c1,c2,c3,c4,c5,c6,c7,c8,c9,c10,c11,c12,c13)), own_sys(NULL) {
	assert(check_nb_var_ctc_list(list));
}

CtcUnion::CtcUnion(Ctc& c1, Ctc& c2, Ctc& c3, Ctc& c4, Ctc& c5, Ctc& c6, Ctc& c7, Ctc& c8, Ctc& c9, Ctc& c10, Ctc& c11, Ctc& c12, Ctc& c13, Ctc& c14) :
		Ctc(c1.nb_var),	list(Array<Ctc>(c1,c2,c3,c4,c5,c6,c7,c8,c9,c10,c11,c12,c13,c14)), own_sys(NULL) {
	assert(check_nb_var_ctc_list(list));
}

CtcUnion::CtcUnion(Ctc& c1, Ctc& c2, Ctc& c3, Ctc& c4, Ctc& c5, Ctc& c6, Ctc& c7, Ctc& c8, Ctc& c9, Ctc& c10, Ctc& c11, Ctc& c12, Ctc& c13, Ctc& c14, Ctc& c15) :
		Ctc(c1.nb_var),	list(Array<Ctc>(c1,c2,c3,c4,c5,c6,c7,c8,c9,c10,c11,c12,c13,c14,c15)), own_sys(NULL) {
	assert(check_nb_var_ctc_list(list));
}

CtcUnion::CtcUnion(Ctc& c1, Ctc& c2, Ctc& c3, Ctc& c4, Ctc& c5, Ctc& c6, Ctc& c7, Ctc& c8, Ctc& c9, Ctc& c10, Ctc& c11, Ctc& c12, Ctc& c13, Ctc& c14, Ctc& c15, Ctc& c16) :
		Ctc(c1.nb_var),	list(Array<Ctc>(c1,c2,c3,c4,c5,c6,c7,c8,c9,c10,c11,c12,c13,c14,c15,c16)), own_sys(NULL) {
	assert(check_nb_var_ctc_list(list));
}

CtcUnion::CtcUnion(Ctc& c1, Ctc& c2, Ctc& c3, Ctc& c4, Ctc& c5, Ctc& c6, Ctc& c7, Ctc& c8, Ctc& c9, Ctc& c10, Ctc& c11, Ctc& c12, Ctc& c13, Ctc& c14, Ctc& c15, Ctc& c16, Ctc& c17) :
		Ctc(c1.nb_var),	list(Array<Ctc>(c1,c2,c3,c4,c5,c6,c7,c8,c9,c10,c11,c12,c13,c14,c15,c16,c17)), own_sys(NULL) {
	assert(check_nb_var_ctc_list(list));
}

CtcUnion::CtcUnion(Ctc& c1, Ctc& c2, Ctc& c3, Ctc& c4, Ctc& c5, Ctc& c6, Ctc& c7, Ctc& c8, Ctc& c9, Ctc& c10, Ctc& c11, Ctc& c12, Ctc& c13, Ctc& c14, Ctc& c15, Ctc& c16, Ctc& c17, Ctc& c18) :
		Ctc(c1.nb_var),	list(Array<Ctc>(c1,c2,c3,c4,c5,c6,c7,c8,c9,c10,c11,c12,c13,c14,c15,c16,c17,c18)), own_sys(NULL) {
	assert(check_nb_var_ctc_list(list));
}

CtcUnion::CtcUnion(Ctc& c1, Ctc& c2, Ctc& c3, Ctc& c4, Ctc& c5, Ctc& c6, Ctc& c7, Ctc& c8, Ctc& c9, Ctc& c10, Ctc& c11, Ctc& c12, Ctc& c13, Ctc& c14, Ctc& c15, Ctc& c16, Ctc& c17, Ctc& c18, Ctc& c19) :
		Ctc(c1.nb_var),	list(Array<Ctc>(c1,c2,c3,c4,c5,c6,c7,c8,c9,c10,c11,c12,c13,c14,c15,c16,c17,c18,c19)), own_sys(NULL) {
	assert(check_nb_var_ctc_list(list));
}

CtcUnion::CtcUnion(Ctc& c1, Ctc& c2, Ctc& c3, Ctc& c4, Ctc& c5, Ctc& c6, Ctc& c7, Ctc& c8, Ctc& c9, Ctc& c10, Ctc& c11, Ctc& c12, Ctc& c13, Ctc& c14, Ctc& c15, Ctc& c16, Ctc& c17, Ctc& c18, Ctc& c19, Ctc& c20) :
		Ctc(c1.nb_var),	list(Array<Ctc>(c1,c2,c3,c4,c5,c6,c7,c8,c9,c10,c11,c12,c13,c14,c15,c16,c17,c18,c19,c20)), own_sys(NULL) {
	assert(check_nb_var_ctc_list(list));
}

void CtcUnion::add_property(const IntervalVector& init_box, BoxProperties& map) {
	for (int i=0; i<list.size(); i++)
		list[i].add_property(init_box, map);
}

CtcUnion::~CtcUnion() {
	if (own_sys) delete own_sys;
}

void CtcUnion::contract(IntervalVector& box) {
	ContractContext context(box);
	contract(box,context);
}

void CtcUnion::contract(IntervalVector& box, ContractContext& context) {
	IntervalVector savebox(box);
	IntervalVector result(IntervalVector::empty(box.size()));

	for (int i=0; i<list.size(); i++) {
		if (i>0) box=savebox;

		ContractContext c_context(context);

		list[i].contract(box,c_context);

		result |= box;

		if (c_context.output_flags[INACTIVE]) {
			context.output_flags.add(INACTIVE);
			break;
		}
	}

	box = result;

	if (!context.output_flags[INACTIVE]) {
		context.prop.update(BoxEvent(box,BoxEvent::CONTRACT));
	}

} // end namespace ibex

}
