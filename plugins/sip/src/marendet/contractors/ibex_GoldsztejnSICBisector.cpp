/* ============================================================================
 * I B E X - ibex_GoldsztejnSICBisector.cpp
 * ============================================================================
 * Copyright   : IMT Atlantique (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Antoine Marendet, Gilles Chabert
 * Created     : May 4, 2018
 * ---------------------------------------------------------------------------- */
 
#include "ibex_GoldsztejnSICBisector.h"

#include "ibex_utils.h"
#include "ibex_SIConstraint.h"
#include "ibex_SIConstraintCache.h"

#include "ibex_Function.h"
#include "ibex_Interval.h"
#include "ibex_IntervalVector.h"
#include "ibex_LargestFirst.h"

#include <algorithm>
#include <iterator>
#include <vector>

using namespace std;

namespace ibex {

GoldsztejnSICBisector::GoldsztejnSICBisector(const SIPSystem& system, double ratio) :
		Ctc(system.ext_nb_var), system_(system), ratio_(ratio) {

}

GoldsztejnSICBisector::~GoldsztejnSICBisector() {

}

ParameterEvaluationsCache _createNewCache(const SIConstraint& constraint, const IntervalVector& box,
		const IntervalVector& parameter_box) {
	return ParameterEvaluationsCache(parameter_box, constraint.evaluate(box, parameter_box),
			constraint.gradient(box, parameter_box));
}

void GoldsztejnSICBisector::add_property(const IntervalVector& init_box, BoxProperties& map) {
    if(map[BxpNodeData::id] == nullptr) {
        map.add(new BxpNodeData(system_.getInitialNodeCaches()));
        ((BxpNodeData*)map[BxpNodeData::id])->init_box = init_box;
    }
}

void GoldsztejnSICBisector::contract(IntervalVector& box) {
    ibex_warning("GoldsztejnSICBisector: called with no context");

}
void GoldsztejnSICBisector::contract(IntervalVector& box, ContractContext& context) {
	LargestFirst bisector;

	BxpNodeData* node_data = (BxpNodeData*) context.prop[BxpNodeData::id];
	if(node_data == nullptr) {
		ibex_error("GoldsztejnSICBisector: BxpNodeData must be set");
	}
	for (int cst_index = 0; cst_index < node_data->sic_constraints_caches.size(); ++cst_index) {
		node_data->sic_constraints_caches[cst_index].update_cache(*system_.sic_constraints_[cst_index].function_,
				box, true);
		// Retrieve list of box and the associated constraint
		bool hasBisected = false;
		auto& cacheList = node_data->sic_constraints_caches[cst_index].parameter_caches_;
		std::sort(cacheList.begin(), cacheList.end(),
				[](const ParameterEvaluationsCache& p1, const ParameterEvaluationsCache& p2) {
					return p1.evaluation.ub() > p2.evaluation.ub();
				});
		const SIConstraint& constraint = system_.sic_constraints_[cst_index];
		const int bisection_limit = 50;
		int bisections = 0;

		int largest_index = -1;
		double largest_diam = 0;
		std::vector<IntervalVector> largest_list; 
		for (int i = 0; i < cacheList.size() /*&& bisections < bisection_limit*/; ++i) {
			if (!cacheList[i].parameter_box.is_bisectable())
				continue;
			auto bisectList = bisectAllDim(cacheList[i].parameter_box);
			if(largest_index < 0 || cacheList[i].parameter_box.max_diam() > largest_diam) {
				largest_diam = cacheList[i].parameter_box.max_diam();
				largest_index = i;
				largest_list = bisectList;
			}
			Interval z = constraint.evaluate(box, cacheList[i].parameter_box);
			if (std::any_of(bisectList.begin(), bisectList.end(), [&](const IntervalVector& iv) {
				Interval newz = constraint.evaluate(box, iv);
				return newz.diam()/z.diam() <= ratio_;
			})) {
				hasBisected = true;
				bisections++;
				cacheList[i] = _createNewCache(constraint, box, bisectList[0]);
				for (int j = 1; j < bisectList.size(); ++j)
					cacheList.emplace_back(_createNewCache(constraint, box, bisectList[j]));
			}
			/*auto pair = bisector.bisect(cacheList[i].parameter_box);
			 Interval z1 = constraint.evaluate(box, pair.first);
			 Interval z2 = constraint.evaluate(box, pair.second);
			 if (z1.diam() / z.diam() <= ratio_ || z2.diam() / z.diam() <= ratio_) {
			 hasBisected = true;
			 cacheList[i] = _createNewCache(system_.sic_constraints_[cst_index], box, pair.first);
			 cacheList.emplace_back(_createNewCache(system_.sic_constraints_[cst_index], box, pair.second));
			 }*/

		}

		/*if (!hasBisected || true) {
			auto it = cacheList.begin();
			while (it != cacheList.end() && !it->parameter_box.is_bisectable())
				++it;
			if (it != cacheList.end()) {
				auto bisectList = bisectAllDim(it->parameter_box);
				*it = _createNewCache(constraint, box, bisectList[0]);
				for (int j = 1; j < bisectList.size(); ++j)
					cacheList.emplace_back(_createNewCache(constraint, box, bisectList[j]));
			}
		}*/

		if(largest_index >= 0) {
			cacheList[largest_index] = _createNewCache(constraint, box, largest_list[0]);
			for (int j = 1; j < largest_list.size(); ++j) {
				cacheList.emplace_back(_createNewCache(constraint, box, largest_list[j]));
			}
		}
	}
}
} // end namespace ibex
