//============================================================================
//                                  I B E X                                   
// File        : ibex_GoldsztejnSICBisector.cpp
// Author      : Antoine Marendet, Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : May 4, 2018
// Last Update : May 4, 2018
//============================================================================

#include "ibex_GoldsztejnSICBisector.h"

#include "main/ibex_utils.h"
#include "system/ibex_SIConstraint.h"
#include "system/ibex_SIConstraintCache.h"

#include "ibex_Cell.h"
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
		CellCtc(system.ext_nb_var), system_(system), ratio_(ratio) {

}

GoldsztejnSICBisector::~GoldsztejnSICBisector() {

}

ParameterEvaluationsCache _createNewCache(const SIConstraint& constraint, const IntervalVector& box,
		const IntervalVector& parameter_box) {
	return ParameterEvaluationsCache(parameter_box, constraint.evaluate(box, parameter_box),
			constraint.gradient(box, parameter_box));
}

void GoldsztejnSICBisector::contractCell(Cell& cell) {
	LargestFirst bisector;
	NodeData& node_data = cell.get<NodeData>();
	for (int cst_index = 0; cst_index < node_data.sic_constraints_caches.size(); ++cst_index) {
		node_data.sic_constraints_caches[cst_index].update_cache(*system_.sic_constraints_[cst_index].function_,
				cell.box, true);
		// Retrieve list of box and the associated constraint
		bool hasBisected = false;
		auto& cacheList = node_data.sic_constraints_caches[cst_index].parameter_caches_;
		std::sort(cacheList.begin(), cacheList.end(),
				[](const ParameterEvaluationsCache& p1, const ParameterEvaluationsCache& p2) {
					return p1.evaluation.ub() > p2.evaluation.ub();
				});
		const SIConstraint& constraint = system_.sic_constraints_[cst_index];
		for (int i = 0; i < cacheList.size(); ++i) {
			if (!cacheList[i].parameter_box.is_bisectable())
				continue;
			auto bisectList = bisectAllDim(cacheList[i].parameter_box);
			Interval z = constraint.evaluate(cell.box, cacheList[i].parameter_box);
			if (std::any_of(bisectList.begin(), bisectList.end(), [&](const IntervalVector& iv) {
				Interval newz = constraint.evaluate(cell.box, iv);
				return newz.diam()/z.diam() <= ratio_;
			})) {
				hasBisected = true;
				cacheList[i] = _createNewCache(constraint, cell.box, bisectList[0]);
				for (int j = 1; j < bisectList.size(); ++j)
					cacheList.emplace_back(_createNewCache(constraint, cell.box, bisectList[j]));
			}
			/*auto pair = bisector.bisect(cacheList[i].parameter_box);
			 Interval z1 = constraint.evaluate(cell.box, pair.first);
			 Interval z2 = constraint.evaluate(cell.box, pair.second);
			 if (z1.diam() / z.diam() <= ratio_ || z2.diam() / z.diam() <= ratio_) {
			 hasBisected = true;
			 cacheList[i] = _createNewCache(system_.sic_constraints_[cst_index], cell.box, pair.first);
			 cacheList.emplace_back(_createNewCache(system_.sic_constraints_[cst_index], cell.box, pair.second));
			 }*/

		}

		if (!hasBisected || true) {
			auto it = cacheList.begin();
			while (it != cacheList.end() && !it->parameter_box.is_bisectable())
				++it;
			if (it != cacheList.end()) {
				/*auto pair = bisector.bisect(it->parameter_box);
				 *it = _createNewCache(system_.sic_constraints_[cst_index], cell.box, pair.first);
				 cacheList.emplace_back(_createNewCache(system_.sic_constraints_[cst_index], cell.box, pair.second));*/
				auto bisectList = bisectAllDim(it->parameter_box);
				*it = _createNewCache(constraint, cell.box, bisectList[0]);
				for (int j = 1; j < bisectList.size(); ++j)
					cacheList.emplace_back(_createNewCache(constraint, cell.box, bisectList[j]));
			}
		}
	}
}
} // end namespace ibex
