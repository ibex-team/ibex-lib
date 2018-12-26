/* ============================================================================
 * I B E X - ibex_CtcFwdBwdSIC.cpp
 * ============================================================================
 * Copyright   : IMT Atlantique (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Antoine Marendet, Gilles Chabert
 * Created     : May 4, 2018
 * ---------------------------------------------------------------------------- */
 
#include "ibex_CtcFwdBwdSIC.h"

#include "ibex_SIConstraint.h"
#include "ibex_SIConstraintCache.h"
#include "ibex_SIPSystem.h"


#include "ibex_BitSet.h"
#include "ibex_Function.h"
#include "ibex_Vector.h"

using namespace std;

namespace ibex {

CtcFwdBwdSIC::CtcFwdBwdSIC(const SIConstraint& constraint, size_t sic_index, const SIPSystem& system) :
		Ctc(constraint.variable_count_), constraint_(constraint), backward_domain_(Interval::NEG_REALS), sic_index_(sic_index),
		system_(system) {
	init();
}

CtcFwdBwdSIC::~CtcFwdBwdSIC() {
	delete input;
	delete output;
}

void CtcFwdBwdSIC::init() {

	input = new BitSet(nb_var);
	output = new BitSet(nb_var);
	for(int i = 0; i < nb_var; ++i) {
		if(constraint_.function_->used_vars[i]) {
			input->add(i);
			output->add(i);
		}
	}
	/*for (BitSet::const_iterator it=constraint_.function_->used_vars.begin(); it!=constraint_.function_->used_vars.end(); it++) {
		if (it < nb_var) {
			output->add(it);
			input->add(it);
		}
	}*/
}

void CtcFwdBwdSIC::add_property(const IntervalVector& init_box, BoxProperties& map) {
    if(map[BxpNodeData::id] == nullptr) {
        map.add(new BxpNodeData(system_.getInitialNodeCaches()));
        ((BxpNodeData*)map[BxpNodeData::id])->init_box = init_box;
    }
}

void CtcFwdBwdSIC::contract(IntervalVector& box) {
    ibex_warning("CtcFwdBwdSIC: called with no context");
}

void CtcFwdBwdSIC::contract(IntervalVector &box, ContractContext& context) {
	IntervalVector full_box(constraint_.function_->nb_var());
	full_box.put(0, box);
	bool fixpoint = true;

	BxpNodeData* node_data = (BxpNodeData*) context.prop[BxpNodeData::id];
	if(node_data == nullptr) {
		ibex_error("CtcFwdBwdSIC: BxpNodeData must be set");
	}

	auto& cache = node_data->sic_constraints_caches[sic_index_];
	for (const auto& parameter_box : cache.parameter_caches_) {
		full_box.put(nb_var, parameter_box.parameter_box.mid());
		if (!constraint_.function_->backward(backward_domain_, full_box)) {
			fixpoint = false;
		}
		if (full_box.is_empty())
			break;
	}
	if (!full_box.is_empty()) {
		for (const auto& box : cache.best_blankenship_points_) {
			full_box.put(nb_var, box);
			if (!constraint_.function_->backward(backward_domain_, full_box)) {
				fixpoint = false;
			}
			if (full_box.is_empty())
				break;
			/*if (!constraint_.function_->backward(backward_domain_, full_box)) {
				fixpoint = false;
			}
			if (full_box.is_empty())
				break;*/
		}
	}

	box = full_box.subvector(0, nb_var - 1);
	//if(full_box.is_empty())
	//	box.set_empty();
	if (fixpoint) {
		context.output_flags.add(FIXPOINT);
		context.output_flags.add(INACTIVE);
	}

	if (box.is_empty()) {
		context.output_flags.add(FIXPOINT);
	}

}
} // end namespace ibex
