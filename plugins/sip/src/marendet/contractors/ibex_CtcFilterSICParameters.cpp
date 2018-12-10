/* ============================================================================
 * I B E X - ibex_CtcFilterSICParameters.cpp
 * ============================================================================
 * Copyright   : IMT Atlantique (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Antoine Marendet, Gilles Chabert
 * Created     : May 4, 2018
 * ---------------------------------------------------------------------------- */
 
#include "ibex_CtcFilterSICParameters.h"

#include "ibex_SIConstraintCache.h"

#include "ibex_Function.h"
#include "ibex_Interval.h"
#include "ibex_SICPaving.h"

#include <algorithm>
#include <iterator>
#include <vector>

namespace ibex {

CtcFilterSICParameters::CtcFilterSICParameters(const SIPSystem& system) :
		Ctc(system.ext_nb_var), system_(system) {

}

CtcFilterSICParameters::~CtcFilterSICParameters() {
}

void CtcFilterSICParameters::add_property(const IntervalVector& init_box, BoxProperties& map) {
    if(map[BxpNodeData::id] == nullptr) {
        map.add(new BxpNodeData(system_.getInitialNodeCaches()));
        ((BxpNodeData*)map[BxpNodeData::id])->init_box = init_box;
    }
}

void CtcFilterSICParameters::contract(IntervalVector& box) {
    ibex_warning("CtcFilterSICParameters: called with no context");

}
void CtcFilterSICParameters::contract(IntervalVector& box, ContractContext& context) {
	BxpNodeData* node_data = (BxpNodeData*) context.prop[BxpNodeData::id];
	if(node_data == nullptr) {
		ibex_error("CtcFilterSICParameters: BxpNodeData must be set");
	}
	for (int i = 0; i < system_.sic_constraints_.size(); ++i) {
		//node_data.sic_constraints_caches[i].update_cache(*system_.sic_constraints_[i].function_, box);
		//contractOneConstraint(i, node_data, box);
		simplify_paving(system_.sic_constraints_[i], node_data->sic_constraints_caches[i], box, true);
	}
}

/*void CtcFilterSICParameters::contractOneConstraint(size_t i, BxpNodeData& node_data, const IntervalVector& box) {
	auto& list = node_data.sic_constraints_caches[i].parameter_caches_;
	auto it = list.begin();
	IntervalVector paramBoxUnion = node_data.sic_constraints_caches[i].initial_box_;
	sort(list.begin(), list.end(), [](const ParameterEvaluationsCache& p1, const ParameterEvaluationsCache& p2) {
		return p1.evaluation.ub() > p2.evaluation.ub();
	});
	while (it != list.end()) {
		bool keepInVector = true;
		for (int paramCount = 0; paramCount < it->parameter_box.size(); ++paramCount) {
			int paramIndex = paramCount + system_.ext_nb_var;
			if (it->full_gradient[paramIndex].lb() > 0) {
				it->parameter_box[paramCount] = it->parameter_box[paramCount].ub();
				if (it->parameter_box[paramCount].ub() < paramBoxUnion[paramCount].ub()) {
					keepInVector = false;
				} else {
					// TODO Maybe update only at the end
					//_updateMemoryBox(*it);
				}
			} else if (it->full_gradient[paramIndex].ub() < 0) {
				it->parameter_box[paramCount] = it->parameter_box[paramCount].lb();
				if (it->parameter_box[paramCount].lb() > paramBoxUnion[paramCount].lb()) {
					keepInVector = false;
				} else {
					//_updateMemoryBox(*it);
				}
			}
		}
		if (!keepInVector) {
			it = list.erase(it);
		} else {
			//_updateMemoryBox(*it);
			++it;
		}
	}
	it = list.begin();
	while (it != list.end()) {
		if (system_.sic_constraints_[i].evaluate(box, it->parameter_box).ub() <= 0) {
			it = list.erase(it);
		} else {
			it++;
		}
	}
	sort(list.begin(), list.end(), [](const ParameterEvaluationsCache& p1, const ParameterEvaluationsCache& p2) {
		return p1.evaluation.ub() > p2.evaluation.ub();
	});
}*/
} // end namespace ibex
