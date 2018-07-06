//============================================================================
//                                  I B E X                                   
// File        : Composition of contractors
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Apr 25, 2012
// Last Update : Apr 25, 2012
//============================================================================

#include "ibex_CtcCompo.h"

namespace ibex {

/*! Default composition ratio. */
const double CtcCompo::default_ratio = 0.1;

void CtcCompo::init_impacts() {

//	impacts = new BoolMask[list.size()];
//
//	for (int i=0; i<list.size(); i++) {
//		assert(list[i].nb_var==nb_var);
//		impacts[i].resize(nb_var);
//		impacts[i].set_all();
//	}
}

CtcCompo::CtcCompo(const Array<Ctc>& list, bool incremental, double ratio) :
		Ctc(list), list(list), incremental(incremental), ratio(ratio) {
	assert(check_nb_var_ctc_list(list));

	init_impacts();
}

CtcCompo::CtcCompo(Ctc& c1, Ctc& c2, bool incremental, double ratio) :
		Ctc(c1.nb_var),list(Array<Ctc>(c1,c2)), incremental(incremental), ratio(ratio) {
	assert(check_nb_var_ctc_list(list));

	init_impacts();
}

CtcCompo::CtcCompo(Ctc& c1, Ctc& c2, Ctc& c3, bool incremental, double ratio) :
		Ctc(c1.nb_var),list(Array<Ctc>(c1,c2,c3)), incremental(incremental), ratio(ratio) {
	assert(check_nb_var_ctc_list(list));

	init_impacts();
}


CtcCompo::CtcCompo(Ctc& c1, Ctc& c2, Ctc& c3, Ctc& c4, bool incremental, double ratio) :
		Ctc(c1.nb_var),	list(Array<Ctc>(c1,c2,c3,c4)), incremental(incremental), ratio(ratio) {
	assert(check_nb_var_ctc_list(list));

	init_impacts();
}

CtcCompo::CtcCompo(Ctc& c1, Ctc& c2, Ctc& c3, Ctc& c4, Ctc& c5, bool incremental, double ratio) :
		Ctc(c1.nb_var),	list(Array<Ctc>(c1,c2,c3,c4,c5)), incremental(incremental), ratio(ratio) {
	assert(check_nb_var_ctc_list(list));

	init_impacts();
}

CtcCompo::CtcCompo(Ctc& c1, Ctc& c2, Ctc& c3, Ctc& c4, Ctc& c5, Ctc& c6, bool incremental, double ratio) :
		Ctc(c1.nb_var),	list(Array<Ctc>(c1,c2,c3,c4,c5,c6)), incremental(incremental), ratio(ratio) {
	assert(check_nb_var_ctc_list(list));

	init_impacts();
}

CtcCompo::CtcCompo(Ctc& c1, Ctc& c2, Ctc& c3, Ctc& c4, Ctc& c5, Ctc& c6, Ctc& c7, bool incremental, double ratio) :
		Ctc(c1.nb_var),	list(Array<Ctc>(c1,c2,c3,c4,c5,c6,c7)), incremental(incremental), ratio(ratio) {
	assert(check_nb_var_ctc_list(list));

	init_impacts();
}

CtcCompo::CtcCompo(Ctc& c1, Ctc& c2, Ctc& c3, Ctc& c4, Ctc& c5, Ctc& c6, Ctc& c7, Ctc& c8, bool incremental, double ratio) :
		Ctc(c1.nb_var),	list(Array<Ctc>(c1,c2,c3,c4,c5,c6,c7,c8)), incremental(incremental), ratio(ratio) {
	assert(check_nb_var_ctc_list(list));

	init_impacts();
}

CtcCompo::CtcCompo(Ctc& c1, Ctc& c2, Ctc& c3, Ctc& c4, Ctc& c5, Ctc& c6, Ctc& c7, Ctc& c8, Ctc& c9, bool incremental, double ratio) :
		Ctc(c1.nb_var),	list(Array<Ctc>(c1,c2,c3,c4,c5,c6,c7,c8,c9)), incremental(incremental), ratio(ratio) {
	assert(check_nb_var_ctc_list(list));

	init_impacts();
}

CtcCompo::CtcCompo(Ctc& c1, Ctc& c2, Ctc& c3, Ctc& c4, Ctc& c5, Ctc& c6, Ctc& c7, Ctc& c8, Ctc& c9, Ctc& c10, bool incremental, double ratio) :
		Ctc(c1.nb_var),	list(Array<Ctc>(c1,c2,c3,c4,c5,c6,c7,c8,c9,c10)), incremental(incremental), ratio(ratio) {
	assert(check_nb_var_ctc_list(list));

	init_impacts();
}

CtcCompo::CtcCompo(Ctc& c1, Ctc& c2, Ctc& c3, Ctc& c4, Ctc& c5, Ctc& c6, Ctc& c7, Ctc& c8, Ctc& c9, Ctc& c10, Ctc& c11, bool incremental, double ratio) :
		Ctc(c1.nb_var),	list(Array<Ctc>(c1,c2,c3,c4,c5,c6,c7,c8,c9,c10,c11)), incremental(incremental), ratio(ratio) {
	assert(check_nb_var_ctc_list(list));

	init_impacts();
}

CtcCompo::CtcCompo(Ctc& c1, Ctc& c2, Ctc& c3, Ctc& c4, Ctc& c5, Ctc& c6, Ctc& c7, Ctc& c8, Ctc& c9, Ctc& c10, Ctc& c11, Ctc& c12, bool incremental, double ratio) :
		Ctc(c1.nb_var),	list(Array<Ctc>(c1,c2,c3,c4,c5,c6,c7,c8,c9,c10,c11,c12)), incremental(incremental), ratio(ratio) {
	assert(check_nb_var_ctc_list(list));

	init_impacts();
}

CtcCompo::CtcCompo(Ctc& c1, Ctc& c2, Ctc& c3, Ctc& c4, Ctc& c5, Ctc& c6, Ctc& c7, Ctc& c8, Ctc& c9, Ctc& c10, Ctc& c11, Ctc& c12, Ctc& c13, bool incremental, double ratio) :
		Ctc(c1.nb_var),	list(Array<Ctc>(c1,c2,c3,c4,c5,c6,c7,c8,c9,c10,c11,c12,c13)), incremental(incremental), ratio(ratio) {
	assert(check_nb_var_ctc_list(list));

	init_impacts();
}

CtcCompo::CtcCompo(Ctc& c1, Ctc& c2, Ctc& c3, Ctc& c4, Ctc& c5, Ctc& c6, Ctc& c7, Ctc& c8, Ctc& c9, Ctc& c10, Ctc& c11, Ctc& c12, Ctc& c13, Ctc& c14, bool incremental, double ratio) :
		Ctc(c1.nb_var),	list(Array<Ctc>(c1,c2,c3,c4,c5,c6,c7,c8,c9,c10,c11,c12,c13,c14)), incremental(incremental), ratio(ratio) {
	assert(check_nb_var_ctc_list(list));

	init_impacts();
}

CtcCompo::CtcCompo(Ctc& c1, Ctc& c2, Ctc& c3, Ctc& c4, Ctc& c5, Ctc& c6, Ctc& c7, Ctc& c8, Ctc& c9, Ctc& c10, Ctc& c11, Ctc& c12, Ctc& c13, Ctc& c14, Ctc& c15, bool incremental, double ratio) :
		Ctc(c1.nb_var),	list(Array<Ctc>(c1,c2,c3,c4,c5,c6,c7,c8,c9,c10,c11,c12,c13,c14,c15)), incremental(incremental), ratio(ratio) {
	assert(check_nb_var_ctc_list(list));

	init_impacts();
}

CtcCompo::CtcCompo(Ctc& c1, Ctc& c2, Ctc& c3, Ctc& c4, Ctc& c5, Ctc& c6, Ctc& c7, Ctc& c8, Ctc& c9, Ctc& c10, Ctc& c11, Ctc& c12, Ctc& c13, Ctc& c14, Ctc& c15, Ctc& c16, bool incremental, double ratio) :
		Ctc(c1.nb_var),	list(Array<Ctc>(c1,c2,c3,c4,c5,c6,c7,c8,c9,c10,c11,c12,c13,c14,c15,c16)), incremental(incremental), ratio(ratio) {
	assert(check_nb_var_ctc_list(list));

	init_impacts();
}

CtcCompo::CtcCompo(Ctc& c1, Ctc& c2, Ctc& c3, Ctc& c4, Ctc& c5, Ctc& c6, Ctc& c7, Ctc& c8, Ctc& c9, Ctc& c10, Ctc& c11, Ctc& c12, Ctc& c13, Ctc& c14, Ctc& c15, Ctc& c16, Ctc& c17, bool incremental, double ratio) :
		Ctc(c1.nb_var),	list(Array<Ctc>(c1,c2,c3,c4,c5,c6,c7,c8,c9,c10,c11,c12,c13,c14,c15,c16,c17)), incremental(incremental), ratio(ratio) {
	assert(check_nb_var_ctc_list(list));

	init_impacts();
}

CtcCompo::CtcCompo(Ctc& c1, Ctc& c2, Ctc& c3, Ctc& c4, Ctc& c5, Ctc& c6, Ctc& c7, Ctc& c8, Ctc& c9, Ctc& c10, Ctc& c11, Ctc& c12, Ctc& c13, Ctc& c14, Ctc& c15, Ctc& c16, Ctc& c17, Ctc& c18, bool incremental, double ratio) :
		Ctc(c1.nb_var),	list(Array<Ctc>(c1,c2,c3,c4,c5,c6,c7,c8,c9,c10,c11,c12,c13,c14,c15,c16,c17,c18)), incremental(incremental), ratio(ratio) {
	assert(check_nb_var_ctc_list(list));

	init_impacts();
}

CtcCompo::CtcCompo(Ctc& c1, Ctc& c2, Ctc& c3, Ctc& c4, Ctc& c5, Ctc& c6, Ctc& c7, Ctc& c8, Ctc& c9, Ctc& c10, Ctc& c11, Ctc& c12, Ctc& c13, Ctc& c14, Ctc& c15, Ctc& c16, Ctc& c17, Ctc& c18, Ctc& c19, bool incremental, double ratio) :
		Ctc(c1.nb_var),	list(Array<Ctc>(c1,c2,c3,c4,c5,c6,c7,c8,c9,c10,c11,c12,c13,c14,c15,c16,c17,c18,c19)), incremental(incremental), ratio(ratio) {
	assert(check_nb_var_ctc_list(list));

	init_impacts();
}

CtcCompo::CtcCompo(Ctc& c1, Ctc& c2, Ctc& c3, Ctc& c4, Ctc& c5, Ctc& c6, Ctc& c7, Ctc& c8, Ctc& c9, Ctc& c10, Ctc& c11, Ctc& c12, Ctc& c13, Ctc& c14, Ctc& c15, Ctc& c16, Ctc& c17, Ctc& c18, Ctc& c19, Ctc& c20, bool incremental, double ratio) :
		Ctc(c1.nb_var),	list(Array<Ctc>(c1,c2,c3,c4,c5,c6,c7,c8,c9,c10,c11,c12,c13,c14,c15,c16,c17,c18,c19,c20)), incremental(incremental), ratio(ratio) {
	assert(check_nb_var_ctc_list(list));

	init_impacts();
}

CtcCompo::~CtcCompo() {
//	delete[] impacts;
}

void CtcCompo::add_property(BoxProperties& map) {
	for (int i=0; i<list.size(); i++)
		list[i].add_property(map);
}

void CtcCompo::contract(IntervalVector& box) {
	CtcContext context;
	contract(box,context);
}

void CtcCompo::contract(IntervalVector& box, CtcContext& context) {

	// TODO: wrong algorithm here
//	if (incremental) {
//		for (int i=0; i<list.size(); i++) {
//			IntervalVector old_box(box);
//			impacts[i] |= impact;
//
//			list[i].contract(box);
//
//			for (int j=0; j<nb_var; j++) {
//				if (old_box[j].rel_distance(box[j])>ratio)
//					for (int i2=0; i2<list.size(); i2++)
//						if (i2!=i) impacts[i2].set(j);
//			}
//		}
//		return;
//	}
	bool inactive= true;
	BitSet flags(BitSet::empty(CtcContext::NB_OUTPUT_FLAGS));

	CtcContext sub_context;
	sub_context.set_output_flags(&flags);
	if (context.data())
		sub_context.set_properties(context.data(), true);

	for (int i=0; i<list.size(); i++) {

		if (inactive) {
			flags.clear();
			list[i].contract(box,sub_context);
			if (!flags[CtcContext::INACTIVE]) {
				inactive=false;
				sub_context.set_output_flags(NULL);
			}
		} else {
			list[i].contract(box,sub_context);
		}

		if (box.is_empty()) {
			context.set_flag(CtcContext::FIXPOINT);
			return;
		}
	}

	if (inactive) context.set_flag(CtcContext::INACTIVE);
}

} // end namespace ibex
