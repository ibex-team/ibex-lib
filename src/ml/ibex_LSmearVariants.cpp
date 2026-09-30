//============================================================================
//                                  I B E X
// File        : ibex_LSmearVariants.cpp
// Author      : Ignacio Araya
// License     : See the LICENSE file
// Created     : 2026
//============================================================================

#include "ibex_LSmearVariants.h"
#include "ibex_NoBisectableVariableException.h"

namespace ibex {

BisectionPoint OptimLargestFirstFixed::choose_var(const Cell& cell) {
	const IntervalVector& box = cell.box;
	int var = -1;
	double l = 0.0;
	for (int i=0; i<box.size(); i++) {
		if (i==goal_var || nobisectable(box,i)) continue;
		double li = (uniform_prec() || !(prec(i)>0)) ? box[i].diam() : box[i].diam()/prec(i);
		if (var==-1 || li>l) { var = i; l = li; }
	}
	// as OptimLargestFirst: the objective, if much wider (same 1e10 limit)
	if (choose_obj && !nobisectable(box,goal_var) && l < box[goal_var].diam()
			&& box[goal_var].diam()/l < 1.e10)
		var = goal_var;
	if (var==-1) throw NoBisectableVariableException();
	return BisectionPoint(var, ratio, true);
}

BisectionPoint LSmearAvoidParent::choose_var(const Cell& cell) {
	avoid = cell.bisected_var;
	BisectionPoint bp = LSmear::choose_var(cell);
	avoid = -1;
	if (cell.bisected_var<0 || bp.var!=cell.bisected_var) return bp;

	// pick() already skipped it: the choice came from a fallback of LSmear's
	// (largest first on an infinite derivative, SmearSumRelative). Take the
	// widest other variable, relative to its precision.
	const IntervalVector& box = cell.box;
	int var = -1; double l = 0;
	for (int i=0; i<box.size(); i++) {
		if (i==cell.bisected_var || i==goal_var() || too_small(box,i) || !box[i].is_bisectable())
			continue;
		double li = prec(i)>0 ? box[i].diam()/prec(i) : box[i].diam();
		if (var==-1 || li>l) { var = i; l = li; }
	}
	return var==-1 ? bp : BisectionPoint(var, bp.rel_pos ? bp.pos : 0.5, true);
}

int LSmearAvoidParent::pick(const std::vector<double>& score) const {
	double best = 0.0;
	int var = -1;
	for (size_t j=0; j<score.size(); j++)
		if ((int) j!=avoid && score[j] > best) { best = score[j]; var = (int) j; }
	return var;
}

int LSmearGrasp::pick(const std::vector<double>& score) const {
	double best = 0.0;
	for (size_t j=0; j<score.size(); j++)
		if (score[j] > best) best = score[j];
	if (!(best > 0)) return -1;
	std::vector<int> rcl;
	for (size_t j=0; j<score.size(); j++)
		if (score[j] > 0 && score[j] >= alpha*best) rcl.push_back((int) j);
	std::uniform_int_distribution<size_t> u(0, rcl.size()-1);
	return rcl[u(gen)];
}

} // end namespace ibex
